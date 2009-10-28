#ifndef HiJetAna_HiDiJetAnalysis_h
#define HiJetAna_HiDiJetAnalysis_h

// root
#include "TTree.h"

// ana tree
#include "PhysicsTools/PatExamples/macros/DiJetTreeAna/TreeDiJetEventData.h"

namespace jetana
{
  class HiDiJetAnalysis
  {
    public:
      // constructor
      HiDiJetAnalysis();

    private:
      TTree * tree_;
      TreeDiJetEventData jd_;
  };
} // jetana namespace

#endif // HiJetAna_HiDiJetAnalysis_h
