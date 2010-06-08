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
    //TString inDir="plots/mcuq120to170_10k/Sel1_N120to170_A100_DPhi26/ff")
    TString inDir="plots/mcuq120to170_10k/Sel1_N120to170_A80_DPhi26/ff")
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

  HisGroup hgMcGenSelRefXiSig("hgMcGenSelRefXiSig");
  hgMcGenSelRefXiSig.Add(inFile,"hXiSig_Near_mcGenSelRef");
  hgMcGenSelRefXiSig.Add(inFile,"hXiRaw_Away_mcGenSelRef");
  hgMcGenSelRefXiSig.Average();

  HisGroup hgMcj2t0XiSig("hgMcj2t0XiSig");
  hgMcj2t0XiSig.Add(inFile,"hXiSig_Near_mcj2t0");
  hgMcj2t0XiSig.Add(inFile,"hXiSig_Away_mcj2t0");
  hgMcj2t0XiSig.Average();

  HisGroup hgMcj2t0SelRefXiSig("hgMcj2t0SelRefXiSig");
  hgMcj2t0SelRefXiSig.Add(inFile,"hXiSig_Near_mcj2t0SelRef");
  hgMcj2t0SelRefXiSig.Add(inFile,"hXiSig_Away_mcj2t0SelRef");
  hgMcj2t0SelRefXiSig.Average();

  HisGroup hgMcGenMatXiSig("hgMcGenMatXiSig");
  hgMcGenMatXiSig.Add(inFile,"hXiSig_Near_mcGenMat");
  hgMcGenMatXiSig.Add(inFile,"hXiSig_Away_mcGenMat");
  hgMcGenMatXiSig.Average();

  HisGroup hgMcj2t0MatXiSig("hgMcj2t0MatXiSig");
  hgMcj2t0MatXiSig.Add(inFile,"hXiSig_Near_mcj2t0Mat");
  hgMcj2t0MatXiSig.Add(inFile,"hXiSig_Away_mcj2t0Mat");
  hgMcj2t0MatXiSig.Average();

  HisGroup hgMcRecoXiSig("hgMcRecoXiSig");
  hgMcRecoXiSig.Add(inFile,"hXiSig_Near_mcReco");
  hgMcRecoXiSig.Add(inFile,"hXiSig_Away_mcReco");
  hgMcRecoXiSig.Average();

  TH1D * hFrame = (TH1D*)hgMcj2t0MatXiSig.hAve_->Clone("hFrame");
  hFrame->Scale(0);

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

  TCanvas * cMcGenMatXiSig = new TCanvas("cMcGenMatXiSig","cMcGenMatXiSig",500,500);
  CPlot cpMcGenMatXiSig("McGenMatXi","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpMcGenMatXiSig.SetYRange(0,6.);
  cpMcGenMatXiSig.AddHist1D(hgMcGenMatXiSig.hm_["hXiSig_Near_mcGenMat"],"Lead GenMatJet","E",kRed,kOpenCircle);
  cpMcGenMatXiSig.AddHist1D(hgMcGenMatXiSig.hm_["hXiSig_Away_mcGenMat"],"Away GenMatJet","E",kBlue,kOpenCircle);
  cpMcGenMatXiSig.AddHist1D(hgMcGenMatXiSig.hAve_,"GenMatJet1,2","E",kBlack,kFullCircle);
  cpMcGenMatXiSig.SetLegendHeader("HI: genjet+genp");
  cpMcGenMatXiSig.Draw(cMcGenMatXiSig,true);

  TCanvas * cMcj2t0MatXiSig = new TCanvas("cMcj2t0MatXiSig","cMcj2t0MatXiSig",500,500);
  CPlot cpMcj2t0MatXiSig("Mcj2t0MatXi","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpMcj2t0MatXiSig.SetYRange(0,6.);
  cpMcj2t0MatXiSig.AddHist1D(hgMcj2t0MatXiSig.hm_["hXiSig_Near_mcj2t0Mat"],"Lead CaloJet","E",kRed,kOpenCircle);
  cpMcj2t0MatXiSig.AddHist1D(hgMcj2t0MatXiSig.hm_["hXiSig_Away_mcj2t0Mat"],"Away CaloJet","E",kBlue,kOpenCircle);
  cpMcj2t0MatXiSig.AddHist1D(hgMcj2t0MatXiSig.hAve_,"CaloJet1,2","E",kBlack,kFullCircle);
  cpMcj2t0MatXiSig.SetLegendHeader("HI: calojet+genp");
  cpMcj2t0MatXiSig.Draw(cMcj2t0MatXiSig,true);

  // === plot combined ===
  /*
  TCanvas * cAwFFAll = new TCanvas("cAwFFAll","cAwFFAll",500,500);
  CPlot cpAwFFAll("AwFFAll","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpAwFFAll.SetYRange(0,6.);
  cpAwFFAll.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpAwFFAll.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpAwFFAll.AddHist1D(hgMcGenTruthXiSig.hm_["hXiSig_Away_mcGenTruth"],"Signal: genjet1,2+genp","histE",kRed,0);
  cpAwFFAll.AddHist1D(hgMcGenXiSig.hm_["hXiSig_Away_mcGen"],"HI: genjet1,2+genp","E",kGreen+2,kOpenSquare);
  cpAwFFAll.AddHist1D(hgMcj2t0XiSig.hm_["hXiSig_Away_mcj2t0"],"HI: calojet1,2+genp","E",kBlue,kFullCircle);
  cpAwFFAll.Draw(cAwFFAll,true);
  */

  TCanvas * cFFMatAll = new TCanvas("cFFMatAll","cFFMatAll",500,500);
  CPlot cpFFMatAll("FFMat","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFFMatAll.SetYRange(0,6.);
  cpFFMatAll.AddHist1D(hgMcGenTruthXiSig.hAve_,"Signal: genjet(Matched)+genp","histE",kRed,0);
  cpFFMatAll.AddHist1D(hgMcGenMatXiSig.hAve_,"HI: genjet(matched)+genp","E",kGreen+2,kOpenSquare);
  cpFFMatAll.AddHist1D(hgMcj2t0MatXiSig.hAve_,"HI: calojet(matched)+genp","E",kBlue,kFullCircle);
  cpFFMatAll.Draw(cFFMatAll,true);

  TCanvas * cFFSelRefNrAw = new TCanvas("cFFSelRefNrAw","cFFSelRefNrAw",700,700);
  CPlot cpFFSelRefNrAw("FFSelRefNrAw","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFFSelRefNrAw.SetYRange(0,6.);
  cpFFSelRefNrAw.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpFFSelRefNrAw.AddHist1D(hFrame,"120GeV<p_{T}^{genjet1}<170GeV","",0,0);
  cpFFSelRefNrAw.AddHist1D(hgMcGenTruthXiSig.hm_["hXiSig_Near_mcGenTruth"],"Signal: genjet1 + gentrk","histE",kRed,0);
  cpFFSelRefNrAw.AddHist1D(hgMcGenTruthXiSig.hm_["hXiSig_Away_mcGenTruth"],"Signal: genjet2 + gentrk","histE",kBlue,0);
  cpFFSelRefNrAw.AddHist1D(hgMcj2t0SelRefXiSig.hm_["hXiSig_Near_mcj2t0SelRef"],"HI: calojet1 (Cut matched genjet) + gentrk","E",kRed,kFullCircle);
  cpFFSelRefNrAw.AddHist1D(hgMcj2t0SelRefXiSig.hm_["hXiSig_Away_mcj2t0SelRef"],"HI: calojet2 (Cut matched genjet) + gentrk","E",kBlue,kFullCircle);
  cpFFSelRefNrAw.SetLegend(0.194,0.64,0.52,0.94);
  cpFFSelRefNrAw.Draw(cFFSelRefNrAw,true);

  TCanvas * cFFSelRef = new TCanvas("cFFSelRef","cFFSelRef",700,700);
  CPlot cpFFSelRef("FFSelRef","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFFSelRef.SetYRange(0,6.);
  cpFFSelRef.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpFFSelRef.AddHist1D(hFrame,"120GeV<p_{T}^{genjet1}<170GeV","",0,0);
  cpFFSelRef.AddHist1D(hgMcGenTruthXiSig.hAve_,"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpFFSelRef.AddHist1D(hgMcj2t0SelRefXiSig.hAve_,"HI: calojet1,2 (Cut genjet JES) + gentrk","E",kGreen+2,kOpenCircle);
  cpFFSelRef.SetLegend(0.194,0.64,0.52,0.94);
  cpFFSelRef.Draw(cFFSelRef,true);

  TCanvas * cFFAll = new TCanvas("cFFAll","cFFAll",700,700);
  CPlot cpFFAll("FFAll","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFFAll.SetYRange(0,6.);
  cpFFAll.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpFFAll.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpFFAll.AddHist1D(hgMcGenTruthXiSig.hAve_,"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpFFAll.AddHist1D(hgMcGenXiSig.hAve_,"HI: genjet1,2 + gentrk","E",kRed+4,kOpenSquare);
  cpFFAll.AddHist1D(hgMcj2t0XiSig.hAve_,"HI: calojet1,2 + gentrk","E",kBlue,kFullCircle);
  cpFFAll.AddHist1D(hgMcj2t0SelRefXiSig.hAve_,"HI: calojet1,2 (Cut genjet JES) + gentrk","E",kGreen+2,kOpenCircle);
  cpFFAll.SetLegend(0.194,0.64,0.52,0.94);
  cpFFAll.Draw(cFFAll,true);

  TCanvas * cFFMeas = new TCanvas("cFFMeas","cFFMeas",700,700);
  CPlot cpFFMeas("FFMeas","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFFMeas.SetYRange(0,6.);
  cpFFMeas.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpFFMeas.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpFFMeas.AddHist1D(hgMcGenTruthXiSig.hAve_,"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpFFMeas.AddHist1D(hgMcj2t0XiSig.hAve_,"HI: calojet1,2 + gentrk","E",kBlue,kFullCircle);
  cpFFMeas.AddHist1D(hgMcRecoXiSig.hAve_,"HI: calojet1,2 + recotrk","E",kBlack,kFullCircle);
  cpFFMeas.SetLegend(0.194,0.64,0.52,0.94);
  cpFFMeas.Draw(cFFMeas,true);
}
