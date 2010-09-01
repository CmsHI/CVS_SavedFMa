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
    TString inFile0Dir="out/JExHdMcV2/S1_100_200_50_25/a5",
    TString AnaType="ff",
    TString header="July Data (Hard Triggered)")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Dir << endl;
  TFile * inFile0 = new TFile(inFile0Dir+"/"+AnaType+".root");
  inFile0->ls();

  TString outdir = inFile0Dir+"/"+AnaType;
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);
  TFile * outf = new TFile(Form("%s/fffCorr%d.root",outdir.Data(),doCorr),"RECREATE");

  Double_t trkEff=0.65,corr=1.;
  if (doCorr==1) corr/=trkEff;
  HisTGroup<TH1D> hg0("hg0");
  hg0.Add(inFile0,"hRaw_XiNr","NrXiRaw",corr);
  hg0.Add(inFile0,"hBkg_XiNr","NrXiBkg",corr);
  hg0.Add(inFile0,"hSig_XiNr","NrXiSig",corr);
  hg0.Add(inFile0,"hRaw_XiAw","AwXiRaw",corr);
  hg0.Add(inFile0,"hBkg_XiAw","AwXiBkg",corr);
  hg0.Add(inFile0,"hSig_XiAw","AwXiSig",corr);
  hg0.Add(inFile0,"hXi_Ave","XiAve",corr);

  hg0.Add(inFile0,"hRaw_Xi2Aw","AwXi2Raw",corr);
  hg0.Add(inFile0,"hBkg_Xi2Aw","AwXi2Bkg",corr);
  hg0.Add(inFile0,"hSig_Xi2Aw","AwXi2Sig",corr);
  hg0.Add(inFile0,"hXi2_Ave","Xi2Ave",corr);

  hg0.Add(inFile0,"hRaw_TrkNr","NrPPtRaw",corr);
  hg0.Add(inFile0,"hBkg_TrkNr","NrPPtBkg",corr);
  hg0.Add(inFile0,"hSig_TrkNr","NrPPtSig",corr);
  hg0.Add(inFile0,"hRaw_TrkAw","AwPPtRaw",corr);
  hg0.Add(inFile0,"hBkg_TrkAw","AwPPtBkg",corr);
  hg0.Add(inFile0,"hSig_TrkAw","AwPPtSig",corr);
  hg0.Add(inFile0,"hTrk_Ave","PPtAve",corr);

  TH1D * hFrame = (TH1D*)hg0.GetH("NrXiSig")->Clone("hFrame");
  hFrame->Scale(0);

  TCanvas * cXi = new TCanvas("Xi","Xi",500,500);
  CPlot cpXi("Xi","Xi","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi.SetXRange(0,6);
  cpXi.SetYRange(0.001,7);
  cpXi.AddHist1D(hg0.H("NrXiSig"),"j1","E",kRed,kOpenSquare);
  cpXi.AddHist1D(hg0.H("AwXiSig"),"j2","E",kBlue,kOpenSquare);
  cpXi.AddHist1D(hg0.H("XiAve"),"j1,j2","E",kBlack,kFullCircle);
  cpXi.SetLegend(0.194,0.7,0.52,0.94);
  cpXi.Draw(cXi,true);

  TCanvas * cXi2 = new TCanvas("Xi2","Xi2",500,500);
  CPlot cpXi2("Xi2","Xi2","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpXi2.SetXRange(0,6);
  cpXi2.SetYRange(0.001,7);
  cpXi2.AddHist1D(hg0.H("NrXiSig"),"j1","E",kRed,kOpenSquare);
  cpXi2.AddHist1D(hg0.H("AwXi2Sig"),"j2","E",kBlue,kOpenSquare);
  cpXi2.AddHist1D(hg0.H("Xi2Ave"),"j1,j2","E",kBlack,kFullCircle);
  cpXi2.SetLegend(0.194,0.7,0.52,0.94);
  cpXi2.Draw(cXi2,true);

  TCanvas * cPPt = new TCanvas("PPt","PPt",500,500);
  CPlot cpPPt("PPt","PPt","p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#p_{T}}");
  cpPPt.SetLogy();
  cpPPt.SetXRange(0,60);
  cpPPt.AddHist1D(hg0.H("NrPPtSig"),"j1","E",kRed,kOpenSquare);
  cpPPt.AddHist1D(hg0.H("AwPPtSig"),"j2","E",kBlue,kOpenSquare);
  cpPPt.AddHist1D(hg0.H("PPtAve"),"j1,j2","E",kBlack,kFullCircle);
  cpPPt.Draw(cPPt,true);

  // check on bg sub
  TCanvas * cNrSub = new TCanvas("cNrSub","cNrSub",500,500);
  CPlot cpNrSub("NrSub","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi}");
  cpNrSub.SetXRange(0,6);
  cpNrSub.SetYRange(0.001,1000);
  cpNrSub.AddHist1D(hFrame,header,"",0,0);
  cpNrSub.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  //cpNrSub.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<170GeV, 50GeV<p_{T}^{jet2}","",0,0);
  cpNrSub.AddHist1D(hg0.H("NrXiRaw"),"Near (Raw)","E",kGreen+2,kOpenSquare);
  cpNrSub.AddHist1D(hg0.H("NrXiBkg"),"Near (Bkg)","E",kBlue,kOpenCircle);
  cpNrSub.AddHist1D(hg0.H("NrXiSig"),"Near (Sig)","E",kBlack,kFullCircle);
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
  cpAwSub.AddHist1D(hg0.H("AwXiRaw"),"Away (Raw)","E",kGreen+2,kOpenSquare);
  cpAwSub.AddHist1D(hg0.H("AwXiBkg"),"Away (Bkg)","E",kBlue,kOpenCircle);
  cpAwSub.AddHist1D(hg0.H("AwXiSig"),"Away (Sig)","E",kBlack,kFullCircle);
  cpAwSub.SetLegend(0.194,0.7,0.52,0.94);
  cpAwSub.SetLogy();
  cpAwSub.Draw(cAwSub,true);

  // All done, save and exit
  outf->Write();
}
