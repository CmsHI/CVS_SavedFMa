#include "PhysicsTools/PatExamples/interface/HiDiJetAlgoHelper.h"

namespace jetana
{
  //  Binary Predicates
  bool lessPt(const LorentzVector & lv1, const LorentzVector & lv2) {
    return (lv1.pt() < lv2.pt());
  }
  double absDPhi(const LorentzVector & lv1, const LorentzVector & lv2) {
    return (fabs(deltaPhi(lv1.phi(),lv2.phi())));
  }
}
