// std
#include <algorithm>
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
// ana classes
#include "PhysicsTools/PatExamples/interface/DiJet.h"
#include "PhysicsTools/PatExamples/interface/HiDiJetAlgorithm.h"
// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace std;
using namespace mystd;

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
  
  LorentzVector v1(0,2,0,2);
  LorentzVector v2(0,-1,0,1);
  LorentzVector v3(2,5,0,5.5);
  LorentzVector v4(3,3,0,4.5);

  cout << v1 << " less pt than " << v2 << "?  " << lessPt(v1,v2) << endl;
  cout << v4 << " less pt than " << v3 << "?  " << lessPt(v4,v3) << endl;

  list<LorentzVector> lv;
  lv.push_back(v1);
  lv.push_back(v2);
  lv.push_back(v3);
  lv.push_back(v4);

  cout << "list of 4vec's: " << endl;
  print_elements(lv);

  cout << "max pt vec: " << *max_element(lv.begin(),lv.end(),lessPt) << endl;

  return 0;
}
