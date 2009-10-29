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
  //  * configure ana
  //  * book the histograms of interest 
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // prepare inputs
  double pt0=500;
  // test no leading case:
  //pt0=10;
  AnaInputItem v0(pt0,0,0,pt0);
  double a1=3.14*0.5;
  double pt1=pt0*0.2;
  double x1=pt1*cos(a1);
  double y1=pt1*sin(a1);
  double a2=3.14-0.6;
  double pt2=pt0*0.3;
  double x2=pt2*cos(a2);
  double y2=pt2*sin(a2);
  double a3=3.14-0.4;
  double pt3=pt0*0.4;
  double x3=pt3*cos(a3);
  double y3=pt3*sin(a3);
  double a4=-1*(3.14-0.2);
  double pt4=pt0*0.6;
  double x4=pt4*cos(a4);
  double y4=pt4*sin(a4);
  double E=pt0;
  AnaInputItem v1(x1,y1,-50,E);
  AnaInputItem v2(x2,y2,0,E);
  AnaInputItem v3(x3,y3,-10,E);
  AnaInputItem v4(x4,y4,30,E);
  AnaInputItem va(3,0,0,5);
  AnaInputItem vb(2,3,0,7);
  AnaInputItem vc(1,-3,0,7);

  DiJet dj1;
  dj1.SetNearJet(v1);
  dj1.SetAwayJet(v4);
  DiJet dj2;
  dj2.SetNearJet(v2);
  dj2.SetAwayJet(v3);

  AnaDiJetCollection dijets;
  dijets.push_back(dj1);
  dijets.push_back(dj2);

  AnaInputCollection tracks;
  tracks.push_back(va);
  tracks.push_back(vb);

  // dijet ana tree data
  HiDiJetAnalysis ana;
  ana.SetVerbosity(3);

  // test helpers
  cout << "===test dijet calc===" << endl;
  ana.CalcJetVars(dj1);
  cout << endl << "===test frag calc===" << endl;
  ana.CalcFragVars(dj1,tracks);

  // test main methods
  cout << endl << "===test fill ===" << endl;
  ana.Fill(dijets,tracks);

  // save
  TFile outf("dijetanadata_driver.root","RECREATE","dijet ana data");
  ana.tree_->Write();
  outf.Close();
  // that's it!
  return 0;
}
