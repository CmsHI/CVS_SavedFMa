//
// This is a class to represent dijets
//
#ifndef DIJETS_H
#define DIJETS_H

#include <iostream>
#include "TString.h"
#include "TTree.h"
#include "AnaCuts.h"
#include "TreeDiJetEventData.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

namespace jetana {
   // Some constants
   const Int_t PHINBIN=50;
   const Float_t PHIMIN=-3.14;
   const Float_t PHIMAX=3.14;
   const Int_t ETANBIN=50;
   const Float_t ETAMIN=-2.8;
   const Float_t ETAMAX=2.8;
   // for jets
   const UInt_t NBIN=100;
   const char * drsgE="E1";
   const char * drdbE="E1 same";
   const char * drsg="hist";
   //const char * drdb="E1 hist same";
   const char * drdb="hist same";
   TString drsg2("colz");
   TString drsg2nc("BOX");
   TString drdb2nc("same");
   const char * drsgP="P";
   const char * drdbP="P same";
   // for FF
   const char * drsgFF="hist";
   const char * drdbFF="hist same";
   const char * drsgFFE="E1 ";
   const char * drdbFFE="E1 same";
   Int_t HJETETBINS = 100;
   Double_t HJETETMIN = 0;
   Double_t HJETETMAX = 200;
   // for HI Event
   Int_t HBBINS = 40;
   Double_t HBMIN = 0;
   Double_t HBMAX = 18;

   // Dijets class definition
   class DiJets
   {
      public:
	 DiJets();
	 DiJets(char*  genTag, char* cutTag, TTree* jetTree); 

	 // --- Accessor Functions ---
	 int GetNNearJets();
	 int GetNAwayJets();
	 TString GetGenTag()                const { return genTag_; }
	 TString GetCutTag()                const { return cut_.GetCutTag(); }
	 TTree* GetJetTree()                const { return jetTree_; }
	 // get cut
	 AnaCuts GetCut()                   const { return cut_; }
	 // dijets related
	 Int_t    GetNumDiJets()            const { return numDijets_; }
	 Double_t GetDiJetsNorm()           const { return 1./(Double_t)numDijets_; }

	 // --- Mutator Functions ---
	 void SetGenTag(char* genTag)             { genTag_ = TString(genTag); }
	 void SetCutTag(char* cutTag)             { cut_.SetCutTag(cutTag); }
	 void SetVerbosity(Int_t v)               { verbosity_ = v; }
	 // jet level cuts
	 void SetNearJetEtMin(Double_t njmin)     { cut_.SetNearJetEtMin(njmin); }
	 void SetNearJetEtMax(Double_t njmax)     { cut_.SetNearJetEtMax(njmax); }
	 void SetAwayJetEtMin(Double_t ajmin)     { cut_.SetAwayJetEtMin(ajmin); }
	 void SetDPhiMin(Double_t dphimin)        { cut_.SetDPhiMin(dphimin); }
	 void SetJetEtaMax(Double_t jetamax)      { cut_.SetJetEtaMax(jetamax); }
	 // particle level cuts
	 void SetPartlPtMin(Double_t pptmin)      { cut_.SetPartlPtMin(pptmin); }
	 void SetJetPartlDRMax(Double_t jpdrmax)  { cut_.SetJetPartlDRMax(jpdrmax); }
	 // create tree cuts
	 void CreateCuts()                        { cut_.CreateJetCut(); cut_.CreateJetParticlesCut(); }
	 
	 // --- dijet calculations ---
	 void CalcNumDiJets();
	 // --- make plots ---
	 void PlotDiJets();


	 // Freind Functions
	 friend ostream& operator <<(ostream& outs, const DiJets& dj);
	 
	 // === public members ===
	 AnaCuts              cut_;

      private:
	 TString	      genTag_;
	 TTree*		      jetTree_;
	 Int_t                verbosity_;
	 // Tree data
	 TreeDiJetEventData td_;
	 // dijets ana info
	 Int_t                numDijets_;
	 Double_t             dijetsNorm_;
   };

   // Dijets class implementation
   // ===Constructors===
   DiJets::DiJets() :
      verbosity_(1),
      genTag_("none")
   {
      jetTree_ = NULL;
   }

   DiJets::DiJets(char*  genTag, char* cutTag, TTree* jetTree) :
      verbosity_(1),
      genTag_(genTag),
      cut_(cutTag)
   {
      jetTree_ = jetTree;
   }

   // === DiJet calculations ===
   void DiJets::CalcNumDiJets()
   {
      Int_t pn = jetTree_->Draw(td_.tNPtnEt_,cut_.GetDiJetCut().Data());
      Int_t jn = jetTree_->Draw(td_.tNJEt_,  cut_.GetDiJetCut().Data());
      if (jn!=pn) {
	 printf("Normalization error: dijetn != partonN.\n");
	 printf("dijetn: %d, partonN: %d\n",jn,pn);
	 exit(1);
      } 
      numDijets_ = jn;
   }

   void DiJets::PlotDiJets()
   {
      //--- Et ---
      //jetTree_->Print();
      drawTree(jetTree_,Form("%s>>hNearLParton%s",td_.tNPtnEt_.Data(),genTag_.Data()),cut_.GetDiJetCut().Data(),drsg, Form("hNearLParton%s",genTag_.Data()),";(near) jet Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX);
      drawTree(jetTree_,Form("%s>>hNearLJet%s",td_.tNJEt_.Data(),genTag_.Data()),     cut_.GetDiJetCut().Data(),drdbE,Form("hNearLJet%s",genTag_.Data()),     ";(near) parton Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX);
      drawTree(jetTree_,Form("%s>>hAwayLParton%s",td_.tAPtnEt_.Data(),genTag_.Data()),cut_.GetDiJetCut().Data(),drsg, Form("hAwayLParton%s",genTag_.Data()),";(away) jet Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX);
      drawTree(jetTree_,Form("%s>>hAwayLJet%s",td_.tAJEt_.Data(),genTag_.Data()),     cut_.GetDiJetCut().Data(),drdbE,Form("hAwayLJet%s",genTag_.Data()),     ";(away) parton Et [GeV];",HJETETBINS,HJETETMIN,HJETETMAX);
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const DiJets& dj)
   {
      using namespace std;
      os << "generator: " << dj.genTag_;
      if (dj.jetTree_) os << "  jet tree: " << dj.jetTree_->GetName();
      if ( dj.verbosity_ >= 1)
	 os << endl << "cut: " << dj.cut_.GetCutTag() << endl;
      if ( dj.verbosity_ >= 2)
	 os << endl << dj.cut_ << endl;
      return os;
   }

}//jetana

#endif //DIJETS_H
