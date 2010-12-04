#include <iostream>
#include <map>
#include "TCanvas.h"
#include "TH1.h"
#include "TChain.h"
#include "TMath.h"
#include "TCut.h"
#include "Saved/DiJetAna/macros/aliases_dijet.C"
using namespace std;

TH1D * plot1D(TTree * tr,TCut sel, TCut cut,TString var, TString name, TString title,Int_t nbins, Float_t min, Float_t max,TString weight="",Int_t normType=0, Float_t normFactor=1)
{
  cout << name << endl;
  Float_t nSelEvt=tr->GetEntries(sel);
  cout << "Sel Evt:  " << TString(sel) << ": " << nSelEvt << endl;
  TH1D * hist = new TH1D(name,title,nbins,min,max);
  TString finalCut;
  if (weight=="")
    finalCut = TString(sel&&cut);
  else
    finalCut = "("+TString(sel&&cut)+")*"+weight;
  Int_t npass = tr->Draw(var+">>"+name,finalCut,"goff");
  cout << "Sel Draw: " << finalCut << ": " << npass << endl;
  if (normType==3)
    hist->Scale(1./(nSelEvt*hist->GetBinWidth(1)));
  if (normType==4)
    hist->Scale(1./(nSelEvt*hist->GetBinWidth(1)*normFactor));
  return hist;
}

TH1D * plotPJDPhi(TTree * tr, TCut sel, TString jeta, TString var, TString name, Float_t deta, Float_t ptMin, Float_t ptMax)
{
  TCut cut=Form("abs(peta-%s)<%.2f&&ppt>=%.2f&&ppt<%.2f",jeta.Data(),deta,ptMin,ptMax);
  return plot1D(tr,sel,cut,var,name,
      ";#Delta#phi(trk,jet);1/N_{DJ} dp_{T}^{Trk}/d(#Delta#phi#Delta#eta)",
      16,0,TMath::PiOver2(),
      "ppt",4,deta);
}

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

  map<TString,TH1D*> histsDPhi;
  Double_t deta=1.5;
  TH1D * hPNDPhiTrk = plotPJDPhi(djcalo,TCut(evtSel),"nljeta","abs(pndphi)","hPNDPhiTrk",deta,1.5,10000);
  TH1D * hPADPhiTrk = plotPJDPhi(djcalo,TCut(evtSel),"aljeta","abs(padphi)","hPADPhiTrk",deta,1.5,10000);
  TH1D * hPNDPhiPix = plotPJDPhi(djcalo,TCut(evtSel),"nljeta","abs(pndphi)","hPNDPhiPix",deta,0.3,10000);
  TH1D * hPADPhiPix = plotPJDPhi(djcalo,TCut(evtSel),"aljeta","abs(padphi)","hPADPhiPix",deta,0.3,10000);

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
  for (Int_t i=0; i<hPADPhiPixRB->GetNbinsX()+2; ++i) {
    Float_t dx = hPADPhiPixRB->GetBinWidth(1);
    Float_t y=hPADPhiPixRB->GetBinContent(i);
    cout << "Away: " << i << ": " << y << "  *dx = " << y*dx<< endl;
    ptSumAway+=y*dx;
  }
  cout << "Away Sum: " << ptSumAway << endl;
}
