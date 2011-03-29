#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
void anaGeneralJF() 
{
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");

  // Inputs
  TChain * tevt = new TChain("t");
  tevt->Add("inputs/matt/Corrected_JetTrackPFCandTuple_HIJet50U_PFTowers.root");

  TChain * tjet = new TChain("t");
  tjet->Add("inputs/matt/Corrected_JetTrackPFCandTuple_HIJet50U_PFTowers.root");

  TChain * tp = new TChain("t");
  tp->Add("inputs/matt/Corrected_JetTrackPFCandTuple_HIJet50U_PFTowers.root");

  // ana
  TFile * outf = new TFile("tr_jf.root","RECREATE");
  GeneralJetFragAna jfana;
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=100;
  jfana.pptMin_=2;
  jfana.Init();
  jfana.Loop();

  // All done
  outf->Write();
}
