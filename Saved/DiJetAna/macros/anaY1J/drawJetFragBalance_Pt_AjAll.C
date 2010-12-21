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
#include "drawJetFragBalance_Pt.C"
using namespace std;

void drawJetFragBalance_Pt_AjAll(
    TString anaV="CorrEtaTrkEffPtBin5RBin20v2",
    TString module="djcalo",
    TString BckSub="SubEtaRefl",
    TString title="_Track",
    TString drawV="d5",
    Int_t logScale=0,
    Int_t normType=1)
{
  TCanvas *c1 = new TCanvas("c1", "",1043,402);

  Float_t leftMargin=0.28,bottomMargin=0.18;
  makeMultiPanelCanvas(c1,3,1,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  if(1){
  c1->cd(1);
  drawJetFragBalance_Pt("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj0to15_"+BckSub+".root",
			"jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj0to15_"+BckSub+".root",0,1);
  drawText("0-30%",0.85,0.9);
  drawText("A_{J}<1.5",0.36,downSpace+0.55);
  }

  c1->cd(2);
  drawJetFragBalance_Pt("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj15to29_"+BckSub+".root",
			"jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj15to29_"+BckSub+".root",0,0);
  drawText("0-30%",0.85,0.9);
  drawText("1.5<A_{J}<0.29",0.13,downSpace+0.56);

  c1->cd(3);

  drawJetFragBalance_Pt("jfh"+anaV+"_HCPR_J50U_"+module+"_Cent0to30_Aj29to100_"+BckSub+".root",
                        "jfh"+anaV+"_Hydjet_"+module+"_Cent0to30_Aj29to100_"+BckSub+".root",0,0);
  drawText("CMS",0.11,0.90);
  drawText("PbPb  #sqrt{s}_{_{NN}}=2.76 TeV",0.11,0.84);
  drawText("#intL dt = 6.7 #mub^{-1}",0.11,0.76);
  drawText("0-30%",0.78,0.9);
  drawText("A_{J}>0.29",0.13,downSpace+0.56);

  c1->Print("RocketProjectionPT_3Aj_v1.gif");
  c1->Print("RocketProjectionPT_3Aj_v1.eps");
  c1->Print("RocketProjectionPT_3Aj_v1.C");
}
