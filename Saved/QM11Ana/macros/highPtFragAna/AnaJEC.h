#ifndef AnaJEC_h
#define AnaJEC_h
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "TROOT.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h" 
using namespace std;

class AnaJEC
{
  public:
    string L2Name;
    string L3Name;
    JetCorrectorParameters *parHI310x_l2;
    JetCorrectorParameters *parHI310x_l3;
    vector<JetCorrectorParameters> vpar_HI310x;
    FactorizedJetCorrector *_JEC_HI310X;

    string L2NameRef;
    string L3NameRef;
    JetCorrectorParameters *parHI310x_l2Ref;
    JetCorrectorParameters *parHI310x_l3Ref;
    vector<JetCorrectorParameters> vpar_HI310xRef;
    FactorizedJetCorrector *_JEC_HI310XRef;

    AnaJEC(TString algo="AK3PF");
    ~AnaJEC();
    void Init();
    Float_t GetJEC(Float_t pt, Float_t eta);
};

AnaJEC::AnaJEC(TString algo) :
  L2Name(Form("data/HI_PFTowers_hiSelFix_L2Relative_%s.txt",algo.Data())),
  L3Name(Form("data/HI_PFTowers_hiSelFix_L3Absolute_%s.txt",algo.Data())),
  L2NameRef("data/HI_PFTowers_hiSelFix_L2Relative_AK5PF.txt"),
  L3NameRef("data/HI_PFTowers_hiSelFix_L3Absolute_AK5PF.txt")
{
}

AnaJEC::~AnaJEC()
{
  delete parHI310x_l2;
  delete parHI310x_l3;
  delete _JEC_HI310X;
  delete parHI310x_l2Ref;
  delete parHI310x_l3Ref;
  delete _JEC_HI310XRef;
}

void AnaJEC::Init()
{
  cout<<L3Name<<endl;
  parHI310x_l2 = new JetCorrectorParameters(L2Name.c_str());
  parHI310x_l3 = new JetCorrectorParameters(L3Name.c_str());
  vpar_HI310x.push_back(*parHI310x_l2);
  vpar_HI310x.push_back(*parHI310x_l3);
  _JEC_HI310X = new FactorizedJetCorrector(vpar_HI310x);

  parHI310x_l2Ref = new JetCorrectorParameters(L2NameRef.c_str());
  parHI310x_l3Ref = new JetCorrectorParameters(L3NameRef.c_str());
  vpar_HI310xRef.push_back(*parHI310x_l2Ref);
  vpar_HI310xRef.push_back(*parHI310x_l3Ref);
  _JEC_HI310XRef = new FactorizedJetCorrector(vpar_HI310xRef);
}

Float_t AnaJEC::GetJEC(Float_t pt, Float_t eta)
{
  _JEC_HI310X->setJetPt(pt);
  _JEC_HI310X->setJetEta(eta);
  Float_t corr = _JEC_HI310X->getCorrection(); 

  _JEC_HI310XRef->setJetPt(pt);
  _JEC_HI310XRef->setJetEta(eta);
  Float_t corrRef = _JEC_HI310XRef->getCorrection(); 
  return corr/corrRef;
}
#endif //AnaJEC_h
