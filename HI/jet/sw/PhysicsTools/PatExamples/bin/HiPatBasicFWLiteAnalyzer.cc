//
// based on
// - https://twiki.cern.ch/twiki/bin/view/CMS/PhysicsToolsFWLiteExecutable
//

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/HeavyIonEvent/interface/HeavyIon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Math/interface/deltaR.h"

// fwlite tools
#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
// ana classes
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"

#include<iostream>
using namespace std;

int main(int argc, char* argv[]) 
{
  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // Tell people what this analysis code does and setup default options.
  optutl::CommandLineParser parser ("Plots HI Event info");
  parser.stringValue ("outputFile") = "hipat_hievent.root"; // .root added automatically
  // Parse the command line arguments
  parser.parseArguments (argc, argv);

  // setup come ana cuts
  HiDiJetAnaConfig anacfg;
  anacfg.trackEtaMax_ = 2.5;
  anacfg.trackPtMin_ = 0.5;

  //////////////////////////////////
  // //////////////////////////// //
  // // Create Event Container // //
  // //////////////////////////// //
  //////////////////////////////////

  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::EventContainer eventCont (parser);

  // book histograms for basic objects
  const double PARTLPTMAX=100;
  const double JETETMAX=250;
  eventCont.add( new TH1D("genjetPt", "pt",    100,  0.,JETETMAX) );
  eventCont.add( new TH1D("genjetEta","eta",   100, -5.,  5.) );
  eventCont.add( new TH1D("genjetPhi","phi",   100, -5.,  5.) );
  eventCont.add( new TH1D("rawjetPt", "pt",    100,  0.,JETETMAX*0.8) );
  eventCont.add( new TH1D("jetPt", "pt",    100,  0.,JETETMAX) );
  eventCont.add( new TH1D("jetEta","eta",   100, -5.,  5.) );
  eventCont.add( new TH1D("jetPhi","phi",   100, -5.,  5.) );
  eventCont.add( new TH2D("matjetDR", "dR",    100,   0, 6,100,0,JETETMAX) ); 
  eventCont.add( new TH2D("jetDR", "dR",    100,   0, 6, 100,0,JETETMAX) ); 
  eventCont.add( new TH2D("jetPtCorrel", "pt gen vs calo",    100,  0.,JETETMAX, 100,0,JETETMAX) );
  
  eventCont.add( new TH1D("partonPt", "pt",    100,  0.,JETETMAX) );
  eventCont.add( new TH1D("partonEta","eta",   100, -5.,  5.) );
  eventCont.add( new TH1D("partonPhi","phi",   100, -5.,  5.) );

  eventCont.add( new TH1D("partlPt", "pt",    100,  0.,PARTLPTMAX) );
  eventCont.add( new TH1D("partlEta","eta",   100, -5.,  5.) );
  eventCont.add( new TH1D("partlPhi","phi",   100, -5.,  5.) );

  eventCont.add( new TH1D("trackPt", "pt",    100,  0.,PARTLPTMAX) );
  eventCont.add( new TH1D("trackEta","eta",   100, -5.,  5.) );
  eventCont.add( new TH1D("trackPhi","phi",   100, -5.,  5.) );
  // Book those histograms for HI Event
  eventCont.add( new TH1F( "genb", "genb", 50, 0, 20) );
  eventCont.add( new TH1F( "gennpart", "gennpart", 100, 0, 300) );
  eventCont.add( new TH1F( "genncoll", "genncoll", 100, 0, 600) );

  // ----------------------------------------------------------------------
  // Second Part: 
  //
  //  * loop the events in the input file 
  //  * receive the collections of interest via fwlite::Handle
  //  * fill the histograms
  //  * after the loop close the input file
  // ----------------------------------------------------------------------

  //////////////////////
  // //////////////// //
  // // Event Loop // //
  // //////////////// //
  //////////////////////

  unsigned int iEvent=0;
  for (eventCont.toBegin(); ! eventCont.atEnd(); ++eventCont, ++iEvent) 
  {
    // we'll use some functionalities of HiJetAnaInput
    HiJetAnaInput anain(&eventCont,&anacfg);

    //////////////////////////////////
    // Take What We Need From Event //
    //////////////////////////////////

    // fwlite::Handle to to jet collection
    fwlite::Handle<std::vector<pat::Jet> > jets;
    jets.getByLabel(eventCont, "selectedLayer1Jets");
    cout << "got pat jets: " << jets->size() << endl;
    
    // fwlite::Handle to genjet collection
    fwlite::Handle<std::vector<reco::GenJet> > gjets;
    gjets.getByLabel(eventCont,"iterativeCone5HiGenJets");
    //gjets.getByLabel(eventCont,"hiGenJetCleaner");
    cout << "got gen jets: " << gjets->size() << endl;

    // loop jet collection and fill histograms
    for(unsigned i=0; i<jets->size(); ++i){
      eventCont.hist("rawjetPt")->Fill( (*jets)[i].correctedJet("raw").pt()  );
      eventCont.hist("jetPt")->Fill( (*jets)[i].pt()  );
      eventCont.hist("jetEta")->Fill( (*jets)[i].eta() );
      eventCont.hist("jetPhi")->Fill( (*jets)[i].phi() );
      printf("cor caljet et|eta|phi: %f|%f|%f\n",
	  (*jets)[i].pt(), (*jets)[i].eta(), (*jets)[i].phi());
      printf("raw caljet et|eta|phi: %f|%f|%f\n",
	  (*jets)[i].correctedJet("raw").pt(),
	  (*jets)[i].correctedJet("raw").eta(),
	  (*jets)[i].correctedJet("raw").phi());

      // If there is a matched genjet to the caljet
      const reco::GenJet * genjet = (*jets)[i].genJet();
      if ( genjet != NULL ) {
	Double_t jetDR = reco::deltaR((*jets)[i],*genjet);
	printf("genjet et|eta|phi: %f|%f|%f, dR: %f\n",
	    genjet->pt(),genjet->eta(),genjet->phi(),jetDR);
	eventCont.hist("matjetDR")->Fill(jetDR,genjet->pt());
	eventCont.hist("jetPtCorrel")->Fill((*jets)[i].pt(),genjet->pt());
      }

      // loop genjet collection
      for (unsigned j=0; j<gjets->size(); ++j) {
	Double_t jetDR = reco::deltaR((*jets)[i],(*gjets)[j]);
	eventCont.hist("jetDR")->Fill(jetDR,(*gjets)[j].pt());
      }  
    }

    // loop over gen jets
    for (unsigned j=0; j<gjets->size(); ++j) {
      eventCont.hist("genjetPt")->Fill( (*gjets)[j].pt()  );
      eventCont.hist("genjetEta")->Fill( (*gjets)[j].eta() );
      eventCont.hist("genjetPhi")->Fill( (*gjets)[j].phi() );
    }  
    

    // fwlight::Handle to gen particles
    fwlite::Handle<std::vector<reco::GenParticle> > particles;
    particles.getByLabel(eventCont, "hiGenParticles");
    // fwlight::Handle to tracks
    fwlite::Handle<std::vector<reco::Track> > tracks;
    tracks.getByLabel(eventCont, "hiSelectedTracks");

    // loop particle collection and fill
    for (unsigned ip=0; ip<particles->size(); ++ip) {
      reco::GenParticle p = (*particles)[ip];

      // check partons
      if (anain.isParton(p)) {
	printf("parton et|eta|phi: %f|%f|%f\n",p.et(),p.eta(),p.phi());
	eventCont.hist("partonPt")->Fill( p.pt()  );
	eventCont.hist("partonEta")->Fill( p.eta() );
	eventCont.hist("partonPhi")->Fill( p.phi() );
      }

      if (p.status() == 1 && p.charge()!=0 && anain.passBasicTrackKin(p.p4())) {
	eventCont.hist("partlPt") ->Fill( (*particles)[ip].pt()  );
	eventCont.hist("partlEta")->Fill( (*particles)[ip].eta() );
	eventCont.hist("partlPhi")->Fill( (*particles)[ip].phi() );
      }
    }

    // loop track collection and fill
    for (unsigned it=0; it<tracks->size(); ++it) {
      eventCont.hist("trackPt") ->Fill( (*tracks)[it].pt()  );
      eventCont.hist("trackEta")->Fill( (*tracks)[it].eta() );
      eventCont.hist("trackPhi")->Fill( (*tracks)[it].phi() );
    }
    // fwlite::Handle to to HI info collections
    fwlite::Handle<pat::HeavyIon> hievt;
    hievt.getByLabel(eventCont,"heavyIon");
    pat::HeavyIon hievtcp = (*hievt);
    cout << "HI event " << iEvent << "  gen b|npart|ncoll:"
      << hievtcp.generatedB() << "|" << hievtcp.generatedNpart() << "|" << hievtcp.generatedNcoll() << endl;
    eventCont.hist("genb")->Fill(hievtcp.generatedB());
    eventCont.hist("gennpart")->Fill(hievtcp.generatedNpart());
    eventCont.hist("genncoll")->Fill(hievtcp.generatedNcoll());
  }

  // that's it!
  return 0;
}
