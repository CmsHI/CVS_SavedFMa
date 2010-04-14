{
  // MC: Pythia D6t
  TCanvas * cDFFitFracPythiaD6t = new TCanvas("cDFFitFracPythiaD6t","cDFFitFracPythiaD6t",600,600);
  TH1D * hDFFitFracPythiaD6t = new TH1D("hDFFitFracPythiaD6t","PythiaD6t;fitted DF frac;",100,0,100);
  hDFFitFracPythiaD6t->Fill(20.4);
  hDFFitFracPythiaD6t->Fill(17.3);
  hDFFitFracPythiaD6t->Fill(16.0);
  hDFFitFracPythiaD6t->Fill(16.4);
  hDFFitFracPythiaD6t->Fill(23.5);
  hDFFitFracPythiaD6t->Fill(16.8);
  hDFFitFracPythiaD6t->Fill(15.5);
  hDFFitFracPythiaD6t->Fill(23.5);
  hDFFitFracPythiaD6t->Fill(17.5);
  hDFFitFracPythiaD6t->Fill(16.2);
  hDFFitFracPythiaD6t->Draw();
  cout << "PythiaD6t - mean: " << hDFFitFracPythiaD6t->GetMean()
    << "  rms: " << hDFFitFracPythiaD6t->GetRMS() << endl;

  // MC: Pythia Atlas
  TCanvas * cDFFitFracPythiaAtlas = new TCanvas("cDFFitFracPythiaAtlas","cDFFitFracPythiaAtlas",600,600);
  TH1D * hDFFitFracPythiaAtlas = new TH1D("hDFFitFracPythiaAtlas","PythiaAtlas;fitted DF frac;",100,0,100);
  hDFFitFracPythiaAtlas->Fill(20.2);
  hDFFitFracPythiaAtlas->Fill(19.8);
  hDFFitFracPythiaAtlas->Fill(17.6);
  hDFFitFracPythiaAtlas->Fill(16.3);
  hDFFitFracPythiaAtlas->Fill(27.4);
  hDFFitFracPythiaAtlas->Fill(19.6);
  hDFFitFracPythiaAtlas->Fill(16.0);
  hDFFitFracPythiaAtlas->Fill(27.0);
  hDFFitFracPythiaAtlas->Fill(20.2);
  hDFFitFracPythiaAtlas->Fill(16.9);
  hDFFitFracPythiaAtlas->Draw();
  cout << "PythiaAtlas - mean: " << hDFFitFracPythiaAtlas->GetMean()
    << "  rms: " << hDFFitFracPythiaAtlas->GetRMS() << endl;

  // MC: Phojet
  TCanvas * cDFFitFracPhojet = new TCanvas("cDFFitFracPhojet","cDFFitFracPhojet",600,600);
  TH1D * hDFFitFracPhojet = new TH1D("hDFFitFracPhojet","Phojet;fitted DF frac;",100,0,100);
  hDFFitFracPhojet->Fill(19.1);
  hDFFitFracPhojet->Fill(28.6);
  hDFFitFracPhojet->Fill(26.8);
  hDFFitFracPhojet->Fill(24.1);
  hDFFitFracPhojet->Fill(30.5);
  hDFFitFracPhojet->Fill(29.6);
  hDFFitFracPhojet->Fill(26.7);
  hDFFitFracPhojet->Fill(30.0);
  hDFFitFracPhojet->Fill(30.0);
  hDFFitFracPhojet->Fill(27.5);
  hDFFitFracPhojet->Draw();
  cout << "Phojet - mean: " << hDFFitFracPhojet->GetMean()
    << "  rms: " << hDFFitFracPhojet->GetRMS() << endl;
}
