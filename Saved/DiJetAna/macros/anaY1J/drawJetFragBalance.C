void drawJetFragBalance() {

  TFile *f = new TFile("jetFragHists_EtaRefl.root");
  //TFile *f = new TFile("jetFragHists_PhiRot.root");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");

  Int_t endBinX=6;
  Int_t endBinY=9;

  TCanvas * c2 = new TCanvas("c2","c2",1200,700);
  c2->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    c2->cd(i);
    Int_t iBeg=i,iEnd=i;
    if (i==endBinX) {iBeg=1; iEnd=100;}
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDR_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADR_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
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
  for (Int_t i=1; i<=endBinY; ++i) {
    c3->cd(i);
    Int_t iBeg=i,iEnd=i;
    if (i==endBinY) {iBeg=1; iEnd=100;}
    c3->GetPad(i)->SetLogy();
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionX(Form("hPtNr_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionX(Form("hPtAw_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionX(Form("hPtBgNr_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionX(Form("hPtBgAw_%d_%d",iBeg,iEnd),iBeg,iEnd);
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
  for (Int_t i=1; i<=endBinX; ++i) {
    Int_t iBeg=i,iEnd=i;
    if (i==endBinX) {iBeg=1; iEnd=100;}
    c4->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBgSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBgSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hAw,-1);
    hNrBg->Add(hAwBg,-1);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(15);
    hNr->Draw();
    hNrBg->Draw("same hist");
  }

  TCanvas * c5 = new TCanvas("c5","c5",1200,900);
  c5->Divide(3,3);
  for (Int_t i=1; i<=endBinY; ++i) {
    Int_t iBeg=i,iEnd=i;
    if (i==endBinY) {iBeg=1; iEnd=100;}
    c5->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionX(Form("hPtNrSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionX(Form("hPtAwSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionX(Form("hPtBgNrSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionX(Form("hPtBgAwSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hAw,-1);
    hNrBg->Add(hAwBg,-1);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(15);
    hNr->Draw();
    hNrBg->Draw("same hist");
  }
}
