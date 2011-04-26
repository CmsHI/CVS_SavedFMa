#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
//////////////////////////////////////////////////////////////
//
// Note
// - Only when running on akpu3pf, akpu4pf do we need jec
//
//////////////////////////////////////////////////////////////

void anaGeneralJF(
  Bool_t doMC = false,
  Int_t jetType=2, // 1 for genjet, 2 for recjet
  Int_t particleType=2, // 0 for genp, 2 for trk
  Int_t ptHatMin=80
  )

{
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");
  TString version("v7");

  Int_t treeFormat=-1,doJEC=-1;
  TString fdataname,tag,algo;

  // Inputs/Output
  if (doMC) {
    // mc
    treeFormat = 0; // 0 for jra, 1 for pfana
    doJEC = 1;
    if (jetType<2) doJEC=0;
    algo = "akPu3PFJetAnalyzer";
    //algo = "j4";
    //algo = "inclusiveJetAnalyzer";
    //fdataname = Form("trees/tr_hydjuq%d_jtv2_2_50k.root",ptHatMin);
    //tag = Form("trana%s_hydjuq%d_mc_akpu3pf_t%d_50k",version.Data(),ptHatMin,jetType,particleType);
    fdataname = Form("~frankma/scratch01/ana/HydjetBass_DJUQ%d_GSR_393_300k_JetTrkv6/tr*.root",ptHatMin);
    tag = Form("trana%s_hydjuq%dv6_akpu3pf_j%dt%d",version.Data(),ptHatMin,jetType,particleType);
    //fdataname = Form("~frankma/scratch01/ana/HydjetBass_DJUQ%d_GSR_393_300k_JetTrkv5GenPSig/tr*.root",ptHatMin);
    //tag = Form("trana%s_hydjuq%dv5_akpu3pf_j%dt%d",version.Data(),ptHatMin,jetType,particleType);
    //fdataname = Form("/net/hidsk0001/d00/scratch/mnguyen/InclusiveJetAnalyzer/310X/Pyquen_UnquenchedDiJet_Pt%d_GEN-SIM-RECO_393_setX/HICorrJetTuples_PFTowers_hiGoodMergedTracks_seedGoodTracks/merged_JetAnalysisTTrees_hiGoodMergedTracks_seedGoodTracks_v1.root",ptHatMin);
    //tag = Form("trana%s_hydjuq%dmattpfgmv1_akpu3pf_t%d",version.Data(),ptHatMin,jetType,particleType);
  }
  else {
    // data
    treeFormat = 0; // 0 for jra, 1 for pfana
    doJEC = 0;
    //algo = "j4";
    algo = "akPu3PFJetAnalyzer";
    //fdataname = Form("trees/merged_JetAnalysisTTrees_hiGoodTracks_condor_v2.root");
    //tag = Form("trana%s_datamattv2_akpu3pf_hiGood",version.Data());
    //fdataname = Form("matttrees/Corrected_JetTrackPFCandTuple_HIJet35U_PFTowers_hiGoodTracks_v2.root");
    fdataname = "/net/hisrv0001/home/mnguyen/scratch/InclusiveJetAnalyzer/310X/MinBiasHfOrBSC-PromptReco-Runs_151077-151211_PAT-v2/merged_inclusiveJetAnalyzer_v1.root";
    tag = Form("trana%s_datamattmbv2_akpu3pf_hiGood",version.Data());
  }

  cout << fdataname << " output: " << tag << endl;
  cout << "Input: " << fdataname << endl;
  cout << " Jet: " << algo << endl;
  cout << "Output: " << tag << endl;

  // Load Trees
  TChain * tevt = 0;
  if (treeFormat==0) tevt = new TChain(algo+"/t");
  else if (treeFormat==1) tevt = new TChain("PFJetAnalyzer/t");
  tevt->Add(fdataname);

  TChain * tjet = 0;
  if (treeFormat==0) tjet = new TChain(algo+"/t");
  else if (treeFormat==1) tjet = new TChain("PFJetAnalyzer/t");
  tjet->Add(fdataname);

  TChain * tp=0;
  if (particleType==2) {
    if (treeFormat==0) tp = new TChain("trkAnalyzer/trackTree");
    else if (treeFormat==1) tp = new TChain("PFJetAnalyzer/t");
  } else if (particleType==0) {
    tp = new TChain("genpAnalyzer/hi");
  }
  tp->Add(fdataname);

  // JEC
  AnaJEC anajec;
  anajec.Init();

  // output
  TFile * outf = new TFile(Form("ntout/%s.root",tag.Data()),"RECREATE");

  // ana
  GeneralJetFragAna jfana(algo);
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.doMC_ = doMC;
  jfana.doJEC_ = doJEC;
  //jfana.doJetOnly_ = true;
  jfana.anajec_ = &anajec;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=90;
  jfana.pptMin_=2;
  jfana.treeFormat_=treeFormat; // 0 for jra, 1 for pftree
  jfana.Init(jetType,particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}

void runData()
{
  anaGeneralJF();
}

void runMcRecJet()
{
  anaGeneralJF(true,2,2,80);
  anaGeneralJF(true,2,0,80);
}

void runMcGenJet()
{
  anaGeneralJF(true,1,2,80);
  anaGeneralJF(true,1,0,80);
}
