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

void ParticleSpecies()
{
  TH1::SetDefaultSumw2();
  
//   TString infrefname="/mnt/hadoop/cms/store/user/tuos/pPb/HiForest/5_3_3_patch3/pPbAMPT_Evening_5_3_3_v0_HiForest2_v07.root";
//   TString infrefname="/mnt/hadoop/cms/store/user/tuos/pPb/HiForest/5_3_3_patch3/pPbHijing_5_3_3_v0_HiForest2_v07.root";
  TString infrefname="/net/hidsk0001/d00/scratch/frankma/tmp/HiForest-Hydjet1.8-STARTHI44_V12-Sep20.root";
  cout << infrefname << endl;

//   TString tag = "AMPT";
//   TString tag = "HIJING";
  TString tag = "Hydjet";
  
  TFile * infref = new TFile(infrefname);
  
//   TTree * tref = (TTree*)infref->Get("ppTrack/trackTree");
  TTree * tref = (TTree*)infref->Get("anaTrack/trackTree");
//   tref->AddFriend("hiEvtAnalyzer/HiTree",infrefname);
  
  TCut sel = "";
  TCut psel = "abs(pEta)<2.4";

  // histograms

  for (int i=0; i<1; ++i) {
    TCanvas * csp = new TCanvas(Form("csp%d",i),Form("csp%d",i),800,400);
    csp->Divide(2,1);
    csp->cd(1);
    gPad->SetLogy();
    Compare cmp0(Form("cmp%d",i),sel,0);
    cmp0.vt.push_back(tref); cmp0.vsel.push_back(psel&&"");
    cmp0.vt.push_back(tref); cmp0.vsel.push_back(psel&&"abs(pPId)==211||abs(pPId)==321");
    cmp0.vt.push_back(tref); cmp0.vsel.push_back(psel&&"abs(pPId)==411||abs(pPId)==431||abs(pPId)==521");
    cmp0.Init(20,0,12);
    cmp0.Project("pPt>>");
    cmp0.Draw(";Particle p_{T} (GeV/c);#",1,1e6);
    cmp0.Legend(0.45,0.67,0.94,0.92);
    cmp0.leg->AddEntry(cmp0.vh[0],tag,"");
    cmp0.leg->AddEntry(cmp0.vh[0],"All","p");
    cmp0.leg->AddEntry(cmp0.vh[1],"#pi^{#pm},K^{#pm}","p");
    cmp0.leg->AddEntry(cmp0.vh[2],"D^{#pm},D_{s}^{#pm},B^{#pm}","p");
    cmp0.leg->Draw();
    csp->cd(2);
    TH1D * hr1 = cmp0.Ratio("Fraction",1,0);
    hr1->SetAxisRange(0,1.2,"Y");
    hr1->Draw("E");
    TH1D * hr2 = cmp0.Ratio("Fraction",2,0);
    hr2->Draw("sameE");
    csp->Print(Form("fig/sp%d_%s.gif",i,tag.Data()));
  }
}
