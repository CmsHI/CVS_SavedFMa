{
  TH1D * hDFFitFrac = new TH1D("hDFFitFrac",";fitted DF frac;",100,0,100);
  hDFFitFrac->Fill(24.5);
  hDFFitFrac->Fill(16.8);
  hDFFitFrac->Fill(15.5);
  hDFFitFrac->Fill(23.5);
  hDFFitFrac->Fill(17.5);
  hDFFitFrac->Fill(16.2);
  hDFFitFrac->Fill(23.5);
  hDFFitFrac->Fill(17.5);
  hDFFitFrac->Fill(20.2);

  hDFFitFrac->Draw();
  cout << "mean: " << hDFFitFrac->GetMean()
    << "rms: " << hDFFitFrac->GetRMS() << endl;
}
