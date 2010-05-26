#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"

#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots

#include <map>
#include <cassert>
using namespace std;

void finalPlots(const char * inFileName="plots/dijetAna_anaJet_Mc1_try26_10k/Sel1_Nr_80_120_Aw_70_DPhi_2.6/ffana_hists.root")
{
  TFile * inFile = new TFile(inFileName);
  CPlot::sOutDir = TString("plots/dijetAna_anaJet_Mc1_try26_10k/ana0525");
  inFile->ls();

  // plot
  TCanvas * cFFAll = new TCanvas("cFFAll","cFFAll",500,500);
  CPlot cpFFAll("FFAll","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpFFAll.SetYRange(0,6.);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcGenTruth","Signal: genjet+genp","histE",kRed,1,0);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcGen","HI: genjet+genp","E",kGreen+2,1,kOpenSquare);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcj2t0","HI: calojet+genp","E",kBlue,1,kOpenCircle);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcj2t0MatOrder","HI: calojet(MatGen)+genp","E",kBlue,1,kOpenStar);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcGenMatOrder","HI: genjet(MatCalo)+genp","E",kGreen+2,1,kOpenStar);
  cpFFAll.Draw(cFFAll,true,"gif");

  TCanvas * cFFMatAll = new TCanvas("cFFMatAll","cFFMatAll",500,500);
  CPlot cpFFMatAll("FFMatAll","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpFFMatAll.SetYRange(0,6.);
  cpFFMatAll.AddHist1D(inFile,"hXiSig_Near_mcGenTruthMat","Signal: genjet(Matched)+genp","histE",kRed,1,0);
  cpFFMatAll.AddHist1D(inFile,"hXiSig_Near_mcGenMat","HI: genjet(matched)+genp","E",kGreen+2,1,kOpenSquare);
  cpFFMatAll.AddHist1D(inFile,"hXiSig_Near_mcj2t0Mat","HI: calojet(matched)+genp","E",kBlue,1,kOpenCircle);
  cpFFMatAll.Draw(cFFMatAll,true,"gif");
}
