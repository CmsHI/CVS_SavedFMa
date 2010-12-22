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
    Int_t plotMode = 0,
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
  Int_t numBinsPt=hPtPNDR->GetNbinsX();
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  cout << "Pt bins: " << numBinsPt << endl;

  // How many pt bins to draw
  Int_t endPtBin=numBinsPt;

  TCanvas * c6 = new TCanvas("c6","c6",1200,700);
  c6->Divide(3,2);
  for (Int_t i=1; i<=endPtBin; ++i) {
    // What pt bins to project
    Int_t iBeg=i,iEnd=i;
    // Last drawn bin draws all remaining pt bins
    if (i==endPtBin) iEnd=numBinsPt;
    TH1D * hNr = (TH1D*)hPtPNDRSub->ProjectionY(Form("hPNDRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADRSub->ProjectionY(Form("hPADRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrGen = (TH1D*)hPtPNDRSubGen->ProjectionY(Form("hPNDRSubGen_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwGen = (TH1D*)hPtPADRSubGen->ProjectionY(Form("hPADRSubGen_%d_%d",iBeg,iEnd),iBeg,iEnd);
    // Print
    cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1))
      << " SigSubBkg Integral - Nr: " << hNr->Integral() << " Aw: " << hAw->Integral() << endl
      << " Gen - Nr: " << hNrGen->Integral() << " Aw: " << hAwGen->Integral() << endl;
    // Draw to inspect
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNr->SetAxisRange(-5,35,"Y");
    hAwGen->SetLineStyle(2);
    c6->cd(i);
    hNr->Draw();
    hNr->SetXTitle("#Delta R");
    hNr->SetYTitle("#Background Subtracted Signal (GeV/c)");
    hAw->Draw("same");
    hNrGen->Draw("hist same");
    hAwGen->Draw("hist same");
    TLine *l = new TLine(0,0,3.14/2,0);
    l->Draw();
    
    TLegend *leg = new TLegend(0.35,0.7,0.85,0.94);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hNr,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1)),"");
    leg->AddEntry(hNr,"Leading (Reco)","pl");
    leg->AddEntry(hNrGen,"Leading (Gen)","l");
    leg->AddEntry(hAw,"SubLeading (Reco)","pl");
    leg->AddEntry(hAwGen,"SubLeading (Gen)","l");
    leg->SetTextSize(0.05);
    leg->Draw();
  }
  c6->Print(Form("plot/%s_%s_sysError.gif",inFileNameStrip.Data(),title.Data()));
}
