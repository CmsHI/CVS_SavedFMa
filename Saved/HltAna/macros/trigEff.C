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
using namespace std;

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2,char *hName="hEff")
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(h2,h1);
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
  t->Draw(var+">>h"+tag,sel);
  t->Draw(var+">>hCut"+tag,sel&&cut);
  TGraphAsymmErrors *g = calcEff(h,hCut);
  //g->Draw("ap");
  return g;
}

TChain * trigEff(
    TString infile="../process_aod/dj_HCPR-MB-151020to151076_trigana1116.root",
    TString header="HLT_HIJet50U_Core"
    )
{
  TChain * dj = new TChain("djcalo/djTree");

  dj->Add(infile);
  dj->AddFriend("hltanalysis/HltTree",infile);
  dj->AddFriend("joc = djcaloJOC/djTree",infile);
  dj->AddFriend("ic5 = djcaloic5/djTree",infile);
  dj->AddFriend("ak5 = djcaloak5/djTree",infile);
  dj->AddFriend("kt4 = djcalokt4/djTree",infile);
  cout << "Total: " << dj->GetEntries() << endl;

  TCut evtSel("hlt[0]&&cent>50");

  TGraphAsymmErrors *g0=0,*g1=0,*g2=0,*g3=0,*g4=0;
  g0 = eff(dj,"nljet",evtSel,"hlt[2]","ic5pu");
  g1 = eff(dj,"joc.nljet",evtSel,"hlt[2]","joc");
  g2 = eff(dj,"ak5.nljet",evtSel,"hlt[2]","ak5");
  g3 = eff(dj,"kt4.nljet",evtSel,"hlt[2]","kt4");
  g4 = eff(dj,"ic5.nljet",evtSel,"hlt[2]","ic5fj");

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
  g2->SetLineColor(kRed);
  g2->SetMarkerColor(kRed);
  g2->SetMarkerStyle(kOpenCircle);
  g3->SetMarkerColor(kBlue);
  g3->SetLineColor(kBlue);
  g3->SetMarkerStyle(kOpenStar);
  g4->SetMarkerStyle(kOrange);
  g4->SetLineStyle(kOrange);
  g4->SetMarkerStyle(kOpenDiamond);
  hTmp->SetAxisRange(0,1.3,"Y");
  hTmp->SetXTitle("Leading E_{T}^{Jet,Corr} [GeV]");
  hTmp->SetYTitle("Eff. (Trigger/MB)");
  hTmp->Draw();
  g0->Draw("p");
  g1->Draw("p");
  g2->Draw("p");
  //g3->Draw("p");
  //g4->Draw("p");
  TLine *l = new TLine(0,1,bin[nBin],1);
  l->SetLineStyle(2);
  l->Draw();
  TLegend *t = new TLegend(0.23,0.78,0.83,0.91);
  t->SetHeader(header);
  t->SetBorderSize(0);
  t->SetFillStyle(0);
  t->AddEntry(g1,"Ic5pu (Online)","pl");
  t->AddEntry(g0,"Ic5pu (Offline)","pl");
  t->AddEntry(g2,"Ak5","pl");
  //t->AddEntry(g3,"Kt4","pl");
  //t->AddEntry(g4,"Ic5Fj","pl");
  t->Draw();

  return dj;
}
