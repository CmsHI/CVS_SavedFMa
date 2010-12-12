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
    TString drawV="d3",
    Int_t logScale=0,
    Int_t normType=1)
{
  TCanvas *c1 = new TCanvas("c1","",720,650);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_PyquenUQ80_"+module+"_Cent0to100_Aj0to100_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("PYTHIA",leftSpace+0.43,0.51);

  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent30to100_Aj0to100_"+BckSub+title+".root",true,false,logScale,normType);
  drawText("30-100%",0.43,0.51);

  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent10to30_Aj0to100_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("CMS",0.56,0.90);
  drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.56,0.82);
  drawText("#intL dt = 6.7 #mub^{-1}",0.56,0.74);
  drawText("10-30%",leftSpace+0.43,downSpace+0.51);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to10_Aj0to100_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("0-10%",0.44,downSpace+0.51);

  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll"+drawV+".gif");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll"+drawV+".eps");
  c1->Print(anaV+"_"+BckSub+title+"TrackEnergyPtRBkgSubCentAll"+drawV+".C");
}


