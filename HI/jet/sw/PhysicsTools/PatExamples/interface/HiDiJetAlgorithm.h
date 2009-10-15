#ifndef HiJetAna_HiDiJetAlgorithm_h
#define HiJetAna_HiDiJetAlgorithm_h

//
// Based on http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoJets/JetAlgorithms/interface/CMSIterativeConeAlgorithm.h?hideattic=0&view=markup
//

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"

namespace jetana
{
  //  Binary Predicates
  bool lessPt(const LorentzVector & lv1, const LorentzVector & lv2) {
    return (lv1.pt() < lv2.pt());
  }
  double absDPhi(const LorentzVector & lv1, const LorentzVector & lv2) {
    return (fabs(deltaPhi(lv1.phi(),lv2.phi())));
  }

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


  //  main class
  class HiDiJetAlgorithm
  {
    public:
      typedef std::list<LorentzVector> InputCollection;
      typedef LorentzVector InputItem;

      HiDiJetAlgorithm(double nearThresh, double awayThresh):
	nearThreshold_(nearThresh),
	awayThreshold_(awayThresh)
      { /* empty */ }

      /// Find di Jets from the collection of input Candidates.
      void group(InputCollection& input, std::vector<DiJet>* output) const;

    private:
      double nearThreshold_;
      double awayThreshold_;
  };
} //jetnana

#endif //HiJetAna_HiDiJetAlgorithm_h
