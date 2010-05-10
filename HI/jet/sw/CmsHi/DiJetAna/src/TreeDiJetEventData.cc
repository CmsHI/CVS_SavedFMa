#include "CmsHi/DiJetAna/interface/TreeDiJetEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

// Defaults
void TreeDiJetEventData::SetDefaults()
{
  // Class Defaults
}
// constructors
TreeDiJetEventData::TreeDiJetEventData()
  //
  // We will call the default constructor of the event variables   
  //
{
  SetDefaults();
}
TreeDiJetEventData::TreeDiJetEventData(TTree * tree)
  //
  // We will call the default constructor of the event variables   
  //
{
  SetDefaults();
  tree_ = tree;
}

// ---------------- Helpers ------------------
void TreeDiJetEventData::CalcDJVars(Bool_t isMC, std::vector<math::PtEtaPhiMLorentzVector> anajets,
    std::vector<math::PtEtaPhiMLorentzVector> refjets)
{
  // near/away info     
  nljet_	     = anajets[0].pt();
  nljeta_	     = anajets[0].eta();
  nljphi_	     = anajets[0].phi();
  aljet_	     = anajets[1].pt();
  aljeta_	     = anajets[1].eta();
  aljphi_	     = anajets[1].phi();

  // dijet info
  Float_t jdphi     = TMath::Abs(reco::deltaPhi(anajets[0].phi(),anajets[1].phi()));
  jdphi_	     = jdphi;
  djmass_	     = (anajets[0]+anajets[1]).M();

  // Done if data
  if (!isMC) return;

  // refjets
  if (refjets.size()>0) {
    // near/away info     
    nlrjet_            = refjets[0].pt();
    nlrjeta_           = refjets[0].eta();
    nlrjphi_           = refjets[0].phi();
    alrjet_            = refjets[1].pt();
    alrjeta_           = refjets[1].eta();
    alrjphi_           = refjets[1].phi();

    // dijet info
    Float_t rjdphi    = TMath::Abs(reco::deltaPhi(refjets[0].phi(),refjets[1].phi()));
    rjdphi_            = rjdphi;
    rdjmass_	     = (refjets[0]+refjets[1]).M();
  }
}

void TreeDiJetEventData::CalcTrkVars(Bool_t isMC,
    std::vector<math::PtEtaPhiMLorentzVector> anajets,
    math::PtEtaPhiMLorentzVector anaTrk, Int_t it)
{
  // Basic Kinematic Info
  ppt_[it]		= anaTrk.pt();
  peta_[it]		= anaTrk.eta();
  pphi_[it]		= anaTrk.phi();

  // Relations to jet
  pndphi_[it]		= TMath::Abs(reco::deltaPhi(pphi_[it],anajets[0].phi()));
  pndeta_[it]		= peta_[it] - anajets[0].eta();;
  pndr_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[0].eta(),anajets[0].phi());

  padphi_[it]		= TMath::Abs(reco::deltaPhi(pphi_[it],anajets[1].phi()));
  padeta_[it]		= peta_[it] - anajets[1].eta();
  padr_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[1].phi(),anajets[1].eta());

  //  - background variables-
  pndrbg_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[0].eta(),anajets[0].phi()+TMath::PiOver2());
  padrbg_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[1].eta(),anajets[1].phi()+TMath::PiOver2());

  // jet cone info
  if (ppt_[it]>0.5){
    if (pndr_[it]<0.5 && ppt_[it]<anajets[0].pt()) {
      ++nljCone5NP_;
      nljCone5Et_+=ppt_[it];
    }
    if (padr_[it]<0.5 && ppt_[it]<anajets[1].pt()) {
      ++aljCone5NP_;
      aljCone5Et_+=ppt_[it];
    }
    if (pndrbg_[it]<0.5 && ppt_[it]<anajets[0].pt()) {
      ++nljCone5NPBg_;
      nljCone5EtBg_+=ppt_[it];
    }
    if (padrbg_[it]<0.5 && ppt_[it]<anajets[1].pt()) {
      ++aljCone5NPBg_;
      aljCone5EtBg_+=ppt_[it];
    }
  }

  // fragmentation variables
  // will change to dijet frame soon
  zn_[it]		= ppt_[it]/anajets[0].pt();
  za_[it]		= ppt_[it]/anajets[1].pt();
}

// set brances
void TreeDiJetEventData::SetBranches()
{
  // --event level--
  tree_->Branch("run", &(this->run_), "run/I");
  tree_->Branch("evt", &(this->evt_), "evt/I");
  tree_->Branch("lumi", &(this->lumi_), "lumi/I");
  tree_->Branch("nvtx", &(this->nvtx_), "nvtx/I");
  tree_->Branch("vtxntrks", &(this->vtxntrks_), "vtxntrks/I");
  tree_->Branch("vtxndof", &(this->vtxndof_), "vtxndof/F");
  tree_->Branch("vtxchi2", &(this->vtxchi2_), "vtxchi2/F");
  tree_->Branch("vz", &(this->vz_), "vz/F");
  // -- hi event var's --
  tree_->Branch("b", &(this->b_), "b/F");
  tree_->Branch("npart", &(this->npart_), "npart/I");
  tree_->Branch("ncoll", &(this->ncoll_), "ncoll/I");

  // -- dijet info --
  tree_->Branch("djmass", &(this->djmass_), "djmass/F");
  tree_->Branch("rdjmass", &(this->rdjmass_), "rdjmass/F");
  tree_->Branch("cmeta", &(this->cmeta_), "cmeta/F");
  tree_->Branch("lajmul", &(this->leadAwayMul_), "lajmul/I");
  // --ref jet info--
  tree_->Branch("nlrjid", &(this->nlrjid_), "nlrjid/I");
  tree_->Branch("nlrjstat", &(this->nlrjstat_), "nlrjstat/I");
  tree_->Branch("nlrjet", &(this->nlrjet_), "nlrjet/F");
  tree_->Branch("nlrjetsm", &(this->nlrjetsm_), "nlrjetsm/F");
  tree_->Branch("nlrjeta", &(this->nlrjeta_), "nlrjeta/F");
  tree_->Branch("nlrjphi", &(this->nlrjphi_), "nlrjphi/F");

  tree_->Branch("alrjid", &(this->alrjid_), "alrjid/I");
  tree_->Branch("alrjstat", &(this->alrjstat_), "alrjstat/I");
  tree_->Branch("alrjet", &(this->alrjet_), "alrjet/F");
  tree_->Branch("alrjetsm", &(this->alrjetsm_), "alrjetsm/F");
  tree_->Branch("alrjeta", &(this->alrjeta_), "alrjeta/F");
  tree_->Branch("alrjphi", &(this->alrjphi_), "alrjphi/F");

  tree_->Branch("rjdphi", &(this->rjdphi_), "rjdphi/F");

  // -- jet info --
  tree_->Branch("nljet", &(this->nljet_), "nljet/F");
  tree_->Branch("nljetsm", &(this->nljetsm_), "nljetsm/F");
  tree_->Branch("nljeta", &(this->nljeta_), "nljeta/F");
  tree_->Branch("nljphi", &(this->nljphi_), "nljphi/F");
  tree_->Branch("nljemf", &(this->nljemf_), "nljemf/F");
  tree_->Branch("nljN90hits", &(this->nljN90hits_), "nljN90hits/I");

  tree_->Branch("aljet", &(this->aljet_), "aljet/F");
  tree_->Branch("aljetsm", &(this->aljetsm_), "aljetsm/F");
  tree_->Branch("aljeta", &(this->aljeta_), "aljeta/F");
  tree_->Branch("aljphi", &(this->aljphi_), "aljphi/F");
  tree_->Branch("aljemf", &(this->aljemf_), "aljemf/F");
  tree_->Branch("aljN90hits", &(this->aljN90hits_), "aljN90hits/I");

  tree_->Branch("jdphi", &(this->jdphi_), "jdphi/F");

  // -- particle info --
  tree_->Branch("evtnp",&(this->evtnp_),"evtnp/I");
  tree_->Branch("ppid",this->ppid_,"ppid[evtnp]/I");
  tree_->Branch("pch",this->pch_,"pch[evtnp]/I");
  tree_->Branch("ppt",this->ppt_,"ppt[evtnp]/F");
  tree_->Branch("peta",this->peta_,"peta[evtnp]/F");
  tree_->Branch("pphi",this->pphi_,"pphi[evtnp]/F");

  tree_->Branch("pndphi",this->pndphi_,"pndphi[evtnp]/F");
  //tree_->Branch("pndeta",this->pndeta_,"pndeta[evtnp]/F");
  tree_->Branch("pndr",this->pndr_,"pndr[evtnp]/F");
  tree_->Branch("pndrbg",this->pndrbg_,"pndrbg[evtnp]/F");

  tree_->Branch("padphi",this->padphi_,"padphi[evtnp]/F");
  //tree_->Branch("padeta",this->padeta_,"padeta[evtnp]/F");
  tree_->Branch("padr",this->padr_,"padr[evtnp]/F");
  tree_->Branch("padrbg",this->padrbg_,"padrbg[evtnp]/F");

  tree_->Branch("zn",this->zn_,"zn[evtnp]/F");
  tree_->Branch("za",this->za_,"za[evtnp]/F");

  //tree_->Branch("trkHP",this->trkHP_,"trkHP[evtnp]/I");
  //tree_->Branch("trkNHits",this->trkNHits_,"trkNHits[evtnp]/I");
  //tree_->Branch("trkPtErr",this->trkPtErr_,"trkPtErr[evtnp]/F");
  //tree_->Branch("trkdz",this->trkdz_,"trkdz[evtnp]/F");
  //tree_->Branch("trkdxy",this->trkdxy_,"trkdxy[evtnp]/F");

  // -- jet cone info --
  tree_->Branch("nljCone5NP", &(this->nljCone5NP_), "nljCone5NP/I");
  tree_->Branch("nljCone5Et", &(this->nljCone5Et_), "nljCone5Et/F");
  tree_->Branch("aljCone5NP", &(this->aljCone5NP_), "aljCone5NP/I");
  tree_->Branch("aljCone5Et", &(this->aljCone5Et_), "aljCone5Et/F");
  tree_->Branch("nljCone5NPBg", &(this->nljCone5NPBg_), "nljCone5NPBg/I");
  tree_->Branch("nljCone5EtBg", &(this->nljCone5EtBg_), "nljCone5EtBg/F");
  tree_->Branch("aljCone5NPBg", &(this->aljCone5NPBg_), "aljCone5NPBg/I");
  tree_->Branch("aljCone5EtBg", &(this->aljCone5EtBg_), "aljCone5EtBg/F");
  
  // -- jes vars --
  //tree_->Branch("meanppt", &(this->meanppt_), "meanppt/F");
}
void TreeDiJetEventData::Clear()
{
  // event
  run_	  = -99;
  evt_	  = -99;
  nvtx_	  = -99;
  vtxntrks_	  = -99;
  vtxndof_	  = -99;
  vz_		  = -99;
  // di-jet
  djmass_	  = -99;
  rdjmass_	  = -99;
  cmeta_	  = -99;
  jdphi_	  = -99;
  // jet
  nljet_	  = -99;
  aljet_	  = -99;
  alrjid_	  = 0;
  alrjstat_	  = -99;
  nlrjet_	  = -99;
  alrjet_	  = -99;
  // jet constituents
  evtnp_      = 0;

  // cone info
  nljCone5NP_ = 0;
  nljCone5Et_ = 0;
  aljCone5NP_ = 0;
  aljCone5Et_ = 0;
  nljCone5NPBg_ = 0;
  nljCone5EtBg_ = 0;
  aljCone5NPBg_ = 0;
  aljCone5EtBg_ = 0;

  // jes vars
  meanppt_	  = -99;
}

