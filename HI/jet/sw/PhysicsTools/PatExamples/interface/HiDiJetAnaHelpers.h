#ifndef HiJetAna_HiDiJetAnaHelpers_h
#define HiJetAna_HiDiJetAnaHelpers_h

namespace jetana
{
  struct HiDiJetAnaConfig
  {
    HiDiJetAnaConfig() :
      // jets
      jetEtaMax_(2.),
      jetEtMin_(25),
      JECFactor_(1.6),
      awayEtFrac_(0.8),
      djDPhiMin_(3.14-0.5),
      doJEC_(true),
      // tracks
      trackEtaMax_(2.5),
      trackPtMin_(1.)
    {
      jetEtUMin_=jetEtMin_/JECFactor_;
    }

    // jets
    float jetEtaMax_;
    float jetEtMin_;
    float JECFactor_;
    float jetEtUMin_;
    float awayEtFrac_;
    float djDPhiMin_;
    bool doJEC_;

    // tracks
    float trackEtaMax_;
    float trackPtMin_;
  };
}
#endif //
