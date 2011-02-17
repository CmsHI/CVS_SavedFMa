//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Dec  2 08:10:00 2010 by ROOT version 5.22/00d
// from TTree djTree/dijet tree
// found on file: dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root
//////////////////////////////////////////////////////////

#ifndef JetFragAna_h
#define JetFragAna_h

#include <vector>
#include <map>
#include "DataFormats/Math/interface/LorentzVector.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
//#include "TF1.h"
#include "TRandom3.h"
#include "selectionCut.h"
const Float_t PI = 3.1415926535897932384626;
const Float_t HPI = PI/2.;
const Int_t kMax = 2;
const Int_t MAXTRK = 100000;
#ifdef __MAKECINT__
#pragma link C++ class std::vector < std::vector<float> >+;   
#endif

struct JetCone {
  vector<vector<Float_t> > cpt;
  vector<vector<Float_t> > cptbg;
  vector<vector<Float_t> > cptpara;
  vector<vector<Float_t> > cptparabg;
  JetCone() :
    cpt(2),
    cptbg(2),
    cptpara(2),
    cptparabg(2)
  {}
  void clear() {
    for (Int_t i=0; i<(Int_t)cpt.size(); ++i) {
      for (Int_t j=0; j<(Int_t)cpt[i].size(); ++j) {
	cpt[i][j]=0;
	cptbg[i][j]=0;
	cptpara[i][j]=0;
	cptparabg[i][j]=0;
	//cout << "jet " << i << " bin " << j << " cleared cpt: " << cpt[i][j] << " cptbg: " << cptbg[i][j] << endl;
      }
    }
  }
  void resizePtBins(Int_t n) {
    for (Int_t i=0; i<(Int_t)cpt.size(); ++i) {
      cpt[i].resize(n);
      cptbg[i].resize(n);
      cptpara[i].resize(n);
      cptparabg[i].resize(n);
    }
  }
};

struct AnaJet {
  Float_t nljet,nljeta,nljphi,aljet,aljeta,aljphi;
};

class JetFragAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   selectionCut    cut;

   // Loop Ana Vars
   Bool_t doEvtSel_;
   Bool_t doEtaCorr_;
   Bool_t doTrackingEffFakeCorr_;
   Bool_t doCentralityReweighting_;
   Bool_t doJetOnly_;
   Int_t anaGenpType_;
   Int_t numDJ_;
   Int_t numJ_[2];
   Float_t numDJReWeighted_;
   Float_t numJReWeighted_[2];
   std::vector<math::PtEtaPhiMLorentzVector> anaJets_;
   Float_t anaJetDPhi_;
   std::vector<math::PtEtaPhiMLorentzVector> refJets_;
   std::vector<math::PtEtaPhiMLorentzVector> p_;
   JetCone jc_;
   Int_t mixOffset_;
   Int_t jetTreeMode_,particleTreeMode_;
   TChain * jetTree_[10];
   AnaJet vj_[10];
   TRandom * r3;

   // Corrections
   std::map<TString,TF1*> jetaCorr_;
   TH2D *trackingEffCorr_[10];
   TH2D *trackingFakeCorr_[10];
   TH1D *trackingPtBin_;
   TH1D *trackingEtaBin_;

   // Centrality Reweighting
   TH1D *hCentralityData_;
   TH1D *hCentralityWeight_;

   // Ntuples
   TNtuple * ntjt;
   TTree * tcone;

   // Histograms
   // jet
   TH1D * hJDPhi;
   TH1D * hJDEta;
   TH1D * hAj;
   TH1D * hJEt[2];
   TH1D * hJEta[2];
   TH1D * hRefJEt[2];
   // cone
   TH1D * hCPt[2];
   TH1D * hCPtBg[2];
   TH1D * hCPtBgSub[2];
   // trk
   TH1D * hPJDPhi[2];
   TH1D * hCPPt[2];
   TH1D * hCPPtBg[2];
   TH1D * hCPPtBgSub[2];
   TH2D * hPtPDR[2];
   TH2D * hPtPDRBg[2];

   // Event cut for centrality reweighting
   TCut evtCut;

   // Declaration of leaf types
   Int_t           run;
   Int_t           evt;
   Int_t           lumi;
   Bool_t	   evtMask;
   vector<bool>    *hlt;
   Int_t           nvtx;
   Int_t           vtxntrks;
   Float_t         vtxndof;
   Float_t         vtxchi2;
   Float_t         vz;
   Int_t           njets;
   Int_t           ntrks;
   Float_t         b;
   Int_t           npart;
   Int_t           ncoll;
   Float_t         cent;
   Float_t         nljet;
   Float_t         nljeta;
   Float_t         nljphi;
   Float_t         aljet;
   Float_t         aljeta;
   Float_t         aljphi;
   Int_t           evtnp;
   Int_t           psube[MAXTRK];   //[evtnp]
   Float_t         ppt[MAXTRK];   //[evtnp]
   Float_t         peta[MAXTRK];   //[evtnp]
   Float_t         pphi[MAXTRK];   //[evtnp]
   Int_t	   pch[MAXTRK];
   Int_t	   ppid[MAXTRK];
   Int_t	   pfid[MAXTRK];

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_hlt;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_vtxntrks;   //!
   TBranch        *b_vtxndof;   //!
   TBranch        *b_vtxchi2;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_njets;   //!
   TBranch        *b_ntrks;   //!
   TBranch        *b_b;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_ncoll;   //!
   TBranch        *b_cent;   //!
   TBranch        *b_nljet;   //!
   TBranch        *b_nljeta;   //!
   TBranch        *b_nljphi;   //!
   TBranch        *b_aljet;   //!
   TBranch        *b_aljeta;   //!
   TBranch        *b_aljphi;   //!
   TBranch        *b_evtnp;   //!
   TBranch        *b_psube;   //!
   TBranch        *b_ppt;   //!
   TBranch        *b_peta;   //!
   TBranch        *b_pphi;   //!
   TBranch        *b_pch;   //!
   TBranch        *b_ppid;   //!
   TBranch        *b_pfid;   //!

   JetFragAna(TTree *tree=0,TString tag="Data",Int_t doMC=0);
   ~JetFragAna();
   Int_t    Cut(Long64_t entry);
   Int_t    GetEntry(Long64_t entry);
   Int_t    GetJetEntry(TChain * t, AnaJet & jet, Long64_t entry);
   Long64_t LoadTree(Long64_t entry);
   void	    Init(TTree *tree);
   void     Loop();
   Bool_t   Notify();
   void     Show(Long64_t entry = -1);
   Bool_t   GetEvtMask();
   double   getEffFakeCorrection(double pt,double eta, double cent);
   void	    SetJetTree(Int_t mode, TString tfile, TString tname);
};

#endif
