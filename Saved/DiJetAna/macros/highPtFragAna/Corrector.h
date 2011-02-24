#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TString.h"
using namespace std;

class Corrector
{
  public:
    vector<TFile*> sample_;
    vector<TString> centBin_;
    TH1D * ptBin_;
    TH1D * etaBin_;
    TH1D * jetBin_;
    TString trkCorrModule_;

    vector<TString> levelName_;
    vector<vector<TString> > levelInput_;
    vector<TString> matName_;
    vector<Float_t> ptHatMin_;

    Int_t numSamples_;
    Int_t numCentBins_;
    Int_t numPtBins_;
    Int_t numEtaBins_;
    Int_t numJEtBins_;
    Int_t numLevels_;

    vector<vector<vector<vector<vector<TH2F*> > > > > correction_;

    Corrector();
    void Init();
    void GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Float_t * corr);
};

Corrector::Corrector() :
  trkCorrModule_("hitrkEffAnalyzer")
{
  centBin_.push_back("0to1");
  centBin_.push_back("2to3");
  centBin_.push_back("4to11");
  centBin_.push_back("12to19");
  centBin_.push_back("20to39");
  numCentBins_ = centBin_.size();

  levelName_.push_back("Eff");
  levelName_.push_back("Fak");
  levelName_.push_back("Mul");
  levelName_.push_back("Sec");
  numLevels_ = levelName_.size();

  levelInput_.resize(numLevels_);
  levelInput_[0].push_back("heff3D"); levelInput_[0].push_back("hsim3D");
  levelInput_[1].push_back("hfak3D"); levelInput_[1].push_back("hrec3D");
  levelInput_[2].push_back("hmul3D"); levelInput_[2].push_back("hsim3D");
  levelInput_[3].push_back("hsec3D"); levelInput_[3].push_back("hrec3D");

  matName_.push_back("Num");
  matName_.push_back("Den");

  ptHatMin_.push_back(30);
  ptHatMin_.push_back(50);
  ptHatMin_.push_back(80);
  ptHatMin_.push_back(110);
  ptHatMin_.push_back(170);

  for (Int_t i=0; i<ptHatMin_.size(); ++i) {
    TString fname=Form("djtrkhist_hydjetBassv2_djuq%.0f.root",ptHatMin_[i]);
    sample_.push_back(new TFile(fname));
    cout << fname << endl;
  }
  numSamples_ = sample_.size();

  // Get x,y,z bins
  TH3F* h3 = (TH3F*)sample_[0]->Get(trkCorrModule_+"/heff3D_cbin"+centBin_[0]);
  if (!h3) {
    cout << "bad input: " << sample_[0]->GetName() << endl;
    exit(1);
  }

  etaBin_ = (TH1D*)h3->Project3D("x");
  ptBin_  = (TH1D*)h3->Project3D("y");
  jetBin_ = (TH1D*)h3->Project3D("z");

  numPtBins_ = ptBin_->GetNbinsX();
  numEtaBins_ = etaBin_->GetNbinsX();
  //numJEtBins_ = jetBin_->GetNbinsX();
  numJEtBins_ = jetBin_->FindBin(400);

  cout << "nbins pt,eta,jet: " << numPtBins_ << " " << numEtaBins_ << " " << numJEtBins_ << endl;

  vector<vector<vector<vector<vector<TH2F*> > > > > v(numLevels_,
      vector<vector<vector<vector<TH2F*> > > > (numSamples_,
	vector<vector<vector<TH2F*> > > (numCentBins_,
	  vector<vector<TH2F*> > (numJEtBins_,
	    vector<TH2F*>(2)
	    )
	  )
	)
      );
  correction_ = v;
}

void Corrector::Init()
{
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    for (Int_t s=0; s<numSamples_; ++s) {
      for (Int_t c=0; c<numCentBins_; ++c) {
	for (Int_t m=0; m<2; ++m) {
	  TString hname(Form("%s/%s_cbin%d",trkCorrModule_.Data(),levelInput_[lv][m].Data(),c));
	  cout << hname << endl;
	  for (Int_t j=0; j<numJEtBins_; ++j) {
	    //cout << correction_[lv][s][c][j][m] << endl;
	  }
	}
      }
    }
  }
}
