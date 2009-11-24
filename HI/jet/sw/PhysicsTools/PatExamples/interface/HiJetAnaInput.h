#ifndef HiJetAna_HiJetAnaInput_h
#define HiJetAna_HiJetAnaInput_h

//stl
#include <map>
// fwlite tools
#include "PhysicsTools/FWLite/interface/EventContainer.h"
// pat
#include "DataFormats/FWLite/interface/Handle.h"
// ana
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgoHelper.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAnaHelpers.h"
// root
#include "TFile.h"
#include "TString.h"

namespace jetana
{
  enum JetType { PARTON, GENJET, PATJET };
  enum TrackType { PARTICLE, TRACK };

  class HiJetAnaInput
  {
    typedef LorentzVector AnaInputItem;
    typedef std::vector<AnaInputItem> AnaInputCollection;

    public:
      // constructor
      HiJetAnaInput(fwlite::EventContainer * ec, HiDiJetAnaConfig * anacfg);

      // settings
      void SetVerbosity(int level) { verbosity_=level; }

      // helper methods
      bool isParton(const reco::GenParticle & p);
      bool isTrack(const reco::GenParticle & p);
      bool passBasicJetKin(const InputItem & cand);
      bool passBasicTrackKin(const AnaInputItem & cand);

      // main methods
      void LoadJets(JetType jetType);
      void LoadTracks(TrackType trackType);

      // data members
      InputCollection jets_; 
      AnaInputCollection tracks_;
      std::map<InputItem*,int> partonSEMap_;

    private:
      fwlite::EventContainer * eventCont_;
      HiDiJetAnaConfig * anacfg_;
      int verbosity_;
  };
} // jetana namespace

#endif //HiJetAna_HiJetAnaInput_h
