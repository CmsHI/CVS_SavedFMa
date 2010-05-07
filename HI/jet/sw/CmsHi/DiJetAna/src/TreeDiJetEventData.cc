#include "CmsHi/DiJetAna/interface/TreeDiJetEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

// Defaults
void TreeDiJetEventData::SetDefaults()
{
  // -- jet cone info --
  nljCone5Et_ = -1;
  nljCone5NP_ = -1;
  nljCone10Et_ = -1;
  nljCone10NP_ = -1;
  nljCone15Et_ = -1;
  nljCone15NP_ = -1;
  aljCone5Et_ = -1;
  aljCone5NP_ = -1;
  aljCone10Et_ = -1;
  aljCone10NP_ = -1;
  aljCone15Et_ = -1;
  aljCone15NP_ = -1;
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
void TreeDiJetEventData::CalcDJVars(Bool_t isMC, std::vector<math::PtEtaPhiMLorentzVectorF> anajets,
    std::vector<math::PtEtaPhiMLorentzVectorF> refjets)
{
  // near/away info     
  nljet_	     = anajets[0].pt();
  nljeta_	     = anajets[0].eta();
  nljphi_	     = anajets[0].phi();
  aljet_	     = anajets[1].pt();
  aljeta_	     = anajets[1].eta();
  aljphi_	     = anajets[1].phi();

  // dijet info
  Double_t jdphi     = TMath::Abs(reco::deltaPhi(anajets[0].phi(),anajets[1].phi()));
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
    Double_t rjdphi    = TMath::Abs(reco::deltaPhi(refjets[0].phi(),refjets[1].phi()));
    rjdphi_            = rjdphi;
    rdjmass_	     = (refjets[0]+refjets[1]).M();
  }
}

void TreeDiJetEventData::CalcTrkVars(Bool_t isMC, std::vector<math::PtEtaPhiMLorentzVectorF> anajets)
{
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
  tree_->Branch("vtxndof", &(this->vtxndof_), "vtxndof/D");
  tree_->Branch("vtxchi2", &(this->vtxchi2_), "vtxchi2/D");
  tree_->Branch("vz", &(this->vz_), "vz/D");
  // -- hi event var's --
  tree_->Branch("b", &(this->b_), "b/D");
  tree_->Branch("npart", &(this->npart_), "npart/I");
  tree_->Branch("ncoll", &(this->ncoll_), "ncoll/I");

  // -- dijet info --
  tree_->Branch("djmass", &(this->djmass_), "djmass/D");
  tree_->Branch("rdjmass", &(this->rdjmass_), "rdjmass/D");
  tree_->Branch("cmeta", &(this->cmeta_), "cmeta/D");
  tree_->Branch("lajmul", &(this->leadAwayMul_), "lajmul/I");
  // --ref jet info--
  tree_->Branch("nlrjid", &(this->nlrjid_), "nlrjid/I");
  tree_->Branch("nlrjstat", &(this->nlrjstat_), "nlrjstat/I");
  tree_->Branch("nlrjet", &(this->nlrjet_), "nlrjet/D");
  tree_->Branch("nlrjetsm", &(this->nlrjetsm_), "nlrjetsm/D");
  tree_->Branch("nlrjeta", &(this->nlrjeta_), "nlrjeta/D");
  tree_->Branch("nlrjphi", &(this->nlrjphi_), "nlrjphi/D");

  tree_->Branch("alrjid", &(this->alrjid_), "alrjid/I");
  tree_->Branch("alrjstat", &(this->alrjstat_), "alrjstat/I");
  tree_->Branch("alrjet", &(this->alrjet_), "alrjet/D");
  tree_->Branch("alrjetsm", &(this->alrjetsm_), "alrjetsm/D");
  tree_->Branch("alrjeta", &(this->alrjeta_), "alrjeta/D");
  tree_->Branch("alrjphi", &(this->alrjphi_), "alrjphi/D");

  tree_->Branch("rjdphi", &(this->rjdphi_), "rjdphi/D");

  // -- jet info --
  tree_->Branch("nljet", &(this->nljet_), "nljet/D");
  tree_->Branch("nljetsm", &(this->nljetsm_), "nljetsm/D");
  tree_->Branch("nljeta", &(this->nljeta_), "nljeta/D");
  tree_->Branch("nljphi", &(this->nljphi_), "nljphi/D");
  tree_->Branch("nljemf", &(this->nljemf_), "nljemf/D");
  tree_->Branch("nljN90hits", &(this->nljN90hits_), "nljN90hits/I");

  tree_->Branch("aljet", &(this->aljet_), "aljet/D");
  tree_->Branch("aljetsm", &(this->aljetsm_), "aljetsm/D");
  tree_->Branch("aljeta", &(this->aljeta_), "aljeta/D");
  tree_->Branch("aljphi", &(this->aljphi_), "aljphi/D");
  tree_->Branch("aljemf", &(this->aljemf_), "aljemf/D");
  tree_->Branch("aljN90hits", &(this->aljN90hits_), "aljN90hits/I");

  tree_->Branch("jdphi", &(this->jdphi_), "jdphi/D");

  // -- particle info --
  tree_->Branch("evtnp",&(this->evtnp_),"evtnp/I");
  tree_->Branch("ppid",this->ppid_,"ppid[evtnp]/I");
  tree_->Branch("pch",this->pch_,"pch[evtnp]/I");
  tree_->Branch("ppt",this->ppt_,"ppt[evtnp]/D");
  tree_->Branch("peta",this->peta_,"peta[evtnp]/D");
  tree_->Branch("pphi",this->pphi_,"pphi[evtnp]/D");

  tree_->Branch("pndphi",this->pndphi_,"pndphi[evtnp]/D");
  tree_->Branch("pndeta",this->pndeta_,"pndeta[evtnp]/D");
  tree_->Branch("pndr",this->pndr_,"pndr[evtnp]/D");
  tree_->Branch("pndrbg",this->pndrbg_,"pndrbg[evtnp]/D");

  tree_->Branch("padphi",this->padphi_,"padphi[evtnp]/D");
  tree_->Branch("padeta",this->padeta_,"padeta[evtnp]/D");
  tree_->Branch("padr",this->padr_,"padr[evtnp]/D");
  tree_->Branch("padrbg",this->padrbg_,"padrbg[evtnp]/D");

  tree_->Branch("zn",this->zn_,"zn[evtnp]/D");
  tree_->Branch("za",this->za_,"za[evtnp]/D");

  tree_->Branch("trkHP",this->trkHP_,"trkHP[evtnp]/I");
  tree_->Branch("trkNHits",this->trkNHits_,"trkNHits[evtnp]/I");
  tree_->Branch("trkPtErr",this->trkPtErr_,"trkPtErr[evtnp]/D");
  tree_->Branch("trkdz",this->trkdz_,"trkdz[evtnp]/D");
  tree_->Branch("trkdxy",this->trkdxy_,"trkdxy[evtnp]/D");

  // -- jet cone info --
  tree_->Branch("nljCone5NP", &(this->nljCone5NP_), "nljCone5NP/I");
  tree_->Branch("nljCone5Et", &(this->nljCone5Et_), "nljCone5Et/D");
  tree_->Branch("aljCone5NP", &(this->aljCone5NP_), "aljCone5NP/I");
  tree_->Branch("aljCone5Et", &(this->aljCone5Et_), "aljCone5Et/D");
  
  // -- jes vars --
  tree_->Branch("meanppt", &(this->meanppt_), "meanppt/D");
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

  // jes vars
  meanppt_	  = -99;
}

