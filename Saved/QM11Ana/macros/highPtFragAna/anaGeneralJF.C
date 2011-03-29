#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
void anaGeneralJF() 
{
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");

  // Inputs
  TString fdataname("inputs/matt/Corrected_JetTrackPFCandTuple_HIJet50U_PFTowers.root");
  //TString fdataname("inputs/matt/Corrected_JetTrackPFCandTuple_HI_pthat80_PFTowers.root");
  TChain * tevt = new TChain("t");
  tevt->Add(fdataname);

  TChain * tjet = new TChain("t");
  tjet->Add(fdataname);

  TChain * tp = new TChain("t");
  tp->Add(fdataname);

  // ana
  TFile * outf = new TFile("tr_j4f_data.root","RECREATE");
  GeneralJetFragAna jfana("j4");
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=100;
  jfana.pptMin_=2;
  jfana.Init();
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}
