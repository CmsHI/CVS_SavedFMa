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

void CompareTrk_DataMC(
   float trkPtMin=10,
   float etaCut = 1.6
)
{
   TH1::SetDefaultSumw2();
   TString tag = "compAlgo456HP";
//    TString tag = "compAlgo456CutChi2";
//    TString tag = "compAlgo45";

   // inputs
   TString infdataname="~/hadoopfranklocal/forest/HiForest_promptskim-hihighpt-jet115-v0_part2.root";
   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("anaTrack/trackTree");
   tdata->AddFriend("hiEvtAnalyzer/HiTree",infdataname);
   tdata->AddFriend("icPu5JetAnalyzer/t",infdataname);

   TString infrefname="~/hadoopfranklocal/forest/Hydjet18/DiJet80_MergeTrk_v1_SimTrack_part2.root";
   TFile * infref = new TFile(infrefname);
   TTree * tref = (TTree*)infref->Get("anaTrack/trackTree");
   tref->AddFriend("hiEvtAnalyzer/HiTree",infrefname);
   tref->AddFriend("icPu5JetAnalyzer/t",infrefname);
   
   TCut evtSel = "hiBin<4&&jtpt[0]>80&&abs(jteta[0])<2";
   TCut trkSel = Form("trkQual>0&&trkPt>%.2f&&trkAlgo<=6",trkPtMin);
   TCut trkHGT = "(trkPtError/trkPt<=0.06&&trkNHit>=13&&trkChi2/trkNdof/trkNlayer<=0.15&&abs(trkDz1/trkDzError1)<3&&abs(trkDxy1/trkDxyError1)<3)";
   trkSel = trkSel&&trkHGT;
//    TCut trkSel = Form("trkQual&&trkPt>%.2f&&trkAlgo<=6&&(abs(trkEta)<%.1f||trkChi2/trkNlayer/trkNdof<0.12)",trkPtMin,etaCut);
//    TCut trkSel = Form("trkQual&&trkPt>%.2f&&trkAlgo<6",trkPtMin);
      
   // histograms
   TCut mysel[2] = {Form("abs(trkEta)<%.1f",etaCut),Form("abs(trkEta)>%.1f",etaCut)};
   TString mytitle[2] = {Form("|#eta| < %.1f",etaCut),Form("|#eta| > %.1f",etaCut)};

   TCanvas * c2 = new TCanvas("c2","c2",800,400);
   c2->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c2->cd(i+1);
      Compare cmp(Form("cmpChi2_%d",i),evtSel&&trkSel,10);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
//       cmp.vsel.push_back(mysel[i]&&"!trkFake");
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]&&"trkFake");
      cmp.Init(40,0,0.3);
      cmp.Project("trkChi2/trkNlayer/trkNdof>>");
      cmp.vnorm[2] = cmp.vh[2]->Integral()/cmp.vh[1]->Integral();
      cmp.Draw(";normalized #chi^2;u.n.");
      cmp.Legend(0.33,0.67,0.79,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],Form("p_{T} > %.0f GeV/c",trkPtMin),"");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Data","p");
//       cmp.leg->AddEntry(cmp.vh[1],"MC Real Track","p");
      cmp.leg->AddEntry(cmp.vh[1],"MC","p");
      cmp.leg->AddEntry(cmp.vh[2],"MC Fake","l");
      cmp.leg->Draw();
   }
   c2->Print(Form("fig/%s_chi2_datamc_pt%.0feta%.0f.gif",tag.Data(),trkPtMin,etaCut*10));

   TCanvas * c3 = new TCanvas("c3","c3",800,400);
   c3->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c3->cd(i+1);
      Compare cmp(Form("cmpAlgo_%d",i),evtSel&&trkSel,10);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
//       cmp.vsel.push_back(mysel[i]&&"!trkFake");
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]&&"trkFake");
      cmp.Init(3,4,7);
      cmp.Project("trkAlgo>>");
      cmp.vnorm[2] = cmp.vh[2]->Integral()/cmp.vh[1]->Integral();
      cmp.Draw(";Track Iteration;u.n.",0,1.2);
      cmp.Legend(0.41,0.67,0.82,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],Form("p_{T} > %.0f GeV/c",trkPtMin),"");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Data","p");
//       cmp.leg->AddEntry(cmp.vh[1],"MC Real Track","p");
      cmp.leg->AddEntry(cmp.vh[1],"MC","p");
      cmp.leg->AddEntry(cmp.vh[2],"MC Fake","l");
      cmp.leg->Draw();
   }
   c3->Print(Form("fig/%s_algo_datamc_pt%.0feta%.0f.gif",tag.Data(),trkPtMin,etaCut*10));
}
