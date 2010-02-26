#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <iostream>
#include "QCDAnalysis/HighPtJetAnalysis/interface/Utilities.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <string>
#include <vector>

using namespace std;

void jec(){
  string JECLevels = "L2:L3";
  //string JECTag = "900GeV_L2Relative_IC5Calo:900GeV_L3Absolute_IC5Calo";
  string JECTag = "900GeV_L2Relative_AK5Calo:900GeV_L3Absolute_AK5Calo";
  CombinedJetCorrector *JEC = new CombinedJetCorrector(JECLevels,JECTag);
  string JECTagIC = "900GeV_L2Relative_IC5Calo:900GeV_L3Absolute_IC5Calo";
  CombinedJetCorrector *JECIC = new CombinedJetCorrector(JECLevels,JECTagIC);

  Int_t N = 100;
  Double_t eta[N], pt[N], E[N], scale[N];

  // ===== check jec vs eta =====
  Double_t etaMin=-3;
  Double_t etaMax=3;
  Double_t etaStep=(etaMax-etaMin)/N;
  Double_t pt0=7;
  Double_t pt1=20;
  TGraph *gr0, *gr1, *gr2, *gr3;

  //  -pt=7 GeV, ak5-
  for (Int_t i=0; i<N; ++i) {
    eta[i]=etaMin+i*etaStep;
    Double_t theta=2*atan(exp(-1*eta[i]));
    E[i]=pt0/cos(theta);
    scale[i] = JEC->getCorrection(pt0,eta[i],E[i]);
  }
  gr0 = new TGraph(N,eta,scale);
  gr0->SetLineColor(kRed);

  //  -pt=20 GeV, ak5-
  for (Int_t i=0; i<N; ++i) {
    eta[i]=etaMin+i*etaStep;
    Double_t theta=2*atan(exp(-1*eta[i]));
    E[i]=pt1/cos(theta);
    scale[i] = JEC->getCorrection(pt1,eta[i],E[i]);
  }
  gr1 = new TGraph(N,eta,scale);
  gr1->SetLineColor(kRed);
  gr1->SetLineStyle(7);

  //  -draw-
  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  TH1D * hEta = new TH1D("hEta","hEta",100,etaMin,etaMax);
  hEta->SetMinimum(0.9);
  hEta->SetMaximum(3.5);
  hEta->Draw();
  gr0->Draw("C");
  gr1->Draw("C");
}

