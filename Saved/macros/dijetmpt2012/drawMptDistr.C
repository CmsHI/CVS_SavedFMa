#include <iostream>
#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TCut.h"
#include "commonUtility.h"

//---------------------------------------------------------------------
void drawMptDistr(
                                    )
{
   TH1::SetDefaultSumw2();
   float minPt1=100,minPt2=50,sigDPhi=2./3*3.14159;
   
//    TString infname="../ntout/output-hy18dj80_IterPixTrkv1_v6_xsec_icPu5.root";
   TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/forest/output-hy18dj80_IterPixTrkv1_v7_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18dj80_IterPixTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root";
   TFile * inf = new TFile(infname);
   TTree * tgj = (TTree*)inf->Get("tgj");

//    TCut mycut="cBin>=0&&cBin<12";
   TCut mycut="cBin>=12&&cBin<40";

   TCut leadingSel  = Form("abs(eta1)<2&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<2&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

   TH1D *vmptxRec[7], *vmptxRaw[7], *vmptxGen[7];
   for (int i=0; i<7; ++i) {
      vmptxRaw[i] = new TH1D(Form("hmptxRaw_%d",i),"",200,-200,200);
      vmptxRaw[i]->SetMarkerStyle(kOpenCircle);
      vmptxRec[i] = new TH1D(Form("hmptxRec_%d",i),"",200,-200,200);
      vmptxGen[i] = new TH1D(Form("hmptxGen_%d",i),"",200,-200,200);
      vmptxGen[i]->SetLineColor(kRed);
   }
   
   for (int i=0; i<7; ++i) {
      if (i<6) {
         tgj->Draw(Form("-mptxgenpAllAllAcc_pt[%d]>>hmptxGen_%d",i,i),mycut&&leadingSel&&awaySel&&sigSel);
         tgj->Draw(Form("-mptxtrkCorrAllAcc_pt[%d]>>hmptxRec_%d",i,i),mycut&&leadingSel&&awaySel&&sigSel);
         tgj->Draw(Form("-mptxtrkAllAcc_pt[%d]>>hmptxRaw_%d",i,i),mycut&&leadingSel&&awaySel&&sigSel);
      } else {
         tgj->Draw(Form("-mptxgenpAllAllAcc>>hmptxGen_%d",i),mycut&&leadingSel&&awaySel&&sigSel);
         tgj->Draw(Form("-mptxtrkCorrAllAcc>>hmptxRec_%d",i),mycut&&leadingSel&&awaySel&&sigSel);
         tgj->Draw(Form("-mptxtrkAllAcc>>hmptxRaw_%d",i),mycut&&leadingSel&&awaySel&&sigSel);
      }
   }

   TCanvas * c2 = new TCanvas("c2","c2",1100,600);
   c2->Divide(3,2);
   for (int i=0; i<6; ++i) {
      c2->cd(i+1);
      gPad->SetLogy();
      vmptxRec[i]->SetTitle(";mptx (GeV/c);");
      vmptxRec[i]->Draw("E");
      vmptxRaw[i]->Draw("sameE");
      vmptxGen[i]->Draw("same hist");
   }

   TCanvas * c3 = new TCanvas("c3","c3",500,500);
      gPad->SetLogy();
   vmptxRec[6]->SetTitle(";mptx (GeV/c);");
   vmptxRec[6]->Draw("E");
   vmptxRaw[6]->Draw("sameE");
   vmptxGen[6]->Draw("same hist");
}