#include <iostream>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>
#include "TLegend.h"
#include "TPad.h"
#include "HisMath.C"
using namespace std;

#define PI 3.14159265358979

void setupAlias(TTree* t)
{   
   t->SetAlias("trkwt","(1-trkfak)*(1-trksec)/(trkeff*(1+trkmul))");
}

void plotJetSamples(
    TCut evtCut="")
{
  //TH1::SetDefaultSumw2();
  const Int_t nfile=5;
  Float_t ptHatMin[nfile]= { 30,50,80,110,170 };
  vector<TFile*> samples;
  vector<TChain*> vt;
  for (Int_t i=0; i<nfile; ++i) {
    TString fname=Form("nt_djhp_HyUQ%.0fv0_djcalo.root",ptHatMin[i]);
    vt.push_back(new TChain("tjttrk"));
    vt.back()->Add(fname);
    cout << fname << " cut " << TString(evtCut) << ": " << vt[i]->GetEntries() << endl;
  }

  vector<TH1D*> vhJEt;
  for (Int_t i=0; i<vt.size(); ++i) {
    vhJEt.push_back(new TH1D(Form("hJEt_f%d",i),";Jet p_{T} (GeV/c);count",100,0,400));
  }

  for (Int_t i=0; i<vt.size(); ++i) {
    vt[i]->Project(vhJEt[i]->GetName(),"jtpt",evtCut);
  }

  TCanvas *c2 = new TCanvas("c2","closure",500,500);
  c2->SetLogy();
  vhJEt[0]->SetAxisRange(0,250,"X");
  vhJEt[0]->SetAxisRange(3e-1,3e4,"Y");
  vhJEt[0]->Draw();
  for (Int_t i=1; i<vt.size(); ++i) {
    vhJEt[i]->Draw("same hist");
  }

  // ====================
  TLegend *leg = new TLegend(0.61,0.78,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  //leg->AddEntry(vhPPtGen[0],"#DeltaR(jet,trk)<0.5","");
  //leg->Draw();
}
