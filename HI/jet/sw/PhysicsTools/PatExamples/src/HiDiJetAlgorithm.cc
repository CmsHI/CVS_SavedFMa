#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"

using namespace std;
using namespace reco;

//  Binary Predicates
bool lessPt(const LorentzVector & lv1, const LorentzVector & lv2)
{
  return (lv1.pt() < lv2.pt());
}

//  Run the algorithm
//  ------------------
void HiDiJetAlgorithm::group(InputCollection& input, vector<DiJet>* output) const
{
  if (!output) return;
}
