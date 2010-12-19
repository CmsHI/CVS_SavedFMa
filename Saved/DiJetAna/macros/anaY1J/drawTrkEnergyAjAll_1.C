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

void drawTrkEnergyAjAll_1(
    TString anaV="CorrEtaPtBin5RBin20v0",
    //TString anaV="CorrEtaTrkEffPtBin5RBin20v1",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    TString drawV="d5",
    Int_t logScale=0,
    Int_t normType=0)
{
  TCanvas *c1 = new TCanvas("c1","",780,650);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj0to22_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("A_{J} < 0.22",leftSpace+0.63,0.52);
  drawText("Leading jet",0.4,downSpace+0.15,15);
  drawText("Sub-leading jet",0.72,downSpace+0.15,15);
  drawText("Hydjet",0.33,0.81);
  drawText("+PYTHIA",0.33,0.73);
  drawText("0-30%",0.33,0.60);

  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj22to100_"+BckSub+title+".root",true,true,logScale,normType);
  drawText("A_{J} > 0.22",0.63,0.52);
  drawText("Leading jet",0.15,downSpace+0.15,15);
  drawText("Sub-leading jet",0.6,downSpace+0.15,15);
 

  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj0to22_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("A_{J} < 0.22",leftSpace+0.63,downSpace+0.43);
  drawText("Leading jet",0.4,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.72,3*downSpace+0.1,15);
  drawText("CMS",0.33,downSpace+0.80);
  drawText("PbPb  #sqrt{s}_{_{NN}}=2.76 TeV",0.31,0.82,15);
  drawText("#intL dt = 6.7 #mub^{-1}",0.31,0.74,15);
  drawText("0-30%",0.33,downSpace+0.51);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj22to100_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("A_{J} > 0.22",0.63,downSpace+0.43);
  drawText("Leading jet",0.15,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.6,3*downSpace+0.1,15);

  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubAjAll_leadSub"+drawV+Form("_norm%d.gif",normType));
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubAjAll_leadSub"+drawV+Form("_norm%d.eps",normType));
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubAjAll_leadSub"+drawV+Form("_norm%d.C",normType));
}
