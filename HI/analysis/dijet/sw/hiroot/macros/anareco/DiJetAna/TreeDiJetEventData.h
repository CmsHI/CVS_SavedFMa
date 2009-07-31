#ifndef TREEDIJETEVENTDATA_H
#define TREEDIJETEVENTDATA_H
#include <vector>
#include "TTree.h"

namespace DiJetAna
{
   //
   // DiJet ana Event Data Tree definition
   //
   class TreeDiJetEventData
   {
      public:
	 // --- DiJet Event variables ---
	 //  --event level--
	 ULong_t		    run;
	 ULong_t		    event;
	 Double_t		    mass;
	 Double_t		    cmeta;

	 //  -- parton info --
	 Double_t		    nlpid;
	 Double_t		    nlpstat;
	 Double_t		    nlpet;
	 Double_t		    nlpetsm;
	 Double_t		    nlpeta;
	 Double_t		    nlpphi;

	 Double_t		    alpid;
	 Double_t		    alpstat;
	 Double_t		    alpet;
	 Double_t		    alpetsm;
	 Double_t		    alpeta;
	 Double_t		    alpphi;

	 Double_t		    pdphi;

	 //  -- jet info --
	 Double_t		    nljet;
	 Double_t		    nljetsm;
	 Double_t		    nljeta;
	 Double_t		    nljphi;
	 ULong_t		    nljnp;

	 Double_t		    aljet;
	 Double_t		    aljetsm;
	 Double_t		    aljeta;
	 Double_t		    aljphi;
	 ULong_t		    aljnp;

	 Double_t		    jdphi;

	 // -- particle info --
	 std::vector<Double_t>     	    ppid;
	 std::vector<Double_t>     	    ppt;
	 std::vector<Double_t>     	    peta;
	 std::vector<Double_t>     	    pphi;

	 std::vector<Double_t>     	    pndphi;
	 std::vector<Double_t>     	    pndeta;
	 std::vector<Double_t>     	    pndr;
	 std::vector<Double_t>     	    pndrbg;

	 std::vector<Double_t>     	    padphi;
	 std::vector<Double_t>     	    padeta;
	 std::vector<Double_t>     	    padr;
	 std::vector<Double_t>     	    padrbg;

	 std::vector<Double_t>     	    zn;
	 std::vector<Double_t>     	    za;
	 

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

	 TreeDiJetEventData();
   };

   // constructor
   TreeDiJetEventData::TreeDiJetEventData() :
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
   }
}

#endif // TREEDIJETEVENTDATA_H
