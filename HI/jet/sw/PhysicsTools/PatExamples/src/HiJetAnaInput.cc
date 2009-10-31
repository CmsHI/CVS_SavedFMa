// pat
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
// ana
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"
// root
#include <TSystem.h>
// helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"

using namespace jetana;
using namespace std;

// Constructor =========================================
HiJetAnaInput::HiJetAnaInput(fwlite::EventContainer * ec, HiDiJetAnaConfig * anacfg) :
  verbosity_(0)
{
  eventCont_=ec;
  anacfg_=anacfg;
}


// Helpers methods =====================================
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

bool HiJetAnaInput::isTrack(const reco::GenParticle & p)
{
  if (p.status()!=1) return false;
  if (p.charge()==0) return false;

  return true;
}

bool HiJetAnaInput::passBasicJetKin(const InputItem & cand)
{
  float jEtMin= (anacfg_->doJEC_)? anacfg_->jetEtMin_ : anacfg_->jetEtUMin_;
  if (cand.pt()<jEtMin) return false;
  // cms HB+HE calo eta limit is 3.0
  // but to accommodate for tracker keep to 2 for FF ana
  if (fabs(cand.eta())>2.0) return false;

  return true;
}
bool HiJetAnaInput::passBasicTrackKin(const AnaInputItem & cand)
{
  if (cand.pt()<anacfg_->trackPtMin_) return false;
  if (fabs(cand.eta())>2.5) return false;

  return true;
}

// Main Methods ============================================
void HiJetAnaInput::LoadJets(JetType jetType)
{
  // - cf http://msdn.microsoft.com/en-us/library/2dzy4k6e%28VS.80%29.aspx
  //   * for example of using enum switch
  switch (jetType) {
    // - cf instantiation in case
    //   * http://docs.freebsd.org/info/g++FAQ/g++FAQ.info.jump_crosses_initialization.html
    case PARTON:
      {
	if (verbosity_>=1) cout << "load parton" << endl;
	// fwlight::Handle to gen particles
	fwlite::Handle<std::vector<reco::GenParticle> > particles;
	particles.getByLabel(*eventCont_, "hiGenParticles");
	for (unsigned ip=0; ip<particles->size(); ++ip) {
	  // select partons
	  if ( isParton((*particles)[ip]) && passBasicJetKin((*particles)[ip].p4()) )
	    jets_.push_back((*particles)[ip].p4());
	}
	break;
      }
    case GENJET:
      {
	if (verbosity_>=1) cout << "load genjet" << endl;
	// fwlite::Handle to genjet collection
	fwlite::Handle<std::vector<reco::GenJet> > gjets;
	gjets.getByLabel(*eventCont_,"iterativeCone5HiGenJets");
	// loop genjet collection
	for (unsigned j=0; j<gjets->size(); ++j) {
	  if ( passBasicJetKin((*gjets)[j].p4()) )
	    jets_.push_back((*gjets)[j].p4());
	}
	break;
      }
    case PATJET: 
      {
	if (verbosity_>=1) cout << "load patjet" << endl;
	fwlite::Handle<std::vector<pat::Jet> > jets;
	jets.getByLabel(*eventCont_, "selectedLayer1Jets");
	//cout << "do jet correction?: " << anacfg_->doJEC_ << endl;
	for(unsigned j=0; j<jets->size(); ++j){
	  // select jets
	  if ( passBasicJetKin((*jets)[j].p4()) ) {
	    if (anacfg_->doJEC_)
	      jets_.push_back((*jets)[j].p4());
	    else {
	      jets_.push_back((*jets)[j].correctedP4("raw"));
	      //cout << "uncorr jet: " << jets_.back() << endl;
	    }
	  }
	}
	break;
      }
    default:
      cout << "bad jet type: " << jetType  << endl;
  }
}

void HiJetAnaInput::LoadTracks(TrackType trackType)
{
  // - cf http://msdn.microsoft.com/en-us/library/2dzy4k6e%28VS.80%29.aspx
  //   * for example of using enum switch
  switch (trackType) {
    // - cf instantiation in case
    //   * http://docs.freebsd.org/info/g++FAQ/g++FAQ.info.jump_crosses_initialization.html
    case PARTICLE:
      {
	if (verbosity_>=1) cout << "load particles" << endl;
	// fwlight::Handle to gen particles
	fwlite::Handle<std::vector<reco::GenParticle> > particles;
	particles.getByLabel(*eventCont_, "hiGenParticles");
	for (unsigned ip=0; ip<particles->size(); ++ip) {
	  // select particles
	  if ( isTrack((*particles)[ip]) && passBasicTrackKin((*particles)[ip].p4()) )
	    tracks_.push_back((*particles)[ip].p4());
	}
	break;
      }
    case TRACK:
      {
	if (verbosity_>=1) cout << "load track" << endl;
	fwlite::Handle<std::vector<reco::Track> > tracks;
	tracks.getByLabel(*eventCont_, "hiSelectedTracks");
	for (unsigned it=0; it<tracks->size(); ++it) {
	  AnaInputItem anatrack((*tracks)[it].px(),
		                (*tracks)[it].py(),
				(*tracks)[it].pz(),
				(*tracks)[it].p()
				);
	  if ( passBasicTrackKin(anatrack) )
	    tracks_.push_back(anatrack);
	}
	break;
      }
    default:
      cout << "bad track type: " << trackType  << endl;
  }

  // done
  if (verbosity_>=2)
    mystd::print_elements(tracks_);
}
