#include <iostream>
#include <vector>
#include <cmath>
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "Corrector.h"
#include "HisMath.C"
using namespace std;

struct JetFrag
{
  Float_t cent;
  Float_t centwt;
  vector<Float_t> * jtpt;
  vector<Float_t> * jteta;
  vector<Float_t> * jtphi;
  Float_t jdphi;
  vector<Float_t> * ppt;
  vector<Float_t> * peta;
  vector<Float_t> * pphi;
  vector<Float_t> * trkeff;
  vector<Float_t> * trkfak;
  vector<Float_t> * trkmul;
  vector<Float_t> * trksec;
  vector<vector<Float_t> > * pdr;
  vector<vector<Float_t> > * pdrbg;
  JetFrag()
  {
    jtpt = 0;
    jteta = 0;
    jtphi = 0;
    ppt = 0;
    peta = 0;
    pphi = 0;
    trkeff = 0;
    trkfak = 0;
    trkmul = 0;
    trksec = 0;
    pdr = 0;
    pdrbg = 0;
  }
};

void SetJetFragTree(TTree * t, JetFrag & jf)
{
  t->SetBranchAddress("cent",&jf.cent);
  t->SetBranchAddress("centwt",&jf.centwt);
  t->SetBranchAddress("jtpt",&jf.jtpt);
  t->SetBranchAddress("jteta",&jf.jteta);
  t->SetBranchAddress("jtphi",&jf.jtphi);
  t->SetBranchAddress("jdphi",&jf.jdphi);
  t->SetBranchAddress("ppt",&jf.ppt);
  t->SetBranchAddress("peta",&jf.peta);
  t->SetBranchAddress("pphi",&jf.pphi);
  t->SetBranchAddress("trkeff",&jf.trkeff);
  t->SetBranchAddress("trkfak",&jf.trkfak);
  t->SetBranchAddress("trkmul",&jf.trkmul);
  t->SetBranchAddress("trksec",&jf.trksec);
  t->SetBranchAddress("pdr",&jf.pdr);
  t->SetBranchAddress("pdrbg",&jf.pdrbg);
}

class FragAnaLoop
{
  public:
    TChain * t_;
    JetFrag jttrk_;
    Corrector * trkCorr_;

    TString name_;
    Int_t anaTrkType_;

    vector<Double_t> numJet_;
    vector<Bool_t> passJet_;

    // analysis histograms
    vector<Double_t> ptBin_;
    vector<vector<TH1D*> > vhPPtCorr_;

    // monitor histograms
    vector<vector<TH2D*> > vhTrkCorrPPt_;
    vector<vector<TH2D*> > vhTrkCorrJEt_;
    vector<vector<TH2D*> > vhTrkCorrCent_;

    // Methods
    FragAnaLoop(TString name);
    void Init();
    void Loop();

    Bool_t SelEvt(const JetFrag & jf) {
      return (jf.cent<30);
    }
    Bool_t SelJet(const JetFrag & jf, Int_t j) {
      if (j==0 && (*jf.jtpt)[j]>=100 && (*jf.jtpt)[j]<200 && fabs((*jf.jteta)[j])<0.8) return true;
      if (j==1 && (*jf.jtpt)[j]>=50 && (*jf.jtpt)[j]<200 && fabs((*jf.jteta)[j])<0.8) return true;
      return false;
    }
    //Bool_t SelFragIncl();
    //Bool_t SelFragCone(Int_t j, Double_t r);
};

FragAnaLoop::FragAnaLoop(TString name) :
  name_(name),
  anaTrkType_(2),
  numJet_(2),
  passJet_(2),
  vhPPtCorr_(2),
  vhTrkCorrPPt_(2),
  vhTrkCorrJEt_(2),
  vhTrkCorrCent_(2)
{
}

void FragAnaLoop::Init()
{
  cout << "================ Ana Loop ==================" << endl;
  cout << " anaTrkType: " << anaTrkType_ << endl;
  cout << "============================================" << endl;

  SetJetFragTree(t_,jttrk_);

  for (Int_t j=0; j<2; ++j) {
    for (Int_t lv=0; lv<5; ++lv) {
      if (anaTrkType_==0 && lv>0) continue;
      vhPPtCorr_[j].push_back(new TH1D(Form("h%sPPtCorr%d_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",ptBin_.size()-1,&ptBin_[0]));
      vhPPtCorr_[j][lv]->Sumw2();

      vhTrkCorrPPt_[j].push_back(new TH2D(Form("h%sTrkCorr%dPPt_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",50,0,100,50,-0.2,1.2));
      vhTrkCorrJEt_[j].push_back(new TH2D(Form("h%sTrkCorr%dJEt_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",100,0,400,50,-0.2,1.2));
      vhTrkCorrCent_[j].push_back(new TH2D(Form("h%sTrkCorr%dCent_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",10,0,100,50,-0.2,1.2));
    }
  }
}

void FragAnaLoop::Loop()
{
  // Initialize counters
  for (Int_t j=0; j<2; ++j) {
    numJet_[j]=0;
    passJet_[j]=false;
  }

  // Main Loop
  Int_t numEntries = t_->GetEntries();

  for (Int_t i=0; i<numEntries; ++i) {
    t_->GetEntry(i);

    for (Int_t j=0; j<2; ++j) {
      if (SelJet(jttrk_,j)) {
	passJet_[j]=true;
	++numJet_[j];
	//cout << jttrk_.jtpt->at(0) << " " << jttrk_.jteta->at(0) << " " << jttrk_.jtphi->at(0) << endl;
      }
    }

    for (Int_t ip=0; ip<jttrk_.ppt->size(); ++ip) {
      Float_t trkEnergy = (*jttrk_.ppt)[ip];
      Float_t trkEta = (*jttrk_.peta)[ip];
      // Correction for this trk
      Double_t corr[4];
      Double_t eff=1,fak=1,mul=1,sec=1,trkwt=1;
      if (anaTrkType_>=2) {
	trkCorr_->GetCorr(trkEnergy,trkEta,(*jttrk_.jtpt)[0],jttrk_.cent,corr);
	eff = corr[0];
	fak = corr[1];
	mul = corr[2];
	sec = corr[3];
	if (eff<1e-5) { eff=1; }
	trkwt = (1-fak)*(1-sec)/(eff*(1+mul));
	if (trkwt<0||trkwt>20) {
	  cout << trkEnergy << " " << eff << " " << fak << " " << mul << " " << sec << " " << trkwt << endl;
	}
      }
      // Fill
      for (Int_t j=0; j<2; ++j) {
	//cout << (*jttrk_.pdr)[j][ip] << endl;
	//if (SelJet(jttrk_,j)&&(*jttrk_.pdr)[j][ip]<0.5)
	if (SelJet(jttrk_,j)&&fabs(trkEta)<1) {
	  vhPPtCorr_[j][0]->Fill(trkEnergy);
	  if (anaTrkType_==0) continue;
	  vhPPtCorr_[j][1]->Fill(trkEnergy,1./eff);
	  vhPPtCorr_[j][2]->Fill(trkEnergy,(1-fak)/eff);
	  vhPPtCorr_[j][3]->Fill(trkEnergy,(1-fak)/(eff*(1+mul)));
	  vhPPtCorr_[j][4]->Fill(trkEnergy,trkwt);
	  // study correction
	  for (Int_t lv=1; lv<=4; ++lv) {
	    vhTrkCorrPPt_[j][lv]->Fill(trkEnergy,corr[lv-1]);
	    vhTrkCorrJEt_[j][lv]->Fill((*jttrk_.jtpt)[j],corr[lv-1]);
	    vhTrkCorrCent_[j][lv]->Fill(jttrk_.cent,corr[lv-1]);
	  }
	}
      }
    }
  } // end of main loop

  // Normalize
  for (Int_t j=0; j<2; ++j) {
    cout << "jet " << j << " count: " << numJet_[j] << endl;
    for (Int_t lv=0; lv<5; ++lv) {
      if (anaTrkType_==0 && lv>0) continue;
      normHist(vhPPtCorr_[j][lv],0,true,1./numJet_[j]);
    }
  }
}
