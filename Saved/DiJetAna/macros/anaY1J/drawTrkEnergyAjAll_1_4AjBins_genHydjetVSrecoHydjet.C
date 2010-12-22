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

void drawTrkEnergyAjAll_1_4AjBins_genHydjetVSrecoHydjet(
			  TString anaV="CorrEtaTrkEffPtBin5RBin20v9",
			  //TString anaV="CorrEtaTrkEffPtBin5RBin20v1",
			  TString module="djcalo",
			  TString BckSub="SubEtaRefl",
			  TString title="_Track",
			  TString drawV="d5",
			  Int_t logScale=0,
			  Int_t normType=0)
{
   TCanvas *c1 = new TCanvas("c1","",1199,645);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  c1->cd(1);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_genp_Cent0to30_Aj0to11_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("A_{J} < 0.11",leftSpace+0.63,0.82);
  drawText("Leading jet",0.4,downSpace+0.15,15);
  drawText("Sub-leading jet",0.72,downSpace+0.15,15);
  drawText("Hydjet",0.33,0.81);
  drawText("+PYTHIA",0.33,0.73);
  drawText("MC truth",0.33,0.5);

  drawText("0-30%",0.33,0.60);
  drawPatch(0,0.0,0.28,0.1);
  
  c1->cd(2);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_genp_Cent0to30_Aj11to22_"+BckSub+title+".root",true,true,logScale,normType);
  drawText("0.11 < A_{J} < 0.22",0.52,0.82);
  drawText("Leading jet",0.15,downSpace+0.15,15);
  drawText("Sub-leading jet",0.6,downSpace+0.15,15);
 
  c1->cd(3);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_genp_Cent0to30_Aj22to33_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("0.22 < A_{J} < 0.33",0.52,0.82);
  drawText("Leading jet",0.15,downSpace+0.15,15);
  drawText("Sub-leading jet",0.6,downSpace+0.15,15);

  c1->cd(4);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_genp_Cent0to30_Aj33to100_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("A_{J} > 0.33",0.63,0.82);
  drawText("Leading jet",0.15,downSpace+0.15,15);
  drawText("Sub-leading jet",0.6,downSpace+0.15,15);
  

  c1->cd(5);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_Cent0to30_Aj0to11_"+BckSub+title+".root",false,true,logScale,normType);
  drawText("A_{J} < 0.11",leftSpace+0.63,downSpace+0.73);
  drawText("Leading jet",0.4,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.72,3*downSpace+0.1,15);
  drawText("Hydjet",0.33,0.81);
  drawText("+PYTHIA",0.33,0.73);
  drawText("MC Reco",0.33,0.54);
  drawText("0-30%",0.33,downSpace+0.51);

  c1->cd(6);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_Cent0to30_Aj11to22_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("0.11 < A_{J} < 0.22",0.52,downSpace+0.73);
  drawText("Leading jet",0.15,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.63,3*downSpace+0.1,15);

  c1->cd(7);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_Cent0to30_Aj22to33_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("0.22 < A_{J} < 0.33",0.52,downSpace+0.73);
  drawText("Leading jet",0.15,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.6,3*downSpace+0.1,15);

  c1->cd(8);
  drawTrkEnergy("plot/drawn_jfh"+anaV+"ReWt_Hydjet_"+module+"_Cent0to30_Aj33to100_"+BckSub+title+".root",false,false,logScale,normType);
  drawText("A_{J} > 0.33",0.63,downSpace+0.73);
  drawText("Leading jet",0.15,3*downSpace+0.1,15);
  drawText("Sub-leading jet",0.6,3*downSpace+0.1,15);

  c1->Print(anaV+"_SubEtaRefl_TrackTrackEnergyPtRBkgSubAjAll_leadSubd5_genTrack_VS_recoTrack.gif");
  c1->Print(anaV+"_SubEtaRefl_TrackTrackEnergyPtRBkgSubAjAll_leadSubd5_genTrack_VS_recoTrack.C");
  c1->Print(anaV+"_SubEtaRefl_TrackTrackEnergyPtRBkgSubAjAll_leadSubd5_genTrack_VS_recoTrack.eps");
}
