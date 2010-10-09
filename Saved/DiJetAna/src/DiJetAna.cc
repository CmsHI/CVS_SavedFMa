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
// $Id: DiJetAna.cc,v 1.59 2010/10/09 00:26:19 frankma Exp $
//
//


// system include files
#include <memory>

// stl
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// ana
#include "Saved/DiJetAna/interface/DiJetAna.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include "TStopwatch.h"

#include "Saved/DiJetAna/interface/TreeDiJetEventData.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"


using namespace std;
using namespace edm;
using namespace reco;

//
// constructors and destructor
//
DiJetAna::DiJetAna(const edm::ParameterSet& iConfig) :
  numHiEvtSel_(0),
  numEvtSel_(0),
  numJetEvtSel_(0),
  cbins_(0)
{
  // Event source
  isMC_ = iConfig.getParameter<bool>("isMC");
  sampleType_ = iConfig.getParameter<int>("sampleType");
  genOnly_ = iConfig.getUntrackedParameter<bool>("genOnly", false);
  // Event Info
  vtxsrc_ = iConfig.getParameter<edm::InputTag>("vtxsrc");
  hltsrc_ = iConfig.getParameter<edm::InputTag>("hltsrc");
  hltNames_ = iConfig.getUntrackedParameter<std::vector <std::string> >("hltNames");
  // jet reco
  jetsrc_ = iConfig.getParameter<edm::InputTag>("jetsrc");
  anaJetType_ = iConfig.getParameter<int>("anaJetType");
  jetEtaMax_ = iConfig.getParameter<double>("jetEtaMax");
  // jet energy correction
  JECLab1_ = iConfig.getParameter<string>("JECLab1");
  doFJL1Corr_ = iConfig.getParameter<bool>("doFJL1Corr");
  // jet mc matching
  refjetsrc_ = iConfig.getParameter<edm::InputTag>("refjetsrc");
  refJetType_ = iConfig.getParameter<int>("refJetType");
  // di-jet reco
  djDPhiMin_ = iConfig.getParameter<double>("djDPhiMin");
  // trk selection
  nearJetPtMin_ = iConfig.getParameter<double>("nearJetPtMin");
  trksrc_ = iConfig.getParameter<edm::InputTag>("trksrc");
  anaTrkType_ = iConfig.getParameter<int>("anaTrkType");
  trkPtMin_ = iConfig.getParameter<double>("trkPtMin");
  // debug
  verbosity_ = iConfig.getUntrackedParameter<int>("verbosity", 0);

  // jec studies
  funcGaus_ = new TF1("funcGaus_","gaus",-1,3);
  funcGaus_->SetParameter(0,1); // norm
  funcGaus_->SetParameter(1,1); // mean
  funcGaus_->SetParameter(2,0.15); // jet et resolution
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
  djEvt_.Clear();

  // Fill Event info
  FillEventInfo(iEvent,iSetup,djEvt_);
  FillTrigInfo(iEvent,djEvt_);

  // Additional Analysis level jet energy corrections on pat jets (not gen)
  if(anaJetType_==2){
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(jetsrc_,jets);
    LoadAnaJECs(iEvent,*jets,anaJECs_);
  } else if(isMC_ && !genOnly_ && refJetType_%10==2){
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(refjetsrc_,jets);
    LoadAnaJECs(iEvent,*jets,anaJECs_);
  }

  // Inclusive Jets Ana
  InclJetAna(iEvent,anaJetType_,anaJECs_,hJetPtEvtPreSel_,hJetEtaEvtPreSel_,hJetPhiEvtPreSel_);

  // Inclusive Trk Ana
  InclTrkAna(iEvent,anaTrkType_);
  ++numHiEvtSel_;

  // Vertex Ana
  FillVtx(iEvent,djEvt_);
  ++numEvtSel_;

  //
  // ---------------------------- Di Jet Analysis ---------------------------------
  //
  // ==== Find Dijet in Event ===
  iNear_ = FindNearJet(iEvent,jetsrc_,anaJetType_, anaJECs_);
  iAway_ = FindAwayJet(iEvent,jetsrc_,anaJetType_,iNear_,anaJECs_);
  //cout << "iNr: " << iNear_ << " iAw_ " << iAway_ << endl;

  // Save DiJet into lorentz vectors, and apply high level JES correction if RecoJet
  DiJetP4(iEvent,jetsrc_,anaJets_,anaJetType_,anaJECs_,iNear_,iAway_);
  //cout << "leadings: " << anaJets_.size() << endl;

  // -- Get Ref jets to the selected dijet (if MC) --
  if (isMC_&&!genOnly_)FindRefJets(iEvent,refJetType_,refJets_);

  // -- some printout --
  if (verbosity_>=2 && (numEvtSel_<=30 || (anaJets_.size()>0 && anaJets_[0].pt()>100))) {
    PrintDJEvent(iEvent,anaJets_,anaJetType_,anaTrkType_);
  }

  // -- Fill DiJet info --
  if (!isMC_&&!genOnly_) FillJets(iEvent,djEvt_,anaJECs_,anaJets_,2,refJets_,-1);
  else { 
    FillJets(iEvent,djEvt_,anaJECs_,anaJets_,anaJetType_,refJets_,refJetType_);
  }

  //
  // === Tracks ===
  //
  // -- leading jet event selection for tracks --
  if (nearJetPtMin_>=0) {
    if (anaJets_.size()==0 || (anaJets_.size()>0 && anaJets_[0].pt()<nearJetPtMin_))
    { djTree_->Fill(); return; }
  }
  ++numJetEvtSel_;
  // Print Tracks
  if (verbosity_>=10 && numJetEvtSel_<=25) PrintTrks(iEvent,anaTrkType_);

  // -- Jet-Track Correlations ---
  FillTrks(iEvent,djEvt_,anaJets_,anaTrkType_);

  // -- Find Leading Trks --
  djEvt_.FindLeadingTrks();

  // All done
  djTree_->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void DiJetAna::beginJob()
{
  // -- histos --
  // event
  hNumVtx_ = fs->make<TH1D>("hNumVtx","; # vtx;#",20,0,20);
  hVtxNumTrksEvtPreSel_ = fs->make<TH1D>("hVtxNumTrksEvtPreSel",";# trks in best vtx (pre evt sel);#",100,0,100);
  hVtxZEvtPreSel_ = fs->make<TH1D>("hVtxZEvtSel","z position of best reconstructed hi selected vertex;vz [cm];best vz;#", 80,-20,20);
  // jets
  hJetPtEvtPreSel_ = fs->make<TH1D>("hJetPtEvtPreSel",";p_{T}^{corr. jet} [GeV/c];#", 200, 0.0, 200.0);
  hJetEtaEvtPreSel_ = fs->make<TH1D>("hJetEtaEvtPreSel",";#eta^{jet};#", 50, -1.5*jetEtaMax_, 1.5*jetEtaMax_);
  hJetPhiEvtPreSel_ = fs->make<TH1D>("hJetPhiEvtPreSel",";#phi^{jet};#", 50, -1*TMath::Pi(), TMath::Pi());
  // trks
  hTrkPtEvtPreSel_ = fs->make<TH1D>("hTrkPtEvtPreSel",";p_{T}^{trk} [GeV/c];#", 200, 0.0, 100.0);
  hTrkEtaEvtPreSel_ = fs->make<TH1D>("hTrkEtaEvtPreSel",";#eta^{trk};#", 50, -3., 3.);
  hTrkPtEtaEvtPreSel_ = fs->make<TH2D>("hTrkPtEtaEvtPreSel",";#eta^{trk};p_{T}^{trk} [GeV/c]", 50, -3., 3.,200,0,100.);
  // -- trees --
  djTree_ = fs->make<TTree>("djTree","dijet tree");
  djEvt_.SetTree(djTree_);
  djEvt_.SetBranches(anaJetType_,anaTrkType_);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiJetAna::endJob() {
  // ===== Done =====
  if (verbosity_>=1) {
    cout << endl << "================ Ana Process Summaries =============" << endl;
    cout << " AnaJet: " << jetsrc_ << endl;
    if (refJetType_>=0) cout << " RefJet: " << refjetsrc_ << endl;
    cout << " AnaTrk: " << trksrc_ << endl;
    cout << "# HI Events : "<< numHiEvtSel_<< endl;
    cout << "# Base Events: "<< numEvtSel_ << endl;
    cout << "# Jet Events: "<< numJetEvtSel_<< endl;
  }
}


// ==================== Member Methods ===========================
double DiJetAna::CalcFJL1Corr(const vector<double> & medianPt, const pat::Jet & jet)
{
  double jetcorr=1.;
  if(fabs(jet.eta())<3){
    double rho=-999;
    if (jet.eta()<-2.5 && jet.eta()>-3.5)rho=medianPt[2];
    if (jet.eta()<-1.5 && jet.eta()>-2.5)rho=medianPt[3];
    if (jet.eta()<-0.5 && jet.eta()>-1.5)rho=medianPt[4];
    if (jet.eta()<0.5 && jet.eta()>-0.5)rho=medianPt[5];
    if (jet.eta()<1.5 && jet.eta()>0.5)rho=medianPt[6];
    if (jet.eta()<2.5 && jet.eta()>1.5)rho=medianPt[7];
    if (jet.eta()<3.5 && jet.eta()>2.5)rho=medianPt[8];
    double jetarea=jet.jetArea();
    jetcorr=1.0-jetarea*rho/jet.et();
  }
  return jetcorr;
}


void DiJetAna::DiJetP4(const edm::Event& iEvent, const edm::InputTag & jsrc, std::vector<math::PtEtaPhiMLorentzVector> & anajets,
    Int_t jetType, const std::vector<double> & anaJECs, Int_t & iNr, Int_t & iAw) 
{
  anajets.clear();

  if (iNr<0) return;
  edm::Handle<reco::CandidateView> jets;
  iEvent.getByLabel(jsrc,jets);

  const reco::Candidate & NrJet = (*jets)[iNr];
  anajets.push_back(math::PtEtaPhiMLorentzVector(NrJet.pt(),NrJet.eta(),NrJet.phi(),NrJet.mass()));
  if (jetType==2) anajets[0] *= anaJECs[iNr];

  if (iAw<0) return;
  const reco::Candidate & AwJet = (*jets)[iAw];
  anajets.push_back(math::PtEtaPhiMLorentzVector(AwJet.pt(),AwJet.eta(),AwJet.phi(),AwJet.mass()));
  if (jetType==2) anajets[1] *= anaJECs[iAw];
}


void DiJetAna::FillEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup, TreeDiJetEventData & jd)
{
  // General Info
  jd.run_	  = iEvent.id().run();
  jd.evt_	  = iEvent.id().event();
  jd.lumi_	  = iEvent.luminosityBlock();

  if(!genOnly_&&sampleType_<10){
    // HI Event info
    edm::Handle<reco::Centrality> cent;
    iEvent.getByLabel(edm::InputTag("hiCentrality"),cent);
    Double_t hf	  = cent->EtHFhitSum();
    // Get Centrality bin
    cbins_ = getCentralityBinsFromDB(iSetup);
    jd.cent_ = cbins_->getBin(hf)*(100./cbins_->getNbins());
  }

  if (isMC_&&sampleType_<10) {
    edm::Handle<edm::GenHIEvent> mchievt;
    iEvent.getByLabel(edm::InputTag("heavyIon"),mchievt);
    jd.b_	  = mchievt->b();
    jd.npart_	  = mchievt->Npart();
    jd.ncoll_	  = mchievt->Ncoll();
  }
}

void  DiJetAna::FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd,
			 const std::vector<double> & anaJECs,
			 const std::vector<math::PtEtaPhiMLorentzVector> & anajets, 
			 Int_t anajetType,
			 const std::vector<math::PtEtaPhiMLorentzVector> & refjets, 
			 Int_t refjetType)
{
  // if no leading jet found in event, then nothing to do
  if (anajets.size()<1) return;

  // Calc dijet vars for ana jets
  jd.CalcDJVars(isMC_,anajets,refjets);

  // -- jec studies --
  jd.njec_[10]				= funcGaus_->GetRandom();
  if (anajets.size()>=2) jd.ajec_[10]	= funcGaus_->GetRandom();

  if (anajetType==2) {
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(jetsrc_,jets);

    // -- jet properties --
    if ((*jets)[iNear_].isCaloJet()) {
      jd.nljemf_	= (*jets)[iNear_].emEnergyFraction();
    }
    jd.nljN90hits_	= (*jets)[iNear_].jetID().n90Hits;
    jd.nljfhpd_		= (*jets)[iNear_].jetID().fHPD;
    jd.nljarea_		= (*jets)[iNear_].jetArea();

    if (anajets.size()>=2) {
      if ((*jets)[iAway_].isCaloJet()) {
	jd.aljemf_	= (*jets)[iAway_].emEnergyFraction();
      }
      jd.aljN90hits_	= (*jets)[iAway_].jetID().n90Hits;
      jd.aljfhpd_	= (*jets)[iAway_].jetID().fHPD;
      jd.aljarea_	= (*jets)[iAway_].jetArea();
    }

    // -- jec --
    //cout << "Current JEC Step: " << "Nr: " << (*jets)[iNear_].corrStep() << " Aw: " <<  (*jets)[iAway_].corrStep() << endl;
    jd.nljrawet_	= (*jets)[iNear_].correctedP4("raw").pt();
    jd.njec_[0]		= (*jets)[iNear_].corrFactor("raw");
    jd.njec_[1]         = anaJECs[iNear_];
    jd.njec_[2]		= (*jets)[iNear_].corrFactor("rel");
    jd.njec_[3]		= (*jets)[iNear_].corrFactor("abs");
    jd.njec_[5]		= (*jets)[iNear_].corrFactor("had","uds");
    jd.njec_[7]		= (*jets)[iNear_].corrFactor("part","uds");

    if (anajets.size()>=2) { // careful away jec is not reset if away not found
      jd.aljrawet_	= (*jets)[iAway_].correctedP4("raw").pt();
      jd.ajec_[0]	= (*jets)[iAway_].corrFactor("raw");
      jd.ajec_[1]       = anaJECs[iAway_];
      jd.ajec_[2]	= (*jets)[iAway_].corrFactor("rel");
      jd.ajec_[3]	= (*jets)[iAway_].corrFactor("abs");
      jd.ajec_[5]	= (*jets)[iAway_].corrFactor("had","glu");
      jd.ajec_[7]	= (*jets)[iAway_].corrFactor("part","glu");
    }
  }
}

void DiJetAna::FillTrigInfo(const edm::Event& iEvent, TreeDiJetEventData & jd)
{
  if (hltsrc_.label()=="None"||hltsrc_.label()=="none") return;

  // get hlt bit
  Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel(hltsrc_, triggerResults);
  const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults); 
  // check hlt paths if verbose
  if (verbosity_ >= 3) {
    for (unsigned i=0; i<triggerNames.size(); i++) { 
      std::string hltName = triggerNames.triggerName(i);
      unsigned int index = triggerNames.triggerIndex(hltName);
      cout << "HLT: " << hltName << " fired? " << triggerResults->accept(index) << endl;
    }
  }

  // Save HLT
  for(unsigned i=0; i<hltNames_.size(); i++) { 
    unsigned index = triggerNames.triggerIndex(hltNames_[i]);
    if(index < triggerResults->size())
      jd.hlt_.push_back(triggerResults->accept(index));
    else {
      edm::LogWarning("TrackSpectraAnalyzer")
	<< "Index returned by TriggerNames object for trigger '"
	<< hltNames_[i]
	<< "' is out of range (" 
	<< index << " >= " << triggerResults->size() << ")";
    }
  }
}

void  DiJetAna::FillTrks(const edm::Event& iEvent, TreeDiJetEventData & jd,
    const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
    Int_t trkType)
{
  if (trkType==2) {
    Handle<vector<Track> > tracks;
    iEvent.getByLabel(trksrc_, tracks);
    int selTrkCt = 0;
    for(unsigned int it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];
      // Trk Selection
      if (!GoodAnaTrk(trk)) continue;
      // fill frag candidates basic info
      jd.trkNHits_[selTrkCt]	       = trk.numberOfValidHits();
      // make trk-jet calcuations
      math::PtEtaPhiMLorentzVector tcand(trk.pt(),trk.eta(),trk.phi(),0.13957); // assume it's pi+/-
      jd.CalcTrkVars(isMC_,anajets,tcand,selTrkCt);
      ++selTrkCt;
    }
    jd.evtnp_			 = selTrkCt;
  } else if (trkType==0||trkType==3) {
    edm::Handle<reco::CandidateView> trks;
    edm::Handle<reco::GenParticleCollection> genps;
    iEvent.getByLabel(trksrc_,trks);
    if (trkType==0) iEvent.getByLabel(trksrc_, genps);

    int selTrkCt = 0;
    for (unsigned int it=0; it<(*trks).size();++it) {
      const reco::Candidate & trk = (*trks)[it];
      // Find Leading particle before selection
      for (UInt_t i=0; i<anajets.size(); ++i) {
	if (trk.status()==1 && trk.pt()>jd.lgppt_[i] && deltaR(trk,anajets[i])<0.5 ) {
	  jd.lgppt_[i] = trk.pt();
	  jd.lgpch_[i] = trk.charge();
	  jd.lgppid_[i] = trk.pdgId();
	}
      }
      // Select charged stable particles
      if (!GoodAnaTrkParticle(trk,trkType)) continue;
      // fill subevent info if genp
      if (trkType==0) {
	const reco::GenParticle & p    = (*genps)[it];
	jd.psube_[selTrkCt]	       = p.collisionId();
      }
      // fill frag candidates basic info
      //jd.ppid_[selTrkCt]	       = trk.pdgId();
      //jd.pch_[selTrkCt]		       = trk.charge();
      // make trk-jet calcuations
      jd.CalcTrkVars(isMC_,anajets,trk.polarP4(),selTrkCt);
      ++selTrkCt;
    }
    jd.evtnp_			 = selTrkCt;
  }
}

void DiJetAna::FillVtx(const edm::Event& iEvent, TreeDiJetEventData & jd)
{
  // reco vtx
  if(!genOnly_){
    // get vtx collection 
    Handle<vector<Vertex> > vertices;
    iEvent.getByLabel(vtxsrc_, vertices);
    Int_t numVtx = (Int_t)vertices->size();
    hNumVtx_->Fill(numVtx);
    if (verbosity_>=2) cout << "# vertices in event: " << numVtx << endl;
    
    Int_t numFake=0, maxtracks=-99;
    double bestndof=-999.9,bestvz=-999.9, bestvx=-999.9, bestvy=-999.9, bestNchi2=999.9;
    for(UInt_t it=0; it<vertices->size(); ++it) {
      const reco::Vertex & vtx = (*vertices)[it];
      // check if valid vertex
      if(vtx.isFake()) {
	++numFake;
	continue;
      }
      // update best vertex
      int vtxntrks = vtx.tracksSize();
      if(vtxntrks > maxtracks || (vtxntrks == maxtracks && vtx.normalizedChi2() < bestNchi2) ) {
	maxtracks = vtx.tracksSize();
	bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
	bestNchi2 = vtx.normalizedChi2();
	bestndof = vtx.ndof();
      } 
    }
    if (verbosity_>=2) cout << "best non-fake vertex ntracks: " << maxtracks << endl;
    hVtxNumTrksEvtPreSel_->Fill(maxtracks);
    hVtxZEvtPreSel_->Fill(bestvz);
    jd.nvtx_ = numVtx;
    jd.vtxntrks_ = maxtracks;
    jd.vtxchi2_ = bestNchi2;
    jd.vz_ = bestvz;
  }
}


Int_t DiJetAna::FindNearJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, const std::vector<double> & anaJECs)
{
  Int_t iNear = -99;
  Double_t NearPtMax=-99;

  if (jetType<=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jsrc,jets);
    for (unsigned int j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      if (fabs(jet.eta())>jetEtaMax_) continue; // only leading jet within analysis eta
      Double_t corrPt = jet.pt();
      if(jetType==2)corrPt *= anaJECs[j];
      if (corrPt>NearPtMax) {
	NearPtMax=corrPt;
	iNear=j;
      }
    }
  }
  return iNear;
}

Int_t DiJetAna::FindAwayJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, Int_t iNr, const std::vector<double> & anaJECs)
{
  Int_t      iAway=-99;
  if (iNr<0) return iAway;

  Double_t   AwayPtMax=-99;
  if (jetType<=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jsrc,jets);
    const reco::Candidate & NrJet = (*jets)[iNr];
    for (unsigned int j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      if (fabs(jet.eta())>jetEtaMax_) continue; // only away jet within analysis eta 
      Double_t jdphi = TMath::Abs(reco::deltaPhi(NrJet.phi(),jet.phi()));
      if (jdphi < djDPhiMin_) continue; // not too close to near jet in dphi

      Double_t corrPt = jet.pt();
      if(jetType==2)corrPt *= anaJECs[j];
      if (corrPt>AwayPtMax) {
	AwayPtMax=corrPt;
	iAway=j;
      }
    }
  }
  return iAway;
}

void DiJetAna::FindRefJets(const edm::Event& iEvent, Int_t refjetType, std::vector<math::PtEtaPhiMLorentzVector> & refjets)
{
  refjets.clear();

  if (anaJets_.size()<1) return;
  Handle<vector<pat::Jet> > matjets;
  iEvent.getByLabel(refjetsrc_,matjets);

  // calojet to genjet
  if (refjetType==21) {
    const reco::GenJet * NrGJet = (*matjets)[iNear_].genJet();
    if (NrGJet) refjets.push_back(math::PtEtaPhiMLorentzVector(NrGJet->pt(),NrGJet->eta(),NrGJet->phi(),NrGJet->mass()));
    if (anaJets_.size()>=2) {
      const reco::GenJet * AwGJet = (*matjets)[iAway_].genJet();
      if (AwGJet) refjets.push_back(math::PtEtaPhiMLorentzVector(AwGJet->pt(),AwGJet->eta(),AwGJet->phi(),AwGJet->mass()));
    }
    return; // done with case 21
  }

  // genjet to calojet
  // * anajets are the genjets, and we have the nr/aw
  // * rejets are the patjets with matching
  if (refjetType==12) {
    // loop through patjets to get matching and compare to the found near/away genjet
    for (unsigned i=0; i<anaJets_.size(); ++i) {
      for (unsigned j=0; j<(*matjets).size();++j) {
	const pat::Jet & matjet = (*matjets)[j];
	const reco::GenJet * GJet = matjet.genJet();
	if (GJet) {
	  double dPt = TMath::Abs(anaJets_[i].pt()-GJet->pt());
	  double dPhi = TMath::Abs(anaJets_[i].phi()-GJet->phi());
	  //cout << "dPt: " << dPt << " dPhi: " << dPhi << endl;
	  if (dPt<0.01 && dPhi<0.01) {
	    refjets.push_back(math::PtEtaPhiMLorentzVector(matjet.pt(),matjet.eta(),matjet.phi(),matjet.mass()));
	    // correct b/c it's a calojet
	    refjets[i] *= anaJECs_[j];
	    break;
	  }
	}
      }
    }
    return; // done with case 12
  }
}


Bool_t DiJetAna::GoodAnaTrk(const reco::Track & trk)
{
  //if(!trk.quality(reco::TrackBase::qualityByName(qualityString))) continue;
  if (trk.pt()<trkPtMin_) return false;
  return true;
}

Bool_t DiJetAna::GoodAnaTrkParticle(const reco::Candidate & p, Int_t trkType)
{
  if (trkType==0) {
    if (p.status()!=1) return false;
    if (p.charge()==0) return false;
  }
  if (p.pt()<trkPtMin_) return false;
  return true;
}


void DiJetAna::InclJetAna(const edm::Event& iEvent, Int_t jetType, const std::vector<double> & anaJECs,
    TH1D * hPt, TH1D * hEta, TH1D * hPhi)
{
  if (jetType<=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jetsrc_,jets);
    djEvt_.njets_ = (*jets).size();
    for (unsigned int j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      if (fabs(jet.eta())>jetEtaMax_) continue; // only jets within analysis eta
      Double_t corrPt=jet.pt();
      if (jetType==2) corrPt *= anaJECs[j];
      hPt->Fill(corrPt);
      hEta->Fill(jet.eta());
      hPhi->Fill(jet.phi());
    }
  }
}

void DiJetAna::InclTrkAna(const edm::Event& iEvent, Int_t trkType)
{
  if (trkType==2) {
    Handle<vector<Track> > tracks;
    iEvent.getByLabel(trksrc_, tracks);
    djEvt_.ntrks_ = tracks->size();
    for(unsigned it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];
      // trk selection
      if (!GoodAnaTrk(trk)) continue;
      hTrkPtEvtPreSel_->Fill(trk.pt());
      hTrkEtaEvtPreSel_->Fill(trk.eta());
      hTrkPtEtaEvtPreSel_->Fill(trk.eta(),trk.pt());
    }
  }

  if (trkType==0||trkType==3) {
    edm::Handle<reco::CandidateView> trks;
    iEvent.getByLabel(trksrc_,trks);
    djEvt_.ntrks_ = (*trks).size();
    for (unsigned int it=0; it<(*trks).size();++it) {
      const reco::Candidate & trk = (*trks)[it];
      // select charged stable particles
      if (!GoodAnaTrkParticle(trk,trkType)) continue;

      hTrkPtEvtPreSel_->Fill(trk.pt());
      hTrkEtaEvtPreSel_->Fill(trk.eta());
      hTrkPtEtaEvtPreSel_->Fill(trk.eta(),trk.pt());
    }
  }
}


void DiJetAna::LoadAnaJECs(const edm::Event & iEvent, const vector<pat::Jet> & jets, vector<double> & JECs)
{
  JECs.clear();

  // FJ rho subtraction
  vector<double> medianPtKt;
  if(doFJL1Corr_){
    edm::Handle<std::vector<double> > rs;
    iEvent.getByLabel(edm::InputTag("kt4CaloJets","rhos"),rs);
    //double puCent[11] = {-5,-4,-3,-2,-1,0,1,2,3,4,5};
    for(unsigned int j = 0; j < rs->size()-11; j++){
      double medianpt=(*rs)[j+11];
      medianPtKt.push_back(medianpt);
    }
  }

  // Fill Analysis Level Jet Energy Corrections
  for (unsigned j=0; j<jets.size();++j) {
    double anaCorr = 1;
    anaCorr *= jets[j].corrFactor(JECLab1_);
    if (doFJL1Corr_) anaCorr *= CalcFJL1Corr(medianPtKt,jets[j]);
    JECs.push_back(anaCorr);
  }
}


void DiJetAna::PrintDJEvent(const edm::Event& iEvent, const std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType, Int_t trkType)
{
  cout << "=== Ana setup: (Base Sel Evt " << numEvtSel_ << ")===" << endl;
  cout << "AnaJet: " << jetsrc_ << ". With anaJetType: " << anaJetType_ << endl;
  if (refJetType_>=0) cout << "RefJet: " << refjetsrc_ << ". With refJetType: " << refJetType_ << endl;
  cout << "AnaTrk: " << trksrc_ << ". With anaTrkType: " << anaTrkType_ << endl;
  cout << " JEC: " << JECLab1_ << ", doFJCorr: " << doFJL1Corr_ << endl;
  // Print found leading dijet
  if (anajets.size()>=1) {
    cout << "Leading dijet - iNear: " << iNear_ << " " <<": "<< anajets[0]
      << "  iAway: " << iAway_;
  }
  if (anajets.size()==1) cout << endl;
  if (anajets.size()>=2) { 
    cout << " " << anajets[1] << endl;
    Double_t ljdphi = TMath::Abs(reco::deltaPhi(anajets[0].phi(),anajets[1].phi()));
    cout << " DiJet dphi: " << ljdphi << endl;
  }
  // Print matched refjets
  for (unsigned int i=0; i<refJets_.size(); ++i) {
    cout << "refjet " << i << ": " << refJets_[i] << endl;
  }
  // Print all jets
  if (jetType<=2 && verbosity_>=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jetsrc_,jets);
    cout << "jetType " << jetType << ", # jets: " << (*jets).size() << ". trkType, " << trkType
      << " # trks: " << djEvt_.ntrks_ << ". centrality: " << djEvt_.cent_ << endl;
    for (unsigned j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      if (fabs(jet.eta())>jetEtaMax_) {
	if (verbosity_<3) continue; // only jets within analysis eta
	cout << " out of ana eta range --> ";
      }
      double corrPt = jet.pt();
      if (jetType==2) corrPt *= anaJECs_[j];
      if (verbosity_<3 && corrPt<20.) continue; // make print not too crowded
      cout << "jet " << j << "  anaCorrEt: "<< corrPt;
      cout <<" et|eta|phi: " << jet.pt() << "|" << jet.eta() << "|" << jet.phi() << endl;
    }
  }
}

void DiJetAna::PrintTrks(const edm::Event& iEvent, Int_t trkType)
{
  if (trkType==2) {
    Handle<vector<Track> > tracks;
    iEvent.getByLabel(trksrc_, tracks);
    cout << " # trks: " << tracks->size() << endl;
    for(unsigned it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];
      if (it<20 || it>(tracks->size()-20))
	cout << "trk " << it << " pt|eta|phi: " << trk.pt() << "|" << trk.eta() << "|" << trk.phi() << endl;
    }
  }

  if (trkType==0||trkType==3) {
    edm::Handle<reco::CandidateView> trks;
    iEvent.getByLabel(trksrc_, trks);
    cout << "# of (type" << trkType <<")" << "trks: " << trks->size() << endl;
    Int_t pct = 0;
    for(unsigned it=0; it<trks->size(); ++it){
      const reco::Candidate & trk = (*trks)[it];
      // select charged stable particles
      if (!GoodAnaTrkParticle(trk,trkType)) continue;
      ++pct;
      if (pct<=20 || it>(trks->size()-20)) {
	cout << "trk " << it;
	if (trkType==0) cout << " pid|status|charge: " << trk.pdgId() << "|" << trk.status() << "|" << trk.charge();
	cout << " pt|eta|phi: " << trk.pt() << "|" << trk.eta() << "|" << trk.phi() << endl;
      }
    }
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiJetAna);
