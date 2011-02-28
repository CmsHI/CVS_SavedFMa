#include <iostream>
#include <map>
#include <TCanvas.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TROOT.h"
#include "TChain.h"
#include "aliases_dijet.C"
#include "JetFragAna.C"

using namespace std;

void anaJetFrag(
    TString inFile0Name = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_djcalo_120_50.root",
    Bool_t doEvtSel = false,
    Bool_t doEtaCorr = true,
    Bool_t doReWeight = false,
    TString BkgSubType = "None", // EtaRefl, PhiRot, None
    int doMC=0,
    TString SrcName = "HCPR_J50U",
    TString AnaVersion = "testtree",
    TString modName = "djcalo",
    Double_t CentMin = 0,
    Double_t CentMax = 30,
    Double_t AjMin = 0,
    Double_t AjMax = .11,
    Double_t NrJEtMin = 120,
    Double_t NrJEtMax = 500,
    Double_t AwJEtMin = 50,
    Double_t AwJEtMax = 500,
    Double_t TrkPtMin = 3, // 0.5 for first paper
    TString evtBase="S1",
    TString DJCutType = "Ana", // Ana
    Int_t mixOffset=0)
{
  // Load Class
  gROOT->ProcessLine(".L JetFragAna.C+");
  Int_t treeType=10; // 0 djana, 10 filtered djana

  //TH1::SetDefaultSumw2();
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain *djTree=0;
  if (treeType==0) djTree = new TChain(modName+"/djTree","dijet Tree");
  else if (treeType==10) djTree = new TChain("djTree","dijet Tree");
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
  TString trkCorrModule("hitrkEffAnalyzer");
  vector<TFile*> fileTrackingCorr;
  //fileTrackingCorr.push_back(new TFile("trkhist_feb032011_v2_hydjetBass_dijet30.root")); //f0
  //fileTrackingCorr.push_back(new TFile("trkhist_feb032011_v2_hydjetBass_dijet50.root")); //f1
  //fileTrackingCorr.push_back(new TFile("trkhist_feb032011_v2_hydjetBass_dijet80.root")); //f2
  //fileTrackingCorr.push_back(new TFile("trkhist_feb032011_v2_hydjetBass_dijet110.root")); //f3
  //fileTrackingCorr.push_back(new TFile("trkhist_feb032011_v2_hydjetBass_dijet170.root")); //f4
  fileTrackingCorr.push_back(new TFile("djtrkhist_hydjetBassv2_djuq30.root")); //f0
  fileTrackingCorr.push_back(new TFile("djtrkhist_hydjetBassv2_djuq50.root")); //f1
  fileTrackingCorr.push_back(new TFile("djtrkhist_hydjetBassv2_djuq80.root")); //f2
  fileTrackingCorr.push_back(new TFile("djtrkhist_hydjetBassv2_djuq110.root")); //f3
  fileTrackingCorr.push_back(new TFile("djtrkhist_hydjetBassv2_djuq170.root")); //f4
  vector<TString> cbins;
  cbins.push_back("0to1");
  cbins.push_back("2to3");
  cbins.push_back("4to11");
  cbins.push_back("12to19");
  cbins.push_back("20to39");
  Int_t nfile=fileTrackingCorr.size(), ncbin=cbins.size();
  vector<vector<TH3F * > > hTrackingEffCorr(nfile,vector<TH3F*>(ncbin));
  vector<vector<TH3F * > > hTrackingFakCorr(nfile,vector<TH3F*>(ncbin));
  vector<vector<TH3F * > > hTrackingMulCorr(nfile,vector<TH3F*>(ncbin));
  vector<vector<TH3F * > > hTrackingSecCorr(nfile,vector<TH3F*>(ncbin));
  for (int i=0;i<fileTrackingCorr.size();++i) {
    for (int j=0;j<cbins.size();++j)
    {
      //cout << fileTrackingCorr[i]->GetName() << " " << trkCorrModule+"/heff3D_cbin"+cbins[j] << endl;
      TH3F * hNum =(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/heff3D_cbin"+cbins[j]);
      TH3F * hDen =(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hsim3D_cbin"+cbins[j]);
      hTrackingEffCorr[i][j]=(TH3F*)hNum->Clone(Form("hTrkEff_f%d_c%d",i,j));
      hTrackingEffCorr[i][j]->Divide(hDen);

      hNum=(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hfak3D_cbin"+cbins[j]);
      hDen=(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hrec3D_cbin"+cbins[j]);
      hTrackingFakCorr[i][j]=(TH3F*)hNum->Clone(Form("hTrkFak_f%d_c%d",i,j));
      hTrackingFakCorr[i][j]->Divide(hDen);

      hNum=(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hmul3D_cbin"+cbins[j]);
      hDen=(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hsim3D_cbin"+cbins[j]);
      hTrackingMulCorr[i][j]=(TH3F*)hNum->Clone(Form("hTrkMul_f%d_c%d",i,j));
      hTrackingMulCorr[i][j]->Divide(hDen);

      hNum=(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hsec3D_cbin"+cbins[j]);
      hDen=(TH3F*)fileTrackingCorr[i]->Get(trkCorrModule+"/hrec3D_cbin"+cbins[j]);
      hTrackingSecCorr[i][j]=(TH3F*)hNum->Clone(Form("hTrkSec_f%d_c%d",i,j));
      hTrackingSecCorr[i][j]->Divide(hDen);
    }
  }

  //=======================================================================================================================
  // centrality reweighting
  //=======================================================================================================================
  TFile *fCent = new TFile("Centrality.root");
  TH1D *hCentralityData = (TH1D*)fCent->Get("h");

  //=======================================================================================================================
  // Output
  //=======================================================================================================================
  TString outName(Form("draw/jfh%s_%s_%s_Cent%.0fto%.0f_Aj%.0fto%.0f_Sub%s.root",AnaVersion.Data(),SrcName.Data(),modName.Data(),CentMin,CentMax,AjMin*100,AjMax*100,BkgSubType.Data()));
  if (!doEvtSel) {
    outName=TString("nt_")+inFile0Name;
    outName.ReplaceAll("dj_","");
    outName.ReplaceAll(".root",Form("_offset%d.root",mixOffset));
    if (inFile0Name.Contains("/")) {
      outName=inFile0Name;
      outName.ReplaceAll(".root","_nt.root");
    }
  }
  cout << "Output: " << outName << endl;
  TFile * outf = new TFile(outName,"RECREATE");

  JetFragAna jana(djTree,SrcName,doMC);
  jana.doEvtSel_ = doEvtSel;
  jana.doEtaCorr_ = doEtaCorr;
  jana.doTrackingEffFakeCorr_ = true;
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
  if (modName=="djcalo_genp"||modName=="djgen"
      || inFile0Name.Contains("djcalo_genp")||inFile0Name.Contains("djgen")) {
    jana.anaGenpType_=1;
    jana.doTrackingEffFakeCorr_ = false;
  }
  if (SrcName.Contains("Sig")) jana.anaGenpType_=10;
  if (SrcName.Contains("PFPhoton")) {
    jana.doTrackingEffFakeCorr_ = false;
  }
  jana.cut.BkgSubType = BkgSubType;
  jana.cut.SetCut();
  jana.cut.DJAnd(TCut(Form("Aj>=%.2f&&Aj<%.2f",AjMin,AjMax)));
  jana.cut.Print(1);
  jana.jetaCorr_ = jetaCorr;
  jana.hCentralityData_ = hCentralityData;  // centrality distribution from data

  jana.mixOffset_ = mixOffset;

  // ====================================================================
  // Independent Jet Collection
  // ====================================================================
  if (treeType==0) jana.SetJetTree(0,inFile0Name,modName+"/djTree");
  else if (treeType==10) jana.SetJetTree(0,inFile0Name,"djTree");
  jana.SetJetTree(1,"dj_data50v2_djcalo_100_50_c0to30.root","djTree");
  jana.jetTreeMode_=0; // 0 for final analysis, 1 for inputsample_trk+data50_jet mix

  // Some Print out
  cout << "======================================================" << endl;
  cout << "Analysis Setting Summary: " << endl;
  cout << "======================================================" << endl;
  cout << "  doEvtSel: " << jana.doEvtSel_ << " doCentReWt: " << jana.doCentralityReweighting_
    << " doEtaCorr: " << jana.doEtaCorr_ << " doTrkEffCorr: " << jana.doTrackingEffFakeCorr_
    << " anaGenpType: " << jana.anaGenpType_ << endl;
  cout << "  mixOffset: " << jana.mixOffset_ << endl;

  jana.fileTrackingCorr_=fileTrackingCorr;
  jana.trackingCentBin_=cbins;
  jana.trackingEffCorr_ = hTrackingEffCorr;
  jana.trackingFakCorr_ = hTrackingFakCorr;
  jana.trackingMulCorr_ = hTrackingMulCorr;
  jana.trackingSecCorr_ = hTrackingSecCorr;
  jana.trackingEtaBin_ = (TH1D*)hTrackingEffCorr[3][0]->Project3D("x");
  jana.trackingPtBin_ = (TH1D*)hTrackingEffCorr[3][0]->Project3D("y");
  jana.trackingJEtBin_ = (TH1D*)hTrackingEffCorr[3][0]->Project3D("z");
  for (int i=0;i<fileTrackingCorr.size();++i) {
    for (int j=0;j<cbins.size();++j)
    {
      hTrackingEffCorr[i][j]->Write();
      hTrackingFakCorr[i][j]->Write();
      hTrackingMulCorr[i][j]->Write();
      hTrackingSecCorr[i][j]->Write();
    }
  }

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << jana.cut.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(jana.cut.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(jana.cut.FinDJCut()) << endl;

  // do ana
  jana.Loop();
  outf->Write();
}
