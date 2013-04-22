{
  TFile *_file0 = TFile::Open("../ntout/jskim_pp-full_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0_addedReweight.root");
  TFile *_file1 = TFile::Open("../ntout/jskim_pp-full_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0_addedFitRewt.root");
  t0=(TTree*)_file0->Get("tdj");
  t1=(TTree*)_file1->Get("tdj");
  // t0.Draw("rewtPt:jetPt","jetPt>100","colz");
  t1.Draw("1+2*(rewtPt-1):jetPt","jetPt>100","colz");
  t1.Draw("rewtPt:jetPt","jetPt>100","box same");
}