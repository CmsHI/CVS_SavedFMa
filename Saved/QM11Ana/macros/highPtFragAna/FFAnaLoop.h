#include <iostream>
#include <vector>
#include <cmath>
#include "TChain.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "Corrector3D.h"
#include "HisMath.C"
#include "selectionCut.h"
#include "../helpers.h"
using namespace std;
const Int_t MAXNP = 100000;

// =====================================================
// Input Structs
// =====================================================
struct JetFrag
{
  Float_t cent;
  Float_t jtpt[2];
  Float_t jteta[2];
  Float_t jtphi[2];
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  void LoadBranches(TTree * t) {
    t->SetBranchAddress("cent",&(this->cent));
    t->SetBranchAddress("jtpt",this->jtpt);
    t->SetBranchAddress("jteta",this->jteta);
    t->SetBranchAddress("jtphi",this->jtphi);
    t->SetBranchAddress("np",&(this->np));
    t->SetBranchAddress("ppt",this->ppt);
    t->SetBranchAddress("peta",this->peta);
    t->SetBranchAddress("pphi",this->pphi);
  }
};

// =====================================================
// Output Structs
// =====================================================
struct JetFragRel
{
  Float_t cent;
  Float_t jtpt[2];
  Float_t jteta[2];
  Float_t jtphi[2];
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  // relations
  Float_t jdphi;
  Float_t pjdr[2][MAXNP];
  Float_t pjdrbg[2][MAXNP];
  // methods
  void LoadBranches(TChain * t) {
    t->SetBranchAddress("cent",&(this->cent));
    t->SetBranchAddress("jtpt",this->jtpt);
    t->SetBranchAddress("jteta",this->jteta);
    t->SetBranchAddress("jtphi",this->jtphi);
    t->SetBranchAddress("np",&(this->np));
    t->SetBranchAddress("ppt",this->ppt);
    t->SetBranchAddress("peta",this->peta);
    t->SetBranchAddress("pphi",this->pphi);
  }
  void SetBranches(TTree * t) {
    t->Branch("cent",&(this->cent),"cent/F");
    t->Branch("jtpt",this->jtpt,"jtpt[2]/F");
    t->Branch("jteta",this->jteta,"jteta[2]/F");
    t->Branch("jtphi",this->jtphi,"jtphi[2]/F");
    t->Branch("np",&(this->np),"np/I");
    t->Branch("ppt",this->ppt,"ppt[np]/F");
    t->Branch("peta",this->peta,"peta[np]/F");
    t->Branch("pphi",this->pphi,"pphi[np]/F");
    // relations
    t->Branch("jdphi",&(this->jdphi),"jdphi/F");
    t->Branch("pjdr",this->pjdr,"pjdr[2][np]/F");
    t->Branch("pjdrbg",this->pjdrbg,"pjdrbg[2][np]/F");
  }
  void clear()
  {
    for (Int_t j=0; j<2; ++j) {
      jtpt[j]=-99;
    }
    np=0;
  }
};

// =====================================================
// Class Definition
// =====================================================
class FFAnaLoop
{
  public:
    TChain * t_;
    //JetFrag jf_;
    Corrector3D * trkCorr_;
    selectionCut * cut_;

    TString name_;
    Double_t ptHatMin_;
    Int_t anaTrkType_;

    vector<Double_t> numJet_;

    // analysis histograms
    vector<Double_t> ptBin_;
    vector<vector<TH1D*> > vhPPtCorr_;
    vector<vector<TH1D*> > vhPPtRat_;
    vector<vector<TH1D*> > vhXi_;
    vector<vector<TH1D*> > vhPDR_;
    // monitor histograms
    vector<vector<TH2D*> > vhTrkCorrPPt_;

    // analysis output
    JetFragRel jfr_;
    TTree * tjfr_;
    TNtuple * nt_;

    // Methods
    FFAnaLoop(TString name);
    void Init();
    void Loop();

    Bool_t SelEvt(const JetFragRel & jfr) {
      return (jfr.cent>=cut_->CentMin && jfr.cent<cut_->CentMax);
    }
    Bool_t SelJet(const JetFragRel & jfr, Int_t j) {
      if (j==0
	  && jfr.jtpt[j]>=cut_->JEtMin[j] && jfr.jtpt[j]<cut_->JEtMax[j] && fabs(jfr.jteta[j])<cut_->JEtaMax[j]
	 ) return true;
      else if (j==1
	  && jfr.jtpt[0]>=120 && jfr.jtpt[0]<250 && fabs(jfr.jteta[0])<2
	  && jfr.jtpt[j]>=cut_->JEtMin[j] && jfr.jtpt[j]<cut_->JEtMax[j] && fabs(jfr.jteta[j])<cut_->JEtaMax[j]
	 ) return true;
      return false;
    }
    Bool_t SelFrag(const JetFragRel & jfr, Int_t ip, Int_t j) {
	//return ( fabs(jfr.peta[ip])<cut_->TrkEtaMax );
	return ( jfr.pjdr[j][ip]<cut_->ConeSize && jfr.ppt[ip]>4);
    }
};

FFAnaLoop::FFAnaLoop(TString name) :
  name_(name),
  anaTrkType_(2),
  numJet_(2),
  vhPPtCorr_(2),
  vhPPtRat_(2),
  vhXi_(2),
  vhPDR_(2),
  vhTrkCorrPPt_(2)
{
}

void FFAnaLoop::Init()
{
  cout << "================ Ana Loop ==================" << endl;
  cout << " anaTrkType: " << anaTrkType_ << endl;
  cout << "============================================" << endl;

  jfr_.LoadBranches(t_);

  // Book Histograms
  for (Int_t j=0; j<2; ++j) {
    for (Int_t lv=0; lv<5; ++lv) {
      if (anaTrkType_==0 && lv>0) continue;
      vhPPtCorr_[j].push_back(new TH1D(Form("h%sPPtCorr%d_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",ptBin_.size()-1,&ptBin_[0]));
      vhPPtCorr_[j][lv]->Sumw2();
      vhPPtRat_[j].push_back(new TH1D(Form("h%sPPtRat%d_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",ptBin_.size()-1,&ptBin_[0]));
      vhPPtRat_[j][lv]->Sumw2();
      vhXi_[j].push_back(new TH1D(Form("h%sXi%d_j%d",name_.Data(),lv,j),"",12,0,6));
      vhXi_[j][lv]->Sumw2();
      vhPDR_[j].push_back(new TH1D(Form("h%sPDR%d_j%d",name_.Data(),lv,j),"",50,0,6.28));
      vhPDR_[j][lv]->Sumw2();

      vhTrkCorrPPt_[j].push_back(new TH2D(Form("h%sTrkCorr%dPPt_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",50,0,100,50,-0.2,1.2));
    }
  }

  // Book ntuples
  tjfr_ = new TTree("tjfr"+name_,"jet-particle relations");
  jfr_.SetBranches(tjfr_);
  nt_ = new TNtuple("nt","nt","pndr:padr");
}

void FFAnaLoop::Loop()
{
  // Initialize counters
  for (Int_t j=0; j<2; ++j) { numJet_[j]=0; }

  // Main Loop
  Int_t numEntries = t_->GetEntries();
  for (Int_t i=0; i<numEntries; ++i) {
    t_->GetEntry(i);
    // Calc Dijet properties
    jfr_.jdphi = deltaPhi(jfr_.jtphi[0],jfr_.jtphi[1]);
    /*
    if (cut_->doSel&&!SelEvt(jfr_)&&!SelJet(jfr_,0)&&!SelJet(jfr_,1)) {
      cout << "skip evt. cent: " << jfr_.cent << " jtpt0: " << jfr_.jtpt[0] << " jtpt1: " << jfr_.jtpt[1] << endl;
      continue;
    }
    */

    for (Int_t j=0; j<2; ++j) {
      if (!cut_->doSel||(SelEvt(jfr_)&&SelJet(jfr_,j))) {
	++numJet_[j];
      }
      //cout << jfr_.jtpt[j] << " " << jfr_.jteta[j] << " " << jfr_.jtphi[j] << " jdphi: " << jfr_.jdphi << endl;
    }

    for (Int_t ip=0; ip<jfr_.np; ++ip) {
      // Calc jet-particle relations
      for (Int_t j=0; j<2; ++j) {
	jfr_.pjdr[j][ip] = deltaR(jfr_.peta[ip],jfr_.pphi[ip],jfr_.jteta[j],jfr_.jtphi[j]);
	//cout << "jet " << j << " dr: " << jfr_.pjdr[j][ip] << endl;
	jfr_.pjdrbg[j][ip] = deltaR(jfr_.peta[ip],jfr_.pphi[ip],jfr_.jteta[j],jfr_.jtphi[j]+TMath::PiOver2());
      }
      Float_t trkEnergy = jfr_.ppt[ip];
      Float_t trkEta = jfr_.peta[ip];
      // Find closest jet
      Float_t jetPt=jfr_.jtpt[0];
      if (jfr_.pjdr[1][ip]<jfr_.pjdr[0][ip]) jetPt=jfr_.jtpt[1];
      //if (trkEnergy>10) cout << "ppt|eta|phi: " << trkEnergy << "|" << trkEta << "|" << jfr_.pphi[ip] << " closest jet: " << jetPt << endl;
      // Correction for this trk
      Double_t corr[4];
      Double_t eff=1,fak=1,mul=1,sec=1,trkwt=1;
      if (anaTrkType_>=2) {
	trkCorr_->GetCorr(trkEnergy,trkEta,jetPt,jfr_.cent,corr);
	//trkCorr_->GetCorr(trkEnergy,trkEta,50,jttrk_.cent,corr);
	eff = corr[0];
	fak = corr[1];
	mul = corr[2];
	sec = corr[3];
	if (eff<1e-5) { eff=1; }
	trkwt = (1-fak)*(1-sec)/(eff*(1+mul));
	//if (trkwt<0||trkwt>20) {
	//  cout << trkEnergy << " " << eff << " " << fak << " " << mul << " " << sec << " " << trkwt << endl;
	//}
      }
      // Fill
      for (Int_t j=0; j<2; ++j) {
	if (!cut_->doSel||(SelEvt(jfr_)&&SelJet(jfr_,j)&&SelFrag(jfr_,ip,j))) {
	  vhPPtCorr_[j][0]->Fill(trkEnergy);
	  vhXi_[j][0]->Fill(log(jfr_.jtpt[j]/trkEnergy));
	  vhPDR_[j][0]->Fill(jfr_.pjdr[j][ip]);
	  if (anaTrkType_==0) continue;
	  vhPPtCorr_[j][1]->Fill(trkEnergy,1./eff);
	  vhPPtCorr_[j][2]->Fill(trkEnergy,(1-fak)/eff);
	  vhPPtCorr_[j][3]->Fill(trkEnergy,(1-fak)/(eff*(1+mul)));
	  vhPPtCorr_[j][4]->Fill(trkEnergy,trkwt);
	  vhXi_[j][4]->Fill(log(jfr_.jtpt[j]/trkEnergy),trkwt);
	  vhPDR_[j][4]->Fill(jfr_.pjdr[j][ip],trkwt);
	  // study correction
	  for (Int_t lv=1; lv<=4; ++lv) {
	    vhTrkCorrPPt_[j][lv]->Fill(trkEnergy,corr[lv-1]);
	  }
	}
      }
    } // end of particle loop
    //for (Int_t ip=0; ip<jfr_.np; ++ip) {
    //  nt_->Fill(jfr_.pjdr[0][ip],jfr_.pjdr[1][ip]);
    //}
    tjfr_->Fill();
  } // end of main loop

  // Normalize
  for (Int_t j=0; j<2; ++j) {
    cout << "jet " << j << " count: " << numJet_[j] << endl;
    for (Int_t lv=0; lv<5; ++lv) {
      if (anaTrkType_==0 && lv>0) continue;
      normHist(vhPPtCorr_[j][lv],0,true,1./numJet_[j]);
      normHist(vhXi_[j][lv],0,true,1./numJet_[j]);
      normHist(vhPDR_[j][lv],0,true,1./numJet_[j]);
    }
  }
}
