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
void drawMptvAj(
                                    )
{
   TH1::SetDefaultSumw2();
   const int nBin = 4;
   float AjBins[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};
   
//    TString infname="../ntout/output-hy18dj80_IterPixTrkv1_v6_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/forest/output-hy18dj80_IterPixTrkv1_v7_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18dj80_IterPixTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18djallpthats_iterTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18dj200_iterTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root";
   TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18dj200_iterTrkv1_v11hgtOnly_hgtcorr_mptOnly_xsec_icPu5.root";
   TFile * inf = new TFile(infname);
   TTree * tgj = (TTree*)inf->Get("tgj");

   TCut mycut="cBin>=0&&cBin<12";
//    TCut mycut="cBin>=12&&cBin<40";

   float minPt1=200,minPt2=50,sigDPhi=2./3*3.14159;
//    TCut leadingSel  = Form("abs(eta1)<2&&pt1>%.3f&&(pt1/ref1pt)>0.9&&(pt1/ref1pt)<1.1",minPt1);
//    TCut leadingSel  = Form("abs(eta1)<2&&pt1>%.3f&&pt1<200",minPt1);
   TCut leadingSel  = Form("abs(eta1)<2&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<2&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

   TString mcweight = "(((samplePtHat==50&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight)";
   
   TCut finalCut = mycut&&leadingSel&&awaySel&&sigSel;
//    finalCut*=mcweight;

   TProfile *vmptxRec[7], *vmptxRaw[7], *vmptxGen[7];
   for (int i=0; i<7; ++i) {
      vmptxRaw[i] = new TProfile(Form("hmptxRaw_%d",i),"",nBin,AjBins);
      vmptxRaw[i]->SetMarkerStyle(kOpenCircle);
      vmptxRec[i] = new TProfile(Form("hmptxRec_%d",i),"",nBin,AjBins);
      vmptxGen[i] = new TProfile(Form("hmptxGen_%d",i),"",nBin,AjBins);
      vmptxGen[i]->SetLineColor(kRed);
   }
   
   cout << finalCut << endl;
   for (int i=0; i<7; ++i) {
      if (i<6) {
         tgj->Draw(Form("-mptxgenpAllAllAcc_pt[%d]:Aj>>hmptxGen_%d",i,i),finalCut);
         tgj->Draw(Form("-mptxtrkCorrAllAcc_pt[%d]:Aj>>hmptxRec_%d",i,i),finalCut);
         tgj->Draw(Form("-mptxtrkAllAcc_pt[%d]/0.72:Aj>>hmptxRaw_%d",i,i),finalCut);
      } else {
         tgj->Draw(Form("-mptxgenpAllAllAcc:Aj>>hmptxGen_%d",i),finalCut);
         tgj->Draw(Form("-mptxtrkCorrAllAcc:Aj>>hmptxRec_%d",i),finalCut);
         tgj->Draw(Form("-mptxtrkAllAcc/0.72:Aj>>hmptxRaw_%d",i),finalCut);
      }
   }
      
   TCanvas * c2 = new TCanvas("c2","c2",1100,600);
   c2->Divide(3,2);
   for (int i=0; i<6; ++i) {
      c2->cd(i+1);
      vmptxRec[i]->SetAxisRange(-60,60,"Y");
      vmptxRec[i]->SetTitle(";Aj;");
      vmptxRec[i]->Draw("E");
      vmptxRaw[i]->Draw("sameE");
      vmptxGen[i]->Draw("same hist");
   }
   c2->Print("mptpt_vs_aj_closure.gif");

   TCanvas * c3 = new TCanvas("c3","c3",500,500);
   vmptxRec[6]->SetAxisRange(-60,60,"Y");
   vmptxRec[6]->SetTitle(";Aj;");
   vmptxRec[6]->Draw("E");
   vmptxRaw[6]->Draw("sameE");
   vmptxGen[6]->Draw("same hist");
   c3->Print("mptall_vs_aj_closure.gif");
}