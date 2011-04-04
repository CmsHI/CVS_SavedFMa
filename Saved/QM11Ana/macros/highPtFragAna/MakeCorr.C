#include <iostream>
#include "TChain.h"
#include "TString.h"
#include "TrkCorrHisAna.h"
using namespace std;

void MakeCorr(Float_t ptHatMin=110)
{
  TString inFile(Form("trees/tr_hydjuq%.0f_jtv2_2.root",ptHatMin));
  TChain * tsim = new TChain("hitrkEffAnalyzer_nt/simTrackTree");
  tsim->Add(inFile);
  //tsim->Print();

  TChain * trec = new TChain("hitrkEffAnalyzer_nt/recTrackTree");
  trec->Add(inFile);
  //trec->Print();

  TFile * outfile = new TFile(Form("trkcorr/trkCorrHisAna_djuq%.0f_jtv2_2_cv2.root",ptHatMin),"RECREATE");

  // =========
  // A0
  // =========
  // pt bins
  const double small = 1e-3;
  double pt;
  vector<Double_t> ptBinsA0;
  for(pt =   0  ; pt <   1.2-small; pt +=  0.05) ptBinsA0.push_back(pt); // 24 bins
  for(pt =   1.2; pt <   2.4-small; pt +=  0.1 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =   2.4; pt <   7.2-small; pt +=  0.2 ) ptBinsA0.push_back(pt); // 24 bins
  for(pt =   7.2; pt <  13.2-small; pt +=  0.5 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =  13.2; pt <  25.2-small; pt +=  1.0 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =  25.2; pt <  61.2-small; pt +=  3.0 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =  61.2; pt < 181.2-small; pt +=  5.0 ) ptBinsA0.push_back(pt); // 24 bins
  ptBinsA0.push_back(181.2);
  // eta bins
  double etaMin   = -2.6;
  double etaMax   =  2.6;
  double etaWidth =  0.4;
  vector<Double_t> etaBins;
  for(double eta = etaMin; eta < etaMax + etaWidth/2; eta += etaWidth)
    etaBins.push_back(eta);
  //jet bins
  float jetMin = 0.0;
  float jetMax = 1000; // good to be matched with ana 
  float jetWidth = 20;
  vector<Double_t> jetBins;
  for(double jet = jetMin; jet < jetMax + jetWidth/2; jet += jetWidth)
    jetBins.push_back(jet);
  //centrality bins
  vector<Int_t> centBins;
  centBins.push_back(0);
  centBins.push_back(1);
  centBins.push_back(3);
  centBins.push_back(11);
  centBins.push_back(19);
  centBins.push_back(35);

  TrkCorrHisAna anaA0("A0");
  anaA0.outFile_ = outfile;
  anaA0.tsim_ = tsim;
  anaA0.trec_ = trec;
  anaA0.ptBins = ptBinsA0;
  anaA0.etaBins = etaBins;
  anaA0.jetBins = jetBins;
  anaA0.centBins = centBins;
  /*
  anaA0.DeclareHistograms();
  anaA0.LoopSim();
  anaA0.LoopRec();
  anaA0.WriteHistograms();

  // =========
  // A1
  // =========
  vector<Double_t> ptBinsA1;
  for(pt =   0  ; pt <   1.2-small; pt +=  0.05) ptBinsA1.push_back(pt); // 24 bins 
  for(pt =   1.2; pt <   2.4-small; pt +=  0.1 ) ptBinsA1.push_back(pt); // 12 bins
  for(pt =   2.4; pt <   7.2-small; pt +=  0.4 ) ptBinsA1.push_back(pt); // 12 bins
  for(pt =   7.2; pt <  16.8-small; pt +=  1.6 ) ptBinsA1.push_back(pt); // 6 bins 
  for(pt =  16.8; pt <  55.2-small; pt +=  6.4 ) ptBinsA1.push_back(pt); // 6 bins
  for(pt =  55.2; pt < 170.4-small; pt +=  9.6 ) ptBinsA1.push_back(pt); // 12 bins
  ptBinsA1.push_back(170.4);

  TrkCorrHisAna anaA1(anaA0);
  anaA1.name_ = "A1";
  anaA1.ptBins = ptBinsA1;
  anaA1.DeclareHistograms();
  anaA1.LoopSim();
  anaA1.LoopRec();
  anaA1.WriteHistograms();
  */

  /*
  // =========
  // B0
  // =========
  Int_t nbin=120;
  vector<Double_t> ptBinsB0;
  cout << "B0: ";
  Double_t a = 1;
  for (Int_t i=nbin; i>0; --i) {
    Double_t y = i/Double_t(nbin);
    Double_t pt = pow(y,-1./a);
    printf("%.3f ",pt);
    ptBinsB0.push_back(pt);
  }
  if (ptBinsB0.back()<200) ptBinsB0.push_back(200);
  cout << ptBinsB0.back() << endl;

  TrkCorrHisAna anaB0(anaA0);
  anaB0.name_ = "B0";
  anaB0.ptBins = ptBinsB0;
  anaB0.DeclareHistograms();
  anaB0.LoopSim();
  anaB0.LoopRec();
  anaB0.WriteHistograms();

  // =========
  // B1
  // =========
  vector<Double_t> ptBinsB1;
  cout << "B1: ";
  a = 0.8;
  for (Int_t i=nbin; i>0; --i) {
    Double_t y = i/Double_t(nbin);
    Double_t pt = pow(y,-1./a);
    printf("%.3f ",pt);
    ptBinsB1.push_back(pt);
  }
  if (ptBinsB1.back()<200) ptBinsB1.push_back(200);
  cout << ptBinsB1.back() << endl;

  TrkCorrHisAna anaB1(anaA0);
  anaB1.name_ = "B1";
  anaB1.ptBins = ptBinsB1;
  anaB1.DeclareHistograms();
  anaB1.LoopSim();
  anaB1.LoopRec();
  anaB1.WriteHistograms();
  */

  // =========
  // B2
  // =========
  /*
  vector<Double_t> ptBinsB2;
  cout << "B2: ";
  for(pt =   0  ; pt <   1.2-small; pt +=  0.05) ptBinsB2.push_back(pt); // 24 bins 
  for(pt =   1.2; pt <   2.4-small; pt +=  0.1 ) ptBinsB2.push_back(pt); // 12 bins
  for(pt =   2.4; pt <   7.2-small; pt +=  0.4 ) ptBinsB2.push_back(pt); // 12 bins
  for(pt =   7.2; pt <  20.0-small; pt +=  1.6 ) ptBinsB2.push_back(pt); // 8 bins 
  for(pt =  20.0; pt <  24-small;	 pt +=  2.0 ) ptBinsB2.push_back(pt); // 2 bins
  for(pt =  24.0; pt <  30-small;	 pt +=  3.0 ) ptBinsB2.push_back(pt); // 2 bins
  for(pt =  30.0; pt <  40-small;	 pt +=  5.0 ) ptBinsB2.push_back(pt); // 2 bins
  for(pt =  40.0; pt <  60-small;	 pt += 10.0 ) ptBinsB2.push_back(pt); // 2 bins
  for(pt =  60.0; pt < 300-small;	 pt += 30.0 ) ptBinsB2.push_back(pt); // 8 bins
  ptBinsB2.push_back(400);
  // Rebin
  Int_t rebinFactor=2;
  vector<Double_t> ptBinsB2Rebin;
  for (Int_t i=0; i<ptBinsB2.size(); i+=rebinFactor) { ptBinsB2Rebin.push_back(ptBinsB2[i]); }
  ptBinsB2 = ptBinsB2Rebin;
  */
  const Int_t numPPtBins=18;
  Float_t pptBins[numPPtBins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};
  vector<Double_t> ptBinsB2(pptBins,pptBins+numPPtBins+1);
  // Print
  cout << ptBinsB2.size()-1 << " bins - ";
  for (Int_t i=0; i<ptBinsB2.size(); ++i) { cout << ptBinsB2[i] << " "; }
  cout << endl;

  TrkCorrHisAna anaB2(anaA0);
  anaB2.name_ = "B2";
  anaB2.ptBins = ptBinsB2;
  anaB2.DeclareHistograms();
  anaB2.LoopSim();
  anaB2.LoopRec();
  anaB2.WriteHistograms();

  // =========
  // B2InConeJ1
  // =========
  TrkCorrHisAna anaB2InConeJ1(anaB2);
  anaB2InConeJ1.name_ = "B2InConeJ1";
  anaB2InConeJ1.selMode_ = 2;
  anaB2InConeJ1.DeclareHistograms();
  anaB2InConeJ1.LoopSim();
  anaB2InConeJ1.LoopRec();
  anaB2InConeJ1.WriteHistograms();

  // =========
  // B2InConeJ2
  // =========
  TrkCorrHisAna anaB2InConeJ2(anaB2);
  anaB2InConeJ2.name_ = "B2InConeJ2";
  anaB2InConeJ2.selMode_ = 3;
  anaB2InConeJ2.DeclareHistograms();
  anaB2InConeJ2.LoopSim();
  anaB2InConeJ2.LoopRec();
  anaB2InConeJ2.WriteHistograms();

  // =========
  // B2NoneJet
  // =========
  TrkCorrHisAna anaB2NoneJet(anaB2);
  anaB2NoneJet.name_ = "B2NoneJet";
  anaB2NoneJet.selMode_ = 11;
  //anaB2NoneJet.DeclareHistograms();
  //anaB2NoneJet.LoopSim();
  //anaB2NoneJet.LoopRec();
  //anaB2NoneJet.WriteHistograms();
}

void MakeCorrAll()
{
  MakeCorr(30);
  MakeCorr(50);
  MakeCorr(80);
  MakeCorr(110);
  MakeCorr(170);
}
