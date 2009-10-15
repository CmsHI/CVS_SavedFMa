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
  //  Run the algorithm
  //  ------------------
  void HiDiJetAlgorithm::group(InputCollection& input, OutputCollection* output)
  {
    if (!output) return;
  }
} //jetana
