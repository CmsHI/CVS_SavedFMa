//
// This is a class to represent analysis cuts
//
#ifndef ANACUTS_H
#define ANACUTS_H

#include <iostream>
#include "TString.h"

namespace DiJetAna
{
   //
   // AnaCuts class definition
   //
   class AnaCuts
   {
      public:
	 AnaCuts();
	 AnaCuts(char* cutTag);

	 // Accessor Functions
	 TString GetCutTag() const { return cutTag_; }
	 // jet level cuts
	 Float_t GetNearJetEtMin() const { return nearJetEtMin_; }
	 Float_t GetNearJetEtMax() const { return nearJetEtMax_; }
	 Float_t GetAwayJetEtMin() const { return awayJetEtMin_; }
	 Float_t GetDPhiMin() const { return dPhiMin_; }
	 // particle level cuts
	 Float_t GetPPtMin() const { return partlPtMin_; }

	 // Mutator Functions
	 void SetCutTag(char* cutTag) { cutTag_ = TString(cutTag); }
	 void SetDefaults();
	 // jet level cuts
	 void SetNearJetEtMin(Float_t njmin) { nearJetEtMin_ = njmin; }
	 void SetNearJetEtMax(Float_t njmax) { nearJetEtMax_ = njmax; }
	 void SetAwayJetEtMin(Float_t ajmin) { awayJetEtMin_ = ajmin; }
	 void SetDPhiMin(Float_t dphimin) { dPhiMin_ = dphimin; }
	 // particle level cuts
	 void SetPPtMin(Float_t pptmin) { partlPtMin_ = pptmin; }

	 // Friend Functions
	 friend ostream& operator <<(ostream& outs, const AnaCuts& ct);
      private:
	 TString cutTag_;
	 // jet level cuts
	 Float_t nearJetEtMin_;
	 Float_t nearJetEtMax_;
	 Float_t awayJetEtMin_;
	 Float_t jetEtaMax_;
	 Float_t dPhiMin_;
	 // particle level cuts
	 Float_t partlPtMin_;

	 // tree branch names
	 // jet level cuts
	 TString tNJEt_;
	 TString tAJEt_;
	 TString tDPhi_;
	 TString tNJEta_;
	 TString tAJEta_;
	 // particle level cuts
	 TString tPPt_;
   };

   //
   // AnaCuts class implementation
   //

   // Default values
   void AnaCuts::SetDefaults()
   {
      partlPtMin_ = 0.5;
      // tree branch names
      tNJEt_ = "nljet";
      tAJEt_ = "aljet";
      tDPhi_ = "jdphi";
      tNJEta_ = "nljeta";
      tAJEta_ = "aljeta";
   }
   // Constructor
   AnaCuts::AnaCuts() :
      // cut
      cutTag_("vdefault"),
      nearJetEtMin_(0),
      nearJetEtMax_(10000),
      awayJetEtMin_(0),
      dPhiMin_(0)
   {
      SetDefaults();
   }
   AnaCuts::AnaCuts(char* cutTag) :
      // cut
      cutTag_(cutTag),
      nearJetEtMin_(-1),
      nearJetEtMax_(-1),
      awayJetEtMin_(-1),
      dPhiMin_(-1)
   {
      SetDefaults();
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const AnaCuts& ct)
   {
      cout << "cut: " << ct.cutTag_ << endl;
      if ( ct.nearJetEtMin_ >= 0) {
	 cout << "njmin: " << ct.nearJetEtMin_ << ", njmax: " << ct.nearJetEtMax_
	    << ", ajmin: " << ct.awayJetEtMin_ << ", dphimin: " << ct.dPhiMin_
	    << ", pptmin: " << ct.partlPtMin_ <<endl;
      }
      cout << "tree branches: " << endl
	 << ct.tNJEt_ << " " << ct.tAJEt_ << " " << ct.tDPhi_ << " " << ct.tNJEta_ << " " << ct.tAJEta_;
      return os;
   }

   //
   // Class to represent dijet cuts
   //

   //
   // Class to represent particle cuts
   //

} //DiJetAna namespace


#endif //ANACUTS_H

