#ifndef HiJetAna_HiDiJetAnalysis_h
#define HiJetAna_HiDiJetAnalysis_h

// std
#include<vector>
// root
#include "TTree.h"
// pat
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/HeavyIonEvent/interface/HeavyIon.h"
// ana
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"
// ana tree
#include "PhysicsTools/PatExamples/macros/DiJetTreeAna/TreeDiJetEventData.h"

namespace jetana
{
  // class definition
  class HiDiJetAnalysis
  {
    public:
      // constructor
      HiDiJetAnalysis();
      ~HiDiJetAnalysis();

      // settings
      void SetVerbosity(int v) { verbosity_ = v;}
      void SetAnaOnlyLeadDijet(bool choice) {anaOnlyLeadDijet_ = choice;}
      void SetAnaFF(bool choice) {anaFF_ = choice;}
      void SetAnaCfg(HiDiJetAnaConfig * cfg) { anacfg_=cfg; }
      // analysis helper methods
      void StoreHiEventVars(fwlite::Handle<pat::HeavyIon> * hievt);
      void CalcJetVars (const DiJet & dijet);
      void CalcFragVars(const DiJet & dijet, AnaInputCollection & fragcands);
      bool isFrag(const DiJet & dijet, AnaInputItem * track);
      // analysis main methods
      void Fill(const AnaDiJetCollection & dijets, AnaInputCollection & tracks, fwlite::Handle<pat::HeavyIon> * hievent = NULL);

      // public data members
      TTree * tree_;

    private:
      TreeDiJetEventData jd_;

      // ana configuration
      bool anaOnlyLeadDijet_;
      bool anaFF_;
      float fragDRMax_;
      HiDiJetAnaConfig * anacfg_;

      // settings
      int verbosity_;
  };
} // jetana namespace

#endif // HiJetAna_HiDiJetAnalysis_h
