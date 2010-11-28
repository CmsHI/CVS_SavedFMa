#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TTree.h>
#include "TChain.h"
#include <TH1F.h>
#include "TH2F.h"
#include <TCut.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCanvas.h>
#include <iostream>

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/Utilities/macros/graph/tgraphTools.C"
using namespace std;

//const int nBin=11;
//Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300};
const int nBin=25;
Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200,240,300};
const int nBinRat=100;
Float_t binRat[nBinRat+1];

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2,TString hName="hEff")
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(h2,h1);
   clearXErrorBar(gEfficiency);
   return gEfficiency;
}

TGraphAsymmErrors *eff(TTree * t,TString var="nljet",TCut sel="",TCut cut="",TString tag="")
{
  cout << "var: " << var << endl;
  cout << "Evt Sel: " << TString(sel) << ": " << t->GetEntries(sel) << endl;
  cout << "Trigger: " << TString(sel&&cut) << ": " << t->GetEntries(sel&&cut) << endl;

  TH1F *h = new TH1F("h"+tag,"",nBin,bin);
  TH1F *hCut = new TH1F("hCut"+tag,"",nBin,bin);
  t->Draw(var+">>h"+tag,sel,"goff");
  t->Draw(var+">>hCut"+tag,sel&&cut,"goff");
  TGraphAsymmErrors *g = calcEff(h,hCut);
  //g->Draw("ap");
  return g;
}

TH2F * eff2d(TTree * t,TString var="aljet:nljet",TCut sel="",TCut cut="",TString tag="")
{
  cout << "var: " << var << endl;
  cout << "Evt Sel: " << TString(sel) << ": " << t->GetEntries(sel) << endl;
  cout << "Trigger: " << TString(sel&&cut) << ": " << t->GetEntries(sel&&cut) << endl;

  TH2F *h = new TH2F("h"+tag,";E_{T}^{Leading Jet} [GeV];E_{T}^{Away Jet} [GeV]",nBin,bin,nBin,bin);
  TH2F *hCut = new TH2F("hCut"+tag,";E_{T}^{Leading Jet} [GeV];E_{T}^{Away Jet} [GeV]",nBin,bin,nBin,bin);
  t->Draw(var+">>h"+tag,sel,"");
  t->Draw(var+">>hCut"+tag,sel&&cut,"");
  TH2F *hRat = (TH2F*)h->Clone("h"+tag+"Eff");
  hRat->Divide(hCut,h);
  hRat->SetMaximum(1.);
  return hRat;
}

TChain * effJet(bool doMC=1,
    TString infile0="dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127.root",
    //TString infile1="dj_Data_MinBias_DijetUnquenched80_d20101125_StdJetGoodTrk1126.root",
    TString header="McDiJet-DataBackground"
    )
{
  TChain * dj = new TChain("djgen/djTree");
  //TChain * dj1 = new TChain("djgen/djTree");

  dj->Add(infile0);
  dj->AddFriend("djcalo = djcalo/djTree",infile0);
  aliases_dijet(dj,1.2,doMC,"djcalo");
  cout << "dj0 Total: " << dj->GetEntries() << endl;

  //dj1->Add(infile1);
  //dj1->AddFriend("djcalo = djcalo/djTree",infile1);
  //aliases_dijet(dj1,1.2,doMC,"djcalo");
  //cout << "dj1 Total: " << dj1->GetEntries() << endl;

  TCut evtSel("cent<10 && nljet>30 && abs(nljeta)<2 && aljet>0 && abs(aljeta)<2 && jdphi>TMath::Pi()*5/6");
  //evtSel = evtSel && "djgen.nljet>0&&djgen.aljet>0" //for now abs eff --- no selection on mc
  TCut evtSelAw = evtSel && "nljet>120";
  //TCut evtSel("HLT_HIJet50U && cent<10 ");

  TCanvas * cDr = new TCanvas("cDr","cDr",500,500);
  dj->Draw("nlrjdr>>hDrNr(300,0,6.28)",evtSel);
  dj->Draw("alrjdr>>hDrAw(300,0,6.28)",evtSel,"same");

  TCanvas * cEt = new TCanvas("cEt","cEt",500,500);
  dj->Draw("nljet",evtSel,"hist");
  dj->Draw("nlrjet",evtSel,"hist E");
  dj->Draw("aljet",evtSel,"same hist");
  dj->Draw("alrjet",evtSel,"same E");

  for (int i=0; i<=nBinRat;++i) binRat[i]=i*2./nBinRat;

  TGraphAsymmErrors *g0=0,*g1=0,*g2=0,*g3=0,*g4=0;
  g0 = eff(dj,"nljet",evtSel,"nlrjdr<0.3","NrJEt");
  g1 = eff(dj,"aljet",evtSelAw,"alrjdr<0.3","AwJEt");

  TH2F * hEff2d = eff2d(dj,"aljet:nljet",evtSel,"nlrjdr<0.3&&alrjdr<0.3","JEt2D");

  // Draw
  TCanvas *cJetEff = new TCanvas("cJetEff","cJetEff",500,500);
  TH1F *hTmp = new TH1F("hTmp","",nBin,bin);
  g1->SetLineColor(kGreen+2); g1->SetMarkerColor(kGreen+2); g1->SetMarkerStyle(kOpenSquare);
  hTmp->SetAxisRange(0,1.3,"Y");
  hTmp->SetXTitle("E_{T}^{GenJet} [GeV]");
  hTmp->SetYTitle("Eff. (Matched Reco/Gen)");
  hTmp->Draw();
  g0->Draw("p");
  g1->Draw("p");
  TLine *l = new TLine(0,1,bin[nBin],1);
  l->SetLineStyle(2);
  l->Draw();
  TLegend *t = new TLegend(0.23,0.78,0.83,0.91);
  t->SetHeader(header);
  t->SetBorderSize(0);
  t->SetFillStyle(0);
  t->AddEntry(g0,"UnQuen MC - Leading Jet","pl");
  t->AddEntry(g1,"UnQuen MC - Away Jet","pl");
  t->Draw();

  TCanvas *cJet2DEff = new TCanvas("cJet2DEff","cJet2DEff",500,500);
  hEff2d->Draw("colz");
  cJet2DEff->SetRightMargin(0.2);
  return dj;
}
