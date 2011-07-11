#include "TROOT.h"
#include "TFile.h"
#include "GeneralJetFragAna.C"
//////////////////////////////////////////////////////////////
//
// Note
// - Only when running on akpu3pf, akpu4pf do we need jec
//
//////////////////////////////////////////////////////////////

void anaGeneralJF_data(
    Int_t jetType=2, // 1 for genjet, 2 for recjet
    Int_t particleType=2 // 0 for genp, 2 for trk
    )

{
  Bool_t doMC = false;
  cout << "start " << endl;
  gSystem->Load("libMathCore");
  gSystem->Load("libPhysics");
  TString version("v12");

  Int_t treeFormat=-1;
  TString fdataname,tag,algo;
  TChain * tevt = 0, * tjet = 0, * tp=0;

  // Inputs/Output
  // data
  treeFormat = 1; // 0 for jra, 1 for pfana
  algo = "j1"; // j1: icpu5calo, j4: akpu3pf
  // === HI ===
  //TChain * chain = new TChain("PFJetAnalyzer/t","");
  //Bool_t useTrkQual=true;
  TChain * chain = new TChain("t","");
  Bool_t useTrkQual=false;
  chain->Add("/net/hisrv0001/home/mnguyen/scratch/InclusiveJetAnalyzer/310X/HIData_Jet35U/hiGoodTightTracks/set1/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass.root");
  chain->Add("/net/hisrv0001/home/mnguyen/scratch/InclusiveJetAnalyzer/310X/HIData_Jet35U/hiGoodTightTracks/set2/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass.root");
  chain->Add("/net/hisrv0001/home/mnguyen/scratch/InclusiveJetAnalyzer/310X/HIData_Jet35U/hiGoodTightTracks/set3/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass.root");
  chain->Add("/net/hisrv0001/home/mnguyen/scratch/InclusiveJetAnalyzer/310X/HIData_Jet35U/hiGoodTightTracks/set4/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass.root");
  //tag = Form("trana%s_dataj35pfhgtv1repass2_%s_j%dt%d_et90",version.Data(),algo.Data(),jetType,particleType);
  tag = Form("trana%s_dataj35pfhgtv1repass2_%s_j%dt%d_et90",version.Data(),algo.Data(),jetType,particleType);
  //chain->Add("~mnguyen/scratch/InclusiveJetAnalyzer/310X/HIData_Jet50U/hiGoodTightTracks/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass.root");
  //tag = Form("trana%s_dataj50corepfhgtv1repass2_%s_j%dt%d_et90",version.Data(),algo.Data(),jetType,particleType);

  cout << chain->GetFile()->GetName() << endl;
  tevt = chain;
  tjet = chain;
  tp = chain;

  cout << fdataname << " output: " << tag << endl;
  cout << "Input: " << tjet->GetName() << endl;
  cout << " Jet: " << algo << endl;
  cout << "Output: " << tag << endl;

  // output
  TFile * outf = new TFile(Form("ntout/%s.root",tag.Data()),"RECREATE");

  // ana
  GeneralJetFragAna jfana(algo);
  jfana.evtTree_ = tevt;
  jfana.jetTree_ = tjet;
  jfana.doMC_ = doMC;
  jfana.useTrkQual_ = useTrkQual;
  jfana.pTree_ = tp;
  jfana.leadJetPtMin_=90;
  jfana.jetEtaMax_ = 2;
  jfana.pptMin_=-1;
  jfana.treeFormat_=treeFormat; // 0 for jra, 1 for pftree
  jfana.Init(jetType,particleType);
  jfana.Loop();

  // All done
  outf->Write();
  outf->Close();
}
