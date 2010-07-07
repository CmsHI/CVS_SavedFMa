#include "TTree.h"
#include <cassert>

void aliases_aod(TTree * Events, int doMC=0)
{ 
  if (!Events) cout << "Events not defined, please check" << endl;
  assert(Events);

  TString pname("RECO");

  // Event
  // vtx: x(),y(),z(),nTracks()
  Events->SetAlias("vtx",Form("recoVertexs_hiSelectedVertex__%s.obj",pname.Data()));
  // cent: EtHFhitSum()
  Events->SetAlias("cent",Form("recoCentrality_hiCentrality__%s.obj",pname.Data()));

  pname="PAT";
  // Jet
  Events->SetAlias("jet",Form("patJets_patJets__%s.obj",pname.Data()));

  // Trk
  Events->SetAlias("trk",Form("recoRecoChargedCandidates_allTracks__%s.obj",pname.Data()));
}
