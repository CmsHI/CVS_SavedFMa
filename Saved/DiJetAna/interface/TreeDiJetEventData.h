//
// Class to represent event tree data
//

#ifndef TREEDIJETEVENTDATA_H
#define TREEDIJETEVENTDATA_H
#include <vector>
#include "TTree.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <vector>

const Int_t MAXTRK = 100000;
const Int_t MAXJEC = 15;
//
// DiJet ana Event Data Tree definition
//
class TreeDiJetEventData
{
  public:
    // ===== Class Methods =====
    void SetDefaults();
    TreeDiJetEventData();
    void SetTree(TTree * t) { tree_=t; }
    void SetBranches(Int_t jetType=2, Int_t trkType=3);
    void Clear();
    void CalcDJVars(Bool_t isMC,
	const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
	const std::vector<math::PtEtaPhiMLorentzVector> & refjets);
    void CalcTrkVars(Bool_t isMC,
	const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
	const math::PtEtaPhiMLorentzVector & anaTrk, Int_t it);
    void FindLeadingTrk(Int_t np, Float_t * dRs, Float_t * pTs, Int_t ijet);
    void FindLeadingTrks()
    {
      if (nljet_>0) FindLeadingTrk(evtnp_,pndr_,ppt_,0);
      if (aljet_>0) FindLeadingTrk(evtnp_,padr_,ppt_,1);
    }

    // ===== Ana Data =====
    //  --- event level ---
    Int_t run_, evt_, lumi_,nvtx_,vtxntrks_,njets_,ntrks_;
    Float_t vtxndof_,vtxchi2_,vz_;
    // -- HI Event variables --
    Float_t		    b_,cent_;
    Int_t		    npart_,ncoll_;
    std::vector<Bool_t>	    hlt_;

    // --- DiJet variables ---
    Float_t		    djmass_,cmeta_,rdjmass_;
    Int_t		    leadAwayMul_;

    //  -- ref jet info --
    Int_t	  	    nlrjid_,nlrjstat_;
    Float_t		    nlrjet_,nlrjeta_,nlrjphi_;

    Int_t	  	    alrjid_,alrjstat_;
    Float_t		    alrjet_,alrjeta_,alrjphi_;

    Float_t		    rjdphi_;

    //  -- jet info --
    Float_t		    nljet_,nljrawet_,nljeta_,nljphi_;
    Float_t		    nljarea_,nljemf_,nljfhpd_;
    Int_t		    nljN90hits_;

    Float_t		    aljet_,aljrawet_,aljeta_,aljphi_;
    Float_t		    aljarea_,aljemf_,aljfhpd_;
    Int_t		    aljN90hits_;

    Float_t		    jdphi_;

    //  -- JEC --
    Int_t		    numJec_;
    Float_t       	    njec_[MAXJEC],ajec_[MAXJEC];

    // -- particle info --
    Int_t		    evtnp_;
    Int_t	  	    psube_[MAXTRK],ppid_[MAXTRK],pch_[MAXTRK];
    Float_t	      	    ppt_[MAXTRK],peta_[MAXTRK],pphi_[MAXTRK];

    Float_t	  	    pndphi_[MAXTRK],pndeta_[MAXTRK],pndr_[MAXTRK],pndrbg_[MAXTRK];

    Float_t        	    padphi_[MAXTRK],padeta_[MAXTRK],padr_[MAXTRK],padrbg_[MAXTRK];

    Float_t       	    zn_[MAXTRK],za_[MAXTRK];

    //  - track sel -
    Int_t	       	    trkNHits_[MAXTRK];
    Float_t	       	    trkPtErr_[MAXTRK],trkdz_[MAXTRK],trkdze_[MAXTRK],trkd0_[MAXTRK],trkd0e_[MAXTRK];

    // -- jet cone info --
    Int_t		    nljC5NP_,nljC5NPBg_;
    Float_t		    nljC5Pt_,nljC5PtBg_;

    Int_t		    aljC5NP_,aljC5NPBg_;
    Float_t	      	    aljC5Pt_,aljC5PtBg_;

    std::vector<Float_t>    lppt_;
    std::vector<Float_t>    lpjdr_;

    std::vector<Float_t>    lgppt_;
    std::vector<Float_t>    lgppid_;
    std::vector<Float_t>    lgpch_;

    // -- jes vars --
    Float_t		    meanppt_;

  private:
    TTree*                 tree_;
};
#endif // TREEDIJETEVENTDATA_H
