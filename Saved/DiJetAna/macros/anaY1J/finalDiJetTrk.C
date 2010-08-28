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

void finalDiJetTrk(int doMC=0,
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
  hg0.Add(inFile0,"hAnaTrkPt","TrkPt");
  hg0.Add(inFile0,"hAnaTrkNHits","TrkNHits");
  hg0.Add(inFile0,"hAnaJTrkDPhi","JTrkDPhi");
  hg0.Add(inFile0,"hAnaJTrkTight3DPhi","JTrkTight3DPhi");
  hg0.Add(inFile0,"hAnaJCTrkPt","JCTrkPt");
  hg0.Add(inFile0,"hRaw_AnaNrTrkPt","NrTrkPtRaw");
  hg0.Add(inFile0,"hBkg_AnaNrTrkPt","NrTrkPtBkg");
  hg0.Add(inFile0,"hSig_AnaNrTrkPt","NrTrkPtSig");
  hg0.Add(inFile0,"hRaw_AnaAwTrkPt","AwTrkPtRaw");
  hg0.Add(inFile0,"hBkg_AnaAwTrkPt","AwTrkPtBkg");
  hg0.Add(inFile0,"hSig_AnaAwTrkPt","AwTrkPtSig");

  // === Event Level ===
  TCanvas * cCompTrkPt = new TCanvas("cCompTrkPt","cCompTrkPt",500,500);
  CPlot cpCompTrkPt("CompTrkPt","CompTrkPt","p_{T}^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dp_{T}}");
  cpCompTrkPt.AddHist1D(hg0.H("TrkPt"),"hiSelectedTrk","E",kBlack,kFullCircle);
  cpCompTrkPt.AddHist1D(hg0.H("JCTrkPt"),"hiSelectedTrk (in jet cone)","E",kBlue,kOpenSquare);
  cpCompTrkPt.SetLegend(0.41,0.76,0.71,0.86);
  cpCompTrkPt.SetLegendHeader(header);
  cpCompTrkPt.SetLogy();
  cpCompTrkPt.Draw(cCompTrkPt,true);

  TCanvas * cCompTrkNHits = new TCanvas("cCompTrkNHits","cCompTrkNHits",500,500);
  CPlot cpCompTrkNHits("CompTrkNHits","CompTrkNHits","NHits^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dNHits}");
  cpCompTrkNHits.AddHist1D(hg0.H("TrkNHits"),"hiSelectedTrk","E",kBlack,kFullCircle);
  cpCompTrkNHits.SetLegend(0.26,0.76,0.56,0.86);
  cpCompTrkNHits.SetLegendHeader(header);
  cpCompTrkNHits.Draw(cCompTrkNHits,true);

  // === Jet Cone ===
  TCanvas * cCompNrTrkPt = new TCanvas("cCompNrTrkPt","cCompNrTrkPt",500,500);
  CPlot cpCompNrTrkPt("CompNrTrkPt","CompNrTrkPt","p_{T}^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dp_{T}}");
  cpCompNrTrkPt.AddHist1D(hg0.H("NrTrkPtRaw"),"Raw","E",kGreen+2,kOpenSquare);
  cpCompNrTrkPt.AddHist1D(hg0.H("NrTrkPtBkg"),"Bkg","E",kBlue,kOpenCircle);
  cpCompNrTrkPt.AddHist1D(hg0.H("NrTrkPtSig"),"Subtracted","E",kBlack,kFullCircle);
  cpCompNrTrkPt.SetLegend(0.23,0.81,0.53,0.90);
  cpCompNrTrkPt.SetLegendHeader(header);
  cpCompNrTrkPt.SetLogx();
  cpCompNrTrkPt.SetLogy();
  cpCompNrTrkPt.Draw(cCompNrTrkPt,true);

  TCanvas * cCompAwTrkPt = new TCanvas("cCompAwTrkPt","cCompAwTrkPt",500,500);
  CPlot cpCompAwTrkPt("CompAwTrkPt","CompAwTrkPt","p_{T}^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dp_{T}}");
  cpCompAwTrkPt.AddHist1D(hg0.H("AwTrkPtRaw"),"Raw","E",kGreen+2,kOpenSquare);
  cpCompAwTrkPt.AddHist1D(hg0.H("AwTrkPtBkg"),"Bkg","E",kBlue,kOpenCircle);
  cpCompAwTrkPt.AddHist1D(hg0.H("AwTrkPtSig"),"Subtracted","E",kBlack,kFullCircle);
  cpCompAwTrkPt.SetLegend(0.61,0.82,0.91,0.92);
  cpCompAwTrkPt.SetLegendHeader(header);
  cpCompAwTrkPt.SetLogx();
  cpCompAwTrkPt.SetLogy();
  cpCompAwTrkPt.Draw(cCompAwTrkPt,true);

  // === Jet Strip ===
  TCanvas * cCompJTrkDPhi = new TCanvas("cCompJTrkDPhi","cCompJTrkDPhi",500,500);
  CPlot cpCompJTrkDPhi("CompJTrkDPhi","CompJTrkDPhi","#Delta#phi(trk,j1)","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{d#Delta#phi} #cbar_{|#Delta#eta(trk,jet)|<0.5}");
  cpCompJTrkDPhi.SetYRange(0,100);
  cpCompJTrkDPhi.AddHist1D(hg0.H("JTrkDPhi"),"hiSelectedTrk (p_{T}>1.2GeV)","E",kBlack,kFullCircle);
  cpCompJTrkDPhi.AddHist1D(hg0.H("JTrkTight3DPhi"),"hiSelectedTrk (p_{T}>3GeV)","E",kBlue,kOpenCircle);
  cpCompJTrkDPhi.SetLegend(0.41,0.76,0.71,0.86);
  cpCompJTrkDPhi.SetLegendHeader(header);
  cpCompJTrkDPhi.Draw(cCompJTrkDPhi,true);
}
