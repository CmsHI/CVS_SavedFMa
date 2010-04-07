{
  TH1D * hDFFitFrac = new TH1D("hDFFitFrac",";fitted DF frac;",100,0,100);
  hDFFitFrac->Fill(21.6);
  hDFFitFrac->Fill(23.8);
  hDFFitFrac->Fill(22.2);
  hDFFitFrac->Fill(24.2);
  hDFFitFrac->Fill(22.7);

  hDFFitFrac->Draw();
  cout << "mean: " << hDFFitFrac->GetMean()
    << "rms: " << hDFFitFrac->GetRMS() << endl;
}
