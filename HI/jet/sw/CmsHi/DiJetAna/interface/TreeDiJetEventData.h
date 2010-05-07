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
	std::vector<math::PtEtaPhiMLorentzVectorF> anajets,
	std::vector<math::PtEtaPhiMLorentzVectorF> refjets);
    void CalcJetTrkVars();

    // ===== Ana Data =====
    //  --- event level ---
    Int_t run_, evt_, lumi_,nvtx_,vtxntrks_;
    Double_t vtxndof_,vtxchi2_,vz_;
    // -- HI Event variables --
    Double_t		    b_;
    Int_t		    npart_,ncoll_;

    // --- DiJet variables ---
    Double_t		    djmass_,cmeta_,rdjmass_;
    Int_t		    leadAwayMul_;

    //  -- ref jet info --
    Int_t	  	    nlrjid_,nlrjstat_;
    Double_t		    nlrjet_,nlrjetsm_,nlrjeta_,nlrjphi_;

    Int_t	  	    alrjid_,alrjstat_;
    Double_t		    alrjet_,alrjetsm_,alrjeta_,alrjphi_;

    Double_t		    rjdphi_;

    //  -- jet info --
    Double_t		    nljet_,nljetsm_,nljeta_,nljphi_;
    Double_t		    nljemf_;
    Int_t		    nljN90hits_;

    Double_t		    aljet_,aljetsm_,aljeta_,aljphi_;
    Double_t		    aljemf_;
    Int_t		    aljN90hits_;

    Double_t		    jdphi_;

    // -- particle info --
    Int_t		    evtnp_;
    Int_t	  	    ppid_[MAXTRK],pch_[MAXTRK];
    Double_t	      	    ppt_[MAXTRK],peta_[MAXTRK],pphi_[MAXTRK];

    Double_t	  	    pndphi_[MAXTRK],pndeta_[MAXTRK],pndr_[MAXTRK],pndrbg_[MAXTRK];

    Double_t        	    padphi_[MAXTRK],padeta_[MAXTRK],padr_[MAXTRK],padrbg_[MAXTRK];

    Double_t       	    zn_[MAXTRK],za_[MAXTRK];

    //  - track sel -
    Int_t	       	    trkHP_[MAXTRK],trkNHits_[MAXTRK];
    Double_t	       	    trkPtErr_[MAXTRK],trkdz_[MAXTRK],trkdxy_[MAXTRK];

    // -- jet cone info --
    Int_t		    nljCone5NP_,nljCone10NP_,nljCone15NP_;
    Double_t		    nljCone5Et_,nljCone10Et_,nljCone15Et_;

    Int_t		    aljCone5NP_,aljCone10NP_,aljCone15NP_;
    Double_t	      	    aljCone5Et_,aljCone10Et_,aljCone15Et_;

    // -- jes vars --
    Double_t		   meanppt_;

  private:
    TTree*                             tree_;
};
#endif // TREEDIJETEVENTDATA_H
