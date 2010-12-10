#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TLine.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>


void drawMeanAJ(
    TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl",
    TString title = "test"
    ) {
  TFile *f = new TFile(inFileName+".root");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");

  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX();
  hPt->SetName("hPt");

  Int_t numBinsPt=hPtPNDR->GetNbinsX();

  Int_t numBinsDR=5; // for the integration in dr we integrate to ~0.8 for now
  cout << "DR vs Pt: pt bins: " << numBinsPt << " DR bins: " << numBinsDR << endl;

  Int_t endBinX=6;
  Int_t endBinY=9;

  TFile *outf = new TFile("plot/drawn_"+inFileName+"_"+title+".root","RECREATE");
  TNtuple *nt = new TNtuple("nt","","aj:dR:et1:et2:bin1:bin2");

  TCanvas * c6 = new TCanvas("c6","c6",1200,700);
  c6->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    Int_t iBeg=i,iEnd=numBinsPt;
    c6->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDRSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADRSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBgSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBgSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hNrBg,-1);
    hAw->Add(hAwBg,-1);
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(120);
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

  TCanvas * c7 = new TCanvas("c7","c7",1200,700);
  c7->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    Int_t iBeg=i,iEnd=numBinsPt;
    c7->cd(i);
    TH1D * hNrRaw = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDRSubBgRaw_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwRaw = (TH1D*)hPtPADR->ProjectionY(Form("hPADRSubBgRaw_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBgRaw = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBgSubBgRaw_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBgRaw = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBgSubBgRaw_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNrRaw->Add(hNrBgRaw,-1);
    hAwRaw->Add(hAwBgRaw,-1);
    
    TH1D * hNr = (TH1D*) hNrRaw->Clone();
    TH1D * hAw = (TH1D*) hAwRaw->Clone();
    hNr->SetName(Form("hNearSideAccumulated_%d_%d",iBeg,iEnd));
    hAw->SetName(Form("hAwaySideAccumulated_%d_%d",iBeg,iEnd));
    
    double sumNear =0;
    double sumAway =0;
    double sumNearErrSq =0;
    double sumAwayErrSq =0;
    for (int j=1;j<hNr->GetNbinsX()+1;j++)
    {
       sumNear += hNrRaw->GetBinContent(j);
       sumNearErrSq += hNrRaw->GetBinError(j)*hNrRaw->GetBinError(j);
       hNr->SetBinContent(j,sumNear);
       hNr->SetBinError(j,sqrt(sumNearErrSq));

       sumAway += hAwRaw->GetBinContent(j);
       sumAwayErrSq += hAwRaw->GetBinError(j)*hAwRaw->GetBinError(j);
       hAw->SetBinContent(j,sumAway);
       hAw->SetBinError(j,sqrt(sumAwayErrSq));
       
       double aj = (sumNear-sumAway)/(sumNear+sumAway);
       nt->Fill(aj,hNr->GetBinLowEdge(j),sumNear,sumAway,iBeg,iEnd);
    }
    
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(200);
    hNr->Draw();
    hNr->SetXTitle("#Delta R");
    hNr->SetYTitle("Integrated bck subtracted signal");
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
  c7->Print(Form("plot/%s_%s_DRSubBg.gif",inFileName.Data(),title.Data()));
  c7->Print(Form("plot/%s_%s_DRSubBg.eps",inFileName.Data(),title.Data()));
  c7->Print(Form("plot/%s_%s_DRSubBg.C",inFileName.Data(),title.Data()));

  TCanvas * c8 = new TCanvas("c8","c8",1200,700);
  c8->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    c8->cd(i);
    TH1D *h = new TH1D("hTmp","",100,0,3.1415926/2.);
    h->SetAxisRange(0,1.2,"Y");
    h->SetXTitle("#Delta R_{Max}");
    h->SetYTitle("<A_{J}>");
    h->Draw();
    Int_t iBeg=i,iEnd=numBinsPt;
    nt->Draw("aj:dR",Form("bin1==%d&&bin2==%d",iBeg,iEnd),"same");
    TLegend *leg = new TLegend(0.35,0.7,0.85,0.94);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(h,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1)),"");
    leg->SetTextSize(0.05);
    leg->Draw();
    
  }
  c8->Print(Form("plot/%s_%s_AJ.gif",inFileName.Data(),title.Data()));
  c8->Print(Form("plot/%s_%s_AJ.eps",inFileName.Data(),title.Data()));
  c8->Print(Form("plot/%s_%s_AJ.C",inFileName.Data(),title.Data()));




  // All Done
  outf->Write();
}
