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

void drawTrkEnergyAjAll_1_4AjBins_recoVSreco_logScale(
    TString anaV="v18", // for paper: v18
    TString anaV2="v20RandomCone", // for paper: v18, MB+Jet v20RandomConeReWt
    TString module="djcalo",
    TString BckSub="SubEtaReflSingle", // for paper: SubEtaReflSingle
    TString title="_Track",
    TString drawV="d5",
    Int_t logScale=0,
    Int_t normType=0)
{
  TString src1="HCPR_J50U"; // for paper: data, HCPR_J50U mc, HydjetSig
  TString src2="HCPR_J50MixJ50Offset1";
  TCanvas *c1 = new TCanvas("c1","",1199,645);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  Float_t j1x=0.14,j1y=4*downSpace,j2x=0.56,j2y=4*downSpace;
  Float_t ax=0.52,ay=0.77;
  
  c1->cd(1);
  drawTrkEnergy("jfh"+anaV+"_"+src1+"_"+module+"_Cent0to30_Aj0to11_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("A_{J} < 0.11",leftSpace+ax+0.1,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x+0.23,j1y,15);
    drawText("Subleading Jet",j2x+leftSpace,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x+0.21, 0.46,15);
     drawText("Subleading Jet",j2x+leftSpace+.02, 0.46,15);
  }

  drawText("CMS",0.33,0.81);
  //drawText("PYTHIA",0.33,0.81);
  //drawText("+HYDJET RECO",0.33,0.73);
  drawText("0-30%",0.33,0.60);
  drawPatch(0,0.0,0.28,0.1);
  drawText("(a)",0.33,0.91);
  c1->cd(2);
  drawTrkEnergy("jfh"+anaV+"_"+src1+"_"+module+"_Cent0to30_Aj11to22_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("0.11 < A_{J} < 0.22",ax,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.46,15);
     drawText("Subleading Jet",j2x+.06, 0.46,15);
  }

  float ptx(0.05),pty(0.88);
  drawText("p_{T,1}  > 120GeV/c",ptx,pty-0.05,15);
  drawText("p_{T,2}  > 50GeV/c",ptx,pty-0.13,15);
  drawText("#Delta#phi_{1,2}>  #frac{2}{3}#pi",ptx,pty-0.21,15);
  drawText("(b)",ptx,0.91);

  c1->cd(3);
  drawTrkEnergy("jfh"+anaV+"_"+src1+"_"+module+"_Cent0to30_Aj22to33_"+BckSub+".root",true,true,logScale,normType,0);
  drawText("0.22 < A_{J} < 0.33",ax,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.46,15);
     drawText("Subleading Jet",j2x+.06, 0.46,15);
  }
  drawText("(c)",ptx,0.91);
  
  c1->cd(4);
  drawTrkEnergy("jfh"+anaV+"_"+src1+"_"+module+"_Cent0to30_Aj33to100_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("A_{J} > 0.33",ax+0.1,ay);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y,15);
    drawText("Subleading Jet",j2x,j2y,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.46,15);
     drawText("Subleading Jet",j2x+.06, 0.46,15);
  }
  drawText("(d)",ptx,0.91);


  c1->cd(5);
  drawTrkEnergy("jfh"+anaV2+"_"+src2+"_"+module+"_Cent0to30_Aj0to11_"+BckSub+".root",false,true,logScale,normType,0);
  drawText("A_{J} < 0.11",leftSpace+0.63,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x+0.23,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x+leftSpace,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x + 0.21, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+leftSpace+.02, 0.48+downSpace,15);
  }
  if (logScale==1) {
     drawErrorBox(.22,22,.30,22 * 1.2,1); //20% of uncertainty  
     drawText("Systematic",j2x+leftSpace+.08, 0.64+downSpace,15);
     drawText("uncertainty",j2x+leftSpace+.08, 0.58+downSpace,15);
   
  }

  drawText("(e)",0.33,0.93);
  drawText("CMS",0.31,downSpace+0.76);
  drawText("Random Cone",0.31,0.8);//,12);
  drawText("(Jet sample)",0.31,0.73);//,12);
  //drawText("Excluded Dead Pix Region",0.31,0.73,12);
  drawText("0-30%",0.32,downSpace+0.56);

  c1->cd(6);
  drawTrkEnergy("jfh"+anaV2+"_"+src2+"_"+module+"_Cent0to30_Aj11to22_"+BckSub+".root",false,false,logScale,normType,0);
  drawText("0.11 < A_{J} < 0.22",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+.06, 0.48+downSpace,15);
  }
  drawText("(f)",ptx,0.93);

  c1->cd(7);
  drawTrkEnergy("jfh"+anaV2+"_"+src2+"_"+module+"_Cent0to30_Aj22to33_"+BckSub+".root",false,false,logScale,normType,0);
  drawText("0.22 < A_{J} < 0.33",0.52,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+.06, 0.48+downSpace,15);
  }

  drawText("(g)",ptx,0.93);

  c1->cd(8);
  drawTrkEnergy("jfh"+anaV2+"_"+src2+"_"+module+"_Cent0to30_Aj33to100_"+BckSub+".root",false,false,logScale,normType,0);
  drawText("A_{J} > 0.33",0.63,downSpace+0.73);
  if (logScale==0) {
    drawText("Leading Jet",j1x,j1y+downSpace+0.02,15);
    drawText("Subleading Jet",j2x,j2y+downSpace+0.02,15);
  }
  if (logScale==1) {
     drawText("Leading Jet",j1x - 0.05, 0.48+downSpace,15);
     drawText("Subleading Jet",j2x+.06, 0.48+downSpace,15);
  }
  drawText("(h)",ptx,0.93);

  c1->Print(anaV+"_"+BckSub+Form("_TrkPtDR_AjAll_reco_vs_reco_log%d.eps",logScale));
}
