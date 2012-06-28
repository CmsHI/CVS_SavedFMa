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
#include "loopMpt.h"

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

void SetAliases(TTree* tm) {
   tm->SetAlias("gen4","mptxgenpAllAllAcc_pt[9]");
}

//---------------------------------------------------------------------
void CompareMptvAj(
                  )
{
   TH1::SetDefaultSumw2();
   const int nComp=2;
   bool doResCorr=false;
   TString inputFile_sim="fig/06.26_tree/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec1.root";
   TString inputFile_rec="fig/06.26_tree/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";
   TString inputFile_raw="fig/06.26_tree/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";

   string path=inputFile_rec.Data();
   TString outdir = path.substr(0, path.find_last_of('/'));
   cout << "Output: " << outdir << endl;
   gSystem->mkdir(outdir,kTRUE);

   TString tag = Form("CorrRes%d_sim_rec",doResCorr);

   //////////////////////////////////////////
   // Analysis Setup
   //////////////////////////////////////////
   TFile * vinf[nComp];
   vinf[0] = new TFile(inputFile_sim);
   vinf[1] = new TFile(inputFile_rec);
   if (nComp>2) vinf[2] = new TFile(inputFile_raw);
   
   TTree * vtm[nComp];
   TString insrc="0to12";
   for (int i=0; i<nComp; ++i) vtm[i] = (TTree*)vinf[i]->Get("t"+insrc);

   //////////////////////////////////////////////////////////
   // Cuts
   float minPt1=100,minPt2=50,sigDPhi=7./8*3.14159;
   TCut leadingSel  = Form("pt1>%.3f&&abs(eta1)<1.6",minPt1);
   TCut awaySel     = Form("pt2>%.3f&&abs(eta2)<1.6",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);
   //////////////////////////////////////////////////////////

//    TString mcweight = "(((samplePtHat==50&&pthat<120)||(samplePtHat==120&&pthat<170)||(samplePtHat==170&&pthat<200)||(samplePtHat==200))*weight*sampleWeight)";
   
   TCut finalCut = leadingSel&&awaySel&&sigSel;
//    finalCut*=mcweight;
   float vnEvt[nComp];
   cout << finalCut << endl;
   for (int i=0; i<nComp; ++i) {
      vnEvt[i] = vtm[i]->GetEntries(finalCut);
      cout << vinf[i]->GetName() << ": " << vnEvt[i] << endl;
   }

   //////////////////////////////////////////
   // Book Histograms
   //////////////////////////////////////////
   TProfile *vhmpt[nptrange+1][nComp];

   for (int i=0; i<nptrange+1; ++i) {
      for (int ic=0; ic<nComp; ++ic) {
         vhmpt[i][ic] = new TProfile(Form("hmpt_pt%d_cmp%d",i,ic),"",nAjBin,AjBins);
         if (ic==0) {
            vhmpt[i][ic]->SetLineColor(kRed);
            vhmpt[i][ic]->SetMarkerColor(kRed);
         }
         if (ic==3) {
            vhmpt[i][ic]->SetMarkerStyle(kOpenCircle);
         }
      }
   }

   // Legend
   Compare cmp0("cmp0","");
   cmp0.Legend(0.31,0.17,0.91,0.38);
   if (insrc=="0to12") cmp0.leg->AddEntry(vhmpt[0][0],"0-30%","");
   cmp0.leg->AddEntry(vhmpt[0][0],"Gen. Particle","l");
   cmp0.leg->AddEntry(vhmpt[0][1],"Rec Trk","p");
   if (nComp>2) cmp0.leg->AddEntry(vhmpt[0][2],"Raw Trk","p");
   vector<Compare> vcmp;
   for (int i=0; i<nptrange+1; ++i) {
      vcmp.push_back(Compare(Form("vcmp%d",i),""));
      vcmp[i].Legend(0.16,0.84,0.76,0.94);
   }

   //////////////////////////////////////////
   // Run Analysis
   //////////////////////////////////////////
   for (int i=0; i<nptrange+1; ++i) {
      float ptmin=ptranges[i];
      float ptmax=ptranges[i+1];
      if (i==nptrange) {
         ptmin=ptranges[0]; ptmax=ptranges[nptrange];
      }
      // Legend
      TString bintitle = Form("%.1f - %.1f GeV/c",ptmin,ptmax);
      vcmp[i].leg->AddEntry(vhmpt[0][0],bintitle,"");
      cout << "Pt " << i << ": " << bintitle << endl;
      // Histograms
      for (int ic=0; ic<nComp; ++ic) {
         TString drawcmd=Form("xpt[%d]:Aj>>%s",i,vhmpt[i][ic]->GetName());
         if (i==nptrange) {
            drawcmd=Form("Sum$(xpt[]):Aj>>%s",vhmpt[i][ic]->GetName());
         }
         float npass = vtm[ic]->Draw(drawcmd,finalCut,"goff");
         cout << drawcmd << endl;
//          cout << finalCut << ": " << npass << endl;
      }
   }

      
   //////////////////////////////////////////
   // Print Info
   //////////////////////////////////////////
//    cout << "Gen: " << endl;
//    PrintMPT(vmptxGen,nptrange);
//    cout << "Raw: " << endl;
//    PrintMPT(vmptxRaw,nptrange);
//    cout << "Corr: " << endl;
//    PrintMPT(vmptxRec,nptrange);
   
   //////////////////////////////////////////
   // Draw
   //////////////////////////////////////////
   int vMode[2] ={0,1}; // 0 mpt, 1 diff
   TLine * l0 = new TLine(AjBins[0],0,AjBins[nAjBin],0);
   l0->SetLineStyle(2);

   TH1D * vhmptdiff[nptrange+1];
   for (int m=0; m<2; ++m) {
      // Individual Pt Ranges
      TCanvas * c2 = new TCanvas(Form("cPt%d",m),Form("cPt%d",m),1000,500);
      c2->Divide(4,2);
      for (int i=0; i<nptrange; ++i) {
         c2->cd(i+1);
         if (m==0) {
            vhmpt[i][0]->SetAxisRange(-60,60,"Y");
            vhmpt[i][0]->SetTitle(";A_{J};<#slash{p}_{T}^{#parallel}> (GeV/c)");
            vhmpt[i][0]->Draw("hist");
            vhmpt[i][1]->Draw("sameE");
            if (nComp>2) vhmpt[i][2]->Draw("sameE");
            if (i==3) cmp0.leg->Draw();
         } else if (m==1) {
            vhmptdiff[i] = (TH1D*)vhmpt[i][1]->Clone(Form("%s_diff",vhmpt[i][1]->GetName()));
            vhmptdiff[i]->Add((TH1D*)vhmpt[i][0],-1);
            vhmptdiff[i]->SetAxisRange(-10,10,"Y");
            vhmptdiff[i]->SetTitle(";A_{J};Rec - Sim");
            vhmptdiff[i]->Draw("E");
         }
         vcmp[i].leg->Draw();
         l0->Draw();
      }
      c2->Print(Form("%s/compMptvAj_Pt_%s_%d.gif",outdir.Data(),tag.Data(),m));
      c2->Print(Form("%s/compMptvAj_Pt_%s_%d.pdf",outdir.Data(),tag.Data(),m));
   
      // All Pt Ranges
      TCanvas * c3 = new TCanvas(Form("cAll%d",m),Form("cAll%d",m),500,500);
      if (m==0) {
         vhmpt[nptrange][0]->SetAxisRange(-60,60,"Y");
         vhmpt[nptrange][0]->SetTitle(";A_{J};<#slash{p}_{T}^{#parallel}> (GeV/c)");
         vhmpt[nptrange][0]->Draw("hist");
         vhmpt[nptrange][1]->Draw("sameE");
         if (nComp>2) vhmpt[nptrange][2]->Draw("sameE");
         cmp0.leg->Draw();
      } else if (m==1) {
         vhmptdiff[nptrange] = (TH1D*)vhmpt[nptrange][1]->Clone(Form("%s_diff",vhmpt[nptrange][1]->GetName()));
         vhmptdiff[nptrange]->Add((TH1D*)vhmpt[nptrange][0],-1);
         vhmptdiff[nptrange]->SetAxisRange(-10,10,"Y");
         vhmptdiff[nptrange]->SetTitle(";A_{J};Rec - Sim");
         vhmptdiff[nptrange]->Draw("E");
      }
      vcmp[nptrange].leg->Draw();
      l0->Draw();
      c3->Print(Form("%s/compMptvAj_All_%s_%d.gif",outdir.Data(),tag.Data(),m));
      c3->Print(Form("%s/compMptvAj_All_%s_%d.pdf",outdir.Data(),tag.Data(),m));
   }
}