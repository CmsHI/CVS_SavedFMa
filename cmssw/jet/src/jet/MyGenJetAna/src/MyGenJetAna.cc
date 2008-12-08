// -*- C++ -*-
//
// Package:    MyGenJetAna
// Class:      MyGenJetAna
// 
/**\class MyGenJetAna MyGenJetAna.cc jet/MyGenJetAna/src/MyGenJetAna.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Teng Ma
//         Created:  Thu Nov 27 08:18:07 EST 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Data formats
//#include <FastSimulation/CaloGeometryTools/interface/Transform3DPJ.h>
//#include <DataFormats/JetReco/interface/GenJet.h>
//#include <DataFormats/JetReco/interface/Jet.h>
//#include "DataFormats/Common/interface/BoolCache.h"
//#include "DataFormats/Math/interface/Point3D.h"
//#include "DataFormats/Math/interface/Vector3D.h"
//#include "DataFormats/Math/interface/LorentzVector.h"

// genjetana include files
#include <vector>
#include <DataFormats/JetReco/interface/GenJet.h>
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "HepMC/GenEvent.h"

// root include files
#include "TFile.h"
#include "TNtuple.h"
//#include "TMath.h"

using namespace std;

//
// class decleration
//

class MyGenJetAna : public edm::EDAnalyzer {
   public:
      explicit MyGenJetAna(const edm::ParameterSet&);
      ~MyGenJetAna();

      // -------------- Define root ntuples -------------
      TNtuple * ntGenPartls;
      TNtuple * ntFullEvtGenJets;
      TNtuple * ntHiGenJets;

      // -------------- Define output file -------------
      TFile * tf;

      // -------------- methods -----------------------
      int fillJetTree(edm::Handle<std::vector<reco::GenJet> > jetvec, TNtuple * nt);
      int fillGenPartlTree(edm::Handle<edm::HepMCProduct> mc, TNtuple * nt);

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
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
MyGenJetAna::MyGenJetAna(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


MyGenJetAna::~MyGenJetAna()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

int MyGenJetAna::fillJetTree(edm::Handle<std::vector<reco::GenJet> > jetvec, TNtuple * nt)
{
   int genjetN = jetvec->size();
   printf("total number of gen jets in collection: %d\n", genjetN);

   // jet variables
   int ct=0;
   vector<reco::GenJet>::const_iterator igenjet;
   for (igenjet = jetvec->begin(); igenjet != jetvec->end(); ++igenjet) {
      cout << "genjet " << ct << endl;
      cout << "had energy: " << (*igenjet).hadEnergy() << endl;
      //-- Constituents --
      //  wait for now b/c not yet implemented for the subevent jets 
      //std::vector <const GenParticle*> mcparts = (*igenjet).getGenConstituents ();
      //cout << (*igenjet).print() << endl;

      float pt = (*igenjet).pt();
//      double pz = (*igenjet).pz(); somehow needs some ROOT::Math lib?
      double pz = -100;
      float eta = (*igenjet).eta();
      float phi = (*igenjet).phi();
      cout << "pt: " << pt << " pz: " << pz << " eta: " << eta <<  " phi: " << phi << endl;
      nt->Fill(pt,pz,eta,phi);
      ++ct;
   }
   return 0;
}

int fillGenPartlsTree(edm::Handle<edm::HepMCProduct> mc, TNtuple * nt)
{
   const HepMC::GenEvent* evt;
   evt = mc->GetEvent();
   int genPartlN = evt->particles_size();
   cout << "Number of Generated particles: " << genPartlN << endl;

   return 0;
}

// ------------ method called to for each event  ------------
void
MyGenJetAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   // genjetana addition:
   using namespace reco;

   // first for the original (all subevents) genJet collection
   Handle< vector<reco::GenJet> > genjetVec;
   iEvent.getByLabel("iterativeCone5GenJets", genjetVec);
   cout << "===GenJet ran on full event===" << endl;
   fillJetTree(genjetVec, ntFullEvtGenJets);
   
   // now for Yetkin's genJets
   Handle< vector<reco::GenJet> > higenjetVec;
   iEvent.getByLabel("iterativeCone5HiGenJets", higenjetVec);
   cout << endl << "===GenJet ran on sub event===" << endl;
   fillJetTree(higenjetVec, ntHiGenJets);

   // The generated particles (via HepMC format)
   Handle<HepMCProduct> mc;
   iEvent.getByLabel("source", mc);
   cout << endl << "===Gen Particles===" << endl;
   fillGenPartlsTree(mc,ntGenPartls);
}


// ------------ method called once each job just before starting event loop  ------------
void 
MyGenJetAna::beginJob(const edm::EventSetup&)
{
   // dynamically allocate memory for the tfile and ntuples.
   tf = new TFile("GenAna.root","RECREATE");
   ntGenPartls = new TNtuple("GenParticles","Generated particles","pt:pz:eta:phi");
   ntFullEvtGenJets = new TNtuple("FEGenJets","Generator level iCone jets on full event","pt:pz:eta:phi");
   ntHiGenJets = new TNtuple("SEGenJets","Generator level iCone jets on subevents","pt:pz:eta:phi");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyGenJetAna::endJob() {
   tf->cd();
   ntGenPartls->Write();
   ntFullEvtGenJets->Write();
   ntHiGenJets->Write();
   tf->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyGenJetAna);
