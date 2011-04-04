#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
void anaGeneralJF(
  Int_t particleType=2 // 0 for genp, 2 for trk
  )

{
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");

  // Inputs
  //TString fdataname("inputs/matt/Corrected_JetTrackPFCandTuple_HIJet50U_PFTowers.root");
  //TString fdataname("inputs/matt/Corrected_JetTrackPFCandTuple_HI_pthat80_PFTowers.root");
  //TString fdataname("trees/tr_hydjuq110_jtv2_2_50k.root");
  TString fdataname("trees/tr_hydjuq110_jtv2_2.root");
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

  // ana
  TFile * outf = new TFile(Form("trana_hydjuq110_mc_akpu3pf_t%d.root",particleType),"RECREATE");
  GeneralJetFragAna jfana("");
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=100;
  jfana.pptMin_=2;
  jfana.Init(particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}
