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
#include "TSystem.h"
#include "drawTrkEnergy_leadSubLeading.h"
using namespace std;

void drawTrkEnergyAjAll_1_4AjBins_genHydjetVSrecoHydjet(
			  TString anaV1="v19ReWt",
			  TString anaV2="v19ReWt", // check v19DeadForPixReWt
			  TString module="djcalo",
			  TString BckSub="SubEtaReflSingle",
			  TString title="_Track",
			  TString drawV="d5",
			  Int_t logScale=1,
			  Int_t normType=0)
{
  TCanvas *c1 = new TCanvas("c1","",1199,645);
  TString mc="HydjetAll";
  TString mc2="HydjetSig";
  TString outdir=anaV2+mc+mc2+BckSub;

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,leftMargin,bottomMargin,0.02);
  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  Float_t j1x=0.14,j1y=4*downSpace,j2x=0.56,j2y=4*downSpace;
  Float_t ax=0.52,ay=0.77;

  c1->cd(1);
  drawTrkEnergy("jfh"+anaV1+"_"+mc2+"_"+module+"_genp_Cent0to30_Aj0to11_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("A_{J} < 0.11",leftSpace+0.63,0.82);
  drawText("Hydjet",0.33,0.81);
  drawText("+PYTHIA",0.33,0.73);
  drawText("MC truth",0.33,0.5);

  drawText("0-30%",0.33,0.60);
  drawPatch(0,0.0,0.28,0.1);
  
  c1->cd(2);
  drawTrkEnergy("jfh"+anaV1+"_"+mc2+"_"+module+"_genp_Cent0to30_Aj11to22_"+BckSub+".root",true,true,logScale,normType,0);
  drawText("0.11 < A_{J} < 0.22",0.52,0.82);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.46,15);
     drawText("Subleading Jet",j2x+.06, 0.46,15);
  }
 
  c1->cd(3);
  drawTrkEnergy("jfh"+anaV1+"_"+mc2+"_"+module+"_genp_Cent0to30_Aj22to33_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("0.22 < A_{J} < 0.33",0.52,0.82);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.46,15);
     drawText("Subleading Jet",j2x+.06, 0.46,15);
  }

  c1->cd(4);
  drawTrkEnergy("jfh"+anaV1+"_"+mc2+"_"+module+"_genp_Cent0to30_Aj33to100_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("A_{J} > 0.33",0.63,0.82);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.46,15);
     drawText("Subleading Jet",j2x+.06, 0.46,15);
  }
  

  c1->cd(5);
  drawTrkEnergy("jfh"+anaV2+"_"+mc+"_"+module+"_Cent0to30_Aj0to11_"+BckSub+".root",false,true,logScale,normType,1);
  drawText("A_{J} < 0.11",leftSpace+0.63,downSpace+0.73);
  drawText("Hydjet",0.33,0.81);
  drawText("+PYTHIA",0.33,0.73);
  drawText("MC Reco",0.33,0.54);
  drawText("0-30%",0.33,downSpace+0.51);

  c1->cd(6);
  drawTrkEnergy("jfh"+anaV2+"_"+mc+"_"+module+"_Cent0to30_Aj11to22_"+BckSub+".root",false,false,logScale,normType,1);
  drawText("0.11 < A_{J} < 0.22",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+.06, 0.48+downSpace,15);
  }

  c1->cd(7);
  drawTrkEnergy("jfh"+anaV2+"_"+mc+"_"+module+"_Cent0to30_Aj22to33_"+BckSub+".root",false,false,logScale,normType,1);
  drawText("0.22 < A_{J} < 0.33",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+.06, 0.48+downSpace,15);
  }

  c1->cd(8);
  drawTrkEnergy("jfh"+anaV2+"_"+mc+"_"+module+"_Cent0to30_Aj33to100_"+BckSub+".root",false,false,logScale,normType,1);
  drawText("A_{J} > 0.33",0.63,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+.06, 0.48+downSpace,15);
  }

  gSystem->mkdir(outdir,kTRUE);
  c1->Print(anaV2+"_"+BckSub+Form("_TrkPtDR_AjAll_genP_vs_recoTrk_log%d.eps",logScale));
}
