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

#include "../../commonUtility.h"
#include "../../tgraphTools.C"

using namespace std;

const int nBin=23;
Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200};

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

  TH1F *h = new TH1F("h"+tag,"",nBin,bin);
  TH1F *hCut = new TH1F("hCut"+tag,"",nBin,bin);
  t->Draw(var+">>h"+tag,sel,"goff");
  t->Draw(var+">>hCut"+tag,sel&&cut,"goff");
  TGraphAsymmErrors *g = calcEff(h,hCut);
  clearXErrorBar(g);
  //g->Draw("ap");
  return g;
}

TTree * trigEff(
    TString header="HLT_HIJet35U",
    TString var="jtpt[0]",
    TString Tag="Corr"
    )
{
  TString infile="../ntout/tranav7_datamattmbv2_akpu3pf_hiGood.root";
  TFile * inf = new TFile(infile);
  TTree * dj = (TTree*)inf->Get("tjf");
  dj->AddFriend("jra = akPu3PFJetAnalyzer/t","/net/hisrv0001/home/mnguyen/scratch/InclusiveJetAnalyzer/310X/MinBiasHfOrBSC-PromptReco-Runs_151077-151211_PAT-v2/merged_inclusiveJetAnalyzer_v1.root");

  cout << "Total: " << dj->GetEntries() << endl;

  TCut evtSel("hltBit[0]&&bin<40&&abs(jteta[0])<2");

  TGraphAsymmErrors *g0 = eff(dj,var,evtSel,"jra.hltBit[1]","akpu3pf");

  // Draw
  TCanvas *cTrigEff = new TCanvas("cTrigEff","cTrigEff",500,550);
  TH1F *hTmp = new TH1F("hTmp","",nBin,bin);
  hTmp->SetAxisRange(0,1.3,"Y");
  hTmp->SetXTitle("Leading Jet p_{T}^{"+Tag+"} (GeV/c)");
  hTmp->SetYTitle("Eff. (Trigger/MB)");
  handsomeTH1(hTmp);
  hTmp->Draw();
  g0->Draw("p");

  TLine *l = new TLine(0,1,bin[nBin],1);
  l->SetLineStyle(2);
  l->Draw();

  TLegend *t = new TLegend(0.23,0.78,0.83,0.91);
  t->SetHeader(header);
  t->SetBorderSize(0);
  t->SetTextSize(0.035);
  t->SetFillStyle(0);
  t->Draw();
  //addCms(cTrigEff);
  drawText("Anti-k_{T} (R=0.3)",0.43,0.30);
  drawText("|#eta|<2",0.45,0.25);
  cTrigEff->Print("TrigEff_"+header+".gif");
  cTrigEff->Print("TrigEff_"+header+".pdf");

  return dj;
}
