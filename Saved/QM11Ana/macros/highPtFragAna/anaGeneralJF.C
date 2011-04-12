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
  Int_t particleType=2, // 0 for genp, 2 for trk
  Int_t ptHatMin=110
  )

{
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");
  TString version("v3");

  // Inputs/Output
  //TString fdataname(Form("matttrees/Corrected_JetTrackPFCandTuple_HIJet35U_PFTowers_hiGoodTracks_v2.root"));
  //TString fdataname(Form("trees/tr_hydjuq%d_jtv2_2_50k.root",ptHatMin));
  TString fdataname(Form("trees/merged_JetAnalysisTTrees_hiGoodTracks_condor_v2.root"));
  //TString tag(Form("trana%s_hydjuq%d_mc_akpu3pf_t%d_50k",version.Data(),ptHatMin,particleType));
  //TString algo = "akPu3PFJetAnalyzer";
  Int_t doJEC = 1;
  TString tag(Form("trana%s_datamattv2_akpu3pf",version.Data()));
  //TString algo = "inclusiveJetAnalyzer";
  //Int_t doJEC = 0;
  //TString tag(Form("trana%s_data_icpu5",version.Data()));
  //TString tag(Form("trana%s_hydjuq%d_icpu5_t%d",version.Data(),ptHatMin,particleType));

  cout << fdataname << " output: " << tag << endl;
  cout << "Input: " << fdataname << endl;
  cout << " Jet: " << algo << endl;
  cout << "Output: " << tag << endl;

  // Load Trees
  TChain * tevt = new TChain(algo+"/t");
  //TChain * tevt = new TChain("t");
  tevt->Add(fdataname);

  TChain * tjet = new TChain(algo+"/t");
  //TChain * tjet = new TChain("t");
  tjet->Add(fdataname);

  TChain * tp=0;
  if (particleType==2) {
    tp = new TChain("trkAnalyzer/trackTree");
    //tp = new TChain("t");
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
  //GeneralJetFragAna jfana("j4");
  GeneralJetFragAna jfana("");
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.doJEC_ = doJEC;
  jfana.anajec_ = &anajec;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=90;
  jfana.pptMin_=1;
  jfana.Init(particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}

void runAnaGeneralJF()
{
  anaGeneralJF(2,80);
  anaGeneralJF(0,80);
  anaGeneralJF(2,110);
  anaGeneralJF(0,110);
  //anaGeneralJF(2,170);
  //anaGeneralJF(0,170);
}
