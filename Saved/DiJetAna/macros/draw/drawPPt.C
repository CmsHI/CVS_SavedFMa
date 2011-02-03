#include <iostream>
#include "HisMath.C"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLine.h"
using namespace std;

void drawPPt(TString infileData="",
    TString infileHyPy="",
    TString infilePy="",
    bool doLeg=true,
    Int_t doRat=0)
{
  cout << "Data: " << infileData << endl;
  cout << "HyPy: " << infileHyPy << endl;
  cout << "Py: " << infilePy << endl;
  // Get Histograms and normalize
  TFile * infData = new TFile(infileData);
  TH1D * hNrBgSubData = (TH1D*)infData->Get("hNrCPPtBgSub");
  normHist(hNrBgSubData,0,true);
  TH1D * hAwBgSubData = (TH1D*)infData->Get("hAwCPPtBgSub");
  normHist(hAwBgSubData,0,true);
  hNrBgSubData->SetMarkerColor(2);
  hNrBgSubData->SetLineColor(2);
  hNrBgSubData->SetMarkerStyle(kOpenCircle);
  hAwBgSubData->SetMarkerColor(2);
  hAwBgSubData->SetLineColor(2);

  TFile * infHyPy = new TFile(infileHyPy);
  TH1D * hNrBgSubHyPy = (TH1D*)infHyPy->Get("hNrCPPtBgSub");
  normHist(hNrBgSubHyPy,0,true);
  TH1D * hAwBgSubHyPy = (TH1D*)infHyPy->Get("hAwCPPtBgSub");
  normHist(hAwBgSubHyPy,0,true);
  hNrBgSubHyPy->SetLineColor(kBlue);
  hNrBgSubHyPy->SetMarkerColor(kBlue);
  hNrBgSubHyPy->SetMarkerStyle(kOpenSquare);
  hAwBgSubHyPy->SetLineColor(kBlue);
  hAwBgSubHyPy->SetMarkerColor(kBlue);
  hAwBgSubHyPy->SetMarkerStyle(kFullSquare);

  //TFile * infPy = new TFile(infilePy);
  //TH1D * hNrBgSubPy = (TH1D*)infPy->Get("hNrCPPtBgSub");
  //normHist(hNrBgSubPy,0,true);
  //TH1D * hAwBgSubPy = (TH1D*)infPy->Get("hAwCPPtBgSub");
  //normHist(hAwBgSubPy,0,true);
  //hNrBgSubPy->SetLineStyle(7);

  // Print out sums
  Float_t nrConePtData=0, awConePtData=0, nrConePtPy=0, awConePtPy=0,nrConePtHyPy=0, awConePtHyPy=0;
  for (Int_t i=0; i<hNrBgSubData->GetNbinsX(); ++i) {
    nrConePtData+=(hNrBgSubData->GetBinContent(i)*hNrBgSubData->GetBinCenter(i)*hNrBgSubData->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubData->GetNbinsX(); ++i) {
    awConePtData+=(hAwBgSubData->GetBinContent(i)*hAwBgSubData->GetBinCenter(i)*hAwBgSubData->GetBinWidth(i));
  }
  for (Int_t i=0; i<hNrBgSubHyPy->GetNbinsX(); ++i) {
    nrConePtHyPy+=(hNrBgSubHyPy->GetBinContent(i)*hNrBgSubHyPy->GetBinCenter(i)*hNrBgSubHyPy->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubHyPy->GetNbinsX(); ++i) {
    awConePtHyPy+=(hAwBgSubHyPy->GetBinContent(i)*hAwBgSubHyPy->GetBinCenter(i)*hAwBgSubHyPy->GetBinWidth(i));
  }
  //for (Int_t i=0; i<hNrBgSubPy->GetNbinsX(); ++i) {
    //nrConePtPy+=(hNrBgSubPy->GetBinContent(i)*hNrBgSubPy->GetBinCenter(i)*hNrBgSubPy->GetBinWidth(i));
  //}
  //for (Int_t i=0; i<hAwBgSubPy->GetNbinsX(); ++i) {
   // awConePtPy+=(hAwBgSubPy->GetBinContent(i)*hAwBgSubPy->GetBinCenter(i)*hAwBgSubPy->GetBinWidth(i));
  //}
  cout << "Data cone pt sum - Nr: " << nrConePtData
    << " Aw: " << awConePtData << endl;
  cout << "HyPy cone pt sum - Nr: " << nrConePtHyPy
    << " Aw: " << awConePtHyPy << endl;
  //cout << "Py cone pt sum - Nr: " << nrConePtPy
   // << " Aw: " << awConePtPy << endl;

  // Plot
  //TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hNrBgSubHyPy->SetXTitle("p_{T}^{track}");
  //hNrBgSubHyPy->SetAxisRange(0,59.99,"X");
  if (doRat==0) {
    gPad->SetLogy();
    hNrBgSubHyPy->SetYTitle("1/N_{Jet} dN/dp_{T} (Bkg Sub)");
    hNrBgSubHyPy->SetAxisRange(1e-4,2e2,"Y");
    hNrBgSubHyPy->Draw("E");
    hAwBgSubHyPy->Draw("sameE");
    hNrBgSubData->Draw("sameE");
    hAwBgSubData->Draw("sameE");
    //hNrBgSubPy->Draw("same hist");
    //hAwBgSubPy->Draw("same hist");

    if (doLeg) {
      TLegend * l = new TLegend(0.34,0.62,0.63,0.92);
      l->SetBorderSize(0);
      l->SetFillStyle(0);
      l->SetTextSize(0.04);
      //l->AddEntry(hNrBgSubData,"Pb+Pb #sqrt{s_{NN}}=2.76TeV","");
      l->AddEntry(hNrBgSubData,"Leading Jet Pythia+Hydjet Trk","pl");
      l->AddEntry(hAwBgSubData,"Sub-Leading Jet Pythia+Hydjet Trk","pl");
      l->AddEntry(hNrBgSubHyPy,"Leading Jet Pythia+Hydjet GenParticle","pl");
      l->AddEntry(hAwBgSubHyPy,"Sub-Leading Jet Pythia+Hydjet GenParticle","pl");
      //l->AddEntry(hNrBgSubPy,"Leading Jet Pythia GenParticle","l");
      //l->AddEntry(hAwBgSubPy,"Sub-Leading Jet Pythia GenParticle","l");
      l->Draw();
    }
  }

  if (doRat==1) {
    TH1D * hNrRatDataPy = (TH1D*)hNrBgSubData->Clone("hNrRatDataPy");
    hNrRatDataPy->Divide(hNrBgSubData,hNrBgSubPy);
    TH1D * hAwRatDataPy = (TH1D*)hAwBgSubData->Clone("hAwRatDataPy");
    hAwRatDataPy->Divide(hAwBgSubData,hAwBgSubPy);
    TH1D * hNrRatHyPyPy = (TH1D*)hNrBgSubHyPy->Clone("hNrRatHyPyPy");
    hNrRatHyPyPy->Divide(hNrBgSubHyPy,hNrBgSubPy);
    TH1D * hAwRatHyPyPy = (TH1D*)hAwBgSubHyPy->Clone("hAwRatHyPyPy");
    hAwRatHyPyPy->Divide(hAwBgSubHyPy,hAwBgSubPy);
    hNrRatDataPy->SetYTitle("Ratio (over Pythia)");
    hNrRatDataPy->SetAxisRange(0.5,3.5,"Y");
    hNrRatDataPy->Draw("E");
    hAwRatDataPy->Draw("Esame");
    hNrRatHyPyPy->Draw("Esame");
    hAwRatHyPyPy->Draw("Esame");
  }

  if (doRat==2) {
    TH1D * hNrRatDataHyPy = (TH1D*)hNrBgSubData->Clone("hNrRatDataHyPy");
    hNrRatDataHyPy->Divide(hNrBgSubData,hNrBgSubHyPy);
    TH1D * hAwRatDataHyPy = (TH1D*)hAwBgSubData->Clone("hAwRatDataHyPy");
    hAwRatDataHyPy->Divide(hAwBgSubData,hAwBgSubHyPy);
    TH1D * hNrRatPyHyPy = (TH1D*)hNrBgSubPy->Clone("hNrRatPyHyPy");
    hNrRatPyHyPy->Divide(hNrBgSubPy,hNrBgSubHyPy);
    TH1D * hAwRatPyHyPy = (TH1D*)hAwBgSubPy->Clone("hAwRatPyHyPy");
    hAwRatPyHyPy->Divide(hAwBgSubPy,hAwBgSubHyPy);
    hNrRatDataHyPy->SetYTitle("Ratio (over Pythia+Hydjet)");
    hNrRatDataHyPy->SetAxisRange(-0.5,3.5,"Y");
    hNrRatDataHyPy->Draw("E");
    hAwRatDataHyPy->Draw("Esame");
    hNrRatPyHyPy->Draw("hist same");
    hAwRatPyHyPy->Draw("hist same");
  }
  if (doRat) {
    TLine * l = new TLine(0,1,80,1);
    l->SetLineStyle(2);
    l->Draw();
  }
}
