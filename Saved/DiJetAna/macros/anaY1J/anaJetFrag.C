#include <iostream>
#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
#include <TCanvas.h>
using namespace std;

void anaJetFrag(int doMC=0,
    TString evtBase="S1",
    TString AnaVersion = "a1006",
    TString modName = "djcalo",
    Double_t NrJEtMin = 120,
    Double_t NrJEtMax = 500,
    Double_t AwJEtMin = 50,
    Double_t AwJEtMax = 500,
    Double_t AwJEtaMax = 2.,
    Double_t JDPhiMin = 2.5,
    TString DJCutType = "Ana", // Ana, Refl, Rotate
    TString TrkCutType = "Ana",
    TString BkgSubType = "EtaRefl", // EtaRefl, PhiRot
    const char * inFile0Name="dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root",
    TString SrcName = "HydjUQDJ80")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  djTree->Add("~/scratch01/ana/merge/dj_HCPR-J50U-OfficialJSON_hiGoodMergedTracks_Runs_150883_to_152485_RECOPAT-v1_OfficialSelNoVtxPatchv1.root");
  djTree->Add("~/scratch01/ana/merge/dj_HCPR-J50U-Pre-OfficialJSON_hiGoodMergedTracks_Runs_152652_to_152791_RECOPAT-v1_OfficialSelNoVtxPatchv1.root");
  aliases_dijet(djTree,doMC);
  cout << " # entries: " << djTree->GetEntries() << endl;

  TFile * outf = new TFile(Form("jetFragHists_%s.root",BkgSubType.Data()),"RECREATE");
  JetFragAna jana(djTree,SrcName,doMC);
  jana.cut.SetDJEt(NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,JDPhiMin);
  jana.cut.BaseCutType=evtBase;
  jana.cut.DJCutType = DJCutType;
  jana.cut.TrkCutType = TrkCutType;
  jana.cut.BkgSubType = BkgSubType;
  jana.cut.SetCut();

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(jana.cut.FinDJCut()) << endl;

  // do ana
  jana.Loop();
  outf->Write();
}
