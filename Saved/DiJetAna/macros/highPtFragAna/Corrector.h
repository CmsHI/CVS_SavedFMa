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

    // setup
    TString trkCorrModule_;
    Int_t ptRebinFactor_;
    Int_t sampleMode_; // 0 choose individually, 1 merge samples
    Int_t smoothLevel_;

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
    void Init(Int_t inputMethod=0, TString corrFileName="");
    Float_t GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * corr);
    TH2D * ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend);
    void Write();
    void InspectCorr(Int_t lv, Int_t s, Int_t c, Float_t jet,Int_t mode=0);
};

Corrector::Corrector() :
  trkCorrModule_("hitrkEffAnalyzer"),
  ptRebinFactor_(6),
  sampleMode_(0),
  smoothLevel_(0)
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

void Corrector::Init(Int_t inputMethod, TString corrFileName)
{
  cout << "==============================================" << endl;
  cout << " Setup Tracking Correction" << endl;
  cout << "==============================================" << endl;
  cout << "inputMethod: " << inputMethod << ", ptRebinFactor: " << ptRebinFactor_ << endl;
  cout << "Retrieval setup - sampleMode: " << sampleMode_ << " smoothLevel: " << smoothLevel_ << endl;
  TFile * corrFile=0;
  if (inputMethod==1) {
    corrFile = new TFile(corrFileName);
    //corrFile->ls();
  }
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
	    TH2D * h2 = 0;
	    TString h2name = Form("h%s_f%.0f_c%d_j%d_%d",levelName_[lv].Data(),ptHatMin_[s],c,j,m);
	    if (inputMethod==0) {
	      h2 = ProjectPtEta(h3,j,j);
	      h2->SetName(h2name);
	    } else if (inputMethod==1) {
	      h2 = (TH2D*)corrFile->Get(h2name);
	    }
	    h2->RebinY(ptRebinFactor_);
	    //cout << h2->GetName() << endl;
	    correction_[lv][s][c][j][m] = h2;
	  }
	}
      }
    }
    cout << endl;
  }

  if (ptRebinFactor_>1) {
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
  if (jetBin>=numJEtBins_-1) jetBin = numJEtBins_-1; // make sure don't exceed vector bound
  //cout << "bins: " << isample << " " << bin << " " << ptBin << " " << etaBin << " " << jetBin << endl;

  vector<vector<Double_t> > mat(numLevels_,vector<Double_t>(2));
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    for (Int_t m=0; m<2; ++m) {
      for (Int_t s=0; s<sample_.size(); ++s) {
	if (sampleMode_==0 && s!=isample) continue;
	if (jet<ptHatMin_[s]) continue;
	for (Int_t j=jetBin; j<=jetBin+1; ++j) {
	  if (smoothLevel_<1&&j!=jetBin) continue;
	  for (Int_t e=etaBin-1; e<etaBin+1; ++e) {
	    if (smoothLevel_<2&&e!=etaBin) continue;
	    if (e<1||e>numEtaBins_) continue;
	    mat[lv][m] += correction_[lv][s][bin][j][m]->GetBinContent(etaBin,ptBin);
	  }
	}
      }
    }
    if (mat[lv][1]>0) corr[lv] = mat[lv][0]/mat[lv][1];
    else {
      corr[lv]=1; // no correction
      cout << "No " << levelName_[lv] << " for pt,eta,jet,cent: " << pt << " " << eta << " " << jet << " " << cent << endl;
    }
  }

  // Done
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

void Corrector::Write()
{
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    for (Int_t s=0; s<numSamples_; ++s) {
      for (Int_t c=0; c<numCentBins_; ++c) {
	for (Int_t m=0; m<2; ++m) {
	  for (Int_t j=1; j<=numJEtBins_; ++j) {
	    correction_[lv][s][c][j][m]->Write();
	  }
	}
      }
    }
  }
}

void Corrector::InspectCorr(Int_t lv, Int_t s, Int_t c, Float_t jet, Int_t mode)
{
  Int_t jetBin = jetBin_->FindBin(jet);
  TH2D *hNum=0, *hDen=0, *hCorr=0;
  if (s>=0) {
    hNum = correction_[lv][s][c][jetBin][0];
    hDen = correction_[lv][s][c][jetBin][1];
    cout << levelName_[lv] << ": " << sample_[s]->GetName() << " (cbin,jetbin): " << c << " " << jetBin << endl;
    hCorr = (TH2D*)hNum->Clone(Form("%s_corr",hNum->GetName()));
  } else {
    hNum = correction_[lv][0][c][jetBin][0];
    hDen = correction_[lv][0][c][jetBin][1];
    for (Int_t i=1; i<sample_.size(); ++i) {
      hNum->Add(correction_[lv][i][c][jetBin][0]);
      hDen->Add(correction_[lv][i][c][jetBin][1]);
    }
    cout << levelName_[lv] << ": all samples (cbin,jetbin): " << c << " " << jetBin << endl;
    hCorr = (TH2D*)hNum->Clone(Form("%s_corrAllSample",hNum->GetName()));
  }

  TH1D * hCorr1D=0;
  if (mode==0) {
    hCorr->Divide(hNum,hDen);
    hCorr->SetAxisRange(0,25.2+3*6*4,"Y");
    hCorr->Draw("colz");
  } else if (mode==1) {
    TH1D * hNum1D = hNum->ProjectionX();
    TH1D * hDen1D = hDen->ProjectionX();
    hCorr1D = (TH1D*)hNum1D->Clone();
    hCorr1D->Divide(hNum1D,hDen1D);
    hCorr1D->SetAxisRange(0,1.2,"Y");
    hCorr1D->Draw();
  } else if (mode==2) {
    TH1D * hNum1D = hNum->ProjectionY();
    TH1D * hDen1D = hDen->ProjectionY();
    hCorr1D = (TH1D*)hNum1D->Clone();
    hCorr1D->Divide(hNum1D,hDen1D);
    hCorr1D->SetAxisRange(0,1.2,"Y");
    hCorr1D->SetAxisRange(0,25.2+3*6*4,"X");
    hCorr1D->Draw();
  }

}
