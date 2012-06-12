#include "TFile.h"
#include "TProfile.h"
#include "TTree.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "commonUtility.h"
#include "HisMath.C"
#include "compare.h"

void TrkClosure()
{
   int anaMode=0; // 0=pt, 1=eta
   bool doCorr=true;
   float maxEta=2.4;
   float ptmin=20,ptmax=200;
   TH1::SetDefaultSumw2();
   TString mod="hitrkEffAnalyzer_MergedGeneral";
   TrackingCorrections trkCorr("itertrkpixtrk",mod);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj30_Forest2_v19.root",30);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj50_Forest2_v19.root",50);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj80_Forest2_v19.root",80);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj120_Forest2_v19.root",120);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj170_Forest2_v19.root",170);
   trkCorr.smoothLevel_ = 1; 	 
   trkCorr.Init();
   
//    TFile * inf = new TFile("/Users/frankma/Dropbox/work/jet_ana_notes/macros/ntout/output-hy18dj80_IterPixTrkv1_v6_xsec_icPu5.root");
//    TFile * inf = new TFile("/Users/frankma/Dropbox/work/jet_ana_notes/macros/ntout/output-hy18dj80_IterPixTrkv1_v8_xsec_icPu5.root");
//    TFile * inf = new TFile("/Users/frankma/Dropbox/work/jet_ana_notes/macros/ntout/output-hy18dj80_IterPixTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root");
//    TFile * inf = new TFile("/Users/frankma/Dropbox/work/jet_ana_notes/macros/ntout/output-dj50_highptv0_xsec_icPu5.root");
//    TFile * inf = new TFile("~/scratch01/work/jet/macros/ntout//output-hy18dj200_iterTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root");
   TFile * inf = new TFile("~/scratch01/work/jet/macros/ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root");
   TTree * t = (TTree*)inf->Get("tgj");
   TString tag="HP";
   
   Compare cmp("pt","");
   TH1D * hGenp, *hTrk, *hTrkCorr;
   if (anaMode==0) {
      hGenp = (TH1D*)trkCorr.ptBin_->Clone("hGenp");
      hTrk = (TH1D*)trkCorr.ptBin_->Clone("hTrk");
      hTrkCorr = (TH1D*)trkCorr.ptBin_->Clone("hTrkCorr");
   } else if (anaMode==1) {
      TH1D * hEtaBin = new TH1D("hEtaBin",";#eta;",12,-2.4,2.4);
//       TH1D * hEtaBin = (TH1D*)trkCorr.etaBin_->Clone("hEtaBin");
//       float etaBins[14] = {-2.4,-2.2,-1.8,-1.4,-1,-0.6,-0.2,0.2,0.6,1,1.4,1.8,2.2,2.4};
//       TH1D * hEtaBin = new TH1D("hEtaBin",";#eta;",13,etaBins);
      hGenp = (TH1D*)hEtaBin->Clone("hGenp");
      hTrk = (TH1D*)hEtaBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hEtaBin->Clone("hTrkCorr");
   }

   TCut sel = "cBin<12&&pt1>120&&pt2>50";
   TCut genpSel = "genpCh!=0&&abs(genpEta)<2.4";
   TCut trkSel = "abs(trkEta)<2.4&&(trkNHit<7||(vtrkQual[][0]))";
//    TCut trkSel = "abs(trkEta)<2.4&&(trkNHit<7||(vtrkQual[][0]&&trkAlgo==4))";
//    TCut trkSel = "abs(trkEta)<2.4&&(trkPt<2||trkAlgo==4&&trkHP)";
//    TCut trkSel = "abs(trkEta)<2.4&&(abs(trkEta)<1.6||trkChi2Norm<0.12)";
   cout << "sel: " << sel << endl;
   if (anaMode==0) {
      t->Draw("genpPt>>hGenp",sel&&genpSel&&Form("abs(genpEta)<%f",maxEta));
      t->Draw("trkPt>>hTrk",sel&&trkSel&&Form("abs(trkEta)<%f",maxEta));
      if (doCorr) t->Draw("trkPt>>hTrkCorr",(sel&&trkSel)*"vtrkWt[][0]");
//       if (doCorr) t->Draw("trkPt>>hTrkCorr",(sel&&trkSel)*"vtrkWt[][1]");
   } else if (anaMode==1) {
      t->Draw("genpEta>>hGenp",sel&&genpSel&&Form("genpPt>=%.f&&genpPt<%.f",ptmin,ptmax));
      t->Draw("trkEta>>hTrk",(sel&&trkSel&&Form("trkPt>=%.f&&trkPt<%.f",ptmin,ptmax)));
      if (doCorr) t->Draw("trkEta>>hTrkCorr",(sel&&trkSel&&Form("trkPt>=%.f&&trkPt<%.f",ptmin,ptmax))*"vtrkWt[][0]");
   }
   
   TCanvas * c2 = new TCanvas("c2","c2",800,400);
   c2->Divide(2,1);
   c2->cd(1);
   if (anaMode==0) {
      gPad->SetLogx();
      gPad->SetLogy();
      hGenp->SetAxisRange(0.5,89.9,"X");
   }
   hGenp->SetLineColor(2);
   hTrk->SetMarkerStyle(kOpenCircle);
   normHist(hGenp,-1,true);
   normHist(hTrk,-1,true);
   normHist(hTrkCorr,-1,true);
   if (anaMode==0) {
      gPad->SetLogx();
      gPad->SetLogy();
      hGenp->SetAxisRange(0.5,89.9,"X");
   } else if (anaMode==1) {
      hGenp->SetAxisRange(0,hGenp->GetMaximum()*1.4,"Y");
   }
   hGenp->Draw("hist");
   hTrk->Draw("sameE");
   if (doCorr) hTrkCorr->Draw("sameE");
   if (anaMode==0) {
      cmp.Legend(0.24,0.23,0.7,0.4);
      cmp.leg->AddEntry(hTrk,Form("|#eta| < %.1f",maxEta),"");
   } else {
      cmp.Legend(0.19,0.76,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f < p_{T} < %.1f GeV/c",ptmin,ptmax),"");
   }
   cmp.leg->AddEntry(hTrk,"Rec. Trk","p");
   cmp.leg->AddEntry(hGenp,"Gen. Particle","l");
   cmp.leg->Draw();

   c2->cd(2);
   TH1D * hTrkRat = (TH1D*)hTrk->Clone("hTrkRat");
   hTrkRat->Divide(hGenp);
   TH1D * hTrkCorrRat = (TH1D*)hTrkCorr->Clone("hTrkCorrRat");
   if (doCorr) hTrkCorrRat->Divide(hGenp);
   if (anaMode==0) {
      gPad->SetLogx();
      hTrkRat->SetAxisRange(0.5,89.9,"X");
   }
   hTrkRat->SetAxisRange(0.,1.4,"Y");
   hTrkRat->Draw("E");
   if (doCorr) hTrkCorrRat->Draw("sameE");
   c2->Print(Form("%sClosure_%d_pt%.0f_eta%.0f.gif",tag.Data(),anaMode,ptmin,maxEta*10));
   
//    TCanvas * c3 = new TCanvas("c3","c3",800,400);
//    TH2D * hTrkWtvPt = new TH2D("hTrkWtvPt","",80,0,80,50,0,3);
//    TH2D * hTrkWtvEta = new TH2D("hTrkWtvEta","",13,-2.6,2.6,50,0,3);
//    c3->Divide(2,1);
//    c3->cd(1);
//    t->Draw("trkWt:trkPt>>hTrkWtvPt",sel,"box");
//    hTrkWtvPt->ProfileX()->Draw("sameE");
//    c3->cd(2);
//    t->Draw("trkWt:trkEta>>hTrkWtvEta",sel,"box");
//    hTrkWtvEta->ProfileX()->Draw("sameE");
}
