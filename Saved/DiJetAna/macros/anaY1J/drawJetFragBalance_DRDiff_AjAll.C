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
#include "drawJetFragBalance_DRDiff.C"
using namespace std;

void drawJetFragBalance_DRDiff_AjAll(
    TString anaV="CorrEtaTrkEffPtBin5RBin20v7",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    TString drawV="d5",
    Int_t logScale=0,
    Int_t normType=1)
{
  TCanvas *c1 = new TCanvas("c1", "",1443,402);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,4,1,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  int canvas = 1;

  c1->cd(canvas++);
  drawJetFragBalance_DRDiff("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj0to11_"+BckSub+".root",
			    "jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj0to11_"+BckSub+".root",0,0,1);
  drawText("0-30%",0.83,0.9);

  drawText("A_{J}<0.11",0.21+leftSpace,downSpace+0.52);
  drawPatch(0.976,0.0972,1.1,0.171);
  
  c1->cd(canvas++);
  drawJetFragBalance_DRDiff("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj11to22_"+BckSub+".root",
                            "jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj11to22_"+BckSub+".root",0,0,0);
  drawText("0-30%",0.78,0.9);
  drawText("0.11<A_{J}<0.22",0.11,downSpace+0.52);

  drawPatch(-0.00007,0.0972,0.0518,0.171);
  drawPatch(0.976,0.0972,1.1,0.141);


  c1->cd(canvas++);
  drawJetFragBalance_DRDiff("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj22to33_"+BckSub+".root",
                            "jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj22to33_"+BckSub+".root",0,0,0);
  drawText("0-30%",0.78,0.9);
  drawText("0.22<A_{J}<0.33",0.11,downSpace+0.52);

  drawPatch(-0.00007,0.0972,0.0518,0.171);
  drawPatch(0.976,0.0972,1.1,0.141);

  
  c1->cd(canvas++);
  drawJetFragBalance_DRDiff("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj33to100_"+BckSub+".root",
                            "jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj33to100_"+BckSub+".root",0,0,0);
  drawText("CMS",0.11,0.90);
  drawText("PbPb  #sqrt{s}_{_{NN}}=2.76 TeV",0.11,0.83);
  drawText("#intL dt = 6.7 #mub^{-1}",0.11,0.73);
  drawText("0-30%",0.78,0.9);
  drawText("A_{J}>0.33",0.11,downSpace+0.52);
  drawPatch(-0.00007,0.0972,0.0518,0.171);

  c1->Print("RocketProjectionDR_3Aj_v1.gif");
  c1->Print("RocketProjectionDR_3Aj_v1.eps");
  c1->Print("RocketProjectionDR_3Aj_v1.C");

}
