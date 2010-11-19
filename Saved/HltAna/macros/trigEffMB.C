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

TGraphAsymmErrors *getEfficiency(TTree *ntleadPhoton, char *cut,int mode = 0)
{

   TH1F *h1 = new TH1F("h1","",20,0,40);
   TH1F *h2 = new TH1F("h2","",20,0,40);

   if (mode==0) {
      // mode = 0 : gen
      ntleadPhoton->Draw("genEtaCutPt>>h1");
      ntleadPhoton->Draw("genEtaCutPt>>h2",cut);
   } else {
      // mode = 1 : reco
      ntleadPhoton->Draw("recoEtaCutPt>>h1");
      ntleadPhoton->Draw("recoEtaCutPt>>h2",cut);
   }

   h1->Sumw2();
   h2->Sumw2();

   TGraphAsymmErrors *result = calcEff(h1,h2);
   if (h1) delete h1;
   if (h2) delete h2;
   return result;
}


TGraphAsymmErrors *eff(TTree * t,TCut sel="",TCut cut="")
{
  t->SetAlias("nhits1","hiHFhit");
  cout << "Sel: " << TString(sel) << ": " << t->GetEntries(sel) << endl;
  cout << "Cut: " << TString(cut) << ": " << t->GetEntries(sel&&cut) << endl;
   
   const int nBin=22;
   Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300,400,500,2000,4000,8000,12000,20000,40000,80000,120000,160000};
   TH1F *h = new TH1F("h","",nBin,bin);
   TH1F *hCut = new TH1F("hCut","",nBin,bin);
   t->Draw("nhits1>>h",sel);
   t->Draw("nhits1>>hCut",sel&&cut);
   if (TString(cut)=="HLT_HIMinBiasPixel_SingleTrack") hCut->Scale(20);
   TGraphAsymmErrors *g = calcEff(h,hCut);
   //g->Draw("ap");
   if (h) delete h;
   if (hCut) delete hCut;
   return g;
}

void trigEffMB(char* infile="~/scratch02/data/HIAllPhysics/r151058/OpenHLT*.root")
{
   TChain * oh = new TChain("hltanalysis/HltTree");
   oh->Add(infile);

   // Pre-firing study:
   // * Make sure use an enabled bit, but with wide bx accept
   // * then probe a un-enabled bit
   TCut evtSel("L1_ZdcCaloPlus_ZdcCaloMinus_BptxAND_5bx&&recoVrtNtrk>0");

   TH1D * hBptx5Bx = new TH1D("hBptx5Bx","hBptx5Bx",31,0,31);
   TH1D * hBsc5Bx = new TH1D("hBsc5Bx","hBsc5Bx",31,0,31);
   TH1D * hHf5Bx = new TH1D("hHf5Bx","hHf5Bx",31,0,31);
   TH1D * hZdc5Bx = new TH1D("hZdc5Bx","hZdc5Bx",31,0,31);

   TCanvas *cBx = new TCanvas("cBx","cBx",500,500);
   //cBx->SetLogy();
   oh->Draw("L1Tech_BPTX_plus_AND_minus.v0_5bx>>hBptx5Bx",evtSel,"E");
   oh->Draw("L1_BscMinBiasInnerThreshold1_5bx>>hBsc5Bx",evtSel,"Esame");
   oh->Draw("L1_HcalHfCoincidencePm_5bx>>hHf5Bx",evtSel,"Esame");
   oh->Draw("L1_ZdcCaloPlus_ZdcCaloMinus_5bx>>hZdc5Bx",evtSel,"Esame");

   TCanvas *c = new TCanvas("c","",600,600);
   const int nBin=20;
   Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300,400,500,2000,4000,8000,12000,20000,40000,160000};
   TH1F *hTmp = new TH1F("hTmp","",nBin,bin);

   TGraphAsymmErrors *g = eff(oh,evtSel,"L1_HcalHfCoincidencePm_5bx");
   //TGraphAsymmErrors *g = eff(oh,evtSel,"HLT_HIMinBiasHfOrBSC");
   //TGraphAsymmErrors *g2 = eff(oh,evtSel,"L1Tech_BSC_HighMultiplicity.v0");
   TGraphAsymmErrors *g3 = eff(oh,evtSel,"L1_BscMinBiasThreshold1_5bx");
   TGraphAsymmErrors *g4 = eff(oh,evtSel,"L1_ZdcCaloPlus_ZdcCaloMinus_5bx");
   //TGraphAsymmErrors *g5 = eff(oh,evtSel,"HLT_HIMinBiasPixel_SingleTrack");
   //TGraphAsymmErrors *g5 = eff(oh,evtSel,"HLT_HIMinBiasZDCPixel_SingleTrack");

   //g2->SetLineColor(2);
   //g2->SetMarkerColor(2);
   //g2->SetMarkerStyle(kOpenStar);
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->SetMarkerStyle(4);
   g4->SetLineColor(kGreen+2);
   g4->SetMarkerColor(kGreen+2);
   g4->SetMarkerStyle(4);
   //g5->SetMarkerStyle(kOpenSquare);
   //g5->SetMarkerColor(kMagenta);
   hTmp->SetAxisRange(0,1.3,"Y");
   hTmp->SetXTitle("HF (GeV)");
   hTmp->SetYTitle("Firing rate");
   hTmp->Draw();
   TLine *l = new TLine(0,1,bin[nBin],1);
   l->SetLineStyle(2);
   //g5->Draw("p same");
   g->Draw("p same");
   g3->Draw("p same");
   g4->Draw("p same");
   //g2->Draw("p same");
   l->Draw();
   TLegend *t = new TLegend(0.34,0.78,0.94,0.91);
   t->SetBorderSize(0);
   t->SetFillStyle(0);
   t->AddEntry(g,"L1_HcalHfCoincidencePm_5bx","pl");
   //t->AddEntry(g2,"L1Tech_BSC_HighMultiplicity.v0","pl");
   t->AddEntry(g3,"L1_BscMinBiasThreshold1_5bx","pl");
   t->AddEntry(g4,"HLT_HIMinBiasZDC_Calo","pl");
   //t->AddEntry(g5,"HLT_HIMinBiasPixel_SingleTrack","pl");
   t->Draw();
}
