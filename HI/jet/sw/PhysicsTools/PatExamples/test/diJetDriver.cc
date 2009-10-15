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

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/Math/interface/LorentzVector.h"
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
  
  LorentzVector v1(0,1,0,1);
  LorentzVector v2(0,-1,0,1);

  DiJet dj;
  dj.SetNearJet(v1);
  dj.SetAwayJet(v2);
  cout << dj << endl;
  
  // that's it!
  return 0;
}
