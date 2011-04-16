#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPad.h"
#include "TMath.h"
#include "TLatex.h"
#include "TString.h"
#include "drawTrkEnergy_leadSubLeading.h"
using namespace std;

void drawTrkEnergyAjAll_1_4AjBins_genHydjetVSrecoHydjet_logScale(
    TString anaV="v22_2ReWt", // 
    TString module="akpu3pf",
    TString BckSub="SubEtaReflSingle", // for paper: SubEtaReflSingle
    TString title="_Track",
    TString drawV="d5",
    Int_t logScale=0,
    Int_t normType=0)
{
  TString pref="jfh";
  TString mcSrc="HydjetGenP";
  TString dataSrc="HydjetReco";
  TString centBin="Cent0to30";
  TCanvas *c1 = new TCanvas("c1","",1199,645);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  Float_t j1x=0.14,j1y=4*downSpace,j2x=0.56,j2y=4*downSpace, j1ylog=0.475;
  Float_t ax=0.52,ay=0.77;
  
  c1->cd(1);
  drawTrkEnergy(pref+anaV+"_"+mcSrc+"_"+module+"_"+centBin+"_Aj0to13_"+BckSub+".root",false,true,logScale,normType,0);
  //drawText("A_{J} < 0.13",leftSpace+ax+0.1,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x+0.23,j1y,15);
    drawText("Subleading Jet",j2x+leftSpace,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x+0.21, j1ylog,15);
     drawText("Subleading Jet",j2x+leftSpace+.02, j1ylog,15);
  }

  drawText("PYTHIA",0.33,0.81);
  drawText("+HYDJET",0.33,0.73);
  drawText("GenP",0.33,0.65);
  //drawText("0-30%",0.33,0.60);
  drawPatch(0,0.0,0.28,0.1);
  drawText("(a)",0.33,0.91);

  c1->cd(2);
  drawTrkEnergy(pref+anaV+"_"+mcSrc+"_"+module+"_"+centBin+"_Aj13to24_"+BckSub+".root",false,true,logScale,normType,0);
  //drawText("0.13 < A_{J} < 0.24",ax,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, j1ylog,15);
     drawText("Subleading Jet",j2x+.06, j1ylog,15);
  }

  float ptx(0.05),pty(0.88);
  drawText("p_{T,1}  > 100GeV/c",ptx,pty-0.05,15);
  drawText("p_{T,2}  > 40GeV/c",ptx,pty-0.13,15);
  drawText("#Delta#phi_{1,2}>  #frac{2}{3}#pi",ptx,pty-0.21,15);
  drawText("(b)",ptx,0.91);

  c1->cd(3);
  drawTrkEnergy(pref+anaV+"_"+mcSrc+"_"+module+"_"+centBin+"_Aj24to35_"+BckSub+".root",true,true,logScale,normType,0);
  //drawText("0.24 < A_{J} < 0.35",ax,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, j1ylog,15);
     drawText("Subleading Jet",j2x+.06, j1ylog,15);
  }
  drawText("(c)",ptx,0.91);
  
  c1->cd(4);
  drawTrkEnergy(pref+anaV+"_"+mcSrc+"_"+module+"_"+centBin+"_Aj35to70_"+BckSub+".root",false,true,logScale,normType,0);
  //drawText("0.35 < A_{J} < 0.7",ax,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, j1ylog,15);
     drawText("Subleading Jet",j2x+.06, j1ylog,15);
  }
  drawText("(d)",ptx,0.91);


  c1->cd(5);
  drawTrkEnergy(pref+anaV+"_"+dataSrc+"_"+module+"_"+centBin+"_Aj0to13_"+BckSub+".root",false,true,logScale,normType,0);  // express sys error by error band for log scale
  drawText("A_{J} < 0.13",leftSpace+0.63,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x+0.23,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x+leftSpace,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x + 0.21, j1ylog+0.02+downSpace,15);
     drawText("Subleading Jet",j2x+leftSpace+.02, j1ylog+0.02+downSpace,15);
  }
  if (logScale==1) {
     //drawErrorBox(.22,22,.30,22 * 1.2,1); //20% of uncertainty  
     //drawText("Systematic",j2x+leftSpace+.08, 0.64+downSpace,15);
     //drawText("uncertainty",j2x+leftSpace+.08, 0.58+downSpace,15);
  }

  drawText("(e)",0.33,0.93);
  drawText("PYTHIA",0.31,downSpace+0.76);
  drawText("+HYDJET",0.31,downSpace+0.69);
  drawText("RECO",0.31,downSpace+0.62);
  //drawText("CMS",0.31,downSpace+0.76);
  //drawText("PbPb  #sqrt{s}_{_{NN}}=2.76 TeV",0.31,0.8,12);
  //drawText("#intL dt = 6.7 #mub^{-1}",0.31,0.73,12);
  drawText("0-30%",0.32,downSpace+0.56);

  c1->cd(6);
  drawTrkEnergy(pref+anaV+"_"+dataSrc+"_"+module+"_"+centBin+"_Aj13to24_"+BckSub+".root",false,false,logScale,normType,0);  // express sys error by error band for log scale
  drawText("0.13 < A_{J} < 0.24",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, j1ylog+0.02+downSpace,15);
     drawText("Subleading Jet",j2x+.06, j1ylog+0.02+downSpace,15);
  }
  drawText("(f)",ptx,0.93);

  c1->cd(7);
  drawTrkEnergy(pref+anaV+"_"+dataSrc+"_"+module+"_"+centBin+"_Aj24to35_"+BckSub+".root",false,false,logScale,normType,0);  // express sys error by error band for log scale
  drawText("0.24 < A_{J} < 0.35",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, j1ylog+0.02+downSpace,15);
     drawText("Subleading Jet",j2x+.06, j1ylog+0.02+downSpace,15);
  }

  drawText("(g)",ptx,0.93);

  c1->cd(8);
  drawTrkEnergy(pref+anaV+"_"+dataSrc+"_"+module+"_"+centBin+"_Aj35to70_"+BckSub+".root",false,false,logScale,normType,0);  // express sys error by error band for log scale
  drawText("0.35 < A_{J} < 0.7",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, j1ylog+0.02+downSpace,15);
     drawText("Subleading Jet",j2x+.06, j1ylog+0.02+downSpace,15);
  }
  drawText("(h)",ptx,0.93);
  drawText("Anti-k_{T} PF",j1x-0.05,downSpace+0.73);
  drawText("R=0.3",j1x-0.05,downSpace+0.65);

  c1->Print(anaV+"_"+BckSub+Form("_TrkPtDR_AjAll_genP_vs_mcReco_log%d.gif",logScale));
  c1->Print(anaV+"_"+BckSub+Form("_TrkPtDR_AjAll_genP_vs_mcReco_log%d.C",logScale));
  c1->Print(anaV+"_"+BckSub+Form("_TrkPtDR_AjAll_genP_vs_mcReco_log%d.eps",logScale));
}
