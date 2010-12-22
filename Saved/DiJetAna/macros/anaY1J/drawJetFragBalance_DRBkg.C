#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TLine.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>


void drawJetFragBalance_DRBkg(
    TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl.root",
    TString title = "test"
    ) {
  TFile *f = new TFile(inFileName);

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");

  // Get Pt info
  Int_t numBinsPt=hPtPNDR->GetNbinsX();
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  cout << "Pt bins: " << numBinsPt << endl;

  // Output
  TString inFileNameStrip(inFileName); inFileNameStrip.ReplaceAll(".root","");
  TFile *outf = new TFile("plot/drawn_"+inFileNameStrip+"_"+title+".root","RECREATE");

  // How many pt bins to draw
  Int_t endPtBin=numBinsPt;

  TCanvas * c6 = new TCanvas("c6","c6",1200,700);
  c6->Divide(3,2);
  for (Int_t i=1; i<=endPtBin; ++i) {
    // What pt bins to project
    Int_t iBeg=i,iEnd=i;
    // Last drawn bin draws all remaining pt bins
    if (i==endPtBin) iEnd=numBinsPt;
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDR_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADR_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    // Print
    cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1))
      << " p_{T} integral - Nr: " << hNr->Integral() << " Aw: " << hAw->Integral() << endl;
    // Draw to inspect
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNrBg->SetMarkerStyle(0);
    hNrBg->SetLineColor(kRed);
    hNrBg->SetLineStyle(9);
    hAwBg->SetMarkerStyle(0);
    hAwBg->SetLineColor(kBlue);
    hAwBg->SetLineStyle(9);
    hNr->SetAxisRange(0,hNr->GetBinLowEdge(hNr->GetNbinsX()/2.),"X");
    hNr->SetAxisRange(-5,30,"Y");
    hNr->SetTitleOffset(1.2,"X");
    hNr->SetTitleOffset(1.2,"Y");
    c6->cd(i);
    hNr->Draw();
    hNr->SetXTitle("#Delta R");
    hNr->SetYTitle("#sump_{T}");
    hAw->Draw("same");
    hNrBg->Draw("same histE");
    hAwBg->Draw("same histE");
    TLine *l = new TLine(hNr->GetBinLowEdge(1),0,hNr->GetBinLowEdge(hNr->GetNbinsX()/2.+1),0);
    l->SetLineStyle(2);
    l->Draw();
    
    TLegend *leg = new TLegend(0.3,0.6,0.85,0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hNr,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1)),"");
    leg->AddEntry(hNr,"Leading Jet","pl");
    leg->AddEntry(hNrBg,"Leading Jet Bkg","l");
    leg->AddEntry(hAw,"SubLeading Jet","pl");
    leg->AddEntry(hAwBg,"SubLeading Jet Bkg","l");
    leg->SetTextSize(0.05);
    leg->Draw();
  }
  c6->Print(Form("plot/%s_%s_DRBg.gif",inFileNameStrip.Data(),title.Data()));
  c6->Print(Form("plot/%s_%s_DRBg.eps",inFileNameStrip.Data(),title.Data()));
  c6->Print(Form("plot/%s_%s_DRBg.C",inFileNameStrip.Data(),title.Data()));

  // All Done
  outf->Write();
}
