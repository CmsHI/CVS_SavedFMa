#ifndef HiJetAna_HiDiJetAlgorithm_h
#define HiJetAna_HiDiJetAlgorithm_h

//
// Based on http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoJets/JetAlgorithms/interface/CMSIterativeConeAlgorithm.h?hideattic=0&view=markup
//

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgoHelper.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAnaHelpers.h"

namespace jetana
{
  // stl style correlation algorithm
  template <class ForwardIterator, class Correlation, class Cand>
    ForwardIterator max_correlated_element ( ForwardIterator first, ForwardIterator last,
	Correlation corr, const Cand & c0)
  {
    ForwardIterator largest = first;
    if (first==last) return largest;
    double max = corr(*first,c0);
    while (++first!=last) {
      double cur=corr(*first,c0);
      if (max < cur)
	largest = first;
    }
    return largest;
  }


  //  jet analysis interface types
  typedef LorentzVector InputItem;
  typedef std::list<InputItem> InputCollection;
  typedef DiJet OutputItem;
  typedef std::vector<OutputItem> OutputCollection;

  //  main class
  class HiDiJetAlgorithm
  {
    public:
      // constructor
      HiDiJetAlgorithm(HiDiJetAnaConfig * anacfg);

      // helpers
      bool PassNearJetCriterion(const InputItem & cand) const;
      bool PassAwayJetCriterion(const InputItem & cand) const;
      InputCollection::iterator FindPair(const InputItem & near, InputCollection & others);
      /// Find di Jets from the collection of input Candidates.
      int Group(InputCollection& input, OutputCollection* output);

      // checks
      void SetVerbosity(int level) {verbosity_ = level;}

    private:
      HiDiJetAnaConfig * anacfg_;
      // checks
      unsigned int evtJetMul_;
      int ldjAwayJetMul_;
      bool foundDijet_;
      int verbosity_;
  };
} //jetnana

#endif //HiJetAna_HiDiJetAlgorithm_h
