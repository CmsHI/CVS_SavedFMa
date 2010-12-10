#include <iostream>
#include <map>
#include <TCanvas.h>
#include "TH1.h"
#include "TF1.h"
#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
using namespace std;

void anaJetFrag(int doMC=0,
    TString evtBase="S1",
    TString AnaVersion = "CorrEtaHighPt",
    TString modName = "djcalo",
    Double_t CentMin = 0,
    Double_t CentMax = 10,
    Double_t NrJEtMin = 120,
    Double_t NrJEtMax = 500,
    Double_t AwJEtMin = 50,
    Double_t AwJEtMax = 500,
    Double_t JDPhiMin = TMath::Pi()*2./3,
    Double_t AjMin = 0.24,
    Double_t AjMax = 1,
    TString DJCutType = "Ana", // Ana
    TString BkgSubType = "EtaRefl", // EtaRefl, PhiRot, None
    // Data
    const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2.root",
    TString SrcName = "HCPR_J50U")
    // MC
    //const char * inFile0Name="dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root",
    //TString SrcName = "PyquenUQ80")
{
  //TH1::SetDefaultSumw2();
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  djTree->Add(inFile0Name);
  aliases_dijet(djTree,doMC);
  cout << " # entries: " << djTree->GetEntries() << endl;

  // Corrections
  TFile *fetacorr = new TFile("etacorr.root");
  std::map<TString,TF1*> jetaCorr;
  jetaCorr["ec1"] = (TF1 *) fetacorr->Get("f1");
  jetaCorr["ec2"] = (TF1 *) fetacorr->Get("f2");
  jetaCorr["ec3"] = (TF1 *) fetacorr->Get("f3");
  jetaCorr["ec4"] = (TF1 *) fetacorr->Get("f4");
  jetaCorr["ec5"] = (TF1 *) fetacorr->Get("f5");

  TFile * outf = new TFile(Form("jfh%s_%s_%s_Cent%.0fto%.0f_Aj%.0fto%.0f_Sub%s.root",AnaVersion.Data(),SrcName.Data(),modName.Data(),CentMin,CentMax,AjMin*100,AjMax*100,BkgSubType.Data()),"RECREATE");
  JetFragAna jana(djTree,SrcName,doMC);
  jana.doEtaCorr_ = true;
  jana.doJetOnly_ = false;
  jana.cut.CentMin = CentMin;
  jana.cut.CentMax = CentMax;
  jana.cut.SetDJEt(NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,JDPhiMin);
  jana.cut.AjMin = AjMin;
  jana.cut.AjMax = AjMax;
  jana.cut.BaseCutType=evtBase;
  jana.cut.DJCutType = DJCutType;
  jana.cut.TrkPtMin = 0.5;
  jana.cut.ConeSize = 0.8;
  if (modName=="djcalo_genp"||modName=="djgen") jana.anaGenpType_=1;
  jana.cut.BkgSubType = BkgSubType;
  jana.cut.SetCut();
  jana.cut.DJAnd(TCut(Form("Aj>=%.2f&&Aj<%.2f",AjMin,AjMax)));
  jana.cut.Print(1);
  jana.jetaCorr_ = jetaCorr;

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(jana.cut.FinDJCut()) << endl;

  // do ana
  jana.Loop();
  outf->Write();
}
