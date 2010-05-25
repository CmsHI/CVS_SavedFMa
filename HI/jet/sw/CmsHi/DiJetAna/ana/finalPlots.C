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

void finalPlots(const char * inFileName="plots/dijetAna_anaJet_Mc1_try26_2k/Sel1_Nr_80_120_Aw_70_DPhi_2.6/ffana_hists.root")
{
  TFile * inFile = new TFile(inFileName);
  CPlot::sOutDir = TString("plots/ana0525");
  inFile->ls();

  // plot
  TCanvas * cFFAll = new TCanvas("cFFAll","cFFAll",500,500);
  CPlot cpFFAll("FFAll","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpFFAll.SetYRange(0,6.);
  cpFFAll.AddHist1D(inFile,"hXiSig_Near_mcGen",kGreen+2);
  cpFFAll.Draw(cFFAll,true,"gif");
}
