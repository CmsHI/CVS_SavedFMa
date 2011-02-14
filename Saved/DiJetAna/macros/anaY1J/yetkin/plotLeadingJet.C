

{

  TH1::SetDefaultSumw2();

  double pt1min = 100;

  int color[25] = {1,2,4,5,6,7,8,9,3,12,13,23,24,32};

  //  TFile* inf1 = new TFile("tuneA_MB/results.root");
  //  TNtuple* nt = (TNtuple*)inf1->Get("nt");

  TChain * nt = new TChain("nt");
  nt->Add("smeared*.root");

  TH1D* h[5];
  for(int j = 0; j < 5; ++j){
    h[j] = new TH1D(Form("h%d",j),"",150,0,300);

    h[j]->SetLineColor(color[j]);
    h[j]->SetMarkerColor(color[j]);
  }

  TCanvas* c1 = new TCanvas("c1","",500,500);
  c1->SetLogy();
  nt->Draw(Form("pt1>>h0"),Form("pt1 > %f",pt1min),"");
  nt->Draw(Form("ptt1>>h1"),Form("ptt1 > %f",pt1min),"same");
  nt->Draw(Form("ptt1>>h2"),Form("pt1 > %f",pt1min),"same");










}

