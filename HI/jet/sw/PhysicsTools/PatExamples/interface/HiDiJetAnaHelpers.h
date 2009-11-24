#ifndef HiJetAna_HiDiJetAnaHelpers_h
#define HiJetAna_HiDiJetAnaHelpers_h

//stl
#include <map>

namespace jetana
{
  //  jet analysis interface types
  typedef LorentzVector InputItem;
  typedef std::list<InputItem> InputCollection;
  typedef DiJet OutputItem;
  typedef std::vector<OutputItem> OutputCollection;

  // jet ana setup helper data
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
      trackPtMin_(1.),
      // event gen: pp(0), HI(1)
      genType_(1)
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

    // gen type
    int genType_;

    // helpers
    std::map<InputItem*,int> partonSEMap_;
    std::map<InputItem*,int> particleSEMap_;
  };
}
#endif //
