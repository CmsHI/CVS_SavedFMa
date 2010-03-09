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
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#endif

#include "TreeDiJetEventData.h"
using namespace std;

void analyzeDiJet(){

  // event cuts
  const float hpFracCut = 0.2; // updated: 0.25
  const unsigned nTrackCut = 10;
  const double hfEThreshold = 3.0;
  const int nTowerThreshold = 1;
  const bool doCleanup = false;
  // track cuts
  const string qualityString = "highPurity";
  const double normD0Cut = 3.0;
  const double normDZCut = 3.0;
  const double ptErrCut = 0.1;
  const unsigned nHitsCut = 1; // at least this many hits on track

  //----- histograms -----
  TH1D::SetDefaultSumw2();
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

  TH1D *hTrkQual    = new TH1D("hTrkQual","track quality", 20, -0.5, 19.5);
  TH1D *hTrkDxyBeam = new TH1D("hTrkDxyBeam","track dxy from beamspot; dxy [cm]", 80, -2.0, 2.0);
  TH1D *hTrkDzVtx   = new TH1D("hTrkDzVtx","track dz from vertex; dz [cm]", 80, -2.0, 2.0);
  TH2D *hTrkPtErrNhits = new TH2D("hTrkPtErrNhits","track relative pt-error vs. nhits; number of valid hits on track; #sigma(p_{T})/p_{T}",30,-0.5,29.5,40,0.0,0.5);
  TH1D *hTrkNhits   = new TH1D("hTrkNhits", "number of valid hits on track", 30,-0.5,29.5);
  TH1D *hTrkPt      = new TH1D("hTrkPt","track p_{T}; p_{T} [GeV/c]", 80, 0.0, 20.0);
  TH1D *hTrkEta     = new TH1D("hTrkEta","track #eta; #eta", 60, -3.0, 3.0);
  TH1D *hTrkPhi     = new TH1D("hTrkPhi","track #phi; #phi [radians]", 56, -3.5, 3.5);

  //----- input files (900 GeV data) -----
  vector<string> fileNames;
  string fileDir = "/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/skim_v3";
  cout << "directory: '" << fileDir << "'" << endl;
  for(int ifile=1; ifile<=3; ifile++) {
    TString name = Form("jetAnaSkimAOD_%d.root",ifile);
    cout << "  adding file: " << name.Data() << endl;
    fileNames.push_back(fileDir + "/" + name.Data());
  }
  fwlite::ChainEvent event(fileNames);

  //----- output file -----
  TFile outFile("FFHists.root", "recreate" );
  TTree * tree_ = new TTree("trDj","dijet tree");
  jetana::TreeDiJetEventData jd_;
  jd_.SetTree(tree_);
  jd_.SetBranches();


  //----- loop over events -----
  unsigned int iEvent=0;
  int nPreSelEvt=0;
  for(event.toBegin(); !event.atEnd(); ++event, ++iEvent){

    //if( iEvent == 1000 ) break;
    if( iEvent % 5000 == 0 ) cout << "Processing " << iEvent<< "th event: "
      << "run " << event.id().run() 
	<< ", lumi " << event.luminosityBlock() 
	<< ", evt " << event.id().event() << endl;

    // fill event info
    hRunLumi->Fill(event.id().run(),event.luminosityBlock());

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
    bool goodVertex=false;
    size_t maxtracks=0; double bestvz=-999.9, bestvx=-999.9, bestvy=-999.9, bestNchi2=999.9;
    for(unsigned it=0; it<vertices->size(); ++it) {
      const reco::Vertex & vtx = (*vertices)[it];
      if(vtx.tracksSize() > maxtracks
	 || (vtx.tracksSize() == maxtracks && vtx.normalizedChi2() < bestNchi2) ) {
	maxtracks = vtx.tracksSize();
	bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
	bestNchi2 = vtx.normalizedChi2();
      }	
      if (!vtx.isFake() && vtx.ndof()>=5 && abs(vtx.z())<15) {
	goodVertex=true;
	++nPreSelEvt;
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

    if (!goodVertex) continue;

    //----- loop over leading jets ------
    fwlite::Handle<reco::CaloJetCollection> jets;
    jets.getByLabel(event,"ak5CaloJets");
    for (unsigned j=0; j<(*jets).size();++j) {
      const reco::Jet & jet = (*jets)[j];
      if (jet.pt()>7) {
	if (j==0) cout << "Event " << event.id().event() << "  # jets: " << (*jets).size() << endl;
	cout << "jet " << j << " pt|eta|phi: " << jet.pt() << "|" << jet.eta() << "|" << jet.phi() << endl;
      }
    }


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
  }
  
  cout<<"Number of events processed : "<<iEvent<<endl;
  cout<<"Number of events pre-selected : "<<nPreSelEvt<<endl;

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

}
