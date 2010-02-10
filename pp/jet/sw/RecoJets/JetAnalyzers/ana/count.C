void count(const char * inf0name = "jet_ana_v3_et_order_2.8TeV.root")
{
  TFile * inf0 = new TFile(inf0name);
  TTree * tree = (TTree*)inf0->FindObjectAny("jets");
  Int_t totn = tree->GetEntries();
  cout << "total number events: " << totn << endl;
  Float_t djn = tree->GetEntries("njet>100 && ajet>70 && dphi>2.2");
  cout << "dijets, near>100GeV, away>70GeV, dphi>2.2: " << djn << endl;

  Float_t djnEst = djn*(24000000./totn);
  cout << "===Prediction for 2.4M at 2.8 TeV===" << endl;
  cout << "dijets, near>100GeV, away>70GeV, dphi>2.2: " << djnEst << endl;
}
