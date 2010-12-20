#include <iostream>
#include <map>
#include <TCanvas.h>
#include "TH1.h"
#include "TF1.h"
#include "aliases_dijet.C"
#include "JetFragAna.C"

using namespace std;

void anaJetFrag(
    // Data
    const char * inFile0Name="dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_djcalo_120_50.root",
    TString SrcName = "HCPR_J50U",
    // MC
    //const char * inFile0Name="dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root",
    //TString SrcName = "PyquenUQ80" 
    int doMC=0,
    TString AnaVersion = "test",
    TString modName = "djcalo",
    Bool_t doEvtSel = true,
    TString BkgSubType = "None", // EtaRefl, PhiRot, None
    Double_t CentMin = 0,
    Double_t CentMax = 100,
    Double_t AjMin = 0,
    Double_t AjMax = 1,
    Double_t NrJEtMin = 100,
    Double_t NrJEtMax = 500,
    Double_t AwJEtMin = 50,
    Double_t AwJEtMax = 500,
    Double_t TrkPtMin = 0.5,
    TString evtBase="S1",
    TString DJCutType = "Ana") // Ana
{
  //TH1::SetDefaultSumw2();
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  //TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  TChain * djTree = new TChain("djTree","dijet Tree");
  djTree->Add(inFile0Name);
  aliases_dijet(djTree,doMC);
  cout << " # entries: " << djTree->GetEntries() << endl;

  //=======================================================================================================================
  // Jet Eta Corrections
  //=======================================================================================================================
  TFile *fetacorr = new TFile("etacorr.root");
  std::map<TString,TF1*> jetaCorr;
  jetaCorr["ec1"] = (TF1 *) fetacorr->Get("f1");
  jetaCorr["ec2"] = (TF1 *) fetacorr->Get("f2");
  jetaCorr["ec3"] = (TF1 *) fetacorr->Get("f3");
  jetaCorr["ec4"] = (TF1 *) fetacorr->Get("f4");
  jetaCorr["ec5"] = (TF1 *) fetacorr->Get("f5");
 
  //=======================================================================================================================
  // tracking efficiency correction
  //=======================================================================================================================
  TFile *fTrackingEffCorr = new TFile("trkCorrJet_bass10k.root");
  TH2D *hTrackingEff[10];
  TH2D *hTrackingFake[10];
  for (int i=0;i<5;i++)
  {
     hTrackingEff[i]=(TH2D*)fTrackingEffCorr->FindObjectAny(Form("rEff_cbin%d",i));
     hTrackingFake[i]=(TH2D*)fTrackingEffCorr->FindObjectAny(Form("rFak_cbin%d",i));
  }

  //=======================================================================================================================
  // tracking efficiency correction
  //=======================================================================================================================
  TString outName(Form("draw/jfh%s_%s_%s_Cent%.0fto%.0f_Aj%.0fto%.0f_Sub%s.root",AnaVersion.Data(),SrcName.Data(),modName.Data(),CentMin,CentMax,AjMin*100,AjMax*100,BkgSubType.Data()));
  if (!doEvtSel) {
    outName=TString("nt_")+inFile0Name;
  }
  cout << "Output: " << outName << endl;
  TFile * outf = new TFile(outName,"RECREATE");

  JetFragAna jana(djTree,SrcName,doMC);
  jana.doEvtSel_ = doEvtSel;
  jana.doEtaCorr_ = true;
  jana.doTrackingEffFakeCorr_ = true;
  jana.doJetOnly_ = false;
  jana.cut.CentMin = CentMin;
  jana.cut.CentMax = CentMax;
  jana.cut.SetDJEt(NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,TMath::Pi()*2./3);
  jana.cut.AjMin = AjMin;
  jana.cut.AjMax = AjMax;
  jana.cut.BaseCutType=evtBase;
  jana.cut.DJCutType = DJCutType;
  jana.cut.TrkPtMin = TrkPtMin;
  jana.cut.ConeSize = 0.8;
  if (modName=="djcalo_genp"||modName=="djgen") {
    jana.anaGenpType_=1;
    jana.doTrackingEffFakeCorr_ = false;
  }
  jana.cut.BkgSubType = BkgSubType;
  jana.cut.SetCut();
  jana.cut.DJAnd(TCut(Form("Aj>=%.2f&&Aj<%.2f",AjMin,AjMax)));
  jana.cut.Print(1);
  jana.jetaCorr_ = jetaCorr;

  jana.trackingEtaBin_ = (TH1D*)hTrackingEff[0]->ProjectionX();
  jana.trackingPtBin_ = (TH1D*)hTrackingEff[0]->ProjectionY();

  for (int i=0;i<5;i++) {
     jana.trackingEffCorr_[i] = hTrackingEff[i];
     jana.trackingFakeCorr_[i] = hTrackingFake[i];

  }

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(jana.cut.FinDJCut()) << endl;

  // do ana
  jana.Loop();
  outf->Write();
}
