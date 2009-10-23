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
	    jets_.push_back((*jets)[i].p4());
	    cout << jets_.back() << endl;
	  }
	  break;
	}
      default:
	cout << "jet type: " << jetType  << endl;
    }
}
