#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CmsHi/DiJetAna/interface/TreeDiJetEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"


//
// class declaration
//

class DiJetAna : public edm::EDAnalyzer {
  public:
    explicit DiJetAna(const edm::ParameterSet&);
    ~DiJetAna();

    // class methods
    void InclJetAna(const edm::Event& iEvent, Int_t jetType,
	TH1D * hPt, TH1D * hEta, TH1D * hPhi);
    void InclTrkAna(const edm::Event& iEvent, Int_t trkType);
    void FillEventInfo(const edm::Event& iEvent, TreeDiJetEventData & jd);
    void FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd,
	std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t anajetType,
	std::vector<math::PtEtaPhiMLorentzVector> & refjets, Int_t refjetType=-1);
    void  FillTrks(const edm::Event& iEvent, TreeDiJetEventData & jd,
	std::vector<math::PtEtaPhiMLorentzVector> & anajets,
	std::vector<math::PtEtaPhiMLorentzVector> & refjets,
	Int_t trkType);
    Int_t FindNearJet(const edm::Event& iEvent, Int_t jetType=2);
    Int_t FindAwayJet(const edm::Event& iEvent, Int_t jetType=2);
    void FindDiJet(const edm::Event& iEvent, std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType=2); 
    void FindRefJets(const edm::Event& iEvent, Int_t anajetType, std::vector<math::PtEtaPhiMLorentzVector> & refjets); 
    Bool_t GoodAnaTrk(const reco::Track & trk);
    Bool_t GoodAnaTrkParticle(const reco::Candidate & p, Int_t trkType);
    void PrintDJEvent(const edm::Event& iEvent, const std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType=2, Int_t trkType=2);
    void PrintTrks(const edm::Event& iEvent, Int_t trkType=2);

  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    edm::Service<TFileService> fs;

    edm::InputTag vtxsrc_;
    edm::InputTag jetsrc_;
    edm::InputTag trksrc_;
    // ana setup
    Bool_t	  isMC_;
    Double_t	  jetEtaMax_;
    Int_t	  nVtxTrkCut_;
    Int_t	  doJEC_;
    Double_t	  nearJetPtMin_;
    Double_t	  awayJetPtMin_;
    Double_t	  trkPtMin_;
    Int_t	  anaJetType_;
    Int_t	  refJetType_;
    Int_t	  anaTrkType_;
    // ana vars
    Int_t	  numPreEvtSel_;
    Int_t	  numDJEvtSel_;
    std::vector<math::PtEtaPhiMLorentzVector> anaJets_;
    std::vector<math::PtEtaPhiMLorentzVector> refJets_;
    Int_t	  iNear_;
    Double_t	  nearJetPt_;
    Int_t	  iAway_;
    Double_t	  awayJetPt_;

    // ana data
    // -- evt --
    TH1D	  *hNumVtx_;
    TH1D	  *hVtxZEvtSel_;
    TH1D	  *hVtxNumTrksPreSel_;
    TH1D	  *hVtxNumTrksEvtSel_;
    // -- jet --
    TH1D	  *hJetPtPreSel_;
    TH1D	  *hJetEtaPreSel_;
    TH1D	  *hJetPhiPreSel_;
    TH1D	  *hJetPtDJSel_;
    TH1D	  *hJetEtaDJSel_;
    TH1D	  *hJetPhiDJSel_;
    // -- trk --
    TH1D	  *hTrkPtDJEvtSel_;
    TH1D	  *hTrkEtaDJEvtSel_;
    TH2D	  *hTrkPtEtaDJEvtSel_;

    TTree	  *djTree_;
    TreeDiJetEventData djEvt_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
