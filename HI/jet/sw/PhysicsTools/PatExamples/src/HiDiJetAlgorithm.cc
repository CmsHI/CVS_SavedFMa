#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"

// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace std;
using namespace reco;

namespace jetana
{
  // constructors
  HiDiJetAlgorithm::HiDiJetAlgorithm():
    nearThreshold_(20),
    awayThreshold_(20),
    dPhiMin_(0.5),
    verbosity_(0)
  {
  } 
  HiDiJetAlgorithm::HiDiJetAlgorithm(double nearThresh, double awayThresh, double dPhiMin):
    nearThreshold_(nearThresh),
    awayThreshold_(awayThresh),
    dPhiMin_(dPhiMin),
    verbosity_(0)
  { /* empty */ }

  // helpers
  InputCollection::const_iterator HiDiJetAlgorithm::FindPair(const InputItem & near, const InputCollection & others) const
  { 
    if (verbosity_>1) {
      cout << "algo input near: " << near << endl;
      cout << "algo input others: " << endl;
      mystd::print_elements(others);
    }

    // Note:
    // - const_iterator c/b function is const and stl::...->begin() uses 
    //   the overloaded return of type const_iterator
    InputCollection::const_iterator it_away=NULL;
    if (others.size() == 0) return it_away;

    double max = 0;
    for (InputCollection::const_iterator icand=others.begin(); icand!=others.end(); ++icand) {
      if (verbosity_>1)
	cout << "processing: " << *icand << endl;
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
  void HiDiJetAlgorithm::Group(InputCollection& input, OutputCollection* output)
  {
    if (!output) return;
  }
} //jetana
