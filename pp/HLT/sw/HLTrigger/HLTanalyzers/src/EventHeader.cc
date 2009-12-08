#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>

#include "HLTrigger/HLTanalyzers/interface/EventHeader.h"

EventHeader::EventHeader() {

  //set parameter defaults 
  _Debug=false;
}

EventHeader::~EventHeader() {

}

/*  Setup the analysis to put the branch-variables into the tree. */
void EventHeader::setup(const edm::ParameterSet& pSet, TTree* HltTree) {
  // Get run-time parameters
  edm::ParameterSet myMCParams = pSet.getParameter<edm::ParameterSet>("RunParameters") ;
  std::vector<std::string> parameterNames = myMCParams.getParameterNames() ;
  
  for ( std::vector<std::string>::iterator iParam = parameterNames.begin();
	iParam != parameterNames.end(); iParam++ ){
    if ( (*iParam) == "Debug" ) _Debug =  myMCParams.getParameter<bool>( *iParam );
  }

  // Initialize variables
  fRun = -1;
  fEvent = -1;
  fOrbitNumber = -1;
  fBunchCrossing = -1;
  fIsRealData = -1;

  HltTree->Branch("Run",&fRun,"Run/I");
  HltTree->Branch("Event",&fEvent,"Event/I");
  HltTree->Branch("LumiBlock",&fLumiBlock,"LumiBlock/I"); 
  HltTree->Branch("OrbitNumber",&fOrbitNumber,"OrbitNumber/I"); 
  HltTree->Branch("BunchCrossing",&fBunchCrossing,"BunchCrossing/I"); 
  HltTree->Branch("RealData",&fIsRealData,"RealData/I"); 
}

/* **Analyze the event** */
void EventHeader::analyze(edm::Event const& iEvent, TTree* HltTree) {

  fRun 		= iEvent.id().run();
  fEvent 	= iEvent.id().event();
  fLumiBlock = iEvent.luminosityBlock();
  fOrbitNumber   = iEvent.orbitNumber();
  fBunchCrossing = iEvent.bunchCrossing();
  fIsRealData    = iEvent.isRealData();

  if (_Debug) {

    std::cout << "EventHeader -- run   = " << fRun << std::endl;
    std::cout << "EventHeader -- event = " << fEvent << std::endl;
    std::cout << "EventHeader -- lumisection = " << fLumiBlock << std::endl; 
    std::cout << "EventHeader -- OrbitNumber = " << fOrbitNumber << std::endl; 
    std::cout << "EventHeader -- BunchCrossing = " << fBunchCrossing << std::endl; 
    std::cout << "EventHeader -- isRealData = " << fIsRealData << std::endl; 
  }

}
