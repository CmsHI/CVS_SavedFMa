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
#include "TMath.h"

// general cmssw
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// jet related
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Math/interface/deltaR.h"
// fwlite tools
#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
// ana classes
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAnalysis.h"

#include<iostream>
using namespace std;


int main(int argc, char* argv[]) 
{
  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * configure ana
  //  * book the histograms of interest 
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // Tell people what this analysis code does and setup default options.
  optutl::CommandLineParser parser ("HI DiJet Analysis");
  // note input file (can be located on castor)
  // configure ana types
  parser.addOption ("jetType", optutl::CommandLineParser::kInteger, 
      "Parton(0), Genjet(1), CaloJet(2)", 2);
  parser.addOption ("trackType", optutl::CommandLineParser::kInteger, 
      "Particle(0), Track(1)", 1);
  parser.addOption ("jetCorrec", optutl::CommandLineParser::kBool,
      "use correted jet energy?",true); 
  // for later: configure whether jet corrected
  // Parse the command line arguments
  parser.parseArguments (argc, argv);
  // Config Analysis
  JetType jetType=(JetType)parser.integerValue("jetType");
  TrackType trackType=(TrackType)parser.integerValue("trackType");
  HiDiJetAnaConfig anacfg;
  anacfg.jetEtMin_ = 25;
  anacfg.JECFactor_ = 1.6; // JEC factor for Uncorrector threshold
  anacfg.awayEtFrac_ = 0.8; // away Et frac for dijet algo threshold
  anacfg.doJEC_ = parser.boolValue("jetCorrec");
  anacfg.trackPtMin_ = 1.2;
  parser.stringValue ("outputFile") = Form("djana_j%d_%d_t%d",jetType,anacfg.doJEC_,trackType); // .root added automatically

  //////////////////////////////////
  // //////////////////////////// //
  // // Create Event Container // //
  // //////////////////////////// //
  //////////////////////////////////

  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::EventContainer eventCont (parser);

  // Book those histograms!
  eventCont.add( new TH1D("jetPt", "pt",    100,  0.,150.) );
  eventCont.add( new TH1D("jetEta","eta",   100, -5.,  5.) );
  eventCont.add( new TH1D("jetPhi","phi",   100, -5.,  5.) );
  eventCont.add( new TH2D("matjetDR", "dR",    100,   0, 6,100,0,100) ); 
  eventCont.add( new TH2D("jetDR", "dR",    100,   0, 6, 100,0,100) ); 
  eventCont.add( new TH2D("dijetDPhi", "dijet dphi vs dijet et ratio",    100,   0, TMath::Pi(), 100,0,1.5) ); 
  eventCont.add( new TH2D("dijetAwayNearEt", "dijet away et vs near et",    100,   0, 100, 100,0,100) ); 
  eventCont.add( new TH2D("ldijetDPhi", "ldijet dphi vs dijet et ratio",    100,   0, TMath::Pi(), 100,0,1.5) ); 
  eventCont.add( new TH2D("ldijetDPhiMul", "dijet dphi vs lead jet away multiplicity",    100,   0, TMath::Pi(), 24,-2,10.) ); 
  eventCont.add( new TH2D("ldijetAwayNearEt", "ldijet away et vs near et",    100,   0, 100, 100,0,100) ); 
  eventCont.add( new TH1D("hldjAwayJetMul","lead di away side jet multiplicity",24,-2.,10.) );
  eventCont.add( new TH1D("hevtJetMul","event jet multiplicity",104,-2.,50.) );

  // allocates memory for dijet ana tree data
  HiDiJetAnalysis ana;
  ana.SetAnaOnlyLeadDijet(true);
  ana.SetAnaFF(true);
  ana.SetVerbosity(1);

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
  for (eventCont.toBegin(); ! eventCont.atEnd(); ++eventCont, ++iEvent) {
    //////////////////////////////////
    // Use What We Need From Event //
    //////////////////////////////////

    // get input collection in event
    HiJetAnaInput anaInput(&eventCont,&anacfg);
    anaInput.SetVerbosity(1);

    anaInput.LoadJets(jetType);
    //   fill some info of the input jet collection
    eventCont.hist("hevtJetMul")->Fill(anaInput.jets_.size());

    // if no jets found, skip this event
    if ( anaInput.jets_.size()==0 ) continue;

    anaInput.LoadTracks(trackType);

    // run dijet algo on input/output
    OutputCollection dijets;
    HiDiJetAlgorithm djalgo(&anacfg);
    djalgo.SetVerbosity(1);
    int ldjAwayJetMul = djalgo.Group(anaInput.jets_,&dijets);

    //   fill histo for some details of the algo
    eventCont.hist("hldjAwayJetMul")->Fill(ldjAwayJetMul);
    //   fill histo for basic jet/tracks info
    //cout << "Dijets: " << endl;
    for (OutputCollection::iterator itdj=dijets.begin(); itdj!=dijets.end(); ++itdj) {
      //cout << *itdj << "  lead away mul: " << ldjAwayJetMul << endl;
      eventCont.hist("dijetDPhi")->Fill((*itdj).dphi_,(*itdj).aj_.pt()/(*itdj).nj_.pt());
      eventCont.hist("dijetAwayNearEt")->Fill((*itdj).nj_.pt(),(*itdj).aj_.pt());
      if (itdj==dijets.begin()) {
	eventCont.hist("ldijetDPhi")->Fill((*itdj).dphi_,(*itdj).aj_.pt()/(*itdj).nj_.pt());
	eventCont.hist("ldijetDPhiMul")->Fill((*itdj).dphi_,ldjAwayJetMul);
	eventCont.hist("ldijetAwayNearEt")->Fill((*itdj).nj_.pt(),(*itdj).aj_.pt());
      }
    }

    // run ana
    ana.Fill(dijets,anaInput.tracks_);

  } // event loop

  // save ana tree
  TFile outf(Form("tree_%s",parser.stringValue("outputfile").c_str()),"RECREATE","dijet ana data");
  ana.tree_->Write();
  outf.Close();

  // that's it!
  return 0;
}
