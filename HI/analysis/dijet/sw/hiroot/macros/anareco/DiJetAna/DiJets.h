//
// This is a class to represent dijets
//
#ifndef DIJETS_H
#define DIJETS_H

#include <iostream>
#include "TString.h"
#include "TTree.h"

namespace DiJetAna {
   // Some constants
   const char * drsgFF="hist";
   const char * drdbFF="hist same";
   const char * drsgFFE="E1 ";
   const char * drdbFFE="E1 same";

   Int_t HJETETBINS = 100;
   Double_t HJETETMIN = 0;
   Double_t HJETETMAX = 200;

   // Dijets class definition
   class DiJets
   {
      public:
	 DiJets();
	 DiJets(char*  genTag, char* cutTag, TTree* jetTree, TTree* particleTree); 

	 // Accessor Functions
	 int GetNNearJets();
	 int GetNAwayJets();
	 TString GetGenTag() const { return genTag_; }
	 TString GetCutTag() const { return cutTag_; }
	 TTree* GetJetTree() const { return jetTree_; }
	 TTree* GetParticleTree() const { return particleTree_; }

	 // Mutator Functions
	 void SetGenTag(char* genTag) { genTag_ = TString(genTag); }
	 void SetCutTag(char* cutTag) { cutTag_ = TString(cutTag); }

	 // Freind Functions
	 friend ostream& operator <<(ostream& outs, const DiJets& dj);
	 

      private:
	 TString	      genTag_;
	 TString	      cutTag_;
	 TTree*		      jetTree_;
	 TTree*		      particleTree_;
   };

   // Dijets class implementation
   // ===Default Constructor===
   DiJets::DiJets() :
      genTag_("none"),
      cutTag_("v0")
   {
      jetTree_ = NULL;
      particleTree_ = NULL;
   }

   DiJets::DiJets(char*  genTag, char* cutTag, TTree* jetTree, TTree* particleTree) :
      genTag_(genTag),
      cutTag_(cutTag)
   {
      jetTree_ = jetTree;
      particleTree_ = particleTree;
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const DiJets& dj)
   {
      cout << "generator: " << dj.genTag_ << "  cut: " << dj.cutTag_;
      if (dj.jetTree_) cout << "  jet tree: " << dj.jetTree_->GetName();
      if (dj.particleTree_) cout << "  particle tree: " << dj.particleTree_->GetName();
      return os;
   }

}//DiJetAna

#endif //DIJETS_H
