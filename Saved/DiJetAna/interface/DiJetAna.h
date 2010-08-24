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
    double CalcFJL1Corr(const std::vector<double> & medianPt, const pat::Jet & jet);

    void DiJetP4(const edm::Event& iEvent, const edm::InputTag & jsrc, std::vector<math::PtEtaPhiMLorentzVector> & anajets,
	Int_t jetType, const std::vector<double> & anaJECs, Int_t & iNr, Int_t & iAw); 

    void FillEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup, TreeDiJetEventData & jd);
    void FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd,
	const std::vector<double> & anaJECs,
	const std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t anajetType,
	const std::vector<math::PtEtaPhiMLorentzVector> & refjets, Int_t refjetType=-1);
    void FillTrigInfo(const edm::Event& iEvent, TreeDiJetEventData & jd);
    void FillTrks(const edm::Event& iEvent, TreeDiJetEventData & jd,
	const std::vector<math::PtEtaPhiMLorentzVector> & anajets,
	Int_t trkType);
    void FillVtx(const edm::Event& iEvent, TreeDiJetEventData & jd);

    Int_t FindNearJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, const std::vector<double> & anaJECs);
    Int_t FindAwayJet(const edm::Event& iEvent, const edm::InputTag & jsrc, Int_t jetType, Int_t iNr, const std::vector<double> & anaJECs);
    void FindRefJets(const edm::Event& iEvent, Int_t refjetType, std::vector<math::PtEtaPhiMLorentzVector> & refjets); 

    Bool_t GoodAnaTrk(const reco::Track & trk);
    Bool_t GoodAnaTrkParticle(const reco::Candidate & p, Int_t trkType);

    void InclJetAna(const edm::Event& iEvent, Int_t jetType, const std::vector<double> & anaJECs,
	TH1D * hPt, TH1D * hEta, TH1D * hPhi);
    void InclTrkAna(const edm::Event& iEvent, Int_t trkType);

    void LoadAnaJECs(const edm::Event & iEvent, const std::vector<pat::Jet> & jets, std::vector<double> & anaJECs);

    void PrintDJEvent(const edm::Event& iEvent, const std::vector<math::PtEtaPhiMLorentzVector> & anajets, Int_t jetType=2, Int_t trkType=2);
    void PrintTrks(const edm::Event& iEvent, Int_t trkType=2);

  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    edm::Service<TFileService> fs;

    // === Ana setup ===
    Bool_t	  isMC_;
    Bool_t	  genOnly_;
    // Event Info
    edm::InputTag vtxsrc_;
    // Jet
    edm::InputTag jetsrc_;
    Int_t	  anaJetType_;
    Double_t	  jetEtaMax_;
    // jet energy correction
    std::string	  JECLab1_;
    Bool_t	  doFJL1Corr_;
    // jet mc matching
    edm::InputTag refjetsrc_;
    Int_t	  refJetType_;
    // di-jet
    Double_t	  djDPhiMin_;
    // trk selection
    Double_t	  nearJetPtMin_;
    edm::InputTag trksrc_;
    Int_t	  anaTrkType_;
    Double_t	  trkPtMin_;
    // debug
    Int_t	  verbosity_;

    // Ana vars
    Int_t	  numHiEvtSel_;
    Int_t	  numEvtSel_;
    Int_t	  numJetEvtSel_;
    std::vector<std::string> hltNames_;
    std::vector<math::PtEtaPhiMLorentzVector> anaJets_;
    std::vector<math::PtEtaPhiMLorentzVector> refJets_;
    Int_t	  iNear_;
    Int_t	  iAway_;
    Int_t	  iNearRef_;
    Int_t	  iAwayRef_;
    const CentralityBins * cbins_;

    // jec studies
    TF1		  *funcGaus_; //! Function parametrizing the jes uncertainty
    std::vector<double> anaJECs_;

    // === Ana data ===
    // -- evt --
    TH1D	  *hNumVtx_;
    TH1D	  *hVtxNumTrksEvtPreSel_;
    TH1D	  *hVtxZEvtPreSel_;
    // -- jet --
    TH1D	  *hJetPtEvtPreSel_;
    TH1D	  *hJetEtaEvtPreSel_;
    TH1D	  *hJetPhiEvtPreSel_;
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
