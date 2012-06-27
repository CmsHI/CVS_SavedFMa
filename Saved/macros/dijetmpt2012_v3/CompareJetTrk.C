#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
#include "commonUtility.h"
#include "compare.h"
using namespace std;

void CompareJetTrk()
{
   TString outdir = "fig/06.26_tree";
   TH1::SetDefaultSumw2();
   TString tag = "compJetTrk_DataMc";
   const int nPtBin=20;
   float ptBins[nPtBin+1] = {0.5,0.75,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120,200};
   // inputs
//    TString infdataname="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
//    TString infrefname = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";
   TString infdataname="../ntout/output-data-Forest2v3_v3_saveTrks_jpt120_icPu5.root";
   TString infrefname = "../ntout/output-hy18dj80_Forest2v21_v3_allTrks_simtrk_jpt120_xsec_icPu5.root";

   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("tgj");
   cout << infdata->GetName() << endl;

   TFile * infref = new TFile(infrefname);   
   TTree * tref = (TTree*)infref->Get("tgj");
   cout << infref->GetName() << endl;
   
   float minPt1=120,minPt2=50,sigDPhi=7./8*3.14159;
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);
   TCut mycut = "cBin<12";

   TCut sel = mycut&&leadingSel&&awaySel&&sigSel;
   TCut trkSel = "(trkNHit<8||(vtrkQual[][0]))";
   // histograms
   for (int i=0; i<3; ++i) {
      TCanvas * c2 = new TCanvas(Form("cComp%d",i),Form("cComp%d",i),400,400);
      int normMode=1;
      if (i==0) normMode=2;
//       if (i>0) normMode=20;
      Compare cmp(Form("cmp_%d",i),sel,normMode);
      cmp.vt.push_back(tdata);
      cmp.vt.push_back(tref);
      cmp.vwt.push_back("");
      cmp.vwt.push_back("weight");
      float lx1=0.45, ly1=0.6, lx2=0.94, ly2=0.92;
      if (i==0) {
         cmp.vsel.push_back(trkSel);
         cmp.vsel.push_back(trkSel);
         c2->SetLogy();
         c2->SetLogx();
         cmp.Init(nPtBin,ptBins);
         cmp.Project("trkPt>>");
         cmp.Draw(";Track p_{T} (GeV/c);u.n.",1e-9,1e1);
         lx1=0.17; ly1=0.17; lx2=0.66; ly2=0.49;
      }
      if (i==1) {
         cmp.vsel.push_back(trkSel&&"trkPt>1");
         cmp.vsel.push_back(trkSel&&"trkPt>1");
         cmp.Init(40,0,3.14);
         cmp.Project("acos(cos(trkPhi-phi1))>>");
         cmp.Draw(";#Delta#phi(Trk,Jet1);u.n.",0,0.045);
         lx1=0.32; ly1=0.61; lx2=0.81; ly2=0.93;
      }
      if (i==2) {
         cmp.vsel.push_back(trkSel&&"trkPt>8");
         cmp.vsel.push_back(trkSel&&"trkPt>8");
         cmp.Init(40,0,3.14);
         cmp.Project("acos(cos(trkPhi-phi1))>>");
         cmp.Draw(";#Delta#phi(Trk,Jet1);per evt.",0,0.7);
         lx1=0.32; ly1=0.61; lx2=0.81; ly2=0.93;
      }
      cmp.Legend(lx1,ly1,lx2,ly2);
      cmp.leg->AddEntry(cmp.vh[0],"0-30\%","");
      cmp.leg->AddEntry(cmp.vh[0],"p_{T,1}, p_{T,2} > 120, 50GeV","");
      cmp.leg->AddEntry(cmp.vh[0],"#Delta#phi_{1,2} > #frac{7}{8}#pi","");
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data","p");
      cmp.leg->AddEntry(cmp.vh[1],"PYTHIA+HYDJET","p");
      cmp.leg->Draw();
      c2->Print(Form("%s/%s_%d.gif",outdir.Data(),tag.Data(),i));
   }
}
