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

Float_t getBinMean(TH1D* h, Int_t ibin)
{
  return h->GetBinContent(ibin);
}

void drawJetFragBalance_Pt(
    TString inFileName="jfhCorrEtaPtBin4RBin20v0_HCPR_J50U_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root",
    TString inFileNameHyPy="jfhCorrEtaPtBin4RBin20v0_Hydjet_J50U_djcalo_Cent0to30_Aj0to100_SubEtaRefl.root",
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

  if (drawMode==1) TCanvas * c6 = new TCanvas("c6","c6",500,500);
  gPad->SetLogx();
  TH1D * hPtBgSubNr = (TH1D*)hPtPNDRBgSub->ProjectionX("hPtBgSubNr");
  TH1D * hPtBgSubAw = (TH1D*)hPtPADRBgSub->ProjectionX("hPtBgSubAw");
  TH1D * hPtBgSubNrHyPy = (TH1D*)hPtPNDRBgSubHyPy->ProjectionX("hPtBgSubNrHyPy");
  TH1D * hPtBgSubAwHyPy = (TH1D*)hPtPADRBgSubHyPy->ProjectionX("hPtBgSubAwHyPy");
  cout << "Data Pt tot  Nr: " << hPtBgSubNr->Integral() << " Aw: " << hPtBgSubAw->Integral() << endl;
  cout << "Pythia+Hydjet Pt tot  Nr: " << hPtBgSubNrHyPy->Integral() << " Aw: " << hPtBgSubAwHyPy->Integral() << endl;
  hPtBgSubNr->Scale(1./totPtBgSubNr);
  hPtBgSubAw->Scale(1./totPtBgSubAw);
  hPtBgSubNrHyPy->Scale(1./totPtBgSubNrHyPy);
  hPtBgSubAwHyPy->Scale(1./totPtBgSubAwHyPy);
  hPtBgSubNr->SetMarkerStyle(kOpenSquare);
  hPtBgSubNrHyPy->SetLineColor(kRed);
  hPtBgSubAwHyPy->SetLineColor(kBlue);
  hPtBgSubNrHyPy->SetMarkerStyle(0);
  hPtBgSubAwHyPy->SetMarkerStyle(0);
  hPtBgSubNr->SetTitle(";p_{T}^{Track};fraction of #sum_{R<0.8}p_{T}^{Track}");
  hPtBgSubNr->SetAxisRange(0,0.8,"Y");
  fixedFontHist(hPtBgSubNr);
  hPtBgSubNr->DrawCopy("E");
  hPtBgSubNrHyPy->DrawCopy("Ehistsame");
  hPtBgSubAwHyPy->DrawCopy("Ehistsame");
  hPtBgSubNr->DrawCopy("Esame");
  hPtBgSubAw->DrawCopy("Esame");

  if (doLeg==1) {
    TLegend *leg = new TLegend(0.302407,0.712258,0.7536548,0.9324599);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    //leg->SetNColumns(2);
    leg->SetTextFont(63);
    leg->SetTextSize(16);
    leg->AddEntry(hPtBgSubNr,"Data Leading Jet","pl");
    leg->AddEntry(hPtBgSubAw,"Data SubLeading Jet","pl");
    leg->AddEntry(hPtBgSubNrHyPy,"PYTHIA+HYDJET Leading Jet","l");
    leg->AddEntry(hPtBgSubAwHyPy,"PYTHIA+HYDJET SubLeading Jet","l");
    leg->Draw();
  }
}
