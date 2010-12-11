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
    TString anaV="CorrEtaHighPtv3WideRBin",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    TString drawV="d1",
    Int_t logScale=0)
{
  TCanvas *c1 = new TCanvas("c1","",720,650);

  makeMultiPanelCanvas(c1,2,2,0.0,0.0,0.22,0.18,0.02);

  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_PyquenUQ80_"+module+"_Cent0to100_Aj0to100_"+BckSub+title+".root",false,true,logScale);
  drawText("PYTHIA",0.56,0.51);

  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent30to100_Aj0to100_"+BckSub+title+".root",true,false,logScale);
  drawText("30-100%",0.43,0.51);

  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent10to30_Aj0to100_"+BckSub+title+".root",false,true,logScale);
  drawText("CMS",0.56,0.90);
  drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.56,0.80);
  drawText("#intL dt = 6.7 #mub^{-1}",0.56,0.70);
  drawText("10-30%",0.56,0.51);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to10_Aj0to100_"+BckSub+title+".root",false,false,logScale);
  drawText("0-10%",0.44,0.51);

  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll"+drawV+".gif");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll"+drawV+".eps");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll"+drawV+".C");
}


