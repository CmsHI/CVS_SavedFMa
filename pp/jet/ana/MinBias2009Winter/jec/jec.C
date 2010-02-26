#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include "QCDAnalysis/HighPtJetAnalysis/interface/Utilities.h"
#include "CondFormats/JetMETObjects/interface/CombinedJetCorrector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <string>
#include <vector>
#include "common.h"

using namespace std;

void jec(){
  // Define JEC's
  string JECLevels = "L2:L3";
  string JECTag = "900GeV_L2Relative_AK5Calo:900GeV_L3Absolute_AK5Calo";
  string JECTagIC = "900GeV_L2Relative_IC5Calo:900GeV_L3Absolute_IC5Calo";
  CombinedJetCorrector* JECs[2];
  JECs[0] = new CombinedJetCorrector(JECLevels,JECTag);
  JECs[1] = new CombinedJetCorrector(JECLevels,JECTagIC);

  // ===== check jec vs eta =====
  Int_t N = 100;
  Double_t eta[N], pt[N], E[N], scale[N];
  Double_t etaMin=-3;
  Double_t etaMax=3;
  Double_t etaStep=(etaMax-etaMin)/N;
  const Int_t NPT = 2;
  Double_t ptS[NPT] = {7,20};
  //Double_t ptS[NPT] = {7,20,40};
  TGraph *grs[2][20];
  Style_t lst[20] = {1,7,6};
  Color_t lc[20] = {kRed, kBlue};

  // make graphs
  for (Int_t ijec=0; ijec<2; ++ijec) {
    for (Int_t ptSl=0; ptSl<NPT; ++ptSl) {
      cout << "JEC " << ijec << "  pt slice: " << ptS[ptSl] << endl;
      for (Int_t i=0; i<N; ++i) {
	eta[i]=etaMin+i*etaStep;
	Double_t theta=2*atan(exp(-1*eta[i]));
	E[i]=ptS[ptSl]/cos(theta);
	scale[i] = JECs[ijec]->getCorrection(ptS[ptSl],eta[i],E[i]);
      }
      grs[ijec][ptSl] = new TGraph(N,eta,scale);
      grs[ijec][ptSl]->SetLineColor(lc[ijec]);
      grs[ijec][ptSl]->SetLineStyle(lst[ptSl]);
    }
  }

  //  -draw-
  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  TH1D * hEta = new TH1D("hEta",";Jet #eta; JEC factor",100,etaMin,etaMax);
  hEta->SetMinimum(0.9);
  hEta->SetMaximum(3.5);
  hEta->Draw("");
  grs[0][0]->Draw("C");
  grs[0][1]->Draw("C");
  //grs[0][2]->Draw("C");
  grs[1][0]->Draw("C");
  grs[1][1]->Draw("C");
  //grs[1][2]->Draw("C");
  TLegend *leg = new TLegend(0.419,0.174,.721,0.37,NULL,"brNDC");
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->AddEntry(grs[0][0],"ak5 raw jet p_{T}=7","l");
  leg->AddEntry(grs[1][0],"ic5 raw jet p_{T}=7","l");
  leg->AddEntry(grs[0][1],"ak5 raw jet p_{T}=20","l");
  leg->AddEntry(grs[1][1],"ic5 raw jet p_{T}=20","l");
  leg->Draw();
  printFinalCanvases(c0,"jec_vs_eta",0);
}

