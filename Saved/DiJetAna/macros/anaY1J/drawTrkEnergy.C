void drawTrkEnergy() {

  TFile *f = new TFile("drawn_jetFragHists_Cent0to10_Aj24to100_SubPhiRot.root");

  TH1D *n0 = (TH1D*) f->Get("hPNDRSubBg_1_1");
  TH1D *n1 = (TH1D*) f->Get("hPNDRSubBg_2_2");
  TH1D *n2 = (TH1D*) f->Get("hPNDRSubBg_3_3");
  TH1D *n4 = (TH1D*) f->Get("hPNDRSubBg_4_4");
  TH1D *n8 = (TH1D*) f->Get("hPNDRSubBg_5_5");
  TH1D *nall = (TH1D*) f->Get("hPNDRSubBg_1_100");

  TH1D *a0 = (TH1D*) f->Get("hPADRSubBg_1_1");
  TH1D *a1 = (TH1D*) f->Get("hPADRSubBg_2_2");
  TH1D *a2 = (TH1D*) f->Get("hPADRSubBg_3_3");
  TH1D *a4 = (TH1D*) f->Get("hPADRSubBg_4_4");
  TH1D *a8 = (TH1D*) f->Get("hPADRSubBg_5_5");
  TH1D *aall = (TH1D*) f->Get("hPADRSubBg_1_100");

  TH1::SetDefaultSumw2();

  n0->SetFillColor(kGray);
  TH1D *n01 = (TH1D*) n0->Clone("n01");
  n01->Add(n1); n01->SetFillColor(kBlue-3);
  TH1D *n12 = (TH1D*) n01->Clone("n12");
  n12->Add(n2); n12->SetFillColor(38);
  TH1D *n124 = (TH1D*) n12->Clone("n124");
  n124->Add(n4); n124->SetFillColor(kOrange-8);
  TH1D *n1248 = (TH1D*) n124->Clone("n1248");
  n1248->Add(n8); n1248->SetFillColor(kRed-6);
  nall->SetFillColor(kBlack);

  a0->SetFillColor(kGray);
  TH1D *a01 = (TH1D*) a0->Clone("a01");
  a01->Add(a1); a01->SetFillColor(kBlue-3);
  TH1D *a12 = (TH1D*) a01->Clone("a12");
  a12->Add(a2); a12->SetFillColor(38);
  TH1D *a124 = (TH1D*) a12->Clone("a124");
  a124->Add(a4); a124->SetFillColor(kOrange-8);
  TH1D *a1248 = (TH1D*) a124->Clone("a1248");
  a1248->Add(a8); a1248->SetFillColor(kRed-6);
  aall->SetFillColor(kBlack);

  /*
  TCanvas *c = new TCanvas("c","c",800,500);
  c->Divide(2,1,-1,-1);
  c->cd(1);
  n1248->SetMinimum(1); n1248->SetMaximum(3000);
  n1248->Draw("chist");
  n124->Draw("chistsame");
  n12->Draw("chistsame");
  n1->Draw("chistsame");
  gPad->SetLogy();
  c->cd(2);
  a1248->SetMinimum(1); a1248->SetMaximum(3000);
  a1248->Draw("chist");
  a124->Draw("chistsame");
  a12->Draw("chistsame");
  a1->Draw("chistsame");
  gPad->SetLogy();
  */

  TCanvas *c2 = new TCanvas("c2","c2",600,500);
  TH1D* hcall = combine(nall,aall);
  TH1D* hc1248 = combine(n1248,a1248);
  TH1D* hc124 = combine(n124,a124);
  TH1D* hc12 = combine(n12,a12);
  TH1D* hc01 = combine(n01,a01);
  TH1D* hc0 = combine(n0,a0);

  hcall->Draw("chist"); hcall->Draw("esame");
  hc1248->Draw("chistsame"); hc1248->Draw("esame"); //chist
  hc124->Draw("chistsame"); hc124->Draw("esame");
  hc12->Draw("chistsame"); hc12->Draw("esame");
  hc01->Draw("chistsame"); hc01->Draw("esame");
  hc0->Draw("chistsame"); hc0->Draw("esame");

  hc1248->GetXaxis()->SetAxisColor(0);
  hc1248->GetXaxis()->SetLabelColor(0);

  TGaxis *naxis = new TGaxis(-1.4,1.0,1.4,1.0,-1.4,1.4,510,"-");
  TGaxis *aaxis = new TGaxis(TMath::Pi()-1.4,1.0,TMath::Pi()+1.4,1.0,-1.4,1.4,510,"-");
  naxis->SetLabelOffset(-0.05); naxis->SetLabelSize(0.03);
  naxis->SetTitle("#DeltaR^{track}_{leading jet}"); naxis->CenterTitle(); naxis->SetTitleOffset(-1.5);
  aaxis->SetLabelOffset(-0.05); aaxis->SetLabelSize(0.03);
  aaxis->SetTitle("#DeltaR^{track}_{sub-leading jet}"); aaxis->CenterTitle(); aaxis->SetTitleOffset(-1.5);
  naxis->Draw();
  aaxis->Draw();
  TGaxis *nuaxis = new TGaxis(-1.0*TMath::PiOver2(),3000.0,1.4,3000.0,-1.0*TMath::PiOver2(),1.4,510,"U-");
  TGaxis *auaxis = new TGaxis(TMath::Pi()-1.4,3000.0,3.0*TMath::PiOver2(),3000.0,-1.4,TMath::PiOver2(),510,"U-");
  nuaxis->Draw();
  auaxis->Draw();

  //gPad->SetLogy();
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.18);

  TLegend *leg = new TLegend(0.45,0.73,0.75,0.88);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetNColumns(2);
  leg->AddEntry(hc0,"0.5-1 GeV/c","f");
  leg->AddEntry(hc01,"1-2 GeV/c","f");
  leg->AddEntry(hc12,"2-4 GeV/c","f");
  leg->AddEntry(hc124,"4-8 GeV/c","f");
  leg->AddEntry(hc1248,"8-16 GeV/c","f");
  leg->AddEntry(hcall,"16+ GeV/c","f");
  leg->Draw();


  double nearsum=0.0, awaysum=0.0;
  for(ibin=1; ibin<=8; ibin++) {
    /*
    double bc = hcall->GetBinContent(ibin);
    cout << bc << endl;
    if(ibin>12 && ibin<=18) nearsum+=bc;
    if(ibin>32 && ibin<=38) awaysum+=bc;
    */
    double bcNr = nall->GetBinContent(ibin);
    double bcAw = aall->GetBinContent(ibin);
    nearsum+=bcNr;
    awaysum+=bcAw;
  }


  cout << "integral of dET/dR = " << nearsum << "(near-side) \t"
       << awaysum << "(away-side)" << endl;

}

TH1D* combine(TH1D* near, TH1D* away) {

  TH1D* hcombine = new TH1D("hcombine","",40,-1.0*TMath::PiOver2(),3.0*TMath::PiOver2());
  for(int bin=1; bin<=10; bin++) {
    hcombine->SetBinContent(bin,near->GetBinContent(11-bin));
    hcombine->SetBinError(bin,near->GetBinError(11-bin));
  }
  for(int bin=11; bin<=20; bin++) {
    hcombine->SetBinContent(bin,near->GetBinContent(bin-10));
    hcombine->SetBinError(bin,near->GetBinError(bin-10));
  }
  for(int bin=21; bin<=30; bin++) {
    hcombine->SetBinContent(bin,away->GetBinContent(31-bin));
    hcombine->SetBinError(bin,away->GetBinError(31-bin));
  }
  for(int bin=31; bin<=40; bin++) {
    hcombine->SetBinContent(bin,away->GetBinContent(bin-30));
    hcombine->SetBinError(bin,away->GetBinError(bin-30));
  }

  hcombine->SetFillColor(near->GetFillColor());
  hcombine->SetStats(0);
  hcombine->SetMinimum(0.1);
  hcombine->SetTitle(";;#frac{1}{N_{dijet}}  #frac{d#sump_{T}^{track} }{ dR }"); // no 2piR in denominator
  //hcombine->SetTitle(";;#frac{1}{N_{dijet}}  #frac{d#sump_{T}^{track} }{  2#pi R dR }");
  hcombine->GetYaxis()->CenterTitle();
  hcombine->GetYaxis()->SetTitleOffset(1.7);

  return hcombine;

}
