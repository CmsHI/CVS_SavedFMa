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
   const int nptrange = 6;
   float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,180};
   
   TH1::SetDefaultSumw2();


//    TString infname="../ntout/output-hy18dj80_IterPixTrkv1_v6_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/forest/output-hy18dj80_IterPixTrkv1_v7_xsec_icPu5.root";
//    TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18dj80_IterPixTrkv1_v9hgtOnly_hgtcorr_xsec_icPu5.root";
   TString infname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/ntout/output-hy18dj200_iterTrkv1_v11hgtOnly_hgtcorr_mptOnly_xsec_icPu5.root";
   TFile * inf = new TFile(infname);
   TTree * tgj = (TTree*)inf->Get("tgj");

   TCut mycut="cBin>=0&&cBin<12";
//    TCut mycut="cBin>=12&&cBin<40";

   float minPt1=200,minPt2=50,sigDPhi=2./3*3.14159;
   TCut leadingSel  = Form("abs(eta1)<2&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<2&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

   TString mcweight = "(((samplePtHat==50&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight)";

   TH1D *vmptxRec[7], *vmptxRaw[7], *vmptxGen[7], *vwtsum[7];
   for (int i=0; i<7; ++i) {
      float mptmax=400;
      vmptxRaw[i] = new TH1D(Form("hmptxRaw_%d",i),"",200,-mptmax,mptmax);
      vmptxRaw[i]->SetMarkerStyle(kOpenCircle);
      vmptxRaw[i]->SetMarkerColor(kBlue);
      vmptxRec[i] = new TH1D(Form("hmptxRec_%d",i),"",200,-mptmax,mptmax);
      vmptxGen[i] = new TH1D(Form("hmptxGen_%d",i),"",200,-mptmax,mptmax);
      vmptxGen[i]->SetLineColor(kRed);
      vwtsum[i] = new TH1D(Form("hmptxWt_%d",i),"",200,-mptmax,mptmax);
   }
   
   TCut finalCut = mycut&&leadingSel&&awaySel&&sigSel;
//       finalCut*=mcweight;
   cout << finalCut << endl;
   for (int i=0; i<7; ++i) {
      if (i<6) {
//          TCut trkCut = Form("trkPt>%.1f&&trkPt<%.1f",ptranges[i],ptranges[i+1]);
//          TCut genCut = Form("genpPt>%.1f&&genpPt<%.1f",ptranges[i],ptranges[i+1]);
//          cout << finalCut*genCut << endl;
//          cout << finalCut*trkCut << endl;
//          tgj->Draw(Form("Sum$(-genpPt*cos(genpPhi-phi1)*(genpPt>0.5&&abs(genpEta)<2.2&&genpCh!=0))>>hmptxGen_%d",i),finalCut*genCut);
//          tgj->Draw(Form("mptxtrkCorrAllAcc_pt[%d]>>hmptxRec_%d",i,i),finalCut*trkCut);
//          tgj->Draw(Form("-mptxtrkAllAcc_pt[%d]>>hmptxRaw_%d",i,i),finalCut*trkCut);
         tgj->Draw(Form("mptxgenpAllAllAcc_pt[%d]>>hmptxGen_%d",i,i),finalCut);
         tgj->Draw(Form("-mptxtrkAllAcc_pt[%d]/0.72>>hmptxRaw_%d",i,i),finalCut);
         tgj->Draw(Form("-mptxtrkAllAcc_pt[%d]>>hmptxRaw_%d",i,i),finalCut);
      } else {
//       finalCut*=mcweight;
//          tgj->Draw(Form("Sum$(-genpPt*cos(genpPhi-phi1)*(genpPt>0.5&&abs(genpEta)<2.2&&genpCh!=0))>>hmptxGen_%d",i),finalCut);
         tgj->Draw(Form("mptxgenpAllAllAcc>>hmptxGen_%d",i),finalCut);
         tgj->Draw(Form("mptxtrkCorrAllAcc>>hmptxRec_%d",i),finalCut);
         tgj->Draw(Form("-mptxtrkAllAcc>>hmptxRaw_%d",i),finalCut);
//          tgj->Draw(Form("-mptxtrkAllAcc/0.72>>hmptxRaw_%d",i),finalCut);
//          tgj->Draw(Form("Sum$(-trkPt*cos(trkPhi-phi1)*(trkPt>0.5&&abs(trkEta)<2.2)*trkWt)>>hmptxRaw_%d",i),finalCut);
//           tgj->Draw(Form("-mptxtrkAllAcc*Sum$( (trkPt>0.5&&abs(trkEta)<2.2)*trkWt )/Sum$( (trkPt>0.5&&abs(trkEta)<2.2) )>>hmptxRaw_%d",i),finalCut);
//           tgj->Draw(Form("1>>hmptxWt_%d",i),finalCut*"Sum$( (trkPt>0.5&&abs(trkEta)<2.2)*trkWt )");
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
//    vmptxRaw[6]->Divide(vwtsum[6]);
   vmptxRaw[6]->Draw("sameE");
   vmptxGen[6]->Draw("same hist");
}