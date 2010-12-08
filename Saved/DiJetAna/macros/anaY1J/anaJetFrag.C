#include <iostream>
#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
#include <TCanvas.h>
using namespace std;

void anaJetFrag(int doMC=0,
    TString evtBase="S1",
    TString AnaVersion = "a1006",
    TString modName = "djcalo",
    Double_t CentMin = 0,
    Double_t CentMax = 100,
    Double_t NrJEtMin = 120,
    Double_t NrJEtMax = 500,
    Double_t AwJEtMin = 50,
    Double_t AwJEtMax = 500,
    Double_t AwJEtaMax = 2.,
    Double_t JDPhiMin = PI*2./3,
    Double_t AjMin = 0.24,
    Double_t AjMax = 1,
    TString DJCutType = "Ana", // Ana
    TString BkgSubType = "EtaRefl", // EtaRefl, PhiRot, None
    // Data
    const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2.root",
    TString SrcName = "HCPR_J50U")
    // MC
    //const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_PyquenUQ80_hiGoodMergedTracks_OfficialSelv2.root",
    //TString SrcName = "PyquenUQ80")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  djTree->Add(inFile0Name);
  aliases_dijet(djTree,doMC);
  cout << " # entries: " << djTree->GetEntries() << endl;

  TFile * outf = new TFile(Form("jfh_%s_%s_Cent%.0fto%.0f_Aj%.0fto%.0f_Sub%s.root",SrcName.Data(),modName.Data(),CentMin,CentMax,AjMin*100,AjMax*100,BkgSubType.Data()),"RECREATE");
  JetFragAna jana(djTree,SrcName,doMC);
  jana.cut.CentMin = CentMin;
  jana.cut.CentMax = CentMax;
  jana.cut.SetDJEt(NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,JDPhiMin);
  jana.cut.AjMin = AjMin;
  jana.cut.AjMax = AjMax;
  jana.cut.BaseCutType=evtBase;
  jana.cut.DJCutType = DJCutType;
  jana.cut.TrkPtMin = 0.5;
  jana.cut.BkgSubType = BkgSubType;
  jana.cut.SetCut();
  jana.cut.Print(1);

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(jana.cut.FinDJCut()) << endl;

  // do ana
  jana.Loop();
  outf->Write();
}
