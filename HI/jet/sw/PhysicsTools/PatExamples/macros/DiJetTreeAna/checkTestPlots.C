#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TGraph.h>
#include "TString.h"
// stl
#include <vector>
//helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

char * drsg = "hist";
char * drdb = "hist same";
char * drsgE = "E1";
char * drdbE = "E1 same";

void checkTestPlots()
{
  // set inputs
  //char * indir = "/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/plots/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV";
  char * indir = "/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/testplots";
  vector<TString> gen;
  gen.push_back("pyquen_quendijet_nocoll_pt120to170_d20091024");
  //gen.push_back("pyquen_unquendijet_nocoll_pt120to170_d20091025");
  vector<TString> anatype;
  anatype.push_back("j2_1_t1");
  //anatype.push_back("j2_0_t1");
  //anatype.push_back("j1_1_t0");
  //anatype.push_back("j0_1_t0");
  char * tag = "sw47_2";

  printf("%s/FFHistos.root\n",indir);
  vector<TFile*> infiles;
  for (int igen=0; igen<gen.size(); ++igen) {
    for (int itype=0; itype<anatype.size(); ++itype) {
      infiles.push_back(new TFile(Form("%s/FFHistos.root",indir)));
      infiles.back()->Print();
    }
  }

  // outout
  char * plotdir = Form("%s/combinedPlots",indir);

  // ============ start to plot ===================
  // final styles
  Size_t mksz=0.5;
  Style_t mkstNear= kFullDotLarge;
  Style_t mkstAway= kCircle;
  Style_t lnst = 1;
  Color_t lcNear = kBlack;
  Color_t lcAway = kBlue;
  Width_t lwUncut = 1;
  Width_t lw=3;
  int plotmode=1;
  Int_t padxsize=400;
  Int_t padysize=400;

  // === Analysis checks ===
  // background subtraction
  TH1D * hXiNearJet; infiles[0]->GetObject("hXiNearJet",hXiNearJet);
  TH1D * hXiAwayJet; infiles[0]->GetObject("hXiAwayJet",hXiAwayJet);
  TH1D * hBgXiNearJet; infiles[0]->GetObject("hBgXiNearJet",hBgXiNearJet);
  TH1D * hBgXiAwayJet; infiles[0]->GetObject("hBgXiAwayJet",hBgXiAwayJet);
  TH1D * hSubXiNearJet = (TH1D*)hXiNearJet->Clone("hSubXiNearJet");
  hSubXiNearJet->SetMarkerColor(kRed);
  TH1D * hSubXiAwayJet = (TH1D*)hXiAwayJet->Clone("hSubXiAwayJet");
  hSubXiAwayJet->SetMarkerColor(kRed);
  // now subtract
  hSubXiNearJet->Add(hBgXiNearJet,-1);
  hSubXiAwayJet->Add(hBgXiAwayJet,-1);
  // plot
  TCanvas * cSubXiNear = new TCanvas("cSubXiNear","cSubXiNear",400,400);
  hXiNearJet->Draw();
  hBgXiNearJet->Draw("hist same");
  hSubXiNearJet->Draw("same");
  TCanvas * cSubXiAway = new TCanvas("cSubXiAway","cSubXiAway",400,400);
  hXiAwayJet->Draw();
  hBgXiAwayJet->Draw("hist same");
  hSubXiAwayJet->Draw("same");

  // A/N strong corr vs no corr
  // -- dphi --
  TCanvas * cANEtCorrJDPhi = new TCanvas("cANEtCorrJDPhi","cANEtCorrJDPhi",400,400);
  TH1D * hANEtCorrRefJDPhi = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrRefJDPhi",drsg,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TH1D * hANEtCorrDiagJDPhi = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrDiagJDPhi",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TH1D * hANEtCorrHoriJDPhi = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrHoriJDPhi",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TLegend *lANEtCorrJDPhi = new TLegend(0.2,0.6,0.5,0.80);
  lANEtCorrJDPhi->AddEntry(hANEtCorrRefJDPhi,"Uncut","L");
  lANEtCorrJDPhi->AddEntry(hANEtCorrDiagJDPhi,"Diag et correlation","L");
  lANEtCorrJDPhi->AddEntry(hANEtCorrHoriJDPhi,"Horiz et correlation","L");
  lANEtCorrJDPhi->SetTextSize(0.03);
  lANEtCorrJDPhi->Draw();
  // -- eta --
  TCanvas * cANEtCorrNJEta = new TCanvas("cANEtCorrNJEta","cANEtCorrNJEta",400,400);
  TH1D * hANEtCorrRefnljeta = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrRefnljeta",drsg,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TH1D * hANEtCorrDiagnljeta = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrDiagnljeta",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TH1D * hANEtCorrHorinljeta = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrHorinljeta",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TLegend *lANEtCorrNJEta = new TLegend(0.2,0.6,0.5,0.80);
  lANEtCorrNJEta->AddEntry(hANEtCorrRefnljeta,"Uncut","L");
  lANEtCorrNJEta->AddEntry(hANEtCorrDiagnljeta,"Diag et correlation","L");
  lANEtCorrNJEta->AddEntry(hANEtCorrHorinljeta,"Horiz et correlation","L");
  lANEtCorrNJEta->SetTextSize(0.03);
  lANEtCorrNJEta->Draw();

  TCanvas * cANEtCorrAJEta = new TCanvas("cANEtCorrAJEta","cANEtCorrAJEta",400,400);
  TH1D * hANEtCorrRefaljeta = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrRefaljeta",drsg,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TH1D * hANEtCorrDiagaljeta = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrDiagaljeta",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TH1D * hANEtCorrHorialjeta = dynamic_cast<TH1D*>(drawNormHist(infiles[0],"hANEtCorrHorialjeta",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  TLegend *lANEtCorrAJEta = new TLegend(0.2,0.6,0.5,0.80);
  lANEtCorrAJEta->AddEntry(hANEtCorrRefaljeta,"Uncut","L");
  lANEtCorrAJEta->AddEntry(hANEtCorrDiagaljeta,"Diag et correlation","L");
  lANEtCorrAJEta->AddEntry(hANEtCorrHorialjeta,"Horiz et correlation","L");
  lANEtCorrAJEta->SetTextSize(0.03);
  lANEtCorrAJEta->Draw();


  printAllCanvases(plotdir);
}
