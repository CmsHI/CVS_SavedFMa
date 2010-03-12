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

//#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
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
#include "QCDAnalysis/HighPtJetAnalysis/interface/Utilities.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#endif

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TreeDiJetEventData.h"
using namespace std;

void printJet(int verbosity=1){
  //----- input files (900 GeV data) -----
  vector<string> fileNames;
  string fileDir = "/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/skim_v3";
  cout << "directory: '" << fileDir << "'" << endl;
  for(int ifile=1; ifile<=1; ifile++) {
    TString name = Form("jetAnaSkimAOD_%d.root",ifile);
    cout << "  adding file: " << name.Data() << endl;
    fileNames.push_back(fileDir + "/" + name.Data());
  }
  fwlite::ChainEvent event(fileNames);

  //----- loop over events -----
  unsigned int iEvent=0;
  unsigned int maxEvent=10;
  for(event.toBegin(); !event.atEnd() && iEvent<maxEvent; ++event, ++iEvent){
    edm::EventBase const & ev = event;
    if( iEvent % 1 == 0 ) cout << "Processing " << iEvent<< "th event: "
      << "run " << event.id().run() 
	<< ", lumi " << event.luminosityBlock() 
	<< ", evt " << event.id().event() << endl;

    /*
    //----- loop over jets fwlite handle------
    fwlite::Handle<reco::CaloJetCollection> jets;
    jets.getByLabel(event,"ak5CaloJets");
    // find leading jet based on corrected pt
    for (unsigned int j=0; j<(*jets).size();++j) {
      const reco::Jet & jet = (*jets)[j];
      // apply JEC
      Double_t corrPt = jet.pt();
      cout << " Jet " << j << ": Pt " << corrPt << endl;
    }
    */

    //----- loop over jets edm handle -----
    edm::Handle<reco::CaloJetCollection> jets;
    ev.getByLabel(edm::InputTag("ak5CaloJets"),jets);
    fwlite::Handle<edm::ValueMap<reco::JetID> > jetsID;
    jetsID.getByLabel(event,"ak5JetID");
    cout << "# of jets: " << (*jets).size() << endl;
    for (unsigned int j=0; j<(*jets).size(); ++j) {
      const reco::Jet & jet = (*jets)[j];
      Double_t corrPt = jet.pt();
      cout << " Jet " << j << ": Pt " << corrPt << endl;
      // Inclusive Jet Analysis
      //edm::Ref<reco::CaloJetCollection> ref(jets,j);
      edm::RefToBase<reco::Jet> jetRef(edm::Ref<reco::CaloJetCollection>(jets,j));
      cout << "   n90Hits: " << (int)(*jetsID)[jetRef].n90Hits << endl;
    }
  }
  
  cout << endl << "================ Ana Process Summaries =============" << endl;
  cout << "Number of events processed : "<<iEvent<<endl;
  cout << "====================================================" << endl;
}
