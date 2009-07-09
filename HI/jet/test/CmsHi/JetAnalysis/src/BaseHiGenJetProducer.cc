// File: BaseHiGenJetProducer.cc
// Author: Y.Yilmaz, 2008
// $Id: BaseHiGenJetProducer.cc,v 1.7 2009/06/27 18:23:43 yilmaz Exp $
//--------------------------------------------
#include <memory>

#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Provenance/interface/ProductID.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "RecoJets/JetAlgorithms/interface/JetRecoTypes.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"
#include "RecoJets/JetAlgorithms/interface/JetMaker.h"
#include "RecoJets/JetAlgorithms/interface/ProtoJet.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "SimDataFormats/HiGenData/interface/SubEventMap.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "CmsHi/JetAnalysis/interface/BaseHiGenJetProducer.h"

using namespace std;
using namespace reco;
using namespace JetReco;

namespace {

   bool checkHydro(GenParticleRef& p){

      const Candidate* m1 = p->mother();
      while(m1){
	 int pdg = abs(m1->pdgId());
	 LogDebug("SubEventMothers")<<"Pdg ID : "<<pdg<<endl;
	 if(pdg < 9 || pdg == 21){
	    LogDebug("SubEventMothers")<<"Parton Found! Pdg ID : "<<pdg<<endl;
	    return false;
	 }
         const Candidate* m = m1->mother();
	 m1 = m;
      }
      return true;
   }
   
   const bool debug = false;

  bool makeCaloJet (const string& fTag) {
    return fTag == "CaloJet";
  }
  bool makePFJet (const string& fTag) {
    return fTag == "PFJet";
  }
  bool makeGenJet (const string& fTag) {
    return fTag == "GenJet";
  }
  bool makeBasicJet (const string& fTag) {
    return fTag == "BasicJet";
  }

  bool makeGenericJet (const string& fTag) {
    return !makeCaloJet (fTag) && !makePFJet (fTag) && !makeGenJet (fTag) && !makeBasicJet (fTag);
  }

  template <class T>  
  void dumpJets (const T& fJets) {
    for (unsigned i = 0; i < fJets.size(); ++i) {
      std::cout<< "Jet # " << i << std::endl << fJets[i].print();
    }
  }

  void copyVariables (const ProtoJet& fProtojet, reco::Jet* fJet) {
    fJet->setJetArea (fProtojet.jetArea ());
    fJet->setPileup (fProtojet.pileup ());
    fJet->setNPasses (fProtojet.nPasses ());
  }

   void copyConstituents (const JetReco::InputCollection& fConstituents, const edm::View <Candidate>& fInput, reco::Jet* fJet) {
      // put constituents
      for (unsigned iConstituent = 0; iConstituent < fConstituents.size (); ++iConstituent) {
	 fJet->addDaughter (fInput.ptrAt (fConstituents[iConstituent].index ()));
      }
   }
   
}

namespace cms
{
  // Constructor takes input parameters now: to be replaced with parameter set.
  BaseHiGenJetProducer::BaseHiGenJetProducer(const edm::ParameterSet& conf)
    : mSrc(conf.getParameter<edm::InputTag>( "src")),
      mapSrc(conf.getParameter<edm::InputTag>( "srcMap")),
      mJetType (conf.getParameter<string>( "jetType")),
      mVerbose (conf.getUntrackedParameter<bool>("verbose", false)),
      mEtInputCut (conf.getParameter<double>("inputEtMin")),
      mEInputCut (conf.getParameter<double>("inputEMin")),
      ignoreHydro_ (conf.getUntrackedParameter<bool>("ignoreHydro", true)),
      nMax_(conf.getUntrackedParameter<double>("maxParticles", 2000))
  {
    std::string alias = conf.getUntrackedParameter<string>( "alias", conf.getParameter<std::string>("@module_label"));
    if (makeCaloJet (mJetType)) {
      produces<CaloJetCollection>().setBranchAlias (alias);
    }
    else if (makePFJet (mJetType)) produces<PFJetCollection>().setBranchAlias (alias);
    else if (makeGenJet (mJetType)) produces<GenJetCollection>().setBranchAlias (alias);
    else if (makeBasicJet (mJetType)) produces<BasicJetCollection>().setBranchAlias (alias);
//     else if (makeGenericJet (mJetType)) produces<GenericJetCollection>().setBranchAlias (alias);
  }

  // Virtual destructor needed.
  BaseHiGenJetProducer::~BaseHiGenJetProducer() { } 

   void BaseHiGenJetProducer::beginJob(const edm::EventSetup& fSetup)
   {
      fSetup.getData(mPdt);
   }

  // Functions that gets called by framework every event
  void BaseHiGenJetProducer::produce(edm::Event& e, const edm::EventSetup& fSetup)
  {
     using namespace reco;

     edm::Handle< edm::View<Candidate> > inputHandle;
     e.getByLabel(mSrc,inputHandle);

     edm::Handle<edm::SubEventMap> subs;
     e.getByLabel(mapSrc,subs);

     vector <ProtoJet> output;
     vector<JetReco::InputCollection> inputs;

     vector<int> nsubparticle;
     vector<int> hydroTag;

     for (unsigned i = 0; i < inputHandle->size(); ++i) {
	
	GenParticleRef pref = inputHandle->refAt(i).castTo<GenParticleRef>();
	int subevent = (*subs)[pref];
        LogDebug("SubEventJets")<<"inputs size "<<inputs.size()<<" subevent "<<subevent;
	
	if(subevent >= inputs.size()){ 
	   //	   hydroTag.push_back(-1);
	   hydroTag.resize(subevent+1);
	   hydroTag[subevent] = -1;
	   inputs.resize(subevent+1);
	   nsubparticle.resize(subevent+1);
	}
	
	if(hydroTag[subevent]<0) hydroTag[subevent] = (int)checkHydro(pref);

	if(nsubparticle[subevent]> nMax_ && hydroTag[subevent] != 1){
           LogDebug("JetsInHydro")<<"More particles than maximum particles cut, although not previously identified as sub-event, Sub-Event :  "<<subevent;
	   
	   hydroTag[subevent] = 1;
	}
	
	
	if(hydroTag[subevent] != 1){
	   (inputs[subevent]).push_back(JetReco::InputItem(&((*inputHandle)[i]),i));
	   nsubparticle[subevent]++;
	}
     }
     
     int nsub = inputs.size();

     for(int isub = 0; isub < nsub; ++isub){
	cout<<"Processing Sub-Event : "<<isub<<endl;
	JetReco::InputCollection & input = inputs[isub];
	if(hydroTag[isub] == 1){
	   cout<<"Sub-Event number "<<isub<<" with more than "<<input.size()<<" particles, skipped as background event. Number of total sub-events: "<<nsub<<endl;
	}else{

	   if (mVerbose) {
	      std::cout << "BaseJetProducer::produce-> INPUT COLLECTION selected from" << mSrc
			<< " with ET > " << mEtInputCut << " and/or E > " << mEInputCut << std::endl;
	      for (unsigned index = 0; index < input.size(); ++index) {
		 std::cout << "  Input " << index << ", px/py/pz/pt/e: "
			   << input[index]->px() << '/' << input[index]->py() << '/' << input[index]->pz() << '/'
			   << input[index]->pt() << '/' << input[index]->energy()
			   << std::endl;
	      }
	   }

	   if (input.empty ()) {
	      edm::LogInfo ("Empty Event") << "empty input for jet algorithm: bypassing..." << std::endl;
	   }
	   else {
	      cout<<"Algorithm running on sub-event..."<<endl;
	      this->runAlgorithm (input, &output);
	   }
	}
     }

     // produce output collection 
     if (mVerbose) {
	std::cout << "OUTPUT JET COLLECTION:" << std::endl;
     }
     reco::Jet::Point vertex (0,0,0); // do not have true vertex yet, use default 
	
	// make sure protojets are sorted                                         
	sortByPt (&output);
	if (makeGenJet (mJetType)) {
	   
	   cout<<"Saving GenJets..."<<endl;
	   
	   auto_ptr<GenJetCollection> jets (new GenJetCollection);
	   jets->reserve(output.size());
	   for (unsigned iJet = 0; iJet < output.size (); ++iJet) {
	      ProtoJet* protojet = &(output [iJet]);
	      const JetReco::InputCollection& constituents = protojet->getTowerList();
	      GenJet::Specific specific;
	      JetMaker::makeSpecific (constituents, &specific);
	      jets->push_back (GenJet (protojet->p4(), vertex, specific));
	      Jet* newJet = &(jets->back());
	      copyConstituents (constituents, *inputHandle, newJet);      
	      copyVariables (*protojet, newJet);
	   }

	   if (mVerbose) dumpJets (*jets);
	   e.put(jets);
	}
  }
}

