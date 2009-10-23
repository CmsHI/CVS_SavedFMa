#ifndef HiJetAna_HiDiJetAlgoHelper_h
#define HiJetAna_HiDiJetAlgoHelper_h

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

namespace jetana
{
  typedef math::XYZTLorentzVector LorentzVector;

  // Binary Predicates
  // - declare as extern so can compile during linking
  //   * cf http://ubuntuforums.org/archive/index.php/t-317212.html 
  extern bool lessPt(const LorentzVector & lv1, const LorentzVector & lv2);
  extern double absDPhi(const LorentzVector & lv1, const LorentzVector & lv2);
}

#endif //HiJetAna_HiDiJetAlgoHelper_h
