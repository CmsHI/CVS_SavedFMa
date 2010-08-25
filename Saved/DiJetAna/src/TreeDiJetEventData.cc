#include "Saved/DiJetAna/interface/TreeDiJetEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
using namespace std;

// Defaults
void TreeDiJetEventData::SetDefaults()
{
  // Class Defaults
  numJec_ = MAXJEC;
  for (Int_t i=0; i<numJec_; ++i) {
    njec_[i]=1;
    ajec_[i]=1;
  }
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
void TreeDiJetEventData::CalcDJVars(Bool_t isMC,
    const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
    const std::vector<math::PtEtaPhiMLorentzVector> & refjets)
{
  // check if lead jet found, if not just use defaults
  if (anajets.size()<1) return;

  // near/away info     
  nljet_	      = anajets[0].pt();
  nljeta_	      = anajets[0].eta();
  nljphi_	      = anajets[0].phi();
  if (anajets.size()>=2) {
    aljet_	      = anajets[1].pt();
    aljeta_	      = anajets[1].eta();
    aljphi_	      = anajets[1].phi();

    // dijet info
    jdphi_	      = TMath::Abs(reco::deltaPhi(anajets[0].phi(),anajets[1].phi()));
    djmass_	      = (anajets[0]+anajets[1]).M();
  }

  // Done if data
  if (!isMC) return;

  // refjets
  // check if there is matched jets, if not just use defaults
  if (refjets.size()<1) return;

  // near/away info     
  nlrjet_	      = refjets[0].pt();
  nlrjeta_	      = refjets[0].eta();
  nlrjphi_	      = refjets[0].phi();
  if (refjets.size()>=2) {
    alrjet_           = refjets[1].pt();
    alrjeta_          = refjets[1].eta();
    alrjphi_          = refjets[1].phi();

    // dijet info
    rjdphi_	      = TMath::Abs(reco::deltaPhi(refjets[0].phi(),refjets[1].phi()));
    rdjmass_	      = (refjets[0]+refjets[1]).M();
  }
}

void TreeDiJetEventData::CalcTrkVars(Bool_t isMC,
    const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
    const math::PtEtaPhiMLorentzVector & anaTrk, Int_t it)
{
  // Basic Kinematic Info
  ppt_[it]		= anaTrk.pt();
  peta_[it]		= anaTrk.eta();
  pphi_[it]		= anaTrk.phi();

  if (anajets.size()<1) {
    pndphi_[it]=-99; pndr_[it]=-99; pndrbg_[it]=-99;
    padphi_[it]=-99; padr_[it]=-99; padrbg_[it]=-99;
    return;
  }
  // Relations to near jet
  pndphi_[it]		= reco::deltaPhi(pphi_[it],anajets[0].phi());
  pndeta_[it]		= peta_[it] - anajets[0].eta();
  pndr_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[0].eta(),anajets[0].phi());
  //  - background variables-
  pndrbg_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[0].eta(),anajets[0].phi()+TMath::PiOver2());
  // jet cone info
  if (ppt_[it]>1.2 && ppt_[it]<anajets[0].pt()){
    if (pndr_[it]<0.5) {
      ++nljC5NP_;
      nljC5Pt_+=ppt_[it];
    }
    if (pndrbg_[it]<0.5) {
      ++nljC5NPBg_;
      nljC5PtBg_+=ppt_[it];
    }
  }
  // fragmentation variables
  // will change to dijet frame soon
  zn_[it]		= ppt_[it]/anajets[0].pt();

  if (anajets.size()<2) {
    padphi_[it]=-99; padr_[it]=-99; padrbg_[it]=-99;
    return;
  }
  // Relations to away jet
  padphi_[it]		= reco::deltaPhi(pphi_[it],anajets[1].phi());
  padeta_[it]		= peta_[it] - anajets[1].eta();
  padr_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[1].eta(),anajets[1].phi());
  //  - background variables-
  padrbg_[it]		= reco::deltaR(peta_[it],pphi_[it],anajets[1].eta(),anajets[1].phi()+TMath::PiOver2());
  // jet cone info
  if (ppt_[it]>1.2 && ppt_[it]<anajets[0].pt()){
    if (padr_[it]<0.5) {
      ++aljC5NP_;
      aljC5Pt_+=ppt_[it];
    }
    if (padrbg_[it]<0.5) {
      ++aljC5NPBg_;
      aljC5PtBg_+=ppt_[it];
    }
  }
  // fragmentation variables
  // will change to dijet frame soon
  za_[it]		= ppt_[it]/anajets[1].pt();
}

// set brances
void TreeDiJetEventData::SetBranches()
{
  // --event level--
  tree_->Branch("run", &(this->run_), "run/I");
  tree_->Branch("evt", &(this->evt_), "evt/I");
  tree_->Branch("lumi", &(this->lumi_), "lumi/I");
  tree_->Branch("hlt", &hlt_);
  tree_->Branch("nvtx", &(this->nvtx_), "nvtx/I");
  tree_->Branch("vtxntrks", &(this->vtxntrks_), "vtxntrks/I");
  tree_->Branch("vtxndof", &(this->vtxndof_), "vtxndof/F");
  tree_->Branch("vtxchi2", &(this->vtxchi2_), "vtxchi2/F");
  tree_->Branch("vz", &(this->vz_), "vz/F");
  tree_->Branch("njets", &(this->njets_), "njets/I");
  tree_->Branch("ntrks", &(this->ntrks_), "ntrks/I");
  // -- hi event var's --
  tree_->Branch("b", &(this->b_), "b/F");
  tree_->Branch("npart", &(this->npart_), "npart/I");
  tree_->Branch("ncoll", &(this->ncoll_), "ncoll/I");
  tree_->Branch("cent", &(this->cent_), "cent/F");

  // -- dijet info --
  tree_->Branch("djmass", &(this->djmass_), "djmass/F");
  tree_->Branch("rdjmass", &(this->rdjmass_), "rdjmass/F");
  tree_->Branch("cmeta", &(this->cmeta_), "cmeta/F");
  //tree_->Branch("lajmul", &(this->leadAwayMul_), "lajmul/I");
  // --ref jet info--
  tree_->Branch("nlrjid", &(this->nlrjid_), "nlrjid/I");
  tree_->Branch("nlrjstat", &(this->nlrjstat_), "nlrjstat/I");
  tree_->Branch("nlrjet", &(this->nlrjet_), "nlrjet/F");
  tree_->Branch("nlrjeta", &(this->nlrjeta_), "nlrjeta/F");
  tree_->Branch("nlrjphi", &(this->nlrjphi_), "nlrjphi/F");

  tree_->Branch("alrjid", &(this->alrjid_), "alrjid/I");
  tree_->Branch("alrjstat", &(this->alrjstat_), "alrjstat/I");
  tree_->Branch("alrjet", &(this->alrjet_), "alrjet/F");
  tree_->Branch("alrjeta", &(this->alrjeta_), "alrjeta/F");
  tree_->Branch("alrjphi", &(this->alrjphi_), "alrjphi/F");

  tree_->Branch("rjdphi", &(this->rjdphi_), "rjdphi/F");

  // -- jet info --
  tree_->Branch("nljet", &(this->nljet_), "nljet/F");
  tree_->Branch("nljrawet", &(this->nljrawet_), "nljrawet/F");
  tree_->Branch("nljeta", &(this->nljeta_), "nljeta/F");
  tree_->Branch("nljphi", &(this->nljphi_), "nljphi/F");
  tree_->Branch("nljarea", &(this->nljarea_), "nljarea/F");
  tree_->Branch("nljemf", &(this->nljemf_), "nljemf/F");
  tree_->Branch("nljN90hits", &(this->nljN90hits_), "nljN90hits/I");

  tree_->Branch("aljet", &(this->aljet_), "aljet/F");
  tree_->Branch("aljrawet", &(this->aljrawet_), "aljrawet/F");
  tree_->Branch("aljeta", &(this->aljeta_), "aljeta/F");
  tree_->Branch("aljphi", &(this->aljphi_), "aljphi/F");
  tree_->Branch("aljemf", &(this->aljemf_), "aljemf/F");
  tree_->Branch("aljarea", &(this->aljarea_), "aljarea/F");
  tree_->Branch("aljN90hits", &(this->aljN90hits_), "aljN90hits/I");

  tree_->Branch("jdphi", &(this->jdphi_), "jdphi/F");

  // -- jec info --
  tree_->Branch("numJec",&(this->numJec_),"numJec/I");
  tree_->Branch("njec",this->njec_,"njec[numJec]/F");
  tree_->Branch("ajec",this->ajec_,"ajec[numJec]/F");

  // -- particle info --
  tree_->Branch("evtnp",&(this->evtnp_),"evtnp/I");
  tree_->Branch("psube",this->psube_,"psube[evtnp]/I");
  tree_->Branch("ppid",this->ppid_,"ppid[evtnp]/I");
  tree_->Branch("pch",this->pch_,"pch[evtnp]/I");
  tree_->Branch("ppt",this->ppt_,"ppt[evtnp]/F");
  tree_->Branch("peta",this->peta_,"peta[evtnp]/F");
  tree_->Branch("pphi",this->pphi_,"pphi[evtnp]/F");

  tree_->Branch("pndphi",this->pndphi_,"pndphi[evtnp]/F");
  tree_->Branch("pndr",this->pndr_,"pndr[evtnp]/F");
  tree_->Branch("pndrbg",this->pndrbg_,"pndrbg[evtnp]/F");

  tree_->Branch("padphi",this->padphi_,"padphi[evtnp]/F");
  tree_->Branch("padr",this->padr_,"padr[evtnp]/F");
  tree_->Branch("padrbg",this->padrbg_,"padrbg[evtnp]/F");

  tree_->Branch("zn",this->zn_,"zn[evtnp]/F");
  tree_->Branch("za",this->za_,"za[evtnp]/F");

  tree_->Branch("trkNHits",this->trkNHits_,"trkNHits[evtnp]/I");
  //tree_->Branch("trkPtErr",this->trkPtErr_,"trkPtErr[evtnp]/F");
  //tree_->Branch("trkdz",this->trkdz_,"trkdz[evtnp]/F");
  //tree_->Branch("trkdxy",this->trkdxy_,"trkdxy[evtnp]/F");

  // -- jet cone info --
  tree_->Branch("nljC5NP", &(this->nljC5NP_), "nljC5NP/I");
  tree_->Branch("nljC5Pt", &(this->nljC5Pt_), "nljC5Pt/F");
  tree_->Branch("aljC5NP", &(this->aljC5NP_), "aljC5NP/I");
  tree_->Branch("aljC5Pt", &(this->aljC5Pt_), "aljC5Pt/F");
  tree_->Branch("nljC5NPBg", &(this->nljC5NPBg_), "nljC5NPBg/I");
  tree_->Branch("nljC5PtBg", &(this->nljC5PtBg_), "nljC5PtBg/F");
  tree_->Branch("aljC5NPBg", &(this->aljC5NPBg_), "aljC5NPBg/I");
  tree_->Branch("aljC5PtBg", &(this->aljC5PtBg_), "aljC5PtBg/F");
  
  // -- jes vars --
  //tree_->Branch("meanppt", &(this->meanppt_), "meanppt/F");
}
void TreeDiJetEventData::Clear()
{
  // event
  run_		  = -99;
  evt_		  = -99;
  lumi_		  = -99;
  hlt_.clear();
  cent_		  = -99;
  nvtx_		  = -99;
  vtxntrks_	  = -99;
  vtxndof_	  = -99;
  vtxchi2_	  = -99;
  vz_		  = -99;
  njets_	  = -99;
  ntrks_	  = -99;
  // di-jet
  djmass_	  = -99;
  rdjmass_	  = -99;
  cmeta_	  = -99;
  jdphi_	  = -99;
  // anajet
  nljet_	  = -99;
  aljet_	  = -99;
  nljrawet_	  = -99;
  aljrawet_	  = -99;
  nljeta_	  = -99;
  nljphi_	  = -99;
  aljeta_	  = -99;
  aljphi_	  = -99;
  nljarea_	  = -99;
  aljarea_	  = -99;
  // refjet
  nlrjet_	  = -99;
  alrjet_	  = -99;
  nlrjeta_	  = -99;
  nlrjphi_	  = -99;
  alrjeta_	  = -99;
  alrjphi_	  = -99;
  nlrjid_	  = 0;
  nlrjstat_	  = -99;
  alrjid_	  = 0;
  alrjstat_	  = -99;
  rjdphi_	  = -99;
  // jet constituents
  evtnp_      = 0;

  // cone info
  nljC5NP_	  = 0;
  nljC5Pt_	  = 0;
  aljC5NP_	  = 0;
  aljC5Pt_	  = 0;
  nljC5NPBg_	  = 0;
  nljC5PtBg_	  = 0;
  aljC5NPBg_	  = 0;
  aljC5PtBg_	  = 0;

  // jes vars
  for (Int_t i=0; i<numJec_; ++i) {
    njec_[i]=1;
    ajec_[i]=1;
  }
  //meanppt_	  = -99;
}

