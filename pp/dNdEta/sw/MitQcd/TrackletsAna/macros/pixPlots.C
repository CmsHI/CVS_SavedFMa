//
{

  TTree *data;
  TTree *mc;

  TFile *fd = new TFile("/home/yjlee/ana/trackletStudy/trackletAna/newUltimateSample/PixelTree-124022-hfcuts.root");
  data = (TTree *) fd->Get("PixelTree");
  TH1F *hdata = new TH1F("hdata","hdata",100,0,80000);

  data->Draw("ch1/cosh(eta1)>>hdata","abs(vz[1])<2&&cs1<20&&ch1/cosh(eta1)<80000&&!(cs1==1&&ch1/cs1<10000)&&abs(eta1)<2","");
  hdata->Sumw2();
  hdata->Scale(1.0/hdata->GetEntries());
  
  TFile *fmc = new TFile("/home/yjlee/ana/trackletStudy/trackletAna/sample-900GeV/Pythia_MinBias_D6T_900GeV_d20091229.root");
  mc = (TTree *) fmc->Get("PixelTree");

  TH1F *hmc = new TH1F("hmc","hmc",100,0,80000);
  mc->Draw("ch1/cosh(eta1)>>hmc","abs(vz[1])<2&&cs1<20&&ch1/cosh(eta1)<80000&&abs(eta1)<2","");
  hmc->Sumw2();
  hmc->Scale(1.0/hmc->GetEntries());

  hmc->Draw();
  hdata->SetLineColor(2);
  hmc->Draw("hist");
  hdata->Draw("same");
  c1->Update();
  getchar();
  TH1F *hmc2 = new TH1F("hmc2","hmc2",100,0,80000);
  TF1 f1("gaus","gaus",0,2);
  f1->SetParameter(0,1);
  f1->SetParameter(1,1);
  f1->SetParameter(2,0.10);

  for(Int_t i = 0;i<1000000;i++)
    {
      hmc2->Fill(hmc->GetRandom()*f1->GetRandom());
    }
  hmc2->Scale(1.0/hmc2->GetEntries());

  hdata->SetLineColor(2);
  hmc2->Draw("hist");
  hdata->Draw("same");
}
