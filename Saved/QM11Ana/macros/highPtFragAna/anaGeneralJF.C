#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
void anaGeneralJF(
  Int_t ptHatMin=110,
  Int_t particleType=2 // 0 for genp, 2 for trk
  )

{
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");

  // Inputs/Output
  /*
  TString fdataname(Form("trees/tr_hydjuq%d_jtv2_2.root",ptHatMin));
  TString tag(Form("trana_hydjuq%d_mc_akpu3pf_t%d.root",ptHatMin,particleType));
  cout << fdataname << " output: " << tag << endl;
  */
  TString fdataname(Form("trees/merged_JetAnalysisTTrees_hiGoodTracks_condor_v2.root"));
  TString tag(Form("trana_datav0_akpu3pf"));
  cout << "Input: " << fdataname << endl;
  cout << "Output: " << tag << endl;

  // Load Trees
  TChain * tevt = new TChain("akPu3PFJetAnalyzer/t");
  tevt->Add(fdataname);

  TChain * tjet = new TChain("inclusiveJetAnalyzer/t");
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
  TFile * outf = new TFile(tag+".root","RECREATE");

  // ana
  GeneralJetFragAna jfana("");
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.doJEC_ = 1;
  jfana.anajec_ = &anajec;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=100;
  jfana.pptMin_=1.5;
  jfana.Init(particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}
