#include <TF1.h>
#include <TCut.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLegend.h>
// #include "commonUtility.h"
#include "multiTreeUtilPhoton2011.h"

// Main Function --------------------------------------------------------------------
void checkPtHats() {
  /////////////////////////////////////////////////////////////////
  // This is the entry function that runs the full analysis
  //   * It decides what to run.
  /////////////////////////////////////////////////////////////////
  TH1::SetDefaultSumw2();

  ///////////////////////////////////////////////////////
  // Input trees
  ///////////////////////////////////////////////////////
  multiTreeUtil* mt  = new multiTreeUtil();
  TCut jetSelCut = "jetPt>100";
  TCut centCut = "";

  mt->addFile("../ntout/jskim_dj50_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=50&&pthat<80",1.021e-3*1.e3);
  mt->addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut && centCut && "pthat>=80&&pthat<9999",9.913e-5*1.e3);
  mt->NormalizeScales(2,"","hEvtCentNoSkim");

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetLogy();
  TH1D * hAll = new TH1D("hAll",";#hat{p}_{T} (GeV/c);",100,0,500);
  mt->Draw(hAll,"pthat","abs(jetEta)<2","1","E");

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  c3->SetLogy();
  TH1D * hAll2 = new TH1D("hAll2",";#hat{p}_{T} (GeV/c);",100,0,500);
  mt->Draw2(hAll2,"pthat","abs(jetEta)<2","1","E");

  TCanvas * c4 = new TCanvas("c4","c4",500,500);
  c4->SetLogy();
  TH1D * hAll3 = new TH1D("hAll3",";jet p_{T} (GeV/c);",100,0,500);
  mt->Draw2(hAll3,"jetPt","abs(jetEta)<2","1","E");
}
