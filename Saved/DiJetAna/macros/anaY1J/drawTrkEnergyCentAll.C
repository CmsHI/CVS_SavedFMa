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
#include "drawTrkEnergy.h"
using namespace std;

void drawTrkEnergyCentAll(
    TString anaV="CorrEtaHighPtv2WideRBin",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    Int_t logScale=0)
{
  TCanvas *c1 = new TCanvas("c1","",700,650);

  makeMultiPanelCanvas(c1,2,2,0.0,0.0,0.2,0.18,0.02);

  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_PyquenUQ80_"+module+"_Cent0to100_Aj0to100_"+BckSub+title+".root",false,true,logScale);
  drawText("PYTHIA",0.22,0.90);

  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent30to100_Aj0to100_"+BckSub+title+".root",true,false,logScale);
  drawText("CMS",0.03,0.90);
  drawText("30-100%",0.03,0.85);

  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent10to30_Aj0to100_"+BckSub+title+".root",false,true,logScale);
  drawText("CMS",0.22,0.90);
  drawText("10-30%",0.22,0.85);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to10_Aj0to100_"+BckSub+title+".root",false,false,logScale);
  drawText("CMS",0.04,0.90);
  drawText("0-10%",0.04,0.85);

  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll.gif");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll.eps");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll.C");
}


