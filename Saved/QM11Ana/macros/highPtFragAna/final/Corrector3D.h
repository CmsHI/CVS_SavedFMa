#ifndef Corrector3D_h
#define Corrector3D_h
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TString.h"
#include "TLine.h"
using namespace std;

class Corrector3D
{
  public:
    vector<TFile*> sample_;
    vector<TString> centBin_;
    TH1D * ptBin_;
    TH1D * etaBin_;
    TH1D * jetBin_;

    // setup
    TString corrSetName_;
    TString corrSetNameApp_;
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

    vector<vector<vector<vector<TH3F*> > > > correction_;

    Corrector3D(TString name="trkCorrHisAna_djuqv1",TString app="",TString mod="hitrkEffAnalyzer");
    void Init(Int_t inputMethod=0, TString corrFileName="");
    Float_t GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * corr, Float_t jet1);
    TH2D * ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend);
    void Write();
    TH1 * InspectCorr(Int_t lv, Int_t isample, Int_t c, Int_t jetBegBin, Int_t jetEndBin,Int_t mode=0,Int_t begbin=0, Int_t endbin=-1);
};

Corrector3D::Corrector3D(TString name, TString append, TString mod) :
  corrSetName_(name),
  corrSetNameApp_(append),
  trkCorrModule_(mod),
  ptRebinFactor_(1),
  sampleMode_(0),
  smoothLevel_(0)
{
  centBin_.push_back("0to1");
  centBin_.push_back("2to3");
  centBin_.push_back("4to11");
  centBin_.push_back("12to19");
  centBin_.push_back("20to35");
  //centBin_.push_back("20to39");
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

}

void Corrector3D::Init(Int_t inputMethod, TString corrFileName)
{
  cout << "==============================================" << endl;
  cout << " Setup: " << corrSetName_+corrSetNameApp_ << "/" << trkCorrModule_ << endl;
  cout << "==============================================" << endl;
  cout << "inputMethod: " << inputMethod << ", ptRebinFactor: " << ptRebinFactor_ << endl;
  cout << "Retrieval setup - sampleMode: " << sampleMode_ << " smoothLevel: " << smoothLevel_ << endl;
  // =============================
  // Setup Inputs
  // =============================
  for (Int_t i=0; i<ptHatMin_.size(); ++i) {
    TString fname=Form("../trkcorr/%s%.0f%s.root",corrSetName_.Data(),ptHatMin_[i],corrSetNameApp_.Data());
    sample_.push_back(new TFile(fname));
    cout << sample_[i]->GetName() << endl;
  }
  numSamples_ = sample_.size();
  if (numSamples_==0) {
    cout << "No input correction file" << endl;
    exit(1);
  }

  // =============================
  // Get x,y,z bins
  // =============================
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

  // =============================
  // allocate memory for correction table
  // =============================
  vector<vector<vector<vector<TH3F*> > > > v(numLevels_,
      vector<vector<vector<TH3F*> > > (numSamples_,
	vector<vector<TH3F*> > (numCentBins_,
	  vector<TH3F*>(2)
	  )
	)
      );
  correction_ = v;

  // =============================
  // load corrections
  // =============================
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    cout << "Load " << levelName_[lv] << " Histograms for ptHatMin";
    for (Int_t s=0; s<numSamples_; ++s) {
      cout << " " << ptHatMin_[s];
      for (Int_t c=0; c<numCentBins_; ++c) {
	for (Int_t m=0; m<2; ++m) {
	  TString hname(Form("%s/%s_cbin%s",trkCorrModule_.Data(),levelInput_[lv][m].Data(),centBin_[c].Data()));
	  TH3F * h3 = (TH3F*)sample_[s]->Get(hname);
	  //cout << hname << " " << h3->GetEntries() << endl;
	  TH3F * hsave = (TH3F*)h3->Clone(Form("h%s_f%.0f_c%d_%d",(corrSetName_+corrSetNameApp_+levelName_[lv]).Data(),ptHatMin_[s],c,m));
	  correction_[lv][s][c][m] = hsave;
	  //cout << correction_[lv][s][c][m]->GetName() << ": " << correction_[lv][s][c][m] << endl;
	}
      }
    }
    cout << endl;
  }
}

Float_t Corrector3D::GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * corr, Float_t jet1)
{
  Int_t bin = -1;
  Int_t isample=-1;

  // Get the corresponding centrality bin
  if (cent<2) {
    bin = 0;
  } else if (cent<4) {
    bin = 1;
  } else if (cent<12) {
    bin = 2;
  } else if (cent<20) {
    bin = 3;   
  } else if (cent<40) {
    bin = 4;
  }

  // Get the corresponding pt_hat min sample
  for (Int_t s=ptHatMin_.size()-1; s>=0; --s) {
    if (jet>=ptHatMin_[s]) {
      isample=s;
      break;
    }
  }

  // Find Bin
  Int_t ptBin = ptBin_->FindBin(pt);
  Int_t etaBin = etaBin_->FindBin(eta);
  Int_t jetBin = jetBin_->FindBin(jet);
  if (jetBin<1) jetBin = 1; // make sure don't exceed vector bound
  if (jetBin>=numJEtBins_-1) jetBin = numJEtBins_-1; // make sure don't exceed vector bound
  //cout << "bins: " << isample << " " << bin << " " << ptBin << " " << etaBin << " " << jetBin << endl;

  vector<vector<Double_t> > mat(numLevels_,vector<Double_t>(2));
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    for (Int_t m=0; m<2; ++m) {
      for (Int_t s=0; s<sample_.size(); ++s) {
	if (sampleMode_==0 && s!=isample) continue;
	if (jet<ptHatMin_[s]) continue;
	for (Int_t j=jetBin; j<=jetBin+2; ++j) {
	  if (smoothLevel_<1&&j!=jetBin) continue;
	  for (Int_t e=etaBin-1; e<etaBin+1; ++e) {
	    if (smoothLevel_<2&&e!=etaBin) continue;
	    if (e<1||e>numEtaBins_) continue;
	    mat[lv][m] += correction_[lv][s][bin][m]->GetBinContent(etaBin,ptBin,j);
	  }
	}
      }
    }
    if (mat[lv][1]>0) corr[lv] = mat[lv][0]/mat[lv][1];
    else {
      corr[lv]=1; // no correction
      //cout << "No " << levelName_[lv] << " for pt,eta,jet,cent: " << pt << " " << eta << " " << jet << " " << cent << endl;
    }
  }

  // Done
  if (corr[0]>(1-1e-3)||corr[0]<1e-3) corr[0] = 0.5; // if 0 or 1 take average trk eff
  Double_t eff = corr[0];
  Double_t fake = corr[1];
  Double_t mul = corr[2];
  Double_t sec = corr[3];
  return (1-fake)*(1-sec)/(eff*(1+mul));
}

TH2D* Corrector3D::ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend)
{
  h3->GetZaxis()->SetRange(zbinbeg,zbinend);
  TH2D * h2 = (TH2D*)h3->Project3D(Form("yx_%d_%d",zbinbeg,zbinend));
  return h2;
}

void Corrector3D::Write()
{
  for (Int_t lv=0; lv<numLevels_; ++lv) {
    for (Int_t s=0; s<numSamples_; ++s) {
      for (Int_t c=0; c<numCentBins_; ++c) {
	for (Int_t m=0; m<2; ++m) {
	  correction_[lv][s][c][m]->Write();
	}
      }
    }
  }
}

TH1 * Corrector3D::InspectCorr(Int_t lv, Int_t isample, Int_t c, Int_t jetBegBin, Int_t jetEndBin, Int_t mode, Int_t begbin, Int_t endbin)
{
  TH3F *hNum=0, *hDen=0;
  TH2D *hNum2D=0, *hDen2D=0, *hCorr2D=0;
  TH1D *hNum1D=0, *hDen1D=0, *hCorr1D=0;
  TString inspName(Form("%s_%s_Lv%d_%d_%d_j%d_%d_%d",(corrSetName_+corrSetNameApp_).Data(),trkCorrModule_.Data(),lv,isample,c,jetBegBin,jetEndBin,mode));
  if (mode>0) inspName+=Form("bin_%d_%d",begbin,endbin);
  cout << inspName << endl;

  hNum = (TH3F*)correction_[lv][0][c][0]->Clone(inspName+"hNum");
  hNum->Reset();
  hNum->Sumw2();
  hDen = (TH3F*)correction_[lv][0][c][1]->Clone(inspName+"hDen");
  hDen->Reset();
  hDen->Sumw2();
  for (Int_t s=0; s<sample_.size(); ++s) {
    if (sampleMode_==0&&s!=isample) continue;
    hNum->Add(correction_[lv][s][c][0]);
    hDen->Add(correction_[lv][s][c][1]);
  }

  hNum->GetZaxis()->SetRange(jetBegBin,jetEndBin);
  hDen->GetZaxis()->SetRange(jetBegBin,jetEndBin);
  hNum2D = (TH2D*)hNum->Project3D("yx");
  hDen2D = (TH2D*)hDen->Project3D("yx");

  // ===============================================
  // 2D Inspection
  // ===============================================
  if (mode==0) {
    hCorr2D = (TH2D*)hNum2D->Clone(inspName+"Corr2D");
    hCorr2D->Divide(hNum2D,hDen2D);
    hCorr2D->SetAxisRange(0,25.2+3*6*4,"Y");
    return hCorr2D;
  }

  // ===============================================
  // 1D Projection
  // ===============================================
  if (mode==1) {
    hNum1D = hNum2D->ProjectionX(inspName+"Num_px",begbin,endbin);
    hDen1D = hDen2D->ProjectionX(inspName+"Den_px",begbin,endbin);
  } else if (mode==2) {
    hNum1D = hNum2D->ProjectionY(inspName+"Num_py",begbin,endbin);
    hDen1D = hDen2D->ProjectionY(inspName+"Den_py",begbin,endbin);
  }

  // ===============================================
  // 1D Inspection
  // ===============================================
  hCorr1D = (TH1D*)hNum1D->Clone(inspName+"Corr1D");
  hCorr1D->Divide(hNum1D,hDen1D);
  if (mode==2) hCorr1D->SetAxisRange(0,25.2+3*6*4,"X");
  hCorr1D->SetAxisRange(0,1.5,"Y");
  TLine * l = new TLine(0,1,120,1);
  l->SetLineStyle(2);
  l->Draw();

  return hCorr1D;
}
#endif // Corrector3D_h
