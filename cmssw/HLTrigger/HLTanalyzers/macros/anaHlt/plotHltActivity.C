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
#include "aliases_openhlt.C"

using namespace std;

void plotHltActivity(
    TString inFile0Name="~/scratch01/ana/Hydj_BSC_HF_L1Emul/oh0928_v2/openhlt_hiReco_RAW2DIGI_RECO_*_ohtree.root",
    TString outdir = "out/Hydj/BSC_HF_L1Emul/devHLT381",
    TString header="Hydjet 2.76TeV"
    )
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TChain * ohTree = new TChain("HltTree","HI OpenHLT Tree");
  aliases_openhlt(ohTree);
  ohTree->Add(inFile0Name);
  cout << " # entries: " << ohTree->GetEntries() << endl;
  
  // Define Output
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);
  TFile * outf = new TFile(Form("%s/activity.root",outdir.Data()),"RECREATE");

  // Book Histograms
  HisTGroup<TProfile> hgEffVsCent("EffVsCent",900,0,900);
  hgEffVsCent.Add1D("HFAct3_1Hit");
  hgEffVsCent.Add1D("HFAct3_Coinc1");
  hgEffVsCent.Add1D("HFAct3_Coinc2");
  hgEffVsCent.Add1D("L1T34");
  hgEffVsCent.Add1D("L1T40");
  hgEffVsCent.Add1D("L1T41");
  hgEffVsCent.Add1D("L1T8");
  hgEffVsCent.Add1D("L1T9");
  hgEffVsCent.Add1D("L1T10");
  TH1D * hNTrks = new TH1D("hNTrks","hNTrks;# of Selected Trks;# Evt",25,0,100);

  HisTGroup<TH2F> hgTrig2Corr("Trig2Corr",2,0,2,2,0,2);
  hgTrig2Corr.Add2D("L1T40_HFCoinc1");
  hgTrig2Corr.Add2D("L1T41_HFCoinc2");

  // Analyze from tree
  //TString centVar("hiNtracks");
  //TString centVarTitle("# of Selected Trks");
  TString centVar("hiNpixelTracks");
  TString centVarTitle("# of Pixel Trks");
  //TString centVar("Ncharged");
  //TString centVarTitle("# of Charged Particles");
  ohTree->Draw("hiNtracks>>hNTrks","","prof goff");
  ohTree->Draw(Form("HLT_HIActivityHF_Single3:%s>>%s",centVar.Data(),hgEffVsCent.GetH("HFAct3_1Hit")->GetName()),"","prof goff");
  ohTree->Draw(Form("HLT_HIActivityHF_Coincidence3:%s>>%s",centVar.Data(),hgEffVsCent.GetH("HFAct3_Coinc1")->GetName()),"","prof goff");
  ohTree->Draw(Form("HLT_HIActivityHF_Coincidence3_2Hit:%s>>%s",centVar.Data(),hgEffVsCent.GetH("HFAct3_Coinc2")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_BSC_minBias_OR.v0:%s>>%s",centVar.Data(),hgEffVsCent.GetH("L1T34")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_BSC_minBias_threshold1.v0:%s>>%s",centVar.Data(),hgEffVsCent.GetH("L1T40")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_BSC_minBias_threshold2.v0:%s>>%s",centVar.Data(),hgEffVsCent.GetH("L1T41")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_HCAL_HF_MM_or_PP_or_PM.v0:%s>>%s",centVar.Data(),hgEffVsCent.GetH("L1T8")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_HCAL_HF_coincidence_PM.v1:%s>>%s",centVar.Data(),hgEffVsCent.GetH("L1T9")->GetName()),"","prof goff");
  ohTree->Draw(Form("L1Tech_HCAL_HF_MMP_or_MPP.v0:%s>>%s",centVar.Data(),hgEffVsCent.GetH("L1T10")->GetName()),"","prof goff");

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
  TCanvas * cNTrks = new TCanvas("cNTrks","NTrks",500,500);
  cNTrks->SetLogy();
  hNTrks->Draw("E");
  cNTrks->Print(outdir+"/"+"NTrks.gif");

  TCanvas * cHltActEffVsCent = new TCanvas("HltActEffVsCent","HltActEffVsCent",500,500);
  CPlot cpHltActEffVsCent("HltActEffVsCent","HltActEffVsCent",centVarTitle,"Trigger Eff.");
  cpHltActEffVsCent.SetXRange(0,20);
  cpHltActEffVsCent.SetYRange(0,1.1);
  //cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_1Hit"),"ActivityHF3 (Any Hit)","E",kBlue,kOpenSquare);
  //cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_Coinc1"),"ActivityHF3 (1 Hit Coinc.)","E",kBlack,kFullCircle);
  //cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("HFAct3_Coinc2"),"ActivityHF3 (2 Hits Coinc.)","E",kRed,kOpenStar);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T8"),"L1 Bit 8","hist",kGreen+2,kOpenCircle);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T34"),"L1 Bit 34","E",kRed,kOpenStar);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T9"),"L1 Bit 9","E",kBlue,kOpenSquare);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T10"),"L1 Bit 10","E",kBlue,kOpenCircle);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T40"),"L1 Bit 40","E",kBlack,kFullCircle);
  cpHltActEffVsCent.AddHist1D(hgEffVsCent.H("L1T41"),"L1 Bit 41","E",kOrange+2,kOpenDiamond);
  cpHltActEffVsCent.SetLegend(0.29,0.21,0.62,0.43);
  cpHltActEffVsCent.SetLegendHeader(header);
  cpHltActEffVsCent.Draw(cHltActEffVsCent,true);
}
