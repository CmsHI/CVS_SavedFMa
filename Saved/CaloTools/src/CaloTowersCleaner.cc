// -*- C++ -*-
//
// Package:    CaloTowersCleaner
// Class:      CaloTowersCleaner
// 
/**\class CaloTowersCleaner CaloTowersCleaner.cc Saved/CaloTools/src/CaloTowersCleaner.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jean-Roch Vlimant,40 3-A28,+41227671209,
//         Created:  Thu Nov  4 16:36:30 CET 2010
// $Id: CaloTowersCleaner.cc,v 1.2 2010/11/24 19:52:16 anastass Exp $
//
//

// Frank Ma: Adapted for Custom cleaning of calotowers


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoLocalCalo/CaloTowersCreator/src/CaloTowersCreator.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

//
// class declaration
//

class CaloTowersCleaner : public edm::EDProducer {
   public:
      explicit CaloTowersCleaner(const edm::ParameterSet&);
      ~CaloTowersCleaner();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------

  edm::InputTag regularTowerTag;
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
CaloTowersCleaner::CaloTowersCleaner(const edm::ParameterSet& iConfig)
{
  regularTowerTag=iConfig.getParameter<edm::InputTag>("regularTowerTag");

   //register your products
   produces<CaloTowerCollection>();
}


CaloTowersCleaner::~CaloTowersCleaner()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CaloTowersCleaner::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<CaloTowerCollection> regTower;
	
  iEvent.getByLabel(regularTowerTag,regTower);
	
  std::auto_ptr<CaloTowerCollection> output;
	
	//new collection
	output.reset(new CaloTowerCollection());
  
	CaloTowerCollection::const_iterator rt_begin = regTower->begin();
	CaloTowerCollection::const_iterator rt_end = regTower->end();
	CaloTowerCollection::const_iterator rt_it = rt_begin;
	
	for (;rt_it!=rt_end;++rt_it){
		//just copy the regular tower over
		output->push_back(*rt_it);
	}
	
	// checks
	//std::cout << "org collection size: " << regTower->size() << " pt: " << regTower->begin()->pt() << std::endl;
	//std::cout << "org collection size: " << output->size() << " pt: " << output->begin()->pt() << std::endl;
	
	iEvent.put(output);
}

// ------------ method called once each job just before starting event loop  ------------
void 
CaloTowersCleaner::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CaloTowersCleaner::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(CaloTowersCleaner);
