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
#include "../HiForest/HEAD/hiForest.h"
using namespace std;


void EffPPbHydjet(
  float maxEta = 2.4 // 0.8, 2.4
  ){
  HiForest *h = new HiForest("/mnt/hadoop/cms/store/user/vzhukova/Hydjet_merged/mergedFile.root","",cPPb);
  TString tag = "Hydjet_Periph";
//   HiForest *h = new HiForest("/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest_pythia533.root","",cPPb);
//   TString tag = "Pythia";
//   tag+="_npgt15_pi_purityonly";

  TString outdir = "fig/2012.10.15_xcheck_ferenc";
  
  // Track Selection
  TString evtSel = "(phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(track.vz[1])<15)";
//   TString evtSel = "(phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(track.vz[1])<15&&ak5Calo.pthat>7)";
//   TString evtSel = "(phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(track.vz[1])<15&&track.nParticle>15)";
  TString trkSel = Form("abs(trkEta)<%.2f",maxEta);;
  TString pSel = Form("abs(pEta)<%.2f",maxEta);
//   TString pSel = Form("abs(pEta)<%.2f&&abs(pPId)==211",maxEta);

  // higeneral
  TString mtrkQual = pSel + "&&(track.mtrkQual&&track.mtrkPtError/track.mtrkPt<=0.1&&abs(track.mtrkDz1/track.mtrkDzError1)<3&&abs(track.mtrkDxy1/track.mtrkDxyError1)<3)";
  TString trkQual=trkSel+"&&(track.highPurity&&track.trkPtError/track.trkPt<=0.1&&abs(track.trkDz1/track.trkDzError1)<3&&abs(track.trkDxy1/track.trkDxyError1)<3)";
//   TString mtrkQual = pSel + "&&(track.mtrkQual)";
//   TString trkQual=trkSel+"&&(track.highPurity)";
  
  cout << "Event" << endl;
  cout << evtSel << endl;
	cout << "Track Quality selection: " << endl;
	cout << "-- matched simtrk: --" << endl;
	cout << pSel << endl;
	cout << mtrkQual << endl;
	cout << endl << "-- rectrk: --" << endl;
	cout << trkSel << endl;
	cout << trkQual << endl;

  // Bins
//   const Int_t nbins = 9;
//   Double_t bins[nbins+1] = {0.45,1,1.5,2,2.5,3,4,5,7.5,10};

  const Int_t nbins = 20;
  Double_t bins[nbins+1];
  for (Int_t i=0; i<=nbins; ++i) { bins[i] = 0.5+0.2*i; }
  
  // 1D
  TProfile * hEff0 = new TProfile("hEff0",";trk p_{T} (GeV/c);Eff x Acc",nbins,bins);
  TString name = hEff0->GetName();
  TString draw = Form("track.pNRec==1&&%s:track.pPt",mtrkQual.Data());
  TString cut = Form("%s&&%s",evtSel.Data(),pSel.Data());
  cout << endl << name << ": " << draw << endl;
  cout << "Cut: " << cut << endl;
  h->tree->Project(name,draw,cut,"prof");

  TProfile * hFak0 = new TProfile("hFak0",";trk p_{T} (GeV/c);Fake",nbins,bins);
  name = hFak0->GetName();
  draw = Form("track.trkFake:track.trkPt");
  cut = Form("%s&&%s",evtSel.Data(),trkQual.Data());
  cout << endl << name << ": " << draw << endl;
  cout << "Cut: " << cut << endl;
  h->tree->Project(name,draw,cut,"prof");

  // check
  h->tree->Draw("trkPt>>hPt(50,0,10)",cut,"");
  
  // Draw
  TCanvas * c2 = new TCanvas("c2","",500,500);
  hEff0->SetAxisRange(0,10,"X");
  hEff0->SetAxisRange(0,1.2,"Y");
  hEff0->GetXaxis()->CenterTitle();
  hEff0->GetYaxis()->CenterTitle();
  hEff0->SetTitle(";p_{T} (GeV/c); Fake Rate                #epsilon #times Acc              ;");
  hEff0->SetMarkerStyle(kFullCircle);
//   hEff0->SetMarkerColor(kRed);
//   hEff0->SetLineColor(kBlue);
  hEff0->Draw();
    
  hFak0->SetMarkerStyle(kOpenCircle);
//   hFak0->SetMarkerColor(kRed);
//   hFak0->SetLineColor(kBlue);
  hFak0->Draw("sameE");

  TLegend *leg = new TLegend(0.5,0.26,0.89,0.39);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hEff0,tag,"");
  leg->AddEntry(hEff0,Form("|#eta|<%.1f",maxEta),"");
  leg->Draw();

  TLine * l = new TLine(bins[0],1,bins[nbins],1);
  l->SetLineStyle(2);
  l->Draw();

  c2->Print(Form("%s/trkeff_%s_maxEta%.0f.gif",outdir.Data(),tag.Data(),maxEta*10));
  c2->Print(Form("%s/trkeff_%s_maxEta%.0f.pdf",outdir.Data(),tag.Data(),maxEta*10));
  
  TCanvas * c2_2 = new TCanvas("c2_2","",500,500);
  hFak0->SetAxisRange(0,10,"X");
  hFak0->SetAxisRange(0,0.05,"Y");
  hFak0->GetXaxis()->CenterTitle();
  hFak0->GetYaxis()->CenterTitle();
  hFak0->SetTitle(";p_{T} (GeV/c); Fake Rate;");
  hFak0->SetMarkerStyle(kOpenCircle);
  hFak0->Draw("E");
  leg->Draw();	
  c2_2->Print(Form("%s/trkfakezoom_%s_maxEta%.0f.gif",outdir.Data(),tag.Data(),maxEta*10));
  c2_2->Print(Form("%s/trkfakezoom_%s_maxEta%.0f.pdf",outdir.Data(),tag.Data(),maxEta*10));
  
  // 2D
  const Int_t netabins = 12;
  Double_t etabins[netabins+1];
  for (Int_t e=0; e<=netabins; ++e) { etabins[e] = -2.4+4.8/netabins*e; }

  draw = Form("track.pNRec==1&&%s:track.pPt:track.pEta",mtrkQual.Data());
  cut = Form("%s&&%s",evtSel.Data(),pSel.Data());
  cout << "2D" << endl << name << ": " << draw << endl;
  cout << "Cut: " << cut << endl;

  TProfile2D * hEff2D = new TProfile2D("hEff2D","",netabins,etabins,nbins,bins);
  h->tree->Project("hEff2D",draw,cut,"prof");
  
//   TProfile2D * hFak2D = new TProfile2D("hFak2D",";trk p_{T} (GeV/c);Fake",netabins,etabins,nbins,bins);
//   t->Project("hFak2D","trkFake:trkPt:trkEta",Form("%s&&%s&&%s",evtSel.Data(),trkSel2.Data(),trkOptSel.Data()),"prof");
  
  // 2D
  TCanvas * c3 = new TCanvas("c3","",500,500);
  c3->SetRightMargin(0.15);
  hEff2D->SetTitle(";#eta;p_{T} (GeV/c);Eff x Acc");
//   hEff2D->SetAxisRange(,"Y");
  hEff2D->SetAxisRange(0,1.,"Z");
  hEff2D->GetXaxis()->CenterTitle();
  hEff2D->GetYaxis()->CenterTitle();
  hEff2D->GetZaxis()->CenterTitle();
  hEff2D->Draw("colz");
  c3->Print(Form("%s/trkeff2d_%s_maxEta%.0f.gif",outdir.Data(),tag.Data(),maxEta*10));
  c3->Print(Form("%s/trkeff2d_%s_maxEta%.0f.pdf",outdir.Data(),tag.Data(),maxEta*10));
}
