#include <iostream>
#include "TH1D.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TMath.h"

#include "Saved/Utilities/macros/cplot/CPlot.h"           // helper class for plots
#include "Saved/Utilities/macros/histogram/HisTGroup.h"

using namespace std;

void plotHltActivity(
    TString inFile0Name="../openhlt/output/openhlt_Hydjet_BSC_HF_v2_100.root",
    TString header="Hydjet 2.76TeV"
    )
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TChain * ohTree = new TChain("HltTree","HI OpenHLT Tree");
  ohTree->Add(inFile0Name);
  cout << " # entries: " << ohTree->GetEntries() << endl;
  
  // Define Output
  TString outdir = "out/Hydj/BSC_HF_L1Emul";
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);
  TFile * outf = new TFile(Form("%s/activity.root",outdir.Data()),"RECREATE");

  // Book Histograms
  HisTGroup<TProfile> hgEffVsCent("EffVsCent",40,0,100);
  hgEffVsCent.Add1D("HFAct3_1Hit");
  hgEffVsCent.Add1D("HFAct3_Coinc1");
  hgEffVsCent.Add1D("HFAct3_Coinc2");
  hgEffVsCent.Add1D("L1T40");
  hgEffVsCent.Add1D("L1T41");

  HisTGroup<TH2F> hgTrig2Corr("Trig2Corr",2,0,2,2,0,2);
  hgTrig2Corr.Add2D("L1T40_HFCoinc1");
  hgTrig2Corr.Add2D("L1T41_HFCoinc2");

  // Analyze from tree
  ohTree->Draw(Form("HLT_HIActivityHF_Single3:hiBin*2.5>>%s",hgEffVsCent.GetH("HFAct3_1Hit")->GetName()),"","prof goff");
  ohTree->Draw(Form("HLT_HIActivityHF_Coincidence3:hiBin*2.5>>%s",hgEffVsCent.GetH("HFAct3_Coinc1")->GetName()),"","prof goff");
  ohTree->Draw(Form("HLT_HIActivityHF_Coincidence3_2Hit:hiBin*2.5>>%s",hgEffVsCent.GetH("HFAct3_Coinc2")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_BSC_minBias_threshold1.v0:hiBin*2.5>>%s",hgEffVsCent.GetH("L1T40")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_BSC_minBias_threshold2.v0:hiBin*2.5>>%s",hgEffVsCent.GetH("L1T41")->GetName()),"","prof goff");

  TCanvas * cL1T40_HFCoinc1 = new TCanvas("L1T40_HFCoinc1","L1T40_HFCoinc1",500,500);
  ohTree->Draw(Form("L1Tech_BSC_minBias_threshold1.v0:HLT_HIActivityHF_Coincidence3>>%s",hgTrig2Corr.GetH("L1T40_HFCoinc1")->GetName()),"","box");
  CPlot cpL1T40_HFCoinc1("L1T40_HFCoinc1","L1T40_HFCoinc1","ActivityHF3 (1Hit Coinc.)","L1T40");
  cpL1T40_HFCoinc1.AddHist2D(hgTrig2Corr.H("L1T40_HFCoinc1"),"box");
  cpL1T40_HFCoinc1.Draw(cL1T40_HFCoinc1,true);

  TCanvas * cL1T41_HFCoinc2 = new TCanvas("L1T41_HFCoinc2","L1T41_HFCoinc2",500,500);
  ohTree->Draw(Form("L1Tech_BSC_minBias_threshold2.v0:HLT_HIActivityHF_Coincidence3_2Hit>>%s",hgTrig2Corr.GetH("L1T41_HFCoinc2")->GetName()),"","goff");
  CPlot cpL1T41_HFCoinc2("L1T41_HFCoinc2","L1T41_HFCoinc2","ActivityHF3 (2Hit Coinc.)","L1T41");
  cpL1T41_HFCoinc2.AddHist2D(hgTrig2Corr.H("L1T41_HFCoinc2"),"box");
  cpL1T41_HFCoinc2.Draw(cL1T41_HFCoinc2,true);

  // Plot
  TCanvas * cHltActEffVsCent = new TCanvas("HltActEffVsCent","HltActEffVsCent",500,500);
  CPlot cpHltActEffVsCent("HltActEffVsCent","HltActEffVsCent","Centrality","HLT Eff.");
  cpHltActEffVsCent.SetXRange(0,100);
  cpHltActEffVsCent.SetYRange(0,1.1);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T40"),"L1 Bit 40","E",kGreen+2,kOpenCircle);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T41"),"L1 Bit 41","E",kOrange+2,kOpenDiamond);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_1Hit"),"ActivityHF3 (Any Hit)","E",kBlue,kOpenSquare);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_Coinc1"),"ActivityHF3 (1 Hit Coinc.)","E",kBlack,kFullCircle);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_Coinc2"),"ActivityHF3 (2 Hits Coinc.)","E",kRed,kOpenStar);
  cpHltActEffVsCent.SetLegend(0.29,0.21,0.62,0.43);
  cpHltActEffVsCent.SetLegendHeader(header);
  cpHltActEffVsCent.Draw(cHltActEffVsCent,true);
}
