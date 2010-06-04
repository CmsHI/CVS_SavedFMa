#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"

#include "CmsHi/DiJetAna/ana/selectionCut.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisGroup.h"

#include <map>
#include <cassert>
using namespace std;

void finalPlots(int doMC=1,
    TString inDir="plots/mcuq120to170_10k/Sel1_N120to170_A100_DPhi26/ff")
{
  TString outdir = inDir;
  selectionCut mcAna(doMC,1,120,170,100);
  TString inFileName=inDir+"/ffana_hists.root";
  TFile * inFile = new TFile(inFileName);
  inFile->ls();

  // Hitograms
  HisGroup hgMcGenTruthXiSig("hgMcGenTruthXiSig");
  hgMcGenTruthXiSig.Add(inFile,"hXiSig_Near_mcGenTruth");
  hgMcGenTruthXiSig.Add(inFile,"hXiSig_Away_mcGenTruth");
  hgMcGenTruthXiSig.Average();

  HisGroup hgMcGenXiSig("hgMcGenXiSig");
  hgMcGenXiSig.Add(inFile,"hXiSig_Near_mcGen");
  hgMcGenXiSig.Add(inFile,"hXiSig_Away_mcGen");
  hgMcGenXiSig.Average();

  HisGroup hgMcj2t0XiSig("hgMcj2t0XiSig");
  hgMcj2t0XiSig.Add(inFile,"hXiSig_Near_mcj2t0");
  hgMcj2t0XiSig.Add(inFile,"hXiSig_Away_mcj2t0");
  hgMcj2t0XiSig.Average();

  // Save output
  TString anaoutdir = outdir;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  TFile * outf = new TFile(Form("%s/final_hists.root",anaoutdir.Data()),"RECREATE");
  CPlot::sOutDir = anaoutdir;

  // === plot individuals ===
  TCanvas * cMcGenTruthXiSig = new TCanvas("cMcGenTruthXiSig","cMcGenTruthXiSig",500,500);
  CPlot cpMcGenTruthXiSig("McGenTruthXi","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpMcGenTruthXiSig.SetYRange(0,6.);
  cpMcGenTruthXiSig.AddHist1D(hgMcGenTruthXiSig.hm_["hXiSig_Near_mcGenTruth"],"Lead GenTruthJet","E",kRed,kOpenCircle);
  cpMcGenTruthXiSig.AddHist1D(hgMcGenTruthXiSig.hm_["hXiSig_Away_mcGenTruth"],"Away GenTruthJet","E",kBlue,kOpenCircle);
  cpMcGenTruthXiSig.AddHist1D(hgMcGenTruthXiSig.hAve_,"GenTruthJet1,2","E",kBlack,kFullCircle);
  cpMcGenTruthXiSig.SetLegendHeader("HI: genjet+genp");
  cpMcGenTruthXiSig.Draw(cMcGenTruthXiSig,true);

  TCanvas * cMcGenXiSig = new TCanvas("cMcGenXiSig","cMcGenXiSig",500,500);
  CPlot cpMcGenXiSig("McGenXi","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpMcGenXiSig.SetYRange(0,6.);
  cpMcGenXiSig.AddHist1D(hgMcGenXiSig.hm_["hXiSig_Near_mcGen"],"Lead GenJet","E",kRed,kOpenCircle);
  cpMcGenXiSig.AddHist1D(hgMcGenXiSig.hm_["hXiSig_Away_mcGen"],"Away GenJet","E",kBlue,kOpenCircle);
  cpMcGenXiSig.AddHist1D(hgMcGenXiSig.hAve_,"GenJet1,2","E",kBlack,kFullCircle);
  cpMcGenXiSig.SetLegendHeader("HI: genjet+genp");
  cpMcGenXiSig.Draw(cMcGenXiSig,true);

  TCanvas * cMcj2t0XiSig = new TCanvas("cMcj2t0XiSig","cMcj2t0XiSig",500,500);
  CPlot cpMcj2t0XiSig("Mcj2t0Xi","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpMcj2t0XiSig.SetYRange(0,6.);
  cpMcj2t0XiSig.AddHist1D(hgMcj2t0XiSig.hm_["hXiSig_Near_mcj2t0"],"Lead CaloJet","E",kRed,kOpenCircle);
  cpMcj2t0XiSig.AddHist1D(hgMcj2t0XiSig.hm_["hXiSig_Away_mcj2t0"],"Away CaloJet","E",kBlue,kOpenCircle);
  cpMcj2t0XiSig.AddHist1D(hgMcj2t0XiSig.hAve_,"CaloJet1,2","E",kBlack,kFullCircle);
  cpMcj2t0XiSig.SetLegendHeader("HI: calojet+genp");
  cpMcj2t0XiSig.Draw(cMcj2t0XiSig,true);

  // === plot combined ===
  TCanvas * cFFAll = new TCanvas("cFFAll","cFFAll",500,500);
  CPlot cpFFAll("FFAll","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpFFAll.SetYRange(0,6.);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcGenTruth","Signal: genjet+genp","histE",kRed,0);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcGen","HI: genjet+genp","E",kGreen+2,kOpenSquare);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcj2t0","HI: calojet+genp","E",kBlue,kOpenCircle);
  cpFFAll.Draw(cFFAll,true);

  TCanvas * cFFMatAll = new TCanvas("cFFMatAll","cFFMatAll",500,500);
  CPlot cpFFMatAll("FFMat","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpFFMatAll.SetYRange(0,6.);
  cpFFMatAll.AddHist1D(inFile,"hXiSig_Near_mcGenTruthMat","Signal: genjet(Matched)+genp","histE",kRed,0);
  cpFFMatAll.AddHist1D(inFile,"hXiSig_Near_mcGenMat","HI: genjet(matched)+genp","E",kGreen+2,kOpenSquare);
  cpFFMatAll.AddHist1D(inFile,"hXiSig_Near_mcj2t0Mat","HI: calojet(matched)+genp","E",kBlue,kOpenCircle);
  cpFFMatAll.Draw(cFFMatAll,true);
}
