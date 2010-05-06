// -*- C++ -*-
//
// Package:    DiJetAna
// Class:      DiJetAna
// 
/**\class DiJetAna DiJetAna.cc ana/DiJetAna/src/DiJetAna.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
 */
//
// Original Author:  Frank Ma,32 4-A06,+41227676980,
//         Created:  Thu May  6 10:29:52 CEST 2010
// $Id: DiJetAna.cc,v 1.12 2010/05/06 17:10:20 frankma Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// ana
#include "CmsHi/DiJetAna/interface/DiJetAna.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include "TStopwatch.h"

#include "CmsHi/DiJetAna/interface/TreeDiJetEventData.h"


using namespace std;
using namespace edm;
using namespace reco;

//
// constructors and destructor
//
DiJetAna::DiJetAna(const edm::ParameterSet& iConfig) :
  numPreEvtSel_(0),
  numDJEvtSel_(0)
{
  //now do what ever initialization is needed
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC", true);
  vtxsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vtxsrc",edm::InputTag("hiSelectedVertex"));
  jetsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsrc",edm::InputTag("akPu5patJets"));
  trksrc_ = iConfig.getUntrackedParameter<edm::InputTag>("trksrc",edm::InputTag("hiSelectTracks"));
  jetEtaMax_ = iConfig.getUntrackedParameter<double>("jetEtaMax", 2.0);
  nVtxTrkCut_ = iConfig.getUntrackedParameter<int>("nVtxTrkCut", 3);
  doJEC_ = iConfig.getUntrackedParameter<int>("doJEC", 3);
  nearJetPtMin_ = iConfig.getUntrackedParameter<double>("nearJetPtMin", 50);
  awayJetPtMin_ = iConfig.getUntrackedParameter<double>("awayJetPtMin", 50);
  anaJetType_ = iConfig.getUntrackedParameter<int>("anaJetType", 2);
  refJetType_ = iConfig.getUntrackedParameter<int>("refJetType", 1);
}


DiJetAna::~DiJetAna()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
  void
DiJetAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  const int nTrigs = 5;
  const string qualityString = "highPurity";
  calojData_.Clear();
  calojGenjData_.Clear();
  calojPtnjData_.Clear();
  genjCalojData_.Clear();
  ptnjCalojData_.Clear();

  //-----------------------  Preselection (This part will be in an EDFilter later)  
  // get vtx collection 
  Handle<vector<Vertex> > vertices;
  iEvent.getByLabel(vtxsrc_, vertices);
  Int_t numVtx = (Int_t)vertices->size();
  hNumVtx_->Fill(numVtx);
  if(numVtx<1) return; // at least one vtx

  Int_t numFake=0, maxtracks=0;
  double bestndof=-999.9,bestvz=-999.9, bestvx=-999.9, bestvy=-999.9, bestNchi2=999.9;
  for(UInt_t it=0; it<vertices->size(); ++it) {
    const reco::Vertex & vtx = (*vertices)[it];
    // check if valid vertex
    if(vtx.isFake()) {
      ++numFake;
      continue;
    }
    // update best vertex
    if(numVtx > maxtracks || (numVtx == maxtracks && vtx.normalizedChi2() < bestNchi2) ) {
      maxtracks = vtx.tracksSize();
      bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
      bestNchi2 = vtx.normalizedChi2();
      bestndof = vtx.ndof();
    } 
  }
  hVtxNumTrksPreSel_->Fill(maxtracks);
  if(maxtracks<nVtxTrkCut_) return; // vtx quality selection
  hVtxNumTrksEvtSel_->Fill(maxtracks);
  hVtxZEvtSel_->Fill(bestvz);
  ++numPreEvtSel_;

  // Done with Event Pre-Selection
  // Fill Event info

  //
  // ===== Inclusive Jet Ana =====
  //
  Handle<vector<pat::Jet> > jets;
  iEvent.getByLabel(jetsrc_,jets);
  // find leading jet based on corrected pt
  for (unsigned int j=0; j<(*jets).size();++j) {
    const pat::Jet & jet = (*jets)[j];
    Double_t corrPt=-99;
    if (doJEC_==3) corrPt = jet.pt();
    hJetPtPreSel_->Fill(corrPt);
    hJetEtaPreSel_->Fill(jet.eta());
    hJetPhiPreSel_->Fill(jet.phi());
  }

  //
  // ===== DiJet Ana =====
  //
  anaJets_.clear();
  iNear_ = FindNearJet(iEvent,2);
  if (iNear_<0) return;

  const pat::Jet & NrJet = (*jets)[iNear_];
  nearJetPt_ = -99;
  if (doJEC_==3) nearJetPt_ = (*jets)[iNear_].correctedP4("abs").pt();
  if (doJEC_==5) nearJetPt_ = (*jets)[iNear_].correctedP4("had","uds").pt();
  if (doJEC_==7) nearJetPt_ = (*jets)[iNear_].correctedP4("part","uds").pt();
  anaJets_.push_back(math::PtEtaPhiMLorentzVectorF(nearJetPt_,NrJet.eta(),NrJet.phi(),NrJet.mass()));

  iAway_ = FindAwayJet(iEvent,2);
  if (iAway_<0) return;

  const pat::Jet & AwJet = (*jets)[iAway_];
  awayJetPt_ = -99;
  if (doJEC_==3) awayJetPt_ = (*jets)[iAway_].correctedP4("abs").pt();
  if (doJEC_==5) awayJetPt_ = (*jets)[iAway_].correctedP4("had","glu").pt();
  if (doJEC_==7) awayJetPt_ = (*jets)[iAway_].correctedP4("part","glu").pt();
  anaJets_.push_back(math::PtEtaPhiMLorentzVectorF(awayJetPt_,AwJet.eta(),AwJet.phi(),AwJet.mass()));

  // === dijet kinematics selection ===
  if (nearJetPt_<nearJetPtMin_ || awayJetPt_<awayJetPtMin_) return;
  ++numDJEvtSel_;
  if (numDJEvtSel_<=10) PrintDJEvent(iEvent);

  // -- Fill jet info --
  if (!isMC_) FillJets(iEvent,calojData_,2);
  else {
    FillJets(iEvent,calojGenjData_,2,1);
  }

  // ===== Tracks =====
  //
  Handle<vector<Track> > tracks;
  iEvent.getByLabel(trksrc_, tracks);

  for(unsigned it=0; it<tracks->size(); ++it){
    const reco::Track & trk = (*tracks)[it];
    //if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;

    hTrkPtPreSel_->Fill(trk.pt());
    hTrkEtaPreSel_->Fill(trk.eta());
    hTrkPtEtaPreSel_->Fill(trk.eta(),trk.pt());
  }

  // All done
  if (!isMC_) calojTree_->Fill();
  else {
    calojGenjTree_->Fill();
    calojPtnjTree_->Fill();
    genjCalojTree_->Fill();
    ptnjCalojTree_->Fill();
  }
}


// ------------ method called once each job just before starting event loop  ------------
void DiJetAna::beginJob()
{
  // histos
  hNumVtx_ = fs->make<TH1D>("hNumVtx","; # vtx;#",20,0,20);
  hVtxZEvtSel_ = fs->make<TH1D>("hVtxZEvtSel","z position of best reconstructed hi selected vertex;vz [cm];best vz;#", 80,-20,20);
  hVtxNumTrksPreSel_ = fs->make<TH1D>("hVtxNumTrksPreSel",";# trks in best vtx (pre evt sel);#",100,0,100);
  hVtxNumTrksEvtSel_ = fs->make<TH1D>("hVtxNumTrksEvtSel",";# trks in best vtx;#",100,0,100);
  // jets
  hJetPtPreSel_ = fs->make<TH1D>("hJetPtPreSel",";p_{T}^{corr. jet} [GeV/c];#", 200, 0.0, 200.0);
  hJetEtaPreSel_ = fs->make<TH1D>("hJetEtaPreSel",";#eta^{jet};#", 50, -1.5*jetEtaMax_, 1.5*jetEtaMax_);
  hJetPhiPreSel_ = fs->make<TH1D>("hJetPhiPreSel",";#phi^{jet};#", 50, -1*TMath::Pi(), TMath::Pi());
  // trks
  hTrkPtPreSel_ = fs->make<TH1D>("hTrkPtPreSel",";p_{T}^{trk} [GeV/c];#", 200, 0.0, 200.0);
  hTrkEtaPreSel_ = fs->make<TH1D>("hTrkEtaPreSel",";#eta^{trk};#", 50, -3., 3.);
  hTrkPtEtaPreSel_ = fs->make<TH2D>("hTrkPtEtaPreSel",";#eta^{trk};p_{T}^{trk} [GeV/c]", 50, -3., 3.,200,0,200.);
  // trees
  calojTree_ = fs->make<TTree>("calojTree","data: calo dijet tree");
  calojData_.SetTree(calojTree_);
  calojData_.SetBranches();
  if ( isMC_ ) {
    calojGenjTree_ = fs->make<TTree>("calojGenjTree","mc: calo dijet tree with genjet ref");
    calojPtnjTree_ = fs->make<TTree>("calojPtnjTree","mc: calo dijet tree with parton ref");
    genjCalojTree_ = fs->make<TTree>("genjCalojTree","mc: genjet dijet tree with calojet ref");
    ptnjCalojTree_ = fs->make<TTree>("ptnjCalojTree","mc: parton dijet tree with calojet ref");
    calojGenjData_.SetTree(calojGenjTree_);
    calojPtnjData_.SetTree(calojPtnjTree_);
    genjCalojData_.SetTree(genjCalojTree_);
    ptnjCalojData_.SetTree(ptnjCalojTree_);
    calojGenjData_.SetBranches();
    calojPtnjData_.SetBranches();
    genjCalojData_.SetBranches();
    ptnjCalojData_.SetBranches();
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiJetAna::endJob() {
  // ===== Done =====
  cout << endl << "================ Ana Process Summaries =============" << endl;
  cout << "Number of events pre-selected : "<< numPreEvtSel_ <<endl;
  cout << "Number of dijet events pre-selected : "<< numDJEvtSel_<<endl;
}



// ==================== Member Methods ===========================
// ------------ Tree Filling --------------
void DiJetAna::FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd_, Int_t jetType, Int_t jetRefType)
{
  // Calc dijet vars for ana jets
  jd_.CalcDJVars(anaJets_);
  if (jetType==2) {
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(jetsrc_,jets);
    jd_.nljemf_		= (*jets)[iNear_].emEnergyFraction();
    jd_.aljemf_		= (*jets)[iAway_].emEnergyFraction();
  }
}

// ------------ Find DiJet ----------------
Int_t DiJetAna::FindNearJet(const edm::Event& iEvent, Int_t jetType)
{
  Handle<vector<pat::Jet> > jets;
  iEvent.getByLabel(jetsrc_,jets);

  Int_t iNear = -99;
  Double_t NearPtMax=-99;
  for (unsigned int j=0; j<(*jets).size();++j) {
    const pat::Jet & jet = (*jets)[j];
    Double_t corrPt = jet.pt();
    if (corrPt>NearPtMax) {
      NearPtMax=corrPt;
      iNear=j;
    }
  }
  return iNear;
}

Int_t DiJetAna::FindAwayJet(const edm::Event& iEvent, Int_t jetType)
{
  Handle<vector<pat::Jet> > jets;
  iEvent.getByLabel(jetsrc_,jets);

  Int_t      iAway=-99;
  Double_t   AwayPtMax=-99;
  for (unsigned j=0; j<(*jets).size();++j) {
    const pat::Jet & jet = (*jets)[j];
    Double_t jdphi = TMath::Abs(reco::deltaPhi(anaJets_[0].phi(),jet.phi()));
    if (jdphi < TMath::Pi()*2./3.) continue; // not too close to near jet in dphi

    Double_t corrPt = jet.pt();
    if (corrPt>AwayPtMax) {
      AwayPtMax=corrPt;
      iAway=j;
    }
  }
  return iAway;
}

// ------------- Helpers ------------------
void DiJetAna::PrintDJEvent(const edm::Event& iEvent)
{
  Handle<vector<pat::Jet> > jets;
  iEvent.getByLabel(jetsrc_,jets);
  cout << "# jets: " << (*jets).size() << endl;
  for (unsigned j=0; j<(*jets).size();++j) {
    const pat::Jet & jet = (*jets)[j];
    cout << "jet " << j << " pt|eta|phi: " << jet.pt() << "|" << jet.eta() << "|" << jet.phi() << endl;
  }
  Double_t ljdphi = TMath::Abs(reco::deltaPhi(anaJets_[0].phi(),anaJets_[1].phi()));
  cout << "corr" << doJEC_ << " leading dijet - iNear: " << iNear_ << " " <<": "<< anaJets_[0]
     << "  iAway: " << iAway_ << " " << anaJets_[1] << endl;
     cout << "DiJet dphi: " << ljdphi << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiJetAna);
