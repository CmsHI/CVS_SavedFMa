void count(const char * inf0name = "jet_ana_v3_et_order_2.8TeV.root")
{
  Double_t NEt = 100.;
  Double_t AEt = 90.;
  Double_t DPhi = 2.2;

  TFile * inf0 = new TFile(inf0name);
  TTree * tree = (TTree*)inf0->FindObjectAny("jets");
  Int_t totn = tree->GetEntries();
  cout << "total number events: " << totn << endl;
  TString djCut(Form("njet>%.1f && ajet>%.1f && dphi>%.2f",NEt,AEt,DPhi));

  cout << "Now apply djCut: " << djCut << endl;
  Float_t djn = tree->GetEntries(djCut);
  cout << "passed dijets in sample: " << djn << endl;

  Float_t djnEst = djn*(24000000./totn);
  cout << "scale to 24M: " << djnEst << endl;
}
