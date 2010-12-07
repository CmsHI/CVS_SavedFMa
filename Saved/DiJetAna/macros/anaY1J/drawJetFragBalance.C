void drawJetFragBalance() {

  TFile *f = new TFile("jetFragHists_EtaRefl.root");
  //TFile *f = new TFile("jetFragHists_PhiRot.root");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");

  TCanvas * c2 = new TCanvas("c2","c2",1200,700);
  c2->Divide(3,2);
  for (Int_t i=1; i<=6; ++i) {
    c2->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDR_%d",i),i,i);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADR_%d",i),i,i);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBg_%d",i),i,i);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBg_%d",i),i,i);
    hNr->SetLineColor(kRed);
    hNr->SetMarkerColor(kRed);
    hAw->SetLineColor(kBlue);
    hAw->SetMarkerColor(kBlue);
    hNrBg->SetLineColor(kRed);
    hNrBg->SetMarkerColor(kRed);
    hAwBg->SetLineColor(kBlue);
    hAwBg->SetMarkerColor(kBlue);
    hNr->SetMinimum(0.1);
    hNr->Draw();
    hAw->Draw("same");
    hNrBg->Draw("same hist");
    hAwBg->Draw("same hist");
  }

  TCanvas * c3 = new TCanvas("c3","c3",1200,900);
  c3->Divide(3,3);
  for (Int_t i=1; i<=8; ++i) {
    c3->cd(i);
    //c3->GetPad(i)->SetLogx();
    c3->GetPad(i)->SetLogy();
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionX(Form("hPtNr_%d",i),i,i);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionX(Form("hPtAw_%d",i),i,i);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionX(Form("hPtBgNr_%d",i),i,i);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionX(Form("hPtBgAw_%d",i),i,i);
    hNr->SetLineColor(kRed);
    hNr->SetMarkerColor(kRed);
    hAw->SetLineColor(kBlue);
    hAw->SetMarkerColor(kBlue);
    hNrBg->SetLineColor(kRed);
    hNrBg->SetMarkerColor(kRed);
    hAwBg->SetLineColor(kBlue);
    hAwBg->SetMarkerColor(kBlue);
    hNr->SetMinimum(0.1);
    hNr->Draw();
    hAw->Draw("same");
    hNrBg->Draw("same hist");
    hAwBg->Draw("same hist");
  }

  TCanvas * c4 = new TCanvas("c4","c4",1200,700);
  c4->Divide(3,2);
  for (Int_t i=1; i<=6; ++i) {
    c4->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDRSub_%d",i),i,i);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADRSub_%d",i),i,i);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBgSub_%d",i),i,i);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBgSub_%d",i),i,i);
    hNr->Add(hAw,-1);
    hNrBg->Add(hAwBg,-1);
    hNr->SetMinimum(-10);
    hNr->SetMaximum(10);
    hNr->Draw();
    hNrBg->Draw("same hist");
  }

  TCanvas * c5 = new TCanvas("c5","c5",1200,900);
  c5->Divide(3,3);
  for (Int_t i=1; i<=8; ++i) {
    c5->cd(i);
    //c5->GetPad(i)->SetLogx();
    //c5->GetPad(i)->SetLogy();
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionX(Form("hPtNrSub_%d",i),i,i);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionX(Form("hPtAwSub_%d",i),i,i);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionX(Form("hPtBgNrSub_%d",i),i,i);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionX(Form("hPtBgAwSub_%d",i),i,i);
    hNr->Add(hAw,-1);
    hNrBg->Add(hAwBg,-1);
    hNr->SetMinimum(-10);
    hNr->SetMaximum(10);
    hNr->Draw();
    hNrBg->Draw("same hist");
  }
}
