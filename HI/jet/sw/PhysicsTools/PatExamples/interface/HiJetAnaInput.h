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
  class HiJetAnaInput
  {
    public:
      // constructor
      HiJetAnaInput(TFile * f) : event_(f) {/*emtpy*/}

      // member functions
      void LoadJets(TString type);

      // data members
      InputCollection jets_; 

    private:
      fwlite::Event event_;
  };
} // jetana namespace

#endif //HiJetAna_HiJetAnaInput_h
