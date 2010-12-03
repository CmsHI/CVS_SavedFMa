#include "Saved/DiJetAna/macros/aliases_dijet.C"

void anaFragShape(TString infile="dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root")
{
  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile);
  aliases_dijet(djcalo);
  TChain * djcalopix = new TChain("djcalo_pxl/djTree");
  djcalopix->Add(infile);
  aliases_dijet(djcalopix);
  TString evtSel("(cent<10 && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2.5 && Aj>0.24)");
  TH1::SetDefaultSumw2();

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  djcalo->Draw("Aj>>hAj(20,0,1)",evtSel);
  djcalopix->Draw("Aj",evtSel,"Esame");

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  TH1D * hJDPhi2 = new TH1D("hJDPhi2","hJDPhi",50,0,3.1416);
  Float_t numDJ = djcalo->Draw("jdphi>>hJDPhi",evtSel);
  Float_t numDJ2 = djcalopix->Draw("jdphi>>hJDPhi2",evtSel);
  cout << "num djs: " << numDJ << "  djs(pix)" << numDJ2 << endl;

  TH1D * hPNDRTrk = new TH1D("hPNDRTrk",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPNDRPix = new TH1D("hPNDRPix",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRTrk = new TH1D("hPADRTrk",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  TH1D * hPADRPix = new TH1D("hPADRPix",";R(trk,jet);1/(N_{DJ} 2#piR) dp_{T}^{Trk}/dR",10,0,TMath::PiOver2());
  djcalo->Draw("pndr>>hPNDRTrk",Form("(%s&&ppt>=1.2)*(ppt/(TMath::TwoPi()*pndr))",evtSel.Data()));
  djcalopix->Draw("pndr>>hPNDRPix",Form("(%s&&ppt>=0.3&&ppt<1.2)*(ppt/(TMath::TwoPi()*pndr))",evtSel.Data()));
  djcalo->Draw("padr>>hPADRTrk",Form("(%s&&ppt>=1.2)*(ppt/(TMath::TwoPi()*padr))",evtSel.Data()));
  djcalopix->Draw("padr>>hPADRPix",Form("(%s&&ppt>=0.3&&ppt<1.2)*(ppt/(TMath::TwoPi()*padr))",evtSel.Data()));
  hPNDRTrk->Scale(1./(numDJ*hPNDRTrk->GetBinWidth(1)));
  hPNDRPix->Scale(1./(numDJ*hPNDRPix->GetBinWidth(1)));
  hPADRTrk->Scale(1./(numDJ*hPADRTrk->GetBinWidth(1)));
  hPADRPix->Scale(1./(numDJ*hPADRPix->GetBinWidth(1)));
  hPNDRPix->SetMinimum(50);
  hPNDRPix->SetMaximum(100);
  hPNDRTrk->SetMinimum(10);
  hPNDRTrk->SetMaximum(2000);
  hPNDRPix->SetMarkerStyle(kFullCircle);
  hPNDRTrk->SetMarkerStyle(kFullSquare);
  hPNDRPix->SetMarkerColor(kRed);
  hPNDRTrk->SetMarkerColor(kRed);
  hPADRPix->SetMarkerStyle(kOpenCircle);
  hPADRTrk->SetMarkerStyle(kOpenSquare);
  hPADRPix->SetMarkerColor(kBlue);
  hPADRTrk->SetMarkerColor(kBlue);
  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  hPNDRPix->Draw("E");
  hPADRPix->Draw("Esame");
  TCanvas * c3_2 = new TCanvas("c3_2","c3_2",500,500);
  c3_2->SetLogy();
  hPNDRTrk->Draw("E");
  hPADRTrk->Draw("Esame");
}
