void compLeadJet () {
  TCut evtSel("HLT_HIMinBiasBSC");
  evtSel = evtSel && "hiBin*2.5<100";

  TChain * tcur = new TChain("hltanalysis/HltTree");
  TChain * told = new TChain("hltanalysisOldAlgo/HltTree");
  tcur->Add("hltana_AllStream_r150590.root");
  told->Add("hltana_AllStream_r150590.root");
  tcur->AddFriend("told = hltanalysisOldAlgo/HltTree","hltana_AllStream_r150590.root");
  cout << "Total: " << tcur->GetEntries() << endl;
  cout << "HLT_HIMinBiasBSC: " << tcur->GetEntries("HLT_HIMinBiasBSC") << endl;
  cout << "Evt Sel: " << TString(evtSel) << ": " << tcur->GetEntries(evtSel) << endl;

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetLogy();
  tcur->Draw("recoJetCalPt[0]>>hJetPt",evtSel,"E");
  tcur->Draw("told.recoJetCalPt[0]>>hJetPtOld",evtSel,"Esame");
  hJetPtOld->SetMarkerStyle(kOpenSquare);
  hJetPtOld->SetMarkerStyle(kOpenSquare);
  hJetPt->SetMarkerColor(2);
  hJetPt->SetLineColor(2);
  hJetPt->SetXTitle("CaloJet Et [GeV]");
  hJetPt->SetYTitle("#");
  hJetPt->Draw("E");
  hJetPtOld->Draw("Esame");

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  c3->SetLogz();
  c3->SetRightMargin(0.1684054);
  TH2D * hPtCorr = new TH2D("hPtCorr","hPtCorr",50,0,120,50,0,2);
  tcur->Draw("recoJetCalPt[0]/told.recoJetCalPt[0]:recoJetCalPt[0]>>hPtCorr",evtSel&&"abs(recoJetCalEta[0]-told.recoJetCalEta[0])<0.2&&abs(recoJetCalEta[0])<2","colz");
  hPtCorr->SetXTitle("Old CaloJet Et [GeV]");
  hPtCorr->SetYTitle("MultAloIterEt/OldCaloJetEt");
  TLine *l = new TLine(0,1,120,1);
  l->SetLineStyle(2);
  hPtCorr->Draw("colz");
  l->Draw();
}
