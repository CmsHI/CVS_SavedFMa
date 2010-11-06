#include <iostream>
#include <vector>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TLegend.h"
using namespace std;

TH1D * plotOne(TTree * oh, Int_t runNum, TString trig,Int_t maxLumi,TString opt="hist")
{
  if (!oh->GetBranchStatus(trig)) return 0;

  TString histName = "hTrigLumi_"+trig;
  Int_t numBins = maxLumi/10.;
  TH1D * hLumi = new TH1D(histName,trig+";Lumi Section;dN^{Evt}/d(LumiSec)",numBins,0,maxLumi);
  TString evtSel(Form("Run==%d&&%s",runNum,trig.Data()));
  cout << evtSel << endl;
  cout << " # of events: " << oh->Draw("LumiBlock>>"+histName,evtSel,opt) << endl;
  hLumi->Scale(1./hLumi->GetBinWidth(1));
  hLumi->SetMinimum(1e-3);
  //hLumi->SetMaximum(hLumi->GetMaximum()*1.6);
  if (trig.Contains("NotBsc2")) {
    hLumi->SetMarkerColor(kBlue);
    hLumi->SetLineColor(kBlue);
    hLumi->SetMarkerStyle(kOpenStar);
  }
  if (trig.Contains("BptxAND")) {
    hLumi->SetMarkerColor(kGreen+2);
    hLumi->SetLineColor(kGreen+2);
    hLumi->SetMarkerStyle(kFullCircle);
  }
  if (trig.Contains("NotBsc2") && trig.Contains("BptxAND")) {
    hLumi->SetMarkerColor(kRed);
    hLumi->SetLineColor(kRed);
    hLumi->SetMarkerStyle(kOpenSquare);
  }
  return hLumi;
}

void plotTrigLumi(
    TString infiles0="~/scratch02/data/HIAllPhysics/HR10All-PR1-v0/*.root",
    Int_t runNum = 150063,
    TString outdir="out/HR10All-PR1-v0")
{
  gStyle->SetTitleColor(1);
  TChain * oh = new TChain("hltanalysis/HltTree");
  oh->Add(infiles0);
  cout << "Tree: " << infiles0 << " has # events: " << oh->GetEntries() << endl;
  // Outputs
  outdir+=Form("/run%d",runNum);
  gSystem->mkdir(outdir.Data(),kTRUE);

  vector<TString> trigs;
  trigs.push_back("L1Tech_BPTX_plus_OR_minus.v0");
  // MB
  trigs.push_back("L1_BscMinBiasInnerThreshold1");
  trigs.push_back("L1_HcalHfCoincidencePm");
  trigs.push_back("L1_ZdcCaloPlus_ZdcCaloMinus");
  trigs.push_back("L1Tech_ZDC_Scint_loose_vertex.v0");
  // Hard
  trigs.push_back("L1_SingleJet20");
  trigs.push_back("L1_SingleEG2");

  Int_t maxLumi = oh->GetMaximum("LumiBlock");
  for (UInt_t i=0; i<trigs.size(); ++i) {
    if (maxLumi<0.001) continue;
    TCanvas * cLumi = new TCanvas("c"+trigs[i],"c"+trigs[i],500,500);
    cLumi->SetLogy();
    TString trigBptx(trigs[i]+"_BptxAND");
    TString trigVetoBsc2(trigs[i]); trigVetoBsc2.ReplaceAll("L1","L1_NotBsc2");
    TString trigBptxVetoBsc2(trigs[i]); trigBptxVetoBsc2.ReplaceAll("L1","L1_NotBsc2_BptxAND");
    TH1D * hLumi = plotOne(oh,runNum,trigs[i],maxLumi,"hist");
    TH1D * hLumiBptx = plotOne(oh,runNum,trigBptx,maxLumi,"p same");
    TH1D * hLumiVetoBsc2 = plotOne(oh,runNum,trigVetoBsc2,maxLumi,"p same");
    TH1D * hLumiBptxVetoBsc2 = plotOne(oh,runNum,trigBptxVetoBsc2,maxLumi,"p same");
    // Clarify Plot
    TLegend *l= new TLegend(0.185,0.17,0.92,0.35);
    l->SetHeader(Form("Run %d",runNum));
    if (hLumi) l->AddEntry(hLumi,trigs[i],"L");
    if (hLumiBptx) l->AddEntry(hLumiBptx,trigBptx,"Lp");
    if (hLumiVetoBsc2) l->AddEntry(hLumiVetoBsc2,trigVetoBsc2,"Lp");
    if (hLumiBptxVetoBsc2) l->AddEntry(hLumiBptxVetoBsc2,trigBptxVetoBsc2,"Lp");
    l->SetFillColor(0);
    //l->SetFillStyle(0);
    l->SetLineColor(0);
    l->SetTextSize(0.03);
    l->Draw();
    cLumi->Print(outdir+"/"+trigs[i]+".gif");
  }
}
