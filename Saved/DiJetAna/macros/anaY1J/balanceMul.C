#include <iostream>
#include <map>
#include <vector>
#include "TCanvas.h"
#include "TH1.h"
#include "TChain.h"
#include "TMath.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
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

void balanceMul(TString infile="dj_HCPR-J50U-hiGoodMergedTracks_Runs152562_152791_StdAna1204v2.root",
    Int_t doBkgSub=1,
    Float_t centMin=0,
    Float_t centMax=10,
    Float_t AjMin=0.24,
    Float_t AjMax=1)
{
  TChain * djcalo = new TChain("djcalo/djTree");
  djcalo->Add(infile);
  aliases_dijet(djcalo);
  TCut evtSel(Form("cent>=%.0f && cent<%.0f && nljet>120 && abs(nljeta)<2 && aljet>50 && abs(aljeta)<2 && jdphi>2.5 && Aj>=%.2f && Aj<%.2f",
	centMin,centMax,AjMin,AjMax));
  if (doBkgSub==1)
    evtSel = evtSel && "abs(aljeta-nljeta)>=1.4";

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  djcalo->Draw("Aj>>hAj(20,0,1)",evtSel);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hJDPhi = new TH1D("hJDPhi","hJDPhi",50,0,3.1416);
  Float_t numDJ = djcalo->Draw("jdphi>>hJDPhi",evtSel);
  cout << "num djs: " << numDJ << endl;

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  djcalo->Draw("peta-nljeta",evtSel);
  djcalo->Draw("peta-aljeta",evtSel,"sameE");

  map<TString,TH1D*> histsDPhi;
  vector<TH1D*> histsBalance;
  Double_t deta=0.7;
  Int_t colors[20] = {kBlack,kGray+2,kViolet,kBlue,kGreen+2,kOrange+2,kMagenta,kRed};
  Int_t mkstls[20] = {kCircle,kOpenDiamond,kOpenStar,kStar,kOpenSquare,kOpenCross,kOpenDiamond};
  const Int_t numPtBins = 5;
  Float_t ptBins[numPtBins+3] = {0,1,2,4,8,10000,0,1000};

  TCanvas * cPJDPhi = new TCanvas("cPJDPhi","cPJDPhi",1200,600);
  cPJDPhi->Divide(4,2);
  for (Int_t i=0; i<numPtBins+2; ++i) {
    if (ptBins[i]>ptBins[i+1]) continue;
    // All
    TString nameNr=Form("hPNDPhiTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]);
    TH1D * hNr = plotPJDPhi(djcalo,evtSel,"nljeta","abs(pndphi)",nameNr,deta,ptBins[i],ptBins[i+1]);
    TString nameAw=Form("hPADPhiTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]);
    TH1D * hAw = plotPJDPhi(djcalo,evtSel,"aljeta","abs(padphi)",nameAw,deta,ptBins[i],ptBins[i+1]);
    // Bkg
    TString nameNrBkg=Form("hPNDPhiBkgTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]);
    TH1D * hNrBkg = plotPJDPhi(djcalo,evtSel,"nljeta","PI-abs(pndphi)",nameNrBkg,deta,ptBins[i],ptBins[i+1]);
    TString nameAwBkg=Form("hPADPhiBkgTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]);
    TH1D * hAwBkg = plotPJDPhi(djcalo,evtSel,"aljeta","PI-abs(padphi)",nameAwBkg,deta,ptBins[i],ptBins[i+1]);
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
    hNrBkg->SetLineColor(kRed);
    hAwBkg->SetLineColor(kBlue);
    hNr->Draw("E");
    hAw->Draw("sameE");
    hNrBkg->Draw("same hist");
    hAwBkg->Draw("same hist");
    // difference
    TH1D * hDiff = (TH1D*)hNr->Clone(Form("hPJDPhiDiffTrk_Pt%.0fto%.0f",ptBins[i],ptBins[i+1]));
    hDiff->Add(hNr,hAw,1,-1);
    hDiff->SetMarkerStyle(mkstls[i]);
    hDiff->SetMarkerColor(colors[i]);
    hDiff->SetLineColor(colors[i]);
    hDiff->SetYTitle("Near-Away");
    histsBalance.push_back(hDiff);
    // Legend
    TLegend *t = new TLegend(0.25,0.76,0.92,0.91);
    t->SetHeader(Form("Trk p_{T}: %.1f to %.1f [GeV],  |#Delta#eta(trk,jet)|<%.1f",ptBins[i],ptBins[i+1],deta));
    t->SetTextSize(0.04);
    t->SetBorderSize(0);
    t->SetFillStyle(0);
    if (i==0) {
      t->AddEntry(hNr,"Near","pl");
      t->AddEntry(hNrBkg,"Near Bkg","l");
      t->AddEntry(hAw,"Away","pl");
      t->AddEntry(hAwBkg,"Away Bkg","l");
    }
    t->Draw();
  }
  cPJDPhi->Print(Form("PJDPhi_Cent%.0fto%.0f_DEta%.0f_Aj%.0fto%.0f_BkgSub%d.gif",centMin,centMax,deta*10,AjMin*100,AjMax*100,doBkgSub));
  cPJDPhi->Print(Form("PJDPhi_Cent%.0fto%.0f_DEta%.0f_Aj%.0fto%.0f_BkgSub%d.C",centMin,centMax,deta*10,AjMin*100,AjMax*100,doBkgSub));

  TCanvas * cBalance = new TCanvas("cBalance","cBalance",500,500);
  histsBalance[histsBalance.size()-1]->SetMarkerStyle(kFullCircle);
  histsBalance[histsBalance.size()-1]->SetMarkerColor(kBlack);
  histsBalance[histsBalance.size()-1]->SetLineColor(kBlack);
  histsBalance[histsBalance.size()-1]->Draw("E");
  TLegend *t = new TLegend(0.48,0.61,0.92,0.91);
  t->SetHeader(Form("|#Delta#eta(trk,jet)|<%.1f",deta));
  t->SetTextSize(0.04);
  t->SetBorderSize(0);
  t->SetFillStyle(0);
  t->AddEntry(histsBalance[histsBalance.size()-1],"All Pt","pl");
  for (Int_t i=0; i<histsBalance.size(); ++i) {
    histsBalance[i]->Draw("Esame");
    if (i<histsBalance.size()-1)
      t->AddEntry(histsBalance[i],Form("Pt %.0f to %.0f",ptBins[i],ptBins[i+1]),"pl");
  }
  t->Draw();
  TLine *l = new TLine(0,0,3.14,0);
  l->SetLineStyle(2);
  l->Draw();
  cBalance->Print(Form("Balance_Cent%.0fto%.0f_DEta%.0f_Aj%.0fto%.0f_BkgSub%d.gif",centMin,centMax,deta*10,AjMin*100,AjMax*100,doBkgSub));
  cBalance->Print(Form("Balance_Cent%.0fto%.0f_DEta%.0f_Aj%.0fto%.0f_BkgSub%d.C",centMin,centMax,deta*10,AjMin*100,AjMax*100,doBkgSub));
}
