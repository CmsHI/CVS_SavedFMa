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
#include "Integrate.h"

void drawJetFragBalance_DRDiff(
			       TString inFileName=    "plot/jfhCorrEtaPtBin4RBin20v2_HCPR_J50U_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root",
			       TString inFileNameHyPy="plot/jfhCorrEtaPtBin4RBin20v2_Hydjet_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root",
    TString title = "test",
    Int_t drawMode=1,
			       Int_t doLeg=1,
bool cumulative = true
    ) {
  TFile *f = new TFile(inFileName);
  TString inFileNameStrip(inFileName); inFileNameStrip.ReplaceAll(".root","");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");
  TH2D * hPtPNDRBgSub = (TH2D*)hPtPNDR->Clone(inFileNameStrip+"hPtPNDRBgSub");
  TH2D * hPtPADRBgSub = (TH2D*)hPtPADR->Clone(inFileNameStrip+"hPtPADRBgSub");
  hPtPNDRBgSub->Add(hPtPNDR,hPtPNDRBg,1,-1);
  hPtPADRBgSub->Add(hPtPADR,hPtPADRBg,1,-1);

  TFile *fhypy = new TFile(inFileNameHyPy);
  TH2D * hPtPNDRHyPy = (TH2D*) fhypy->Get("hPtPNDR");
  TH2D * hPtPADRHyPy = (TH2D*) fhypy->Get("hPtPADR");
  TH2D * hPtPNDRBgHyPy = (TH2D*) fhypy->Get("hPtPNDRBg");
  TH2D * hPtPADRBgHyPy = (TH2D*) fhypy->Get("hPtPADRBg");
  TH2D * hPtPNDRBgSubHyPy = (TH2D*)hPtPNDR->Clone(inFileNameStrip+"hPtPNDRBgSubHyPy");
  TH2D * hPtPADRBgSubHyPy = (TH2D*)hPtPADR->Clone(inFileNameStrip+"hPtPADRBgSubHyPy");
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

  int ptUp = 2;
  TH1D * hDRBgSubNr = (TH1D*)hPtPNDRBgSub->ProjectionY(inFileNameStrip+"hDRBgSubNr",1,ptUp);
  TH1D * hDRBgSubAw = (TH1D*)hPtPADRBgSub->ProjectionY(inFileNameStrip+"hDRBgSubAw",1,ptUp);
  TH1D * hDRBgSubNrHyPy = (TH1D*)hPtPNDRBgSubHyPy->ProjectionY(inFileNameStrip+"hDRBgSubNrHyPy",1,ptUp);
  TH1D * hDRBgSubAwHyPy = (TH1D*)hPtPADRBgSubHyPy->ProjectionY(inFileNameStrip+"hDRBgSubAwHyPy",1,ptUp);
  // Print
  cout << Form("%.1f < p_{T} < %.1f GeV/c: ",hPt->GetBinLowEdge(1),hPt->GetBinLowEdge(ptUp+1)) << " SigSubBkg Integral - Nr: " << endl;
  cout << " Data - Nr: " << hDRBgSubNr->Integral() << " Aw: " << hDRBgSubAw->Integral() << endl;
  cout << " Pythia+Hydjet - Nr: " << hDRBgSubNrHyPy->Integral() << " Aw: " << hDRBgSubAwHyPy->Integral() << endl;

  if (drawMode==1) TCanvas * c6 = new TCanvas("c6","c6",500,500);
  hDRBgSubNr->Scale(1./totPtBgSubNr);
  hDRBgSubAw->Scale(1./totPtBgSubAw);
  hDRBgSubNrHyPy->Scale(1./totPtBgSubNrHyPy);
  hDRBgSubAwHyPy->Scale(1./totPtBgSubAwHyPy);

  if(cumulative){
     hDRBgSubNr = IntegrateFromLeft(hDRBgSubNr);
     hDRBgSubAw = IntegrateFromLeft(hDRBgSubAw);
     hDRBgSubNrHyPy = IntegrateFromLeft(hDRBgSubNrHyPy);
     hDRBgSubAwHyPy = IntegrateFromLeft(hDRBgSubAwHyPy);
  }

  // Set Styles
  hDRBgSubNr->SetMarkerStyle(kOpenSquare);
  mcStyle1(hDRBgSubNrHyPy);
  mcStyle2(hDRBgSubAwHyPy);
  hDRBgSubNrHyPy->SetMarkerStyle(0);
  hDRBgSubAwHyPy->SetMarkerStyle(0);
  // Draw
  hDRBgSubNrHyPy->SetTitle(";#DeltaR_{max};F(#DeltaR<#DeltaR_{max})");
  hDRBgSubNrHyPy->SetAxisRange(0,0.784,"X");
  hDRBgSubNrHyPy->SetAxisRange(0,0.7,"Y");
  fixedFontHist(hDRBgSubNrHyPy);
  hDRBgSubNrHyPy->DrawCopy("Ehist");
  hDRBgSubAwHyPy->DrawCopy("Ehistsame");
  hDRBgSubNr->DrawCopy("Esame");
  hDRBgSubAw->DrawCopy("Esame");

  if (doLeg==1) {
     TLegend *leg = new TLegend(0.302407,0.67,0.7536548,0.9324599);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(63);
    leg->SetTextSize(16);
    leg->AddEntry(hDRBgSubNr,Form("%.1f < p_{T} < %.1f GeV/c",hPt->GetBinLowEdge(1),hPt->GetBinLowEdge(ptUp+1)),"");
    leg->AddEntry(hDRBgSubNr,"Data Leading Jet","pl");
    leg->AddEntry(hDRBgSubAw,"Data SubLeading Jet","pl");
    leg->AddEntry(hDRBgSubNrHyPy,"MC Leading Jet","l");
    leg->AddEntry(hDRBgSubAwHyPy,"MC SubLeading Jet","l");
    leg->Draw();
  }
}
