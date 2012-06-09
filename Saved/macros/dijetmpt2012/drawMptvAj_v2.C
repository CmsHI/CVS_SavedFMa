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

void SetAliases(TTree* tgj) {
   tgj->SetAlias("gen4","mptxgenpAllAllAcc_pt[9]");
   tgj->SetAlias("rec4","mptxgenpAllAllAcc_pt[9]");
   tgj->SetAlias("cor4","mptxgenpAllAllAcc_pt[9]");
//    tgj->SetAlias("gen4","mptxgenpAllAllAcc_pt[7]+mptxgenpAllAllAcc_pt[8]+mptxgenpAllAllAcc_pt[9]+mptxgenpAllAllAcc_pt[10]+mptxgenpAllAllAcc_pt[11]");
//    tgj->SetAlias("rec4","mptxgenpAllAllAcc_pt[7]+mptxgenpAllAllAcc_pt[8]+mptxgenpAllAllAcc_pt[9]+mptxgenpAllAllAcc_pt[10]+mptxgenpAllAllAcc_pt[11]");
//    tgj->SetAlias("cor4","mptxgenpAllAllAcc_pt[7]+mptxgenpAllAllAcc_pt[8]+mptxgenpAllAllAcc_pt[9]+mptxgenpAllAllAcc_pt[10]+mptxgenpAllAllAcc_pt[11]");
}
//---------------------------------------------------------------------
void drawMptvAj_v2(
                                    )
{
   TH1::SetDefaultSumw2();
   const int nBin = 4;
   float AjBins[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};
   
//    TString infname="../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";
   TString infname="../forestv2/output-hy18dj80_forest2_v0_xsec_icPu5.root";
   TFile * inf = new TFile(infname);
   cout << "input: " << inf->GetName() << endl;
   TTree * tgj = (TTree*)inf->Get("tgj");
   SetAliases(tgj);
   TH1D * hTrkPt = (TH1D*)inf->Get("hTrkPt");

   TCut mycut="cBin>=0&&cBin<12";
//    TCut mycut="cBin>=12&&cBin<40";

   float minPt1=120,minPt2=50,sigDPhi=2./3*3.14159;
   TCut leadingSel  = Form("abs(eta1)<2&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<2&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

//    TString mcweight = "(((samplePtHat==50&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight)";
   
   TCut finalCut = mycut&&leadingSel&&awaySel&&sigSel;
//    finalCut*=mcweight;

   const int nPtBin = 12;
   TProfile *vmptxRec[nPtBin+1], *vmptxRaw[nPtBin+1], *vmptxGen[nPtBin+1];

   for (int i=0; i<nPtBin+1; ++i) {
      vmptxRaw[i] = new TProfile(Form("hmptxRaw_%d",i),"",nBin,AjBins);
      vmptxRaw[i]->SetMarkerStyle(kOpenCircle);
      vmptxRec[i] = new TProfile(Form("hmptxRec_%d",i),"",nBin,AjBins);
      vmptxGen[i] = new TProfile(Form("hmptxGen_%d",i),"",nBin,AjBins);
      vmptxGen[i]->SetLineColor(kRed);
   }
   
   cout << finalCut << endl;
//    tgj->Draw(Form("-gen4:Aj>>hmptxGen_%d",4),finalCut);
//    tgj->Draw(Form("-mptxgenpAllAllAcc:Aj>>hmptxGen_%d",4),finalCut);

   TString vars[3] = {"-mptxgenpAllAllAcc","-mptxtrkCorrAllAcc","-mptxtrkAllAcc"};
   TString hists[3] = {"hmptxGen","hmptxRec","hmptxRaw"};
   for (int i=0; i<nPtBin+1; ++i) {
      for (int d=0; d<3; ++d) {
         TString drawcmd=Form("%s_pt[%d]:Aj>>%s_%d",vars[d].Data(),i,hists[d].Data(),i);
         if (i==nPtBin) drawcmd=Form("%s:Aj>>%s_%d",vars[d].Data(),hists[d].Data(),i);
         cout << drawcmd << endl;
         tgj->Draw(drawcmd,finalCut);
      }
   }
      
   TCanvas * c2 = new TCanvas("c2","c2",1500,500);
   c2->Divide(6,2);
   for (int i=0; i<nPtBin; ++i) {
      c2->cd(i+1);
      vmptxRec[i]->SetAxisRange(-60,60,"Y");
      vmptxRec[i]->SetTitle(";Aj;");
      vmptxRec[i]->Draw("E");
      vmptxRaw[i]->Draw("sameE");
      vmptxGen[i]->Draw("same hist");
      // Legend
      TLegend *leg = new TLegend(0.10,0.68,0.70,0.96);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextFont(63);
      leg->SetTextSize(16);
      leg->AddEntry(vmptxRec[i],Form("%.1f - %.1f GeV/c",hTrkPt->GetBinLowEdge(i+1),hTrkPt->GetBinLowEdge(i+2)),"");
      leg->Draw();
   }
   c2->Print("mptpt_vs_aj_closure_v2.gif");

   TCanvas * c3 = new TCanvas("c3","c3",500,500);
   vmptxRec[nPtBin]->SetAxisRange(-60,60,"Y");
   vmptxRec[nPtBin]->SetTitle(";Aj;");
   vmptxRec[nPtBin]->Draw("E");
   vmptxRaw[nPtBin]->Draw("sameE");
   vmptxGen[nPtBin]->Draw("same hist");
   c3->Print("mptall_vs_aj_closure_v2.gif");
}