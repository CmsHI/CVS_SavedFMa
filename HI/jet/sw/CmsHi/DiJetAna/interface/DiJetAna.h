#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1D.h"

//
// class declaration
//

class DiJetAna : public edm::EDAnalyzer {
  public:
    explicit DiJetAna(const edm::ParameterSet&);
    ~DiJetAna();


  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    edm::Service<TFileService> fs;

    edm::InputTag vtxsrc_;
    edm::InputTag jetsrc_;
    edm::InputTag trksrc_;
    bool isMC_;
    double  jetEtaMax_;
    int32_t nVtxTrkCut_;

    // ana data
    TH1D	  *hNumVtx_;
    TH1D	  *hVtxZEvtSel_;
    TH1D	  *hVtxNumTrksPreSel_;
    TH1D	  *hVtxNumTrksEvtSel_;
    TH1D	  *hJetPtPreSel_;
    TH1D	  *hTrkPtPreSel_;

    TTree	  *calojTree_;
    TTree	  *calojGenjTree_;
    TTree	  *calojPtnjTree_;
    TTree	  *genjCalojTree_;
    TTree	  *ptnjCalojTree_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

