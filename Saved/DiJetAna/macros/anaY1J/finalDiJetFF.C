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

void finalDiJetFF(int doCorr=0,
    TString inFile0Dir="out/HCPR-GoodTrk1123_All0/S2_120_500_50_26/1125a1r1",
    TString header="HLT_HIJet50U",
    TString AnaType="ffd2")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Dir << endl;
  TFile * inFile0 = new TFile(inFile0Dir+"/"+AnaType+".root");
  //inFile0->ls();

  TString outdir = inFile0Dir+"/"+AnaType;
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);
  TFile * outf = new TFile(Form("%s/fffCorr%d.root",outdir.Data(),doCorr),"RECREATE");

  Double_t trkEff=0.65,corr=1.;
  if (doCorr==1) corr/=trkEff;
  HisTGroup<TH1D> hg0("hg0");
  hg0.Add(inFile0,"hRaw_XiNr","XiNrRaw",corr);
  hg0.Add(inFile0,"hBkg_XiNr","XiNrBkg",corr);
  hg0.Add(inFile0,"hSig_XiNr","XiNrSig",corr);
  hg0.Add(inFile0,"hRaw_XiAw","XiAwRaw",corr);
  hg0.Add(inFile0,"hBkg_XiAw","XiAwBkg",corr);
  hg0.Add(inFile0,"hSig_XiAw","XiAwSig",corr);
  hg0.Add(inFile0,"hXi_Ave","XiAve",corr);

  hg0.Add(inFile0,"hRaw_XiE1Aw","XiE1AwRaw",corr);
  hg0.Add(inFile0,"hBkg_XiE1Aw","XiE1AwBkg",corr);
  hg0.Add(inFile0,"hSig_XiE1Aw","XiE1AwSig",corr);
  hg0.Add(inFile0,"hXiE1_Ave","XiE1Ave",corr);

  hg0.Add(inFile0,"hRaw_PPtNr","PPtNrRaw",corr);
  hg0.Add(inFile0,"hBkg_PPtNr","PPtNrBkg",corr);
  hg0.Add(inFile0,"hSig_PPtNr","PPtNrSig",corr);
  hg0.Add(inFile0,"hRaw_PPtAw","PPtAwRaw",corr);
  hg0.Add(inFile0,"hBkg_PPtAw","PPtAwBkg",corr);
  hg0.Add(inFile0,"hSig_PPtAw","PPtAwSig",corr);
  hg0.Add(inFile0,"hPPt_Ave","PPtAve",corr);

  TH1D * hFrame = (TH1D*)hg0.GetH("XiNrSig")->Clone("hFrame");
  hFrame->Scale(0);

  TCanvas * cXi = new TCanvas("Xi","Xi",500,500);
  CPlot cpXi("Xi","Xi","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi.SetXRange(0,6);
  cpXi.SetYRange(0.001,7);
  cpXi.AddHist1D(hg0.H("XiNrSig"),"j1","E",kRed,kOpenSquare);
  cpXi.AddHist1D(hg0.H("XiAwSig"),"j2","E",kBlack,kFullCircle);
  //cpXi.AddHist1D(hg0.H("XiAve"),"j1,j2","E",kBlack,kOpenCircle);
  cpXi.SetLegend(0.194,0.7,0.52,0.94);
  cpXi.Draw(cXi,true);

  TCanvas * cXiE1 = new TCanvas("XiE1","XiE1",500,500);
  CPlot cpXiE1("XiE1","XiE1","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXiE1.SetXRange(0,6);
  cpXiE1.SetYRange(0.001,7);
  cpXiE1.AddHist1D(hg0.H("XiNrSig"),"j1","E",kRed,kOpenSquare);
  cpXiE1.AddHist1D(hg0.H("XiE1AwSig"),"j2","E",kBlack,kFullCircle);
  //cpXiE1.AddHist1D(hg0.H("XiE1Ave"),"j1,j2","E",kBlack,kOpenCircle);
  cpXiE1.SetLegend(0.194,0.7,0.52,0.94);
  cpXiE1.Draw(cXiE1,true);

  TCanvas * cPPt = new TCanvas("PPt","PPt",500,500);
  CPlot cpPPt("PPt","PPt","p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#p_{T}}");
  cpPPt.SetLogy();
  cpPPt.SetXRange(0,60);
  cpPPt.AddHist1D(hg0.H("PPtNrSig"),"j1","E",kRed,kOpenSquare);
  cpPPt.AddHist1D(hg0.H("PPtAwSig"),"j2","E",kBlack,kFullCircle);
  //cpPPt.AddHist1D(hg0.H("PPtAve"),"j1,j2","E",kBlack,kOpenCircle);
  cpPPt.Draw(cPPt,true);

  // check on bg sub
  TCanvas * cNrSub = new TCanvas("cNrSub","cNrSub",500,500);
  CPlot cpNrSub("NrSub","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpNrSub.SetXRange(0,6);
  cpNrSub.SetYRange(0.001,1000);
  cpNrSub.AddHist1D(hFrame,header,"",0,0);
  cpNrSub.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  //cpNrSub.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<170GeV, 50GeV<p_{T}^{jet2}","",0,0);
  cpNrSub.AddHist1D(hg0.H("XiNrRaw"),"Near (Raw)","E",kGreen+2,kOpenSquare);
  cpNrSub.AddHist1D(hg0.H("XiNrBkg"),"Near (Bkg)","E",kBlue,kOpenCircle);
  cpNrSub.AddHist1D(hg0.H("XiNrSig"),"Near (Sig)","E",kBlack,kFullCircle);
  cpNrSub.SetLegend(0.194,0.7,0.52,0.94);
  cpNrSub.SetLogy();
  cpNrSub.Draw(cNrSub,true);

  TCanvas * cAwSub = new TCanvas("cAwSub","cAwSub",500,500);
  CPlot cpAwSub("AwSub","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpAwSub.SetXRange(0,6);
  cpAwSub.SetYRange(0.001,1000);
  cpAwSub.AddHist1D(hFrame,header,"",0,0);
  cpAwSub.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  //cpAwSub.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<170GeV, 50GeV<p_{T}^{jet2}","",0,0);
  cpAwSub.AddHist1D(hg0.H("XiAwRaw"),"Away (Raw)","E",kGreen+2,kOpenSquare);
  cpAwSub.AddHist1D(hg0.H("XiAwBkg"),"Away (Bkg)","E",kBlue,kOpenCircle);
  cpAwSub.AddHist1D(hg0.H("XiAwSig"),"Away (Sig)","E",kBlack,kFullCircle);
  cpAwSub.SetLegend(0.194,0.7,0.52,0.94);
  cpAwSub.SetLogy();
  cpAwSub.Draw(cAwSub,true);

  TCanvas * cE1AwSub = new TCanvas("cE1AwSub","cE1AwSub",500,500);
  CPlot cpE1AwSub("E1AwSub","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpE1AwSub.SetXRange(0,6);
  cpE1AwSub.SetYRange(0.001,1000);
  cpE1AwSub.AddHist1D(hFrame,header,"",0,0);
  cpE1AwSub.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  //cpE1AwSub.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<170GeV, 50GeV<p_{T}^{jet2}","",0,0);
  cpE1AwSub.AddHist1D(hg0.H("XiE1AwRaw"),"E1Away (Raw)","E",kGreen+2,kOpenSquare);
  cpE1AwSub.AddHist1D(hg0.H("XiE1AwBkg"),"E1Away (Bkg)","E",kBlue,kOpenCircle);
  cpE1AwSub.AddHist1D(hg0.H("XiE1AwSig"),"E1Away (Sig)","E",kBlack,kFullCircle);
  cpE1AwSub.SetLegend(0.194,0.7,0.52,0.94);
  cpE1AwSub.SetLogy();
  cpE1AwSub.Draw(cE1AwSub,true);

  // All done, save and exit
  outf->Write();
}
