#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TH2F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

// general cmssw
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// jet related
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Math/interface/deltaR.h"
// ana classes
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"
using namespace std;
using namespace jetana;

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
  
  // open input file (can be located on castor)
  //TFile* inFile = TFile::Open( "file:PATLayer1_Output.fromAOD_full.root" );
  char * inFileName;
  if (argc==1) {
    inFileName = "file:/d01/frankma/scratch/data/pat/cmssw330pre5/yetkin_RelValHydjetQ_MinBias_4TeV/edmfile_1.root";
  }
  else {
    inFileName = argv[1];
  }
  printf("inFileName: %s\n",inFileName);
  TFile* inFile = TFile::Open(inFileName);

  // ----------------------------------------------------------------------
  unsigned int iEvent=0;
  fwlite::Event event(inFile);
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){
    // break loop after end of file is reached 
    // or after 1000 events have been processed
    if( iEvent==1000 ) break;

    // simple event counter
    if(iEvent>0 && iEvent%100==0){
      std::cout << "  processing event: " << iEvent << std::endl;
    }

    // get input collection in event
    HiJetAnaInput jetinput(&event);
    jetinput.LoadJets(PATJET);
  } // event loop
  // that's it!
  return 0;
}
