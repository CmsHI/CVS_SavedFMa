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

void CompareTrk_DataMC_prof(
   float trkPtMin=2,
   float etaCut = 1.6
)
{
   TH1::SetDefaultSumw2();
   TString tag = "comp_algo6Ratio_allQual";

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
   TCut trkSel = Form("trkQual>=0&&trkPt>%.2f",trkPtMin);
      
   // histograms
   TCut mysel[2] = {Form("abs(trkEta)<%.1f",etaCut),Form("abs(trkEta)>%.1f",etaCut)};
   TCut ratCut[2] = {"trkAlgo==4","trkAlgo==6"};
   TString mytitle[2] = {Form("|#eta| < %.1f",etaCut),Form("|#eta| > %.1f",etaCut)};

   const int nPtBin = 18;
   float ptBins[nPtBin+1] = {0.5,0.75,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90};
   TCanvas * c3 = new TCanvas("c3","c3",800,400);
   c3->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c3->cd(i+1);
      Compare cmp(Form("cmpPt_%d",i),evtSel&&trkSel,0);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]&&ratCut[0]);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]&&ratCut[1]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]&&ratCut[0]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]&&ratCut[1]);
      cmp.Init(nPtBin,ptBins);
      cmp.Project("trkPt>>");
      TH1D * hDataRat = (TH1D*)cmp.vh[1]->Clone(Form("%s_rat",cmp.vh[0]->GetName()));
      hDataRat->Divide(cmp.vh[0]);
      TH1D * hMcRat = (TH1D*)cmp.vh[3]->Clone(Form("%s_rat",cmp.vh[3]->GetName()));
      hMcRat->Divide(cmp.vh[2]);
      hDataRat->Draw("E");
      hMcRat->SetMarkerStyle(kOpenCircle);
      hMcRat->SetMarkerColor(kRed);
      hMcRat->SetLineColor(kRed);
      hMcRat->Draw("sameE");
      cmp.Legend(0.41,0.67,0.82,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(hDataRat,"Data","p");
      cmp.leg->AddEntry(hMcRat,"MC","p");
      cmp.leg->Draw();
   }
   c3->Print(Form("fig/%s_v_pt_datamc_pt%.0feta%.0f.gif",tag.Data(),trkPtMin,etaCut));

   TCanvas * c4 = new TCanvas("c4","c4",800,400);
   c4->Divide(2,1);
   TCut myselPt[2] = {"trkPt>2","trkPt>10"};
   TString mytitlePt[2] = {Form("p_{T} > 2 GeV/c"),Form("p_{T} > 10 GeV/c")};
   for (int i=0; i<2; ++i) {
      c4->cd(i+1);
      Compare cmp(Form("cmpEta_%d",i),evtSel&&trkSel,0);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(myselPt[i]&&ratCut[0]);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(myselPt[i]&&ratCut[1]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(myselPt[i]&&ratCut[0]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(myselPt[i]&&ratCut[1]);
      cmp.Init(24,-2.4,2.4);
      cmp.Project("trkEta>>");
      TH1D * hDataRat = (TH1D*)cmp.vh[1]->Clone(Form("%s_rat",cmp.vh[0]->GetName()));
      hDataRat->Divide(cmp.vh[0]);
      TH1D * hMcRat = (TH1D*)cmp.vh[3]->Clone(Form("%s_rat",cmp.vh[3]->GetName()));
      hMcRat->Divide(cmp.vh[2]);
      hDataRat->Draw("E");
      hMcRat->SetMarkerStyle(kOpenCircle);
      hMcRat->SetMarkerColor(kRed);
      hMcRat->SetLineColor(kRed);
      hMcRat->Draw("sameE");
      cmp.Legend(0.41,0.67,0.82,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],mytitlePt[i],"");
      cmp.leg->AddEntry(hDataRat,"Data","p");
      cmp.leg->AddEntry(hMcRat,"MC","p");
      cmp.leg->Draw();
   }
   c4->Print(Form("fig/%s_v_eta_datamc_pt%.0f.gif",tag.Data(),trkPtMin));
}
