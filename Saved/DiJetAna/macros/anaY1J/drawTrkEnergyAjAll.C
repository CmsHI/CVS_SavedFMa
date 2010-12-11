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

void drawTrkEnergyAjAll(
    TString anaV="CorrEtaHighPtv3WideRBin",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    TString drawV="d2",
    Int_t logScale=0)
{
  TCanvas *c1 = new TCanvas("c1","",700,650);

  makeMultiPanelCanvas(c1,2,2,0.0,0.0,0.22,0.18,0.02);

  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_PyquenUQ80_"+module+"_Cent0to100_Aj0to24_"+BckSub+title+".root",false,true,logScale);
  drawText("PYTHIA",0.56,0.51);
  drawText("A_{J}<0.24",0.56,0.43);

  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_PyquenUQ80_"+module+"_Cent0to100_Aj24to100_"+BckSub+title+".root",true,false,logScale);
  drawText("PYTHIA",0.43,0.51);
  drawText("A_{J}>0.24",0.43,0.43);

  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj0to24_"+BckSub+title+".root",false,true,logScale);
  drawText("0-30%",0.56,0.51);
  drawText("A_{J}<0.24",0.56,0.43);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj24to100_"+BckSub+title+".root",false,false,logScale);
  drawText("CMS",0.43,0.90);
  drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.43,0.82);
  drawText("#intL dt = 6.7 #mub^{-1}",0.43,0.74);
  drawText("10-30%",0.43,0.51);
  drawText("A_{J}>0.24",0.43,0.43);

  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubAjAll"+drawV+".gif");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubAjAll"+drawV+".eps");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubAjAll"+drawV+".C");
}
