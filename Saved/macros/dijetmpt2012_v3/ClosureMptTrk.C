#include "TFile.h"
#include "TProfile.h"
#include "TTree.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "TSystem.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "commonUtility.h"
#include "HisMath.C"
#include "compare.h"
#include "loopMpt.h"

TString ClosureMptTrk(
   int anaMode=0, // 0=pt, 1=eta, 2=dphi
   int idraw=-1,
   TCanvas ** cOut=0,
   TString outdir = "06.26_TrkClos"
)
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   TString mod="hitrkEffAnalyzer_MergedGeneral";
   TrackingCorrections trkCorr("Forest2_v19",mod);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj30_Forest2_v19.root",30);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj50_Forest2_v19.root",50);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj80_Forest2_v19.root",80);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj120_Forest2_v19.root",120);
   trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj170_Forest2_v19.root",170);
   trkCorr.smoothLevel_ = 1; 	 
   trkCorr.Init();

//    TFile * inf = new TFile("~/scratch01/work/jet/macros/ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root");
   TFile * inf = new TFile("../ntout/output-hy18dj80_Forest2v21_v3_allTrks_simtrk_jpt120_xsec_icPu5.root");
   TTree * t = (TTree*)inf->Get("tgj");
   
   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   bool doCorr=true;
   float maxEta=2.4;
   float ptmin=20,ptmax=200;
   if (idraw>=0) {
      ptmin=ptranges[idraw];
      ptmax=ptranges[idraw+1];
   }
   
   TString tag=Form("trkClos%d",anaMode);

   TCut sel = "cBin<12&&pt1>120&&abs(eta1)<1.6&&pt2>50&&abs(eta2)<1.6&&abs(dphi)>2.1";
//    sel = sel && "Aj>0.2";

   TCut genpSel = "";
   TCut trkSel = "(trkAlgo<4||vtrkQual[][0])";
   TString trkWt = "trkWt";

//    TCut trkSel = "(trkAlgo<4||vtrkQual[][0])&&!trkIsFake";
//    TString trkWt = "trkWt/(1-trkFak)";
//    TCut genpSel = "simHasRec";
//    TString trkWt = "trkEff";

   float nEvt = t->GetEntries(sel);
   cout << sel << ": " << nEvt << " events" << endl;
   
   Compare cmp("cmp","");
   TH1D * hGenp, *hTrk, *hTrkCorr;
   float xmin=0, xmax=0, ymin=0, ymax=0;
   bool doLogx=true, doLogy=true;
   TString genVar, trkVar;
   TCut finalGenSel,finalTrkSel;
   if (anaMode==0) {
      TH1D * hPtBin = (TH1D*)trkCorr.ptBin_->Clone("hPtBin");
      hGenp = (TH1D*)trkCorr.ptBin_->Clone("hGenp");
      hTrk = (TH1D*)trkCorr.ptBin_->Clone("hTrk");
      hTrkCorr = (TH1D*)trkCorr.ptBin_->Clone("hTrkCorr");
      cmp.Legend(0.24,0.23,0.7,0.4);
      cmp.leg->AddEntry(hTrk,Form("|#eta| < %.1f",maxEta),"");
      tag += Form("_vs_pt_eta%.0f",maxEta*10);
      genVar="simPt"; trkVar="trkPt";
      finalGenSel = sel&&genpSel&&Form("abs(simEta)<%.2f",maxEta);
      finalTrkSel = sel&&trkSel&&Form("abs(trkEta)<%.2f",maxEta);
      xmin=0.5; xmax=179.9;
   } else if (anaMode==1) {
      TH1D * hEtaBin = (TH1D*)trkCorr.etaBin_->Clone("hEtaBin");
      hGenp = (TH1D*)hEtaBin->Clone("hGenp");
      hTrk = (TH1D*)hEtaBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hEtaBin->Clone("hTrkCorr");
      doLogx=false; doLogy=false;
      cmp.Legend(0.19,0.68,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f<p_{T}<%.1f GeV/c",ptmin,ptmax),"");
      tag += Form("vs_eta_pt%.0f",ptmin);
      genVar="simEta"; trkVar="trkEta";
      finalGenSel = sel&&genpSel&&Form("simPt>=%.2f&&simPt<%.2f",ptmin,ptmax);
      finalTrkSel = sel&&trkSel&&Form("trkPt>=%.2f&&trkPt<%.2f",ptmin,ptmax);
      ymin=0; ymax=1;
   } else if (anaMode==2) {
      TH1D * hBin = new TH1D("hBin",";#Delta#phi(trk,jet1); # per evt",25,0,3.14159);
      hGenp = (TH1D*)hBin->Clone("hGenp");
      hTrk = (TH1D*)hBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hBin->Clone("hTrkCorr");
      doLogx=false; doLogy=true;
      cmp.Legend(0.19,0.68,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f<p_{T}<%.1f GeV/c",ptmin,ptmax),"");
      tag += Form("_vs_dphi_pt%.0f",ptmin);
      genVar="acos(cos(simPhi-phi1))"; trkVar="acos(cos(trkPhi-phi1))";
      finalGenSel = sel&&genpSel&&Form("simPt>=%.2f&&simPt<%.2f",ptmin,ptmax);
      finalTrkSel = sel&&trkSel&&Form("trkPt>=%.2f&&trkPt<%.2f",ptmin,ptmax);
   }

   // Legend
   if (idraw<1) {
      cmp.leg->AddEntry(hTrk,"Raw Trk","p");
      if (doCorr) cmp.leg->AddEntry(hTrkCorr,"Corr. Trk","p");
      cmp.leg->AddEntry(hGenp,"Gen. Particle","l");
   }

   //////////////////////////////////////////
   // Run Analysis
   //////////////////////////////////////////
   cout << genVar << ": " << finalGenSel << endl;
   cout << trkVar << ": " << finalTrkSel << endl;
   if (doCorr) cout << "Corr: " << (finalTrkSel)*trkWt << endl;
   t->Draw(genVar+">>hGenp",finalGenSel,"goff");
   t->Draw(trkVar+">>hTrk",finalTrkSel,"goff");
   if (doCorr) t->Draw(trkVar+">>hTrkCorr",(finalTrkSel)*trkWt,"goff");
   
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
   if (xmax>xmin) hGenp->SetAxisRange(xmin,xmax,"X");
   if (ymax>ymin) hGenp->SetAxisRange(ymin,ymax,"Y");
   hGenp->Draw("hist");
   hTrk->Draw("sameE");
   if (doCorr) hTrkCorr->Draw("sameE");
   cmp.leg->Draw();

   c2->cd(2);
   TH1D * hGenRat = (TH1D*)hGenp->Clone("hGenRat");
   hGenRat->Divide(hGenp);
   TH1D * hTrkRat = (TH1D*)hTrk->Clone("hTrkRat");
   hTrkRat->Divide(hGenp);
   TH1D * hTrkCorrRat = (TH1D*)hTrkCorr->Clone("hTrkCorrRat");
   if (doCorr) hTrkCorrRat->Divide(hGenp);
   if (doLogx) gPad->SetLogx();
   if (xmax>xmin) hTrkRat->SetAxisRange(xmin,xmax,"X");
   hTrkRat->SetAxisRange(0.,2,"Y");
   hTrkRat->SetYTitle("Ratio");
   hTrkRat->Draw("E");
   hGenRat->Draw("same hist");
   hTrkRat->Draw("sameE");
   if (doCorr) hTrkCorrRat->Draw("sameE");

   c2->Print(Form("%s/%s.gif",outdir.Data(),tag.Data()));
   
   if (idraw>=0) delete c2;
   
   // Summary
   if (idraw>=0) {
      cOut[0]->cd(idraw+1);
      if (doLogx) gPad->SetLogx();
      if (doLogy) gPad->SetLogy();
      hGenp->Draw("hist");
      hTrk->Draw("sameE");
      if (doCorr) hTrkCorr->Draw("sameE");
      cmp.leg->Draw();

      cOut[1]->cd(idraw+1);
      if (doLogx) gPad->SetLogx();
      hTrkRat->Draw("E");
      hGenRat->Draw("same hist");
      hTrkRat->Draw("sameE");
      if (doCorr) hTrkCorrRat->Draw("sameE");      
      cmp.leg->Draw();
   }
   
   return tag;
}

void ClosureAll()
{
   TString outdir = "fig/06.13MptClos";
   TString tag;
   int anamode = 2;
   
   TCanvas * call[2] = { new TCanvas("call","call",1000,500), new TCanvas("crat","crat",1000,500) };
//    TCanvas * call[2] = { new TCanvas("call","call",1500,500), new TCanvas("crat","crat",1500,500) };
   for (int k=0; k<2; ++k) {
//       call[k]->Divide(6,2);
      call[k]->Divide(3,2);
   }

   for (int i=0; i<nptrange; ++i) {
      tag = ClosureMptTrk(anamode,i,call,outdir);
   }
   
   for (int k=0; k<2; ++k) {
      call[k]->cd();
      call[k]->Print(Form("%s/summary%d_%s.gif",outdir.Data(),k,tag.Data()));
   }
}