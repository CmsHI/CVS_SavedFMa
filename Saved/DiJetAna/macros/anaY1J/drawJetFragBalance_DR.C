#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TLine.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>


void drawJetFragBalance_DR(
    TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl",
    TString title = "test"
    ) {
  TFile *f = new TFile(inFileName+".root");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");

  // Get Pt info
  Int_t numBinsPt=hPtPNDR->GetNbinsX();
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  cout << "Pt bins: " << numBinsPt << endl;

  // Output
  TFile *outf = new TFile("plot/drawn_"+inFileName+"_"+title+".root","RECREATE");

  // How many pt bins to draw
  Int_t endPtBin=6;

  TCanvas * c6 = new TCanvas("c6","c6",1200,700);
  c6->Divide(3,2);
  for (Int_t i=1; i<=endPtBin; ++i) {
    // What pt bins to project
    Int_t iBeg=1,iEnd=i;
    // Last drawn bin draws all remaining pt bins
    if (i==endPtBin) iEnd=numBinsPt;
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDR_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADR_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hNrBg,-1);
    hAw->Add(hAwBg,-1);
    // Print
    cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1))
      << " SigSubBkg Integral - Nr: " << hNr->Integral() << " Aw: " << hAw->Integral() << endl;
    // Draw to inspect
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNr->SetMinimum(-10);
    hNr->SetMaximum(60);
    c6->cd(i);
    hNr->Draw();
    hNr->SetXTitle("#Delta R");
    hNr->SetYTitle("Background subtrated signal");
    hAw->Draw("same");
    TLine *l = new TLine(0,0,3.14/2,0);
    l->SetLineStyle(2);
    l->Draw();
    
    TLegend *leg = new TLegend(0.35,0.7,0.85,0.94);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hNr,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1)),"");
    leg->AddEntry(hNr,"Near Side","pl");
    leg->AddEntry(hAw,"Away Side","pl");
    leg->SetTextSize(0.05);
    leg->Draw();
  }
  c6->Print(Form("plot/%s_%s_DRSubBg.gif",inFileName.Data(),title.Data()));
  c6->Print(Form("plot/%s_%s_DRSubBg.eps",inFileName.Data(),title.Data()));
  c6->Print(Form("plot/%s_%s_DRSubBg.C",inFileName.Data(),title.Data()));

  // All Done
  outf->Write();
}
