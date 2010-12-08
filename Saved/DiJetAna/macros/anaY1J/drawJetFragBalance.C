void drawJetFragBalance(
    //TString inFileName="jfh_HCPR_J50U_Cent0to10_Aj0to100_SubNone"
    //TString inFileName="jfh_HCPR_J50U_Cent0to10_Aj0to24_SubEtaRefl"
    //TString inFileName="jfh_HCPR_J50U_Cent0to10_Aj24to100_SubEtaRefl"
    //TString inFileName="jfh_HCPR_J50U_Cent0to10_Aj0to24_SubPhiRot"
    //TString inFileName="jfh_HCPR_J50U_Cent0to10_Aj24to100_SubPhiRot"

    //TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to24_SubEtaRefl"
    //TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj24to100_SubEtaRefl"
    TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl"
    //TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to24_SubPhiRot"
    //TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj24to100_SubPhiRot"
    //TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubPhiRot"

    //TString inFileName="jfh_PyquenUQ80_Cent0to100_Aj0to100_SubNone"
    ) {
  TFile *f = new TFile(inFileName+".root");

  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");

  Int_t numBinsPt=hPtPNDR->GetNbinsX();
  //Int_t numBinsDR=hPtPNDR->GetNbinsY();
  Int_t numBinsDR=5; // for the integration in dr we integrate to ~0.8 for now
  cout << "DR vs Pt: pt bins: " << numBinsPt << " DR bins: " << numBinsDR << endl;

  Int_t endBinX=6;
  Int_t endBinY=9;

  TFile *outf = new TFile("drawn_"+inFileName+".root","RECREATE");

  TCanvas * c2 = new TCanvas("c2","c2",1200,700);
  c2->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    c2->cd(i);
    Int_t iBeg=i,iEnd=i;
    if (i==endBinX) {iBeg=1; iEnd=numBinsPt;}
    if (i==endBinX-1) {iBeg=i; iEnd=numBinsPt;}
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
  c2->Print(Form("%s_DR.gif",inFileName.Data()));
  c2->Print(Form("%s_DR.eps",inFileName.Data()));
  c2->Print(Form("%s_DR.C",inFileName.Data()));

  TCanvas * c3 = new TCanvas("c3","c3",1200,900);
  c3->Divide(3,3);
  for (Int_t i=1; i<=endBinY; ++i) {
    c3->cd(i);
    Int_t iBeg=i,iEnd=i;
    if (i==endBinY) {iBeg=1; iEnd=numBinsDR;}
    if (i==endBinX-1) {iBeg=i; iEnd=numBinsDR;}
    c3->GetPad(i)->SetLogx();
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
  c3->Print(Form("%s_Pt.gif",inFileName.Data()));
  c3->Print(Form("%s_Pt.eps",inFileName.Data()));
  c3->Print(Form("%s_Pt.C",inFileName.Data()));

  TCanvas * c4 = new TCanvas("c4","c4",1200,700);
  c4->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    Int_t iBeg=i,iEnd=i;
    if (i==endBinX) {iBeg=1; iEnd=numBinsPt;}
    if (i==endBinX-1) {iBeg=i; iEnd=numBinsPt;}
    c4->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADRSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBgSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBgSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hAw,-1);
    hNrBg->Add(hAwBg,-1);
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(30);
    hNr->Draw();
    hNrBg->Draw("same hist");
    TLine *l = new TLine(0,0,3.14/2,0);
    l->SetLineStyle(2);
    l->Draw();
  }
  c4->Print(Form("%s_DRNrSubAw.gif",inFileName.Data()));
  c4->Print(Form("%s_DRNrSubAw.eps",inFileName.Data()));
  c4->Print(Form("%s_DRNrSubAw.C",inFileName.Data()));

  TCanvas * c5 = new TCanvas("c5","c5",1200,900);
  c5->Divide(3,3);
  for (Int_t i=1; i<=endBinY; ++i) {
    Int_t iBeg=i,iEnd=i;
    if (i==endBinY) {iBeg=1; iEnd=numBinsDR;}
    if (i==endBinX-1) {iBeg=i; iEnd=numBinsDR;}
    c5->cd(i);
    c5->GetPad(i)->SetLogx();
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionX(Form("hPtNrSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionX(Form("hPtAwSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionX(Form("hPtBgNrSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionX(Form("hPtBgAwSub_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hAw,-1);
    hNrBg->Add(hAwBg,-1);
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hNr->SetMinimum(-25);
    hNr->SetMaximum(25);
    hNr->Draw();
    hNrBg->Draw("same hist");
    TLine *l = new TLine(0,0,100,0);
    l->SetLineStyle(2);
    l->Draw();
  }
  c5->Print(Form("%s_PtNrSubAw.gif",inFileName.Data()));
  c5->Print(Form("%s_PtNrSubAw.eps",inFileName.Data()));
  c5->Print(Form("%s_PtNrSubAw.C",inFileName.Data()));

  TCanvas * c6 = new TCanvas("c6","c6",1200,700);
  c6->Divide(3,2);
  for (Int_t i=1; i<=endBinX; ++i) {
    Int_t iBeg=i,iEnd=i;
    if (i==endBinX) {iBeg=1; iEnd=numBinsPt;}
    if (i==endBinX-1) {iBeg=i; iEnd=numBinsPt;}
    c6->cd(i);
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionY(Form("hPNDRSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionY(Form("hPADRSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionY(Form("hPNDRBgSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionY(Form("hPADRBgSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hNrBg,-1);
    hAw->Add(hAwBg,-1);
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(40);
    hNr->Draw();
    hAw->Draw("same");
    TLine *l = new TLine(0,0,3.14/2,0);
    l->SetLineStyle(2);
    l->Draw();
  }
  c6->Print(Form("%s_DRSubBg.gif",inFileName.Data()));
  c6->Print(Form("%s_DRSubBg.eps",inFileName.Data()));
  c6->Print(Form("%s_DRSubBg.C",inFileName.Data()));

  TCanvas * c7 = new TCanvas("c7","c7",1200,900);
  c7->Divide(3,3);
  for (Int_t i=1; i<=endBinY; ++i) {
    Int_t iBeg=i,iEnd=i;
    if (i==endBinY) {iBeg=1; iEnd=numBinsDR;}
    if (i==endBinX-1) {iBeg=i; iEnd=numBinsDR;}
    c7->cd(i);
    c7->GetPad(i)->SetLogx();
    TH1D * hNr = (TH1D*)hPtPNDR->ProjectionX(Form("hPtNrSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAw = (TH1D*)hPtPADR->ProjectionX(Form("hPtAwSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrBg = (TH1D*)hPtPNDRBg->ProjectionX(Form("hPtBgNrSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hAwBg = (TH1D*)hPtPADRBg->ProjectionX(Form("hPtBgAwSubBg_%d_%d",iBeg,iEnd),iBeg,iEnd);
    hNr->Add(hNrBg,-1);
    hAw->Add(hAwBg,-1);
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hNr->SetMinimum(-15);
    hNr->SetMaximum(40);
    hNr->Draw();
    hAw->Draw("same");
    TLine *l = new TLine(0,0,100,0);
    l->SetLineStyle(2);
    l->Draw();
  }
  c7->Print(Form("%s_PtSubBg.gif",inFileName.Data()));
  c7->Print(Form("%s_PtSubBg.eps",inFileName.Data()));
  c7->Print(Form("%s_PtSubBg.C",inFileName.Data()));

  // All Done
  outf->Write();
}
