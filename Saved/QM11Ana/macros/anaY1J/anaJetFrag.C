#include <iostream>
#include <map>
#include <TCanvas.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TROOT.h"
#include "TChain.h"
#include "aliases_dijet.C"
#include "JetFragAna.C"

using namespace std;

void anaJetFrag(
    // Data
    //const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root",
    //TString inFile0Name = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_djcalo_120_50.root",
    TString inFile0Name = "../highPtFragAna/ntout/tranav1_datamattv2_akpu3pf.root",
    Bool_t doEvtSel = false,
    Bool_t doEtaCorr = false,
    Bool_t doReWeight = false,
    TString BkgSubType = "None", // EtaRefl, PhiRot, None
    int doMC=0,
    // MC
    //const char * inFile0Name="dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root",
    //TString SrcName = "PyquenUQ80" 
    TString SrcName = "HAZS_J35U",
    TString AnaVersion = "testtree",
    TString modName = "akpu3pf",
    Double_t CentMin = 0,
    Double_t CentMax = 30,
    Double_t AjMin = 0,
    Double_t AjMax = 1,
    Double_t NrJEtMin = 100,
    Double_t NrJEtMax = 500,
    Double_t AwJEtMin = 40,
    Double_t AwJEtMax = 500,
    Double_t TrkPtMin = 0.5, // 0.5 for paper
    TString evtBase="S1",
    TString DJCutType = "Ana", // Ana
    Int_t mixOffset=0)
{
  // Load Class
  gROOT->ProcessLine(".L JetFragAna.C+");

  //TH1::SetDefaultSumw2();
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  //TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  //TChain * djTree = new TChain("djTree","dijet Tree");
  TChain * djTree = new TChain("tjf","dijet Tree");
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
  TString trkcorrVersion="cv6";
  Corrector3D trkCorr("trkCorrHisAna_djuq","_jtv5_"+trkcorrVersion,"B2");
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorr.Init(0);
  Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_jtv5_"+trkcorrVersion,"B2InConeJ1");
  trkCorrJ1.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ1.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorrJ1.Init();
  Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_jtv5_"+trkcorrVersion,"B2InConeJ2");
  trkCorrJ2.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ2.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorrJ2.Init();

  //=======================================================================================================================
  // centrality reweighting
  //=======================================================================================================================
  TFile *fCent = new TFile("Centrality.root");
  TH1D *hCentralityData = (TH1D*)fCent->Get("h");

  //=======================================================================================================================
  // Output
  //=======================================================================================================================
  TString outName(Form("qm11final/jfh%s_%s_%s_Cent%.0fto%.0f_Aj%.0fto%.0f_Sub%s.root",AnaVersion.Data(),SrcName.Data(),modName.Data(),CentMin,CentMax,AjMin*100,AjMax*100,BkgSubType.Data()));
  if (!doEvtSel) {
    outName=TString("nt_")+inFile0Name;
    outName.ReplaceAll("dj_","");
    outName.ReplaceAll(".root",Form("_offset%d.root",mixOffset));
  }
  cout << "Output: " << outName << endl;
  TFile * outf = new TFile(outName,"RECREATE");

  JetFragAna jana(djTree,SrcName,doMC);
  jana.doEvtSel_ = doEvtSel;
  jana.doEtaCorr_ = doEtaCorr;
  jana.doTrackingEffFakeCorr_ = false;
  jana.doCentralityReweighting_ = doReWeight;
  jana.doJetOnly_ = false;
  jana.cut.NrJEtaMax = 1.6; // 2, 1.6
  jana.cut.AwJEtaMax = 1.6; // 2, 1.6
  jana.cut.CentMin = CentMin;
  jana.cut.CentMax = CentMax;
  jana.cut.SetDJEt(NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,TMath::Pi()*2./3);
  jana.cut.AjMin = AjMin;
  jana.cut.AjMax = AjMax;
  jana.cut.BaseCutType=evtBase;
  jana.cut.DJCutType = DJCutType;
  jana.cut.TrkPtMin = TrkPtMin;
  jana.cut.ConeSize = 0.8;
  if (inFile0Name.Contains("_t2")||
      inFile0Name.Contains("data")) {
    jana.doTrackingEffFakeCorr_ = true;
  }
  jana.cut.BkgSubType = BkgSubType;
  jana.cut.SetCut();
  jana.cut.DJAnd(TCut(Form("Aj>=%.2f&&Aj<%.2f",AjMin,AjMax)));
  jana.cut.Print(1);
  jana.jetaCorr_ = jetaCorr;
  jana.hCentralityData_ = hCentralityData;  // centrality distribution from data

  jana.mixOffset_ = mixOffset;

  jana.trkCorr_ = &trkCorr;
  jana.vtrkCorr_[0] = &trkCorrJ1;
  jana.vtrkCorr_[1] = &trkCorrJ2;

  // ====================================================================
  // Independent Jet Collection
  // ====================================================================
  //jana.SetJetTree(0,inFile0Name,"djTree");
  jana.SetJetTree(0,inFile0Name,"tjf");
  jana.SetJetTree(1,"dj_data50v2_djcalo_100_50_c0to30.root","djTree"); //for mixed sample
  jana.jetTreeMode_=0; // 0 for final analysis, 1 for inputsample_trk+data50_jet mix

  // Some Print out
  cout << "======================================================" << endl;
  cout << "Analysis Setting Summary: " << endl;
  cout << "======================================================" << endl;
  cout << "  doEvtSel: " << jana.doEvtSel_ << " doCentReWt: " << jana.doCentralityReweighting_
    << " doEtaCorr: " << jana.doEtaCorr_ << " doTrkEffCorr: " << jana.doTrackingEffFakeCorr_
    << " anaGenpType: " << jana.anaGenpType_ << endl;
  cout << "  mixOffset: " << jana.mixOffset_ << endl;

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(jana.cut.FinDJCut()) << endl;

  // do ana
  jana.Loop();
  outf->Write();
}
