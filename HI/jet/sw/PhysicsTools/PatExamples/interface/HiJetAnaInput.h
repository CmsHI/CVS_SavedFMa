#ifndef HiJetAna_HiJetAnaInput_h
#define HiJetAna_HiJetAnaInput_h

// pat
#include "DataFormats/FWLite/interface/Handle.h"
// ana
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgoHelper.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"
// root
#include "TFile.h"
#include "TString.h"

namespace jetana
{
  enum JetType { PARTON, GENJET, PATJET };

  class HiJetAnaInput
  {
    public:
      // constructor
      HiJetAnaInput(fwlite::Event * e) {event_=e;}

      // member functions
      void LoadJets(JetType jetType);
      bool isParton(const reco::GenParticle & p);
      bool passBasicKin(const InputItem & cand);

      // data members
      InputCollection jets_; 

    private:
      fwlite::Event * event_;
  };
} // jetana namespace

#endif //HiJetAna_HiJetAnaInput_h
