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

  // Inputs
  TString fdataname(Form("trees/tr_hydjuq%d_jtv2_2.root",ptHatMin));
  cout << fdataname << endl;
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

  // ana
  TFile * outf = new TFile(Form("trana_hydjuq%d_mc_akpu3pf_t%d.root",ptHatMin,particleType),"RECREATE");
  GeneralJetFragAna jfana("");
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.doJEC_ = 1;
  jfana.anajec_ = &anajec;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=100;
  jfana.pptMin_=2;
  jfana.Init(particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}
