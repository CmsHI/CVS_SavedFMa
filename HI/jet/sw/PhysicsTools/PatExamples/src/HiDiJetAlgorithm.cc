#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"

using namespace std;
using namespace reco;

namespace jetana
{
  // constructor 
  HiDiJetAlgorithm::HiDiJetAlgorithm(double nearThresh, double awayThresh, double dPhiMin):
    nearThreshold_(nearThresh),
    awayThreshold_(awayThresh),
    dPhiMin_(dPhiMin)
  { /* empty */ }

  // helpers
  HiDiJetAlgorithm::InputCollection::const_iterator HiDiJetAlgorithm::FindPair(const InputItem & near, const InputCollection & others) const
  { 
    // Note:
    // - const_iterator c/b function is const and stl::...->begin() uses 
    //   the overloaded return of type const_iterator
    InputCollection::const_iterator it_away=NULL;
    if (others.size() == 0) return it_away;

    double max = 0;
    for (InputCollection::const_iterator icand=others.begin(); icand!=others.end(); ++icand) {
      if (!PassAwayJetCriterion(*icand))
	continue;
      double dphi=absDPhi(near,*icand);
      if (dphi>dPhiMin_ && max<dphi)
	it_away = icand;
    }

    return it_away;
  }
  //  Run the algorithm
  //  ------------------
  void HiDiJetAlgorithm::group(InputCollection& input, OutputCollection* output)
  {
    if (!output) return;
  }
} //jetana
