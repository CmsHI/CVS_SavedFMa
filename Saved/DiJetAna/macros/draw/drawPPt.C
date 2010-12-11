#include <iostream>
#include "HisMath.C"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
using namespace std;

void drawPPt(TString infileData="",
    TString infileMc="", bool doLeg=false)
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

  TFile * infMc = new TFile(infileMc);
  TH1D * hNrBgSubMc = (TH1D*)infMc->Get("hNrCPPtBgSub");
  TH1D * hAwBgSubMc = (TH1D*)infMc->Get("hAwCPPtBgSub");
  normHist(hNrBgSubMc,0,true);
  hNrBgSubMc->SetLineStyle(7);
  normHist(hAwBgSubMc,0,true);

  //TCanvas * c2 = new TCanvas("c2","c2",500,500);
  gPad->SetLogy();
  hNrBgSubData->SetXTitle("p_{T}^{track}");
  hNrBgSubData->SetYTitle("1/N_{Jet} dN/dp_{T} (Bkg Sub)");
  hNrBgSubData->SetAxisRange(0,59.99,"X");
  hNrBgSubData->SetAxisRange(1e-4,2e2,"Y");
  hNrBgSubData->Draw("E");
  hAwBgSubData->Draw("sameE");
  hNrBgSubMc->Draw("same hist");
  hAwBgSubMc->Draw("same hist");

  Float_t nrConePtData=0, awConePtData=0, nrConePtMc=0, awConePtMc=0;
  for (Int_t i=0; i<hNrBgSubData->GetNbinsX(); ++i) {
    nrConePtData+=(hNrBgSubData->GetBinContent(i)*hNrBgSubData->GetBinCenter(i)*hNrBgSubData->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubData->GetNbinsX(); ++i) {
    awConePtData+=(hAwBgSubData->GetBinContent(i)*hAwBgSubData->GetBinCenter(i)*hAwBgSubData->GetBinWidth(i));
  }
  for (Int_t i=0; i<hNrBgSubMc->GetNbinsX(); ++i) {
    nrConePtMc+=(hNrBgSubMc->GetBinContent(i)*hNrBgSubMc->GetBinCenter(i)*hNrBgSubMc->GetBinWidth(i));
  }
  for (Int_t i=0; i<hAwBgSubMc->GetNbinsX(); ++i) {
    awConePtMc+=(hAwBgSubMc->GetBinContent(i)*hAwBgSubMc->GetBinCenter(i)*hAwBgSubMc->GetBinWidth(i));
  }
  cout << "Data cone pt sum - Nr: " << nrConePtData
    << " Aw: " << awConePtData << endl;
  cout << "Mc cone pt sum - Nr: " << nrConePtMc
    << " Aw: " << awConePtMc << endl;

  if (doLeg) {
    TLegend * l = new TLegend(0.56,0.62,0.86,0.92);
    l->SetBorderSize(0);
    l->SetFillStyle(0);
    l->SetTextSize(0.03);
    l->AddEntry(hNrBgSubData,"Pb+Pb #sqrt{s_{NN}}=2.76TeV","");
    l->AddEntry(hNrBgSubData,"Leading Jet Data","pl");
    l->AddEntry(hAwBgSubData,"Sub-Leading Jet Data","pl");
    l->AddEntry(hNrBgSubMc,"Leading Jet Pythia","l");
    l->AddEntry(hAwBgSubMc,"Sub-Leading Jet Pythia","l");
    l->Draw();
  }
}
