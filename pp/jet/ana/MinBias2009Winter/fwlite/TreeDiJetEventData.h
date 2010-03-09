//
// Class to represent event tree data
// - inspiration from:
//   * http://root.cern.ch/root/html/tutorials/tree/hvector.C.html
//

#ifndef TREEDIJETEVENTDATA_H
#define TREEDIJETEVENTDATA_H
#include <vector>
#include "TTree.h"

namespace jetana
{
  const Int_t MAXTRK = 20000;
  //
  // DiJet ana Event Data Tree definition
  //
  class TreeDiJetEventData
  {
    public:
      //  --- event level ---
      Int_t run_, evt_, lumi_,nvtx_;
      Double_t vtxndof,_vtxchi2_,vz_;
      // -- HI Event variables --
      Double_t		    b_;
      Int_t		    npart_,ncoll_;

      // --- DiJet variables ---
      Double_t		    mass_,cmeta_;

      //  -- parton info --
      Int_t	  	    nlpid_,nlpstat_;
      Double_t		    nlpet_,nlpetsm_;
      Double_t		    nlpeta_,nlpphi_;

      Int_t	  	    alpid_,alpstat_;
      Double_t		    alpet_,alpetsm_;
      Double_t		    alpeta_,alpphi_;

      Double_t		    pdphi_;

      //  -- jet info --
      Double_t		    nljet_,nljetsm_;
      Double_t		    nljeta_,nljphi_;
      Double_t		    nljemf_;
      Int_t		    nljN90hits_;

      Double_t		    aljet_,aljetsm_;
      Double_t		    aljeta_,aljphi_;
      Double_t		    aljemf_;
      Int_t		    aljN90hits_;

      Double_t		    jdphi_;
      Int_t		    leadAwayMul_;

      // -- particle info --
      Int_t		    evtnp_;
      Int_t	  	    ppid_[MAXTRK],pch_[MAXTRK];
      Double_t	      	    ppt_[MAXTRK],peta_[MAXTRK],pphi_[MAXTRK];

      Double_t	  	    pndphi_[MAXTRK],pndeta_[MAXTRK];
      Double_t	   	    pndr_[MAXTRK],pndrbg_[MAXTRK];

      Double_t        	    padphi_[MAXTRK],padeta_[MAXTRK];
      Double_t       	    padr_[MAXTRK],padrbg_[MAXTRK];

      Double_t       	    zn_[MAXTRK],za_[MAXTRK];

      //  - track sel -
      Int_t	       	    trkHP_[MAXTRK],trkNHits_[MAXTRK];
      Double_t	       	    trkPtErr_[MAXTRK],trkdz_[MAXTRK],trkdxy_[MAXTRK];

      // -- jet cone info --
      Double_t		    nljCone5Et_,nljCone10Et_,nljCone15Et_;
      Int_t		    nljCone5NP_,nljCone10NP_,nljCone15NP_;

      Double_t	      	    aljCone5Et_,aljCone10Et_,aljCone15Et_;
      Int_t		    aljCone5NP_,aljCone10NP_,aljCone15NP_;

      // === Functions ===
      void SetDefaults();
      TreeDiJetEventData();
      TreeDiJetEventData(TTree * tree);
      void SetTree(TTree * t) { tree_=t; }
      void SetBranches();
      void Clear();

    private:
      TTree*                             tree_;
  };

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

  // set brances
  void TreeDiJetEventData::SetBranches()
  {
    // --event level--
    tree_->Branch("run", &(this->run_), "run/I");
    tree_->Branch("evt", &(this->evt_), "evt/I");
    tree_->Branch("lumi", &(this->lumi_), "lumi/I");
    tree_->Branch("vtxchi2", &(this->vtxchi2_), "vtxchi2/D");
    tree_->Branch("vtxndof", &(this->vtxndof_), "vtxndof/D");
    tree_->Branch("nvtx", &(this->nvtx_), "nvtx/I");
    tree_->Branch("vz", &(this->vz_), "vz/D");
    // -- hi event var's --
    tree_->Branch("b", &(this->b_), "b/D");
    tree_->Branch("npart", &(this->npart_), "npart/I");
    tree_->Branch("ncoll", &(this->ncoll_), "ncoll/I");

    // -- dijet info --
    tree_->Branch("mass", &(this->mass_), "mass/D");
    tree_->Branch("cmeta", &(this->cmeta_), "cmeta/D");
    tree_->Branch("lajmul", &(this->leadAwayMul_), "lajmul/I");
    // --parton info--
    tree_->Branch("nlpid", &(this->nlpid_), "nlpid/I");
    tree_->Branch("nlpstat", &(this->nlpstat_), "nlpstat/I");
    tree_->Branch("nlpet", &(this->nlpet_), "nlpet/D");
    tree_->Branch("nlpetsm", &(this->nlpetsm_), "nlpetsm/D");
    tree_->Branch("nlpeta", &(this->nlpeta_), "nlpeta/D");
    tree_->Branch("nlpphi", &(this->nlpphi_), "nlpphi/D");

    tree_->Branch("alpid", &(this->alpid_), "alpid/I");
    tree_->Branch("alpstat", &(this->alpstat_), "alpstat/I");
    tree_->Branch("alpet", &(this->alpet_), "alpet/D");
    tree_->Branch("alpetsm", &(this->alpetsm_), "alpetsm/D");
    tree_->Branch("alpeta", &(this->alpeta_), "alpeta/D");
    tree_->Branch("alpphi", &(this->alpphi_), "alpphi/D");

    tree_->Branch("pdphi", &(this->pdphi_), "pdphi/D");

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
    tree_->Branch("aljN90hits", &(this->aljN90hits_), "aljN90hits/D");

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
    tree_->Branch("nljCone5Et", &(this->nljCone5Et_), "nljCone5Et/D");
    tree_->Branch("nljCone5NP", &(this->nljCone5NP_), "nljCone5NP/I");
    tree_->Branch("nljCone10Et", &(this->nljCone10Et_), "nljCone10Et/D");
    tree_->Branch("nljCone10NP", &(this->nljCone10NP_), "nljCone10NP/I");
    tree_->Branch("nljCone15Et", &(this->nljCone15Et_), "nljCone15Et/D");
    tree_->Branch("nljCone15NP", &(this->nljCone15NP_), "nljCone15NP/I");

    tree_->Branch("aljCone5Et", &(this->aljCone5Et_), "aljCone5Et/D");
    tree_->Branch("aljCone5NP", &(this->aljCone5NP_), "aljCone5NP/I");
    tree_->Branch("aljCone10Et", &(this->aljCone10Et_), "aljCone10Et/D");
    tree_->Branch("aljCone10NP", &(this->aljCone10NP_), "aljCone10NP/I");
    tree_->Branch("aljCone15Et", &(this->aljCone15Et_), "aljCone15Et/D");
    tree_->Branch("aljCone15NP", &(this->aljCone15NP_), "aljCone15NP/I");
  }
  void TreeDiJetEventData::Clear()
  {
    // event
    run_	  = -99;
    evt_	  = -99;
    vtxndof_	  = -99;
    vz_		  = -99;
    nvtx_	  = -99;
    // di-jet
    mass_	  = -99;
    cmeta_	  = -99;
    jdphi_	  = -99;
    // jet
    nljet_	  = -99;
    aljet_	  = -99;
    // jet constituents
    evtnp_      = 0;
    nljCone5Et_ = 0;
    nljCone5NP_ = 0;
    nljCone10Et_ = 0;
    nljCone10NP_ = 0;
    nljCone15Et_ = 0;
    nljCone15NP_ = 0;
    aljCone5Et_ = 0;
    aljCone5NP_ = 0;
    aljCone10Et_ = 0;
    aljCone10NP_ = 0;
    aljCone15Et_ = 0;
    aljCone15NP_ = 0;
  }
}

#endif // TREEDIJETEVENTDATA_H
