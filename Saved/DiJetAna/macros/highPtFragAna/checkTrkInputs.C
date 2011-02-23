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

void checkTrkInputs(
    TString infrec="nt_djhp_HyUQ110v0_djcalo.root",
    TString infgen="nt_djhp_HyUQ110v0_djcalo_genp.root",
    TCut evtCut="cent<30")
{
  //TH1::SetDefaultSumw2();
  TChain * trec = new TChain("tjttrk");
  trec->Add(infrec);
  setupAlias(trec);
  TChain * tgen = new TChain("tjttrk");
  tgen->Add(infgen);
  cout << infrec << " cut " << TString(evtCut) << ": " << trec->GetEntries() << endl;
  cout << infgen << " cut " << TString(evtCut) << ": " << tgen->GetEntries() << endl;
   
  // Correction Histograms
  TFile * fTrkCorr = new TFile("djtrkhist_hydjetBassv2_djuq110.root");
  TString corrModule = "hitrkEffAnalyzer";
  TH2F * hrec = (TH2F*)fTrkCorr->Get(Form("%s/hrec",corrModule.Data()));
  TH1D * hrec_pt = (TH1D*)hrec->ProjectionY();
  TH2F * hsim = (TH2F*)fTrkCorr->Get(Form("%s/hsim",corrModule.Data()));
  TH1D * hsim_pt = (TH1D*)hsim->ProjectionY();
  
  // Frag Histograms
  //TH1D * hPPtRecRaw = new TH1D("hPPtRecRaw",";p_{T} (GeV/c); count;");
  TH1D * hPPtRecRaw = (TH1D*)hrec_pt->Clone("hPPtRecRaw");
  hPPtRecRaw->Reset();
  TH1D * hPPtGen = (TH1D*)hrec_pt->Clone("hPPtGen");
  hPPtGen->Reset();
  trec->Project("hPPtRecRaw","ppt","");
  tgen->Project("hPPtGen","ppt","");
  TH1D * hRecSimRat = (TH1D*)hrec_pt->Clone("hRecSimRat");
  hRecSimRat->Sumw2();
  hRecSimRat->Divide(hrec_pt,hsim_pt);
  TH1D * hPPtRat = (TH1D*)hrec_pt->Clone("hPPtRat");
  hPPtRat->Sumw2();
  hPPtRat->Divide(hPPtRecRaw,hPPtGen);

  hsim_pt->SetTitle(";p_{T} (GeV/c); count");
  hRecSimRat->SetTitle(";p_{T} (GeV/c); reco/gen ratio");
  hsim_pt->SetLineColor(kRed);
  hPPtGen->SetMarkerColor(kRed);
  hPPtGen->SetLineColor(kRed);
  hRecSimRat->SetLineColor(kRed);
  TCanvas *cRec = new TCanvas("cRec","Rec",500,900);
  cRec->Divide(1,2);
  cRec->cd(1);
  cRec->GetPad(1)->SetLogy();
  hsim_pt->Draw("hist");
  hPPtGen->Draw("sameE");
  hrec_pt->Draw("hist same");
  hPPtRecRaw->Draw("sameE");
  cRec->cd(2);
  hRecSimRat->Draw("hist");
  hPPtRat->Draw("sameE");

  // ====================
  TLegend *leg = new TLegend(0.61,0.78,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
}
