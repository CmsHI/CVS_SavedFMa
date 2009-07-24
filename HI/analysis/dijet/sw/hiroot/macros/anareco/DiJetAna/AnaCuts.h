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

	 // Mutator Functions
	 void SetCutTag(char* cutTag) { cutTag_ = TString(cutTag); }

	 // Friend Functions
	 friend ostream& operator <<(ostream& outs, const AnaCuts& ct);
      private:
	 TString cutTag_;
   };

   //
   // AnaCuts class implementation
   //

   // Constructor
   AnaCuts::AnaCuts() :
      cutTag_("vdefault")
   {
      // empty
   }
   AnaCuts::AnaCuts(char* cutTag) :
      cutTag_(cutTag)
   {
      // empty
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const AnaCuts& ct)
   {
      cout << "cut: " << ct.cutTag_;
      return os;
   }

   //
   // Class to represent dijet cuts
   //
//	 Float_t nearJetEtMin_;
//	 Float_t nearJetEtMax_;
//	 Float_t awayJetEtMin_;
//	 Float_t dijetDPhiMin_;

   //
   // Class to represent particle cuts
   //

} //DiJetAna namespace


#endif //ANACUTS_H

