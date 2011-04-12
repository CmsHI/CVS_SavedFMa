#ifndef GeneralJetFragAna_h
#define GeneralJetFragAna_h
#include <iostream>
#include "TSystem.h"
#include "TChain.h"
#include "TString.h"
#include "Math/Vector3D.h"
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiM4D.h"
#include "AnaJEC.h"
using namespace ROOT::Math;
const Int_t MAXNJETS = 2000;
const Int_t MAXNP = 100000;

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMLorentzVectorD;

// =====================================================
// Input Structs
// =====================================================
struct AnaEvt
{
  Int_t run, evt, lumi, cbin;
  Float_t cent,vz;
  void LoadBranches(TChain * t) {
    t->SetBranchAddress("run",&(this->run));
    t->SetBranchAddress("evt",&(this->evt));
    t->SetBranchAddress("lumi",&(this->lumi));
    t->SetBranchAddress("bin",&(this->cbin));
    t->SetBranchAddress("vz",&(this->vz));
  }
};

struct AnaJets
{
  Int_t njets;
  Float_t jtpt[MAXNJETS];
  Float_t rawpt[MAXNJETS];
  Float_t jteta[MAXNJETS];
  Float_t jtphi[MAXNJETS];
  void LoadBranches(TChain * t,TString name) {
    /*
    t->SetBranchAddress("n"+name,&(this->njets));
    t->SetBranchAddress("corrpt_"+name,this->jtpt);
    t->SetBranchAddress("jteta_"+name,this->jteta);
    t->SetBranchAddress("jtphi_"+name,this->jtphi);
    */
    t->SetBranchAddress("nref",&(this->njets));
    t->SetBranchAddress("jtpt",this->jtpt);
    t->SetBranchAddress("rawpt",this->rawpt);
    t->SetBranchAddress("jteta",this->jteta);
    t->SetBranchAddress("jtphi",this->jtphi);
  }
};

struct AnaParticles
{
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  void LoadBranches(TChain * t, Int_t pType) {
    if (pType==2) {
      // trkAna
      /*
      t->SetBranchAddress("ntrack",&(this->np));
      t->SetBranchAddress("trackpt",this->ppt);
      t->SetBranchAddress("tracketa",this->peta);
      t->SetBranchAddress("trackphi",this->pphi);
       */
      t->SetBranchAddress("nTrk",&(this->np));
      t->SetBranchAddress("trkPt",this->ppt);
      t->SetBranchAddress("trkEta",this->peta);
      t->SetBranchAddress("trkPhi",this->pphi);
    } else if (pType==0) {
      // genpAna
      t->SetBranchAddress("mult",&(this->np));
      t->SetBranchAddress("pt",this->ppt);
      t->SetBranchAddress("eta",this->peta);
      t->SetBranchAddress("phi",this->pphi);
    }
  }
};

// =====================================================
// Output Structs
// =====================================================
struct JetFrag
{
  Int_t cbin;
  Float_t cent;
  Float_t jtpt[2];
  Float_t rawpt[2];
  Float_t jteta[2];
  Float_t jtphi[2];
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  void SetBranches(TTree * t) {
    t->Branch("cbin",&(this->cbin),"cbin/I");
    t->Branch("jtpt",this->jtpt,"jtpt[2]/F");
    t->Branch("rawpt",this->rawpt,"rawpt[2]/F");
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
    TString name_;
    TChain * evtTree_;
    TChain * jetTree_;
    TChain * pTree_;

    Int_t doJEC_;
    AnaJEC * anajec_;

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
    GeneralJetFragAna(TString name);
    void Init(Int_t pType);
    void Loop();
    Int_t GetLeadingJet(AnaJets & jets,std::vector<PtEtaPhiMLorentzVectorD> & jv);
    Int_t GetJet2(AnaJets & jets,std::vector<PtEtaPhiMLorentzVectorD> & jv, Int_t leadJetInd);
};
#endif // GeneralJetFragAna_h
