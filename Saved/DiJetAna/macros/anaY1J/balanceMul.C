#include <iostream>
#include <map>
#include "TCanvas.h"
#include "TH1.h"
#include "TChain.h"
#include "TMath.h"
#include "TCut.h"
#include "TLegend.h"
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

void balanceMul(TString infile="dj_HCPR-J50U-JSON_hiGoodMergedTrksRuns152562to152643-v1_StdAna1204v2.root",
    Float_t centMin=0,
    Float_t centMax=10,
    Float_t AjMin=0.24,
    Float_t AjMax=1)
{
  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile);
  aliases_dijet(djcalo);
  TString evtSel(Form("(cent>=%.0f && cent<%.0f && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2.5 && Aj>=%.2f && Aj<%.2f)",
	centMin,centMax,AjMin,AjMax));

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  djcalo->Draw("Aj>>hAj(20,0,1)",evtSel);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  Float_t numDJ = djcalo->Draw("jdphi>>hJDPhi",evtSel);
  cout << "num djs: " << numDJ << endl;

  map<TString,TH1D*> histsDPhi;
  Double_t deta=1.5;
  Int_t colors[20] = {kBlack,kGray+2,kViolet,kBlue,kGreen+2,kOrange+2,kMagenta,kRed};
  const Int_t numPtBins = 5;
  Float_t ptBins[numPtBins+3] = {0,1,2,4,8,10000,0,1000};

  TCanvas * cPJDPhi = new TCanvas("cPJDPhi","cPJDPhi",1200,600);
  cPJDPhi->Divide(4,2);
  for (Int_t i=0; i<numPtBins+2; ++i) {
    if (ptBins[i]>ptBins[i+1]) continue;
    TString nameNr=Form("hPNDPhiTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]);
    TH1D * hNr = plotPJDPhi(djcalo,TCut(evtSel),"nljeta","abs(pndphi)",nameNr,deta,ptBins[i],ptBins[i+1]);
    TString nameAw=Form("hPADPhiTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]);
    TH1D * hAw = plotPJDPhi(djcalo,TCut(evtSel),"aljeta","abs(padphi)",nameAw,deta,ptBins[i],ptBins[i+1]);
    cPJDPhi->cd(i+1);
    if ((i+1)<=4)
      hNr->SetMaximum(hNr->GetMinimum()+(hNr->GetMaximum()-hNr->GetMinimum())*2);
    else {
      //hNr->SetMinimum(0.1);
      //hNr->SetMaximum(3000);
      cPJDPhi->GetPad(i+1)->SetLogy();
    }
    hNr->SetLineColor(kRed);
    hNr->SetMarkerColor(kRed);
    hAw->SetMarkerColor(kBlue);
    hAw->SetLineColor(kBlue);
    hAw->SetMarkerStyle(kOpenSquare);
    hNr->Draw("E");
    hAw->Draw("sameE");
    // Legend
    TLegend *t = new TLegend(0.25,0.76,0.92,0.91);
    t->SetHeader(Form("Trk p_{T}: %.1f to %.1f [GeV],  |#Delta#eta(trk,jet)|<%.1f",ptBins[i],ptBins[i+1],deta));
    t->SetTextSize(0.04);
    t->SetBorderSize(0);
    t->SetFillStyle(0);
    if (i==0) {
      t->AddEntry(hNr,"Near","pl");
      t->AddEntry(hAw,"Aw","pl");
    }
    t->Draw();
  }
  cPJDPhi->Print(Form("PJDPhi_Cent%.0fto%.0f_Aj%.0fto%.0f.gif",centMin,centMax,AjMin*100,AjMax*100));
  cPJDPhi->Print(Form("PJDPhi_Cent%.0fto%.0f_Aj%.0fto%.0f.C",centMin,centMax,AjMin*100,AjMax*100));
}
