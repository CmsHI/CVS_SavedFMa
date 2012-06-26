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

TString HisClosure(
   TString indir = "fig/06.26_genploop",
   int anaMode=0, // 0=pt, 1=eta, 2=dphi
   int idraw=-1,
   TCanvas ** cOut=0
)
{
   TH1::SetDefaultSumw2();
   TString outdir = indir;
   gSystem->mkdir(outdir,kTRUE);

   bool doCorr=true;
   TString infname = "HisMc_icPu5_trkHPCorr_120_50_2094_eta24_prec1.root";
   TFile * inf = new TFile(indir+"/"+infname);
   
   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   TString evt="0to12";
   TString src=infname;
   src.ReplaceAll(".root","");
   TString tag=Form("hisClos%d_%s_%s",anaMode,src.Data(),evt.Data());

   Compare cmp("cmp","");
   TH1D * hGenp, *hTrk, *hTrkCorr;
   float xmin=0, xmax=0;
   bool doLogx=true, doLogy=true;
   TString genVar, trkVar;
   TCut finalGenSel,finalTrkSel;
   if (anaMode==0) {
      hGenp = (TH1D*)inf->Get("hGenpPt"+evt);
      hTrk = (TH1D*)inf->Get("hTrkPt"+evt);
      hTrkCorr = (TH1D*)inf->Get("hTrkCorrPt"+evt);
      cmp.Legend(0.24,0.23,0.7,0.4);
//       cmp.leg->AddEntry(hTrk,Form("|#eta| < %.1f",maxEta),"");
//       tag += Form("_vs_pt_eta%.0f",maxEta*10);
      tag += Form("_vs_pt");
      xmin=0.5; xmax=179.9;
   } else if (anaMode==1) {
      hGenp = (TH1D*)inf->Get("hGenpEta"+evt);
      hTrk = (TH1D*)inf->Get("hTrkEta"+evt);
      hTrkCorr = (TH1D*)inf->Get("hTrkCorrEta"+evt);
      doLogx=false; doLogy=false;
      cmp.Legend(0.19,0.68,0.65,0.93);
//       cmp.leg->AddEntry(hTrk,Form("%.1f<p_{T}<%.1f GeV/c",ptmin,ptmax),"");
//       tag += Form("vs_eta_pt%.0f",ptmin);
      tag += Form("vs_eta");
      genVar="genpEta"; trkVar="trkEta";
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
//    hGenp->Scale(1./nEvt);
//    hTrk->Scale(1./nEvt);
//    hTrkCorr->Scale(1./nEvt);

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
      
   // Summary
   if (idraw>=0) {
      delete c2;
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

// void ClosureAll()
// {
//    TString outdir = "fig/06.13MptClos";
//    TString tag;
//    int anamode = 2;
//    
//    TCanvas * call[2] = { new TCanvas("call","call",1000,500), new TCanvas("crat","crat",1000,500) };
// //    TCanvas * call[2] = { new TCanvas("call","call",1500,500), new TCanvas("crat","crat",1500,500) };
//    for (int k=0; k<2; ++k) {
// //       call[k]->Divide(6,2);
//       call[k]->Divide(3,2);
//    }
// 
//    for (int i=0; i<nptrange; ++i) {
//       tag = ClosureMptTrk(anamode,i,call,outdir);
//    }
//    
//    for (int k=0; k<2; ++k) {
//       call[k]->cd();
//       call[k]->Print(Form("%s/summary%d_%s.gif",outdir.Data(),k,tag.Data()));
//    }
// }