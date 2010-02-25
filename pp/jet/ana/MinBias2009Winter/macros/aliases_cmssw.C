// TChain * Events = new TChain("Events","reco tree")
// Events->Add("rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/*.root")
// Events->Add("rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0021/*.root")
// Events->Add("rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0019/*.root")

void aliases_cmssw(TTree* Events)
{
  Events->SetAlias("run","EventAuxiliary.run()");
  Events->SetAlias("goodRuns","(run==123596||run==123615||run==123732||run==123815||run==123818||run==123906||run==123908||run==124008||run==124009||run==124020||run==124022||run==124023||run==124024||run==124025||run==124027||run==124030)");
  // vtx:
  //   z()
  //   ndof_
  //   isFake()
  Events->SetAlias("vtx","recoVertexs_offlinePrimaryVertices__RERECO.obj");
  Events->SetAlias("goodVtx","(!vtx.isFake() && vtx.ndof_>=5 && abs(vtx.z())<15)");

  // jet
  //   pt(), eta(), phi()
  Events->SetAlias("jet","recoCaloJets_ak5CaloJets__RERECO.obj");

  // tracks
  // vz()
  Events->SetAlias("trk","recoTracks_generalTracks__RERECO.obj");
}
