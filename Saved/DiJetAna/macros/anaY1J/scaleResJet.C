#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TTree.h>
#include "TChain.h"
#include <TH1F.h>
#include "TH2.h"
#include <TCut.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCanvas.h>
#include "TProfile.h"
#include <iostream>

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/Utilities/macros/cplot/CPlot.h"           // helper class for plots
#include "Saved/Utilities/macros/graph/tgraphTools.C"
using namespace std;

const int nBin=25;
Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200,240,300};
const int nBinRat=20;
Float_t binRat[nBinRat+1];

TH2F * JES(TTree * t,TString var="nljet/nlrjet:nlrjet",TCut sel="",TCut cut="",TString tag="")
{
  cout << "var: " << var << endl;
  cout << "Trigger: " << TString(sel&&cut) << ": " << t->GetEntries(sel&&cut) << endl;

  TH2F *h2d = new TH2F("h"+tag,"",nBin,bin,nBinRat,binRat);
  //TH2F *h2d = new TH2F("h"+tag,"",50,0,200,20,0,2);
  t->Draw(var+">>h"+tag,sel&&cut,"goff");

  h2d->FitSlicesY();
  //h2d->FitSlicesY(0,0,-1,10);
  return h2d;
}

TChain * scaleResJet(bool doMC=1,
    //TString infile0="dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127v2.root",
    TString infile0="dj_Data_MinBias_DijetUnquenched80_d20101125and1126_MatchedJetGoodTrk1127v2.root",
    //TString infile1="dj_Data_MinBias0to20_DijetUnquenched50_d20101124_StdJetGoodTrk1126.root",
    TString header="McDiJet-DataBackground"
    )
{
  TChain * dj = new TChain("djcalo/djTree");
  //TChain * dj1 = new TChain("djcalo/djTree");

  dj->Add(infile0);
  dj->AddFriend("djgen = djgen/djTree",infile0);
  aliases_dijet(dj,1.2,doMC,"djgen");
  cout << "dj0 Total: " << dj->GetEntries() << endl;

  //dj1->Add(infile1);
  //dj1->AddFriend("djgen = djgen/djTree",infile1);
  //aliases_dijet(dj1,1.2,doMC,"djgen");
  //cout << "dj1 Total: " << dj1->GetEntries() << endl;

  TCut evtSel("cent<10 && nlrjet>80 && abs(nljeta)<2 && alrjet>0 && abs(aljeta)<2 && jdphi>TMath::Pi()*5/6");
  //evtSel = evtSel && "djgen.nljet>0&&djgen.aljet>0" //for now abs eff --- no selection on mc
  TCut evtSelAw = evtSel && "alrjet>80";
  //TCut evtSel("HLT_HIJet50U && cent<10 ");
  
  for (int i=0; i<=nBinRat;++i) binRat[i]=i*2./nBinRat;

  TCanvas * cEtNr2D = new TCanvas("cEtNr2D","cEtNr2D",500,500);
  TH2D * hEtNr2D = new TH2D("hEtNr2D",";E_{T}^{GenJet};E_{T}^{RecoJet}",60,0,300,60,0,300);
  dj->Draw("nljet:nlrjet>>hEtNr2D",evtSel,"colz");
  hEtNr2D->ProfileX()->Draw("same");
  TLine *l45 = new TLine(0,0,300,300);
  l45->SetLineStyle(2);
  l45->Draw();
  TCanvas * cEtAw2D = new TCanvas("cEtAw2D","cEtAw2D",500,500);
  TH2D * hEtAw2D = new TH2D("hEtAw2D",";E_{T}^{GenJet};E_{T}^{RecoJet}",60,0,300,60,0,300);
  dj->Draw("aljet:alrjet>>hEtAw2D",evtSelAw,"colz");
  hEtAw2D->ProfileX()->Draw("same");
  l45->Draw();

  TCanvas * cDr = new TCanvas("cDr","cDr",500,500);
  dj->Draw("nlrjdr",evtSel,"hist");
  dj->Draw("alrjdr",evtSelAw,"sameE");

  TH2F * hJESNr2D = JES(dj,"nljet/nlrjet:nlrjet",evtSel,"nlrjdr<0.3","JESNr2D");
  TH1D * hJESNr2D_1 = (TH1D*)gDirectory->Get("hJESNr2D_1");
  TH1D * hJESNr2D_2 = (TH1D*)gDirectory->Get("hJESNr2D_2");
  TH2F * hJESAw2D = JES(dj,"aljet/alrjet:alrjet",evtSelAw,"alrjdr<0.3","JESAw2D");
  TH1D * hJESAw2D_1 = (TH1D*)gDirectory->Get("hJESAw2D_1");
  TH1D * hJESAw2D_2 = (TH1D*)gDirectory->Get("hJESAw2D_2");

  // Draw
  TCanvas * cJES = new TCanvas("cJES","cJES",500,500);
  CPlot cpJES("JES","JES","E_{T}^{GenJet} [GeV]","E_{T}^{RecoJet}/E_{T}^{GenJet}");
  cpJES.SetYRange(0,1.5);
  cpJES.AddHist1D(hJESNr2D_1,"Leading Jet Reponse","E",kBlack,kFullCircle);
  cpJES.AddHist1D(hJESAw2D_1,"Away Jet Reponse","E",kRed,kFullSquare);
  cpJES.AddHist1D(hJESNr2D_2,"Leading Jet Resolution","E",kBlack,kOpenCircle);
  cpJES.AddHist1D(hJESAw2D_2,"Away Jet Resolution","E",kRed,kOpenSquare);
  cpJES.SetLegend(0.54,0.8,0.86,0.92);
  cpJES.Draw(cJES,false);
  TLine *l = new TLine(0,1,bin[nBin],1);
  l->SetLineStyle(2);
  l->Draw();

  return dj;
}
