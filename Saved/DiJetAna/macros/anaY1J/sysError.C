#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TLine.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>


void sysError(
    TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl.root",
    TString inFileNameGen="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl.root",
    Int_t sysMode = 0, // 0 for simple plot, 1 for difference
    TString title = "test"
    ) {
  TFile *f = new TFile(inFileName);
  TString inFileNameStrip(inFileName); inFileNameStrip.ReplaceAll(".root","");
  TFile *fgen = new TFile(inFileNameGen);
  TString inFileNameStripGen(inFileNameGen); inFileNameStripGen.ReplaceAll(".root","");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");
  TH2D * hPtPNDRSub = (TH2D*)hPtPNDR->Clone(inFileNameStrip+"hPtPNDRSub");
  TH2D * hPtPADRSub = (TH2D*)hPtPADR->Clone(inFileNameStrip+"hPtPADRSub");
  hPtPNDRSub->Add(hPtPNDR,hPtPNDRBg,1,-1);
  hPtPADRSub->Add(hPtPADR,hPtPADRBg,1,-1);

  TH2D * hPtPNDRGen = (TH2D*) fgen->Get("hPtPNDR");
  TH2D * hPtPADRGen = (TH2D*) fgen->Get("hPtPADR");
  TH2D * hPtPNDRBgGen = (TH2D*) fgen->Get("hPtPNDRBg");
  TH2D * hPtPADRBgGen = (TH2D*) fgen->Get("hPtPADRBg");
  TH2D * hPtPNDRSubGen = (TH2D*)hPtPNDRGen->Clone(inFileNameStripGen+"hPtPNDRSub");
  TH2D * hPtPADRSubGen = (TH2D*)hPtPADRGen->Clone(inFileNameStripGen+"hPtPADRSub");
  hPtPNDRSubGen->Add(hPtPNDRGen,hPtPNDRBgGen,1,-1);
  hPtPADRSubGen->Add(hPtPADRGen,hPtPADRBgGen,1,-1);

  // Get Pt info
  Int_t numPtBins=hPtPNDR->GetNbinsX();
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  cout << "Pt bins: " << numPtBins << endl;
  for (Int_t i=0; i<numPtBins+2; ++i) {
    cout << "Pt Bin " << i << " Low Edge: " << hPt->GetBinLowEdge(i) << endl;
  }

  // What pt bins to draw
  const Int_t numPtBinsDraw=3;

  TCanvas * c6 = new TCanvas("c6","c6",1400,500);
  c6->Divide(3,1);
  for (Int_t i=0; i<numPtBinsDraw; ++i) {
    Int_t iBeg,iEnd;
    if (i==0) { iBeg=1; iEnd=2;}
    if (i==1) { iBeg=3; iEnd=3;}
    if (i==2) { iBeg=4; iEnd=numPtBins;}
    cout << "Bin: " << iBeg <<  " to " << iEnd << endl;
    TH1D * hNr = (TH1D*)hPtPNDRSub->ProjectionY(Form("hPNDRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADRSub->ProjectionY(Form("hPADRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrGen = (TH1D*)hPtPNDRSubGen->ProjectionY(Form("hPNDRSubGen_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwGen = (TH1D*)hPtPADRSubGen->ProjectionY(Form("hPADRSubGen_%d_%d",iBeg,iEnd),iBeg,iEnd);
    if (sysMode==1) {
      hNr->Add(hNrGen,-1);
      hAw->Add(hAwGen,-1);
    }
    if (sysMode==2) {
      hNr->Divide(hNrGen);
      hAw->Divide(hAwGen);
    }
    // Print
    cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1))
      << " SigSubBkg Integral - Nr: " << hNr->Integral() << " Aw: " << hAw->Integral() << endl
      << " Gen - Nr: " << hNrGen->Integral() << " Aw: " << hAwGen->Integral() << endl;
    // Styles
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hAwGen->SetLineStyle(2);
    // Axis
    if (sysMode==0) {
      hNr->SetYTitle("Background Subtracted Signal (GeV/c)");
      hNr->SetAxisRange(-5,60,"Y");
    }
    if (sysMode==1) {
      hNr->SetYTitle("Reco-Gen (GeV/c)");
      hNr->SetAxisRange(-20,20,"Y");
    }
    if (sysMode==2) {
      hNr->SetYTitle("Reco/Gen (GeV/c)");
      hNr->SetAxisRange(0,2,"Y");
    }
    hNr->SetXTitle("#Delta R");
    hNr->SetAxisRange(0,0.8,"X");
    hNr->SetTitleOffset(1.5,"X");
    hNr->GetXaxis()->CenterTitle();
    hNr->GetYaxis()->CenterTitle();
    c6->cd(i+1);
    // Draw to Inspect
    hNr->Draw();
    hAw->Draw("same");
    if (sysMode==0) {
      hNrGen->Draw("hist same");
      hAwGen->Draw("hist same");
    }
    if (sysMode==0||sysMode==1) {
      TLine *l = new TLine(0,0,0.8,0);
      l->Draw();
    }
    if (sysMode==2) {
      TLine *l = new TLine(0,1,0.8,1);
      l->SetLineStyle(2);
      l->Draw();
    }
    
    TLegend *leg = new TLegend(0.35,0.7,0.85,0.94);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hNr,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1)),"");
    leg->AddEntry(hNr,"Leading (Reco)","pl");
    if (sysMode==0) leg->AddEntry(hNrGen,"Leading (Gen)","l");
    leg->AddEntry(hAw,"SubLeading (Reco)","pl");
    if (sysMode==0) leg->AddEntry(hAwGen,"SubLeading (Gen)","l");
    leg->SetTextSize(0.05);
    leg->Draw();
  }
  c6->Print(Form("plot/%s_%s_%s_sysError%d.gif",inFileNameStrip.Data(),inFileNameStripGen.Data(),title.Data(),sysMode));
}
