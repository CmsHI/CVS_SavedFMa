//
// Class to represent event tree data
// - inspiration from:
//   * http://root.cern.ch/root/html/tutorials/tree/hvector.C.html
//

#ifndef TREEDIJETEVENTDATA_H
#define TREEDIJETEVENTDATA_H
#include <vector>
#include "TTree.h"

namespace DiJetAna
{
   const Int_t MAXNPARTICLES = 10000;
   //
   // DiJet ana Event Data Tree definition
   //
   class TreeDiJetEventData
   {
      public:
	 // --- DiJet Event variables ---
	 //  --event level--
	 ULong_t		    run_;
	 ULong_t		    event_;
	 Double_t		    mass_;
	 Double_t		    cmeta_;

	 //  -- parton info --
	 Int_t	  		    nlpid_;
	 Int_t	  		    nlpstat_;
	 Double_t		    nlpet_;
	 Double_t		    nlpetsm_;
	 Double_t		    nlpeta_;
	 Double_t		    nlpphi_;

	 Int_t	  		    alpid_;
	 Int_t	  		    alpstat_;
	 Double_t		    alpet_;
	 Double_t		    alpetsm_;
	 Double_t		    alpeta_;
	 Double_t		    alpphi_;

	 Double_t		    pdphi_;

	 //  -- jet info --
	 Double_t		    nljet_;
	 Double_t		    nljetsm_;
	 Double_t		    nljeta_;
	 Double_t		    nljphi_;
	 ULong_t		    nljnp_;

	 Double_t		    aljet_;
	 Double_t		    aljetsm_;
	 Double_t		    aljeta_;
	 Double_t		    aljphi_;
	 ULong_t		    aljnp_;

	 Double_t		    jdphi_;

	 // -- particle info --
	 UInt_t			    evtnp_;
	 Int_t		  	    ppid_[MAXNPARTICLES];
	 Double_t	      	    ppt_[MAXNPARTICLES];
	 Double_t		    peta_[MAXNPARTICLES];
	 Double_t		    pphi_[MAXNPARTICLES];

	 Double_t	  	    pndphi_[MAXNPARTICLES];
	 Double_t	    	    pndeta_[MAXNPARTICLES];
	 Double_t	   	    pndr_[MAXNPARTICLES];
	 Double_t		    pndrbg_[MAXNPARTICLES];

	 Double_t        	    padphi_[MAXNPARTICLES];
	 Double_t       	    padeta_[MAXNPARTICLES];
	 Double_t       	    padr_[MAXNPARTICLES];
	 Double_t       	    padrbg_[MAXNPARTICLES];

	 Double_t       	    zn_[MAXNPARTICLES];
	 Double_t       	    za_[MAXNPARTICLES];
	 

	 // --- tree branch names ---
	 // jet level
	 TString tNJEt_;
	 TString tAJEt_;
	 TString tNPtnEt_;
	 TString tAPtnEt_;
	 TString tDPhi_;
	 TString tNJEta_;
	 TString tAJEta_;
	 // particle level
	 TString tPPt_;
	 TString tNJPDR_;
	 TString tAJPDR_;

	 // === Functions ===
	 TreeDiJetEventData(TTree * tree);
	 void SetBranches();

      private:
	 TTree*                             tree_;
   };

   // constructor
   TreeDiJetEventData::TreeDiJetEventData(TTree * tree) :
      //
      // We will call the default constructor of the event variables   
      //

      // tree branch names
      tNJEt_("nljet"),
      tAJEt_("aljet"),
      tNPtnEt_("nlpet"),
      tAPtnEt_("alpet"),
      tDPhi_("jdphi"),
      tNJEta_("nljeta"),
      tAJEta_("aljeta"),
      tPPt_("ppt"),
      tNJPDR_("pndr"),
      tAJPDR_("padr")
   {
      tree_ = tree;
   }

   // set brances
   void TreeDiJetEventData::SetBranches()
   {
      // --event level--
      tree_->Branch("run", &(this->run_), "run/l");
      tree_->Branch("event", &(this->event_), "event/l");
      tree_->Branch("mass", &(this->mass_), "mass/D");
      tree_->Branch("cmeta", &(this->cmeta_), "cmeta/D");

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
      tree_->Branch("nljnp", &(this->nljnp_), "nljnp/l");

      tree_->Branch("aljet", &(this->aljet_), "aljet/D");
      tree_->Branch("aljetsm", &(this->aljetsm_), "aljetsm/D");
      tree_->Branch("aljeta", &(this->aljeta_), "aljeta/D");
      tree_->Branch("aljphi", &(this->aljphi_), "aljphi/D");
      tree_->Branch("aljnp", &(this->aljnp_), "aljnp/l");

      tree_->Branch("jdphi", &(this->jdphi_), "jdphi/D");

      // -- particle info --
      tree_->Branch("evtnp",&(this->evtnp_),"evtnp/i");
      tree_->Branch("ppid",this->ppid_,"ppid[evtnp]/I");
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
   }
}

#endif // TREEDIJETEVENTDATA_H
