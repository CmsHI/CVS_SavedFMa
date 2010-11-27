#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TTree.h>
#include "TChain.h"
#include <TH1F.h>
#include <TCut.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCanvas.h>
#include <iostream>

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/Utilities/macros/graph/tgraphTools.C"
using namespace std;

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2,char *hName="hEff")
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

  //const int nBin=11;
  //Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300};
  const int nBin=25;
  Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200,240,300};
  TH1F *h = new TH1F("h"+tag,"",nBin,bin);
  TH1F *hCut = new TH1F("hCut"+tag,"",nBin,bin);
  t->Draw(var+">>h"+tag,sel,"goff");
  t->Draw(var+">>hCut"+tag,sel&&cut,"goff");
  TGraphAsymmErrors *g = calcEff(h,hCut);
  //g->Draw("ap");
  return g;
}

TChain * effJet(bool doMC=1,
    TString infile="dj_Data_MinBias0to20_DijetUnquenched50_d20101124_StdJetGoodTrk1126.root",
    TString header="QuenDiJet-DataBackground"
    )
{
  TChain * dj = new TChain("djgen/djTree");

  dj->Add(infile);
  dj->AddFriend("djcalo = djcalo/djTree",infile);
  aliases_dijet(dj,1.2,doMC,"djcalo");
  cout << "Total: " << dj->GetEntries() << endl;

  TCut evtSel("cent<10 && nljet>0 && abs(nljeta)<2 && aljet>0 && abs(aljeta)<2");
  //evtSel = evtSel && "djgen.nljet>0&&djgen.aljet>0" //for now abs eff --- no selection on mc
  evtSel = evtSel && "jdphi>TMath::Pi()*5/6";
  //TCut evtSel("HLT_HIJet50U && cent<10 ");

  TCanvas * cDr = new TCanvas("cDr","cDr",500,500);
  dj->Draw("tr2nljdr",evtSel,"hist");
  dj->Draw("tr2aljdr",evtSel,"sameE");

  TGraphAsymmErrors *g0=0,*g1=0,*g2=0,*g3=0,*g4=0;
  g0 = eff(dj,"nljet",evtSel,"tr2nljdr<0.3","NrJEt");
  g1 = eff(dj,"aljet",evtSel,"tr2aljdr<0.3","AwJEt");

  // Draw
  TCanvas *cTrigEff = new TCanvas("cTrigEff","cTrigEff",600,600);
  //const int nBin=11;
  //Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300};
  const int nBin=25;
  Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200,240,300};
  TH1F *hTmp = new TH1F("hTmp","",nBin,bin);
  g1->SetLineColor(kGreen+2);
  g1->SetMarkerColor(kGreen+2);
  g1->SetMarkerStyle(kOpenSquare);
  hTmp->SetAxisRange(0,1.3,"Y");
  hTmp->SetXTitle("Leading E_{T}^{Jet,Corr} [GeV]");
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
  t->AddEntry(g0,"Leading Jet","pl");
  t->AddEntry(g1,"Away Jet","pl");
  t->Draw();

  return dj;
}
