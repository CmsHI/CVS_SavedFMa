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
    TString inFile0Name="~/scratch01/ana/Hydj_BSC_HF_L1Emul/oh0928_v2/openhlt_hiReco_RAW2DIGI_RECO_*_djana.root",
    TString outdir = "out/Hydj/BSC_HF_L1Emul/devHLT381")
{
  TChain * djTree = new TChain("dijetAna_data_calojet_tower/djTree");
  djTree->Add(inFile0Name);
  aliases_dj(djTree);
  cout << " # entries: " << djTree->GetEntries() << endl;

  gSystem->mkdir(outdir.Data(),kTRUE);
  Int_t NTowerMax=70;


  pair<TString,TString> t = make_pair("name","cut");
  vector<pair<TString,TCut> > trigs;
  trigs.push_back(make_pair("All","1==1"));
  trigs.push_back(make_pair("HfAnyHit","hlt[2]"));
  trigs.push_back(make_pair("HfCoic1","hlt[3]"));
  trigs.push_back(make_pair("HfCoic2","hlt[4]"));

  for (unsigned int i=0; i<trigs.size(); ++i) {
    TCanvas * cHfNvsP = new TCanvas("HfNvsP_"+trigs[i].first,"HfNvsP_"+trigs[i].first,500,500);
    //cHfNvsP->SetLogx();
    //cHfNvsP->SetLogy();
    TH2D * hHfNvsP = new TH2D("hHfNvsP"+trigs[i].first,"hHfNvsP_",NTowerMax,0,NTowerMax,NTowerMax,0,NTowerMax);
    hHfNvsP->SetAxisRange(0.1,400,"X");
    hHfNvsP->SetAxisRange(0.1,400,"Y");
    djTree->Draw("NHfN:NHfP>>hHfNvsP"+trigs[i].first,trigs[i].second,"colz");
    hHfNvsP->SetTitle("HF Towers;# HF+ Towers (E>3GeV);# HF- Towers (E>3GeV)");
    cHfNvsP->Print(Form("%s/trHfNvsP_%s.gif",outdir.Data(),trigs[i].first.Data()));
  }
}
