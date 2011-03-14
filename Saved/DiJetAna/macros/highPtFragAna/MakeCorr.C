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

  TrkCorrHisAna hisana("A0");
  hisana.tsim_ = tsim;
  hisana.trec_ = trec;
  hisana.LoopSim();
  hisana.LoopRec();
}
