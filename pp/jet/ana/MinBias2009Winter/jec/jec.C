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

  // ===== check jec vs eta =====
  Int_t N = 100;
  Double_t eta[N], pt[N], E[N], scale[N];
  Double_t etaMin=-3;
  Double_t etaMax=3;
  Double_t etaStep=(etaMax-etaMin)/N;
  const Int_t NPT = 2;
  Double_t ptS[NPT] = {7,20};
  TGraph *grs[20];
  Style_t lst[20] = {1,7,6};

  //  -pt=7 GeV, ak5-
  for (Int_t ptSl=0; ptSl<NPT; ++ptSl) {
    for (Int_t i=0; i<N; ++i) {
      eta[i]=etaMin+i*etaStep;
      Double_t theta=2*atan(exp(-1*eta[i]));
      E[i]=ptS[ptSl]/cos(theta);
      scale[i] = JEC->getCorrection(ptS[ptSl],eta[i],E[i]);
    }
    grs[ptSl] = new TGraph(N,eta,scale);
    grs[ptSl]->SetLineColor(kRed);
    grs[ptSl]->SetLineStyle(lst[ptSl]);
  }

  //  -draw-
  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  TH1D * hEta = new TH1D("hEta","hEta",100,etaMin,etaMax);
  hEta->SetMinimum(0.9);
  hEta->SetMaximum(3.5);
  hEta->Draw();
  grs[0]->Draw("C");
  grs[1]->Draw("C");
}

