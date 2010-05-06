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

//
// class declaration
//

class DiJetAna : public edm::EDAnalyzer {
  public:
    explicit DiJetAna(const edm::ParameterSet&);
    ~DiJetAna();

    // class methods
    void  FillJets(const edm::Event& iEvent, TreeDiJetEventData & jd_, Int_t jetType=2, Int_t jetRefType=-1);
    Int_t FindNearJet(const edm::Event& iEvent, Int_t jetType=2);
    Int_t FindAwayJet(const edm::Event& iEvent, Int_t jetType=2);
    void PrintDJEvent(const edm::Event& iEvent);

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
    // ana vars
    Int_t	  numPreEvtSel_;
    Int_t	  numDJEvtSel_;
    std::vector<math::PtEtaPhiMLorentzVectorF> anaJets_;
    Int_t	  iNear_;
    Double_t	  nearJetPt_;
    Int_t	  iAway_;
    Double_t	  awayJetPt_;

    // ana data
    TH1D	  *hNumVtx_;
    TH1D	  *hVtxZEvtSel_;
    TH1D	  *hVtxNumTrksPreSel_;
    TH1D	  *hVtxNumTrksEvtSel_;
    TH1D	  *hJetPtPreSel_;
    TH1D	  *hJetEtaPreSel_;
    TH1D	  *hJetPhiPreSel_;
    TH1D	  *hTrkPtPreSel_;
    TH1D	  *hTrkEtaPreSel_;
    TH2D	  *hTrkPtEtaPreSel_;

    TTree	  *calojTree_;
    TTree	  *calojGenjTree_;
    TTree	  *calojPtnjTree_;
    TTree	  *genjCalojTree_;
    TTree	  *ptnjCalojTree_;
    TreeDiJetEventData calojData_;
    TreeDiJetEventData calojGenjData_;
    TreeDiJetEventData calojPtnjData_;
    TreeDiJetEventData genjCalojData_;
    TreeDiJetEventData ptnjCalojData_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
