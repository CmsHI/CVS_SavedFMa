{
  gROOT->ProcessLine(".x /home/frankma/UserCode/SavedFMa/cmssw/root/macros/rootlogon.C");
  TFile * fin = new TFile("/d100/mc/reco/Summer09/MinBias/STARTUP3X_V8P_900GeV_Jan29ReReco-v1/FEE5AED4-540D-DF11-819B-00E08178C129.root");
  Events->SetAlias("vtx","recoVertexs_offlinePrimaryVertices__RECO.obj");
  Events->Draw("vtx.z()");
  Events->Draw("vtx.z()","!vtx.isFake()","same");
}
