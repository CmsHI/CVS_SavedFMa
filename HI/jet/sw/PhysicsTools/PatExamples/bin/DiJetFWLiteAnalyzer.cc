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
#include "TMath.h"

// general cmssw
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// jet related
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Math/interface/deltaR.h"
// ana classes
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"

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
  TH1F* jetPt_  = new TH1F("jetPt", "pt",    100,  0.,150.);
  TH1F* jetEta_ = new TH1F("jetEta","eta",   100, -5.,  5.);
  TH1F* jetPhi_ = new TH1F("jetPhi","phi",   100, -5.,  5.);

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

  // open output file
  TFile outFile( "analyzePatBasics.root", "recreate" );
  outFile.mkdir("analyzeBasicPat");
  outFile.cd("analyzeBasicPat");
  // lazy way to write histos to file book a set of histograms
  TH2F* matjetDR_  = new TH2F("matjetDR", "dR",    100,   0, 6,100,0,100); 
  TH2F* jetDR_  = new TH2F("jetDR", "dR",    100,   0, 6, 100,0,100); 
  TH2F* dijetDPhi  = new TH2F("dijetDPhi", "dijet dphi",    100,   0, TMath::Pi(), 100,0,1.5); 
  
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

    // get input collection in event
    HiJetAnaInput jetinput(&event);
    jetinput.LoadJets(PATJET);

    // prepare output
    OutputCollection output;

    // run dijet algo on input/output
    HiDiJetAlgorithm djalgo;
    djalgo.SetVerbosity(0);
    djalgo.Group(jetinput.jets_,&output);

    // test output
    cout << "Dijets: " << endl;
    for (OutputCollection::iterator itdj=output.begin(); itdj!=output.end(); ++itdj) {
      cout << *itdj << endl;
      dijetDPhi->Fill((*itdj).dphi_,(*itdj).aj_.pt()/(*itdj).nj_.pt());
    }
  } // event loop

  // close input file
  inFile->Close();

  // ----------------------------------------------------------------------
  // Third Part: 
  //
  //  * open the output file 
  //  * write the histograms to the output file
  //  * close the output file
  // ----------------------------------------------------------------------
  
  jetPt_ ->Write( );
  jetEta_->Write( );
  jetPhi_->Write( );
  outFile.Write();
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
  // 
  
  // that's it!
  return 0;
}
