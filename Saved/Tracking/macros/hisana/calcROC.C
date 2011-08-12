{
  TFile * inf = new TFile("../trees/hypyuq170-393-trkeff_itertrk-v5/merge/all.root");
  TTree * t = (TTree*)inf->Get("anaTrack/trackTree");
  t->AddFriend("icPu5JetAnalyzer/t");
  t->Show(0);
  
  TCut evtSel = "bin<4";
  TCut trkSel = "abs(trkEta)<2.4";
  
  TCut trkSelHgt = "(trkPtError/trkPt<=0.06&&trkNHit>=13&&trkChi2/trkNdof/trkNlayer<=0.15&&abs(trkDz1/trkDzError1)<3&&abs(trkDxy1/trkDxyError1)<3)";
    
  TProfile * hEff0 = new TProfile("hEff0",";trk p_{T} (GeV/c);Eff",10,0,100);
  Float_t signum = t->Draw("trkPt",evtSel&&trkSel&&trkSelHgt&&"!trkFake","goff");
  Float_t sigden = t->Draw("trkPt",evtSel&&trkSel&&"!trkFake","goff");

  Float_t bkgnum = t->Draw("trkPt",evtSel&&trkSel&&trkSelHgt&&"trkFake","goff");
  Float_t bkgden = t->Draw("trkPt",evtSel&&trkSel&&"trkFake","goff");

  cout << "sig eff" << signum/sigden << endl;
  cout << "bkg rej" << 1-bkgnum/bkgden << endl;
}