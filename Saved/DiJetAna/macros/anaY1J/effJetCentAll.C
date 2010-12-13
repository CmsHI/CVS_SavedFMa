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
#include "effJet.C"
using namespace std;

void effJetCentAll(
    TString drawV="d0")
{
  TCanvas *c1 = new TCanvas("c1","",1250,500);

  Float_t leftMargin=0.22,bottomMargin=0.16;
  makeMultiPanelCanvas(c1,3,1,0.0,0.0,leftMargin,bottomMargin,0.02);

  Float_t leftSpace=leftMargin/2., downSpace=bottomMargin/2.;
  c1->cd(1);
  effJet(0,"dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127.root",30,100,0,true,0);
  drawText("CMS",0.28,0.90);
  drawText("30-100%",leftSpace+0.43,0.51);

  c1->cd(2);
  effJet(0,"dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127.root",10,30,0,true,1);
  drawText("10-30%",0.43,0.51);

  c1->cd(3);
  effJet(0,"dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127.root",0,10,0,true,0);
  //drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.56,0.82);
  //drawText("#intL dt = 6.7 #mub^{-1}",0.56,0.74);
  drawText("0-10%",0.43,0.51);

  c1->Print("DataMix_Eff_vs_GenJEt"+drawV+".gif");
  c1->Print("DataMix_Eff_vs_GenJEt"+drawV+".eps");
  c1->Print("DataMix_Eff_vs_GenJEt"+drawV+".C");
}


