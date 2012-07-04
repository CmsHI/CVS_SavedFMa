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

void HisClosure(
//    string infpath = "fig/06.26_tree/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec3.root",
   string infpath = "fig/07.03_mithig/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec0_resc0.root",
   int anaMode=10, // 0=pt, 1=eta, 2=dphi
   int iStudy=0,
   int jStudy=0,
   TString legTitle=""
)
{
   TH1::SetDefaultSumw2();

   bool doCorr=true;
   TFile * inf = new TFile(infpath.c_str());
   
   TString outdir = infpath.substr(0, infpath.find_last_of('/'));
   cout << "Output: " << outdir << endl;
   gSystem->mkdir(outdir,kTRUE);

   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   TString evt="0to12";
   TString src=infpath.substr(infpath.find_last_of('/')+1);
   src.ReplaceAll(".root","");
   TString tag=Form("hisClos%d_%s_%s",anaMode,src.Data(),evt.Data());
   bool doDiff=true;
   
   // MPT Closure study
   if (anaMode>=10) tag+=Form("_st%d_%d",iStudy,jStudy);

   //////////////////////////////////////////
   // Get Normalization
   //////////////////////////////////////////
   TH1D * hNorm = (TH1D*)inf->Get("hCentrality_"+evt);
   if (anaMode>=10) hNorm = (TH1D*)inf->Get(Form("hAj_%d_%d_%s",iStudy,jStudy,evt.Data()));
   if (!hNorm) {
      cout << "bad normalization file" << endl;
      exit(0);
   }
   float nEvt = hNorm->GetEntries();
   cout << "=============================================" << endl;
   cout << "nEvt = " << nEvt << endl;
   cout << "=============================================" << endl;

   //////////////////////////////////////////
   // Get Distributions
   //////////////////////////////////////////
   Compare cmp("cmp","");
   TH1D * hGenp, *hTrk, *hTrkCorr;
   float xmin=0, xmax=0, ymin=0, ymax=0;
   bool doLogx=true, doLogy=true;
   TCut finalGenSel,finalTrkSel;
   if (anaMode==0) {
      hGenp = (TH1D*)inf->Get("hGenpPt_"+evt);
      hTrk = (TH1D*)inf->Get("hTrkPt_"+evt);
      hTrkCorr = (TH1D*)inf->Get("hTrkCorrPt_"+evt);
      cmp.Legend(0.24,0.23,0.7,0.4);
//       cmp.leg->AddEntry(hTrk,Form("|#eta| < %.1f",maxEta),"");
      xmin=0.5; xmax=179.9;
   } else if (anaMode==1) {
      hGenp = (TH1D*)inf->Get("hGenpEta_"+evt);
      hGenp->Rebin(4);
      hTrk = (TH1D*)inf->Get("hTrkEta_"+evt);
      hTrk->Rebin(4);
      hTrkCorr = (TH1D*)inf->Get("hTrkCorrEta_"+evt);
      hTrkCorr->Rebin(4);
      doLogx=false; doLogy=false;
      cmp.Legend(0.19,0.68,0.65,0.93);
//       cmp.leg->AddEntry(hTrk,Form("%.1f<p_{T}<%.1f GeV/c",ptmin,ptmax),"");
      ymin=0; ymax=hGenp->GetMaximum()*4*1.5;
   } else if (anaMode==10) {
      hGenp = (TH1D*)inf->Get(Form("hGenpDPhi_%d_%d_%s",iStudy,jStudy,evt.Data()));
      hTrk = (TH1D*)inf->Get(Form("hTrkDPhi_%d_%d_%s",iStudy,jStudy,evt.Data()));
      hTrkCorr = (TH1D*)inf->Get(Form("hTrkCorrDPhi_%d_%d_%s",iStudy,jStudy,evt.Data()));
      doLogx=false; doLogy=false;
      cmp.Legend(0.29,0.68,0.75,0.93);
//       cmp.leg->AddEntry(hTrk,Form("%.1f<p_{T}<%.1f GeV/c",ptmin,ptmax),"");
      hGenp->SetTitle(";#Delta#phi(trk,jet1);p_{T}^{#parallel} (GeV/c)");
      hGenp->SetTitle(";#Delta#phi(trk,jet1);p_{T}^{#parallel} (GeV/c)");
   }
   
   // Check
   if (!hGenp||!hTrk||!hTrkCorr) {
      cout << "bad histogram input" << endl;
      exit(1);
   }

   // Legend
   if (evt=="0to12") cmp.leg->AddEntry(hTrk,"0-10%","");
   if (legTitle!="") cmp.leg->AddEntry(hTrk,legTitle,"");
   cmp.leg->AddEntry(hTrk,"Raw Trk","p");
   if (doCorr) cmp.leg->AddEntry(hTrkCorr,"Corr. Trk","p");
   cmp.leg->AddEntry(hGenp,"Gen. Particle","l");

   //////////////////////////////////////////
   // Run Analysis
   //////////////////////////////////////////
   hGenp->Scale(1./nEvt);
   hTrk->Scale(1./nEvt);
   hTrkCorr->Scale(1./nEvt);

   //////////////////////////////////////////
   // Draw
   //////////////////////////////////////////
   TCanvas * c2 = new TCanvas("c2","c2",800,400);
   c2->Divide(2,1);
   c2->cd(1);
   hTrk->SetMarkerStyle(kOpenCircle);
   if (anaMode==0) {
      normHist(hGenp,-1,true);
      normHist(hTrk,-1,true);
      normHist(hTrkCorr,-1,true);
   }
   if (doLogx) gPad->SetLogx();
   if (doLogy) gPad->SetLogy();
   if (xmax>xmin) hGenp->SetAxisRange(xmin,xmax,"X");
   if (ymax>ymin) hGenp->SetAxisRange(ymin,ymax,"Y");
   handsomeTH1(hGenp,kRed);
   hGenp->Draw("hist");
   hTrk->Draw("sameE");
   if (doCorr) hTrkCorr->Draw("sameE");
   cmp.leg->Draw();

   c2->cd(2);
   TH1D * hGenRat = (TH1D*)hGenp->Clone("hGenRat");
   if (!doDiff) hGenRat->Divide(hGenp);
   else hGenRat->Add(hGenp,-1);

   TH1D * hTrkRat = (TH1D*)hTrk->Clone("hTrkRat");
   if (!doDiff) hTrkRat->Divide(hGenp);
   else hTrkRat->Add(hGenp,-1);
   
   TH1D * hTrkCorrRat = (TH1D*)hTrkCorr->Clone("hTrkCorrRat");
   if (doCorr) {
      if (!doDiff) hTrkCorrRat->Divide(hGenp);
      else hTrkCorrRat->Add(hGenp,-1);
   }

   if (doLogx) gPad->SetLogx();
   if (xmax>xmin) hTrkRat->SetAxisRange(xmin,xmax,"X");
   if (!doDiff) {
      hGenRat->SetAxisRange(0.,2,"Y");
      hGenRat->SetYTitle("Ratio");
   } else {
      hGenRat->SetAxisRange(-10,10,"Y");
      hGenRat->SetYTitle("Rec - Gen");
   }
   handsomeTH1(hGenRat,kRed);
   hGenRat->Draw("hist");
   hTrkRat->Draw("same E");
   hTrkRat->Draw("sameE");
   if (doCorr) hTrkCorrRat->Draw("sameE");

   c2->Print(Form("%s/%s.gif",outdir.Data(),tag.Data()));
}