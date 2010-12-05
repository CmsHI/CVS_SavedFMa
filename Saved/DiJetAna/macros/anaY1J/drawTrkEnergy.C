void drawTrkEnergy() {

  TFile *f = new TFile("hists_Aj_0_24_cent_30_100.root");
  
  TH1D *n1 = (TH1D*) f->Get("hPNDRSigTrk1");
  TH1D *n2 = (TH1D*) f->Get("hPNDRSigTrk2");
  TH1D *n4 = (TH1D*) f->Get("hPNDRSigTrk4");
  TH1D *n8 = (TH1D*) f->Get("hPNDRSigTrk8");

  TH1D *a1 = (TH1D*) f->Get("hPADRSigTrk1");
  TH1D *a2 = (TH1D*) f->Get("hPADRSigTrk2");
  TH1D *a4 = (TH1D*) f->Get("hPADRSigTrk4");
  TH1D *a8 = (TH1D*) f->Get("hPADRSigTrk8");

  TH1::SetDefaultSumw2();

  n1->SetFillColor(kBlue-3);
  TH1D *n12 = (TH1D*) n1->Clone("n12");
  n12->Add(n2); n12->SetFillColor(38);
  TH1D *n124 = (TH1D*) n12->Clone("n124");
  n124->Add(n4); n124->SetFillColor(kOrange-8);
  TH1D *n1248 = (TH1D*) n124->Clone("n1248");
  n1248->Add(n8); n1248->SetFillColor(kRed-6);

  a1->SetFillColor(kBlue-3);
  TH1D *a12 = (TH1D*) a1->Clone("a12");
  a12->Add(a2); a12->SetFillColor(38);
  TH1D *a124 = (TH1D*) a12->Clone("a124");
  a124->Add(a4); a124->SetFillColor(kOrange-8);
  TH1D *a1248 = (TH1D*) a124->Clone("a1248");
  a1248->Add(a8); a1248->SetFillColor(kRed-6);

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
  TH1D* hc1248 = combine(n1248,a1248);
  TH1D* hc124 = combine(n124,a124);
  TH1D* hc12 = combine(n12,a12);
  TH1D* hc1 = combine(n1,a1);

  hc1248->Draw("chist");
  hc124->Draw("chistsame");
  hc12->Draw("chistsame");
  hc1->Draw("chistsame");

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

  gPad->SetLogy();
  gPad->SetRightMargin(0.05);
  gPad->SetLeftMargin(0.18);

  TLegend *leg = new TLegend(0.45,0.73,0.75,0.88);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetNColumns(2);
  leg->AddEntry(hc1,"1-2 GeV/c","f");
  leg->AddEntry(hc12,"2-4 GeV/c","f");
  leg->AddEntry(hc124,"4-8 GeV/c","f");
  leg->AddEntry(hc1248,"8+ GeV/c","f");
  leg->Draw();
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
  hcombine->SetMinimum(1.0);
  hcombine->SetTitle(";;#frac{1}{N_{dijet}}  #frac{d#sump_{T}^{track} }{  2#pi R dR }");
  hcombine->GetYaxis()->CenterTitle();
  hcombine->GetYaxis()->SetTitleOffset(1.7);

  return hcombine;

}
