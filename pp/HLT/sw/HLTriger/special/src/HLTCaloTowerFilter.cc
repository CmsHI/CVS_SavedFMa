// -*- C++ -*-
//
// Package:    HLTCaloTowerFilter
// Class:      HLTCaloTowerFilter
// 
/**\class HLTCaloTowerFilter HLTCaloTowerFilter.cc Work/HLTCaloTowerFilter/src/HLTCaloTowerFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yen-Jie Lee
//         Created:  Wed Nov 13 16:12:29 CEST 2009
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
//
// class declaration
//

class HLTCaloTowerFilter : public HLTFilter {
public:
    explicit HLTCaloTowerFilter(const edm::ParameterSet&);
    ~HLTCaloTowerFilter();
    
private:
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    
    // ----------member data ---------------------------
    edm::InputTag inputTag_; // input tag identifying product
    bool saveTag_;           // whether to save this tag
    double min_Pt_;          // pt threshold in GeV 
    double max_Eta_;         // eta range (symmetric)
    int    min_N_;           // number of objects passing cuts required
    
};

//
// constructors and destructor
//
HLTCaloTowerFilter::HLTCaloTowerFilter(const edm::ParameterSet& iConfig) :
  inputTag_ (iConfig.getParameter<edm::InputTag>("inputTag")),
  min_Pt_   (iConfig.getParameter<double>       ("MinPt"   )),
  max_Eta_  (iConfig.getParameter<double>       ("MaxEta"  )),
  min_N_    (iConfig.getParameter<int>          ("MinN"    ))
{
    
}


HLTCaloTowerFilter::~HLTCaloTowerFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
HLTCaloTowerFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace std;
   using namespace edm;
   using namespace reco;

   // get hold of collection of objects
   Handle<CaloTowerCollection> CaloTowers;
   iEvent.getByLabel (inputTag_,CaloTowers);

   // look at all objects, check cuts and add to filter object
   int n(0);
   CaloTowerCollection::const_iterator i ( CaloTowers->begin() );
   for (; i!=CaloTowers->end(); i++) {
     if ( (i->pt() >= min_Pt_) && 
	  ( (max_Eta_ < 0.0) || (abs(i->eta()) <= max_Eta_) ) ) {
       n++;
     }
   }

   // filter decision
   bool accept(n>=min_N_);

   return accept;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTCaloTowerFilter);
