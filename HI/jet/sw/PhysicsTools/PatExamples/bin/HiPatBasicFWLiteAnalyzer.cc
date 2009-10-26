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

  //////////////////////////////////
  // //////////////////////////// //
  // // Create Event Container // //
  // //////////////////////////// //
  //////////////////////////////////

  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::EventContainer eventCont (parser);

  // Book those histograms!
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
    //////////////////////////////////
    // Take What We Need From Event //
    //////////////////////////////////
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
