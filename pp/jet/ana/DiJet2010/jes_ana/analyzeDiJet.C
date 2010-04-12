// Usage:
// .x rootlogon.C
// .x analyzeTracks.C++

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "math.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#if !defined(__CINT__) && !defined(__MAKECINT__)

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#endif

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TreeDiJetEventData.h"
using namespace std;

void analyzeDiJet(int doMC=1, int verbosity=1){
  // === Start Ana Macro ===
  TStopwatch sw;

  // event cuts
  const float hpFracCut = 0.2; // updated: 0.25
  const unsigned nTrackCut = 10;
  const bool doCleanup = (doMC==0);
  // track cuts
  const string qualityString = "highPurity";
  const double normD0Cut = 3.0;
  const double normDZCut = 3.0;
  const double ptErrCut = 0.1;
  const unsigned nHitsCut = 1; // at least this many hits on track

  // JEC
  string JECLevels = "L2:L3";
  string JECTag = "Summer09_7TeV_ReReco332_L2Relative_AK5Calo:Summer09_7TeV_ReReco332_L3Absolute_AK5Calo";
  CombinedJetCorrector *JEC = new CombinedJetCorrector(JECLevels,JECTag);
  bool doJEC = false;

  //----- histograms -----
  TH1D::SetDefaultSumw2();
  // Event
  TH2D *hRunLumi    = new TH2D("hRunLumi","Event information; run number; luminosity block",500,123549.5,124049.5,200,-0.5,199.5);
  TH1D *hL1TechBits = new TH1D("hL1TechBits","L1 technical trigger bits before mask",64,-0.5,63.5);
  TH2D *hHPFracNtrk = new TH2D("hHPFracNtrk","High purity fraction vs. # of tracks; number of tracks; highPurity fraction",50,0,500,50,0,1);
  TH2D *hHfTowers   = new TH2D("hHfTowers","Number of HF tower above threshold; positive side; negative side",80,-0.5,79.5,80,-0.5,79.5);

  TH1D *hVtxSize    = new TH1D("hVtxSize","number of reconstructed pixel vertices",10,-0.5,9.5);
  TH1D *hVtxTrks    = new TH1D("hVtxTrks","number of tracks used to fit pixel vertex",50,-0.5,49.5);
  TH1D *hVtxZ       = new TH1D("hVtxZ","z position of best reconstructed pixel vertex", 80,-20,20);
  TH2D *hBeamXRun   = new TH2D("hBeamXRun","x position of beamspot; run number",500,123549.5,124049.5,80,-0.2,0.2);
  TH2D *hBeamYRun   = new TH2D("hBeamYRun","y position of beamspot; run number",500,123549.5,124049.5,80,-0.2,0.2);
  TH2D *hBeamZRun   = new TH2D("hBeamZRun","z position of beamspot; run number",500,123549.5,124049.5,80,-2,2);

  // inclusive jet
  TH1D *hJetEt	    = new TH1D("hJetEt","inclusive jet Et; jet E_{T} [GeV]",16,0,80);
  TH1D *hJetEta	    = new TH1D("hJetEta","inclusive jet Eta; jet #eta",20,-3,3);
  TH1D *hJetPhi	    = new TH1D("hJetPhi","inclusive jet Phi; jet #phi",10,-3.14,3.14);

  // dijet
  TH1D *hDJJetEt	    = new TH1D("hDJJetEt",";j^{1,2}Et [GeV];",16,0,80);
  TH1D *hDJJetEta	    = new TH1D("hDJJetEta",";j^{1,2} Eta;",20,-3,3);
  TH1D *hDJJetPhi	    = new TH1D("hDJJetPhi",";j^{1,2} Phi;",10,-3.14,3.14);
  TH1D *hDJDphi	  	    = new TH1D("hDJDphi",";dijet d #phi;",30,0,3.14);
  TH1D *hDJMass	  	    = new TH1D("hDJMass",";dijet mass [GeV];",30,0,125);

  // tracks
  TH1D *hTrkQual    = new TH1D("hTrkQual","track quality", 20, -0.5, 19.5);
  TH1D *hTrkDxyBeam = new TH1D("hTrkDxyBeam","track dxy from beamspot; dxy [cm]", 80, -2.0, 2.0);
  TH1D *hTrkDzVtx   = new TH1D("hTrkDzVtx","track dz from vertex; dz [cm]", 80, -2.0, 2.0);
  TH2D *hTrkPtErrNhits = new TH2D("hTrkPtErrNhits","track relative pt-error vs. nhits; number of valid hits on track; #sigma(p_{T})/p_{T}",30,-0.5,29.5,40,0.0,0.5);
  TH1D *hTrkNhits   = new TH1D("hTrkNhits", "number of valid hits on track", 30,-0.5,29.5);
  TH1D *hTrkPt      = new TH1D("hTrkPt","track p_{T}; p_{T} [GeV/c]", 80, 0.0, 20.0);
  TH1D *hTrkEta     = new TH1D("hTrkEta","track #eta; #eta", 60, -3.0, 3.0);
  TH1D *hTrkPhi     = new TH1D("hTrkPhi","track #phi; #phi [radians]", 56, -3.5, 3.5);

  //----- input files  -----
  vector<string> fileNames;
  string fileDir = "/d01/frankma/scratch/HI/jet/pppatCMSSW_3_5_4_patch1/outputs/";
  cout << "directory: '" << fileDir << "'" << endl;
  for(int ifile=1; ifile<=1; ifile++) {
    TString name = Form("jetSkimRelValQCD_Pt_80_120_sw354-JEC_Summer09_7TeV.root");
    cout << "  adding file: " << name.Data() << endl;
    fileNames.push_back(fileDir + "/" + name.Data());
  }
  fwlite::ChainEvent event(fileNames);

  //----- output file -----
  TFile outFile("jes_ana.root", "recreate" );
  TTree * dijetTree = new TTree("djtree","dijet tree");
  jetana::TreeDiJetEventData jd_;
  jd_.SetTree(dijetTree);
  jd_.SetBranches();


  //----- loop over events -----
  unsigned int iEvent=0;
  int nPreSelEvt=0;
  int nDJEvt=0;
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){
    // clear event data class
    jd_.Clear();

    if( iEvent % 1000 == 0 ) cout << "Processing " << iEvent<< "th event: "
      << "run " << event.id().run() 
	<< ", lumi " << event.luminosityBlock() 
	<< ", evt " << event.id().event() << endl;

    // fill event info
    hRunLumi->Fill(event.id().run(),event.luminosityBlock());

    //
    // === Event Preselection ===
    //
    // select on high-purity track fraction
    fwlite::Handle<std::vector<reco::Track> > tracks;
    tracks.getByLabel(event, "generalTracks");
    if (doCleanup) {
      int numhighpurity = 0;
      float fraction = 0;
      for(unsigned it=0; it<tracks->size(); ++it)
	if((*tracks)[it].quality(reco::TrackBase::qualityByName(qualityString))) numhighpurity++;
      if(tracks->size() > 0) fraction = (float)numhighpurity/(float)tracks->size();
      hHPFracNtrk->Fill(tracks->size(),fraction);
      if(fraction<hpFracCut && tracks->size()>nTrackCut) continue;
    }

    // select on requirement of valid vertex
    fwlite::Handle<std::vector<reco::Vertex> > vertices;
    vertices.getByLabel(event, "offlinePrimaryVertices");
    hVtxSize->Fill(vertices->size());
    if(!vertices->size()) continue;
    Int_t maxtracks=0;
    double bestndof=-999.9,bestvz=-999.9, bestvx=-999.9, bestvy=-999.9, bestNchi2=999.9;
    for(unsigned it=0; it<vertices->size(); ++it) {
      const reco::Vertex & vtx = (*vertices)[it];
      if(vtx.tracksSize() > maxtracks
	 || (vtx.tracksSize() == maxtracks && vtx.normalizedChi2() < bestNchi2) ) {
	maxtracks = vtx.tracksSize();
	bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
	bestNchi2 = vtx.normalizedChi2();
	bestndof = vtx.ndof();
      }	
    }
    hVtxTrks->Fill(maxtracks);
    hVtxZ->Fill(bestvz);

    // get beamspot
    fwlite::Handle<reco::BeamSpot> beamspot;
    beamspot.getByLabel(event, "offlineBeamSpot");
    hBeamXRun->Fill(event.id().run(),beamspot->x0());
    hBeamYRun->Fill(event.id().run(),beamspot->y0());
    hBeamZRun->Fill(event.id().run(),beamspot->z0());

    // Do Vertex Selection
    if (bestndof>=5 && abs(bestvz)<15) ++nPreSelEvt;
    else continue;

    // Done with Event Pre-Selection
    // Fill Event info
    jd_.run_ = event.id().run();
    jd_.lumi_ = event.luminosityBlock();
    jd_.evt_ = event.id().event();
    // Fill vtx info
    jd_.nvtx_ = vertices->size();
    jd_.vtxntrks_ = maxtracks;
    jd_.vtxndof_ = bestndof;
    jd_.vtxchi2_ = bestNchi2;
    jd_.vz_ = bestvz;

    //
    // ===== Inclusive Jet Ana =====
    //
    fwlite::Handle<vector<pat::Jet> > jets;
    jets.getByLabel(event,"patJets");
    Double_t   NearEtMax=-99;
    Int_t      iNear=-99;
    std::vector<math::PtEtaPhiMLorentzVectorF> anajets;
    // find leading jet based on corrected pt
    for (unsigned int j=0; j<(*jets).size();++j) {
      const pat::Jet & jet = (*jets)[j];
      // apply JEC
      Double_t corrPt = jet.pt();
      if (doJEC) corrPt *= JEC->getCorrection(jet.pt(),jet.eta(),jet.energy());
      if (corrPt>NearEtMax) {
	NearEtMax=corrPt;
	iNear=j;
      }
      if (corrPt>10) {
	hJetEt->Fill(corrPt);
	hJetEta->Fill(jet.eta());
	hJetPhi->Fill(jet.phi());
      }
    }

    //
    // ===== Near Jet Ana =====
    //
    if (NearEtMax>0) {
      const pat::Jet & NrJet = (*jets)[iNear];
      anajets.push_back(math::PtEtaPhiMLorentzVectorF(NrJet.pt(),NrJet.eta(),NrJet.phi(),NrJet.mass()));
      if (doJEC) anajets[0] *= JEC->getCorrection(NrJet.pt(),NrJet.eta(),NrJet.energy());
    }

    //
    // ===== Away Jet Ana =====
    //
    Double_t   AwayEtMax=-99;
    Int_t      iAway=-99;
    for (unsigned j=0; j<(*jets).size();++j) {
      const pat::Jet & jet = (*jets)[j];
      // look at away side
      Double_t jdphi = TMath::Abs(reco::deltaPhi(anajets[0].phi(),jet.phi()));
      if (jdphi < TMath::PiOver2()) continue;
      // apply JEC
      Double_t corrPt = jet.pt();
      if (doJEC) corrPt *= JEC->getCorrection(jet.pt(),jet.eta(),jet.energy());
      if (corrPt>AwayEtMax) {
	AwayEtMax=corrPt;
	iAway=j;
      }
    }
    if (AwayEtMax>0) {
      const pat::Jet & AwJet = (*jets)[iAway];
      anajets.push_back(math::PtEtaPhiMLorentzVectorF(AwJet.pt(),AwJet.eta(),AwJet.phi(),AwJet.mass()));
      if (doJEC) anajets[1] *= JEC->getCorrection(AwJet.pt(),AwJet.eta(),AwJet.energy());
    }

    // === dijet kinematics selection ===
    if (NearEtMax<5 || AwayEtMax<5) continue;
    ++nDJEvt;

    // -- Fill jet info --
    Double_t ljdphi = TMath::Abs(reco::deltaPhi(anajets[0].phi(),anajets[1].phi()));
    // fill dijet info
    jd_.jdphi_          = ljdphi;
    jd_.mass_	       = (anajets[0]+anajets[1]).M();

    // near/away info
    jd_.nljet_		= anajets[0].pt();
    jd_.nljeta_		= anajets[0].eta();
    jd_.nljphi_		= anajets[0].phi();
    jd_.nljemf_		= (*jets)[iNear].emEnergyFraction();

    jd_.aljet_		= anajets[1].pt();
    jd_.aljeta_		= anajets[1].eta();
    jd_.aljphi_		= anajets[1].phi();
    jd_.aljemf_		= (*jets)[iAway].emEnergyFraction();

    // print
    if (verbosity>=1 && nDJEvt<=5) {
      cout << "Event " << event.id().event()
	<< ", lumi " << event.luminosityBlock() 
	<< ", evt " << event.id().event()
	<< ",  # jets: " << (*jets).size() << endl;
      for (unsigned j=0; j<(*jets).size();++j) {
	const pat::Jet & jet = (*jets)[j];
	cout << "jet " << j << " pt|eta|phi: " << jet.pt() << "|" << jet.eta() << "|" << jet.phi() << endl;
      }
      cout << "corr" << doJEC << " leading dijet - iNear: " << iNear << " " <<": "<< anajets[0]
	<< "  iAway: " << iAway << " " << anajets[1] << endl;
      cout << "DiJet dphi: " << ljdphi << endl;
      cout << endl;
    }

    /*
    //----- loop over tracks -----
    for(unsigned it=0; it<tracks->size(); ++it){
      
      const reco::Track & trk = (*tracks)[it];
      
      // select high purity tracks
      hTrkQual->Fill(trk.qualityMask());
      if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;

      // select tracks based on transverse proximity to beamspot
      double dxybeam = trk.dxy(beamspot->position());
      hTrkDxyBeam->Fill(dxybeam);
      if(fabs(dxybeam/trk.d0Error()) > normD0Cut) continue;

      // select tracks based on z-proximity to best vertex 
      math::XYZPoint bestvtx(bestvx,bestvy,bestvz);
      double dzvtx = trk.dz(bestvtx);
      hTrkDzVtx->Fill(dzvtx);
      if(fabs(dzvtx/trk.dzError()) > normDZCut) continue;

      // select tracks based on relative pt error
      double pterr = trk.ptError()/trk.pt();
      unsigned nhits = trk.numberOfValidHits();
      hTrkPtErrNhits->Fill(nhits,pterr);
      if(pterr > ptErrCut) continue;

      // select tracks based on number of valid rechits
      hTrkNhits->Fill(nhits);
      if(nhits < nHitsCut) continue;

      // fill selected track histograms
      hTrkPt->Fill(trk.pt());
      hTrkEta->Fill(trk.eta());
      hTrkPhi->Fill(trk.phi());

    }
    */

    // done with event write tree content
    dijetTree->Fill();
  }
  
  cout << endl << "================ Ana Process Summaries =============" << endl;
  cout << "Number of events processed : "<<iEvent<<endl;
  cout << "Number of events pre-selected : "<<nPreSelEvt<<endl;
  cout << "Number of dijet pre-selected : "<<nDJEvt<<endl;

  // write to output file
  outFile.Write();
  outFile.cd(); outFile.mkdir("evt"); outFile.cd("evt");
  hRunLumi->Write();
  hL1TechBits->Write();
  hHPFracNtrk->Write();
  hHfTowers->Write();

  outFile.cd(); outFile.mkdir("vtx"); outFile.cd("vtx");
  hVtxSize->Write();
  hVtxTrks->Write();
  hVtxZ->Write();
  hBeamXRun->Write();
  hBeamYRun->Write();
  hBeamZRun->Write();

  outFile.cd(); outFile.mkdir("jet"); outFile.cd("jet");
  hJetEt->Write();
  hJetEta->Write();
  hJetPhi->Write();
  hDJJetEt->Write();
  hDJJetEta->Write();
  hDJJetPhi->Write();
  hDJDphi->Write();
  hDJMass->Write();


  outFile.cd(); outFile.mkdir("trk"); outFile.cd("trk");
  hTrkQual->Write();
  hTrkDxyBeam->Write();
  hTrkDzVtx->Write();
  hTrkPtErrNhits->Write();
  hTrkNhits->Write();
  hTrkPt->Write();
  hTrkEta->Write();
  hTrkPhi->Write();
  
  outFile.ls();
  outFile.Close();

  // free allocated space
  delete hRunLumi;
  delete hL1TechBits; 
  delete hHPFracNtrk;
  delete hHfTowers;

  delete hVtxSize;
  delete hVtxTrks;
  delete hVtxZ;
  delete hBeamXRun;
  delete hBeamYRun;
  delete hBeamZRun;

  delete hTrkQual; 
  delete hTrkDxyBeam;
  delete hTrkDzVtx;
  delete hTrkPtErrNhits; 
  delete hTrkNhits;
  delete hTrkPt;
  delete hTrkEta;
  delete hTrkPhi;

  // All done
  sw.Stop();
  cout << "Processing time --> "  << flush; sw.Print();
  cout << "====================================================" << endl;
}
