#include <iostream>
#include <vector>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TStyle.h"
#include "TString.h"
#include "TLegend.h"
using namespace std;

TH1D * plotOne(TTree * oh, Int_t runNum, TString trig,Int_t maxLumi)
{
  TString histName = "hTrigLumi_"+trig;
  Int_t numBins = maxLumi/10.;
  TH1D * hLumi = new TH1D(histName,trig+";Lumi Section;# Events/Lumi",numBins,0,maxLumi);
  TCanvas * cLumi = new TCanvas("c"+trig,"c"+trig,500,500);
  TString evtSel(Form("Run==%d&&%s",runNum,trig.Data()));
  cout << evtSel << endl;
  cout << " # of events: " << oh->Draw("LumiBlock>>"+histName,evtSel,"hist") << endl;
  hLumi->Scale(1./hLumi->GetBinWidth(1));
  hLumi->SetMinimum(0);
  hLumi->SetMaximum(hLumi->GetMaximum()*1.4);
  return hLumi;
}

void plotTrigLumi(TString infiles0="hltana*.root",
    Int_t runNum = 150063)
{
  gStyle->SetTitleColor(1);
  TChain * oh = new TChain("hltanalysis/HltTree");
  oh->Add(infiles0);
  cout << "Tree: " << infiles0 << " has # events: " << oh->GetEntries() << endl;

  vector<TString> trigs;
  trigs.push_back("L1Tech_BPTX_plus_OR_minus.v0");
  trigs.push_back("L1_BscMinBiasInnerThreshold1_BptxAND");
  trigs.push_back("L1_NotBsc2_BscMinBiasThreshold1");
  trigs.push_back("L1_HcalHfCoincidencePm");
  trigs.push_back("L1Tech_ZDC_Scint_loose_vertex.v0");

  Int_t maxLumi = oh->GetMaximum("LumiBlock");
  for (UInt_t i=0; i<trigs.size(); ++i) {
    TH1D * hLumi = plotOne(oh,runNum,trigs[i],maxLumi);
    TLegend *l= new TLegend(0.2,0.85,0.35,0.94);
    l->SetHeader(Form("Run %d",runNum));
    l->AddEntry(hLumi,trigs[i],"L");
    l->SetFillColor(0);
    l->SetFillStyle(0);
    l->SetLineColor(0);
    l->SetTextSize(0.035);
    l->Draw();
  }
}
