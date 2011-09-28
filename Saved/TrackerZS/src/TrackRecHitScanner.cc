// -*- C++ -*-
//
// Package:    TrackRecHitScanner
// Class:      TrackRecHitScanner
// 
/**\class TrackRecHitScanner TrackRecHitScanner.cc Saved/TrackRecHitScanner/src/TrackRecHitScanner.cc
 
 Description: [one line class summary]
 
 Implementation:
 [Notes on implementation]
 */
//
// Original Author:  Teng Ma
//         Created:  Tue Sep 27 12:17:27 EDT 2011
// $Id$
//
//


// system include files
#include <memory>
#include <vector>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1DCollection.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "RecoTracker/TrackProducer/interface/ClusterRemovalRefSetter.h"

//
// class declaration
//

class TrackRecHitScanner : public edm::EDAnalyzer {
public:
  explicit TrackRecHitScanner(const edm::ParameterSet&);
  ~TrackRecHitScanner();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
  // ----------member data ---------------------------
  edm::InputTag trkSrc_;
  double trkPtMin_;
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
TrackRecHitScanner::TrackRecHitScanner(const edm::ParameterSet& iConfig) :
trkSrc_(iConfig.getParameter<edm::InputTag>("trkSrc")),
trkPtMin_(iConfig.getParameter<double>("trkPtMin"))
{
  //now do what ever initialization is needed
  
}


TrackRecHitScanner::~TrackRecHitScanner()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called for each event  ------------
void
TrackRecHitScanner::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //
  // extract tracker geometry
  //
  //edm::ESHandle<TrackerGeometry> theG;
  //iSetup.get<TrackerDigiGeometryRecord>().get(theG);
  
  using namespace edm;
  using namespace std;
  
  Handle<vector<reco::Track> > trks;
  iEvent.getByLabel(trkSrc_,trks);
  
  for (size_t i=0; i<trks->size(); ++i) {
    const reco::Track & trk = (*trks)[i];
    if (trk.pt()<trkPtMin_) continue;
    cout << i << " pt: " << trk.pt() << endl;
    
    trackingRecHit_iterator itB = trk.recHitsBegin();
    trackingRecHit_iterator itE = trk.recHitsEnd();
    int ihit=0;
    for (trackingRecHit_iterator it = itB;  it != itE; ++it, ++ihit) { 
      cout << "hit " << ihit << ": ";
      if ((*it)->isValid()){
        int recHitDetId = (*it)->geographicalId();
        cout << " DetId: " << recHitDetId;
        //cout <<"\t\t\tRecHit on det " <<(*it)->geographicalId().rawId()<<std::endl;
        //cout <<"\t\t\tRecHit in LP "  <<(*it)->localPosition()<<std::endl;
        //cout <<"\t\t\tRecHit in GP "  <<theG->idToDet((*it)->geographicalId())->surface().toGlobal((*it)->localPosition()) <<std::endl;
      }
      cout << endl;
    }
  }
  
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackRecHitScanner::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrackRecHitScanner::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
TrackRecHitScanner::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TrackRecHitScanner::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TrackRecHitScanner::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TrackRecHitScanner::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TrackRecHitScanner::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackRecHitScanner);
