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

	 // ---Accessor Functions---
	 TString GetCutTag() const { return cutTag_; }
	 // jet level cuts
	 Float_t GetNearJetEtMin() const { return nearJetEtMin_; }
	 Float_t GetNearJetEtMax() const { return nearJetEtMax_; }
	 Float_t GetAwayJetEtMin() const { return awayJetEtMin_; }
	 Float_t GetDPhiMin() const { return dPhiMin_; }
	 // particle level cuts
	 Float_t GetPPtMin() const { return partlPtMin_; }
	 // final tree cut
	 TString GetJetTreeCut() const { return jetTreeCut_; }

	 // --- Mutator Functions ---
	 void SetCutTag(char* cutTag) { cutTag_ = TString(cutTag); }
	 void SetDefaults();
	 // jet level cuts
	 void SetNearJetEtMin(Float_t njmin) { nearJetEtMin_ = njmin; }
	 void SetNearJetEtMax(Float_t njmax) { nearJetEtMax_ = njmax; }
	 void SetAwayJetEtMin(Float_t ajmin) { awayJetEtMin_ = ajmin; }
	 void SetDPhiMin(Float_t dphimin) { dPhiMin_ = dphimin; }
	 void SetJetEtaMax(Float_t jetamax) { jetEtaMax_ = jetamax; }
	 // particle level cuts
	 void SetPPtMin(Float_t pptmin) { partlPtMin_ = pptmin; }
	 // final tree cut
	 TString AndCut(TString var, Float_t val, TString opt);
	 void CreateJetTreeCut();
	 void CreateJetParticlesTreeCut();

	 // --- Friend Functions ---
	 friend ostream& operator <<(ostream& outs, const AnaCuts& ct);
      protected:
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
	 // jet level
	 TString tNJEt_;
	 TString tAJEt_;
	 TString tDPhi_;
	 TString tNJEta_;
	 TString tAJEta_;
	 // particle level
	 TString tPPt_;

	 // Final jet cut string for tree
	 TString jetTreeCut_;
	 TString partlsTreeCut_;
	 TString jetPartlsTreeCut_;
   };

   //
   // AnaCuts class implementation
   //

   // Default values
   void AnaCuts::SetDefaults()
   {
      partlPtMin_ = 0.5;
      jetEtaMax_ = 2.0;
      // tree branch names
      tNJEt_ = "nljet";
      tAJEt_ = "aljet";
      tDPhi_ = "jdphi";
      tNJEta_ = "nljeta";
      tAJEta_ = "aljeta";
      tPPt_ = "ppt";
      // tree cuts
      jetTreeCut_ = "";
      partlsTreeCut_ = "";
      jetPartlsTreeCut_ = "";
   }
   // === Constructors ===
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
   
   // === String for Tree Cut ===
   TString AnaCuts::AndCut(TString var, Float_t val, TString opt)
   {
      TString result;
      if (!opt.Contains("first"))
	 result += " && ";
      if (opt.Contains("abs"))
	 var = TString("abs(") + var + ")";

      if (opt.Contains("min"))
	 result += (var + ">" + Form("%.1f",val));
      if (opt.Contains("max"))
	 result += (var + "<" + Form("%.1f",val));

      return result;
   }

   void AnaCuts::CreateJetTreeCut()
   {
      jetTreeCut_ += AndCut(tNJEta_,jetEtaMax_,"first max abs");
      jetTreeCut_ += AndCut(tAJEta_,jetEtaMax_,"max abs");
      jetTreeCut_ += AndCut(tNJEt_,nearJetEtMin_,"min");
      jetTreeCut_ += AndCut(tNJEt_,nearJetEtMax_,"max");
      jetTreeCut_ += AndCut(tAJEt_,awayJetEtMin_,"min");
      jetTreeCut_ += AndCut(tDPhi_,dPhiMin_,"min");
   }

   void AnaCuts::CreateJetParticlesTreeCut()
   {
      partlsTreeCut_ += AndCut(tPPt_,partlPtMin_,"first min");
      jetPartlsTreeCut_ = jetTreeCut_ + " && " + partlsTreeCut_;
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const AnaCuts& ct)
   {
      cout << "cut: " << ct.cutTag_ << endl;
      cout << "Default cuts:" << endl;
      cout << "  jet_eta_max: " << ct.jetEtaMax_ 
	   << ", pptmin: " << ct.partlPtMin_ <<endl;
      cout << "Aet ana cuts:" << endl;
      if ( ct.nearJetEtMin_ >= 0) {
	 cout << "  njmin: " << ct.nearJetEtMin_ << ", njmax: " << ct.nearJetEtMax_
	    << ", ajmin: " << ct.awayJetEtMin_ << ", dphimin: " << ct.dPhiMin_ << endl;
      }
      cout << "Tree branches: " << endl;
      cout << "  jet:       " << ct.tNJEt_ << " " << ct.tAJEt_ << " " << ct.tDPhi_ << " " << ct.tNJEta_ << " " << ct.tAJEta_ << endl;
      cout << "  particles: " << ct.tPPt_ << endl;
      cout << "tree cuts: " << endl;
      cout << "  jet:           " <<  ct.jetTreeCut_ << endl;
      cout << "  particles:     " <<  ct.partlsTreeCut_ << endl;
      cout << "  jet_particles: " <<  ct.jetPartlsTreeCut_ << endl;
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

