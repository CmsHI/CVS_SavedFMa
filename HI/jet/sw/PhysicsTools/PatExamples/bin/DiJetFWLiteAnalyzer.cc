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
  TH1D* jetPt_  = new TH1D("jetPt", "pt",    100,  0.,150.);
  TH1D* jetEta_ = new TH1D("jetEta","eta",   100, -5.,  5.);
  TH1D* jetPhi_ = new TH1D("jetPhi","phi",   100, -5.,  5.);

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // open input file (can be located on castor)
  // set defaults
  char * inFileName = "file:/d01/frankma/scratch/data/pat/cmssw330pre5/yetkin_RelValHydjetQ_MinBias_4TeV/edmfile_1.root";
  JetType jetType=PATJET;
  if (argc>=2) {
    inFileName = argv[1];
  }
  if (argc>=3) {
    jetType=(JetType)(TString(argv[2]).Atoi());
  }
  printf("inFileName: %s\n",inFileName);
  TFile* inFile = TFile::Open(inFileName);

  // open output file
  TFile outFile( Form("analyzeDiJetsType%d.root",jetType), "recreate" );
  outFile.mkdir("analyzeBasicPat");
  outFile.cd("analyzeBasicPat");
  // lazy way to write histos to file book a set of histograms
  TH2D* matjetDR_  = new TH2D("matjetDR", "dR",    100,   0, 6,100,0,100); 
  TH2D* jetDR_  = new TH2D("jetDR", "dR",    100,   0, 6, 100,0,100); 
  TH2D* dijetDPhi  = new TH2D("dijetDPhi", "dijet dphi vs dijet et ratio",    100,   0, TMath::Pi(), 100,0,1.5); 
  TH2D* dijetAwayNearEt  = new TH2D("dijetAwayNearEt", "dijet away et vs near et",    100,   0, 100, 100,0,100); 
  TH2D* ldijetDPhi  = new TH2D("ldijetDPhi", "ldijet dphi vs dijet et ratio",    100,   0, TMath::Pi(), 100,0,1.5); 
  TH2D* ldijetDPhiMul  = new TH2D("ldijetDPhiMul", "dijet dphi vs lead jet away multiplicity",    100,   0, TMath::Pi(), 24,-2,10.); 
  TH2D* ldijetAwayNearEt  = new TH2D("ldijetAwayNearEt", "ldijet away et vs near et",    100,   0, 100, 100,0,100); 
  TH1D* hldjAwayJetMul = new TH1D("hldjAwayJetMul","lead di away side jet multiplicity",24,-2.,10.);
  TH1D* hevtJetMul = new TH1D("hevtJetMul","event jet multiplicity",104,-2.,50.);

  
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
    if(iEvent>0 && iEvent%1==0){
      std::cout << "  processing event: " << iEvent << std::endl;
    }

    // get input collection in event
    HiJetAnaInput jetinput(&event);
    jetinput.LoadJets(jetType);
    //   fill some info of the input collection
    hevtJetMul->Fill(jetinput.jets_.size());

    // prepare output
    OutputCollection output;

    // run dijet algo on input/output
    HiDiJetAlgorithm djalgo;
    djalgo.SetVerbosity(3);
    int ldjAwayJetMul = djalgo.Group(jetinput.jets_,&output);
    //   fill some details of the algo
    hldjAwayJetMul->Fill(ldjAwayJetMul);

    // test output
    cout << "Dijets: " << endl;
    for (OutputCollection::iterator itdj=output.begin(); itdj!=output.end(); ++itdj) {
      cout << *itdj << "  lead away mul: " << ldjAwayJetMul << endl;
      dijetDPhi->Fill((*itdj).dphi_,(*itdj).aj_.pt()/(*itdj).nj_.pt());
      dijetAwayNearEt->Fill((*itdj).nj_.pt(),(*itdj).aj_.pt());
      if (itdj==output.begin()) {
	ldijetDPhi->Fill((*itdj).dphi_,(*itdj).aj_.pt()/(*itdj).nj_.pt());
	ldijetDPhiMul->Fill((*itdj).dphi_,ldjAwayJetMul);
	ldijetAwayNearEt->Fill((*itdj).nj_.pt(),(*itdj).aj_.pt());
      }
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
