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
#include "selectionCut.h"
const Float_t PI = 3.1415926535897932384626;
const Float_t HPI = PI/2.;
const Int_t kMax = 2;
const Int_t MAXTRK = 100000;

class JetFragAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   selectionCut    cut;

   // Loop Ana Vars
   Bool_t doEvtSel_;
   Bool_t doEtaCorr_;
   Bool_t doTrackingEffFakeCorr_;
   Bool_t doJetOnly_;
   Int_t anaGenpType_;
   Int_t numDJ_;
   std::vector<math::PtEtaPhiMLorentzVector> anaJets_;
   std::vector<math::PtEtaPhiMLorentzVector> refJets_;
   std::vector<math::PtEtaPhiMLorentzVector> particles_;

   // Corrections
   std::map<TString,TF1*> jetaCorr_;
   TH2D* trackingEffCorr_[10];
   TH2D* trackingFakeCorr_[10];
   TH1D* trackingPtBin_;
   TH1D* trackingEtaBin_;

   // Ntuples
   TNtuple * ntjt;

   // Histograms
   // jet
   TH1D * hJDPhi;
   TH1D * hJEtNr;
   TH1D * hJEtAw;
   TH1D * hAj;
   TH1D * hJEtaNr;
   TH1D * hJEtaAw;
   TH1D * hJDEta;
   TH1D * hRefJEtNr;
   TH1D * hRefJEtAw;
   // cone
   TH1D * hNrCPt;
   TH1D * hNrCPtBg;
   TH1D * hNrCPtBgSub;
   TH1D * hAwCPt;
   TH1D * hAwCPtBg;
   TH1D * hAwCPtBgSub;
   // trk
   TH1D * hNrCPPt;
   TH1D * hNrCPPtBg;
   TH1D * hNrCPPtBgSub;
   TH1D * hAwCPPt;
   TH1D * hAwCPPtBg;
   TH1D * hAwCPPtBgSub;
   TH1D * hPNDR;
   TH1D * hPADR;
   TH1D * hPNDRBg;
   TH1D * hPADRBg;
   TH2D * hPtPNDR;
   TH2D * hPtPADR;
   TH2D * hPtPNDRBg;
   TH2D * hPtPADRBg;

   TH1D * hPNDRDens;
   TH1D * hPADRDens;
   TH1D * hPNDRDensBg;
   TH1D * hPADRDensBg;
   TH2D * hPtPNDRDens;
   TH2D * hPtPADRDens;
   TH2D * hPtPNDRDensBg;
   TH2D * hPtPADRDensBg;

   TH1D * hPNDPhi;
   TH1D * hPADPhi;
   TH1D * hPNDPhiBg;
   TH1D * hPADPhiBg;
   TH2D * hPtPNDPhi;
   TH2D * hPtPADPhi;
   TH2D * hPtPNDPhiBg;
   TH2D * hPtPADPhiBg;

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
   Float_t         djmass;
   Float_t         rdjmass;
   Float_t         cmeta;
   Int_t           nlrjid;
   Int_t           nlrjstat;
   Float_t         nlrjet;
   Float_t         nlrjeta;
   Float_t         nlrjphi;
   Int_t           alrjid;
   Int_t           alrjstat;
   Float_t         alrjet;
   Float_t         alrjeta;
   Float_t         alrjphi;
   Float_t         rjdphi;
   Float_t         nljet;
   Float_t         nljrawet;
   Float_t         nljeta;
   Float_t         nljphi;
   Float_t         nljarea;
   Float_t         aljet;
   Float_t         aljrawet;
   Float_t         aljeta;
   Float_t         aljphi;
   Float_t         aljarea;
   Float_t         nljemf;
   Int_t           nljN90hits;
   Float_t         nljfhpd;
   Float_t         aljemf;
   Int_t           aljN90hits;
   Float_t         aljfhpd;
   Float_t         jdphi;
   Int_t           numJec;
   Float_t         njec[15];   //[numJec]
   Float_t         ajec[15];   //[numJec]
   Int_t           evtnp;
   Int_t           psube[MAXTRK];   //[evtnp]
   Float_t         ppt[MAXTRK];   //[evtnp]
   Float_t         peta[MAXTRK];   //[evtnp]
   Float_t         pphi[MAXTRK];   //[evtnp]
   Int_t	   pch[MAXTRK];
   Int_t	   ppid[MAXTRK];
   Float_t         pndphi[MAXTRK];   //[evtnp]
   Float_t         pndr[MAXTRK];   //[evtnp]
   Float_t         pndrbg[MAXTRK];   //[evtnp]
   Float_t         padphi[MAXTRK];   //[evtnp]
   Float_t         padr[MAXTRK];   //[evtnp]
   Float_t         padrbg[MAXTRK];   //[evtnp]
   Int_t           lp_;
   Double_t        lp_fCoordinates_fRho[kMax];   //[_]
   Double_t        lp_fCoordinates_fEta[kMax];   //[_]
   Double_t        lp_fCoordinates_fPhi[kMax];   //[_]
   Int_t           trkNHits[MAXTRK];   //[evtnp]
   Float_t         trkdz[MAXTRK];   //[evtnp]
   Float_t         trkdze[MAXTRK];   //[evtnp]
   Float_t         trkd0[MAXTRK];   //[evtnp]
   Float_t         trkd0e[MAXTRK];   //[evtnp]
   vector<int>     *ljcnp;
   vector<int>     *ljcnpbg;
   vector<float>   *ljcpt;
   vector<float>   *ljcpt2;
   vector<float>   *ljcptr;
   vector<float>   *ljcptbg;
   vector<float>   *ljcpt2bg;
   vector<float>   *ljcptrbg;
   vector<float>   *lppt;
   vector<float>   *lpjdr;
   vector<float>   *lgppt;
   vector<float>   *lgpch;
   vector<float>   *lgppid;

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
   TBranch        *b_djmass;   //!
   TBranch        *b_rdjmass;   //!
   TBranch        *b_cmeta;   //!
   TBranch        *b_nlrjid;   //!
   TBranch        *b_nlrjstat;   //!
   TBranch        *b_nlrjet;   //!
   TBranch        *b_nlrjeta;   //!
   TBranch        *b_nlrjphi;   //!
   TBranch        *b_alrjid;   //!
   TBranch        *b_alrjstat;   //!
   TBranch        *b_alrjet;   //!
   TBranch        *b_alrjeta;   //!
   TBranch        *b_alrjphi;   //!
   TBranch        *b_rjdphi;   //!
   TBranch        *b_nljet;   //!
   TBranch        *b_nljrawet;   //!
   TBranch        *b_nljeta;   //!
   TBranch        *b_nljphi;   //!
   TBranch        *b_nljarea;   //!
   TBranch        *b_aljet;   //!
   TBranch        *b_aljrawet;   //!
   TBranch        *b_aljeta;   //!
   TBranch        *b_aljphi;   //!
   TBranch        *b_aljarea;   //!
   TBranch        *b_nljemf;   //!
   TBranch        *b_nljN90hits;   //!
   TBranch        *b_nljfhpd;   //!
   TBranch        *b_aljemf;   //!
   TBranch        *b_aljN90hits;   //!
   TBranch        *b_aljfhpd;   //!
   TBranch        *b_jdphi;   //!
   TBranch        *b_numJec;   //!
   TBranch        *b_njec;   //!
   TBranch        *b_ajec;   //!
   TBranch        *b_evtnp;   //!
   TBranch        *b_psube;   //!
   TBranch        *b_ppt;   //!
   TBranch        *b_peta;   //!
   TBranch        *b_pphi;   //!
   TBranch        *b_pch;   //!
   TBranch        *b_ppid;   //!
   TBranch        *b_pndphi;   //!
   TBranch        *b_pndr;   //!
   TBranch        *b_pndrbg;   //!
   TBranch        *b_padphi;   //!
   TBranch        *b_padr;   //!
   TBranch        *b_padrbg;   //!
   TBranch        *b__;   //!
   TBranch        *b_lp_fCoordinates_fRho;   //!
   TBranch        *b_lp_fCoordinates_fEta;   //!
   TBranch        *b_lp_fCoordinates_fPhi;   //!
   TBranch        *b_trkNHits;   //!
   TBranch        *b_trkdz;   //!
   TBranch        *b_trkdze;   //!
   TBranch        *b_trkd0;   //!
   TBranch        *b_trkd0e;   //!
   TBranch        *b_ljcnp;   //!
   TBranch        *b_ljcnpbg;   //!
   TBranch        *b_ljcpt;   //!
   TBranch        *b_ljcpt2;   //!
   TBranch        *b_ljcptr;   //!
   TBranch        *b_ljcptbg;   //!
   TBranch        *b_ljcpt2bg;   //!
   TBranch        *b_ljcptrbg;   //!
   TBranch        *b_lppt;   //!
   TBranch        *b_lpjdr;   //!
   TBranch        *b_lgppt;   //!
   TBranch        *b_lgpch;   //!
   TBranch        *b_lgppid;   //!

   JetFragAna(TTree *tree=0,TString tag="Data",Int_t doMC=0);
   ~JetFragAna();
   Int_t    Cut(Long64_t entry);
   Int_t    GetEntry(Long64_t entry);
   Long64_t LoadTree(Long64_t entry);
   void	    Init(TTree *tree);
   void     Loop();
   Bool_t   Notify();
   void     Show(Long64_t entry = -1);
   Bool_t   GetEvtMask();
   double   getEffFakeCorrection(double pt,double eta, double cent);
};

#endif
