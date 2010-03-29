void aliases(TTree * Events)
{
  Events->SetAlias("jet","patJets_selectedPatJets__PAT.obj");
  Events->SetAlias("gjet","jet.genJet()");
}

void drawJes(TTree * Events)
{
  aliases(Events);
  TProfile * hJes = new TProfile("hJes","jes",20,0,150);
  TCanvas * cJes = new TCanvas("cJes","cJes",500,500);
  Events->Draw("jet.p4().pt()/gjet.p4().pt():gjet.p4().pt()>>hJes");
}
