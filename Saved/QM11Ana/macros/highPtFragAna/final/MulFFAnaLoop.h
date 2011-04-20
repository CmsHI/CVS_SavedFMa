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
#include "../../helpers.h"
using namespace std;
const Int_t MAXNP = 100000;

struct JetFragRel
{
  Int_t cbin;
  Float_t jtpt[2];
  Float_t jteta[2];
  Float_t jtphi[2];
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  // relations
  Float_t jdphi;
  Float_t clppt[2];
  Int_t clpind[2];
  Float_t cltrkwt[2];
  Float_t cpptsum[2];
  Int_t cnp[2];
  Float_t pjdr[2][MAXNP];
  Float_t pjdrbg[2][MAXNP];
  // corr
  Float_t trkeff[MAXNP];
  Float_t trkfak[MAXNP];
  Float_t trkwt[MAXNP];
  void SetBranches(TTree * t) {
    t->Branch("cbin",&(this->cbin),"cbin/I");
    t->Branch("jtpt",this->jtpt,"jtpt[2]/F");
    t->Branch("jteta",this->jteta,"jteta[2]/F");
    t->Branch("jtphi",this->jtphi,"jtphi[2]/F");
    t->Branch("np",&(this->np),"np/I");
    t->Branch("ppt",this->ppt,"ppt[np]/F");
    t->Branch("peta",this->peta,"peta[np]/F");
    t->Branch("pphi",this->pphi,"pphi[np]/F");
    t->Branch("jdphi",&(this->jdphi),"jdphi/F");
    t->Branch("clppt",this->clppt,"clppt[2]/F");
    t->Branch("clpind",this->clpind,"clpind[2]/I");
    t->Branch("cltrkwt",this->cltrkwt,"cltrkwt[2]/F");
    t->Branch("cpptsum",this->cpptsum,"cpptsum[2]/F");
    t->Branch("cnp",this->cnp,"cnp[2]/I");
    t->Branch("pj0dr",(this->pjdr)[0],"pj0dr[np]/F");
    t->Branch("pj1dr",(this->pjdr)[1],"pj1dr[np]/F");
    t->Branch("trkeff",this->trkeff,"trkeff[np]/F");
    t->Branch("trkfak",this->trkfak,"trkfak[np]/F");
    t->Branch("trkwt",this->trkwt,"trkwt[np]/F");
  }
  void LoadBranches(TTree * t) {
    t->SetBranchAddress("cbin",&(this->cbin));
    t->SetBranchAddress("jtpt",this->jtpt);
    //t->SetBranchAddress("refpt",this->jtpt);
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
    vector<TH1D*> vhJetAj_;
    vector<vector<TH1D*> > vhPPtCorr_;
    vector<vector<TH1D*> > vhPPtRat_;
    vector<vector<TH1D*> > vhXiCorr_;
    vector<vector<TH1D*> > vhXiRat_;

    // monitor histograms
    vector<vector<TH2D*> > vhTrkCorrPPt_;
    vector<vector<TH2D*> > vhTrkCorrJEt_;
    vector<vector<TH2D*> > vhTrkCorrCent_;

    // Methods
    FragAnaLoop(TString name);
    void Init();
    void Loop();

    Bool_t SelEvt(const JetFragRel & jfr) {
      //return (jfr.cbin>=cut_->CentMin && jfr.cbin<cut_->CentMax);
      return (
	  jfr.cbin>=cut_->CentMin && jfr.cbin<cut_->CentMax
	  && jfr.jtpt[0]>50
	  && (jfr.jtpt[0]-jfr.jtpt[1])/(jfr.jtpt[0]+jfr.jtpt[1]) >= cut_->AjMin
	  && (jfr.jtpt[0]-jfr.jtpt[1])/(jfr.jtpt[0]+jfr.jtpt[1]) < cut_->AjMax
	  );
    }
    Bool_t SelJet(const JetFragRel & jfr, Int_t j) {
      return (jfr.jtpt[0]>=cut_->JEtMin[0] && jfr.jtpt[0]<cut_->JEtMax[0] && fabs(jfr.jteta[0])<cut_->JEtaMax[0]
	  &&  jfr.jtpt[1]>=cut_->JEtMin[1] && jfr.jtpt[1]<cut_->JEtMax[1] && fabs(jfr.jteta[1])<cut_->JEtaMax[1]
	  && fabs(jfr.jdphi)>cut_->JDPhiMin);
      /*
      if (j==0
	  && jfr.jtpt[j]>=cut_->JEtMin[j] && jfr.jtpt[j]<cut_->JEtMax[j] && fabs(jfr.jteta[j])<cut_->JEtaMax[j]
	 ) return true;
      else if (j==1
	  && jfr.jtpt[0]>=cut_->JEtMin[0] && jfr.jtpt[0]<cut_->JEtMax[0] && fabs(jfr.jteta[0])<cut_->JEtaMax[0]
	  && jfr.jtpt[j]>=cut_->JEtMin[j] && jfr.jtpt[j]<cut_->JEtMax[j] && fabs(jfr.jteta[j])<cut_->JEtaMax[j]
	 ) return true;
      return false;
	*/
    }
    Bool_t SelFrag(const JetFragRel & jfr, Int_t ip, Int_t j) {
	return ( jfr.ppt[ip]>cut_->TrkPtMin && jfr.pjdr[j][ip]<cut_->ConeSize);
    }
};

FragAnaLoop::FragAnaLoop(TString name) :
  name_(name),
  vtrkCorr_(2),
  anaTrkType_(2),
  numJet_(2),
  vhPPtCorr_(2),
  vhPPtRat_(2),
  vhXiCorr_(2),
  vhXiRat_(2),
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
  const Int_t numEffBins = 50;
  Double_t effBins[numEffBins+1];
  for (Int_t i=0;i<numEffBins+1;++i) {
    effBins[i] = -0.2+i*1.4/numEffBins;
  }
  for (Int_t j=0; j<2; ++j) {
    vhJetAj_.push_back(new TH1D(Form("h%sJetAj_j%d",name_.Data(),j),";A_{J};",20,0,1));
    for (Int_t lv=0; lv<5; ++lv) {
      if (anaTrkType_==0 && lv>0) continue;
      vhPPtCorr_[j].push_back(new TH1D(Form("h%sPPtCorr%d_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",ptBin_.size()-1,&ptBin_[0]));
      vhPPtCorr_[j][lv]->Sumw2();
      vhPPtRat_[j].push_back(new TH1D(Form("h%sPPtRat%d_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",ptBin_.size()-1,&ptBin_[0]));
      vhPPtRat_[j][lv]->Sumw2();

      vhXiCorr_[j].push_back(new TH1D(Form("h%sXiCorr%d_j%d",name_.Data(),lv,j),"",10,0,5));
      vhXiCorr_[j][lv]->Sumw2();
      vhXiRat_[j].push_back(new TH1D(Form("h%sXiRat%d_j%d",name_.Data(),lv,j),"",10,0,5));
      vhXiRat_[j][lv]->Sumw2();

      vhTrkCorrPPt_[j].push_back(new TH2D(Form("h%sTrkCorr%dPPt_j%d",name_.Data(),lv,j),";p_{T} (GeV/c);",ptBin_.size()-1,&ptBin_[0],numEffBins,effBins));
      vhTrkCorrJEt_[j].push_back(new TH2D(Form("h%sTrkCorr%dJEt_j%d",name_.Data(),lv,j),";Jet p_{T} (GeV/c);",100,0,400,50,-0.2,1.2));
      vhTrkCorrCent_[j].push_back(new TH2D(Form("h%sTrkCorr%dCent_j%d",name_.Data(),lv,j),";Centrality (%);",10,0,40,50,-0.2,1.2));
    }
  }

  // Book ntuples
  tout_ = new TTree("tjfr"+name_,"jet-trk relations");
  jfr_.SetBranches(tout_);
  tout_->SetAlias("Aj","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])");
  tout_->SetAlias("dPt","(jtpt[0]-jtpt[1])");
}

void FragAnaLoop::Loop()
{
  // Initialize event level counters
  for (Int_t j=0; j<2; ++j) {
    numJet_[j]=0;
  }

  Int_t numEntries = t_->GetEntries();
  cout << t_->GetName() << ": " << numEntries << endl;
  // ===========================
  // Event Loop
  // ===========================
  for (Int_t i=0; i<numEntries; ++i) {
    t_->GetEntry(i);
    if (i%5000==0) cout << "Entry " << i << " (" << (Float_t)i/numEntries*100 << "%) cbin: " << jfr_.cbin << " jet0: " << jfr_.jtpt[0] << " " << jfr_.jteta[0] << " " << jfr_.jtphi[0] << endl;
    // Calc Dijet properties
    jfr_.jdphi = deltaPhi(jfr_.jtphi[0],jfr_.jtphi[1]);
    // ===========================
    // Initialize variables
    // ===========================
    for (Int_t j=0; j<2; ++j) {
      jfr_.clppt[j] = -99;
      jfr_.clpind[j] = -99;
      jfr_.cltrkwt[j] = 1;
      jfr_.cpptsum[j] = 0;
      jfr_.cnp[j] = 0;
    }

    // ===========================
    // Basic Event preselection
    // ===========================
    if (!SelEvt(jfr_)) continue;

    // ===========================
    // jet count
    // ===========================
    for (Int_t j=0; j<2; ++j) {
      if (SelEvt(jfr_)&&SelJet(jfr_,j)) {
	++numJet_[j];
	vhJetAj_[j]->Fill((jfr_.jtpt[0]-jfr_.jtpt[1])/(jfr_.jtpt[0]+jfr_.jtpt[1]));
      }
    }

    for (Int_t ip=0; ip<jfr_.np; ++ip) {
      Float_t trkEnergy = jfr_.ppt[ip];
      Float_t trkEta = jfr_.peta[ip];
      // basic trk kinematic cuts
      if (trkEnergy<cut_->TrkPtMin) continue;
      if (fabs(trkEta)>2.4) continue;

      // Correction for this trk
      Double_t corr[4];
      Double_t eff=1,fak=1,mul=1,sec=1,trkwt=1;
      // Fill
      for (Int_t j=0; j<2; ++j) {
	// =======================
	// Calc jet-particle relations
	// =======================
	jfr_.pjdr[j][ip] = deltaR(jfr_.peta[ip],jfr_.pphi[ip],jfr_.jteta[j],jfr_.jtphi[j]);
	//cout << "jet " << j << " dr: " << jfr_.pjdr[j][ip] << endl;
	jfr_.pjdrbg[j][ip] = deltaR(jfr_.peta[ip],jfr_.pphi[ip],-1*jfr_.jteta[j],jfr_.jtphi[j]);
	//cout << (*jfr_.pdr)[j][ip] << endl;
	// =======================
	// Get Corrections
	// =======================
	if (anaTrkType_>=2&&jfr_.pjdr[j][ip]<cut_->ConeSize) {
	  trkwt = vtrkCorr_[j]->GetCorr(trkEnergy,trkEta,jfr_.jtpt[j],jfr_.cbin,corr);
	  eff = corr[0];
	  fak = corr[1];
	  mul = corr[2];
	  sec = corr[3];
	  jfr_.trkeff[ip] = eff;
	  jfr_.trkfak[ip] = fak;
	  //if (eff<1e-5) { eff=1; }
	  //trkwt = (1-fak)*(1-sec)/(eff*(1+mul));
	  jfr_.trkwt[ip] = trkwt;
	}
	// Cone ana
	if (jfr_.pjdr[j][ip]<cut_->ConeSize) {
	  // leading particle in cone
	  if (trkEnergy>jfr_.clppt[j]) {
	    jfr_.clppt[j]=trkEnergy;
	    jfr_.clpind[j]=ip;
	    jfr_.cltrkwt[j] = trkwt;
	  }
	  // cone sum properties
	  if (trkEnergy>4) {
	    jfr_.cpptsum[j]+=trkEnergy*trkwt;
	    jfr_.cnp[j]+=trkwt;
	  }
	}
	// =======================
	// Histogram Ana
	// =======================
	if (SelEvt(jfr_)&&SelJet(jfr_,j)&&SelFrag(jfr_,ip,j)) {
	  if (j==1) {
	    if (trkEnergy>20) {
	      //cout << jfr_.pjdr[j][ip];
	      //cout << " jet pt|eta|phi: " << jfr_.jtpt[j] << "|" << jfr_.jteta[j] << "|" << jfr_.jtphi[j];
	      //cout << " p pt|eta|phi: " << jfr_.ppt[ip] << "|" << jfr_.peta[ip] << "|" << jfr_.pphi[ip] << endl;
	    }
	  }
	  vhPPtCorr_[j][0]->Fill(trkEnergy);
	  vhXiCorr_[j][0]->Fill(log(jfr_.jtpt[j]/trkEnergy));

	  if (anaTrkType_==0) continue;
	  vhPPtCorr_[j][1]->Fill(trkEnergy,1./eff);
	  vhPPtCorr_[j][2]->Fill(trkEnergy,(1-fak)/eff);
	  vhPPtCorr_[j][3]->Fill(trkEnergy,(1-fak)/(eff*(1+mul)));
	  vhPPtCorr_[j][4]->Fill(trkEnergy,trkwt);

	  vhXiCorr_[j][1]->Fill(log(jfr_.jtpt[j]/trkEnergy),1./eff);
	  vhXiCorr_[j][2]->Fill(log(jfr_.jtpt[j]/trkEnergy),(1-fak)/eff);
	  vhXiCorr_[j][3]->Fill(log(jfr_.jtpt[j]/trkEnergy),(1-fak)/(eff*(1+mul)));
	  vhXiCorr_[j][4]->Fill(log(jfr_.jtpt[j]/trkEnergy),trkwt);

	  // study correction
	  for (Int_t lv=1; lv<=4; ++lv) {
	    vhTrkCorrPPt_[j][lv]->Fill(trkEnergy,corr[lv-1]);
	    vhTrkCorrJEt_[j][lv]->Fill(jfr_.jtpt[j],corr[lv-1]);
	    vhTrkCorrCent_[j][lv]->Fill(jfr_.cbin,corr[lv-1]);
	  }
	}
      }
    }
    if (!cut_->doSel||(SelEvt(jfr_)&&SelJet(jfr_,1))) {
      tout_->Fill();
    }
  } // end of main loop

  // Normalize
  for (Int_t j=0; j<2; ++j) {
    cout << "jet " << j << " count: " << numJet_[j] << endl;
    for (Int_t lv=0; lv<5; ++lv) {
      if (anaTrkType_==0 && lv>0) continue;
      normHist(vhPPtCorr_[j][lv],0,true,1./numJet_[j]);
      normHist(vhXiCorr_[j][lv],0,true,1./numJet_[j]);
    }
  }
}
