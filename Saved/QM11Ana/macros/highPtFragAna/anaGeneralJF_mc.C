#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
//////////////////////////////////////////////////////////////
//
// Note
// - Only when running on akpu3pf, akpu4pf do we need jec
//
//////////////////////////////////////////////////////////////

void anaGeneralJF_mc(
    Int_t jetType=2, // 1 for genjet, 2 for recjet
    Int_t particleType=2 // 0 for genp, 2 for trk
    )

{
  Bool_t doMC = true;
  cout << "start " << endl;
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");
  TString version("v12");

  Int_t treeFormat=-1,doJEC=-1;
  TString fdataname,tag,algo;

  TChain * tevt = 0, * tjet = 0, * tp=0;

  // Inputs/Output
  // data
  treeFormat = 1; // 0 for jra, 1 for pfana
  doJEC = 0;
  algo = "j4";
  // === HI ===
  TChain * chain = new TChain("PFJetAnalyzer/t","");
  chain->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80*-v1.root");
  tag = Form("trana%s_hydjuq80pfhgtv1_%s_j%dt%d_pt4",version.Data(),algo.Data(),jetType,particleType);

  cout << chain->GetFile()->GetName() << endl;
  tevt = chain;
  tjet = chain;
  tp = chain;

  cout << fdataname << " output: " << tag << endl;
  cout << "Input: " << fdataname << endl;
  cout << " Jet: " << algo << endl;
  cout << "Output: " << tag << endl;

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
  jfana.leadJetPtMin_=-1;
  jfana.pptMin_=4;
  jfana.treeFormat_=treeFormat; // 0 for jra, 1 for pftree
  jfana.Init(jetType,particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}
