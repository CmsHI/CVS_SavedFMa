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
      TNtuple * ntHiGenJets;

      // -------------- Define output file -------------
      TFile * tf;

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
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

   int genjetN = genjetVec->size();
   printf("total number of standard gen jets: %d\n", genjetN);

   // jet variables
   int ct=0;
   vector<reco::GenJet>::const_iterator igenjet;
   for (igenjet = genjetVec->begin(); igenjet != genjetVec->end(); ++igenjet) {
      cout << "genjet " << ct << endl;
      cout << "had energy: " << (*igenjet).hadEnergy() << endl;
      //-- Constituents --
      //  wait for now b/c not yet implemented for the subevent jets 
      //std::vector <const GenParticle*> mcparts = (*igenjet).getGenConstituents ();
      //cout << (*igenjet).print() << endl;
      ++ct;
   }

   // now for Yetkin's genJets
   Handle< vector<reco::GenJet> > higenjetVec;
   iEvent.getByLabel("iterativeCone5HiGenJets", higenjetVec);

   int higenjetN = higenjetVec->size();
   printf("total number of subevent gen jets: %d\n", higenjetN);

   // jet variables
   int hict=0;
   vector<reco::GenJet>::const_iterator ihigenjet;
   for (ihigenjet = higenjetVec->begin(); ihigenjet != higenjetVec->end(); ++ihigenjet) {
      cout << "higenjet " << hict << endl;
      cout << (*ihigenjet).print() << endl;
      float pt = (*ihigenjet).pt();
//      double pz = (*ihigenjet).pz(); somehow needs some ROOT::Math lib?
      double pz = -100;
      float eta = (*ihigenjet).eta();
      float phi = (*ihigenjet).phi();
      cout << "pt: " << pt << " pz: " << pz << " eta: " << eta <<  " phi: " << phi << endl;
      ntHiGenJets->Fill(pt,pz,eta,phi);
      ++hict;
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
MyGenJetAna::beginJob(const edm::EventSetup&)
{
   // dynamically allocate memory for the tfile and ntuples.
   tf = new TFile("GenAna.root","RECREATE");
   ntGenPartls = new TNtuple("GenParticles","Generated particles","pt:pz:eta:phi");
   ntHiGenJets = new TNtuple("GenJets","Generator level iCone jets","pt:pz:eta:phi");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyGenJetAna::endJob() {
   tf->cd();
   ntGenPartls->Write();
   ntHiGenJets->Write();
   tf->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyGenJetAna);
