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
  
  // prepare inputs
  InputItem v0(200,0,0,200);
  double a1=3.14*0.5;
  double pt1=100;
  double x1=pt1*cos(a1);
  double y1=pt1*sin(a1);
  double a2=3.14-0.6;
  double x2=pt1*cos(a2);
  double y2=pt1*sin(a2);
  double a3=3.14-0.4;
  double x3=pt1*cos(a3);
  double y3=pt1*sin(a3);
  double a4=-1*(3.14-0.2);
  double x4=pt1*cos(a4);
  double y4=pt1*sin(a4);
  double pt2=300;
  InputItem v1(x1,y1,-50,pt2);
  InputItem v2(x2,y2,0,pt2);
  InputItem v3(x3,y3,-10,pt2);
  InputItem v4(x4,y4,30,pt2);

  InputCollection lv;
  lv.push_back(v1);
  lv.push_back(v2);
  lv.push_back(v3);
  lv.push_back(v4);
  cout << "list of 4vec's: " << endl;
  print_elements(lv);

  // test algo
  HiDiJetAlgorithm djalgo;
  djalgo.SetVerbosity(2);
  InputCollection::const_iterator result = djalgo.FindPair(v0,lv);
  if (result!=NULL)
    cout << "given near: " << v0 << "  found away: " << *result << endl;
  else
    cout << "no away jet that passed away side criteron found" << endl;
  
  return 0;
}
