#include <iostream>
#include <map>
#include "TCanvas.h"
#include "TH1.h"
#include "TChain.h"
#include "TMath.h"
#include "TCut.h"
#include "Saved/DiJetAna/macros/aliases_dijet.C"
using namespace std;

//
// Macro to draw trk pt in two hemisphers
//

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
  if (normType==1)
    hist->Scale(1./nSelEvt);
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
      40,0,TMath::Pi(),
      "ppt",1);
}

void balance(TString infile="dj_HCPR-J50U-JSON_hiGoodMergedTrksRuns152562to152643-v1_StdAna1204v2.root")
{
  TH1::SetDefaultSumw2();
  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile);
  aliases_dijet(djcalo);
  TString evtSel("(cent<10 && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2./3*TMath::Pi() && Aj>=0)");

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  djcalo->Draw("Aj>>hAj(20,0,1)",evtSel);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  Float_t numDJ = djcalo->Draw("jdphi>>hJDPhi",evtSel);
  cout << "num djs: " << numDJ << endl;

  Double_t deta=10;
  TH1D * hPNDPhiTrk = plotPJDPhi(djcalo,TCut(evtSel),"nljeta","abs(pndphi)","hPNDPhiTrk",deta,1.5,2);
  //TH1D * hPADPhiTrk = plotPJDPhi(djcalo,TCut(evtSel),"aljeta","abs(padphi)","hPADPhiTrk",deta,10,10000);

  //hPNDPhiTrk->SetMinimum(0.01);
  //hPNDPhiTrk->SetMaximum(700);
  hPNDPhiTrk->SetMarkerStyle(kOpenSquare);
  hPNDPhiTrk->SetMarkerColor(kRed);
  //hPADPhiTrk->SetMarkerColor(kBlue);
  TCanvas * c3_2 = new TCanvas("c3_2","c3_2",500,500);
  hPNDPhiTrk->Draw("E");
  //hPADPhiTrk->Draw("Esame");

  Float_t ptSumNear=0,ptSumAway=0;
  for (Int_t i=0; i<hPNDPhiTrk->GetNbinsX()+2; ++i) {
    Float_t dx = hPNDPhiTrk->GetBinWidth(1);
    Float_t y=hPNDPhiTrk->GetBinContent(i);
    if (i>=1&&i<=(hPNDPhiTrk->GetNbinsX()/2.)) {
      //cout << "Near: " << i << ": " << y << "  *dx = " << y*dx<< endl;
      ptSumNear+=y;
    }
    if (i>=(hPNDPhiTrk->GetNbinsX()/2.+1)&&i<=(hPNDPhiTrk->GetNbinsX())) {
      //cout << "Away: " << i << ": " << y << "  *dx = " << y*dx<< endl;
      ptSumAway+=y;
    }
  }
  cout << "Near Sum: " << ptSumNear << endl;
  cout << "Away Sum: " << ptSumAway << endl;
  /*
  Float_t ptSumAway=0;
  for (Int_t i=0; i<hPADPhiTrkRB->GetNbinsX()+2; ++i) {
    Float_t dx = hPADPhiTrkRB->GetBinWidth(1);
    Float_t y=hPADPhiTrkRB->GetBinContent(i);
    cout << "Away: " << i << ": " << y << "  *dx = " << y*dx<< endl;
    ptSumAway+=y*dx;
  }
  */
}
