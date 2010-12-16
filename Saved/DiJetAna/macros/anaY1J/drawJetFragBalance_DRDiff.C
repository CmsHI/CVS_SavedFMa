#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TLine.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
using namespace std;
#include "Saved/DiJetAna/macros/commonUtility.h"

void drawJetFragBalance_DRDiff(
    TString inFileName="jfhCorrEtaPtBin4RBin20v0_HCPR_J50U_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root",
    TString inFileNameHyPy="jfhCorrEtaPtBin4RBin20v0_Hydjet_J50U_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root",
    TString title = "test",
    Int_t drawMode=1,
    Int_t doLeg=1
    ) {
  TFile *f = new TFile(inFileName);

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");
  TH2D * hPtPNDRBgSub = (TH2D*)hPtPNDR->Clone("hPtPNDRBgSub");
  TH2D * hPtPADRBgSub = (TH2D*)hPtPADR->Clone("hPtPADRBgSub");
  hPtPNDRBgSub->Add(hPtPNDR,hPtPNDRBg,1,-1);
  hPtPADRBgSub->Add(hPtPADR,hPtPADRBg,1,-1);

  TFile *fhypy = new TFile(inFileNameHyPy);
  TH2D * hPtPNDRHyPy = (TH2D*) fhypy->Get("hPtPNDR");
  TH2D * hPtPADRHyPy = (TH2D*) fhypy->Get("hPtPADR");
  TH2D * hPtPNDRBgHyPy = (TH2D*) fhypy->Get("hPtPNDRBg");
  TH2D * hPtPADRBgHyPy = (TH2D*) fhypy->Get("hPtPADRBg");
  TH2D * hPtPNDRBgSubHyPy = (TH2D*)hPtPNDR->Clone("hPtPNDRBgSubHyPy");
  TH2D * hPtPADRBgSubHyPy = (TH2D*)hPtPADR->Clone("hPtPADRBgSubHyPy");
  hPtPNDRBgSubHyPy->Add(hPtPNDRHyPy,hPtPNDRBgHyPy,1,-1);
  hPtPADRBgSubHyPy->Add(hPtPADRHyPy,hPtPADRBgHyPy,1,-1);

  // Get Pt info
  Int_t numBinsPt=hPtPNDR->GetNbinsX();
  Int_t numBinsDR=hPtPNDR->GetNbinsY();
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  cout << "Pt bins: " << numBinsPt << endl;

  Double_t totPtBgSubNr=hPtPNDRBgSub->Integral();
  Double_t totPtBgSubAw=hPtPADRBgSub->Integral();

  Double_t totPtBgSubNrHyPy=hPtPNDRBgSubHyPy->Integral();
  Double_t totPtBgSubAwHyPy=hPtPADRBgSubHyPy->Integral();

  TH1D * hDRBgSubNr = (TH1D*)hPtPNDRBgSub->ProjectionY("hDRBgSubNr",1,1);
  TH1D * hDRBgSubAw = (TH1D*)hPtPADRBgSub->ProjectionY("hDRBgSubAw",1,1);
  TH1D * hDRBgSubNrHyPy = (TH1D*)hPtPNDRBgSubHyPy->ProjectionY("hDRBgSubNrHyPy",1,1);
  TH1D * hDRBgSubAwHyPy = (TH1D*)hPtPADRBgSubHyPy->ProjectionY("hDRBgSubAwHyPy",1,1);
  // Print
  cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(1),hPt->GetBinLowEdge(2)) << " SigSubBkg Integral - Nr: " << endl;
  cout << " Data - Nr: " << hDRBgSubNr->Integral() << " Aw: " << hDRBgSubAw->Integral() << endl;
  cout << " Pythia+Hydjet - Nr: " << hDRBgSubNrHyPy->Integral() << " Aw: " << hDRBgSubAwHyPy->Integral() << endl;

  if (drawMode==1) TCanvas * c6 = new TCanvas("c6","c6",500,500);
  hDRBgSubNr->Scale(1./totPtBgSubNr);
  hDRBgSubAw->Scale(1./totPtBgSubAw);
  hDRBgSubNrHyPy->Scale(1./totPtBgSubNrHyPy);
  hDRBgSubAwHyPy->Scale(1./totPtBgSubAwHyPy);
  hDRBgSubNr->SetMarkerStyle(kOpenSquare);
  hDRBgSubNrHyPy->SetLineColor(kRed);
  hDRBgSubAwHyPy->SetLineColor(kBlue);
  hDRBgSubNrHyPy->SetMarkerStyle(0);
  hDRBgSubAwHyPy->SetMarkerStyle(0);
  hDRBgSubNr->SetTitle(";R^{track}_{jet};fraction of #sum_{R<0.8}p_{T}^{Track}");
  hDRBgSubNr->SetAxisRange(0,0.784,"X");
  hDRBgSubNr->SetAxisRange(0,0.15,"Y");
  fixedFontHist(hDRBgSubNr);
  hDRBgSubNr->Draw("E");
  hDRBgSubNrHyPy->Draw("Ehistsame");
  hDRBgSubAwHyPy->Draw("Ehistsame");
  hDRBgSubNr->Draw("Esame");
  hDRBgSubAw->Draw("Esame");

  if (doLeg==1) {
    TLegend *leg = new TLegend(0.2,0.66,0.8,0.93);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(63);
    leg->SetTextSize(16);
    leg->AddEntry(hDRBgSubNr,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(1),hPt->GetBinLowEdge(2)),"");
    leg->AddEntry(hDRBgSubNr,"Data Leading Jet","pl");
    leg->AddEntry(hDRBgSubAw,"Data SubLeading Jet","pl");
    leg->AddEntry(hDRBgSubNrHyPy,"PYTHIA+HYDJET Leading Jet","l");
    leg->AddEntry(hDRBgSubAwHyPy,"PYTHIA+HYDJET SubLeading Jet","l");
    leg->Draw();
  }
}
