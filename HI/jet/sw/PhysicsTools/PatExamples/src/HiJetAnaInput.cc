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

void HiJetAnaInput::LoadJets(JetType jetType)
{
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

    fwlite::Handle<std::vector<pat::Jet> > jets;
    // - cf http://msdn.microsoft.com/en-us/library/2dzy4k6e%28VS.80%29.aspx
    //   * for example of using enum switch
    switch (jetType) {
      // - cf instantiation in case
      //   * http://docs.freebsd.org/info/g++FAQ/g++FAQ.info.jump_crosses_initialization.html
      case PATJET: 
	{
	  cout << "load patjet" << endl;
	  jets.getByLabel(*event_, "selectedLayer1Jets");
	  for(unsigned i=0; i<jets->size(); ++i){
	    // select jets
	    if ( passBasicKin((*jets)[i].p4()) )
	      jets_.push_back((*jets)[i].p4());
	  }
	  break;
	}
      case PARTON:
	{
	  cout << "load parton" << endl;
	  // fwlight::Handle to gen particles
	  fwlite::Handle<std::vector<reco::GenParticle> > particles;
	  particles.getByLabel(*event_, "hiGenParticles");
	  for (unsigned ip=0; ip<particles->size(); ++ip) {
	    double abseta = fabs((*particles)[ip].eta());
	    // select partons
	    if ( isParton((*particles)[ip]) && passBasicKin((*particles)[ip].p4()) )
	      jets_.push_back((*particles)[ip].p4());
	  }
	}
	break;
      default:
	cout << "jet type: " << jetType  << endl;
    }
}
