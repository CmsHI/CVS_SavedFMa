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

void TrkClosure(
   int anaMode=0 // 0=pt, 1=eta, 2=dphi
)
{
   bool doCorr=true;
   float maxEta=2.4;
   float ptmin=8,ptmax=200;
   TH1::SetDefaultSumw2();
   TString mod="hitrkEffAnalyzer_MergedGeneral";
   TrackingCorrections trkCorr("Forest2_v19",mod);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj30_Forest2_v19.root",30);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj50_Forest2_v19.root",50);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj80_Forest2_v19.root",80);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj120_Forest2_v19.root",120);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj170_Forest2_v19.root",170);
   trkCorr.smoothLevel_ = 1; 	 
   trkCorr.Init();
   
   TFile * inf = new TFile("~/scratch01/work/jet/macros/ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root");
   TTree * t = (TTree*)inf->Get("tgj");
   
   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   TString tag=Form("trkClos%d_Aj3",anaMode);
   TCut sel = "cBin<12&&pt1>120&&pt2>50&&abs(dphi)>2.1&&Aj>0.33";
   TCut genpSel = "genpCh!=0";
   TCut trkSel = "trkNHit<8||vtrkQual[][0]";
   TString trkWt = "vtrkWt[][0]";
//    TCut trkSel = "trkNHit<8||(vtrkQual[][0]&&trkAlgo==4)";
//    TString trkWt = "vtrkWt[][1]";
//    TCut trkSel = "";
//    TString trkWt = "vtrkWt[][2]";

   float nEvt = t->GetEntries(sel);
   cout << sel << ": " << nEvt << " events" << endl;
   
   Compare cmp("cmp","");
   TH1D * hGenp, *hTrk, *hTrkCorr;
//    float xmin=0.5, xmax=179.9;
   bool doLogx=true, doLogy=true;
   TString genVar, trkVar;
   TCut finalGenSel,finalTrkSel;
   if (anaMode==0) {
      TH1D * hPtBin = (TH1D*)trkCorr.ptBin_->Clone("hPtBin");
      hPtBin->SetAxisRange(0.5,179.9,"X");
      hGenp = (TH1D*)trkCorr.ptBin_->Clone("hGenp");
      hTrk = (TH1D*)trkCorr.ptBin_->Clone("hTrk");
      hTrkCorr = (TH1D*)trkCorr.ptBin_->Clone("hTrkCorr");
      cmp.Legend(0.24,0.23,0.7,0.4);
      cmp.leg->AddEntry(hTrk,Form("|#eta| < %.1f",maxEta),"");
      tag += Form("_eta%.0f",maxEta*10);
      genVar="genpPt"; trkVar="trkPt";
      finalGenSel = sel&&genpSel&&Form("abs(genpEta)<%.2f",maxEta);
      finalTrkSel = sel&&trkSel&&Form("abs(trkEta)<%.2f",maxEta);
   } else if (anaMode==1) {
      TH1D * hEtaBin = (TH1D*)trkCorr.etaBin_->Clone("hEtaBin");
      hGenp = (TH1D*)hEtaBin->Clone("hGenp");
      hTrk = (TH1D*)hEtaBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hEtaBin->Clone("hTrkCorr");
      doLogx=false; doLogy=false;
      cmp.Legend(0.19,0.76,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f < p_{T} < %.1f GeV/c",ptmin,ptmax),"");
      tag += Form("_pt%.0f",ptmin);
      genVar="genpEta"; trkVar="trkEta";
      finalGenSel = sel&&genpSel&&Form("genpPt>=%.2f&&genpPt<%.2f",ptmin,ptmax);
      finalTrkSel = sel&&trkSel&&Form("trkPt>=%.2f&&trkPt<%.2f",ptmin,ptmax);
   } else if (anaMode==2) {
      TH1D * hBin = new TH1D("hBin",";#Delta#phi(trk,jet1)",25,0,3.14159);
      hGenp = (TH1D*)hBin->Clone("hGenp");
      hTrk = (TH1D*)hBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hBin->Clone("hTrkCorr");
      doLogx=false; doLogy=true;
      cmp.Legend(0.19,0.76,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f < p_{T} < %.1f GeV/c",ptmin,ptmax),"");
      tag += Form("_pt%.0f",ptmin);
      genVar="acos(cos(genpPhi-phi1))"; trkVar="acos(cos(trkPhi-phi1))";
      finalGenSel = sel&&genpSel&&Form("genpPt>=%.2f&&genpPt<%.2f",ptmin,ptmax);
      finalTrkSel = sel&&trkSel&&Form("trkPt>=%.2f&&trkPt<%.2f",ptmin,ptmax);
   }

   // Legend
   cmp.leg->AddEntry(hTrk,"Raw Trk","p");
   if (doCorr) cmp.leg->AddEntry(hTrkCorr,"Corr. Trk","p");
   cmp.leg->AddEntry(hGenp,"Gen. Particle","l");

   //////////////////////////////////////////
   // Run Analysis
   //////////////////////////////////////////
   cout << genVar << ": " << finalGenSel << endl;
   cout << trkVar << ": " << finalTrkSel << endl;
   if (doCorr) cout << "Corr: " << (finalTrkSel)*trkWt << endl;
   t->Draw(genVar+">>hGenp",finalGenSel,"goff");
   t->Draw(trkVar+">>hTrk",finalTrkSel,"goff");
   if (doCorr) t->Draw(trkVar+">>hTrkCorr",(finalTrkSel)*trkWt);
   
   hGenp->Scale(1./nEvt);
   hTrk->Scale(1./nEvt);
   hTrkCorr->Scale(1./nEvt);

   //////////////////////////////////////////
   // Draw
   //////////////////////////////////////////
   TCanvas * c2 = new TCanvas("c2","c2",800,400);
   c2->Divide(2,1);
   c2->cd(1);
   hGenp->SetLineColor(2);
   hTrk->SetMarkerStyle(kOpenCircle);
   normHist(hGenp,-1,true);
   normHist(hTrk,-1,true);
   normHist(hTrkCorr,-1,true);
   if (doLogx) gPad->SetLogx();
   if (doLogy) gPad->SetLogy();
//    hGenp->SetAxisRange(xmin,xmax,"X");
   hGenp->Draw("hist");
   hTrk->Draw("sameE");
   if (doCorr) hTrkCorr->Draw("sameE");
   cmp.leg->Draw();

   c2->cd(2);
   TH1D * hTrkRat = (TH1D*)hTrk->Clone("hTrkRat");
   hTrkRat->Divide(hGenp);
   TH1D * hTrkCorrRat = (TH1D*)hTrkCorr->Clone("hTrkCorrRat");
   if (doCorr) hTrkCorrRat->Divide(hGenp);
   if (doLogx) gPad->SetLogx();
//    hTrkRat->SetAxisRange(xmin,xmax,"X");
   hTrkRat->SetAxisRange(0.,2,"Y");
   hTrkRat->Draw("E");
   if (doCorr) hTrkCorrRat->Draw("sameE");

   c2->Print(Form("%s.gif",tag.Data()));
   
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
