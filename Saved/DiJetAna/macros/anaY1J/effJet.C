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
#include "TF1.h"
#include <iostream>
using namespace std;

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/commonUtility.h"
#include "Saved/Utilities/macros/graph/tgraphTools.C"

//const int nBin=11;
//Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300};
//const int nBin=25;
//Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200,240,300};
const int nBin=17;
Float_t bin[nBin+1]={
  18, 21, 24, 28, 32, 
  37, 43, 50, 56, 64, 
  74, 84, 97, 114, 133,
  174,220,300 };
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

  TH2F *h = new TH2F("h"+tag,";p_{T}^{Leading Jet} (GeV/c);E_{T}^{Away Jet} (GeV)",nBin,bin,nBin,bin);
  TH2F *hCut = new TH2F("hCut"+tag,";p_{T}^{Leading Jet} (GeV/c);E_{T}^{Away Jet} (GeV/c)",nBin,bin,nBin,bin);
  t->Draw(var+">>h"+tag,sel,"");
  t->Draw(var+">>hCut"+tag,sel&&cut,"");
  TH2F *hRat = (TH2F*)h->Clone("h"+tag+"Eff");
  hRat->Divide(hCut,h);
  hRat->SetMaximum(1.);
  return hRat;
}

TChain * effJet(bool doMC=0,
    TString infile0="dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127.root",
    //TString infile1="dj_Data_MinBias_DijetUnquenched80_d20101125_StdJetGoodTrk1126.root",
    Float_t centMin=0,
    Float_t centMax=10,
    Int_t doJEC=0,
    bool doMultiCanvas=0,
    Int_t doLeg=1,
    TString outtag="DataMix_Pt50"
    )
{
  TString header="PYTHIA+DATA";
  outtag+=Form("_Cent%.0fto%0.f",centMin,centMax);
  TChain * dj = new TChain("djgen/djTree");
  TString tag=Form("GenJet_%.0fto%.0f",centMin,centMax);
  //TChain * dj1 = new TChain("djgen/djTree");

  dj->Add(infile0);
  //dj->AddFriend("djcalo = djcalo/djTree",infile0);
  aliases_dijet(dj,1.2,doMC,"djcalo");
  cout << "dj0 Total: " << dj->GetEntries() << endl;

  //dj1->Add(infile1);
  //dj1->AddFriend("djcalo = djcalo/djTree",infile1);
  //aliases_dijet(dj1,1.2,doMC,"djcalo");
  //cout << "dj1 Total: " << dj1->GetEntries() << endl;

  TCut evtSel(Form("cent>=%.0f && cent<%.0f && nljet>50 && abs(nljeta)<2 && aljet>15 && abs(aljeta)<2 && jdphi>TMath::Pi()*5/6",
	centMin,centMax));
  //evtSel = evtSel && "djgen.nljet>0&&djgen.aljet>0" //for now abs eff --- no selection on mc
  TCut evtSelAw = evtSel;//&& "nljet>120";
  //TCut evtSel("HLT_HIJet50U && cent<10 ");

  for (int i=0; i<=nBinRat;++i) binRat[i]=i*2./nBinRat;

  TGraphAsymmErrors *g0=0,*g1=0,*g2=0,*g3=0,*g4=0;
  TH2F *hEff2d=0;
  if (doJEC==0) {
    g0 = eff(dj,"nljet",evtSel,"nlrjdr<0.3","NrJEt");
    g1 = eff(dj,"aljet",evtSelAw,"alrjdr<0.3","AwJEt");
    hEff2d = eff2d(dj,"aljet:nljet",evtSel,"nlrjdr<0.3&&alrjdr<0.3","JEt2D");
  }
  if (doJEC==1) {
    dj->SetAlias("fscNr","(1.03-2.33e-4*nljet+6.5e3/pow(nljet,3))");
    dj->SetAlias("fscAw","(0.87+2.05e-4*nljet+5.74e3/pow(nljet,3))");
    g0 = eff(dj,"nljet*fscNr",evtSel,"nlrjdr<0.3","NrJEt");
    g1 = eff(dj,"aljet*fscAw",evtSelAw,"alrjdr<0.3","AwJEt");
    hEff2d = eff2d(dj,"aljet*fscAw:nljet*fscNr",evtSel,"nlrjdr<0.3&&alrjdr<0.3","JEt2D");
  }

  // Draw
  TCanvas *cJetEff=0;
  if (!doMultiCanvas) cJetEff = new TCanvas("cJetEff","cJetEff",500,550);
  g1->SetLineColor(kRed); g1->SetMarkerColor(kRed); g1->SetMarkerStyle(kOpenSquare);
  // Axis Styles
  TH1F *hTmp = new TH1F("hTmp","",nBin,bin);
  hTmp->SetAxisRange(0,1.4,"Y");
  hTmp->SetAxisRange(0,219,"X");
  hTmp->SetNdivisions(505);
  hTmp->GetXaxis()->SetLabelFont(63);
  hTmp->GetXaxis()->SetLabelSize(24);
  hTmp->GetYaxis()->SetLabelFont(63);
  hTmp->GetYaxis()->SetLabelSize(24);
  hTmp->GetXaxis()->SetTitleFont(63);
  hTmp->GetXaxis()->SetTitleSize(28);
  hTmp->GetYaxis()->SetTitleFont(63);
  hTmp->GetYaxis()->SetTitleSize(24);
  hTmp->GetXaxis()->SetTitleOffset(1.2);
  hTmp->GetYaxis()->SetTitleOffset(1.8);
  hTmp->GetXaxis()->CenterTitle();
  hTmp->GetYaxis()->CenterTitle();
  // Axis Titles
  if (doJEC==0) hTmp->SetXTitle("p_{T}^{GenJet} (GeV/c)");
  if (doJEC==1) hTmp->SetXTitle("p_{T}^{GenJet} (GeV/c)");
  hTmp->SetYTitle("Eff. (Matched Reco/Gen)");
  hTmp->Draw();
  g0->Draw("p");
  g1->Draw("p");
  TLine *l = new TLine(bin[0],1,bin[nBin],1);
  l->SetLineStyle(2);
  l->Draw();
  if (doLeg==1) {
    TLegend *t = new TLegend(0.31,0.80,0.72,0.93);
    t->SetHeader(header);
    t->SetBorderSize(0);
    t->SetFillStyle(0);
    t->SetTextSize(0.05);
    t->AddEntry(g0,"Leading Jet","pl");
    t->AddEntry(g1,"SubLeading Jet","pl");
    t->Draw();
  }

  if (doMultiCanvas) return dj;
  cJetEff->Print(outtag+"_Eff_vs_GenJEt.gif");
  cJetEff->Print(outtag+"_Eff_vs_GenJEt.C");
  cJetEff->Print(outtag+"_Eff_vs_GenJEt.pdf");

  // xcheck plots
  TCanvas * cDr = new TCanvas("cDr","cDr",500,500);
  dj->Draw("nlrjdr>>hDrNr(300,0,6.28)",evtSel);
  dj->Draw("alrjdr>>hDrAw(300,0,6.28)",evtSel,"same");

  TCanvas * cEt = new TCanvas("cEt","cEt",500,500);
  dj->Draw("nljet",evtSel,"hist");
  dj->Draw("nlrjet",evtSel,"hist E");
  dj->Draw("aljet",evtSel,"same hist");
  dj->Draw("alrjet",evtSel,"same E");

  TCanvas *cJet2DEff = new TCanvas("cJet2DEff","cJet2DEff",500,550);
  hEff2d->Draw("colz");
  cJet2DEff->SetRightMargin(0.2);
  cJet2DEff->Print(outtag+"_Eff2D_vs_GenJEt.gif");
  cJet2DEff->Print(outtag+"_Eff2D_vs_GenJEt.C");
  cJet2DEff->Print(outtag+"_Eff2D_vs_GenJEt.pdf");

  return dj;
}
