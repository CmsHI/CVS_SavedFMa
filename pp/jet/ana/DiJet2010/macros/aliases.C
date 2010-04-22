void aliases(TTree * Events)
{
  // Event
  Events->SetAlias("vtx","recoVertexs_offlinePrimaryVertices__RECO.obj");

  // jets
  Events->SetAlias("jet","patJets_selectedPatJets__PAT.obj");

  // mc
  Events->SetAlias("gjet","jet.genJet()");
  Events->SetAlias("pjet","jet.genParton()");
}

void drawJes(TTree * Events)
{
  aliases(Events);
  TProfile * hJes = new TProfile("hJes","jes",20,0,150);
  TCanvas * cJes = new TCanvas("cJes","cJes",500,500);
  Events->Draw("jet.p4().pt()/gjet.p4().pt():gjet.p4().pt()>>hJes");
}
