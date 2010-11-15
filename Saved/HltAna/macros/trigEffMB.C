#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TTree.h>
#include "TChain.h"
#include <TH1F.h>
#include <TCut.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCanvas.h>


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


TGraphAsymmErrors *eff(char *infname,int type = 1,TCut cut="")
{
   TFile *inf= new TFile(infname);
   TTree *t = (TTree*) inf->FindObjectAny("PixelTree");
   if (t==0) t = (TTree*) inf->FindObjectAny("HltTree");
   TCut sel; 
//   TCut cut;
   
   if (type==1) {
    sel = "vz[1]>-99";
    cut = "l1TBit[41]==1";
    t->SetAlias("nhits1","mult2");
   } else {
//    sel = "((nLumi>=29&&nLumi<=237)||(nLumi>=288&&nLumi<=1336)||(nLumi>=1376&&nLumi<=1786))&&l1TBit[0]==1&&abs(vz[1]-0.0700738)>0.000001&&abs(vz[1])>-99";
    sel = "L1Tech_BPTX_plus_AND_minus.v0_5bx&&recoVrtNtrk>0";
    //sel = "L1Tech_BPTX_plus_AND_minus.v0_5bx";
    //sel = "L1_BptxPlusANDMinus_5bx&&recoVrtNtrk>0";
    t->SetAlias("nhits1","hiHFhit");
   }
   
   const int nBin=22;
   Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300,400,500,2000,4000,8000,12000,20000,40000,80000,120000,160000};
   TH1F *h = new TH1F("h","",nBin,bin);
   TH1F *hCut = new TH1F("hCut","",nBin,bin);
   t->Draw("nhits1>>h",sel);
   t->Draw("nhits1>>hCut",sel&&cut);
   if (TString(cut)=="HLT_HIMinBiasPixel_SingleTrack") hCut->Scale(20);
   TGraphAsymmErrors *g = calcEff(h,hCut);
   //g->Draw("ap");
   return g;
}

void trigEffMB(char* infile="hltana_AllStream_r150887.root")
{
   TCanvas *cBptx = new TCanvas("cBptx","cBptx",600,600);
   cBptx->SetLogy();
   TChain * oh = new TChain("hltanalysis/HltTree");
   oh->Add(infile);
   oh->Draw("L1Tech_BPTX_plus_AND_minus.v0_5bx");

   TCanvas *c = new TCanvas("c","",600,600);
   const int nBin=20;
   Float_t bin[nBin+1]={0,5,10,15,30,60,80,100,120,160,200,300,400,500,2000,4000,8000,12000,20000,40000,160000};
   TH1F *hTmp = new TH1F("hTmp","",nBin,bin);

   //TGraphAsymmErrors *g = eff(infile,0,"L1_HcalHfCoincidencePm_BptxAND");
   TGraphAsymmErrors *g = eff(infile,0,"HLT_HIMinBiasHfOrBSC");
   TGraphAsymmErrors *g2 = eff(infile,0,"L1Tech_BSC_HighMultiplicity.v0");
   TGraphAsymmErrors *g3 = eff(infile,0,"L1_BscMinBiasThreshold1_BptxAND");
   TGraphAsymmErrors *g4 = eff(infile,0,"L1_ZdcCaloPlus_ZdcCaloMinus_BptxAND");
   //TGraphAsymmErrors *g5 = eff(infile,0,"HLT_HIMinBiasPixel_SingleTrack");
   TGraphAsymmErrors *g5 = eff(infile,0,"HLT_HIMinBiasZDCPixel_SingleTrack");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g2->SetMarkerStyle(kOpenStar);
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->SetMarkerStyle(4);
   g4->SetLineColor(kGreen+2);
   g4->SetMarkerColor(kGreen+2);
   g4->SetMarkerStyle(4);
   g5->SetMarkerStyle(kOpenSquare);
   g5->SetMarkerColor(kMagenta);
   hTmp->SetAxisRange(0,1.3,"Y");
   hTmp->SetXTitle("HF (GeV)");
   hTmp->SetYTitle("Firing rate");
   hTmp->Draw();
   TLine *l = new TLine(0,1,bin[nBin],1);
   l->SetLineStyle(2);
   g5->Draw("p same");
   g->Draw("p same");
   //g3->Draw("p same");
   //g4->Draw("p same");
   //g2->Draw("p same");
   l->Draw();
   TLegend *t = new TLegend(0.34,0.78,0.94,0.91);
   t->SetBorderSize(0);
   t->SetFillStyle(0);
   //t->AddEntry(g,"L1_HcalHfCoincidencePm_BptxAND","pl");
   t->AddEntry(g,"HLT_HIMinBiasHfOrBSC","pl");
   //t->AddEntry(g2,"L1Tech_BSC_HighMultiplicity.v0","pl");
   //t->AddEntry(g3,"L1_BscMinBiasThreshold1_BptxAND","pl");
   //t->AddEntry(g4,"L1_ZdcCaloPlus_ZdcCaloMinus_BptxAND","pl");
   t->AddEntry(g5,"HLT_HIMinBiasPixel_SingleTrack","pl");
   t->Draw();
}
