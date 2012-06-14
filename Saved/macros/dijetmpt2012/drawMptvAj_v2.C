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
#include "TSystem.h"
#include "commonUtility.h"
#include "compare.h"

const int nBin = 4;
float AjBins[nBin+1] = {0.0001,0.11,0.22,0.33,0.49999};

const int nptrange = 6;
float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,200};

void PrintMPT(TProfile ** vmpt, int nPtBin) {
   float sum[4];
   for (int a=0; a<4; ++a) {
      sum[a]=0;
      for (int i=0; i<nPtBin; ++i) {
         sum[a]+=vmpt[i]->GetBinContent(a+1);
         cout << "Aj/pt " << a << "/" << i << ", mpt: " << vmpt[i]->GetBinContent(a+1) << " mpt sum: " << sum[a] << " mptall: " << vmpt[nPtBin]->GetBinContent(a+1) << endl;
      }
   }
}

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
                  TString outdir = "fig/06.13MptClos"
                  )
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   TString infname="../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";
   TFile * inf = new TFile(infname);
   cout << "input: " << inf->GetName() << endl;
   TTree * tgj = (TTree*)inf->Get("tgj");
   SetAliases(tgj);
   TH1D * hTrkPt = (TH1D*)inf->Get("hTrkPt");

   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   bool doResum=true;
   bool doZoom=true;
   float etamax=1; //2.4
   int cMin=0, cMax=30;
   TString tag=Form("HP_Resum_eta%.0f_c%dto%d",etamax*10,cMin,cMax);
   
   TString genMptVar = "-genpPt*cos(genpPhi-phi1)";
   TString recMptVar = "-trkPt*cos(trkPhi-phi1)";
   TString genpSel = Form("abs(genpEta)<%.1f&&genpCh!=0",etamax);
   TString trkSel = Form("abs(trkEta)<%.1f&&(trkNHit<8||vtrkQual[][0])",etamax);
   TString trkWt = "vtrkWt[][0]";
//    TCut trkSel = Form("abs(trkEta)<%.1f && (trkNHit<8||(vtrkQual[][0]&&trkAlgo==4))",etamax);
//    TString trkWt = "vtrkWt[][1]";
//    TCut trkSel = Form("abs(trkEta)<%.1f",etamax);
//    TString trkWt = "vtrkWt[][2]";
   
   TCut mycut=Form("cBin>=%d&&cBin<%d",cMin,cMax);

   float minPt1=120,minPt2=50,sigDPhi=2./3*3.14159;
   TCut leadingSel  = Form("pt1>%.3f&&abs(eta1)<1.6",minPt1);
   TCut awaySel     = Form("pt2>%.3f&&abs(eta2)<1.6",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

//    TString mcweight = "(((samplePtHat==50&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight)";
   
   TCut finalCut = mycut&&leadingSel&&awaySel&&sigSel;
//    finalCut*=mcweight;
   float nEvt = tgj->GetEntries(finalCut);
   cout << finalCut << ": " << nEvt << endl;

   //////////////////////////////////////////
   // Book Histograms
   //////////////////////////////////////////
//    const int nPtBin = 12;
   const int nPtBin = nptrange;
   TProfile *vmptxRec[nPtBin+1], *vmptxRaw[nPtBin+1], *vmptxGen[nPtBin+1], *vtrkWt[nPtBin+1];

   vector<Compare> vcmp;
   for (int i=0; i<nPtBin+1; ++i) {
      vcmp.push_back(Compare(Form("cmp%d",i),""));
      if (i==0) vcmp[i].Legend(0.16,0.65,0.76,0.94);
      else if (i==nPtBin) vcmp[i].Legend(0.36,0.78,0.79,0.94);
      else vcmp[i].Legend(0.16,0.84,0.76,0.94);
   }
   for (int i=0; i<nPtBin+1; ++i) {
      vmptxRaw[i] = new TProfile(Form("hmptxRaw_%d",i),"",nBin,AjBins);
      vmptxRaw[i]->SetMarkerStyle(kOpenCircle);
      vmptxRec[i] = new TProfile(Form("hmptxRec_%d",i),"",nBin,AjBins);
      vmptxGen[i] = new TProfile(Form("hmptxGen_%d",i),"",nBin,AjBins);
      vmptxGen[i]->SetLineColor(kRed);
      vtrkWt[i] = new TProfile(Form("htrkWt_%d",i),"",nBin,AjBins);
   }
   
//    tgj->Draw(Form("-gen4:Aj>>hmptxGen_%d",4),finalCut);
//    tgj->Draw(Form("-mptxgenpAllAllAcc:Aj>>hmptxGen_%d",4),finalCut);

   //////////////////////////////////////////
   // Run Analysis
   //////////////////////////////////////////
   TString vars[3] = {"-mptxgenpAllAllAcc","-mptxtrkCorrAllAcc","-mptxtrkAllAcc"};
   TString hists[3] = {"hmptxGen","hmptxRec","hmptxRaw"};
   for (int i=0; i<nPtBin+1; ++i) {
//       float ptmin=hTrkPt->GetBinLowEdge(i+1);
//       float ptmax=hTrkPt->GetBinLowEdge(i+2);
      float ptmin=ptranges[i];
      float ptmax=ptranges[i+1];
      if (i==nPtBin) {
         ptmin=ptranges[0];
         ptmax=ptranges[nptrange];
      }
      // Legend
      TString bintitle = Form("%.1f - %.1f GeV/c",ptmin,ptmax);
      vcmp[i].leg->AddEntry(vmptxRec[i],bintitle,"");
      if (i==0||i==nPtBin) {
         vcmp[i].leg->AddEntry(vmptxGen[0],"Gen. Particle","l");
         vcmp[i].leg->AddEntry(vmptxRaw[1],"Raw Trk","p");
         vcmp[i].leg->AddEntry(vmptxRec[2],"Corr. Trk","p");
      }
      cout << bintitle << endl;
      // Histograms
      for (int d=0; d<3; ++d) {
         TString drawcmd=Form("%s_pt[%d]:Aj>>%s_%d",vars[d].Data(),i,hists[d].Data(),i);
         if (i==nPtBin) {
            drawcmd=Form("%s:Aj>>%s_%d",vars[d].Data(),hists[d].Data(),i);
         }
         if (doResum) {
            TString mpt= Form("%s*(genpPt>=%.2f&&genpPt<%.2f&&%s)",genMptVar.Data(),ptmin,ptmax,genpSel.Data());
            if (d>0) mpt=Form("%s*(trkPt>=%.2f&&trkPt<%.2f&&%s)",recMptVar.Data(),ptmin,ptmax,trkSel.Data());
            if (d==1) mpt+=("*"+trkWt);
            drawcmd=Form("Sum$(%s):Aj>>%s_%d",mpt.Data(),hists[d].Data(),i);
         }
         cout << drawcmd << endl;
         tgj->Draw(drawcmd,finalCut,"goff");
      }
   }

   //////////////////////////////////////////
   // Additional Overall Corrections
   //////////////////////////////////////////
   if (0) {
//       for (int i=0; i<nPtBin+1; ++i) {
//          TString drawcmd;
//          float ptmin=hTrkPt->GetBinLowEdge(i+1);
//          float ptmax=hTrkPt->GetBinLowEdge(i+2);
//          if (i==nPtBin) {
//             ptmin=hTrkPt->GetBinLowEdge(1);
//             ptmax=1000;
//          }
//          TCut trkSel=Form("(trkPt>=%.2f&&trkPt<%.2f&&abs(trkEta)<2.4&&(trkNHit<7||(vtrkQual[][0])))",ptmin,ptmax);
//          drawcmd=Form("vtrkWt[][0]:Aj>>%s",vtrkWt[i]->GetName());
//          cout << drawcmd << " " << TString(finalCut&&trkSel) << endl;
//          tgj->Draw(drawcmd,finalCut&&trkSel,"goff prof",5000);
//       }
//       TCanvas * c0 = new TCanvas("c0","c0",1500,500);
//       c0->Divide(6,2);
//       for (int i=0; i<nPtBin; ++i) {
//          c0->cd(i+1);
//          vtrkWt[i]->SetAxisRange(1,2,"Y");
//          vtrkWt[i]->Draw("E");
//          // Legend
//          TLegend *leg = new TLegend(0.10,0.68,0.70,0.96);
//          leg->SetFillStyle(0);
//          leg->SetBorderSize(0);
//          leg->SetTextFont(63);
//          leg->SetTextSize(16);
//          leg->AddEntry(vmptxRec[i],Form("%.1f - %.1f GeV/c",hTrkPt->GetBinLowEdge(i+1),hTrkPt->GetBinLowEdge(i+2)),"");
//          leg->Draw();
//       }
//       c0->Print(Form("trkwt_vs_aj_closure_v2_%s.gif",tag.Data()));
   }

      
   //////////////////////////////////////////
   // Print Info
   //////////////////////////////////////////
   cout << "Gen: " << endl;
   PrintMPT(vmptxGen,nPtBin);
   cout << "Raw: " << endl;
   PrintMPT(vmptxRaw,nPtBin);
   cout << "Corr: " << endl;
   PrintMPT(vmptxRec,nPtBin);
   
   //////////////////////////////////////////
   // Draw
   //////////////////////////////////////////
   // Axis Range
   float ymin[nPtBin+1],ymax[nPtBin+1];
   for (int i=0; i<nPtBin+1; ++i) {
      ymin[i]=-60; ymax[i]=60;
      if (doZoom) {
         ymin[i]=(vmptxRec[i]->GetMinimum()<vmptxGen[i]->GetMinimum() ? vmptxRec[i]->GetMinimum() : vmptxGen[i]->GetMinimum());
         ymax[i]=(vmptxRec[i]->GetMaximum()>vmptxGen[i]->GetMaximum() ? vmptxRec[i]->GetMaximum() : vmptxGen[i]->GetMaximum());
         float diff=ymax[i]-ymin[i];
         ymin[i]-=diff*0.4;
         ymax[i]+=diff*0.4;
//          if (ymin[i]>-12) ymin[i]=-12;
//          else if (ymin[i]>-35) ymin[i]=-35;
//          if (ymax[i]<12) ymax[i]=12;
//          else if (ymax[i]<35) ymax[i]=35;
      }
   }
   
   TLine * l0 = new TLine(AjBins[0],0,AjBins[nBin],0);
   // Individual Pt Ranges
   float canx=1500,cany=500;
   if (nPtBin==6) { canx=750; cany=500; }
   TCanvas * c2 = new TCanvas("c2","c2",canx,cany);
   if (nPtBin==12) c2->Divide(6,2);
   else if (nPtBin==6) c2->Divide(3,2);
   for (int i=0; i<nPtBin; ++i) {
      c2->cd(i+1);
      vmptxRec[i]->SetAxisRange(ymin[i],ymax[i],"Y");
      vmptxRec[i]->SetTitle(";Aj;");
      vmptxRec[i]->Draw("E");
      vmptxRaw[i]->Draw("sameE");
      vmptxGen[i]->Draw("same hist");
      vcmp[i].leg->Draw();
      l0->Draw();
   }
   c2->Print(Form("%s/mptpt_vs_aj_mc_%s.gif",outdir.Data(),tag.Data()));

   // All Pt Ranges
   TCanvas * c3 = new TCanvas("c3","c3",500,500);
   vmptxRec[nPtBin]->SetAxisRange(ymin[nPtBin],ymax[nPtBin],"Y");
   vmptxRec[nPtBin]->SetTitle(";Aj;");
   vmptxRec[nPtBin]->Draw("E");
   vmptxRaw[nPtBin]->Draw("sameE");
   vmptxGen[nPtBin]->Draw("same hist");
   vcmp[nPtBin].leg->Draw();
   l0->Draw();
   c3->Print(Form("%s/mptall_vs_aj_mc_%s.gif",outdir.Data(),tag.Data()));
}