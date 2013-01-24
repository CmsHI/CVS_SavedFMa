{
  float nJet=hjetPt_himc_icent4_irj999_fragMode2_closure100_jtrewt0_.Integral();
  float nRaw=hpt_jet_rawTrk_himc_icent4_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300->Integral();
  float nBkg=hpt_jet_mbTrk_himc_icent4_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300->Integral();
  float nSig=hpt_jet_sigTrk_himc_icent4_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300->Integral();
  cout << "pbpb: " << endl;
  cout << "nJet: " << nJet << endl;
  cout << "nRaw: " << nRaw << " nBkg: " << nBkg << " nSig: " << nSig << endl;

  nJet=hjetPt_ppmc_icent4_irj999_fragMode2_closure100_jtrewt0_.Integral();
  nRaw=hpt_jet_rawTrk_ppmc_icent4_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300->Integral();
  nBkg=hpt_jet_mbTrk_ppmc_icent4_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300->Integral();
  nSig=hpt_jet_sigTrk_ppmc_icent4_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300->Integral();
  cout << "pp: " << endl;
  cout << "nJet: " << nJet << endl;
  cout << "nRaw: " << nRaw << " nBkg: " << nBkg << " nSig: " << nSig << endl;
}