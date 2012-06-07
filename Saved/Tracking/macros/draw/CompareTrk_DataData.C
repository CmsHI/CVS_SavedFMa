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

void CompareTrk_DataData()
{
   TH1::SetDefaultSumw2();
   TString tag = "compForest2_myitercalo";
   // inputs
//    TString infdataname="~/hadoopfranklocal/forest/HiForest_promptskim-hihighpt-jet115-v0_part2.root";
//    TString infrefname="~/hadoopfranklocal/forest/Hydjet18/DiJet80_MergeTrk_v1_SimTrack_part2.root";
//    if (isData) infrefname = "~/hadoopfranklocal/forest/HiForest_promptskim-hihighpt-jet115-v0_part2.root";
   TString infdataname="/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest-promptskim-hihighpt-hltjet80-pt90-v2_v3_part.root";
   TString infrefname = "/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest_promptskim-hihighpt-jet115-v2MgTrk.root";
//    TString infrefname = "/mnt/hadoop/cms/store/user/velicanu/forest/HiForestDijet_v7.root";

   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("mergedTrack/trackTree");
   tdata->AddFriend("hiEvtAnalyzer/HiTree",infdataname);
   tdata->AddFriend("icPu5JetAnalyzer/t",infdataname);
   cout << infdata->GetName() << endl;

   TFile * infref = new TFile(infrefname);   
   TTree * tref = (TTree*)infref->Get("anaTrack/trackTree");
   tref->AddFriend("hiEvtAnalyzer/HiTree",infrefname);
   tref->AddFriend("icPu5JetAnalyzer/t",infrefname);
   cout << infref->GetName() << endl;
   
//    TCut sel = "hiBin<4&&trkQual&&trkPt>2&&trkAlgo>=0";
   TCut sel = "hiBin<4&&jtpt[0]>120&&trkAlgo==4";
      
   // histograms
   TCut mysel[2] = {"trkPt>2&&abs(trkEta)<1.6","trkPt>2&&abs(trkEta)>1.6"};
   TString mytitle[2] = {"|#eta| < 1.6","|#eta| > 1.6"};

   TCanvas * c2 = new TCanvas("c2","c2",800,400);
   c2->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c2->cd(i+1);
      Compare cmp(Form("cmpChi2_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]);
      cmp.Init(100,0,0.5);
      cmp.Project("trkChi2/trkNlayer/trkNdof>>");
      cmp.Draw(";normalized #chi^2;# tracks");
      cmp.Legend(0.45,0.67,0.94,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data 0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],"p_{T} > 2 GeV/c","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Forest2","p");
      cmp.leg->AddEntry(cmp.vh[1],"Forest1","p");
      cmp.leg->Draw();
   }
   c2->Print(Form("fig/%s_chi2.gif",tag.Data()));

   TCanvas * c3 = new TCanvas("c3","c3",800,400);
   c3->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c3->cd(i+1);
      Compare cmp(Form("cmpNHit_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]);
      cmp.Init(25,5,30);
      cmp.Project("trkNHit>>");
      cmp.Draw(";# valid hits;# tracks");
      cmp.Legend(0.21,0.67,0.70,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data 0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],"p_{T} > 2 GeV/c","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Forest2","p");
      cmp.leg->AddEntry(cmp.vh[1],"Forest1","p");
      cmp.leg->Draw();
   }
   c3->Print(Form("fig/%s_nhit.gif",tag.Data()));   

   TCanvas * c4 = new TCanvas("c4","c4",800,400);
   c4->Divide(2,1);
   for (int i=0; i<2; ++i) {
      c4->cd(i+1);
      Compare cmp(Form("cmpPtError_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]);
      cmp.Init(100,0,0.1);
      cmp.Project("trkPtError/trkPt>>");
      cmp.Draw(";ptError/pt;# tracks");
      cmp.Legend(0.45,0.67,0.94,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data 0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],"p_{T} > 2 GeV/c","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Forest2","p");
      cmp.leg->AddEntry(cmp.vh[1],"Forest1","p");
      cmp.leg->Draw();
   }
   c4->Print(Form("fig/%s_pterror.gif",tag.Data()));//    for (int i=0; i<2; ++i) {

   TCanvas * cpt = new TCanvas("cpt","cpt",800,400);
   cpt->Divide(2,1);
   for (int i=0; i<2; ++i) {
      cpt->cd(i+1);
      gPad->SetLogy();
      Compare cmp(Form("cmpPt_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]);
      cmp.Init(100,0,100);
      cmp.Project("trkPt>>");
      cmp.Draw(";p_{T} (GeV/c);# tracks",1e-5,5);
      cmp.Legend(0.45,0.67,0.94,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data 0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],"p_{T} > 2 GeV/c","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Forest2","p");
      cmp.leg->AddEntry(cmp.vh[1],"Forest1","p");
      cmp.leg->Draw();
   }
   cpt->Print(Form("fig/%s_pt.gif",tag.Data()));//    for (int i=0; i<2; ++i) {

   TCut mysel2[2] = {"abs(trkPt)>2","abs(trkPt)>10"};
   TString mytitle2[2] = {"p_{T} > 2 GeV/c","p_{T} > 10 GeV/c"};
   TCanvas * ceta = new TCanvas("ceta","ceta",800,400);
   ceta->Divide(2,1);
   for (int i=0; i<2; ++i) {
      ceta->cd(i+1);
//       gPad->SetLogy();
      Compare cmp(Form("cmpEta_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel2[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel2[i]);
      cmp.Init(48,-2.4,2.4);
      cmp.Project("trkEta>>");
      cmp.Draw(";p_{T} (GeV/c);# tracks");
      cmp.Legend(0.45,0.67,0.94,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data 0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle2[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Forest2","p");
      cmp.leg->AddEntry(cmp.vh[1],"Forest1","p");
      cmp.leg->Draw();
   }
   ceta->Print(Form("fig/%s_eta.gif",tag.Data()));//    for (int i=0; i<2; ++i) {

   TCut mysel3[1] = {"abs(jteta[0])<2"};
   TString mytitle3[1] = {"jet |#eta|<2"};
   TCanvas * cjpt = new TCanvas("cjpt","cjpt",400,400);
   for (int i=0; i<1; ++i) {
      gPad->SetLogy();
      Compare cmp(Form("cmpJPt_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel2[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel2[i]);
      cmp.Init(100,90,290);
      cmp.Project("jtpt[0]>>");
      cmp.Draw(";jet p_{T} (GeV/c);# jets",1e-4,1);
      cmp.Legend(0.45,0.67,0.94,0.92);
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data 0-10\%","");
      cmp.leg->AddEntry(cmp.vh[0],mytitle3[i],"");
      cmp.leg->AddEntry(cmp.vh[0],"Forest2","p");
      cmp.leg->AddEntry(cmp.vh[1],"Forest1","p");
      cmp.leg->Draw();
   }
}
