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

void CompareTrk()
{
   TH1::SetDefaultSumw2();
   int isData=1;
   TString tag = "comp";
   // inputs
//    TString infdataname="~/hadoopfranklocal/forest/HiForest_promptskim-hihighpt-jet115-v0_part2.root";
   TString infrefname="/net/hidsk0001/d00/scratch/frankma/work/jet/macros/forest/output-data-DiJetTrk_v0_v8_icPu5.root";
   if (isData) infrefname = "~/hadoopfranklocal/forest/HiForest_promptskim-hihighpt-jet115-v0_part2.root";
//    TFile * infdata = new TFile(infdataname);
   TFile * infref = new TFile(infrefname);
   
   TTree * tref = (TTree*)infref->Get("anaTrack/trackTree");
   tref->AddFriend("hiEvtAnalyzer/HiTree",infrefname);
   
   TCut sel = "hiBin<2&&trkQual&&trkPt>10&&trkAlgo>=0";
      
   // histograms
   TCut mysel[2] = {"abs(trkEta)<1.6","abs(trkEta)>1.6"};
   TString mytitle[2] = {"|#eta| < 1.6","|#eta| > 1.6"};

   TCanvas * c2 = new TCanvas("c2","c2",800,400);
   c2->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c2->cd(i+1);
      Compare cmpChi2(Form("cmpChi2_%d",i),sel,0);
      cmpChi2.vt.push_back(tref);
      if (isData) cmpChi2.vsel.push_back(mysel[i]);
      else cmpChi2.vsel.push_back(mysel[i]&&"!trkFake");
      if (!isData) {
         cmpChi2.vt.push_back(tref);
         cmpChi2.vsel.push_back(mysel[i]&&"trkFake");
      }
      cmpChi2.Init(100,0,0.5);
      cmpChi2.Project("trkChi2/trkNlayer/trkNdof>>");
      cmpChi2.Draw(";normalized #chi^2;# tracks",0,0,0.45,0.67,0.94,0.92);
      if (isData) cmpChi2.leg->AddEntry(cmpChi2.vh[0],"PbPb Data 0-5\%","");
      else cmpChi2.leg->AddEntry(cmpChi2.vh[0],"PYTHIA+HYDJET 0-5\%","");
      cmpChi2.leg->AddEntry(cmpChi2.vh[0],"p_{T} > 10 GeV/c","");
      cmpChi2.leg->AddEntry(cmpChi2.vh[0],mytitle[i],"");
      if (!isData) {
         cmpChi2.leg->AddEntry(cmpChi2.vh[0],"Real Track","p");
         cmpChi2.leg->AddEntry(cmpChi2.vh[1],"Fake Track","p");
      }
      cmpChi2.leg->Draw();
   }
   c2->Print(Form("fig/%s_chi2_data%d.gif",tag.Data(),isData));

   TCanvas * c3 = new TCanvas("c3","c3",800,400);
   c3->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c3->cd(i+1);
      Compare cmpChi2(Form("cmpNHit_%d",i),sel,0);
      cmpChi2.vt.push_back(tref);
      if (isData) cmpChi2.vsel.push_back(mysel[i]);
      else cmpChi2.vsel.push_back(mysel[i]&&"!trkFake");
      if (!isData) {
         cmpChi2.vt.push_back(tref);
         cmpChi2.vsel.push_back(mysel[i]&&"trkFake");
      }
      cmpChi2.Init(25,5,30);
      cmpChi2.Project("trkNHit>>");
      cmpChi2.Draw(";# valid hits;# tracks",0,0,0.15,0.67,0.56,0.92);
      if (isData) cmpChi2.leg->AddEntry(cmpChi2.vh[0],"PbPb Data 0-5\%","");
      else cmpChi2.leg->AddEntry(cmpChi2.vh[0],"PYTHIA+HYDJET 0-5\%","");
      cmpChi2.leg->AddEntry(cmpChi2.vh[0],"p_{T} > 10 GeV/c","");
      cmpChi2.leg->AddEntry(cmpChi2.vh[0],mytitle[i],"");
      if (!isData) {
         cmpChi2.leg->AddEntry(cmpChi2.vh[0],"Real Track","p");
         cmpChi2.leg->AddEntry(cmpChi2.vh[1],"Fake Track","p");
      }
      cmpChi2.leg->Draw();
   }
   c3->Print(Form("fig/%s_nhit_data%d.gif",tag.Data(),isData));

   TCanvas * c4 = new TCanvas("c4","c4",800,400);
   c4->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c4->cd(i+1);
      Compare cmpChi2(Form("cmpPtError_%d",i),sel,0);
      cmpChi2.vt.push_back(tref);
      if (isData) cmpChi2.vsel.push_back(mysel[i]);
      else cmpChi2.vsel.push_back(mysel[i]&&"!trkFake");
      if (!isData) {
         cmpChi2.vt.push_back(tref);
         cmpChi2.vsel.push_back(mysel[i]&&"trkFake");
      }
      cmpChi2.Init(100,0,0.2);
      cmpChi2.Project("trkPtError/trkPt>>");
      cmpChi2.Draw(";ptError/pt;# tracks",0,0,0.45,0.67,0.94,0.92);
      if (isData) cmpChi2.leg->AddEntry(cmpChi2.vh[0],"PbPb Data 0-5\%","");
      else cmpChi2.leg->AddEntry(cmpChi2.vh[0],"PYTHIA+HYDJET 0-5\%","");
      cmpChi2.leg->AddEntry(cmpChi2.vh[0],"p_{T} > 10 GeV/c","");
      cmpChi2.leg->AddEntry(cmpChi2.vh[0],mytitle[i],"");
      if (!isData) {
         cmpChi2.leg->AddEntry(cmpChi2.vh[0],"Real Track","p");
         cmpChi2.leg->AddEntry(cmpChi2.vh[1],"Fake Track","p");
      }
      cmpChi2.leg->Draw();
   }
   c4->Print(Form("fig/%s_pterror_data%d.gif",tag.Data(),isData));
}
