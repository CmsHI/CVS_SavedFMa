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
  InputItem v0(500,0,0,500);
  double a1=3.14*0.5;
  double pt1=100;
  double x1=pt1*cos(a1);
  double y1=pt1*sin(a1);
  double a2=3.14-0.6;
  double pt2=120;
  double x2=pt2*cos(a2);
  double y2=pt2*sin(a2);
  double a3=3.14-0.4;
  double pt3=200;
  double x3=pt3*cos(a3);
  double y3=pt3*sin(a3);
  double a4=-1*(3.14-0.2);
  double pt4=300;
  double x4=pt4*cos(a4);
  double y4=pt4*sin(a4);
  double E=300;
  InputItem v1(x1,y1,-50,E);
  InputItem v2(x2,y2,0,E);
  InputItem v3(x3,y3,-10,E);
  InputItem v4(x4,y4,30,E);

  InputCollection lv;
  lv.push_back(v1);
  lv.push_back(v2);
  lv.push_back(v3);
//  lv.push_back(v4);

  // test pair algo
  HiDiJetAlgorithm djalgo;
  djalgo.SetVerbosity(2);
  InputCollection::const_iterator result = djalgo.FindPair(v0,lv);
  if (result!=NULL)
    cout << "given near: " << v0 << "  found away: " << *result << endl;
  else
    cout << "no away jet that passed away side criteron found" << endl;
  
  // test group algo
  lv.push_back(v0);
  OutputCollection output;
  djalgo.Group(lv,&output);
  cout << "grouped dijets:" << endl;
  print_elements(output);
  return 0;
}
