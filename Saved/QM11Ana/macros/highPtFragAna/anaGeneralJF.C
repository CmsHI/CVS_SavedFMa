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
  TString version("v1");

  // Inputs/Output
  TString fdataname(Form("trees/tr_hydjuq%d_jtv2_2.root",ptHatMin));
  //TString tag(Form("trana%s_hydjuq%d_mc_akpu3pf_t%d.root",version.Data(),ptHatMin,particleType));
  /*
  TString fdataname(Form("trees/merged_JetAnalysisTTrees_hiGoodTracks_condor_v2.root"));
  */
  //TString algo = "akPu3PFJetAnalyzer";
  //Int_t doJEC = 1;
  //TString tag(Form("trana%s_data_akpu3pf",version.Data()));
  TString algo = "inclusiveJetAnalyzer";
  Int_t doJEC = 0;
  //TString tag(Form("trana%s_data_icpu5",version.Data()));
  TString tag(Form("trana%s_hydjuq%d_mc_icpu5_t%d.root",version.Data(),ptHatMin,particleType));

  cout << fdataname << " output: " << tag << endl;
  cout << "Input: " << fdataname << endl;
  cout << " Jet: " << algo << endl;
  cout << "Output: " << tag << endl;

  // Load Trees
  TChain * tevt = new TChain(algo+"/t");
  tevt->Add(fdataname);

  TChain * tjet = new TChain(algo+"/t");
  tjet->Add(fdataname);

  TChain * tp=0;
  if (particleType==2) {
    tp = new TChain("trkAnalyzer/trackTree");
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
