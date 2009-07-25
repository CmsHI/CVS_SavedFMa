//
// This is a class to represent analysis cuts
//
#ifndef ANACUTS_H
#define ANACUTS_H

#include <iostream>
#include "TString.h"
#include "TreeData.h"

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
	 Double_t GetNearJetEtMin() const { return nearJetEtMin_; }
	 Double_t GetNearJetEtMax() const { return nearJetEtMax_; }
	 Double_t GetAwayJetEtMin() const { return awayJetEtMin_; }
	 Double_t GetDPhiMin() const { return dPhiMin_; }
	 // particle level cuts
	 Double_t GetPPtMin() const { return partlPtMin_; }
	 // final tree cut
	 TString GetDiJetCut() const { return dijetCut_; }
	 TString GetNJetPartlCut() const { return nearJetPartlsCut_; }
	 TString GetAJetPartlCut() const { return awayJetPartlsCut_; }

	 // --- Mutator Functions ---
	 void SetCutTag(char* cutTag) { cutTag_ = TString(cutTag); }
	 void SetDefaults();
	 // jet level cuts
	 void SetNearJetEtMin(Double_t njmin) { nearJetEtMin_ = njmin; }
	 void SetNearJetEtMax(Double_t njmax) { nearJetEtMax_ = njmax; }
	 void SetAwayJetEtMin(Double_t ajmin) { awayJetEtMin_ = ajmin; }
	 void SetDPhiMin(Double_t dphimin) { dPhiMin_ = dphimin; }
	 void SetJetEtaMax(Double_t jetamax) { jetEtaMax_ = jetamax; }
	 // particle level cuts
	 void SetPartlPtMin(Double_t pptmin) { partlPtMin_ = pptmin; }
	 void SetJetPartlDRMax(Double_t jpdrmax) { jetPartlDRMax_ = jpdrmax; }
	 // final tree cut
	 TString AndCut(TString var, Double_t val, TString opt);
	 void CreateJetCut();
	 void CreateJetParticlesCut();

	 // --- Friend Functions ---
	 friend ostream& operator <<(ostream& outs, const AnaCuts& ct);
      protected:
	 TString cutTag_;
	 // jet level cuts
	 Double_t nearJetEtMin_;
	 Double_t nearJetEtMax_;
	 Double_t awayJetEtMin_;
	 Double_t jetEtaMax_;
	 Double_t dPhiMin_;
	 // particle level cuts
	 Double_t partlPtMin_;
	 Double_t jetPartlDRMax_;
	 TreeData td_;

	 // Final jet cut string for tree
	 TString dijetCut_;
	 TString partlsCut_;
	 TString nearJetPartlsCut_;
	 TString awayJetPartlsCut_;
   };

   //
   // AnaCuts class implementation
   //

   // Default values
   void AnaCuts::SetDefaults()
   {
      partlPtMin_ = 0.5;
      jetEtaMax_ = 2.0;
      // tree cuts
      dijetCut_ = "";
      partlsCut_ = "";
      nearJetPartlsCut_ = "";
      awayJetPartlsCut_ = "";
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
   TString AnaCuts::AndCut(TString var, Double_t val, TString opt)
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

   void AnaCuts::CreateJetCut()
   {
      dijetCut_ += AndCut(td_.tNJEta_,jetEtaMax_,"first max abs");
      dijetCut_ += AndCut(td_.tAJEta_,jetEtaMax_,"max abs");
      dijetCut_ += AndCut(td_.tNJEt_,nearJetEtMin_,"min");
      dijetCut_ += AndCut(td_.tNJEt_,nearJetEtMax_,"max");
      dijetCut_ += AndCut(td_.tAJEt_,awayJetEtMin_,"min");
      dijetCut_ += AndCut(td_.tDPhi_,dPhiMin_,"min");
   }

   void AnaCuts::CreateJetParticlesCut()
   {
      // first all particles are given a pt cut
      partlsCut_ += AndCut(td_.tPPt_,partlPtMin_,"first min");

      // now dR cut from n,a jet
      Double_t dR2Max=jetPartlDRMax_*jetPartlDRMax_;
      // from near jet
      nearJetPartlsCut_ = dijetCut_ + " && " + partlsCut_;
      nearJetPartlsCut_ += AndCut(td_.tNJPDR2_,dR2Max,"max");
      // from away jet
      awayJetPartlsCut_ = dijetCut_ + " && " + partlsCut_;
      awayJetPartlsCut_ += AndCut(td_.tAJPDR2_,dR2Max,"max");
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
      os << "Tree branches' names: " << endl;
      os << "  jet:       " << ct.td_.tNJEt_ << " " << ct.td_.tAJEt_ << " " << ct.td_.tDPhi_ << " " << ct.td_.tNJEta_ << " " << ct.td_.tAJEta_ 
	   << " " << ct.td_.tNJPDR2_ << " " << ct.td_.tAJPDR2_ << endl;
      os << "  particles: " << ct.td_.tPPt_ << endl;
      os << "tree cuts: " << endl;
      os << "  jet:                " <<  ct.dijetCut_ << endl;
      os << "  particles:          " <<  ct.partlsCut_ << endl;
      os << "  near_jet_particles: " <<  ct.nearJetPartlsCut_ << endl;
      os << "  away_jet_particles: " <<  ct.awayJetPartlsCut_ << endl;
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

