#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "MitAna/DataTree/interface/CaloJetCol.h"
#include "MitAna/DataTree/interface/GenJetCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/DataBaseCol.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace mithep;

void analyze(double whatever = 0){

  TChain* tree = new TChain("Events");
  tree->Add("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSCNOBEAMHALO_000_*.root");
  
  mithep::Array<mithep::CaloJet> *jets;
  mithep::Array<mithep::Track> *tracks;
  mithep::Array<mithep::Vertex> *vertices;
  mithep::DataBase *L1T;
  mithep::DataBase *L1A;
  mithep::Array<mithep::MCParticle> *genparticles;
  mithep::Array<mithep::GenJet> *genjets;

  tree->SetBranchAddress("ItrCone5Jets",&jets);
  tree->SetBranchAddress("Tracks",&tracks);
  tree->SetBranchAddress("PrimaryVertexes",&vertices);
  tree->SetBranchAddress("L1TechBitsBeforeMask",&L1T);
  tree->SetBranchAddress("L1AlgoBitsBeforeMask",&L1A);

  int nevents = tree->GetEntries();
  nevents = 2; // shorten

  for(int iev = 0; iev < nevents; ++iev){

    tree->GetEntry(iev);

  }

}


