//
// This is a class to represent dijets
//
#ifndef DIJETS_H
#define DIJETS_H

#include <iostream>
#include "TString.h"
#include "TTree.h"
#include "AnaCuts.h"

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

	 // --- Accessor Functions ---
	 int GetNNearJets();
	 int GetNAwayJets();
	 TString GetGenTag() const { return genTag_; }
	 TString GetCutTag() const { return cut_.GetCutTag(); }
	 TTree* GetJetTree() const { return jetTree_; }
	 TTree* GetParticleTree() const { return particleTree_; }
	 // get cut
	 AnaCuts GetCut() const { return cut_; }

	 // --- Mutator Functions ---
	 void SetGenTag(char* genTag) { genTag_ = TString(genTag); }
	 void SetCutTag(char* cutTag) { cut_.SetCutTag(cutTag); }
	 void SetVerbosity(Int_t v) { verbosity_ = v; }
	 // jet level cuts
	 void SetNearJetEtMin(Double_t njmin) { cut_.SetNearJetEtMin(njmin); }
	 void SetNearJetEtMax(Double_t njmax) { cut_.SetNearJetEtMax(njmax); }
	 void SetAwayJetEtMin(Double_t ajmin) { cut_.SetAwayJetEtMin(ajmin); }
	 void SetDPhiMin(Double_t dphimin) { cut_.SetDPhiMin(dphimin); }
	 void SetJetEtaMax(Double_t jetamax) { cut_.SetJetEtaMax(jetamax); }
	 // particle level cuts
	 void SetPartlPtMin(Double_t pptmin) { cut_.SetPartlPtMin(pptmin); }
	 void SetJetPartlDRMax(Double_t jpdrmax) { cut_.SetJetPartlDRMax(jpdrmax); }
	 // create tree cuts
	 void CreateCuts() { cut_.CreateJetCut(); cut_.CreateJetParticlesCut(); }


	 // Freind Functions
	 friend ostream& operator <<(ostream& outs, const DiJets& dj);
	 

      private:
	 TString	      genTag_;
	 AnaCuts              cut_;
	 TTree*		      jetTree_;
	 TTree*		      particleTree_;
	 Int_t                verbosity_;
   };

   // Dijets class implementation
   // ===Constructors===
   DiJets::DiJets() :
      verbosity_(1),
      genTag_("none")
   {
      jetTree_ = NULL;
      particleTree_ = NULL;
   }

   DiJets::DiJets(char*  genTag, char* cutTag, TTree* jetTree, TTree* particleTree) :
      verbosity_(1),
      genTag_(genTag),
      cut_(cutTag)
   {
      jetTree_ = jetTree;
      particleTree_ = particleTree;
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const DiJets& dj)
   {
      os << "generator: " << dj.genTag_;
      if (dj.jetTree_) os << "  jet tree: " << dj.jetTree_->GetName();
      if (dj.particleTree_) os << "  particle tree: " << dj.particleTree_->GetName();
      if ( dj.verbosity_ >= 1)
	 os << endl << "cut: " << dj.cut_.GetCutTag() << endl;
      if ( dj.verbosity_ >= 2)
	 os << endl << dj.cut_ << endl;
      return os;
   }

}//DiJetAna

#endif //DIJETS_H
