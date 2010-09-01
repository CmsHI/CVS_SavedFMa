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

void finalDiJetFF(int doMC=0,
    TString inFile0Dir="out/JExHdMc/S0_100_200_50_25/a2",
    TString AnaType="djtrk",
    TString header="July Data (Hard Triggered)")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Dir << endl;
  TFile * inFile0 = new TFile(inFile0Dir+"/"+AnaType+".root");
  inFile0->ls();

  CPlot::sOutDir = inFile0Dir+"/"+AnaType;

  HisTGroup<TH1D> hg0("hg0");
  hg0.Add(inFile0,"hRaw_XiNr","NrXiRaw");
  hg0.Add(inFile0,"hBkg_XiNr","NrXiBkg");
  hg0.Add(inFile0,"hSig_XiNr","NrXiSig");
  hg0.Add(inFile0,"hRaw_XiAw","AwXiRaw");
  hg0.Add(inFile0,"hBkg_XiAw","AwXiBkg");
  hg0.Add(inFile0,"hSig_XiAw","AwXiSig");
  hg0.Add(inFile0,"hXi_Ave","XiAve");

  hg0.Add(inFile0,"hRaw_Xi2Aw","AwXi2Raw");
  hg0.Add(inFile0,"hBkg_Xi2Aw","AwXi2Bkg");
  hg0.Add(inFile0,"hSig_Xi2Aw","AwXi2Sig");
  hg0.Add(inFile0,"hXi2_Ave","Xi2Ave");

  hg0.Add(inFile0,"hRaw_TrkNr","NrPPtRaw");
  hg0.Add(inFile0,"hBkg_TrkNr","NrPPtBkg");
  hg0.Add(inFile0,"hSig_TrkNr","NrPPtSig");
  hg0.Add(inFile0,"hRaw_TrkAw","AwPPtRaw");
  hg0.Add(inFile0,"hBkg_TrkAw","AwPPtBkg");
  hg0.Add(inFile0,"hSig_TrkAw","AwPPtSig");
  hg0.Add(inFile0,"hTrk_Ave","PPtAve");

  TCanvas * cXi = new TCanvas("Xi","Xi",500,500);
  CPlot cpXi("Xi","Xi","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi.SetXRange(0,6);
  cpXi.SetYRange(0.001,7);
  cpXi.AddHist1D(hg0.H("NrXiSig"),"j1","E",kRed,kOpenSquare);
  cpXi.AddHist1D(hg0.H("AwXiSig"),"j2","E",kBlue,kOpenSquare);
  cpXi.AddHist1D(hg0.H("XiAve"),"j1,j2","E",kBlack,kFullCircle);
  cpXi.SetLegend(0.194,0.7,0.52,0.94);
  cpXi.Draw(cXi,false);

  TCanvas * cXi2 = new TCanvas("Xi2","Xi2",500,500);
  CPlot cpXi2("Xi2","Xi2","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi2.SetXRange(0,6);
  cpXi2.SetYRange(0.001,7);
  cpXi2.AddHist1D(hg0.H("NrXiSig"),"j1","E",kRed,kOpenSquare);
  cpXi2.AddHist1D(hg0.H("AwXi2Sig"),"j2","E",kBlue,kOpenSquare);
  cpXi2.AddHist1D(hg0.H("Xi2Ave"),"j1,j2","E",kBlack,kFullCircle);
  cpXi2.SetLegend(0.194,0.7,0.52,0.94);
  cpXi2.Draw(cXi2,false);

  TCanvas * cPPt = new TCanvas("PPt","PPt",500,500);
  CPlot cpPPt("PPt","PPt","p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#p_{T}}");
  cpPPt.SetLogy();
  cpPPt.SetXRange(0,60);
  cpPPt.AddHist1D(hg0.H("NrPPtSig"),"j1","E",kRed,kOpenSquare);
  cpPPt.AddHist1D(hg0.H("AwPPtSig"),"j2","E",kBlue,kOpenSquare);
  cpPPt.AddHist1D(hg0.H("PPtAve"),"j1,j2","E",kBlack,kFullCircle);
  cpPPt.Draw(cPPt,false);
}
