void balance(TString infile="dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root")
{
  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile);
  TChain * djcalopix = new TChain("djcalo_pxl/djTree");
  djcalopix->Add(infile);
  TString evtSel("(cent<10 && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2.5)");

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  TH1D * hJDPhi2 = new TH1D("hJDPhi2","hJDPhi",50,0,3.1416);
  Float_t numDJ = djcalo->Draw("jdphi>>hJDPhi",evtSel);
  Float_t numDJ2 = djcalopix->Draw("jdphi>>hJDPhi2",evtSel);
  cout << "num djs: " << numDJ << "  djs(pix)" << numDJ2 << endl;

  TH1D * hPNDPhiTrk = new TH1D("hPNDPhiTrk",";#Delta#phi(j1,trk);1/N_{DJ} dp_{T}^{Trk}/d(#Delta #phi)",40,0,TMath::Pi());
  TH1D * hPNDPhiPix = new TH1D("hPNDPhiPix",";#Delta#phi(j1,trk);1/N_{DJ} dp_{T}^{Trk}/d(#Delta #phi)",40,0,TMath::Pi());
  TH1D * hPADPhiTrk = new TH1D("hPADPhiTrk",";#Delta#phi(j1,trk);1/N_{DJ} dp_{T}^{Trk}/d(#Delta #phi)",40,0,TMath::Pi());
  TH1D * hPADPhiPix = new TH1D("hPADPhiPix",";#Delta#phi(j1,trk);1/N_{DJ} dp_{T}^{Trk}/d(#Delta #phi)",40,0,TMath::Pi());
  djcalo->Draw("abs(pndphi)>>hPNDPhiTrk","("+evtSel+"&&ppt>=1.2)*(ppt)");
  djcalopix->Draw("abs(pndphi)>>hPNDPhiPix","("+evtSel+"&&ppt>0.3&&ppt<1.2)*(ppt)");
  djcalo->Draw("abs(padphi)>>hPADPhiTrk","("+evtSel+"&&ppt>=1.2)*(ppt)");
  djcalopix->Draw("abs(padphi)>>hPADPhiPix","("+evtSel+"&&ppt>0.3&&ppt<1.2)*(ppt)");
  hPNDPhiTrk->Scale(1./(numDJ*hPNDPhiTrk->GetBinWidth(1)));
  hPNDPhiPix->Scale(1./(numDJ*hPNDPhiPix->GetBinWidth(1)));
  hPADPhiTrk->Scale(1./(numDJ*hPADPhiTrk->GetBinWidth(1)));
  hPADPhiPix->Scale(1./(numDJ*hPADPhiPix->GetBinWidth(1)));
  hPNDPhiPix->SetMinimum(0);
  hPNDPhiPix->SetMaximum(800);
  hPNDPhiTrk->SetMinimum(0);
  hPNDPhiTrk->SetMaximum(800);
  hPNDPhiPix->SetMarkerStyle(kFullCircle);
  hPNDPhiTrk->SetMarkerStyle(kFullSquare);
  hPNDPhiPix->SetMarkerColor(kRed);
  hPNDPhiTrk->SetMarkerColor(kRed);
  hPADPhiPix->SetMinimum(0);
  hPADPhiPix->SetMaximum(800);
  hPADPhiTrk->SetMinimum(0);
  hPADPhiTrk->SetMaximum(800);
  hPADPhiPix->SetMarkerStyle(kOpenCircle);
  hPADPhiTrk->SetMarkerStyle(kOpenSquare);
  hPADPhiPix->SetMarkerColor(kBlue);
  hPADPhiTrk->SetMarkerColor(kBlue);
  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  hPNDPhiPix->Draw("E");
  hPADPhiPix->Draw("Esame");
  TCanvas * c3_2 = new TCanvas("c3_2","c3_2",500,500);
  hPNDPhiTrk->Draw("E");
  hPADPhiTrk->Draw("Esame");

  TCanvas * c4 = new TCanvas("c4","c4",500,500);
  TH1D * hPNDPhiPixRB = (TH1D*)hPNDPhiPix->Clone("hPNDPhiPixRB");
  TH1D * hPNDPhiTrkRB = (TH1D*)hPNDPhiTrk->Clone("hPNDPhiTrkRB");
  hPNDPhiPixRB->Rebin(20);
  hPNDPhiTrkRB->Rebin(20);
  hPNDPhiPixRB->Draw("E");
  hPNDPhiTrkRB->Draw("Esame");
  hPNDPhiTrkRB->Draw("Esame");
  for (Int_t i=0; i<hPNDPhiPixRB->GetNbinsX()+2; ++i) {
    Float_t dx = hPNDPhiPix->GetBinWidth(1);
    Float_t y=hPNDPhiPixRB->GetBinContent(i);
    cout << "Pix: " << i << ": " << y << "  *dx = " << y*dx<< endl;
  }
  for (Int_t i=0; i<hPNDPhiTrkRB->GetNbinsX()+2; ++i) {
    Float_t dx = hPNDPhiTrk->GetBinWidth(1);
    Float_t y=hPNDPhiTrkRB->GetBinContent(i);
    cout << "Trk: " << i << ": " << y << "  *dx = " << y*dx<< endl;
  }
}
