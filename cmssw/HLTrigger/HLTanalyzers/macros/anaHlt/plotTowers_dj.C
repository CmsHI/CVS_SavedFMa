#include <utility>
#include <iostream>
#include "TSystem.h"
#include "TChain.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "aliases_dj.C"
using namespace std;

void plotTowers_dj(
    TString infilename="../openhlt/output/dijetAna_Hydjet_BSC_HF_v2_800.root",
    TString outdir = "out/Hydj/BSC_HF_L1Emul")
{
  TChain * djTree = new TChain("dijetAna_data_calojet_tower/djTree");
  djTree->Add(infilename);
  aliases_dj(djTree);

  gSystem->mkdir(outdir.Data(),kTRUE);

  pair<TString,TString> t = make_pair("name","cut");
  vector<pair<TString,TCut> > trigs;
  trigs.push_back(make_pair("All","1==1"));
  trigs.push_back(make_pair("HfAnyHit","hlt[5]"));
  trigs.push_back(make_pair("HfCoic1","hlt[6]"));
  trigs.push_back(make_pair("HfCoic2","hlt[7]"));

  for (unsigned int i=0; i<trigs.size(); ++i) {
    TCanvas * cHfNvsP = new TCanvas("HfNvsP_"+trigs[i].first,"HfNvsP_"+trigs[i].first,500,500);
    cHfNvsP->SetLogx();
    cHfNvsP->SetLogy();
    TH2D * hHfNvsP = new TH2D("hHfNvsP"+trigs[i].first,"hHfNvsP_",100,0,100,100,0,100);
    hHfNvsP->SetAxisRange(0.1,400,"X");
    hHfNvsP->SetAxisRange(0.1,400,"Y");
    djTree->Draw("NHfN:NHfP>>hHfNvsP"+trigs[i].first,trigs[i].second,"box");
    hHfNvsP->SetTitle("HF Towers;# HF+ Towers (E>3GeV);# HF- Towers (E>3GeV)");
    cHfNvsP->Print(Form("%s/trHfNvsP_%s.gif",outdir.Data(),trigs[i].first.Data()));
  }
}
