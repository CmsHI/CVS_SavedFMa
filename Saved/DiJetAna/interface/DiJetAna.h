#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "Saved/DiJetAna/interface/TreeDiJetEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//
// class declaration
//

class DiJetAna : public edm::EDAnalyzer {
  public:
    explicit DiJetAna(const edm::ParameterSet&);
    ~DiJetAna();

    // class methods
    void InclJetAna(const edm::Event& iEvent, Int_t jetType, const std::vector<double> & anaJECs,
	TH1D * hPt, TH1D * hEta, TH1D * hPhi);
    void InclTrkAna(const edm::Event& iEvent, Int_t trkType);
    void FillEventInfo(const edm::Event& iEvent, TreeDiJetEventData & jd);
    double CalcFJL1Corr(const std::vector<double> & medianPt, const pat::Jet & jet);
    void FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd,
	const std::vector<double> & anaJECs,
	const std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t anajetType,
	const std::vector<math::PtEtaPhiMLorentzVector> & refjets, Int_t refjetType=-1);
    void LoadAnaJECs(const edm::Event & iEvent, const std::vector<pat::Jet> & jets, std::vector<double> & anaJECs);
    void  FillTrks(const edm::Event& iEvent, TreeDiJetEventData & jd,
	const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
	Int_t trkType);
    Int_t FindNearJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, const std::vector<double> & anaJECs);
    Int_t FindAwayJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, Int_t iNr, const std::vector<double> & anaJECs);
    // for data, where L1 correction is present
    void DiJetP4(const edm::Event& iEvent, const edm::InputTag & jsrc, std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType, const std::vector<double> & anaJECs, Double_t & nrjetPt, Int_t & iNr, Double_t & awjetPt, Int_t & iAw); 
    void FindRefJets(const edm::Event& iEvent, Int_t refjetType, std::vector<math::PtEtaPhiMLorentzVector> & refjets); 
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

    std::string centFile_;
    std::string centLabel_;
    edm::InputTag vtxsrc_;
    edm::InputTag jetsrc_;
    edm::InputTag refjetsrc_;
    edm::InputTag trksrc_;
    // ana setup
    Bool_t	  isMC_;
    Bool_t	  genOnly_;
    Bool_t	  evtAnaOnly_;
    Bool_t	  applyAnaJEC_;
    Double_t	  jetEtaMax_;
    Int_t	  nVtxTrkCut_;
    std::string	  JECLab1_;
    Double_t	  nearJetPtMin_;
    Double_t	  awayJetPtMin_;
    Double_t	  djDPhiMin_;
    Double_t	  trkPtMin_;
    Int_t	  anaJetType_;
    Int_t	  refJetType_;
    Int_t	  anaTrkType_;
    // ana vars
    Int_t	  numHiEvtSel_;
    Int_t	  numPreEvtSel_;
    Int_t	  numDJEvtSel_;
    std::vector<math::PtEtaPhiMLorentzVector> anaJets_;
    std::vector<math::PtEtaPhiMLorentzVector> refJets_;
    Int_t	  iNear_;
    Double_t	  nearJetPt_;
    Int_t	  iAway_;
    Double_t	  awayJetPt_;
    Int_t	  iNearRef_;
    Double_t	  nearRefJetPt_;
    Int_t	  iAwayRef_;
    Double_t	  awayRefJetPt_;
    CentralityBins::RunMap HFhitBinMap_;
    Int_t	  centBinBeg_;
    Int_t	  centBinEnd_;
    // debug
    Int_t	  verbosity_;
    // jec studies
    TF1		  *funcGaus_; //! Function parametrizing the jes uncertainty
    std::vector<double> anaJECs_;

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
    TH1D	  *hTrkPtEvtPreSel_;
    TH1D	  *hTrkEtaEvtPreSel_;
    TH2D	  *hTrkPtEtaEvtPreSel_;

    TTree	  *djTree_;
    TreeDiJetEventData djEvt_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
