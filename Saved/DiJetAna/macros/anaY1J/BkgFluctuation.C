#include <iostream>
#include <vector>
#include <TH1D.h>
#include "TChain.h"
#include <TTree.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>
#include <TProfile.h>
#include <TText.h>
#include "TStyle.h"
#include "commonUtility.h"
#include "colorScheme.h"
using namespace std;

Float_t fracArea(Float_t r, Float_t dr=0.1, Float_t R=0.8)
{
  Float_t f = (2*r*dr+dr*dr)/R*R;
  return f;
}

void BkgFluctuation(TString infname = "../ntv6_datambv3rand_djcalo_c0to30_offset0.root",
                    TCut myCut = "cent<30", char *title = "",bool drawLegend = false,
                    bool drawSys = true
		    )
{
  gStyle->SetHistLineWidth(2);
  // ===========================================================
  // Get Input
  // ===========================================================
  TChain* t = new TChain("ntjt");
  t->Add(infname);
  t->AddFriend("tcone",infname);
  cout << infname << ": " << t->GetEntries() << endl;
  t->SetAlias("cptsub0Merge0","cpt[0][1]+cpt[0][2]-(cptbg[0][1]+cptbg[0][2])");
  t->SetAlias("cptsub0Merge1","cpt[0][3]-(cptbg[0][3])");
  t->SetAlias("cptsub0Merge2","cpt[0][4]+cpt[0][5]-(cptbg[0][4]+cptbg[0][5])");
  t->SetAlias("cptsub0Merge3","(Sum$(cpt[0])-cpt[0][0])-(Sum$(cptbg[0])-cptbg[0][0])");

  // ===========================================================
  // Analysis Setup
  // ===========================================================
  TH1::SetDefaultSumw2();
   const int nBin = 3;
   double bins[nBin+1] = {1.,4,8,160};  
  TCut evtCut = "nljet>100&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&jdphi>2./3*TMath::Pi()&&!maskEvt";
  TCut exclusion = "abs(nljetacorr)>0.8"; //&&abs(aljetacorr)>0.8";
  evtCut = evtCut&&exclusion;
  //Int_t colors[7] = {kBlue, kCyan-3, kYellow+1, kOrange+1, kGreen+2, kMagenta, kRed };
  Int_t colors[7] = {kYellow+2, kGreen+2,kRed };
  
  // ===========================================================
  // Book Histograms
  // ===========================================================
  vector<TH1D*> vhBgSub;
  for (Int_t i=0; i<nBin+1; ++i) {
    vhBgSub.push_back(new TH1D(Form("hBgSub_%d"),";(Sig Cone #Sigma p_{T}) - (Bkg Cone #Sigma p_{T}) (GeV/c);fraction;",50,-300,300));
  }

  Float_t numEvt = t->GetEntries(evtCut&&myCut);
  cout << "Total Sel evt: " << numEvt << endl;
  for (Int_t i=0; i<nBin+1; ++i) {
    TString var(Form("cptsub0Merge%i",i));
    cout << "Sel evt: " << t->GetEntries(evtCut&&myCut&&var) << endl;
    cout << "var: " << t->GetAlias(var) << endl;
    t->Project(vhBgSub[i]->GetName(),var,(evtCut&&myCut&&var)*"weight");
    Float_t mean = vhBgSub[i]->GetMean();
    Float_t rms = vhBgSub[i]->GetRMS();
    cout << "bgsub mean: " << mean << " rms: " << rms << endl;
    vhBgSub[i]->Scale(1./numEvt);
    cout << "Bin " << i << " rms/sqrt(250): " << rms/sqrt(250)
      << " r=0: " << rms/sqrt(250.)*sqrt(fracArea(0))
      << " r=0.8: " << rms/sqrt(250.)*sqrt(fracArea(0.8)) << endl;
  }

  TCanvas * c2 = new TCanvas("c2","c2",600,600);
  c2->SetLogy();
  vhBgSub[nBin]->SetAxisRange(1e-5,5,"Y");
  vhBgSub[nBin]->Draw("E");
  for (Int_t i=0; i<nBin; ++i) {
    vhBgSub[i]->SetLineColor(colors[i]);
    vhBgSub[i]->Draw("same hist");
  }
  vhBgSub[nBin]->Draw("Esame");

  TLegend *leg = new TLegend(0.19,0.70,0.53,0.92);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(vhBgSub[nBin],"CMS 0-30%","");
  leg->AddEntry(vhBgSub[nBin],"Random Cone (MB Sample)","");
  leg->AddEntry(vhBgSub[nBin],Form("> %.1f GeV/c",bins[0]),"pl");
  for (int i=0;i<nBin;++i) {
    if (i!=nBin-1){
      leg->AddEntry(vhBgSub[i],Form("%.1f - %.1f GeV/c",bins[i],bins[i+1]),"l");
    } else {
      leg->AddEntry(vhBgSub[i],Form("> %.1f GeV/c",bins[i]),"l");
    }
  }
  leg->Draw();
}

