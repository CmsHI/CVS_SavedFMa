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
// dijet classes
#include "PhysicsTools/PatExamples/interface/DiJet.h"
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
  
  // book a set of histograms
  TH1F* jetPt_  = new TH1F("jetPt", "pt",    100,  0.,150.);
  TH1F* jetEta_ = new TH1F("jetEta","eta",   100, -5.,  5.);
  TH1F* jetPhi_ = new TH1F("jetPhi","phi",   100, -5.,  5.);
  TH2F* matjetDR_  = new TH2F("matjetDR", "dR",    100,   0, 6,100,0,100); 
  TH2F* jetDR_  = new TH2F("jetDR", "dR",    100,   0, 6, 100,0,100); 
  
  TH1F* partlPt_  = new TH1F("partlPt", "pt",    100,  0.,50.);
  TH1F* partlEta_ = new TH1F("partlEta","eta",   100, -5.,  5.);
  TH1F* partlPhi_ = new TH1F("partlPhi","phi",   100, -5.,  5.);

  TH1F* trackPt_  = new TH1F("trackPt", "pt",    100,  0.,50.);
  TH1F* trackEta_ = new TH1F("trackEta","eta",   100, -5.,  5.);
  TH1F* trackPhi_ = new TH1F("trackPhi","phi",   100, -5.,  5.);

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
  // Second Part: 
  //
  //  * loop the events in the input file 
  //  * receive the collections of interest via fwlite::Handle
  //  * fill the histograms
  //  * after the loop close the input file
  // ----------------------------------------------------------------------

  // loop the events
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

    // fwlite::Handle to to jet collection
    fwlite::Handle<std::vector<pat::Jet> > jets;
    jets.getByLabel(event, "selectedLayer1Jets");

    // loop jet collection and fill histograms
    for(unsigned i=0; i<jets->size(); ++i){
      printf("caljet et|eta|phi: %f|%f|%f\n",
	  (*jets)[i].pt(), (*jets)[i].eta(), (*jets)[i].phi());
      cout << (*jets)[i].p4() << endl;
    }

    // fwlite::Handle to general collection
    //fwlite::Handle<reco::CandidateView> inputsHandle;
    fwlite::Handle<std::vector<reco::Candidate> > inputsHandle;
    //inputsHandle.getByLabel(event, "selectedLayer1Jets");
//    inputsHandle.getByLabel(event, "hiSelectedTracks");
//    for(unsigned i=0; i<inputsHandle->size(); ++i){
//      printf("caljet et|eta|phi: %f|%f|%f\n",
//	  (*inputsHandle)[i].pt(), (*inputsHandle)[i].eta(), (*inputsHandle)[i].phi());
//    }
  }

  // close input file
  inFile->Close();

  // ----------------------------------------------------------------------
  // Third Part: 
  //
  //  * open the output file 
  //  * write the histograms to the output file
  //  * close the output file
  // ----------------------------------------------------------------------
  
  //open output file
  TFile outFile( "analyzePatBasics.root", "recreate" );
  outFile.mkdir("analyzeBasicPat");
  outFile.cd("analyzeBasicPat");
  jetPt_ ->Write( );
  jetEta_->Write( );
  jetPhi_->Write( );
  partlPt_ ->Write( );
  partlEta_->Write( );
  partlPhi_->Write( );
  trackPt_ ->Write( );
  trackEta_->Write( );
  trackPhi_->Write( );
  matjetDR_->Write();
  jetDR_->Write();
  outFile.Close();
  
  // ----------------------------------------------------------------------
  // Fourth Part: 
  //
  //  * never forgett to free the memory of the histograms
  // ----------------------------------------------------------------------

  // free allocated space
  delete jetPt_;
  delete jetEta_;
  delete jetPhi_;
  delete partlPt_;
  delete partlEta_;
  delete partlPhi_;
  delete trackPt_;
  delete trackEta_;
  delete trackPhi_;
  
  // that's it!
  return 0;
}
