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

    vector<vector<vector<vector<vector<TH2D*> > > > > correction_;

    Corrector();
    void Init();
    Float_t GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * corr);
    TH2D * ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend);
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
    cout << sample_[i]->GetName() << endl;
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
  numJEtBins_ = jetBin_->FindBin(399);

  cout << "nbins pt,eta,jet: " << numPtBins_ << " " << numEtaBins_ << " " << numJEtBins_ << endl;

  vector<vector<vector<vector<vector<TH2D*> > > > > v(numLevels_,
      vector<vector<vector<vector<TH2D*> > > > (numSamples_,
	vector<vector<vector<TH2D*> > > (numCentBins_,
	  vector<vector<TH2D*> > (numJEtBins_+1,
	    vector<TH2D*>(2)
	    )
	  )
	)
      );
  correction_ = v;
}

void Corrector::Init()
{
  Int_t ptRebinFactor=6;
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    cout << "Load " << levelName_[lv] << " Histograms for ptHatMin";
    for (Int_t s=0; s<numSamples_; ++s) {
      cout << " " << ptHatMin_[s];
      for (Int_t c=0; c<numCentBins_; ++c) {
	for (Int_t m=0; m<2; ++m) {
	  TString hname(Form("%s/%s_cbin%s",trkCorrModule_.Data(),levelInput_[lv][m].Data(),centBin_[c].Data()));
	  TH3F * h3 = (TH3F*)sample_[s]->Get(hname);
	  //cout << hname << " " << h3->GetEntries() << endl;
	  for (Int_t j=1; j<=numJEtBins_; ++j) {
	    TH2D * h2 = ProjectPtEta(h3,j,j);
	    h2->SetName(Form("h%s_f%.0f_c%d_j%d_%d",levelName_[lv].Data(),ptHatMin_[s],c,j,m));
	    h2->RebinY(ptRebinFactor);
	    //cout << h2->GetName() << endl;
	    correction_[lv][s][c][j][m] = h2;
	  }
	}
      }
    }
    cout << endl;
  }

  if (ptRebinFactor>1) {
    ptBin_  = (TH1D*)correction_[0][0][0][7][0]->ProjectionY();
    numPtBins_ = ptBin_->GetNbinsX();
  }
}

Float_t Corrector::GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * corr)
{
  Int_t bin = -1;
  Int_t isample=-1;

  // Get the corresponding centrality bin
  if (cent<5) {
    bin = 0;
  } else if (cent<10) {
    bin = 1;
  } else if (cent<30) {
    bin = 2;
  } else if (cent<50) {
    bin = 3;   
  } else if (cent<100) {
    bin = 4;
  }

  // Get the corresponding pt_hat min sample
  if (jet>=170) {
    isample=4;
  }  else if (jet>=110) {
    isample=3;
  }  else if (jet>=80) {
    isample=2;
  } else if (jet>=50) {
    isample=1;
  } else { 
    isample=0;
  }

  // Find Bin
  Int_t ptBin = ptBin_->FindBin(pt);
  Int_t etaBin = etaBin_->FindBin(eta);
  Int_t jetBin = jetBin_->FindBin(jet);
  if (jetBin>=numJEtBins_) jetBin = numJEtBins_; // make sure don't exceed vector bound
  //cout << "bins: " << isample << " " << bin << " " << ptBin << " " << etaBin << " " << jetBin << endl;

  vector<vector<Double_t> > mat(numLevels_,vector<Double_t>(2));
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    for (Int_t m=0; m<2; ++m) {
      mat[lv][m] = correction_[lv][isample][bin][jetBin][m]->GetBinContent(etaBin,ptBin);
    }
    if (mat[lv][1]>0) corr[lv] = mat[lv][0]/mat[lv][1];
    else {
      corr[lv]=1; // no correction
      cout << "No " << levelName_[lv] << " for pt,eta,jet,cent: " << pt << " " << eta << " " << jet << " " << cent << endl;
    }
  }
  Double_t eff = corr[0];
  Double_t fake = corr[1];
  Double_t mul = corr[2];
  Double_t sec = corr[3];
  return (1-fake)*(1-sec)/(eff*(1+mul));
}

TH2D* Corrector::ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend)
{
  h3->GetZaxis()->SetRange(zbinbeg,zbinend);
  TH2D * h2 = (TH2D*)h3->Project3D(Form("yx_%d_%d",zbinbeg,zbinend));
  return h2;
}
