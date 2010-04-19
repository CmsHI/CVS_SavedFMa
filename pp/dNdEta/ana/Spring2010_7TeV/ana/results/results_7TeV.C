{
  // MC: Pythia D6t
  TCanvas * cDFFitFracPythiaD6t = new TCanvas("cDFFitFracPythiaD6t","cDFFitFracPythiaD6t",600,600);
  TH1D * hDFFitFracPythiaD6t = new TH1D("hDFFitFracPythiaD6t","PythiaD6t;fitted DF frac;",100,0,100);
  hDFFitFracPythiaD6t->Fill(20.4);
  hDFFitFracPythiaD6t->Fill(17.3);
  hDFFitFracPythiaD6t->Fill(16.0);
  hDFFitFracPythiaD6t->Fill(16.4);
  hDFFitFracPythiaD6t->Fill(16.0);
  hDFFitFracPythiaD6t->Fill(16.4);
  hDFFitFracPythiaD6t->Fill(24.5);
  hDFFitFracPythiaD6t->Fill(16.8);
  hDFFitFracPythiaD6t->Fill(15.5);
  hDFFitFracPythiaD6t->Fill(13.9);
  hDFFitFracPythiaD6t->Fill(12.0);
  hDFFitFracPythiaD6t->Fill(23.5);
  hDFFitFracPythiaD6t->Fill(17.5);
  hDFFitFracPythiaD6t->Fill(16.2);
  hDFFitFracPythiaD6t->Fill(15.0);
  hDFFitFracPythiaD6t->Fill(13.3);
  hDFFitFracPythiaD6t->Fill(17.1);
  hDFFitFracPythiaD6t->Fill(17.8);
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
  hDFFitFracPythiaAtlas->Fill(14.5);
  hDFFitFracPythiaAtlas->Fill(14.8);
  hDFFitFracPythiaAtlas->Fill(27.4);
  hDFFitFracPythiaAtlas->Fill(19.6);
  hDFFitFracPythiaAtlas->Fill(16.0);
  hDFFitFracPythiaAtlas->Fill(11.2);
  hDFFitFracPythiaAtlas->Fill(9.1);
  hDFFitFracPythiaAtlas->Fill(27.0);
  hDFFitFracPythiaAtlas->Fill(20.2);
  hDFFitFracPythiaAtlas->Fill(16.9);
  hDFFitFracPythiaAtlas->Fill(12.1);
  hDFFitFracPythiaAtlas->Fill(10.3);
  hDFFitFracPythiaAtlas->Fill(16.2);
  hDFFitFracPythiaAtlas->Fill(17.5);
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
  hDFFitFracPhojet->Fill(19.6);
  hDFFitFracPhojet->Fill(17.3);
  hDFFitFracPhojet->Fill(30.5);
  hDFFitFracPhojet->Fill(29.6);
  hDFFitFracPhojet->Fill(26.7);
  hDFFitFracPhojet->Fill(20.1);
  hDFFitFracPhojet->Fill(12.4);
  hDFFitFracPhojet->Fill(30.0);
  hDFFitFracPhojet->Fill(30.0);
  hDFFitFracPhojet->Fill(27.5);
  hDFFitFracPhojet->Fill(21.2);
  hDFFitFracPhojet->Fill(13.9);
  hDFFitFracPhojet->Fill(19.1);
  hDFFitFracPhojet->Fill(20.1);
  hDFFitFracPhojet->Draw();
  cout << "Phojet - mean: " << hDFFitFracPhojet->GetMean()
    << "  rms: " << hDFFitFracPhojet->GetRMS() << endl;

  // MC: PhojetPythiaAtlas
  TCanvas * cDFFitFracPhojetPythiaAtlas = new TCanvas("cDFFitFracPhojetPythiaAtlas","cDFFitFracPhojetPythiaAtlas",600,600);
  TH1D * hDFFitFracPhojetPythiaAtlas = new TH1D("hDFFitFracPhojetPythiaAtlas","PhojetPythiaAtlas;fitted DF frac;",100,0,100);
  hDFFitFracPhojetPythiaAtlas->Fill(22.4);
  hDFFitFracPhojetPythiaAtlas->Fill(21.8);
  hDFFitFracPhojetPythiaAtlas->Fill(20.1);
  hDFFitFracPhojetPythiaAtlas->Fill(17.2);
  hDFFitFracPhojetPythiaAtlas->Fill(17.7);
  hDFFitFracPhojetPythiaAtlas->Fill(25.6);
  hDFFitFracPhojetPythiaAtlas->Fill(25.5);
  hDFFitFracPhojetPythiaAtlas->Fill(23.6);
  hDFFitFracPhojetPythiaAtlas->Fill(17.8);
  hDFFitFracPhojetPythiaAtlas->Fill(13.7);
  hDFFitFracPhojetPythiaAtlas->Fill(24.5);
  hDFFitFracPhojetPythiaAtlas->Fill(25.2);
  hDFFitFracPhojetPythiaAtlas->Fill(23.9);
  hDFFitFracPhojetPythiaAtlas->Fill(18.4);
  hDFFitFracPhojetPythiaAtlas->Fill(14.9);
  hDFFitFracPhojetPythiaAtlas->Fill(19.1);
  hDFFitFracPhojetPythiaAtlas->Fill(19.5);
  hDFFitFracPhojetPythiaAtlas->Draw();
  cout << "PhojetPythiaAtlas - mean: " << hDFFitFracPhojetPythiaAtlas->GetMean()
    << "  rms: " << hDFFitFracPhojetPythiaAtlas->GetRMS() << endl;

  // MC: PhojetPythiaD6t
  TCanvas * cDFFitFracPhojetPythiaD6t = new TCanvas("cDFFitFracPhojetPythiaD6t","cDFFitFracPhojetPythiaD6t",600,600);
  TH1D * hDFFitFracPhojetPythiaD6t = new TH1D("hDFFitFracPhojetPythiaD6t","PhojetPythiaD6t;fitted DF frac;",100,0,100);
  hDFFitFracPhojetPythiaD6t->Fill(19.5);
  hDFFitFracPhojetPythiaD6t->Fill(19.9);
  hDFFitFracPhojetPythiaD6t->Fill(20.5);
  hDFFitFracPhojetPythiaD6t->Fill(19.5);
  hDFFitFracPhojetPythiaD6t->Fill(19.8);
  hDFFitFracPhojetPythiaD6t->Fill(22.0);
  hDFFitFracPhojetPythiaD6t->Fill(21.8);
  hDFFitFracPhojetPythiaD6t->Fill(22.5);
  hDFFitFracPhojetPythiaD6t->Fill(20.2);
  hDFFitFracPhojetPythiaD6t->Fill(16.3);
  hDFFitFracPhojetPythiaD6t->Fill(21.6);
  hDFFitFracPhojetPythiaD6t->Fill(22.1);
  hDFFitFracPhojetPythiaD6t->Fill(23.1);
  hDFFitFracPhojetPythiaD6t->Fill(21.6);
  hDFFitFracPhojetPythiaD6t->Fill(17.9);
  hDFFitFracPhojetPythiaD6t->Fill(19.4);
  hDFFitFracPhojetPythiaD6t->Fill(20.1);
  hDFFitFracPhojetPythiaD6t->Draw();
  cout << "PhojetPythiaD6t - mean: " << hDFFitFracPhojetPythiaD6t->GetMean()
    << "  rms: " << hDFFitFracPhojetPythiaD6t->GetRMS() << endl;

  // Combine
  TCanvas * cDFFitFracComb = new TCanvas("cDFFitFracComb","cDFFitFracComb",600,600);
  TH1D * hDFFitFracComb = new TH1D("hDFFitFracComb","Phojet;fitted DF frac;",100,0,100);
  hDFFitFracComb->Fill(20.4);
  hDFFitFracComb->Fill(20.2);
  hDFFitFracComb->Fill(19.1);
  hDFFitFracComb->Fill(17.4);
  hDFFitFracComb->Fill(16.6);
  hDFFitFracComb->Fill(19.6);
  hDFFitFracComb->Draw();
  cout << "Comb: EvtEta and 2Bin - mean: " << hDFFitFracComb->GetMean()
    << "  rms: " << hDFFitFracComb->GetRMS() << endl;

  TCanvas * cDFFitFracCombEvtEta = new TCanvas("cDFFitFracCombEvtEta","cDFFitFracCombEvtEta",600,600);
  TH1D * hDFFitFracCombEvtEta = new TH1D("hDFFitFracCombEvtEta",";fitted DF frac;",100,0,100);
  hDFFitFracCombEvtEta->Fill(20.4);
  hDFFitFracCombEvtEta->Fill(20.2);
  hDFFitFracCombEvtEta->Fill(19.1);
  hDFFitFracCombEvtEta->Draw();
  cout << "Comb: EvtEta only - mean: " << hDFFitFracCombEvtEta->GetMean()
    << "  rms: " << hDFFitFracCombEvtEta->GetRMS() << endl;

  TCanvas * cDFFitFracCombAll = new TCanvas("cDFFitFracCombAll","cDFFitFracCombAll",600,600);
  TH1D * hDFFitFracCombAll = new TH1D("hDFFitFracCombAll","All;fitted DF frac;",100,0,100);
  hDFFitFracCombAll->Add(hDFFitFracPythiaD6t,hDFFitFracPythiaAtlas);
  hDFFitFracCombAll->Add(hDFFitFracPhojet);
  cout << "Comb: All: " << hDFFitFracCombAll->GetMean()
    << "  rms: " << hDFFitFracCombAll->GetRMS() << endl;
  hDFFitFracCombAll->Draw();
  cDFFitFracCombAll->Print("results_7TeV_all.gif");
}
