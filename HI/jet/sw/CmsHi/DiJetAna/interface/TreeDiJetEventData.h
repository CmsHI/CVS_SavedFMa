//
// Class to represent event tree data
//

#ifndef TREEDIJETEVENTDATA_H
#define TREEDIJETEVENTDATA_H
#include <vector>
#include "TTree.h"
#include "DataFormats/Math/interface/LorentzVector.h"

const Int_t MAXTRK = 100000;
//
// DiJet ana Event Data Tree definition
//
class TreeDiJetEventData
{
  public:
    // ===== Class Methods =====
    void SetDefaults();
    TreeDiJetEventData();
    TreeDiJetEventData(TTree * tree);
    void SetTree(TTree * t) { tree_=t; }
    void SetBranches();
    void Clear();
    void CalcDJVars(Bool_t isMC,
	std::vector<math::PtEtaPhiMLorentzVector> anajets,
	std::vector<math::PtEtaPhiMLorentzVector> refjets);
    void CalcTrkVars(Bool_t isMC,
	std::vector<math::PtEtaPhiMLorentzVector> anajets,
	math::PtEtaPhiMLorentzVector anaTrk, Int_t it);

    // ===== Ana Data =====
    //  --- event level ---
    Int_t run_, evt_, lumi_,nvtx_,vtxntrks_;
    Float_t vtxndof_,vtxchi2_,vz_;
    // -- HI Event variables --
    Float_t		    b_;
    Int_t		    npart_,ncoll_;

    // --- DiJet variables ---
    Float_t		    djmass_,cmeta_,rdjmass_;
    Int_t		    leadAwayMul_;

    //  -- ref jet info --
    Int_t	  	    nlrjid_,nlrjstat_;
    Float_t		    nlrjet_,nlrjetsm_,nlrjeta_,nlrjphi_;

    Int_t	  	    alrjid_,alrjstat_;
    Float_t		    alrjet_,alrjetsm_,alrjeta_,alrjphi_;

    Float_t		    rjdphi_;

    //  -- jet info --
    Float_t		    nljet_,nljetsm_,nljeta_,nljphi_;
    Float_t		    nljemf_;
    Int_t		    nljN90hits_;

    Float_t		    aljet_,aljetsm_,aljeta_,aljphi_;
    Float_t		    aljemf_;
    Int_t		    aljN90hits_;

    Float_t		    jdphi_;

    // -- particle info --
    Int_t		    evtnp_;
    Int_t	  	    psube_[MAXTRK],ppid_[MAXTRK],pch_[MAXTRK];
    Float_t	      	    ppt_[MAXTRK],peta_[MAXTRK],pphi_[MAXTRK];

    Float_t	  	    pndphi_[MAXTRK],pndeta_[MAXTRK],pndr_[MAXTRK],pndrbg_[MAXTRK];

    Float_t        	    padphi_[MAXTRK],padeta_[MAXTRK],padr_[MAXTRK],padrbg_[MAXTRK];

    Float_t       	    zn_[MAXTRK],za_[MAXTRK];

    //  - track sel -
    //Int_t	       	    trkHP_[MAXTRK],trkNHits_[MAXTRK];
    //Float_t	       	    trkPtErr_[MAXTRK],trkdz_[MAXTRK],trkdxy_[MAXTRK];

    // -- jet cone info --
    Int_t		    nljCone5NP_,nljCone5NPBg_;
    Float_t		    nljCone5Et_,nljCone5EtBg_;

    Int_t		    aljCone5NP_,aljCone5NPBg_;
    Float_t	      	    aljCone5Et_,aljCone5EtBg_;

    // -- jes vars --
    Float_t		   meanppt_;

  private:
    TTree*                 tree_;
};
#endif // TREEDIJETEVENTDATA_H
