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
  void LoadBranches(TChain * t, Int_t treeFormat) {
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
  Float_t jteta[MAXNJETS];
  Float_t jtphi[MAXNJETS];
  Float_t refpt[MAXNJETS];
  Float_t rawpt[MAXNJETS];
  void LoadBranches(TChain * t, Int_t treeFormat, TString name="",Int_t jType=2,Bool_t doMC=false) {
    TString jetPt = "jtpt_";
    if (treeFormat==0) {
      if (jType==2) {
	t->SetBranchAddress("nref",&(this->njets));
	t->SetBranchAddress(jetPt,this->jtpt);
	t->SetBranchAddress("jteta",this->jteta);
	t->SetBranchAddress("jtphi",this->jtphi);
	t->SetBranchAddress("rawpt",this->rawpt);
      } else if (jType==1) {
	jetPt = "genpt";
	t->SetBranchAddress("ngen",&(this->njets));
	t->SetBranchAddress(jetPt,this->jtpt);
	t->SetBranchAddress("geneta",this->jteta);
	t->SetBranchAddress("genphi",this->jtphi);
      }
      if (doMC) {
	t->SetBranchAddress("refpt",this->refpt);
      }
    } else if (treeFormat==1) {
      jetPt = "corrpt_";
      t->SetBranchAddress("n"+name,&(this->njets));
      t->SetBranchAddress(jetPt+name,this->jtpt);
      t->SetBranchAddress("jteta_"+name,this->jteta);
      t->SetBranchAddress("jtphi_"+name,this->jtphi);
      t->SetBranchAddress("rawpt_"+name,this->rawpt);
      if (doMC) {
	t->SetBranchAddress("refpt_"+name,this->refpt);
      }
    }
    cout << "jetpt branch: " << jetPt << endl;
  }
};

struct AnaParticles
{
  Int_t np;
  Int_t ppid[MAXNP];
  Int_t pfid[MAXNP];
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  Int_t trkNHits[MAXNP];
  Int_t trkQual[MAXNP];
  void LoadBranches(TChain * t, Int_t treeFormat, Int_t pType) {
    if (pType==2) { // tracks
      if (treeFormat==0) {
	// trkAna
	t->SetBranchAddress("nTrk",&(this->np));
	t->SetBranchAddress("trkPt",this->ppt);
	t->SetBranchAddress("trkEta",this->peta);
	t->SetBranchAddress("trkPhi",this->pphi);
	t->SetBranchAddress("trkNHit",this->trkNHits);
      } else if (treeFormat==1) {
	// pf ana
	t->SetBranchAddress("ntrack",&(this->np));
	t->SetBranchAddress("trackpt",this->ppt);
	t->SetBranchAddress("tracketa",this->peta);
	t->SetBranchAddress("trackphi",this->pphi);
	t->SetBranchAddress("tracknhits",this->trkNHits);
      }
    } else if (pType==3) { // pf cand
      if (treeFormat==1) {
	t->SetBranchAddress("nPFcand",&(this->np));
	t->SetBranchAddress("candId",this->pfid);
	t->SetBranchAddress("candpt",this->ppt);
	t->SetBranchAddress("candeta",this->peta);
	t->SetBranchAddress("candphi",this->pphi);
      }
    } else if (pType==0) { // genp
      if (treeFormat==0) {
	t->SetBranchAddress("mult",&(this->np));
	t->SetBranchAddress("pt",this->ppt);
	t->SetBranchAddress("eta",this->peta);
	t->SetBranchAddress("phi",this->pphi);
      } else if (treeFormat==1) {
	t->SetBranchAddress("ngenp",&(this->np));
	t->SetBranchAddress("genppdgId",this->ppid);
	t->SetBranchAddress("genppt",this->ppt);
	t->SetBranchAddress("genpeta",this->peta);
	t->SetBranchAddress("genpphi",this->pphi);
      }
    }
  }
};

// =====================================================
// Output Structs
// =====================================================
struct JetFrag
{
  Float_t cent;
  Int_t cbin;
  Float_t vz;
  Float_t jtpt[2];
  Float_t jteta[2];
  Float_t jtphi[2];
  Float_t refpt[2];
  Float_t rawpt[2];
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  Int_t pfid[MAXNP];
  void SetBranches(TTree * t) {
    t->Branch("cent",&(this->cent),"cent/F");
    t->Branch("cbin",&(this->cbin),"cbin/I");
    t->Branch("vz",&(this->vz),"vz/F");
    t->Branch("jtpt",this->jtpt,"jtpt[2]/F");
    t->Branch("jteta",this->jteta,"jteta[2]/F");
    t->Branch("jtphi",this->jtphi,"jtphi[2]/F");
    t->Branch("refpt",this->refpt,"refpt[2]/F");
    t->Branch("rawpt",this->rawpt,"rawpt[2]/F");
    t->Branch("np",&(this->np),"np/I");
    t->Branch("ppt",this->ppt,"ppt[np]/F");
    t->Branch("peta",this->peta,"peta[np]/F");
    t->Branch("pphi",this->pphi,"pphi[np]/F");
    t->Branch("pfid",this->pfid,"pfid[np]/I");
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

    Bool_t doMC_;
    Int_t doJEC_;
    Bool_t doJetOnly_;
    AnaJEC * anajec_;

    AnaEvt anaEvt_;
    AnaJets anaJets_;
    AnaParticles anaPs_;

    Float_t leadJetPtMin_;
    Float_t pptMin_;
    Int_t treeFormat_;
    Int_t jetType_;
    Int_t pType_;

    // ana members
    TTree * jfTree_;
    JetFrag jf_;
    std::vector<PtEtaPhiMLorentzVectorD> anaJetv_;
    std::vector<PtEtaPhiMLorentzVectorD> pv_;

    // methods
    GeneralJetFragAna(TString name);
    void Init(Int_t jType, Int_t pType);
    void Loop();
    Int_t GetLeadingJet(AnaJets & jets,std::vector<PtEtaPhiMLorentzVectorD> & jv);
    Int_t GetJet2(AnaJets & jets,std::vector<PtEtaPhiMLorentzVectorD> & jv, Int_t leadJetInd);
};
#endif // GeneralJetFragAna_h
