#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>

#include "DataFormats/Common/interface/Handle.h"
#include "HLTrigger/HLTanalyzers/interface/HLTAnaInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/GenEvent.h"

HLTAnaInfo::HLTAnaInfo() {

  //set parameter defaults 
  _Debug=true;
}

HLTAnaInfo::~HLTAnaInfo() {

}

/*  Setup the analysis to put the branch-variables into the tree. */
void HLTAnaInfo::setup(TTree* HltTree) {
  HltTree->Branch("evtType",&fEvtType,"evtType/I");
}

/* **Analyze the event** */
void HLTAnaInfo::analyze(edm::Event const& iEvent, TTree* HltTree) {
  using namespace edm;
  edm::Handle<HepMCProduct> mc;
  iEvent.getByLabel("generator",mc);
  const HepMC::GenEvent* evt = mc->GetEvent();

  fEvtType = evt->signal_process_id();

  if (_Debug) {
    std::cout << "HLTAnaInfo -- evtType   = " << fEvtType << std::endl;
  }

}
