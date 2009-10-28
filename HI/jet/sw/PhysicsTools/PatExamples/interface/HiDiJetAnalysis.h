#ifndef HiJetAna_HiDiJetAnalysis_h
#define HiJetAna_HiDiJetAnalysis_h

// std
#include<vector>
// root
#include "TTree.h"
// dataformats
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"

// ana tree
#include "PhysicsTools/PatExamples/macros/DiJetTreeAna/TreeDiJetEventData.h"

namespace jetana
{
  // analysis formats
  typedef std::vector<DiJet> AnaDiJetCollection;
  typedef LorentzVector AnaInputItem;
  typedef std::vector<AnaInputItem> AnaInputCollection;

  // class definition
  class HiDiJetAnalysis
  {
    public:
      // constructor
      HiDiJetAnalysis();

      // analysis methods
      void CalcJetVars(const AnaDiJetCollection & dijets);
      void CalcTrackVars(const AnaInputCollection & tracks);

    private:
      TTree * tree_;
      TreeDiJetEventData jd_;
  };
} // jetana namespace

#endif // HiJetAna_HiDiJetAnalysis_h
