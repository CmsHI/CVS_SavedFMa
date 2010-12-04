#include <iostream>
#include "TCanvas.h"
#include "TH1.h"
#include "TChain.h"
#include "TMath.h"
#include "Saved/DiJetAna/macros/aliases_dijet.C"
using namespace std;

void balance(TString infile="dj_HCPR-J50U-JSON_hiGoodMergedTrksRuns152562to152643-v1_StdAna1204v2.root")
{
  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile);
  aliases_dijet(djcalo);
  TString evtSel("(cent<10 && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2.5 && Aj>0.24)");

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  djcalo->Draw("Aj>>hAj(20,0,1)",evtSel);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  Float_t numDJ = djcalo->Draw("jdphi>>hJDPhi",evtSel);
  cout << "num djs: " << numDJ << endl;

  TH1D * hPNDPhiTrk = new TH1D("hPNDPhiTrk",";#Delta#phi(trk,jet);1/N_{DJ} dp_{T}^{Trk}/d(#Delta#phi#Delta#eta)",16,0,TMath::PiOver2());
  TH1D * hPNDPhiPix = new TH1D("hPNDPhiPix",";#Delta#phi(trk,jet);1/N_{DJ} dp_{T}^{Trk}/d(#Delta#phi#Delta#eta)",16,0,TMath::PiOver2());
  TH1D * hPADPhiTrk = new TH1D("hPADPhiTrk",";#Delta#phi(trk,jet);1/N_{DJ} dp_{T}^{Trk}/d(#Delta#phi#Delta#eta)",16,0,TMath::PiOver2());
  TH1D * hPADPhiPix = new TH1D("hPADPhiPix",";#Delta#phi(trk,jet);1/N_{DJ} dp_{T}^{Trk}/d(#Delta#phi#Delta#eta)",16,0,TMath::PiOver2());
  Double_t deta=1.5;
  djcalo->Draw("abs(pndphi)>>hPNDPhiTrk",Form("(%s&&abs(peta-nljeta)<%f&&ppt>=1.5)*(ppt)",evtSel.Data(),deta),"goff");
  djcalo->Draw("abs(pndphi)>>hPNDPhiPix",Form("(%s&&abs(peta-nljeta)<%f&&ppt>=0.3)*(ppt)",evtSel.Data(),deta),"goff");
  djcalo->Draw("abs(padphi)>>hPADPhiTrk",Form("(%s&&abs(peta-aljeta)<%f&&ppt>=0.3&&ppt>=1.5)*(ppt)",evtSel.Data(),deta),"goff");
  djcalo->Draw("abs(padphi)>>hPADPhiPix",Form("(%s&&abs(peta-aljeta)<%f&&ppt>=0.3)*(ppt)",evtSel.Data(),deta),"goff");
  hPNDPhiTrk->Scale(1./(numDJ*hPNDPhiTrk->GetBinWidth(1)*deta));
  hPNDPhiPix->Scale(1./(numDJ*hPNDPhiPix->GetBinWidth(1)*deta));
  hPADPhiTrk->Scale(1./(numDJ*hPADPhiTrk->GetBinWidth(1)*deta));
  hPADPhiPix->Scale(1./(numDJ*hPADPhiPix->GetBinWidth(1)*deta));
  hPNDPhiPix->SetMinimum(0.01);
  hPNDPhiPix->SetMaximum(1200);
  hPNDPhiTrk->SetMinimum(0.01);
  hPNDPhiTrk->SetMaximum(700);
  hPNDPhiPix->SetMarkerStyle(kFullCircle);
  hPNDPhiTrk->SetMarkerStyle(kFullSquare);
  hPNDPhiPix->SetMarkerColor(kRed);
  hPNDPhiTrk->SetMarkerColor(kRed);
  hPADPhiPix->SetMinimum(0.01);
  hPADPhiPix->SetMaximum(1200);
  hPADPhiTrk->SetMinimum(0.01);
  hPADPhiTrk->SetMaximum(700);
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
  TH1D * hPADPhiPixRB = (TH1D*)hPADPhiPix->Clone("hPADPhiPixRB");
  hPNDPhiPixRB->Rebin(4);
  hPNDPhiPixRB->SetMaximum(4*1200);
  hPADPhiPixRB->Rebin(4);
  hPADPhiPixRB->SetMaximum(4*1200);
  hPNDPhiPixRB->Draw("E");
  hPADPhiPixRB->Draw("Esame");
  Float_t ptSumNear=0;
  for (Int_t i=0; i<hPNDPhiPixRB->GetNbinsX()+2; ++i) {
    Float_t dx = hPNDPhiPix->GetBinWidth(1);
    Float_t y=hPNDPhiPixRB->GetBinContent(i);
    cout << "Near: " << i << ": " << y << "  *dx = " << y*dx<< endl;
    ptSumNear+=y*dx;
  }
  cout << "Near Sum: " << ptSumNear << endl;
  Float_t ptSumAway=0;
  for (Int_t i=0; i<hPADPhiPix->GetNbinsX()+2; ++i) {
    Float_t dx = hPADPhiPix->GetBinWidth(1);
    Float_t y=hPADPhiPixRB->GetBinContent(i);
    cout << "Away: " << i << ": " << y << "  *dx = " << y*dx<< endl;
    ptSumAway+=y*dx;
  }
  cout << "Away Sum: " << ptSumAway << endl;
}
