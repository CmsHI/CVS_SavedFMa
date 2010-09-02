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

  HisTGroup<TH1D> hgCm("hgCm");
  hgCm.Add(inFile0,"schXi_Ave","XiAve");
  hgCm.Add(inFile0,"schXi2_Ave","Xi2Ave");
  hgCm.Add(inFile0,"schTrk_Ave","PPtAve");

  TH1D * hFrame = (TH1D*)hgCm.GetH("XiAve")->Clone("hFrame");
  hFrame->Scale(0);

  // === Comparison histograms ===
  hgCm.Add(inFile1,"schXi_Ave","Cm1XiAve");
  hgCm.Add(inFile1,"schXi2_Ave","Cm1Xi2Ave");
  hgCm.Add(inFile1,"schTrk_Ave","Cm1PPtAve");
  hgCm.Divide("XiAve","Cm1XiAve");
  hgCm.Divide("Xi2Ave","Cm1Xi2Ave");
  hgCm.Divide("PPtAve","Cm1PPtAve");

  TCanvas * cXi = new TCanvas("Xi","Xi",1000,500);
  cXi->Divide(2,1);
  CPlot cpXi("Xi","Xi","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi.SetXRange(0,6);
  cpXi.SetYRange(0.001,7);
  cpXi.AddHist1D(hgCm.H("XiAve"),"reco corr FF - j1,j2","E",kBlack,kFullCircle);
  cpXi.AddHist1D(hgCm.H("Cm1XiAve"),"gen FF - j1,j2","E",kRed,kOpenStar);
  cpXi.SetLegend(0.194,0.7,0.52,0.94);
  cXi->cd(1); cpXi.Draw((TPad*)cXi->GetPad(1),false);
  CPlot cpXiRat("XiRat","XiRat","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","reco/gen");
  cpXiRat.AddHist1D(hgCm.R("XiAveDivCm1XiAve"));
  cpXiRat.SetYRange(0,2);
  cXi->cd(2); cpXiRat.Draw((TPad*)cXi->GetPad(2),false);

  TCanvas * cXi2 = new TCanvas("Xi2","Xi2",1000,500);
  cXi2->Divide(2,1);
  CPlot cpXi2("Xi2","Xi2","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi2.SetXRange(0,6);
  cpXi2.SetYRange(0.001,7);
  cpXi2.AddHist1D(hgCm.H("Xi2Ave"),"reco corr FF - j1,j2","E",kBlack,kFullCircle);
  cpXi2.AddHist1D(hgCm.H("Cm1Xi2Ave"),"gen FF - j1,j2","E",kRed,kOpenStar);
  cpXi2.SetLegend(0.194,0.7,0.52,0.94);
  cXi2->cd(1); cpXi2.Draw((TPad*)cXi2->GetPad(1),false);
  CPlot cpXi2Rat("Xi2Rat","Xi2Rat","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","reco/gen");
  cpXi2Rat.AddHist1D(hgCm.R("Xi2AveDivCm1Xi2Ave"));
  cpXi2Rat.SetYRange(0,2);
  cXi2->cd(2); cpXi2Rat.Draw((TPad*)cXi2->GetPad(2),false);

  TCanvas * cPPt = new TCanvas("PPt","PPt",1000,500);
  cPPt->Divide(2,1);
  CPlot cpPPt("PPt","PPt","p_{T}^{trk}","#frac{1}{N_{jet}} #frac{dN}{dp_{T}}");
  cpPPt.SetLogy();
  cpPPt.SetXRange(0,60);
  cpPPt.AddHist1D(hgCm.H("PPtAve"),"reco corr FF - j1,j2","E",kBlack,kFullCircle);
  cpPPt.AddHist1D(hgCm.H("Cm1PPtAve"),"gen FF - j1,j2","E",kRed,kOpenStar);
  cPPt->cd(1); cpPPt.Draw((TPad*)cPPt->GetPad(1),false);
  CPlot cpPPtRat("PPtRat","PPtRat","p_{T}^{trk}","reco/gen");
  cpPPtRat.AddHist1D(hgCm.R("PPtAveDivCm1PPtAve"));
  cpPPtRat.SetYRange(0,2);
  cPPt->cd(2); cpPPtRat.Draw((TPad*)cPPt->GetPad(2),false);
}
