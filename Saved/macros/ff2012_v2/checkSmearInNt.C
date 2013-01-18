{
  TH1::SetDefaultSumw2();
  TCanvas * c2 = new TCanvas("c2","c2",800,800);
  TH2D * hpt2d = new TH2D("hpt2d",";raw p_{T} (GeV/c); smeared p_{T} (GeV/c);",30,0,300,30,0,300);
  TH2D * h2 = new TH2D("h2",";raw p_{T} (GeV/c);smeared p_{T} / raw p_{T};",30,0,300,20,0,2);
  c2->Divide(2,2);
  c2->cd(1);
  tdj.Draw("jetPt:jetUnSmPt>>hpt2d","","colz");
  c2->cd(2);
  tdj->Draw("jetPt/jetUnSmPt:jetUnSmPt>>h2","","colz");
  // slices
  h2->FitSlicesY();
  c2->cd(3);
  h2_1->Draw();
  c2->cd(4);
  h2_2->SetAxisRange(0,0.12,"Y");
  h2_2->Draw();

  TCanvas * c3 = new TCanvas("c3","c3",1200,400);
  TH1D * hBefore = new TH1D("hBefore",";p_{T} (GeV/c);",40,100,300);
  TH1D * hAfter = new TH1D("hAfter",";p_{T} (GeV/c);",40,100,300);
  c3->Divide(3,1);
  c3->cd(1);
  tdj->Draw("jetUnSmPt>>hBefore","","");
  tdj->Draw("jetPt>>hAfter","","");
  gPad->SetLogy();
  hBefore->SetMarkerStyle(kOpenCircle);
  hBefore->DrawClone("E");
  hAfter->DrawClone("sameE");
  c3->cd(2);
  gPad->SetLogy();
  hBefore->Scale(1./hBefore->Integral());
  hAfter->Scale(1./hAfter->Integral());
  hBefore->DrawClone("E");
  hAfter->DrawClone("sameE");
  c3->cd(3);
  hAfter->Divide(hBefore);
  hAfter->SetAxisRange(0.5,1.5,"Y");
  hAfter->Draw();
}
