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
// $Id: DiJetAna.cc,v 1.8 2010/07/26 15:41:18 frankma Exp $
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
  numPreEvtSel_(0),
  numHiEvtSel_(0),
  numDJEvtSel_(0)
{
  //now do what ever initialization is needed
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC", true);
  genOnly_ = iConfig.getUntrackedParameter<bool>("genOnly", true);
  applyLAnaJEC_ = iConfig.getUntrackedParameter<bool>("applyLAnaJEC", true);
  centFile_ = iConfig.getParameter<string>("centFile");
  centLabel_ = iConfig.getParameter<string>("centLabel");
  vtxsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("vtxsrc",edm::InputTag("hiSelectedVertex"));
  jetsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("jetsrc",edm::InputTag("patJets"));
  refjetsrc_ = iConfig.getUntrackedParameter<edm::InputTag>("refjetsrc",edm::InputTag("patJets"));
  trksrc_ = iConfig.getUntrackedParameter<edm::InputTag>("trksrc",edm::InputTag("hiSelectTracks"));
  jetEtaMax_ = iConfig.getUntrackedParameter<double>("jetEtaMax", 2.0);
  nVtxTrkCut_ = iConfig.getUntrackedParameter<int>("nVtxTrkCut", 3);
  JECLab1_ = iConfig.getParameter<string>("JECLab1");
  JECLab2Nr_ = iConfig.getParameter<string>("JECLab2Nr");
  JECLab2Aw_ = iConfig.getParameter<string>("JECLab2Aw");
  // dijet
  nearJetPtMin_ = iConfig.getUntrackedParameter<double>("nearJetPtMin", 50);
  awayJetPtMin_ = iConfig.getUntrackedParameter<double>("awayJetPtMin", 50);
  djDPhiMin_ = iConfig.getParameter<double>("djDPhiMin");
  // trk
  trkPtMin_ = iConfig.getUntrackedParameter<double>("trkPtMin", 0.3);
  anaJetType_ = iConfig.getUntrackedParameter<int>("anaJetType", 2);
  refJetType_ = iConfig.getUntrackedParameter<int>("refJetType", 11);
  // centrality
  anaTrkType_ = iConfig.getUntrackedParameter<int>("anaTrkType", 3);
  centBinBeg_ = iConfig.getUntrackedParameter<int>("centBinBeg", 0);
  centBinEnd_ = iConfig.getUntrackedParameter<int>("centBinEnd", 6);
  // verbosity
  verbosity_ = iConfig.getUntrackedParameter<int>("verbosity", 0);

  // Setup centrality
  TFile * centFile = new TFile(centFile_.c_str());
  HFhitBinMap_ = getCentralityFromFile(centFile,centLabel_.c_str(),0,99);

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
  const int nTrigs = 5;
  const string qualityString = "highPurity";
  djEvt_.Clear();

  if(!anaJetType_==2){
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
  }

  //-----------------------  HI Evt election (This part will be in an EDFilter later)  
  if(!genOnly_){
    edm::Handle<reco::Centrality> cent;
    iEvent.getByLabel(edm::InputTag("hiCentrality"),cent);
    Double_t hf	  = cent->EtHFhitSum();
    Int_t cbin	  = HFhitBinMap_[1]->getBin(hf);
    //cout << "cbin: " << cbin << endl;
    if (cbin<centBinBeg_ || cbin>=centBinEnd_) return;
  }
  ++numHiEvtSel_;

  // Done with Event Pre-Selection
  // Fill Event info
  FillEventInfo(iEvent,djEvt_);

  // Grab L1 Corrections if doing fastJet-style PU
  if(applyLAnaJEC_){
    edm::Handle<std::vector<double> > rs;
    iEvent.getByLabel(edm::InputTag("kt4CaloJets","rhos"),rs);
   
    double puCent[11] = {-5,-4,-3,-2,-1,0,1,2,3,4,5};
    
    double ymin, ymax;
    
    int rsize = rs->size();

    for(int j = 0; j < rsize-11; j++){
      
      //     cout<<j<<"    "<<rs->at(j+11)<<endl;
      ymin=puCent[j]-0.5;
      ymax=puCent[j]+0.5;
      double medianpt=rs->at(j+11);
      medianPtkt[j]=medianpt;
      //    cout<<" ymin  "<<ymin<<"  ymax "<<ymax<<endl;         
      
    }
  }

  std::vector<double> L1Corrs;
  // only grab correction for pat jets (not gen)
  if(anaJetType_==2){
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(jetsrc_,jets);
    L1Corrs =  FillL1Corrs(jets);
  }

  //
  // ---------------------------- Jet Analysis ---------------------------------
  //
  // Check Inclusive Jets Before DJ Selection
  InclJetAna(iEvent,anaJetType_,L1Corrs,hJetPtPreSel_,hJetEtaPreSel_,hJetPhiPreSel_);

  //
  // ============================= DiJet Ana =============================
  //
  nearJetPt_ = -99; awayJetPt_ = -99;
  FindDiJet(iEvent,jetsrc_,anaJets_,anaJetType_,L1Corrs,nearJetPt_,iNear_,awayJetPt_,iAway_);

  // === basic dijet selection to simulate jet trigger ===
  if (nearJetPt_<nearJetPtMin_ || awayJetPt_<awayJetPtMin_) return;
  ++numDJEvtSel_;
  if (verbosity_>=1 && numDJEvtSel_<=7) PrintDJEvent(iEvent,anaJets_,anaJetType_,anaTrkType_);
  // Check Inclusive Jets After DJ Selection
  InclJetAna(iEvent,anaJetType_,L1Corrs,hJetPtDJSel_,hJetEtaDJSel_,hJetPhiDJSel_);

  // -- Get Ref jets to the selected dijet (if MC) --
  if (isMC_&&!genOnly_)FindRefJets(iEvent,refJetType_,refJets_);

  // -- Fill DiJet info --
  if (!isMC_&&!genOnly_) FillJets(iEvent,djEvt_,L1Corrs,anaJets_,2,refJets_,-1);
  else { 
    FillJets(iEvent,djEvt_,L1Corrs,anaJets_,anaJetType_,refJets_,refJetType_);
  }

  //
  // =============================== Tracks ==============================
  //
  // Inclusive Trk ana
  InclTrkAna(iEvent,anaTrkType_);

  // Print Tracks
  if (verbosity_>=10 && numDJEvtSel_<=7) PrintTrks(iEvent,anaTrkType_);

  // -- Jet-Track Correlations ---
  FillTrks(iEvent,djEvt_,anaJets_,refJets_,anaTrkType_);

  // All done
  djTree_->Fill();
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
  hJetPtDJSel_ = fs->make<TH1D>("hJetPtDJSel",";p_{T}^{corr. jet} [GeV/c];#", 200, 0.0, 200.0);
  hJetEtaDJSel_ = fs->make<TH1D>("hJetEtaDJSel",";#eta^{jet};#", 50, -1.5*jetEtaMax_, 1.5*jetEtaMax_);
  hJetPhiDJSel_ = fs->make<TH1D>("hJetPhiDJSel",";#phi^{jet};#", 50, -1*TMath::Pi(), TMath::Pi());
  // trks
  hTrkPtDJEvtSel_ = fs->make<TH1D>("hTrkPtDJEvtSel",";p_{T}^{trk} [GeV/c];#", 200, 0.0, 100.0);
  hTrkEtaDJEvtSel_ = fs->make<TH1D>("hTrkEtaDJEvtSel",";#eta^{trk};#", 50, -3., 3.);
  hTrkPtEtaDJEvtSel_ = fs->make<TH2D>("hTrkPtEtaDJEvtSel",";#eta^{trk};p_{T}^{trk} [GeV/c]", 50, -3., 3.,200,0,100.);
  // trees
  djTree_ = fs->make<TTree>("djTree","dijet tree");
  djEvt_.SetTree(djTree_);
  djEvt_.SetBranches();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiJetAna::endJob() {
  // ===== Done =====
  cout << endl << "================ Ana Process Summaries =============" << endl;
  cout << "Centrality: " << centLabel_ << endl;
  cout << "AnaJet: " << jetsrc_;
  if (refJetType_>=0) cout << " RefJet: " << refjetsrc_;
  cout << "  AnaTrk: " << trksrc_ << endl;
  cout << "Number of events pre-selected : "<< numPreEvtSel_ <<endl;
  cout << "Number of HI events selected : "<< numHiEvtSel_<<endl;
  cout << "Number of dijet events selected : "<< numDJEvtSel_<<endl;
}



// ==================== Member Methods ===========================
// ------------ Inclusive Ana -------------
void DiJetAna::InclJetAna(const edm::Event& iEvent, Int_t jetType, std::vector<double>L1Corrs,
    TH1D * hPt, TH1D * hEta, TH1D * hPhi)
{
  if (jetType<=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jetsrc_,jets);
    for (unsigned int j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      Double_t corrPt=jet.pt();
      
      if (jetType==2) {
	Handle<vector<pat::Jet> > patjets;
	iEvent.getByLabel(jetsrc_,patjets);
	corrPt = (*patjets)[j].correctedP4(JECLab1_).pt()*L1Corrs[j];
      }
      if(abs(jet.eta())<2.){
	hPt->Fill(corrPt);
	hEta->Fill(jet.eta());
	hPhi->Fill(jet.phi());
      }
    }
  }
}
void DiJetAna::InclTrkAna(const edm::Event& iEvent, Int_t trkType)
{
  if (trkType==2) {
    Handle<vector<Track> > tracks;
    iEvent.getByLabel(trksrc_, tracks);
    for(unsigned it=0; it<tracks->size(); ++it){
      const reco::Track & trk = (*tracks)[it];
      // trk selection
      if (!GoodAnaTrk(trk)) continue;
      hTrkPtDJEvtSel_->Fill(trk.pt());
      hTrkEtaDJEvtSel_->Fill(trk.eta());
      hTrkPtEtaDJEvtSel_->Fill(trk.eta(),trk.pt());
    }
  }

  if (trkType==0||trkType==3) {
    edm::Handle<reco::CandidateView> trks;
    iEvent.getByLabel(trksrc_,trks);
    for (unsigned int it=0; it<(*trks).size();++it) {
      const reco::Candidate & trk = (*trks)[it];
      // select charged stable particles
      if (!GoodAnaTrkParticle(trk,trkType)) continue;

      hTrkPtDJEvtSel_->Fill(trk.pt());
      hTrkEtaDJEvtSel_->Fill(trk.eta());
      hTrkPtEtaDJEvtSel_->Fill(trk.eta(),trk.pt());
    }
  }
}

// ------------ Tree Filling --------------
void DiJetAna::FillEventInfo(const edm::Event& iEvent, TreeDiJetEventData & jd)
{
  // General Info
  jd.run_	  = iEvent.id().run();
  jd.evt_	  = iEvent.id().event();
  jd.lumi_	  = iEvent.luminosityBlock();

  if(!genOnly_){
    // HI Event info
    edm::Handle<reco::Centrality> cent;
    iEvent.getByLabel(edm::InputTag("hiCentrality"),cent);
    Double_t hf	  = cent->EtHFhitSum();
    jd.cbin_	  = HFhitBinMap_[1]->getBin(hf);
  }

  if (isMC_) {
    edm::Handle<edm::GenHIEvent> mchievt;
    iEvent.getByLabel(edm::InputTag("heavyIon"),mchievt);
    jd.b_	  = mchievt->b();
    jd.npart_	  = mchievt->Npart();
    jd.ncoll_	  = mchievt->Ncoll();
  }
}


vector<double> 
DiJetAna::FillL1Corrs(edm::Handle<vector<pat::Jet> > jets)
{
  
  vector<double> L1Corrs;

  for (unsigned j=0; j<(*jets).size();++j) {
    

    const pat::Jet & jet = (*jets)[j];    

    if(fabs(jet.eta())<=3 && applyLAnaJEC_){
      
      double rho=-999;
      
      if (jet.eta()<-2.5 && jet.eta()>-3.5)rho=medianPtkt[2];
      if (jet.eta()<-1.5 && jet.eta()>-2.5)rho=medianPtkt[3];
      if (jet.eta()<-0.5 && jet.eta()>-1.5)rho=medianPtkt[4];
      if (jet.eta()<0.5 && jet.eta()>-0.5)rho=medianPtkt[5];
      if (jet.eta()<1.5 && jet.eta()>0.5)rho=medianPtkt[6];
      if (jet.eta()<2.5 && jet.eta()>1.5)rho=medianPtkt[7];
      if (jet.eta()<3.5 && jet.eta()>2.5)rho=medianPtkt[8];
      
      double jetarea=jet.jetArea();
      double jetcorr=1.0-jetarea*rho/jet.et();
      L1Corrs.push_back(jetcorr);
    }
    else  L1Corrs.push_back(1.);
    
  }
  
  return L1Corrs;

}


void  DiJetAna::FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd,
			 std::vector<double>L1Corrs,
			 std::vector<math::PtEtaPhiMLorentzVector> & anajets, 
			 Int_t anajetType,
			 std::vector<math::PtEtaPhiMLorentzVector> & refjets, 
			 Int_t refjetType)
{
  // Calc dijet vars for ana jets
  jd.CalcDJVars(isMC_,anajets,refjets);
  // -- jec studies --
  jd.njec_[10]	= funcGaus_->GetRandom();
  jd.ajec_[10]	= funcGaus_->GetRandom();

  if (anajetType==2) {
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(jetsrc_,jets);
    // -- jec --
    //cout << "Current JEC Step: " << "Nr: " << (*jets)[iNear_].corrStep() << " Aw: " <<  (*jets)[iAway_].corrStep() << endl;
    jd.nljrawet_	= (*jets)[iNear_].correctedP4("raw").pt();
    jd.aljrawet_	= (*jets)[iAway_].correctedP4("raw").pt();

    jd.njec_[0]		= (*jets)[iNear_].corrFactor("raw");
    jd.njec_[1]         = L1Corrs[iNear_];
    jd.njec_[2]		= (*jets)[iNear_].corrFactor("rel");
    jd.njec_[3]		= (*jets)[iNear_].corrFactor("abs");
    jd.njec_[5]		= (*jets)[iNear_].corrFactor("had","uds");
    jd.njec_[7]		= (*jets)[iNear_].corrFactor("part","uds");

    jd.ajec_[0]		= (*jets)[iAway_].corrFactor("raw");
    jd.ajec_[1]         = L1Corrs[iAway_];
    jd.ajec_[2]		= (*jets)[iAway_].corrFactor("rel");
    jd.ajec_[3]		= (*jets)[iAway_].corrFactor("abs");
    jd.ajec_[5]		= (*jets)[iAway_].corrFactor("had","glu");
    jd.ajec_[7]		= (*jets)[iAway_].corrFactor("part","glu");

    // -- jet id --
    jd.nljemf_		= (*jets)[iNear_].emEnergyFraction();
    jd.aljemf_		= (*jets)[iAway_].emEnergyFraction();
  }
}

void  DiJetAna::FillTrks(const edm::Event& iEvent, TreeDiJetEventData & jd,
    std::vector<math::PtEtaPhiMLorentzVector> & anajets,
    std::vector<math::PtEtaPhiMLorentzVector> & refjets,
    Int_t trkType)
{
  if (anajets.size()<2) return;

  if (trkType==2) {
    Handle<vector<Track> > tracks;
    iEvent.getByLabel(trksrc_, tracks);
    int selTrkCt = 0;
    for(unsigned it=0; it<tracks->size(); ++it){
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
      // select charged stable particles
      if (!GoodAnaTrkParticle(trk,trkType)) continue;
      // fill subevent info if genp
      if (trkType==0) {
	const reco::GenParticle & p    = (*genps)[it];
	jd.psube_[selTrkCt]	       = p.collisionId();
      }
      // fill frag candidates basic info
      jd.ppid_[selTrkCt]	       = trk.pdgId();
      jd.pch_[selTrkCt]		       = trk.charge();
      // make trk-jet calcuations
      jd.CalcTrkVars(isMC_,anajets,trk.polarP4(),selTrkCt);
      ++selTrkCt;
    }
    jd.evtnp_			 = selTrkCt;
  }

}

// ------------ Find DiJet ----------------
Int_t DiJetAna::FindNearJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, std::vector<double> L1Corrs)
{
  Int_t iNear = -99;
  Double_t NearPtMax=-99;

  if (jetType<=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jsrc,jets);
    for (unsigned int j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      Double_t corrPt = jet.pt();
      if(jetType==2)corrPt *= L1Corrs[j];
      if (corrPt>NearPtMax) {
	NearPtMax=corrPt;
	iNear=j;
      }
    }
  }
  return iNear;
}

Int_t DiJetAna::FindAwayJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, Int_t iNr, std::vector<double> L1Corrs)
{
  Int_t      iAway=-99;
  Double_t   AwayPtMax=-99;
  if (jetType<=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jsrc,jets);
    const reco::Candidate & NrJet = (*jets)[iNr];
    for (unsigned j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      Double_t jdphi = TMath::Abs(reco::deltaPhi(NrJet.phi(),jet.phi()));
      if (jdphi < djDPhiMin_) continue; // not too close to near jet in dphi

      Double_t corrPt = jet.pt();
      if(jetType==2)corrPt *= L1Corrs[j];
      if (corrPt>AwayPtMax) {
	AwayPtMax=corrPt;
	iAway=j;
      }
    }
  }
  return iAway;
}

void DiJetAna::FindDiJet(const edm::Event& iEvent, const edm::InputTag & jsrc, std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType, std::vector<double> L1Corrs, Double_t & nrjetPt, Int_t & iNr, Double_t & awjetPt, Int_t & iAw) 
{
  anajets.clear();

  iNr = FindNearJet(iEvent,jsrc,jetType, L1Corrs);
  if (iNr<0) return;

  // Establish JES for reco jet
  if (jetType==2) {
    Handle<vector<pat::Jet> > jets;
    iEvent.getByLabel(jsrc,jets);
    const pat::Jet & NrJet = (*jets)[iNr];
    nrjetPt = (*jets)[iNr].correctedP4(JECLab1_,JECLab2Nr_).pt()*L1Corrs[iNr];
    anajets.push_back(math::PtEtaPhiMLorentzVector(nrjetPt,NrJet.eta(),NrJet.phi(),NrJet.mass()));

    iAw = FindAwayJet(iEvent,jsrc,jetType,iNr,L1Corrs);
    if (iAw<0) return;

    const pat::Jet & AwJet = (*jets)[iAw];
    awjetPt = (*jets)[iAw].correctedP4(JECLab1_,JECLab2Aw_).pt()*L1Corrs[iAw];
    anajets.push_back(math::PtEtaPhiMLorentzVector(awjetPt,AwJet.eta(),AwJet.phi(),AwJet.mass()));
  }

  // For gen level don't consider JES for now
  if (jetType<2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jsrc,jets);
    const reco::Candidate & NrJet = (*jets)[iNr];
    nrjetPt = NrJet.pt();
    anajets.push_back(math::PtEtaPhiMLorentzVector(nrjetPt,NrJet.eta(),NrJet.phi(),NrJet.mass()));
    
    iAw = FindAwayJet(iEvent,jsrc,jetType,iNr,L1Corrs);
    if (iAw<0) return;

    const reco::Candidate & AwJet = (*jets)[iAw];
    awjetPt = AwJet.pt();
    anajets.push_back(math::PtEtaPhiMLorentzVector(awjetPt,AwJet.eta(),AwJet.phi(),AwJet.mass()));
  }
}

// ------------- Reference Jets ---------------
void DiJetAna::FindRefJets(const edm::Event& iEvent, Int_t refjetType, std::vector<math::PtEtaPhiMLorentzVector> & refjets)
{
  refjets.clear();


  // For matching use patjet matched
  Handle<vector<pat::Jet> > jets;
  iEvent.getByLabel(refjetsrc_,jets);
  // Just make a dummy L1 Correction for refjets for the moment
  vector<double> L1Corr_dum;
  for (unsigned j=0; j<(*jets).size();++j) L1Corr_dum.push_back(1.);

  // Find DJ from refjet collection
  nearRefJetPt_ = -99; awayRefJetPt_ = -99;
  if (refjetType<10)
    FindDiJet(iEvent,refjetsrc_,refjets,refjetType,L1Corr_dum,nearRefJetPt_,iNearRef_,awayRefJetPt_,iAwayRef_);
  else
    FindDiJet(iEvent,refjetsrc_,refjets,2,L1Corr_dum,nearRefJetPt_,iNearRef_,awayRefJetPt_,iAwayRef_);
  if (refjets.size()<2) return;

  // Print some refjet info
  if (numDJEvtSel_<=20) {
    cout << "Ref Jets (j"<<refjetType<<"): " << endl;
    cout << " refjetsrc_: " << refjetsrc_ << endl;
    cout << " sel nearRefJetPt_: " << nearRefJetPt_ << " iNearRef_: " << iNearRef_
      << " sel awayRefJetPt_: " << awayRefJetPt_ << " iAwayRef_: " << iAwayRef_ << endl;
  }

  // If not matching, we're done
  if (refjetType<10) return;


  if (refjetType<20) {
    // genjet matching
    const reco::GenJet * NrGJet = (*jets)[iNearRef_].genJet();
    const reco::GenJet * AwGJet = (*jets)[iAwayRef_].genJet();
    if (NrGJet && AwGJet) {
      // got matched jets
      if (refjetType==11) {
	// just use the pat matching directly
	refjets.clear();
	refjets.push_back(math::PtEtaPhiMLorentzVector(NrGJet->pt(),NrGJet->eta(),NrGJet->phi(),NrGJet->mass()));
	refjets.push_back(math::PtEtaPhiMLorentzVector(AwGJet->pt(),AwGJet->eta(),AwGJet->phi(),AwGJet->mass()));
      } else if (refjetType==12) {
	// check genjets matched to leading calojets are the same as
	// the current leading genjets
	if (anaJets_.size()<2) { cout << "warning no anajets" << endl; return; }// just safety
	Double_t dPt0 = TMath::Abs(anaJets_[0].pt()-TMath::Max(NrGJet->pt(),AwGJet->pt()));
	Double_t dPt1 = TMath::Abs(anaJets_[1].pt()-TMath::Min(NrGJet->pt(),AwGJet->pt()));
	//cout << " matched anaJetPt-refJetPt Nr: " << dPt0 << endl;
	//cout << " matched anaJetPt-refJetPt Aw: " << dPt1 << endl;
	if (dPt0>0.01||dPt1>0.01) { refjets.clear(); } // the calojet matched genjets are not the leading genjets
	else if (NrGJet->pt()<AwGJet->pt()) {
	  // with matched genjets to calo, we can use the found leading calojets
	  // just make sure the ordering is following genjet
	  std::swap(refjets[0],refjets[1]);
	}
      }
      // printout patjet based match result
      if (numDJEvtSel_<20) cout << " Matched: nearRefJet: " << refjets[0] << " awayRefJet: " << refjets[1] << endl;
    } else { // no match
      refjets.clear();
    }
  }
}

// ------------- Helpers ------------------
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
void DiJetAna::PrintDJEvent(const edm::Event& iEvent, const std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType, Int_t trkType)
{
  cout << endl << "=== Ana setup: ===" << endl;
  cout << "AnaJet: " << jetsrc_ << " with anaJetType: " << anaJetType_ << endl;
  cout << "RefJet: " << refjetsrc_ << " with refJetType: " << refJetType_ << endl;
  cout << "AnaTrk: " << trksrc_ << " with anaTrkType: " << anaTrkType_ << endl;
  if (jetType<=2 && verbosity_>=2) {
    edm::Handle<reco::CandidateView> jets;
    iEvent.getByLabel(jetsrc_,jets);
    cout << "jetType " << jetType << ", # jets: " << (*jets).size() << ". trkType " << trkType << endl;
    for (unsigned j=0; j<(*jets).size();++j) {
      const reco::Candidate & jet = (*jets)[j];
      if (jet.pt()>(nearJetPtMin_/2.)) {
	cout << "jet " << j;
	if (applyLAnaJEC_) cout << "  L1CorrEt: "<< jet.pt();
	cout <<" et|eta|phi: " << jet.pt() << "|" << jet.eta() << "|" << jet.phi() << endl;
      }
    }
  }
  Double_t ljdphi = TMath::Abs(reco::deltaPhi(anajets[0].phi(),anajets[1].phi()));
  cout << "JEC: " << JECLab1_ << " (Nr:" << JECLab2Nr_ << "|Aw:" << JECLab2Aw_ << ") "
    << " leading dijet - iNear: " << iNear_ << " " <<": "<< anajets[0]
    << "  iAway: " << iAway_ << " " << anajets[1] << endl;
  cout << "DiJet dphi: " << ljdphi << endl;
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
