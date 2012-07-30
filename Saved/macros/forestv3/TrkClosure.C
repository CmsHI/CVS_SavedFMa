#include "TFile.h"
#include "TProfile.h"
#include "TTree.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "../HiForest_HEAD/TrackingCorrectionsv6.h"
#include "TLegend.h"
#include "TSystem.h"
#include "commonUtility.h"
#include "HisMath.C"
#include "compare.h"

void TrkClosure(
   int anaMode=0, // 0=pt, 1=eta, 2=dphi, 3=xi
   TString outdir="fig/07.28_closure"
)
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   bool doCorr=true;
   float maxEta=2.4;
   float ptmin=0.5,ptmax=200;
   TString mod="Forest2_MergedGeneral";
   TrackingCorrections trkCorr("Forest2STAv14",mod);
   trkCorr.AddSample("trkcorr/IterTrkCorrv14XSec/IterTrkCorrv14XSec_hy18dj80to100_akPu3Calo_100_-1_-1000_genJetMode0.root",80);
   trkCorr.Init();
   
//  TFile * inf = new TFile("output_hy18dj80_fv27_ntv1_corrv13_akPu3PF_100_-1_-1000_saveTrk1_jmin100_tmin1_genJetMode0.root");
//   TFile * inf = new TFile("output_hy18dj100_fv27_ntv1_corrv13_akPu3PF_100_-1_-1000_saveTrk1_jmin100_tmin1_genJetMode0.root");
  TFile * inf = new TFile("output_hy18dj80_fv27_ntv1_corrv14_akPu3PF_100_-1_-1000_saveTrk1_jmin100_tmin1_genJetMode0.root");
//    TFile * inf = new TFile("output_sigdj80_fv27_ntv1_akPu3PF_100_-1_-1000_saveTrk1_jmin100_tmin1_genJetMode0_corrhi.root");
//    TFile * inf = new TFile("output_sigdj80_fv27_ntv1_akPu3PF_100_-1_-1000_saveTrk1_jmin100_tmin1_genJetMode0_corrppv14jetfinexsec.root");
   TTree * t = (TTree*)inf->Get("tgj");
   
   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   TString tag=Form("trkClos%d_0to30_jpt100_incone",anaMode);
   TCut sel = "cBin<12&&pt1>100";
//    TCut sel = "pt1>100";
  TCut genpSel = "";
  TCut trkSel = "";
//     TCut genpSel = "simAsso==1||simAsso==2";
//     TCut trkSel = "trkAsso==1||trkAsso==2";
   TString trkWt = "trkWt";

   float nEvt = t->GetEntries(sel);
   cout << sel << ": " << nEvt << " events" << endl;
   
   Compare cmp("cmp","");
   TH1D * hGenp, *hTrk, *hTrkCorr;
   float xmin=1, xmax=119.9;
   bool doLogx=true, doLogy=true;
   TString genVar, trkVar;
   TCut finalGenSel,finalTrkSel;
   if (anaMode==0) {
      TH1D * hPtBin = (TH1D*)trkCorr.ptBin_->Clone("hPtBin");
      hPtBin->Reset();
      hPtBin->SetAxisRange(0.5,179.9,"X");
      hGenp = (TH1D*)trkCorr.ptBin_->Clone("hGenp");
      hTrk = (TH1D*)trkCorr.ptBin_->Clone("hTrk");
      hTrkCorr = (TH1D*)trkCorr.ptBin_->Clone("hTrkCorr");
      cmp.Legend(0.24,0.23,0.7,0.4);
      cmp.leg->AddEntry(hTrk,Form("|#eta| < %.1f",maxEta),"");
      tag += Form("_eta%.0f",maxEta*10);
      genVar="simPt"; trkVar="trkPt";
      finalGenSel = sel&&genpSel&&Form("abs(simEta)<%.2f",maxEta);
      finalTrkSel = sel&&trkSel&&Form("abs(trkEta)<%.2f",maxEta);
   } else if (anaMode==1) {
      TH1D * hEtaBin = (TH1D*)trkCorr.etaBin_->Clone("hEtaBin");
      hEtaBin->Reset();
      hGenp = (TH1D*)hEtaBin->Clone("hGenp");
      hTrk = (TH1D*)hEtaBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hEtaBin->Clone("hTrkCorr");
      doLogx=false; doLogy=false;
      cmp.Legend(0.19,0.76,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f < p_{T} < %.1f GeV/c",ptmin,ptmax),"");
      tag += Form("_pt%.0f",ptmin);
      genVar="simEta"; trkVar="trkEta";
      finalGenSel = sel&&genpSel&&Form("simPt>=%.2f&&simPt<%.2f",ptmin,ptmax);
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
      genVar="acos(cos(simPhi-phi1))"; trkVar="acos(cos(trkPhi-phi1))";
      finalGenSel = sel&&genpSel&&Form("simPt>=%.2f&&simPt<%.2f",ptmin,ptmax);
      finalTrkSel = sel&&trkSel&&Form("trkPt>=%.2f&&trkPt<%.2f",ptmin,ptmax);
   } else if (anaMode==3) {
      TH1D * hBin = new TH1D("hBin",";#xi=1/ln(z)",12,0,6);
      hGenp = (TH1D*)hBin->Clone("hGenp");
      hTrk = (TH1D*)hBin->Clone("hTrk");
      hTrkCorr = (TH1D*)hBin->Clone("hTrkCorr");
      doLogx=false; doLogy=false;
      cmp.Legend(0.19,0.76,0.65,0.93);
      cmp.leg->AddEntry(hTrk,Form("%.1f < p_{T} < %.1f GeV/c",ptmin,ptmax),"");
      tag += Form("_pt%.0f",ptmin);
      genVar="log(pt1/simPt)"; trkVar="log(pt1/trkPt)";
      finalGenSel = sel&&genpSel&&Form("simPt>=%.2f&&simPt<%.2f&&(simAsso==1||simAsso==2)",ptmin,ptmax);
      finalTrkSel = sel&&trkSel&&Form("trkPt>=%.2f&&trkPt<%.2f&&(trkAsso==1||trkAsso==2)",ptmin,ptmax);
   }
   if (anaMode>0) {
      xmin= hGenp->GetBinLowEdge(1);
      xmax= hGenp->GetBinLowEdge(hGenp->GetNbinsX());
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
   else hGenp->SetAxisRange(0,hGenp->GetMaximum()*1.4,"Y");
   if (anaMode==0) hGenp->SetAxisRange(xmin,xmax,"X");
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
   if (anaMode==0) hTrkRat->SetAxisRange(xmin,xmax,"X");
   hTrkRat->SetAxisRange(0.,2,"Y");
   TLine * l1 = new TLine(xmin,1,xmax,1);
   l1->SetLineStyle(2);
   hTrkRat->Draw("E");
   l1->Draw();
   if (doCorr) hTrkCorrRat->Draw("sameE");

   c2->Print(Form("%s/%s.gif",outdir.Data(),tag.Data()));   
   c2->Print(Form("%s/%s.pdf",outdir.Data(),tag.Data()));   
   c2->Print(Form("%s/%s.C",outdir.Data(),tag.Data()));   
}
