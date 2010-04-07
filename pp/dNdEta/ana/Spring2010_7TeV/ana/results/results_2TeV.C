{
  TH1D * hDFFitFrac = new TH1D("hDFFitFrac",";fitted DF frac;",100,0,100);
  hDFFitFrac->Fill(18.2);
  hDFFitFrac->Fill(22.2);
  hDFFitFrac->Fill(19);
  hDFFitFrac->Fill(22.4);
  hDFFitFrac->Fill(19.9);

  hDFFitFrac->Draw();
  cout << "mean: " << hDFFitFrac->GetMean()
    << "rms: " << hDFFitFrac->GetRMS() << endl;
}
