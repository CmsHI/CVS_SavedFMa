#include <iostream>
#include "HisMath.C"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
using namespace std;

void drawPPt(TString infileData="",
    TString infilePy="",
    TString infileHyPy="",
    bool doLeg=true)
{
  TFile * infData = new TFile(infileData);
  TH1D * hNrBgSubData = (TH1D*)infData->Get("hNrCPPtBgSub");
  TH1D * hAwBgSubData = (TH1D*)infData->Get("hAwCPPtBgSub");
  normHist(hNrBgSubData,0,true);
  hNrBgSubData->SetMarkerColor(2);
  hNrBgSubData->SetLineColor(2);
  hNrBgSubData->SetMarkerStyle(kOpenCircle);
  normHist(hAwBgSubData,0,true);
  hAwBgSubData->SetMarkerColor(2);
  hAwBgSubData->SetLineColor(2);

  TFile * infPy = new TFile(infilePy);
  TH1D * hNrBgSubPy = (TH1D*)infPy->Get("hNrCPPtBgSub");
  TH1D * hAwBgSubPy = (TH1D*)infPy->Get("hAwCPPtBgSub");
  normHist(hNrBgSubPy,0,true);
  hNrBgSubPy->SetLineStyle(7);
  normHist(hAwBgSubPy,0,true);

  TFile * infHyPy = new TFile(infileHyPy);
  TH1D * hNrBgSubHyPy = (TH1D*)infHyPy->Get("hNrCPPtBgSub");
  TH1D * hAwBgSubHyPy = (TH1D*)infHyPy->Get("hAwCPPtBgSub");
  normHist(hNrBgSubHyPy,0,true);
  normHist(hAwBgSubHyPy,0,true);
  hNrBgSubHyPy->SetLineColor(kBlue);
  hNrBgSubHyPy->SetMarkerColor(kBlue);
  hNrBgSubHyPy->SetMarkerStyle(kOpenSquare);
  hAwBgSubHyPy->SetLineColor(kBlue);
  hAwBgSubHyPy->SetMarkerColor(kBlue);
  hAwBgSubHyPy->SetMarkerStyle(kFullSquare);

  //TCanvas * c2 = new TCanvas("c2","c2",500,500);
  gPad->SetLogy();
  hNrBgSubData->SetXTitle("p_{T}^{track}");
  hNrBgSubData->SetYTitle("1/N_{Jet} dN/dp_{T} (Bkg Sub)");
  hNrBgSubData->SetAxisRange(0,59.99,"X");
  hNrBgSubData->SetAxisRange(1e-4,2e2,"Y");
  hNrBgSubData->Draw("E");
  hAwBgSubData->Draw("sameE");
  hNrBgSubPy->Draw("same hist");
  hAwBgSubPy->Draw("same hist");
  hNrBgSubHyPy->Draw("same E");
  hAwBgSubHyPy->Draw("same E");

  Float_t nrConePtData=0, awConePtData=0, nrConePtPy=0, awConePtPy=0,nrConePtHyPy=0, awConePtHyPy=0;
  for (Int_t i=0; i<hNrBgSubData->GetNbinsX(); ++i) {
    nrConePtData+=(hNrBgSubData->GetBinContent(i)*hNrBgSubData->GetBinCenter(i)*hNrBgSubData->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubData->GetNbinsX(); ++i) {
    awConePtData+=(hAwBgSubData->GetBinContent(i)*hAwBgSubData->GetBinCenter(i)*hAwBgSubData->GetBinWidth(i));
  }
  for (Int_t i=0; i<hNrBgSubPy->GetNbinsX(); ++i) {
    nrConePtPy+=(hNrBgSubPy->GetBinContent(i)*hNrBgSubPy->GetBinCenter(i)*hNrBgSubPy->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubPy->GetNbinsX(); ++i) {
    awConePtPy+=(hAwBgSubPy->GetBinContent(i)*hAwBgSubPy->GetBinCenter(i)*hAwBgSubPy->GetBinWidth(i));
  }
  for (Int_t i=0; i<hNrBgSubHyPy->GetNbinsX(); ++i) {
    nrConePtHyPy+=(hNrBgSubHyPy->GetBinContent(i)*hNrBgSubHyPy->GetBinCenter(i)*hNrBgSubHyPy->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubHyPy->GetNbinsX(); ++i) {
    awConePtHyPy+=(hAwBgSubHyPy->GetBinContent(i)*hAwBgSubHyPy->GetBinCenter(i)*hAwBgSubHyPy->GetBinWidth(i));
  }
  cout << "Data cone pt sum - Nr: " << nrConePtData
    << " Aw: " << awConePtData << endl;
  cout << "Mc cone pt sum - Nr: " << nrConePtPy
    << " Aw: " << awConePtPy << endl;
  cout << "Mc cone pt sum - Nr: " << nrConePtHyPy
    << " Aw: " << awConePtHyPy << endl;

  if (doLeg) {
    TLegend * l = new TLegend(0.42,0.62,0.71,0.92);
    l->SetBorderSize(0);
    l->SetFillStyle(0);
    l->SetTextSize(0.03);
    l->AddEntry(hNrBgSubData,"Pb+Pb #sqrt{s_{NN}}=2.76TeV","");
    l->AddEntry(hNrBgSubData,"Leading Jet Data","pl");
    l->AddEntry(hAwBgSubData,"Sub-Leading Jet Data","pl");
    l->AddEntry(hNrBgSubPy,"Leading Jet Pythia","l");
    l->AddEntry(hAwBgSubPy,"Sub-Leading Jet Pythia","l");
    l->AddEntry(hNrBgSubHyPy,"Leading Jet Pythia+Hydjet","pl");
    l->AddEntry(hAwBgSubHyPy,"Sub-Leading Jet Pythia+Hydjet","pl");
    l->Draw();
  }
}
