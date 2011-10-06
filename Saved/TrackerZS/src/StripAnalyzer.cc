// -*- C++ -*-
//
// Package:    StripAnalyzer
// Class:      StripAnalyzer
// 
/**\class StripAnalyzer StripAnalyzer.cc Saved/TrackerZS/src/StripAnalyzer.cc
 
 Description: [one line class summary]
 
 Implementation:
 [Notes on implementation]
 */
//
// Original Author:  Yetkin Yilmaz
// Modified: Frank Ma
//         Created:  Wed Sep 28 16:27:01 EDT 2011
// $Id: StripAnalyzer.cc,v 1.1 2011/10/05 09:33:51 yilmaz Exp $
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include "DataFormats/SiStripDigi/interface/SiStripProcessedRawDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripCluster/interface/SiStripClusterCollection.h"

#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "CondFormats/DataRecord/interface/SiStripPedestalsRcd.h"

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingFactory.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

#include "TTree.h"

using namespace std;
const int MAXSTRIP = 1000;

struct StripEvent{
  uint32_t run;
  uint32_t evt;
  uint32_t id;
  uint32_t mod;
  uint32_t nstrip;
  uint32_t curs[MAXSTRIP];
  uint32_t strip[MAXSTRIP];
  float adc[MAXSTRIP];
  
  TTree* t;
};

//
// class declaration
//

class StripAnalyzer : public edm::EDAnalyzer {
public:
  explicit StripAnalyzer(const edm::ParameterSet&);
  ~StripAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  void analyzeDigi(edm::Handle< edm::DetSetVector<SiStripDigi> >& , StripEvent& t);
  void analyzeRawDigi(edm::Handle< edm::DetSetVector<SiStripRawDigi> >& , StripEvent& t);
  void analyzeProcessedRawDigi(edm::Handle< edm::DetSetVector<SiStripProcessedRawDigi> >& , StripEvent& t);
  
  void prepareTree(TTree * tree, StripEvent& s);
  
  // ----------member data ---------------------------
  edm::Service<TFileService> fs;
  edm::InputTag tag_;
  edm::InputTag mctag_;
  edm::InputTag vrtag_;
  
  edm::InputTag zstag_;
  edm::InputTag digitag_;
  
  
  TTree * tVR_;
  
  StripEvent vr_;
  
  int run_;
  int evt_;
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
StripAnalyzer::StripAnalyzer(const edm::ParameterSet& iConfig)
{
  vrtag_ = iConfig.getParameter<edm::InputTag>("vr");
  //now do what ever initialization is needed  
}


StripAnalyzer::~StripAnalyzer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called for each event  ------------
void
StripAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  run_ = iEvent.id().run();
  evt_ = iEvent.id().event();
  edm::Handle< edm::DetSetVector<SiStripRawDigi> >   handVR;
  iEvent.getByLabel(vrtag_,handVR);
  
  analyzeRawDigi(handVR, vr_);
}

void
StripAnalyzer::analyzeDigi(edm::Handle< edm::DetSetVector<SiStripDigi> >& hSSRD, StripEvent& t)
{
  edm::DetSetVector<SiStripDigi>::const_iterator rawDSViter=hSSRD->begin();
  t.run = run_;
  t.evt = evt_;
  t.mod = 0;
  for (; rawDSViter!=hSSRD->end();rawDSViter++){
    t.nstrip = 0;
    t.id = rawDSViter->detId();
    //float fdetid = detid;
    //cout<<"currentmodule : "<<currentmodule<<"   detid : "<<detid<<"    fdetid : "<<fdetid<<endl;
    edm::DetSet<SiStripDigi>::const_iterator iRawDigi = rawDSViter->begin();
    while( iRawDigi != rawDSViter->end() ) {
      t.curs[t.nstrip] = t.nstrip;
      t.strip[t.nstrip] = iRawDigi->strip();
      t.adc[t.nstrip] = iRawDigi->adc();
      iRawDigi++;
      t.nstrip++;
    }
    t.t->Fill();
    ++t.mod;
  }
}

void
StripAnalyzer::analyzeRawDigi(edm::Handle< edm::DetSetVector<SiStripRawDigi> >& hSSRD, StripEvent& t)
{
  edm::DetSetVector<SiStripRawDigi>::const_iterator rawDSViter=hSSRD->begin();
  t.run = run_;
  t.evt = evt_;
  t.mod = 0;
  for (; rawDSViter!=hSSRD->end();rawDSViter++){
    t.nstrip = 0;
    t.id = rawDSViter->detId();
    edm::DetSet<SiStripRawDigi>::const_iterator iRawDigi = rawDSViter->begin();
    while( iRawDigi != rawDSViter->end() ) {
      t.curs[t.nstrip] = 0;
      t.strip[t.nstrip] = t.nstrip;
      t.adc[t.nstrip] = iRawDigi->adc();
      iRawDigi++;
      t.nstrip++;
    }
    t.t->Fill();
    ++t.mod;
  }
}

void
StripAnalyzer::analyzeProcessedRawDigi(edm::Handle< edm::DetSetVector<SiStripProcessedRawDigi> >& hSSRD, StripEvent& t)
{
  edm::DetSetVector<SiStripProcessedRawDigi>::const_iterator rawDSViter=hSSRD->begin();
  t.run = run_;
  t.evt = evt_;
  t.mod = 0;
  for (; rawDSViter!=hSSRD->end();rawDSViter++){
    t.nstrip = 0;
    t.id = rawDSViter->detId();
    edm::DetSet<SiStripProcessedRawDigi>::const_iterator iRawDigi = rawDSViter->begin();
    while( iRawDigi != rawDSViter->end() ) {
      t.curs[t.nstrip] = 0;
      t.strip[t.nstrip] = t.nstrip;
      t.adc[t.nstrip] = iRawDigi->adc();
      iRawDigi++;
      t.nstrip++;
    }
    t.t->Fill();
    ++t.mod;
  }
}

// ------------ method called once each job just before starting event loop  ------------
void 
StripAnalyzer::beginJob()
{
  tVR_ = fs->make<TTree>("vr","");  
  
  prepareTree(tVR_,vr_);
}

void StripAnalyzer::prepareTree(TTree * tree, StripEvent& s){
  s.t = tree;
  tree->Branch("module",&s.run,"run/i:evt:id:mod:nstrip");
  tree->Branch("curs",s.curs,"curs[nstrip]/i");
  tree->Branch("strip",s.strip,"strip[nstrip]/i");
  tree->Branch("adc",s.adc,"adc[nstrip]/F");
}



// ------------ method called once each job just after ending the event loop  ------------
void 
StripAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
StripAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
StripAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
StripAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
StripAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
StripAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(StripAnalyzer);
