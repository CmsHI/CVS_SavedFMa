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
      ~HiDiJetAnalysis();

      // settings
      void SetVerbosity(int v) { verbosity_ = v;}
      void SetAnaOnlyLeadDijet(bool choice) {anaOnlyLeadDijet_ = choice;}
      void SetAnaFF(bool choice) {anaFF_ = choice;}
      // analysis helper methods
      void CalcJetVars (const DiJet & dijet);
      void CalcFragVars(const DiJet & dijet, const AnaInputCollection & fragcands);
      bool isFrag(const DiJet & dijet, const AnaInputItem & track);
      // analysis main methods
      void Fill(const AnaDiJetCollection & dijets, const AnaInputCollection & tracks);

      // public data members
      TTree * tree_;

    private:
      TreeDiJetEventData jd_;

      // ana configuration
      bool anaOnlyLeadDijet_;
      bool anaFF_;
      float fragDRMax_;

      // settings
      int verbosity_;
  };
} // jetana namespace

#endif // HiJetAna_HiDiJetAnalysis_h
