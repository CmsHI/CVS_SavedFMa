// File: BaseHiGenJetProducer.cc
// Author: F.Ratnikov UMd Aug 22, 2006
// $Id: BaseHiGenJetProducer.cc,v 1.5 2009/01/14 11:09:11 yilmaz Exp $
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

#include "RecoHI/HiJetAlgos/interface/BaseHiGenJetProducer.h"

#include "SimDataFormats/HiGenData/interface/SubEventMap.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

using namespace std;
using namespace reco;
using namespace JetReco;

namespace {
  const bool debug = false;

   bool selectForJet(const reco::GenParticle& par){
      if(par.status() != 1) return false;

      // Below to be FIXED!!! What is a Jet???
      if(abs(par.pdgId()) == 11 || abs(par.pdgId()) == 12 ||
	 abs(par.pdgId()) == 13 ||abs(par.pdgId()) == 14 ||
	 abs(par.pdgId()) == 15 ||abs(par.pdgId()) == 16 
	 ) return false;

	 return true;
   }


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
      std::cout << "Jet # " << i << std::endl << fJets[i].print();
    }
  }

  void copyVariables (const ProtoJet& fProtojet, reco::Jet* fJet) {
    fJet->setJetArea (fProtojet.jetArea ());
    fJet->setPileup (fProtojet.pileup ());
    fJet->setNPasses (fProtojet.nPasses ());
  }

  void copyConstituents (const JetReco::InputCollection& fConstituents, const reco::GenParticleCollection& fInput, reco::Jet* fJet) {
    // put constituents
    for (unsigned iConstituent = 0; iConstituent < fConstituents.size (); ++iConstituent) {
       fJet->addDaughter (fInput[fConstituents[iConstituent].index ()].sourceCandidatePtr(0));
    }
  }

}

namespace cms
{
  // Constructor takes input parameters now: to be replaced with parameter set.
  BaseHiGenJetProducer::BaseHiGenJetProducer(const edm::ParameterSet& conf)
    : mSrc(conf.getUntrackedParameter<edm::InputTag>( "src", edm::InputTag("hiGenParticles") )),
      mJetType (conf.getUntrackedParameter<string>( "jetType", "GenJet")),
      mVerbose (conf.getUntrackedParameter<bool>("verbose", false)),
      mEtInputCut (conf.getParameter<double>("inputEtMin")),
      mEInputCut (conf.getParameter<double>("inputEMin")),
      skipLastSubEvent_ (conf.getUntrackedParameter<bool>("skipLastSubEvent", true))
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

     edm::Handle<GenParticleCollection> inputHandle;
     e.getByLabel(mSrc,inputHandle);

     edm::Handle<edm::SubEventMap> subs;
     e.getByLabel(mSrc,subs);

     vector <ProtoJet> output;
     vector<JetReco::InputCollection> inputs;

     for (unsigned i = 0; i < inputHandle->size(); ++i) {

	const GenParticle & p = (*inputHandle)[i];
        if(!selectForJet(p)) continue;
	int subevent = (*subs)[GenParticleRef(inputHandle,i)];
        // cout<<"inputs size "<<inputs.size()<<" subevent "<<subevent<<endl;

	if(subevent >= inputs.size()) inputs.resize(subevent+1);

	//	cout<<"inputs size "<<inputs.size()<<" subevent "<<subevent<<endl;

	(inputs[subevent]).push_back(JetReco::InputItem(&p,i));
     }

     int nsub = inputs.size();
     for(int isub = 0; isub < nsub; ++isub){
	cout<<"Processing Sub-Event : "<<isub<<endl;
	JetReco::InputCollection & input = inputs[isub];
	if(skipLastSubEvent_ && isub == nsub-1){
	   cout<<"Sub-Event number "<<isub<<" with "<<input.size()<<" particles, skipped as background event."<<endl;
	}else{
	   cout << "           number of particles in subevent to make genjet: " << input.size() << endl;

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

