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
//#include "TF1.h"
#include "Saved/DiJetAna/macros/selectionCut.h"
const Float_t PI = 3.1415926535897932384626;
const Float_t HPI = PI/2.;
const Int_t kMax = 2;
const Int_t MAXTRK = 100000;
const Int_t numPtBins = 7;
//Double_t ptBins[numPtBins+1]={0.2,1,2,4,8,16,64,200};
Double_t ptBins[numPtBins+1]={1.5,3,6,9,18,36,72,200};
const Int_t numDRBins = 10;
//Double_t dRBins[numDRBins+1]={0,0.2,0.4,0.6,0.8,1.,1.2,1.4,1.6,1.8,2.};
Double_t dRBins[numDRBins+1]={0,0.1*HPI,0.2*HPI,0.3*HPI,0.4*HPI,0.5*HPI,0.6*HPI,0.7*HPI,0.8*HPI,0.9*HPI,HPI};
const Int_t numDPhiBins = 10;
Double_t dPhiBins[numDPhiBins+1]={0,0.1*HPI,0.2*HPI,0.3*HPI,0.4*HPI,0.5*HPI,0.6*HPI,0.7*HPI,0.8*HPI,0.9*HPI,HPI};

class JetFragAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   selectionCut    cut;

   // Loop Ana Vars
   Bool_t doEtaCorr_;
   Int_t numDJ_;
   std::vector<math::PtEtaPhiMLorentzVector> anaJets_;
   std::vector<math::PtEtaPhiMLorentzVector> refJets_;
   std::vector<math::PtEtaPhiMLorentzVector> particles_;

   // Corrections
   std::map<TString,TF1*> jetaCorr_;

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
   TH1D * hCNPNr;
   TH1D * hCNPBgNr;
   TH1D * hCNPSubNr;
   TH1D * hCNPAw;
   TH1D * hCNPBgAw;
   TH1D * hCNPSubAw;
   // trk
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
   virtual ~JetFragAna();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef JetFragAna_cxx
JetFragAna::~JetFragAna()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Long64_t JetFragAna::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void JetFragAna::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   hlt = 0;
   ljcnp = 0;
   ljcnpbg = 0;
   ljcpt = 0;
   ljcpt2 = 0;
   ljcptr = 0;
   ljcptbg = 0;
   ljcpt2bg = 0;
   ljcptrbg = 0;
   lppt = 0;
   lpjdr = 0;
   lgppt = 0;
   lgpch = 0;
   lgppid = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("hlt", &hlt, &b_hlt);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("vtxntrks", &vtxntrks, &b_vtxntrks);
   fChain->SetBranchAddress("vtxndof", &vtxndof, &b_vtxndof);
   fChain->SetBranchAddress("vtxchi2", &vtxchi2, &b_vtxchi2);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("njets", &njets, &b_njets);
   fChain->SetBranchAddress("ntrks", &ntrks, &b_ntrks);
   fChain->SetBranchAddress("b", &b, &b_b);
   fChain->SetBranchAddress("npart", &npart, &b_npart);
   fChain->SetBranchAddress("ncoll", &ncoll, &b_ncoll);
   fChain->SetBranchAddress("cent", &cent, &b_cent);
   fChain->SetBranchAddress("djmass", &djmass, &b_djmass);
   fChain->SetBranchAddress("rdjmass", &rdjmass, &b_rdjmass);
   fChain->SetBranchAddress("cmeta", &cmeta, &b_cmeta);
   fChain->SetBranchAddress("nlrjid", &nlrjid, &b_nlrjid);
   fChain->SetBranchAddress("nlrjstat", &nlrjstat, &b_nlrjstat);
   fChain->SetBranchAddress("nlrjet", &nlrjet, &b_nlrjet);
   fChain->SetBranchAddress("nlrjeta", &nlrjeta, &b_nlrjeta);
   fChain->SetBranchAddress("nlrjphi", &nlrjphi, &b_nlrjphi);
   fChain->SetBranchAddress("alrjid", &alrjid, &b_alrjid);
   fChain->SetBranchAddress("alrjstat", &alrjstat, &b_alrjstat);
   fChain->SetBranchAddress("alrjet", &alrjet, &b_alrjet);
   fChain->SetBranchAddress("alrjeta", &alrjeta, &b_alrjeta);
   fChain->SetBranchAddress("alrjphi", &alrjphi, &b_alrjphi);
   fChain->SetBranchAddress("rjdphi", &rjdphi, &b_rjdphi);
   fChain->SetBranchAddress("nljet", &nljet, &b_nljet);
   fChain->SetBranchAddress("nljrawet", &nljrawet, &b_nljrawet);
   fChain->SetBranchAddress("nljeta", &nljeta, &b_nljeta);
   fChain->SetBranchAddress("nljphi", &nljphi, &b_nljphi);
   fChain->SetBranchAddress("nljarea", &nljarea, &b_nljarea);
   fChain->SetBranchAddress("aljet", &aljet, &b_aljet);
   fChain->SetBranchAddress("aljrawet", &aljrawet, &b_aljrawet);
   fChain->SetBranchAddress("aljeta", &aljeta, &b_aljeta);
   fChain->SetBranchAddress("aljphi", &aljphi, &b_aljphi);
   fChain->SetBranchAddress("aljarea", &aljarea, &b_aljarea);
   fChain->SetBranchAddress("nljemf", &nljemf, &b_nljemf);
   fChain->SetBranchAddress("nljN90hits", &nljN90hits, &b_nljN90hits);
   fChain->SetBranchAddress("nljfhpd", &nljfhpd, &b_nljfhpd);
   fChain->SetBranchAddress("aljemf", &aljemf, &b_aljemf);
   fChain->SetBranchAddress("aljN90hits", &aljN90hits, &b_aljN90hits);
   fChain->SetBranchAddress("aljfhpd", &aljfhpd, &b_aljfhpd);
   fChain->SetBranchAddress("jdphi", &jdphi, &b_jdphi);
   fChain->SetBranchAddress("numJec", &numJec, &b_numJec);
   fChain->SetBranchAddress("njec", njec, &b_njec);
   fChain->SetBranchAddress("ajec", ajec, &b_ajec);
   fChain->SetBranchAddress("evtnp", &evtnp, &b_evtnp);
   fChain->SetBranchAddress("psube", psube, &b_psube);
   fChain->SetBranchAddress("ppt", ppt, &b_ppt);
   fChain->SetBranchAddress("peta", peta, &b_peta);
   fChain->SetBranchAddress("pphi", pphi, &b_pphi);
   fChain->SetBranchAddress("pndphi", pndphi, &b_pndphi);
   fChain->SetBranchAddress("pndr", pndr, &b_pndr);
   fChain->SetBranchAddress("pndrbg", pndrbg, &b_pndrbg);
   fChain->SetBranchAddress("padphi", padphi, &b_padphi);
   fChain->SetBranchAddress("padr", padr, &b_padr);
   fChain->SetBranchAddress("padrbg", padrbg, &b_padrbg);
   fChain->SetBranchAddress("lp", &lp_, &b__);
   fChain->SetBranchAddress("lp.fCoordinates.fRho", lp_fCoordinates_fRho, &b_lp_fCoordinates_fRho);
   fChain->SetBranchAddress("lp.fCoordinates.fEta", lp_fCoordinates_fEta, &b_lp_fCoordinates_fEta);
   fChain->SetBranchAddress("lp.fCoordinates.fPhi", lp_fCoordinates_fPhi, &b_lp_fCoordinates_fPhi);
   fChain->SetBranchAddress("trkNHits", trkNHits, &b_trkNHits);
   fChain->SetBranchAddress("trkdz", trkdz, &b_trkdz);
   fChain->SetBranchAddress("trkdze", trkdze, &b_trkdze);
   fChain->SetBranchAddress("trkd0", trkd0, &b_trkd0);
   fChain->SetBranchAddress("trkd0e", trkd0e, &b_trkd0e);
   fChain->SetBranchAddress("ljcnp", &ljcnp, &b_ljcnp);
   fChain->SetBranchAddress("ljcnpbg", &ljcnpbg, &b_ljcnpbg);
   fChain->SetBranchAddress("ljcpt", &ljcpt, &b_ljcpt);
   fChain->SetBranchAddress("ljcpt2", &ljcpt2, &b_ljcpt2);
   fChain->SetBranchAddress("ljcptr", &ljcptr, &b_ljcptr);
   fChain->SetBranchAddress("ljcptbg", &ljcptbg, &b_ljcptbg);
   fChain->SetBranchAddress("ljcpt2bg", &ljcpt2bg, &b_ljcpt2bg);
   fChain->SetBranchAddress("ljcptrbg", &ljcptrbg, &b_ljcptrbg);
   fChain->SetBranchAddress("lppt", &lppt, &b_lppt);
   fChain->SetBranchAddress("lpjdr", &lpjdr, &b_lpjdr);
   fChain->SetBranchAddress("lgppt", &lgppt, &b_lgppt);
   fChain->SetBranchAddress("lgpch", &lgpch, &b_lgpch);
   fChain->SetBranchAddress("lgppid", &lgppid, &b_lgppid);
   Notify();
}

Bool_t JetFragAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void JetFragAna::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
#endif // #ifdef JetFragAna_cxx
