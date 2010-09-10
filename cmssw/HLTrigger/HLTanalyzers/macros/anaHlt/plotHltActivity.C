#include <iostream>
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
    TString inFile0Name="../openhlt/output/openhlt_try1_1k.root",
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
  TString outdir = "out/Hydj";
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);
  TFile * outf = new TFile(Form("%s/activity.root",outdir.Data()),"RECREATE");

  // Book Histograms
  HisTGroup<TProfile> hgEffVsCent("EffVsCent",40,0,100);
  hgEffVsCent.Add1D("HFAct3_1Hit");
  hgEffVsCent.Add1D("HFAct3_Coinc1");
  hgEffVsCent.Add1D("HFAct3_Coinc2");

  // Analyze from tree
  ohTree->Draw(Form("HLT_ActivityHF3:hiBin*2.5>>%s",hgEffVsCent.GetH("HFAct3_1Hit")->GetName()),"","prof goff");
  ohTree->Draw(Form("HLT_ActivityHF3_Coinc1:hiBin*2.5>>%s",hgEffVsCent.GetH("HFAct3_Coinc1")->GetName()),"","prof goff");
  ohTree->Draw(Form("HLT_ActivityHF3_Coinc2:hiBin*2.5>>%s",hgEffVsCent.GetH("HFAct3_Coinc2")->GetName()),"","prof goff");

  // Plot
  TCanvas * cHltActEffVsCent = new TCanvas("HltActEffVsCent","HltActEffVsCent",500,500);
  CPlot cpHltActEffVsCent("HltActEffVsCent","HltActEffVsCent","Centrality","HLT Eff.");
  cpHltActEffVsCent.SetXRange(0,100);
  cpHltActEffVsCent.SetYRange(0,1.2);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_1Hit"),"ActivityHF3 (Any Hit)","E",kBlue,kOpenSquare);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_Coinc1"),"ActivityHF3 (1 Hit Coinc.)","E",kBlack,kFullCircle);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_Coinc2"),"ActivityHF3 (2 Hits Coinc.)","E",kRed,kOpenStar);
  cpHltActEffVsCent.SetLegend(0.25,0.28,0.58,0.46);
  cpHltActEffVsCent.SetLegendHeader(header);
  cpHltActEffVsCent.Draw(cHltActEffVsCent,true);
}
