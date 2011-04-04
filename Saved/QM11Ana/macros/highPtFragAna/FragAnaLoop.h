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
  void SetBranches(TTree * t) {
    t->Branch("cent",&(this->cent),"cent/F");
    t->Branch("jtpt",this->jtpt,"jtpt[2]/F");
    t->Branch("jteta",this->jteta,"jteta[2]/F");
    t->Branch("jtphi",this->jtphi,"jtphi[2]/F");
    t->Branch("np",&(this->np),"np/I");
    t->Branch("ppt",this->ppt,"ppt[np]/F");
    t->Branch("peta",this->peta,"peta[np]/F");
    t->Branch("pphi",this->pphi,"pphi[np]/F");
    t->Branch("jdphi",&(this->jdphi),"jdphi/F");
    t->Branch("pj0dr",(this->pjdr)[0],"pj0dr[np]/F");
    t->Branch("pj1dr",(this->pjdr)[1],"pj1dr[np]/F");
  }
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
class FragAnaLoop
{
  public:
    TChain * t_;
    JetFragRel jfr_;
    Corrector3D * trkCorr_;
    vector<Corrector3D *> vtrkCorr_;
    selectionCut * cut_;
    TTree * tout_;

    TString name_;
    Double_t ptHatMin_;
    Int_t anaTrkType_;

    vector<Double_t> numJet_;

    // analysis histograms
    vector<Double_t> ptBin_;
    vector<vector<TH1D*> > vhPPtCorr_;
    vector<vector<TH1D*> > vhPPtRat_;

    // monitor histograms
    vector<vector<TH2D*> > vhTrkCorrPPt_;
    vector<vector<TH2D*> > vhTrkCorrJEt_;
    vector<vector<TH2D*> > vhTrkCorrCent_;

    // ntuples
    TNtuple * ntCorr_;

    // Methods
    FragAnaLoop(TString name);
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
	  && jfr.jtpt[0]>=cut_->JEtMin[0] && jfr.jtpt[0]<cut_->JEtMax[0] && fabs(jfr.jteta[0])<cut_->JEtaMax[0]
	  && jfr.jtpt[j]>=cut_->JEtMin[j] && jfr.jtpt[j]<cut_->JEtMax[j] && fabs(jfr.jteta[j])<cut_->JEtaMax[j]
	 ) return true;
      return false;
    }
    Bool_t SelFrag(const JetFragRel & jfr, Int_t ip, Int_t j) {
	//return ( fabs(jfr.peta[ip])<cut_->TrkEtaMax );
	return ( jfr.pjdr[j][ip]<cut_->ConeSize && jfr.ppt[ip]>3);
    }
};

FragAnaLoop::FragAnaLoop(TString name) :
  name_(name),
  vtrkCorr_(2),
  anaTrkType_(2),
  numJet_(2),
  vhPPtCorr_(2),
  vhPPtRat_(2),
  vhTrkCorrPPt_(2),
  vhTrkCorrJEt_(2),
  vhTrkCorrCent_(2)
{
}

void FragAnaLoop::Init()
{
  cout << "================ Ana Loop ==================" << endl;
  cout << " anaTrkType: " << anaTrkType_ << endl;
  cout << " LeadingJet: " << cut_->JEtMin[0] << "," << cut_->JEtMax[0] << " SubLeadingJet: " << cut_->JEtMin[1] << " " << cut_->JEtMax[1] << endl;
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

      vhTrkCorrPPt_[j].push_back(new TH2D(Form("h%sTrkCorr%dPPt_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",50,0,100,50,-0.2,1.2));
      vhTrkCorrJEt_[j].push_back(new TH2D(Form("h%sTrkCorr%dJEt_j%d",name_.Data(),lv,j),";Jet p_{T} (GeV/c);",100,0,400,50,-0.2,1.2));
      vhTrkCorrCent_[j].push_back(new TH2D(Form("h%sTrkCorr%dCent_j%d",name_.Data(),lv,j),";Centrality (%);",10,0,100,50,-0.2,1.2));
    }
  }

  // Book ntuples
  ntCorr_ = new TNtuple("ntCorr","nt of trk corrections",
      "cent:pthatmin:"
      "jtpt1:jteta1:jtpt2:jteta2:"
      "ppt:peta:pj1dr:pj2dr:"
      "eff:fak");
  tout_ = new TTree("tjfr"+name_,"jet-trk relations");
  jfr_.SetBranches(tout_);
}

void FragAnaLoop::Loop()
{
  // Initialize counters
  for (Int_t j=0; j<2; ++j) {
    numJet_[j]=0;
  }

  // Main Loop
  Int_t numEntries = t_->GetEntries();
  cout << t_->GetName() << ": " << numEntries << endl;
  for (Int_t i=0; i<numEntries; ++i) {
    t_->GetEntry(i);
    // Calc Dijet properties
    jfr_.jdphi = deltaPhi(jfr_.jtphi[0],jfr_.jtphi[1]);
    // ===========================
    // Basic Event preselection
    // ===========================
    if (jfr_.jtpt[0]<50) continue;

    for (Int_t j=0; j<2; ++j) {
      if (!cut_->doSel||(SelEvt(jfr_)&&SelJet(jfr_,j))) {
	++numJet_[j];
	//cout << jfr_.jtpt[0] << " " << jfr_.jteta[0] << " " << jfr_.jtphi[0] << endl;
      }
    }

    for (Int_t ip=0; ip<jfr_.np; ++ip) {
      Float_t trkEnergy = jfr_.ppt[ip];
      Float_t trkEta = jfr_.peta[ip];
      // Correction for this trk
      Double_t corr[4];
      Double_t eff=1,fak=1,mul=1,sec=1,trkwt=1;
      /*
      if (anaTrkType_>=2) {
	trkCorr_->GetCorr(trkEnergy,trkEta,(*jfr_.jtpt)[0],jfr_.cent,corr);
	//trkCorr_->GetCorr(trkEnergy,trkEta,50,jfr_.cent,corr);
	eff = corr[0];
	fak = corr[1];
	mul = corr[2];
	sec = corr[3];
	ntCorr_->Fill(jfr_.cent,ptHatMin_,
	    (*jfr_.jtpt)[0],(*jfr_.jteta)[0],(*jfr_.jtpt)[1],(*jfr_.jteta)[1],
	    trkEnergy,trkEta,(*jfr_.pdr)[0][ip],(*jfr_.pdr)[1][ip],
	    eff,fak
	    );
	if (eff<1e-5) { eff=1; }
	trkwt = (1-fak)*(1-sec)/(eff*(1+mul));
	//if (trkwt<0||trkwt>20) {
	//  cout << trkEnergy << " " << eff << " " << fak << " " << mul << " " << sec << " " << trkwt << endl;
	//}
      }
      */
      // Fill
      for (Int_t j=0; j<2; ++j) {
	// =======================
	// Calc jet-particle relations
	// =======================
	jfr_.pjdr[j][ip] = deltaR(jfr_.peta[ip],jfr_.pphi[ip],jfr_.jteta[j],jfr_.jtphi[j]);
	//cout << "jet " << j << " dr: " << jfr_.pjdr[j][ip] << endl;
	jfr_.pjdrbg[j][ip] = deltaR(jfr_.peta[ip],jfr_.pphi[ip],jfr_.jteta[j],jfr_.jtphi[j]+TMath::PiOver2());
	// =======================
	// Get Corrections
	// =======================
	//cout << (*jfr_.pdr)[j][ip] << endl;
	if (!cut_->doSel||(SelEvt(jfr_)&&SelJet(jfr_,j)&&SelFrag(jfr_,ip,j))) {
	  if (anaTrkType_>=2) {
	    vtrkCorr_[j]->GetCorr(trkEnergy,trkEta,jfr_.jtpt[j],jfr_.cent,corr);
	    eff = corr[0];
	    fak = corr[1];
	    mul = corr[2];
	    sec = corr[3];
	    if (eff<1e-5) { eff=1; }
	    trkwt = (1-fak)*(1-sec)/(eff*(1+mul));
	  }
	  // fill
	  vhPPtCorr_[j][0]->Fill(trkEnergy);
	  if (anaTrkType_==0) continue;
	  vhPPtCorr_[j][1]->Fill(trkEnergy,1./eff);
	  vhPPtCorr_[j][2]->Fill(trkEnergy,(1-fak)/eff);
	  vhPPtCorr_[j][3]->Fill(trkEnergy,(1-fak)/(eff*(1+mul)));
	  vhPPtCorr_[j][4]->Fill(trkEnergy,trkwt);
	  // study correction
	  for (Int_t lv=1; lv<=4; ++lv) {
	    vhTrkCorrPPt_[j][lv]->Fill(trkEnergy,corr[lv-1]);
	    vhTrkCorrJEt_[j][lv]->Fill(jfr_.jtpt[j],corr[lv-1]);
	    vhTrkCorrCent_[j][lv]->Fill(jfr_.cent,corr[lv-1]);
	  }
	}
      }
    }
    if (!cut_->doSel||(SelEvt(jfr_))) {
      tout_->Fill();
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
