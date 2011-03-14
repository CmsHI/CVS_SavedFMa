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
    vector<Double_t> ptBins;
    vector<Double_t> etaBins;
    vector<Double_t> jetBins;
    vector<Int_t> centBins;

    TString name_;
    // SimTrack
    TH2F* hsim;
    TH2F* hacc;
    TH2F* heff;
    TH2F* hmul;

    TH3F* hsim3D;
    TH3F* heff3D;
    TH3F* hmul3D;
    TH3F* hresStoR3D;

    // RecTrack
    TH2F* hrec;
    TH2F* hfak;
    TH2F* hsec;

    TH3F* hrec3D;
    TH3F* hfak3D;
    TH3F* hsec3D;

    // vector of histograms
    std::vector<TH3F*> vhsim3D;
    std::vector<TH3F*> vheff3D;
    std::vector<TH3F*> vhmul3D;

    std::vector<TH3F*> vhrec3D;
    std::vector<TH3F*> vhfak3D;
    std::vector<TH3F*> vhsec3D;

    std::vector<TH3F*> vhresStoR3D;

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

void TrkCorrHisAna::DeclareHistograms()
{
  // simulated
  hsim = new TH2F("hsim","Sim Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // accepted
  hacc = new TH2F("hacc","Accepted Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // efficiency
  heff = new TH2F("heff","Effic Rec Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // multiply reconstructed
  hmul = new TH2F("hmul","Mult Rec Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // reconstructed
  hrec = new TH2F("hrec","Rec Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // fakes
  hfak = new TH2F("hfak","Fake Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // secondary
  hsec = new TH2F("hsec","Secondary Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);


  // simulated 3D 
  hsim3D = new TH3F("hsim3D","Sim Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // efficiency  3D 
  heff3D = new TH3F("heff3D","Effic Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // multiply reconstructed 3D 
  hmul3D = new TH3F("hmul3D","Mult Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);


  // reconstructed 3D 
  hrec3D = new TH3F("hrec3D","Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // fakes 3D 
  hfak3D = new TH3F("hfak3D","Fake Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // secondary
  hsec3D = new TH3F("hsec3D","Secondary Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // mom resolution (Sim to Rec) 
  hresStoR3D = new TH3F("hresStoR3D","Momentum resolution (sim to rec);#eta;sim p_{T} (GeV/c);rec p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      ptBins.size()-1, &ptBins[0]);

  for(unsigned i=0;i<centBins.size()-1;i++){
    vhsim3D.push_back(new TH3F("","Sim Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vheff3D.push_back(new TH3F("","Effic Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhmul3D.push_back(new TH3F("","Mult Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhrec3D.push_back(new TH3F("","Rec Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhfak3D.push_back(new TH3F("","Fake Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhsec3D.push_back(new TH3F("","Secondary Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhresStoR3D.push_back(new TH3F("","Momentum resolution (sim to rec);#eta;sim p_{T} (GeV/c);rec p_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], ptBins.size()-1, &ptBins[0]) );
    if(i==0) {
      vhsim3D[i]->SetName(Form("hsim3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vheff3D[i]->SetName(Form("heff3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhrec3D[i]->SetName(Form("hrec3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhfak3D[i]->SetName(Form("hfak3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhmul3D[i]->SetName(Form("hmul3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhsec3D[i]->SetName(Form("hsec3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhresStoR3D[i]->SetName(Form("hresStoR3D_cbin%dto%d",centBins[i],centBins[i+1]));
    }else{
      vhsim3D[i]->SetName(Form("hsim3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vheff3D[i]->SetName(Form("heff3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhrec3D[i]->SetName(Form("hrec3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhfak3D[i]->SetName(Form("hfak3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhmul3D[i]->SetName(Form("hmul3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhsec3D[i]->SetName(Form("hsec3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhresStoR3D[i]->SetName(Form("hresStoR3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
    }
  }
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
