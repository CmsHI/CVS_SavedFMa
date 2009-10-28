// pat
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
// ana
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"
// root
#include <TSystem.h>

using namespace jetana;
using namespace std;

bool HiJetAnaInput::isParton(const reco::GenParticle & p)
{
  // cf PhysicsTools/JetMCAlgos/plugins/PartonSelector.cc
  bool result=false;

  // check status
  int status = p.status();
  if (status!=3) return false;

  // check pdg id
  int flavor = abs(p.pdgId());
  if(flavor == 1 ||
      flavor == 2 ||
      flavor == 3 ||
      flavor == 4 ||
      flavor == 5 ||  
      flavor == 6 ||
      flavor == 21 )
    result = true;

  return result;
}

bool HiJetAnaInput::passBasicKin(const InputItem & cand)
{
  if (cand.pt()<5) return false;
  if (fabs(cand.eta())>3.0) return false;

  return true;
}

void HiJetAnaInput::LoadJets(JetType jetType, bool corrected)
{
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

    fwlite::Handle<std::vector<pat::Jet> > jets;
    // - cf http://msdn.microsoft.com/en-us/library/2dzy4k6e%28VS.80%29.aspx
    //   * for example of using enum switch
    switch (jetType) {
      // - cf instantiation in case
      //   * http://docs.freebsd.org/info/g++FAQ/g++FAQ.info.jump_crosses_initialization.html
      case PARTON:
	{
	  cout << "load parton" << endl;
	  // fwlight::Handle to gen particles
	  fwlite::Handle<std::vector<reco::GenParticle> > particles;
	  particles.getByLabel(*eventCont_, "hiGenParticles");
	  for (unsigned ip=0; ip<particles->size(); ++ip) {
	    // select partons
	    if ( isParton((*particles)[ip]) && passBasicKin((*particles)[ip].p4()) )
	      jets_.push_back((*particles)[ip].p4());
	  }
	  break;
	}
      case GENJET:
	{
	  cout << "load genjet" << endl;
	  // fwlite::Handle to genjet collection
	  fwlite::Handle<std::vector<reco::GenJet> > gjets;
	  gjets.getByLabel(*eventCont_,"iterativeCone5HiGenJets");
	  // loop genjet collection
	  for (unsigned j=0; j<gjets->size(); ++j) {
	    if ( passBasicKin((*gjets)[j].p4()) )
	      jets_.push_back((*gjets)[j].p4());
	  }
	  break;
	}
      case PATJET: 
	{
	  cout << "load patjet" << endl;
	  jets.getByLabel(*eventCont_, "selectedLayer1Jets");
	  for(unsigned j=0; j<jets->size(); ++j){
	    // select jets
	    if ( passBasicKin((*jets)[j].p4()) ) {
	      if (!corrected)
		jets_.push_back((*jets)[j].p4());
	      // implement else!
	    }
	  }
	  break;
	}
      default:
	cout << "jet type: " << jetType  << endl;
    }
}
