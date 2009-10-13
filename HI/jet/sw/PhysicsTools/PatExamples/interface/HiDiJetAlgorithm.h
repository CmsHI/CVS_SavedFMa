#ifndef HiJetAna_HiDiJetAlgorithm_h
#define HiJetAna_HiDiJetAlgorithm_h

//
// Based on http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoJets/JetAlgorithms/interface/CMSIterativeConeAlgorithm.h?hideattic=0&view=markup
//

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"

class HiDiJetAlgorithm
{
  public:
    typedef std::list<math::XYZTLorentzVector> InputCollection;
    typedef math::XYZTLorentzVector InputItem;

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

#endif //HiJetAna_HiDiJetAlgorithm_h
