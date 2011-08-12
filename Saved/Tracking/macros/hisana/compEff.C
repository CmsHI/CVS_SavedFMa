#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TProfile.h>
#include "TProfile2D.h"
#include "TLine.h"
#include "TLegend.h"


void compEff(){
  TFile * inf0 = new TFile("out/20110812_trkseladdendum/eff_hiOpt2CalCompTrk.root");
  TFile * inf1 = new TFile("out/20110812_trkseladdendum/eff_hiGeneralTrk.root");
  TFile * inf2 = new TFile("out/20110812_trkseladdendum/eff_hiGoodTightTrk.root");

	TProfile * hEffMidEta0 = (TProfile*)inf0->Get("hEff0");
	TProfile * hEffMidEta1 = (TProfile*)inf1->Get("hEff0");
	TProfile * hEffMidEta2 = (TProfile*)inf2->Get("hEff0");
  
	TProfile * hFakMidEta0 = (TProfile*)inf0->Get("hFak0");
	TProfile * hFakMidEta1 = (TProfile*)inf1->Get("hFak0");
	TProfile * hFakMidEta2 = (TProfile*)inf2->Get("hFak0");
	
  // Draw
  TCanvas * c2 = new TCanvas("c2","",500,500);
  c2->SetLogx();
  hEffMidEta0->SetAxisRange(4,119,"X");
  hEffMidEta0->SetAxisRange(0,1.,"Y");
  hEffMidEta0->GetXaxis()->CenterTitle();
  hEffMidEta0->GetYaxis()->CenterTitle();
  hEffMidEta0->SetTitle(";p_{T} (GeV/c); Fake Rate              #epsilon #times Acc         ;");
  hEffMidEta0->SetMarkerStyle(kFullCircle);
  hEffMidEta0->SetMarkerColor(kBlue);
  hEffMidEta0->SetLineColor(kBlue);
  hEffMidEta1->SetMarkerStyle(kFullSquare);
  hEffMidEta1->SetMarkerColor(kRed);
  hEffMidEta1->SetLineColor(kRed);
  hEffMidEta2->SetMarkerStyle(kFullTriangleUp);
  hEffMidEta2->SetMarkerColor(kGreen+2);
  hEffMidEta2->SetLineColor(kGreen+2);
  hEffMidEta0->Draw();
  hEffMidEta1->Draw("sameE");
	hEffMidEta2->Draw("sameE");
  hEffMidEta0->Draw("sameE");
  
  hFakMidEta0->SetMarkerStyle(kOpenCircle);
  hFakMidEta0->SetMarkerColor(kBlue);
  hFakMidEta0->SetLineColor(kBlue);
  hFakMidEta1->SetMarkerStyle(kOpenSquare);
  hFakMidEta1->SetMarkerColor(kRed);
  hFakMidEta1->SetLineColor(kRed);
  hFakMidEta2->SetMarkerStyle(kOpenTriangleUp);
  hFakMidEta2->SetMarkerColor(kGreen+2);
  hFakMidEta2->SetLineColor(kGreen+2);
  hFakMidEta0->Draw("sameE");
  hFakMidEta1->Draw("sameE");
  hFakMidEta2->Draw("sameE");
	
  TLine * l = new TLine(0,1,120,1);
  l->SetLineStyle(2);
  l->Draw();
	
  TLegend *leg = new TLegend(0.4,0.24,0.89,0.42);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hEffMidEta0,"PYTHIA+HYDJET 0-10%","");
  leg->AddEntry(hEffMidEta0,"|#eta|<1","");
  leg->AddEntry(hEffMidEta0,"hiOptCaloComp","p");
	leg->AddEntry(hEffMidEta1,"hiGeneral","p");
	leg->AddEntry(hEffMidEta2,"hiGoodTight","p");
  leg->Draw();
  
  c2->Print("effCompare.gif");
}
