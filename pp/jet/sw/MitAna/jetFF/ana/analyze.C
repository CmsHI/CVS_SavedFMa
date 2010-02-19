#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "MitAna/DataTree/interface/EventHeader.h"
#include "MitAna/DataTree/interface/L1TriggerMask.h"
#include "MitAna/DataTree/interface/CaloJetCol.h"
#include "MitAna/DataTree/interface/GenJetCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/TrackCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include "MitAna/DataTree/interface/DataBaseCol.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/Utilities.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <iostream>
#include <string>
#include <vector>
// ana ntuple
#include "TreeDiJetEventData.h"

using namespace std;
using namespace mithep;
using namespace jetana;

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

  // === input ===
  TChain* tree = new TChain("Events");
  tree->Add("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSCNOBEAMHALO_000_*.root");
  //tree->Add("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSCNOBEAMHALO_000_1.root");
  
  // === ana config ===
  double etBinsArray[nEtBins] = {0,5,10,15,20,30,70,120};
  vector<double> etBins(etBinsArray,etBinsArray+nEtBins);

  string JECLevels = "L2:L3";
  //string JECTag = "900GeV_L2Relative_IC5Calo:900GeV_L3Absolute_IC5Calo";
  string JECTag = "900GeV_L2Relative_AK5Calo:900GeV_L3Absolute_AK5Calo";
  CombinedJetCorrector *JEC = new CombinedJetCorrector(JECLevels,JECTag);

  // === setup output ===
  char* outname = Form("hists.root",ifiles);
  TFile* outf = new TFile(outname,"recreate");
  TH1::SetDefaultSumw2();

  TH1D* hptTr[nEtBins+1];
  TH1D* hptGen[nEtBins+1];
  
  TTree * tree_ = new TTree("dijetTree","dijet tree");
  TreeDiJetEventData jd_;
  jd_.SetTree(tree_);
  jd_.SetBranches();

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


  // === setup branches
  mithep::Array<mithep::CaloJet> *jets;
  mithep::Array<mithep::Track> *tracks;
  mithep::Array<mithep::Vertex> *vertices;
  mithep::L1TriggerMask *L1T;
  mithep::L1TriggerMask *L1A;
  mithep::Array<mithep::MCParticle> *genparticles;
  mithep::Array<mithep::GenJet> *genjets;
  mithep::EventHeader *evInfo;

  //tree->SetBranchAddress("ItrCone5Jets",&jets);
  tree->SetBranchAddress("AKt5Jets",&jets);
  tree->SetBranchAddress("Tracks",&tracks);
  tree->SetBranchAddress("PrimaryVertexes",&vertices);
  tree->SetBranchAddress("L1TechBitsBeforeMask",&L1T);
  tree->SetBranchAddress("L1AlgoBitsBeforeMask",&L1A);
  tree->SetBranchAddress("EventHeader",&evInfo);

  /*
  if(MC){
    tree->SetBranchAddress("MCParticles",&genparticles);
    tree->SetBranchAddress("IC5GenJets",&genjets);
  }
  */

  int nevents = tree->GetEntries();
  int nevtrig = 0;
  double njet[nEtBins] = {0.,0.,0.,0.,0.,0.,0.,0.};
  double ngenjet[nEtBins] = {0.,0.,0.,0.,0.,0.,0.,0.};

  //  nevents = 50;  
  for(int iev = 0; iev < nevents; ++iev){
    tree->GetEntry(iev);

    // clear jet data
    jd_.ClearCounters();

    // trigger info
    bool A0 = L1A->Get().TestBit(0);

    // run level
    UInt_t runNum=evInfo->RunNum();
    //  - good runs -
    if (!MC &&
	runNum!=123596 &&
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

    // print out info (out of good runs) before any filters
    cout << "Run #: " << runNum << " Event: " << evInfo->EvtNum() << " Lumi: " << evInfo->LumiSec() << " PhysDeclared?: " << evInfo->IsPhysDec()
      << " L1A0?: " << A0 << endl;

    // Filter on phys declared
    if (!MC && (!evInfo->IsPhysDec() || !A0)) continue;

    // === Event Level ===
    // if no vertex nothing to do
    if(vertices->GetEntries() < 1) continue;

    // there is a vertex
    const mithep::Vertex * vtx = (Vertex*)vertices->At(0);
    bool goodVertex = vtx->Ndof()> 4 && TMath::Abs(vtx->Z() < 15.);
    cout << " EventInfo - nvtx: " << vertices->GetEntries() << " vtxndof: " << vtx->Ndof() << endl;
    //  - save event info
    jd_.run_ = runNum;
    jd_.event_ = evInfo->EvtNum();
    jd_.vtxdof_ = vtx->Ndof();
    jd_.vz_ = vtx->Z();

    if(!goodVertex) { tree_->Fill(); continue; }

    //  - got good event vertex -
    nevtrig++;

    // === Jet Level ===
    int njets = jets->GetEntries();
    int nGoodJets = 0;
    double awayEtMax=-99;
    double ptljet[2] = {-99,-99};
    for(int i = 0; i < njets; ++i){
      CaloJet* jet = (CaloJet*)jets->At(i);

      jet->DisableCorrections();
      double ptjet = jet->Pt();
      double etajet = jet->Eta();
      double phijet = jet->Phi();
      if (ptjet>3) cout << "  raw jet "<<i<<": Pt|eta|phi: " << ptjet <<"|"<< etajet << "|" << phijet << endl;

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
      //cout << " jet" << i << " Good . corr"<<correct<<"Pt|eta|phi: " << ptjet <<"|"<< etajet << "|" << phijet << endl;
    }
    
    // === Dijet Ana ===
    if (njets<2) {
      // nothing to do for dijet ana, fill some inclusive track info
      int ntracks = tracks->GetEntries();
      int nHP = 0;
      for(int it = 0; it < ntracks; ++it){
	Track* track		       = (Track*)(tracks->At(it));
	mithep::TrackQuality& quality  = track->Quality();
	bool highPurity		       = quality.QualityMask().TestBit(2);
	jd_.trkNHits_[it]	       = track->NHits();
	jd_.trkHP_[it]		       = highPurity;
	jd_.ppt_[it]		       = track->Pt();
	jd_.peta_[it]		       = track->Eta();
	jd_.pphi_[it]		       = track->Phi();
	if (highPurity) ++nHP;
      }
      jd_.evtnp_		 = ntracks;
      jd_.fracHP_		 = (double)nHP/(double)ntracks;
      tree_->Fill();
      continue;
    }

    // there are 2 jets
    CaloJet* jet0 = (CaloJet*)jets->At(0);
    CaloJet* jet1 = (CaloJet*)jets->At(1);
    // apply corrections
    jet0->DisableCorrections();
    jet1->DisableCorrections();
    double ptjet0 = jet0->Pt();
    double ptjet1 = jet1->Pt();
    double etajet0 = jet0->Eta();
    double etajet1 = jet1->Eta();
    double phijet0 = jet0->Phi();
    double phijet1 = jet1->Phi();
    FourVectorM jet0p4 = jet0->RawMom();
    FourVectorM jet1p4 = jet1->RawMom();
    if(correct) {
      double scale = JEC->getCorrection(ptjet0,etajet0,jet0->E());
      ptjet0 *= scale;
      jet0p4 *= scale;
    }
    if(correct) {
      double scale = JEC->getCorrection(ptjet1,etajet1,jet1->E());
      ptjet1 *= scale;
      jet1p4 *= scale;
    }
    // cut jet
    //bool goodDiJet = ptjet0>1 && ptjet1>1;
    //if (!goodDiJet) continue;
    cout << " jet 0 corr"<<correct<<"Pt|eta|phi: " << ptjet0 <<"|"<< etajet0 << "|" << phijet0 << " p4: " << jet0p4 << endl;
    cout << " jet 1 corr"<<correct<<"Pt|eta|phi: " << ptjet1 <<"|"<< etajet1 << "|" << phijet1 << " p4: " << jet1p4 << endl;
    double ljdphi = TMath::Abs(reco::deltaPhi(phijet0,phijet1));
    cout << "   leading jets dphi: " << ljdphi << endl;
    // -- Fill jet info --
    // fill dijet info
    jd_.jdphi_	       = ljdphi;
    jd_.mass_	       = (jet0p4+jet1p4).M();

    // near/away info
    jd_.nljet_         = ptjet0;
    jd_.nljeta_	       = etajet0;
    jd_.nljphi_        = phijet0;
    jd_.nljemf_        = jet0->EnergyFractionEm();

    jd_.aljet_         = ptjet1;
    jd_.aljeta_	       = etajet1;
    jd_.aljphi_        = phijet1;
    jd_.aljemf_        = jet1->EnergyFractionEm();

    // === Track Level ===
    int ntracks = tracks->GetEntries();
    int nHP = 0;
    int selTrkCt = 0;
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
      if (highPurity) ++nHP;

      //if(!goodTrack) continue;
      //cout << "sel track: " << selTrkCt << " pt eta phi: " << pttrack << "|" << etatrack << "|" << phitrack << endl;

      // -- Fill Tracks --
      // fill frag candidates basic info
      jd_.trkNHits_[selTrkCt]      = track->NHits();
      jd_.trkHP_[selTrkCt]      = highPurity;
      jd_.ppt_[selTrkCt]      = pttrack;
      jd_.peta_[selTrkCt]     = etatrack;
      jd_.pphi_[selTrkCt]     = phitrack;

      // Relations to jet
      jd_.pndphi_[selTrkCt]   = TMath::Abs(reco::deltaPhi(phitrack,phijet0));
      jd_.pndeta_[selTrkCt]   = etatrack - etajet0;
      jd_.pndr_[selTrkCt]     = reco::deltaR(etatrack,phitrack,etajet0,phijet0);

      jd_.padphi_[selTrkCt]   = TMath::Abs(reco::deltaPhi(phitrack,phijet1));
      jd_.padeta_[selTrkCt]   = etatrack - etajet1;
      jd_.padr_[selTrkCt]     = reco::deltaR(etatrack,phitrack,etajet1,phijet1);

      //  - background variables-
      jd_.pndrbg_[selTrkCt]	  = reco::deltaR(etatrack,phitrack,etajet0,phijet0+TMath::Pi()/2);
      jd_.padrbg_[selTrkCt]	= reco::deltaR(etatrack,phitrack,etajet1,phijet1+TMath::Pi()/2);

      // jet cone info
      if (pttrack>0.3 && pttrack<60 && highPurity && track->NHits()>=8 && jd_.pndr_[selTrkCt]<0.5) ++jd_.nljCone5NP_;

      // fragmentation variables
      jd_.zn_[selTrkCt]	= pttrack/ptjet0;
      jd_.za_[selTrkCt] = pttrack/ptjet1;

      // save counter
      ++selTrkCt;


      /*
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
      */
    } // tracks
    jd_.evtnp_		 = selTrkCt;
    jd_.fracHP_		 = (double)nHP/(double)ntracks;


    // mc input
    /*
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
    */

    // all done!
    tree_->Fill();
  } // event end

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


  /*
  if(MC){
    TCanvas* c4 = new TCanvas();
    c4->Divide(2,2);
    c4->cd(1);

    c4->cd(2);
    hFFzGen->Draw();
    c4->cd(3);
    hXiGen->Draw();
  }
  */



  /*
  for(int is = 0; is < nEtBins; ++is){
    hptTr[is]->Write();
    hptGen[is]->Write();
  }

  hDR->Write();
  hFFz->Write();
  hXi->Write();
  */

  outf->Write();
  outf->Close();

  cout<<"---------------------------------------"<<endl;
  cout<<"Total Number of Events used : "<<nevtrig<<endl;
  cout<<"---------------------------------------"<<endl;
}


