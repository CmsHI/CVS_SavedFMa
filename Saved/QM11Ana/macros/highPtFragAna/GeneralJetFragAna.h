#ifndef GeneralJetFragAna_h
#define GeneralJetFragAna_h
#include <iostream>
#include "TSystem.h"
#include "TChain.h"
#include "TString.h"
#include "Math/Vector3D.h"
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiM4D.h"
using namespace ROOT::Math;
const Int_t MAXNJETS = 2000;
const Int_t MAXNP = 100000;

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMLorentzVectorD;

// =====================================================
// Input Structs
// =====================================================
struct AnaEvt
{
  Int_t run, evt, lumi, bin;
  Float_t cent,vz;
  void LoadBranches(TChain * t) {
    t->SetBranchAddress("run",&(this->run));
    t->SetBranchAddress("evt",&(this->evt));
    t->SetBranchAddress("lumi",&(this->lumi));
    t->SetBranchAddress("bin",&(this->bin));
    t->SetBranchAddress("vz",&(this->vz));
  }
};

struct AnaJets
{
  Int_t njets;
  Float_t jtpt[MAXNJETS];
  Float_t jteta[MAXNJETS];
  Float_t jtphi[MAXNJETS];
  void LoadBranches(TChain * t) {
    t->SetBranchAddress("nj1",&(this->njets));
    t->SetBranchAddress("corrpt_j1",this->jtpt);
    t->SetBranchAddress("jteta_j1",this->jteta);
    t->SetBranchAddress("jtphi_j1",this->jtphi);
  }
};

struct AnaParticles
{
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  void LoadBranches(TChain * t) {
    t->SetBranchAddress("ntrack",&(this->np));
    t->SetBranchAddress("trackpt",this->ppt);
    t->SetBranchAddress("tracketa",this->peta);
    t->SetBranchAddress("trackphi",this->pphi);
  }
};

// =====================================================
// Output Structs
// =====================================================
struct JetFrag
{
  Float_t cent;
  Float_t jtpt[2];
  Float_t jteta[2];
  Float_t jtphi[2];
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  void SetBranches(TTree * t) {
    t->Branch("cent",&(this->cent),"cent/F");
    t->Branch("jtpt",this->jtpt,"jtpt[2]/F");
    t->Branch("jteta",this->jteta,"jteta[2]/F");
    t->Branch("jtphi",this->jtphi,"jtphi[2]/F");
    t->Branch("np",&(this->np),"np/I");
    t->Branch("ppt",this->ppt,"ppt[np]/F");
    t->Branch("peta",this->peta,"peta[np]/F");
    t->Branch("pphi",this->pphi,"pphi[np]/F");
  }
  void clear()
  {
    for (Int_t j=0; j<2; ++j) {
      jtpt[j]=-99;
    }
    np=0;
  }
};

// =====================================================
// Class Definition
// =====================================================
class GeneralJetFragAna
{
  public:
    // ana setup
    TChain * evtTree_;
    TChain * jetTree_;
    TChain * pTree_;

    AnaEvt anaEvt_;
    AnaJets anaJets_;
    AnaParticles anaPs_;

    Float_t leadJetPtMin_;
    Float_t pptMin_;

    // ana members
    TTree * jfTree_;
    JetFrag jf_;
    std::vector<PtEtaPhiMLorentzVectorD> anaJetv_;
    std::vector<PtEtaPhiMLorentzVectorD> pv_;

    // methods
    GeneralJetFragAna();
    void Init();
    void Loop();
    Int_t GetLeadingJet(AnaJets & jets,std::vector<PtEtaPhiMLorentzVectorD> & jv);
    Int_t GetJet2(AnaJets & jets,std::vector<PtEtaPhiMLorentzVectorD> & jv, Int_t leadJetInd);
};
#endif // GeneralJetFragAna_h
