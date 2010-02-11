#include "TClonesArray.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#include "MitAna/DataTree/interface/CaloJetCol.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/Utilities.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

using namespace mithep;

void bambu(){

  TFile* inf = new TFile("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSCNOBEAMHALO_000_1.root");
  TTree* tree = (TTree*)inf->Get("Events");

  string JECLevels = "L2:L3";
  string JECTag = "900GeV_L2Relative_AK5Calo:900GeV_L3Absolute_AK5Calo";

  CombinedJetCorrector *JEC = new CombinedJetCorrector(JECLevels,JECTag);

  mithep::Array<mithep::CaloJet> *jetarray;

  TH1D* hpt = new TH1D("hpt",";p_{T} [GeV];jets",100,0,20);
  TH2D* hptCor = new TH2D("hptCor",";p_{T}^{raw} [GeV];p_{T}^{corrected} [GeV]",100,0,20,100,0,20);
  
  tree->SetBranchAddress("ItrCone5Jets",&jetarray);
  tree->SetBranchStatus("ItrCone5Jets",1);
  tree->SetBranchStatus("PixelTracks",0);
  tree->SetBranchStatus("PixelLessTracks",0);
  tree->SetBranchStatus("ConversionOutInTracks",0);
  tree->SetBranchStatus("GsfTracks",0);
  tree->SetBranchStatus("ConversionInOutTracks",0);
  tree->SetBranchStatus("StandaloneMuonTracksWVtxConstraint",0);
  tree->SetBranchStatus("StandaloneMuonTracks",0);
  tree->SetBranchStatus("GlobalMuonTracks",0);

  int nevents = tree->GetEntries();
  
  for(int iev = 0; iev < nevents; ++iev){

    cout<<"a"<<endl;
    tree->GetEntry(iev);

    int njets = jetarray->GetEntries();
    cout<<"N jets : "<<njets<<endl;
    
    for(int i = 0; i < njets; ++i){

      cout<<"i "<<i<<endl;

      CaloJet* jet = (CaloJet*)jetarray->At(i);
      jet->DisableCorrections();
      double pt = jet->Pt();

      double eta = jet->Eta();
      double energy = jet->E();

      hpt->Fill(pt);

      double corpt = pt*JEC->getCorrection(pt,eta,energy);
      hptCor->Fill(pt,corpt);

    }

  }

  hpt->Draw();

  TCanvas* c2 = new TCanvas();
  hptCor->Draw("colz");


}


