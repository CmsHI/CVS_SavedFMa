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
	 void SetPartlPtMin(Float_t pptmin) { partlPtMin_ = pptmin; }
	 void SetJetPartlDRMax(Float_t jpdrmax) { jetPartlDRMax_ = jpdrmax; }
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
	 Float_t jetPartlDRMax_;

	 // tree branch names
	 // jet level
	 TString tNJEt_;
	 TString tAJEt_;
	 TString tDPhi_;
	 TString tNJEta_;
	 TString tAJEta_;
	 // particle level
	 TString tPPt_;
	 TString tNJPDR2_;
	 TString tAJPDR2_;

	 // Final jet cut string for tree
	 TString jetTreeCut_;
	 TString partlsTreeCut_;
	 TString nearJetPartlsTreeCut_;
	 TString awayJetPartlsTreeCut_;
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
      tNJPDR2_ = "pndr";
      tAJPDR2_ = "padr";
      // tree cuts
      jetTreeCut_ = "";
      partlsTreeCut_ = "";
      nearJetPartlsTreeCut_ = "";
      awayJetPartlsTreeCut_ = "";
   }
   // === Constructors ===
   AnaCuts::AnaCuts() :
      // cut
      cutTag_("vdefault"),
      nearJetEtMin_(0),
      nearJetEtMax_(10000),
      awayJetEtMin_(0),
      dPhiMin_(0),
      jetPartlDRMax_(0.5)
   {
      SetDefaults();
   }
   AnaCuts::AnaCuts(char* cutTag) :
      // cut
      cutTag_(cutTag),
      nearJetEtMin_(-1),
      nearJetEtMax_(-1),
      awayJetEtMin_(-1),
      dPhiMin_(-1),
      jetPartlDRMax_(-1)
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

      TString opr;
      if (opt.Contains("min"))
	 opr=">";
      if (opt.Contains("max"))
	 opr="<";

      //Final result
      result += (var + opr + Form("%.2f",val));
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
      // first all particles are given a pt cut
      partlsTreeCut_ += AndCut(tPPt_,partlPtMin_,"first min");

      // now dR cut from n,a jet
      Float_t dR2Max=jetPartlDRMax_*jetPartlDRMax_;
      // from near jet
      nearJetPartlsTreeCut_ = jetTreeCut_ + " && " + partlsTreeCut_;
      nearJetPartlsTreeCut_ += AndCut(tNJPDR2_,dR2Max,"max");
      // from away jet
      awayJetPartlsTreeCut_ = jetTreeCut_ + " && " + partlsTreeCut_;
      awayJetPartlsTreeCut_ += AndCut(tAJPDR2_,dR2Max,"max");
   }

   // === Friend Functions ===
   ostream& operator <<(ostream& os, const AnaCuts& ct)
   {
      os << "cut: " << ct.cutTag_ << endl;
      os << "Default cuts:" << endl;
      os << "  jet_eta_max: " << ct.jetEtaMax_ 
	   << ", pptmin: " << ct.partlPtMin_ <<endl;
      if ( ct.nearJetEtMin_ >= 0) {
	 os << "Jet ana cuts:" << endl;
	 os << "  njmin: " << ct.nearJetEtMin_ << ", njmax: " << ct.nearJetEtMax_
	    << ", ajmin: " << ct.awayJetEtMin_ << ", dphimin: " << ct.dPhiMin_ << endl;
      }
      if ( ct.jetPartlDRMax_ >= 0) {
	 os << "Jet_Particle cuts:" << endl;
	 os << "  jpdRmax: " << ct.jetPartlDRMax_ << endl;
      }
      os << "Tree branches: " << endl;
      os << "  jet:       " << ct.tNJEt_ << " " << ct.tAJEt_ << " " << ct.tDPhi_ << " " << ct.tNJEta_ << " " << ct.tAJEta_ 
	   << " " << ct.tNJPDR2_ << " " << ct.tAJPDR2_ << endl;
      os << "  particles: " << ct.tPPt_ << endl;
      os << "tree cuts: " << endl;
      os << "  jet:                " <<  ct.jetTreeCut_ << endl;
      os << "  particles:          " <<  ct.partlsTreeCut_ << endl;
      os << "  near_jet_particles: " <<  ct.nearJetPartlsTreeCut_ << endl;
      os << "  away_jet_particles: " <<  ct.awayJetPartlsTreeCut_ << endl;
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

