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
    TString inFile0Name="out/HydUQDJ80/S1_100_200_0_25/a9/ffs01.root",
    TString inFile1Name="out/HydUQDJ80/S1_100_200_0_25/a9/ffs00.root",
    TString compTag="ffs01ffs00",
    TString outdir="out/HydUQDJ80/compare/a9",
    TString JEtNr0="hJEtNr",
    TString JEtNr1="hJEtNr",
    TString header="Hydjet+UQDiJet80to120",
    TString finalMeas="hSig_XiE1Aw",
    TString finalMeasTitle="j2(E1)")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TFile * inFile0 = new TFile(inFile0Name);
  //inFile0->ls();
  cout << inFile1Name << endl;
  TFile * inFile1 = new TFile(inFile1Name);

  // === Define Output ===
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);

  TString JEtAw0(JEtNr0); JEtAw0.ReplaceAll("Nr","Aw");
  HisTGroup<TH1D> hgCm("hgCm");
  hgCm.Add(inFile0,"hSig_XiNr","XiNr");
  hgCm.Add(inFile0,"hSig_XiAw","XiAw");
  hgCm.Add(inFile0,finalMeas,"XiE1Aw");
  hgCm.Add(inFile0,"hSig_PPtNr","PPtNr");
  hgCm.Add(inFile0,"hSig_PPtAw","PPtAw");
  hgCm.Add(inFile0,JEtNr0,"JEtNr");
  hgCm.Add(inFile0,JEtAw0,"JEtAw");
  hgCm.Add(inFile0,"hC5NPSubNr","C5NPSubNr");
  hgCm.Add(inFile0,"hC5NPSubAw","C5NPSubAw");
  cout << "Got Ana Histograms" << endl;

  TH1D * hFrame = (TH1D*)hgCm.GetH("XiE1Aw")->Clone("hFrame");
  hFrame->Scale(0);

  // === Comparison histograms ===
  TString JEtAw1(JEtNr1); JEtAw1.ReplaceAll("Nr","Aw");
  hgCm.Add(inFile1,"hSig_XiNr","Cm1XiNr");
  hgCm.Add(inFile1,"hSig_XiAw","Cm1XiAw");
  hgCm.Add(inFile1,finalMeas,"Cm1XiE1Aw");
  hgCm.Add(inFile1,"hSig_PPtNr","Cm1PPtNr");
  hgCm.Add(inFile1,"hSig_PPtAw","Cm1PPtAw");
  hgCm.Add(inFile1,JEtNr1,"Cm1JEtNr");
  hgCm.Add(inFile1,JEtAw1,"Cm1JEtAw");
  hgCm.Add(inFile1,"hC5NPSubNr","Cm1C5NPSubNr");
  hgCm.Add(inFile1,"hC5NPSubAw","Cm1C5NPSubAw");
  cout << "Got Comparison Histograms" << endl;

  hgCm.Divide("XiNr","Cm1XiNr");
  hgCm.Divide("XiAw","Cm1XiAw");
  hgCm.Divide("XiE1Aw","Cm1XiE1Aw");
  hgCm.Add("XiE1Aw","Cm1XiE1Aw",1,-1);
  hgCm.Divide("PPtNr","Cm1PPtNr");
  hgCm.Divide("PPtAw","Cm1PPtAw");
  hgCm.Divide("JEtNr","Cm1JEtNr");
  hgCm.Divide("JEtAw","Cm1JEtAw");
  hgCm.Add("C5NPSubNr","Cm1C5NPSubNr",1,-1);
  hgCm.Add("C5NPSubAw","Cm1C5NPSubAw",1,-1);
  cout << "Finished Comparing Histograms" << endl;

  TCanvas * cXiNr = new TCanvas("XiNr","XiNr",1000,500);
  cXiNr->Divide(2,1);
  CPlot cpXiNr("XiNr","XiNr","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (bg subtracted)");
  cpXiNr.SetXRange(0,6);
  cpXiNr.SetYRange(0.001,7);
  cpXiNr.AddHist1D(hgCm.H("Cm1XiNr"),"Ref - j1","hist",kOrange+2,kOpenCircle);
  cpXiNr.AddHist1D(hgCm.H("XiNr"),"Ana - j1","E",kBlue,kOpenSquare);
  cpXiNr.AddHist1D(hgCm.H("Cm1XiE1Aw"),"Ref - "+finalMeasTitle,"hist",kRed,kOpenStar);
  cpXiNr.AddHist1D(hgCm.H("XiE1Aw"),"Ana - "+finalMeasTitle,"E",kBlack,kFullCircle);
  cpXiNr.SetLegend(0.194,0.7,0.52,0.94);
  cpXiNr.SetLegendHeader(header);
  cXiNr->cd(1); cpXiNr.Draw((TPad*)cXiNr->GetPad(1),false);
  CPlot cpXiNrRat("XiNrRat","XiNrRat","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","Ana/Ref");
  cpXiNrRat.AddHist1D(hgCm.R("XiNrDivCm1XiNr"),"",kBlue,kOpenSquare);
  cpXiNrRat.AddHist1D(hgCm.R("XiE1AwDivCm1XiE1Aw"),"",kBlack);
  cpXiNrRat.SetYRange(-0.5,2.5);
  cXiNr->cd(2); cpXiNrRat.Draw((TPad*)cXiNr->GetPad(2),false);
  cXiNr->Print(outdir+"/XiNr"+compTag+".gif");

  TCanvas * cXiAw = new TCanvas("XiAw","XiAw",1000,500);
  cXiAw->Divide(2,1);
  CPlot cpXiAw("XiAw","XiAw","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (bg subtracted)");
  cpXiAw.SetXRange(0,6);
  cpXiAw.SetYRange(0.001,7);
  //cpXiAw.AddHist1D(hgCm.H("Cm1XiAw"),"Ref - j2E2","E",kOrange+2,kOpenDiamond);
  //cpXiAw.AddHist1D(hgCm.H("XiAw"),"Ana - j2E2","E",kGreen+2,kOpenTriangleUp);
  cpXiAw.AddHist1D(hgCm.H("Cm1XiE1Aw"),"Ref - "+finalMeasTitle,"hist",kRed,kOpenStar);
  cpXiAw.AddHist1D(hgCm.H("XiE1Aw"),"Ana - "+finalMeasTitle,"E",kBlack,kFullCircle);
  cpXiAw.SetLegend(0.194,0.7,0.52,0.94);
  cpXiAw.SetLegendHeader(header);
  cXiAw->cd(1); cpXiAw.Draw((TPad*)cXiAw->GetPad(1),false);
  CPlot cpXiAwRat("XiAwRat","XiAwRat","#xi=ln(E_{T}^{Jet1}/p_{T}^{trk})","Ana/Ref");
  //cpXiAwRat.AddHist1D(hgCm.R("XiAwDivCm1XiAw"),"j2E2^{reco}/j2E2^{Ref}",kGreen+2,kOpenTriangleUp);
  cpXiAwRat.AddHist1D(hgCm.R("XiE1AwDivCm1XiE1Aw"),"j2(E1)^{reco}/j2(E1)^{Ref}",kBlack);
  //cpXiAwRat.AddHist1D(hgCm.R("XiE1AwSubCm1XiE1Aw"),"j2(E1)^{reco}/j2(E1)^{Ref}",kBlack);
  cpXiAwRat.SetYRange(-0.5,2.5);
  cXiAw->cd(2); cpXiAwRat.Draw((TPad*)cXiAw->GetPad(2),false);
  cXiAw->Print(outdir+"/XiAw"+compTag+".gif");

  TCanvas * cPPt = new TCanvas("PPt","PPt",1000,500);
  cPPt->Divide(2,1);
  CPlot cpPPt("PPt","PPt","p_{T}^{trk} in jet cone","#frac{1}{N_{jet}} #frac{dN}{dp_{T}} (bg subtracted)");
  cpPPt.SetLogy();
  cpPPt.SetXRange(0,60);
  cpPPt.AddHist1D(hgCm.H("Cm1PPtNr"),"Ref - j1","hist",kOrange+2,kOpenDiamond);
  cpPPt.AddHist1D(hgCm.H("PPtNr"),"Ana - j1","E",kBlue,kOpenSquare);
  cpPPt.AddHist1D(hgCm.H("Cm1PPtAw"),"Ref - j2","hist",kRed,kOpenStar);
  cpPPt.AddHist1D(hgCm.H("PPtAw"),"Ana - j2","E",kBlack,kFullCircle);
  cpPPt.SetLegendHeader(header);
  cPPt->cd(1); cpPPt.Draw((TPad*)cPPt->GetPad(1),false);
  CPlot cpPPtRat("PPtRat","PPtRat","p_{T}^{trk}","Ana/Ref");
  cpPPtRat.AddHist1D(hgCm.R("PPtNrDivCm1PPtNr"),"j1",kBlue,kOpenSquare);
  cpPPtRat.AddHist1D(hgCm.R("PPtAwDivCm1PPtAw"),"j2");
  cpPPtRat.SetYRange(-0.5,2.5);
  //cpPPtRat.Rebin(2);
  cPPt->cd(2); cpPPtRat.Draw((TPad*)cPPt->GetPad(2),false);
  cPPt->Print(outdir+"/PPt"+compTag+".gif");

  TCanvas * cJEt = new TCanvas("JEt","JEt",1000,500);
  cJEt->Divide(2,1);
  CPlot cpJEt("JEt","JEt","E_{T}^{genjet}","##/4GeV");
  cpJEt.SetLogy();
  cpJEt.AddHist1D(hgCm.H("Cm1JEtNr"),"Ref - j1","hist",kOrange+2,kOpenDiamond);
  cpJEt.AddHist1D(hgCm.H("JEtNr"),"Ana - j1","E",kBlue,kOpenSquare);
  cpJEt.AddHist1D(hgCm.H("Cm1JEtAw"),"Ref - j2","hist",kRed,kOpenStar);
  cpJEt.AddHist1D(hgCm.H("JEtAw"),"Ana - j2","E",kBlack,kFullCircle);
  cpJEt.SetLegend(0.21,0.74,0.54,0.94);
  cpJEt.SetLegendHeader(header);
  cJEt->cd(1); cpJEt.Draw((TPad*)cJEt->GetPad(1),false);
  CPlot cpJEtRat("JEtRat","JEtRat","E_{T}^{genjet}","Ana/Ref");
  cpJEtRat.AddHist1D(hgCm.R("JEtNrDivCm1JEtNr"),"j1",kBlue,kOpenSquare);
  cpJEtRat.AddHist1D(hgCm.R("JEtAwDivCm1JEtAw"),"j2");
  cpJEtRat.SetYRange(-0.5,2.5);
  //cpJEtRat.Rebin(2);
  cJEt->cd(2); cpJEtRat.Draw((TPad*)cJEt->GetPad(2),false);
  cJEt->Print(outdir+"/JEt"+compTag+".gif");

  TCanvas * cC5NP = new TCanvas("C5NP","C5NP",1000,500);
  cC5NP->Divide(2,1);
  CPlot cpC5NP("C5NP","C5NP","# of Trks (Bg Sub.) in Jet Cone","# of Jets");
  cpC5NP.AddHist1D(hgCm.H("Cm1C5NPSubNr"),"Ref - j1","hist",kOrange+2,kOpenCircle);
  cpC5NP.AddHist1D(hgCm.H("C5NPSubNr"),"Ana - j1","E",kBlue,kOpenSquare);
  cpC5NP.AddHist1D(hgCm.H("Cm1C5NPSubAw"),"Ref - j2","hist",kRed,kOpenStar);
  cpC5NP.AddHist1D(hgCm.H("C5NPSubAw"),"Ana - j2","E",kBlack,kFullCircle);
  cpC5NP.SetLegend(0.194,0.7,0.52,0.94);
  cpC5NP.SetLegendHeader(header);
  cpC5NP.Rebin(5);
  cpC5NP.ShowStats(11111);
  cC5NP->cd(1); cpC5NP.Draw((TPad*)cC5NP->GetPad(1),false);
  CPlot cpC5NPRat("C5NPRat","C5NPRat","# of Trks (Bg Sub.) in Jet Cone","Ana/Ref");
  //cpC5NPRat.AddHist1D(hgCm.R("C5NPSubNrSubCm1C5NPSubNr"),"",kBlue,kOpenSquare);
  //cpC5NPRat.AddHist1D(hgCm.R("C5NPSubAwSubCm1C5NPSubAw"),"",kBlack);
  //cpC5NPRat.Rebin(5);
  //cC5NP->cd(2); cpC5NPRat.Draw((TPad*)cC5NP->GetPad(2),false);
  cC5NP->Print(outdir+"/C5NP"+compTag+".gif");
}
