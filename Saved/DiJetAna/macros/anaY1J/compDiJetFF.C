#include <iostream>
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TMath.h"

#include "Saved/Utilities/macros/cplot/CPlot.h"           // helper class for plots
#include "Saved/Utilities/macros/histogram/HisTGroup.h"

using namespace std;

void compDiJetFF(
    TString inFile0Dir="out/JExHdMcV2/S1_100_200_50_25/a5/ff",
    TString inFile1Dir="out/JExHdMcV2/S1_100_200_50_25/a5j1t0/ff",
    TString AnaType0="fffCorr0",
    TString AnaType1="fffCorr1",
    TString header="July Data (Hard Triggered)")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Dir+"/"+AnaType0+".root" << endl;
  TFile * inFile0 = new TFile(inFile0Dir+"/"+AnaType0+".root");
  //inFile0->ls();
  cout << inFile1Dir+"/"+AnaType1+".root" << endl;
  TFile * inFile1 = new TFile(inFile1Dir+"/"+AnaType1+".root");

  // === Define Output ===
  TString outdir = inFile0Dir;
  outdir.ReplaceAll("ff","compare");
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);

  HisTGroup<TH1D> hg0("hg0");
  hg0.Add(inFile0,"schXi_Ave","XiAve");
  hg0.Add(inFile0,"schXi2_Ave","Xi2Ave");
  hg0.Add(inFile0,"schTrk_Ave","PPtAve");

  TH1D * hFrame = (TH1D*)hg0.GetH("XiAve")->Clone("hFrame");
  hFrame->Scale(0);

  // === Comparison histograms ===
  HisTGroup<TH1D> hg1("hg1");
  hg1.Add(inFile1,"schXi_Ave","XiAve");
  hg1.Add(inFile1,"schXi2_Ave","Xi2Ave");
  hg1.Add(inFile1,"schTrk_Ave","PPtAve");

  TCanvas * cXi = new TCanvas("Xi","Xi",500,500);
  CPlot cpXi("Xi","Xi","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi.SetXRange(0,6);
  cpXi.SetYRange(0.001,7);
  cpXi.AddHist1D(hg0.H("XiAve"),"reco corr FF - j1,j2","E",kBlack,kFullCircle);
  cpXi.AddHist1D(hg1.H("XiAve"),"gen FF - j1,j2","E",kRed,kOpenStar);
  cpXi.SetLegend(0.194,0.7,0.52,0.94);
  cpXi.Draw(cXi,false);

  TCanvas * cXi2 = new TCanvas("Xi2","Xi2",500,500);
  CPlot cpXi2("Xi2","Xi2","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi2.SetXRange(0,6);
  cpXi2.SetYRange(0.001,7);
  cpXi2.AddHist1D(hg0.H("Xi2Ave"),"reco corr FF - j1,j2","E",kBlack,kFullCircle);
  cpXi2.AddHist1D(hg1.H("Xi2Ave"),"gen FF - j1,j2","E",kRed,kOpenStar);
  cpXi2.SetLegend(0.194,0.7,0.52,0.94);
  cpXi2.Draw(cXi2,false);

  TCanvas * cPPt = new TCanvas("PPt","PPt",500,500);
  CPlot cpPPt("PPt","PPt","p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#p_{T}}");
  cpPPt.SetLogy();
  cpPPt.SetXRange(0,60);
  cpPPt.AddHist1D(hg0.H("PPtAve"),"reco corr FF - j1,j2","E",kBlack,kFullCircle);
  cpPPt.AddHist1D(hg1.H("PPtAve"),"gen FF - j1,j2","E",kRed,kOpenStar);
  cpPPt.Draw(cPPt,false);
}
