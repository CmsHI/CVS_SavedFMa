#ifndef TrkCorrHisAna_h
#define TrkCorrHisAna_h
#include "TChain.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"

#include <iostream>
#include <cmath>
using namespace std;

typedef struct
{
  int ids;
  float etas;
  float pts;
  int hits;
  int status;
  int acc;
  int nrec;
  float ptr;
  float dz;
  float d0;
  float pterr;
  float d0err;
  float dzerr;
  int hitr;
  int algo;
  float jetr;
  int cbin;
} SimTrack_t;

typedef struct
{
  int charge;
  float etar;
  float ptr;
  float phir;
  float dz;
  float d0;
  float pterr;
  float d0err;
  float dzerr;
  int hitr; 
  int algo;
  int nsim;
  int status;
  int ids;
  int parids;
  float etas;
  float pts;
  float jetr;
  int cbin;
} RecTrack_t;

class TrkCorrHisAna
{
  public:
    TChain * tsim_;
    TChain * trec_;
    vector<Double_t> ptBins_;
    vector<Double_t> etaBins_;
    vector<Double_t> jetBins_;
    vector<Int_t> centBins_;

    TString name_;

    // methods
    TrkCorrHisAna(TString name);
    void DeclareHistograms();
    void FillSimHistograms(const SimTrack_t & s);
    void FillRecHistograms(const RecTrack_t & r);
    void LoopSim();
    void LoopRec();
    void WriteHistograms();
};


TrkCorrHisAna::TrkCorrHisAna(TString name) :
  name_(name)
{
}

void TrkCorrHisAna::LoopSim()
{
  SimTrack_t s;
  tsim_->SetBranchAddress("simTrackValues",&s.ids);
  for (Int_t i=0; i<tsim_->GetEntries(); ++i) {
    tsim_->GetEntry(i);
    if (s.pts<150) continue;
    cout << s.ids << " " << s.etas << " " << s.pts << " " << s.jetr << " " << s.cbin << endl;
  }
}

void TrkCorrHisAna::LoopRec()
{
  RecTrack_t r;
  trec_->SetBranchAddress("recTrackValues",&r.charge);
  for (Int_t i=0; i<trec_->GetEntries(); ++i) {
    trec_->GetEntry(i);
    if (r.ptr<150) continue;
    cout << r.charge << " " << r.etar << " " << r.ptr << " " << r.jetr << " " << r.cbin << endl;
  }
}
#endif //TrkCorrHisAna_h
