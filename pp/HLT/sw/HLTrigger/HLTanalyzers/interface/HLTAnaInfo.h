#ifndef HLTAnaInfo_H
#define HLTAnaInfo_H

#include "FWCore/Framework/interface/Event.h"
#include "TChain.h"


/** \class HLTAnaInfo
  *  
  */
class HLTAnaInfo {
public:
  HLTAnaInfo(); 
	~HLTAnaInfo();

  void setup(TTree* tree);

  /** Analyze the Data */
  void analyze(edm::Event const& iEvent, TTree* tree);

private:

  // Tree variables
  int fEvtType;

  // input variables
  bool _Debug;

  // trigger names
};

#endif
