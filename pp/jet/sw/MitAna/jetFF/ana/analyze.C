#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "MitAna/DataTree/interface/EventHeader.h"
#include "MitAna/DataTree/interface/CaloJetCol.h"
#include "MitAna/DataTree/interface/GenJetCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/DataBaseCol.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/Utilities.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace mithep;

void analyze(){

  int ifiles = 0;

  bool MC = false;
  bool correct = true;
  double drMax = 0.2;

  double ptMin = 0.3;
  double ptMax = 15;

  double jetPtMin = 7.;
  double jetEtaMax = 2.;
  double jetEMFMin = 0.01;

  const int nEtBins = 8;
  char* outname = Form("hists.root",ifiles);
  TFile* outf = new TFile(outname,"recreate");

  TChain* tree = new TChain("Events");
  //tree->Add("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSCNOBEAMHALO_000_*.root");
  tree->Add("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSCNOBEAMHALO_000_1.root");
  
  double etBinsArray[nEtBins] = {0,5,10,15,20,30,70,120};
  vector<double> etBins(etBinsArray,etBinsArray+nEtBins);

  string JECLevels = "L2:L3";
  string JECTag = "900GeV_L2Relative_IC5Calo:900GeV_L3Absolute_IC5Calo";
  CombinedJetCorrector *JEC = new CombinedJetCorrector(JECLevels,JECTag);

  TH1::SetDefaultSumw2();

  TH1D* hptTr[nEtBins+1];
  TH1D* hptGen[nEtBins+1];
  
  for(int ih = 0; ih < nEtBins+1; ++ih){
    hptTr[ih] =  new TH1D(Form("hptTr%d",ih),Form("Tracks in cone |#Delta R| < %f; p_{T}^{track}; N_{track}/N_{jet}",drMax),250,ptMin,ptMax);
    hptGen[ih] =  new TH1D(Form("hptGen%d",ih),Form("Charged Particles in cone |#Delta R| < %f; p_{T}^{chg}; N_{chg}/N_{jet}",drMax),250,ptMin,ptMax);
  }

  TH1D* hptJet = new TH1D("hptJet",";p_{T} [GeV];jets",100,0,20);
  TH2D* hptCor = new TH2D("hptCor",";p_{T}^{raw} [GeV];p_{T}^{corrected} [GeV]",100,0,20,100,0,20);
  TH1D* hDR = new TH1D("hDR",";#Delta R;tracks",100,0,3.2);
  TH1D* hFFz = new TH1D("hFFz","",100,0,1.2);
  TH1D* hXi = new TH1D("hXi","",100,-2,8);
  TH1D* hFFzGen = new TH1D("hFFzGen","",100,0,1.2);
  TH1D* hXiGen = new TH1D("hXiGen","",100,-2,8);


  mithep::Array<mithep::CaloJet> *jets;
  mithep::Array<mithep::Track> *tracks;
  mithep::Array<mithep::Vertex> *vertices;
  mithep::DataBase *L1T;
  mithep::DataBase *L1A;
  mithep::Array<mithep::MCParticle> *genparticles;
  mithep::Array<mithep::GenJet> *genjets;
  mithep::EventHeader *evInfo;

  tree->SetBranchAddress("ItrCone5Jets",&jets);
  tree->SetBranchAddress("Tracks",&tracks);
  tree->SetBranchAddress("PrimaryVertexes",&vertices);
  tree->SetBranchAddress("L1TechBitsBeforeMask",&L1T);
  tree->SetBranchAddress("L1AlgoBitsBeforeMask",&L1A);
  tree->SetBranchAddress("EventHeader",&evInfo);

  if(MC){
    tree->SetBranchAddress("MCParticles",&genparticles);
    tree->SetBranchAddress("IC5GenJets",&genjets);
  }

  int nevents = tree->GetEntries();
  int nevtrig = 0;
  double njet[nEtBins] = {0.,0.,0.,0.,0.,0.,0.,0.};
  double ngenjet[nEtBins] = {0.,0.,0.,0.,0.,0.,0.,0.};

  //  nevents = 50;  
  for(int iev = 0; iev < nevents; ++iev){

    tree->GetEntry(iev);
    bool T36 = L1T->TestBit(36);
    bool T37 = L1T->TestBit(37);
    bool T38 = L1T->TestBit(38);
    bool T39 = L1T->TestBit(39);
    bool haloVeto = !(T36 || T37 || T38 || T39);
    bool T40 = L1T->TestBit(40);
    bool T41 = L1T->TestBit(41);
    bool A0 = L1A->TestBit(40);
    bool A82 = L1A->TestBit(82);

    // run level
    UInt_t runNum=evInfo->RunNum();
    //  - good runs -
    if (runNum!=123596 &&
	runNum!=123615 &&
	runNum!=123732 &&
	runNum!=123815 &&
	runNum!=123818 &&
	runNum!=123906 &&
	runNum!=123908 &&
	runNum!=124008 &&
	runNum!=124009 &&
	runNum!=124020 &&
	runNum!=124022 &&
	runNum!=124023 &&
	runNum!=124024 &&
	runNum!=124025 &&
	runNum!=124027 &&
	runNum!=124030 
       )
      continue;

    // === Event Level ===
    if(vertices->GetEntries() < 1) continue;
    const mithep::Vertex * vtx = (Vertex*)vertices->At(0);
    bool goodVertex = vtx->Ndof()> 5 && TMath::Abs(vtx->Z() < 15.);
    cout << "Run #: " << runNum << " Event: " << evInfo->EvtNum() << " Lumi: " << evInfo->LumiSec() << endl;

    if(!goodVertex) continue;

    //  - got good event vertex -
    nevtrig++;

    // === Jet Level ===
    int njets = jets->GetEntries();
    for(int i = 0; i < njets; ++i){
      CaloJet* jet = (CaloJet*)jets->At(i);

      jet->DisableCorrections();
      double ptjet = jet->Pt();
      double etajet = jet->Eta();
      double phijet = jet->Phi();
      cout << "  jet "<<i<<": Pt|eta|phi: " << ptjet <<"|"<< etajet << "|" << phijet << endl;

      double emf = jet->EnergyFractionEm();

      bool goodJet = TMath::Abs(etajet) < jetEtaMax && emf > jetEMFMin;
      if(!goodJet) continue;

      double energy = jet->E();

      int jetbin = getBin(ptjet,etBins)+1;
      njet[jetbin] += 1.;

      hptJet->Fill(ptjet);


      double corpt = ptjet*JEC->getCorrection(ptjet,etajet,energy);
      hptCor->Fill(ptjet,corpt);

      if(correct) ptjet = corpt;

      if(ptjet< jetPtMin) continue;
      cout << "Good jet. corr"<<correct<<"Pt|eta|phi: " << ptjet <<"|"<< etajet << "|" << phijet << endl;

      int ntracks = tracks->GetEntries();
      for(int j = 0; j < ntracks; ++j){
	Track* track = (Track*)(tracks->At(j));

	double pttrack = track->Pt();
        double etatrack = track->Eta();
        double phitrack = track->Phi();


	mithep::TrackQuality& quality = track->Quality();

	if(0){
	  double d0err = track->D0Err();
	  double sigXY = sqrt(d0err*d0err + 0.04*0.04);
	  double qope = track->QOverPErr();
	  double lambdaerr = track->LambdaErr();
	  double pz = track->Pz();
	  int chg = track->Charge();
	}
	
	bool highPurity = quality.QualityMask().TestBit(2);
	//cout << "highPurity?: " << quality.QualityMask().TestBit(2) << endl;
	bool goodTrack = highPurity;

	if(!goodTrack) continue;

	double dr = reco::deltaR(etatrack,phitrack,etajet,phijet);
	hDR->Fill(dr);

	if(dr > drMax) continue;

	hptTr[0]->Fill(pttrack);
        hptTr[jetbin]->Fill(pttrack);

	if(pttrack < ptMin) continue;

	double ffz = pttrack/ptjet;
	double xi = -log(ffz);

        hFFz->Fill(ffz);
	hXi->Fill(xi);
      }

    }

      if(MC){
	int ngenjets = genjets->GetEntries();
	for(int i = 0; i < ngenjets; ++i){
	  GenJet* jet = (GenJet*)genjets->At(i);

	  double ptjet = jet->Pt();
	  double etajet = jet->Eta();
	  double phijet = jet->Phi();

	  double energy = jet->E();

	  int jetbin = getBin(ptjet,etBins)+1;
	  ngenjet[jetbin] += 1.;

	  int nparticles = genparticles->GetEntries();
	  for(int j = 0; j < nparticles; ++j){
	    MCParticle* p = (MCParticle*)(genparticles->At(j));

	    int chg = p->Charge();
	    
	    if(chg == 0 || chg < -10) continue;
	    
	    double ptpar = p->Pt();
	    double etapar = p->Eta();
	    double phipar = p->Phi();
	    
	    double dr = reco::deltaR(etapar,phipar,etapar,phipar);
	    
	    if(dr > drMax) continue;

	    if(ptpar < ptMin) continue;
	    double ffz = ptpar/ptjet;
	    double xi = -log(ffz);
	    hFFzGen->Fill(ffz);
	    hXiGen->Fill(xi);
	    
	    
	  }
	}

      }

  }

  hptJet->Draw();

  TCanvas* c2 = new TCanvas();
  hptCor->Draw("colz");

  TCanvas* c3 = new TCanvas();
  c3->Divide(2,2);
  c3->cd(1);
  hDR->Draw();

  c3->cd(2);
  hFFz->Draw();
  c3->cd(3);
  hXi->Draw();


  if(MC){
    TCanvas* c4 = new TCanvas();
    c4->Divide(2,2);
    c4->cd(1);

    c4->cd(2);
    hFFzGen->Draw();
    c4->cd(3);
    hXiGen->Draw();
  }



  for(int is = 0; is < nEtBins; ++is){
    hptTr[is]->Write();
    hptGen[is]->Write();
  }

  hDR->Write();
  hFFz->Write();
  hXi->Write();

  //  outf->Write();

  cout<<"---------------------------------------"<<endl;
  cout<<"Total Number of Events used : "<<nevtrig<<endl;
  cout<<"---------------------------------------"<<endl;


}


