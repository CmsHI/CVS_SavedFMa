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


void Eff(){
  TFile * inf = new TFile("../trees/hypyuq170-393-trkeff_itertrk-v5/merge/all_1.root");
  //TFile * inf = new TFile("../trees/hymb-393-trkeff_itertrk-v5.root");
  TTree * t = (TTree*)inf->Get("anaTrack/trackTree");
  t->AddFriend("icPu5JetAnalyzer/t");
  
  // Track Selection
  TString evtSel = "bin<4";
  TString trkSel = "abs(trkEta)<1";
  TString pSel = "abs(pEta)<1";
  TString trkSel2 = "abs(trkEta)<2.4";
  TString pSel2 = "abs(pEta)<2.4";
  // higeneral
  TString mtrkHgtSel = "(mtrkPtError/mtrkPt<=0.06&&mtrkNHit>=13&&mtrkChi2/mtrkNdof/mtrkNdof<=0.15&&abs(mtrkDz1/mtrkDzError1)<3&&abs(mtrkDxy1/mtrkDxyError1)<3)";
  TString trkHgtSel="(trkPtError/trkPt<=0.06&&trkNHit>=13&&trkChi2/trkNdof/trkNlayer<=0.15&&abs(trkDz1/trkDzError1)<3&&abs(trkDxy1/trkDxyError1)<3)";
	//TString mtrkOptSel = mtrkHgtSel;
	//TString trkOptSel = trkHgtSel;
  // Optinimzed
	TString mtrkOptSel = Form("(%s||mtrkPt>=30)",mtrkHgtSel.Data());
	TString trkOptSel = Form("(%s||trkPt>=30)",trkHgtSel.Data());
  TString mtrkLooseSel = "(mtrkPtError/mtrkPt<=0.1&&mtrkNHit>=10&&mtrkChi2/mtrkNdof/mtrkNdof<=0.2&&abs(mtrkDz1/mtrkDzError1)<8&&abs(mtrkDxy1/mtrkDxyError1)<8)";
  TString trkLooseSel="(trkPtError/trkPt<=0.1&&trkNHit>=10&&trkChi2/trkNdof/trkNlayer<=0.2&&abs(trkDz1/trkDzError1)<8&&abs(trkDxy1/trkDxyError1)<8)";

  mtrkOptSel = mtrkOptSel + Form("&&(mtrkPt<30||(mtrkPfCandPt/mtrkPt>=0.15&&%s)||(mtrkPfCandPt/mtrkPt<0.15&&%s))",mtrkLooseSel.Data(),mtrkHgtSel.Data());
  trkOptSel = trkOptSel + Form("&&(trkPt<30||(pfCandPt/trkPt>=0.15&&%s)||(pfCandPt/trkPt<0.15&&%s))",trkLooseSel.Data(),trkHgtSel.Data());
  
	cout << "Track Quality selection: " << endl;
	cout << "-- matched simtrk: --" << endl;
	cout << mtrkOptSel << endl;
	cout << endl << "-- rectrk: --" << endl;
	cout << trkOptSel << endl;

  //TCanvas * cchk0 = new TCanvas("cchk0","cchk0",500,500);
  //t->Draw("bin>>hCent(40,0,40)",evtSel);

  // Bins
  //Int_t nbins = 17;
  //Float_t bins[nbins+1] = {0,1,2,4,8,12,16,20,25,30,35,40,50,60,70,80,95,110};
  const Int_t nbins = 20;
  Double_t bins[nbins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120,180,300};
  //Int_t nbins = 11;
  //Float_t bins[nbins+1] = {4,5,7.5,10,12,14,16,20,24,30,40,100};
  
  const Int_t netabins = 12;
  Double_t etabins[netabins+1];
  for (Int_t e=0; e<=netabins; ++e) { etabins[e] = -2.4+4.8/netabins*e; }
  
  // 1D
  TProfile * hEff0 = new TProfile("hEff0",";trk p_{T} (GeV/c);Eff",nbins,bins);
  TProfile * hEff2 = new TProfile("hEff2",";trk p_{T} (GeV/c);Eff",nbins,bins);
  t->Project("hEff0",Form("pNRec>0&&%s:pPt",mtrkOptSel.Data()),Form("%s&&%s",evtSel.Data(),pSel.Data()),"prof");
  t->Project("hEff2",Form("pNRec>0&&%s:pPt",mtrkOptSel.Data()),Form("%s&&%s",evtSel.Data(),pSel2.Data()),"prof");

  TProfile * hFak0 = new TProfile("hFak0",";trk p_{T} (GeV/c);Fake",nbins,bins);
  TProfile * hFak2 = new TProfile("hFak2",";trk p_{T} (GeV/c);Fake",nbins,bins);
  t->Project("hFak0","trkFake:trkPt",Form("%s&&%s&&%s",evtSel.Data(),trkSel.Data(),trkOptSel.Data()),"prof");
  t->Project("hFak2","trkFake:trkPt",Form("%s&&%s&&%s",evtSel.Data(),trkSel2.Data(),trkOptSel.Data()),"prof");

  // 2D
  TProfile2D * hEff2D = new TProfile2D("hEff2D",";trk p_{T} (GeV/c);Eff",netabins,etabins,nbins,bins);
  t->Project("hEff2D",Form("pNRec>0&&%s:pPt:pEta",mtrkOptSel.Data()),Form("%s&&%s",evtSel.Data(),pSel2.Data()),"prof");
  
  TProfile2D * hFak2D = new TProfile2D("hFak2D",";trk p_{T} (GeV/c);Fake",netabins,etabins,nbins,bins);
  t->Project("hFak2D","trkFake:trkPt:trkEta",Form("%s&&%s&&%s",evtSel.Data(),trkSel2.Data(),trkOptSel.Data()),"prof");
  
  // 2D
  TCanvas * c3 = new TCanvas("c3","",550,500);
  c3->SetRightMargin(0.15);
  hEff2D->SetTitle(";#eta;p_{T} (GeV/c);#epsilon");
  hEff2D->SetAxisRange(4,119,"Y");
  hEff2D->SetAxisRange(0,1.,"Z");
  hEff2D->GetXaxis()->CenterTitle();
  hEff2D->GetYaxis()->CenterTitle();
  hEff2D->GetZaxis()->CenterTitle();
  hEff2D->Draw("colz");
  c3->Print("eff2D.gif");
  
  TCanvas * c4 = new TCanvas("c4","",550,500);
  c4->SetRightMargin(0.15);
  hFak2D->SetTitle(";#eta;p_{T} (GeV/c);Fake Rate");
  hFak2D->SetAxisRange(4,119,"Y");
  hFak2D->SetAxisRange(0,1.,"Z");
  hFak2D->GetXaxis()->CenterTitle();
  hFak2D->GetYaxis()->CenterTitle();
  hFak2D->GetZaxis()->CenterTitle();
  hFak2D->Draw("colz");
  c4->Print("fak2D.gif");
	
  // Draw
  TCanvas * c2 = new TCanvas("c2","",500,500);
  c2->SetLogx();
  hEff0->SetAxisRange(4,119,"X");
  hEff0->SetAxisRange(0,1.,"Y");
  hEff0->GetXaxis()->CenterTitle();
  hEff0->GetYaxis()->CenterTitle();
  hEff0->SetTitle(";p_{T} (GeV/c); Fake Rate              #epsilon #times Acc         ;");
  hEff0->SetMarkerStyle(kFullCircle);
  hEff0->SetMarkerColor(kBlue);
  hEff0->SetLineColor(kBlue);
  hEff2->SetMarkerStyle(kFullSquare);
  hEff2->SetMarkerColor(kRed);
  hEff2->SetLineColor(kRed);
  hEff0->Draw();
  hEff2->Draw("sameE");
  
  hFak0->SetMarkerStyle(kOpenCircle);
  hFak0->SetMarkerColor(kBlue);
  hFak0->SetLineColor(kBlue);
  hFak2->SetMarkerStyle(kOpenSquare);
  hFak2->SetMarkerColor(kRed);
  hFak2->SetLineColor(kRed);
  hFak0->Draw("sameE");
  hFak2->Draw("sameE");
	
  TLine * l = new TLine(0,1,120,1);
  l->SetLineStyle(2);
  l->Draw();
	
  TLegend *leg = new TLegend(0.5,0.26,0.89,0.39);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hEff0,"|#eta|<1","p");
  leg->AddEntry(hEff2,"|#eta|<2.4","p");
  leg->Draw();
  
  c2->Print("eff.gif");
}
