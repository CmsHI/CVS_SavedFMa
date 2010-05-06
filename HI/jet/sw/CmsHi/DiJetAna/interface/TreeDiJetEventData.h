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
    void CalcDJVars(std::vector<math::PtEtaPhiMLorentzVectorF> anajets);
    void CalcJetTrkVars();

    // ===== Ana Data =====
    //  --- event level ---
    Int_t run_, evt_, lumi_,nvtx_,vtxntrks_;
    Double_t vtxndof_,vtxchi2_,vz_;
    // -- HI Event variables --
    Double_t		    b_;
    Int_t		    npart_,ncoll_;

    // --- DiJet variables ---
    Double_t		    mass_,cmeta_;
    Int_t		    leadAwayMul_;

    //  -- gen jet/parton info --
    Int_t	  	    nlpid_,nlpstat_;
    Double_t		    nlpet_,nlpetsm_,nlpeta_,nlpphi_;

    Int_t	  	    alpid_,alpstat_;
    Double_t		    alpet_,alpetsm_,alpeta_,alpphi_;

    Double_t		    pdphi_;

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
