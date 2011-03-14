#include <iostream>
#include "TChain.h"
#include "TString.h"
#include "TrkCorrHisAna.h"
using namespace std;

void MakeCorr()
{
  TChain * tsim = new TChain("hitrkEffAnalyzer/simTrackTree");
  tsim->Add("~/scratch01/ana/HydjetBass_DJUQ110_GSR_v2_HighPtv0EffNt/mergeAll/all_nt.root");
  tsim->Print();

  TChain * trec = new TChain("hitrkEffAnalyzer/recTrackTree");
  trec->Add("~/scratch01/ana/HydjetBass_DJUQ110_GSR_v2_HighPtv0EffNt/mergeAll/all_nt.root");
  trec->Print();

  TFile * outfile = new TFile("trkCorrHisAna.root","RECREATE");

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
  centBins.push_back(39);

  TrkCorrHisAna hisana("A0");
  hisana.outFile_ = outfile;
  hisana.tsim_ = tsim;
  hisana.trec_ = trec;
  hisana.ptBins = ptBinsA0;
  hisana.etaBins = etaBins;
  hisana.jetBins = jetBins;
  hisana.centBins = centBins;
  hisana.DeclareHistograms();
  hisana.LoopSim();
  hisana.LoopRec();
  hisana.WriteHistograms();
}
