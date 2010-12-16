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


void drawTrkEnergyCentAll32_1(
    TString anaV="CorrEtaPtBin4RBin20v0",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    TString drawV="d5",
    Int_t logScale=0,
    Int_t normType=0)
{
  TCanvas *c1 = new TCanvas("c1","",1100,650);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,3,2,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_Hydjet_"+module+"_Cent30to100_Aj0to100_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("Leading jet",0.4,downSpace+0.15,15);
  drawText("Sub-leading jet",0.72,downSpace+0.15,15);


  //  drawText("Hydjet",leftSpace+0.63,0.81);
  //  drawText("+PYTHIA",leftSpace+0.63,0.73);
  drawText("30-100%",leftSpace+0.63,0.60);

  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_Hydjet_"+module+"_Cent10to30_Aj0to100_"+BckSub+title+".root",false,true,logScale,normType);
  //  drawText("Hydjet",0.63,0.81);
  //  drawText("+PYTHIA",0.63,0.73);
  drawText("10-30%",0.63,0.60);
  drawText("Leading jet",0.15,downSpace+0.15,15);
  drawText("Sub-leading jet",0.6,downSpace+0.15,15);


  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_Hydjet_"+module+"_Cent0to10_Aj0to100_"+BckSub+title+".root",true,true,logScale,normType);
  drawText("Hydjet",0.63,0.81);
  drawText("+PYTHIA",0.63,0.73);
  drawText("0-10%",0.63,0.60);
  drawText("Leading jet",0.15,downSpace+0.15,15);
  drawText("Sub-leading jet",0.6,downSpace+0.15,15);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent30to100_Aj0to100_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("30-100%",leftSpace+0.63,downSpace+0.51);
  //  drawText("CMS",leftSpace+0.53,downSpace+0.80);
  drawText("Leading jet",0.4,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.72,3*downSpace+0.1,15);

  c1->cd(5);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent10to30_Aj0to100_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("10-30%",0.63,downSpace+0.51);
  //  drawText("CMS",0.53,downSpace+0.80);
  drawText("Leading jet",0.15,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.6,3*downSpace+0.1,15);

  c1->cd(6);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to10_Aj0to100_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("0-10%",0.63,downSpace+0.51);
  drawText("CMS",0.53,downSpace+0.80);
  drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.53,0.82,15);
  drawText("#intL dt = 6.7 #mub^{-1}",0.53,0.74,15);
  drawText("Leading jet",0.15,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.6,3*downSpace+0.1,15);


  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll32"+drawV+Form("_norm%d.gif",normType));
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll32"+drawV+Form("_norm%d.eps",normType));
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll32"+drawV+Form("_norm%d.C",normType));
}



