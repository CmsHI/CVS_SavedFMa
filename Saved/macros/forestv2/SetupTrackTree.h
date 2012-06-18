//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec  7 12:57:09 2011 by ROOT version 5.27/06b
// from TTree trackTree/v1
// found on file: ../HiForestMinBias_v2.root
//////////////////////////////////////////////////////////
#include "commonSetup.h"
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class Tracks {
public :
   Tracks(){};
   ~Tracks(){};

   // Declaration of leaf types
   Int_t           nEv;
   Int_t           nLumi;
   Int_t           nBX;
   Int_t           nRun;
   Int_t           nv;
   Float_t         vx[4];   //[nv]
   Float_t         vy[4];   //[nv]
   Float_t         vz[4];   //[nv]
   Int_t           nTrk;
   Float_t         trkPt[maxEntryTrack];   //[nTrk]
   Float_t         trkPtError[maxEntryTrack];   //[nTrk]
   Int_t           trkNHit[maxEntryTrack];   //[nTrk]
   Int_t           trkNlayer[maxEntryTrack];   //[nTrk]
   Int_t           trkNlayer3D[maxEntryTrack];   //[nTrk]
   Float_t         trkEta[maxEntryTrack];   //[nTrk]
   Float_t         trkPhi[maxEntryTrack];   //[nTrk]
   Float_t         dedx[maxEntryTrack];   //[nTrk]
   Bool_t          highPurity[maxEntryTrack];   //[nTrk]
   Bool_t          highPuritySetWithPV[maxEntryTrack];   //[nTrk]
   Float_t         trkChi2[maxEntryTrack];   //[nTrk]
   Float_t         trkChi2hit1D[maxEntryTrack];   //[nTrk]
   Float_t         trkNdof[maxEntryTrack];   //[nTrk]
   Float_t         trkDz[maxEntryTrack];   //[nTrk]
   Float_t         trkDzError[maxEntryTrack];   //[nTrk]
   Float_t         trkDzError1[maxEntryTrack];   //[nTrk]
   Float_t         trkDzError2[maxEntryTrack];   //[nTrk]
   Float_t         trkDxy[maxEntryTrack];   //[nTrk]
   Float_t         trkDxyBS[maxEntryTrack];   //[nTrk]
   Float_t         trkDxy1[maxEntryTrack];   //[nTrk]
   Float_t         trkDxy2[maxEntryTrack];   //[nTrk]
   Float_t         trkDxyError[maxEntryTrack];   //[nTrk]
   Float_t         trkDxyErrorBS[maxEntryTrack];   //[nTrk]
   Float_t         trkDxyError1[maxEntryTrack];   //[nTrk]
   Float_t         trkDxyError2[maxEntryTrack];   //[nTrk]
   Float_t         trkDz1[maxEntryTrack];   //[nTrk]
   Float_t         trkDz2[maxEntryTrack];   //[nTrk]
   Float_t         trkVx[maxEntryTrack];   //[nTrk]
   Float_t         trkVy[maxEntryTrack];   //[nTrk]
   Float_t         trkVz[maxEntryTrack];   //[nTrk]
   Bool_t          trkFake[maxEntryTrack];   //[nTrk]
   Float_t         trkAlgo[maxEntryTrack];   //[nTrk]
   Int_t           pfType[maxEntryTrack];   //[nTrk]
   Float_t         pfCandPt[maxEntryTrack];   //[nTrk]
   Float_t         pfSumEcal[maxEntryTrack];   //[nTrk]
   Float_t         pfSumHcal[maxEntryTrack];   //[nTrk]
   Int_t           nParticle;
   Float_t         pStatus[maxEntryTrack];   //[nParticle]
   Float_t         pPId[maxEntryTrack];   //[nParticle]
   Float_t         pEta[maxEntryTrack];   //[nParticle]
   Float_t         pPhi[maxEntryTrack];   //[nParticle]
   Float_t         pPt[maxEntryTrack];   //[nParticle]
   Float_t         pAcc[maxEntryTrack];   //[nParticle]
   Float_t         pNRec[maxEntryTrack];   //[nParticle]
   Int_t           pNHit[maxEntryTrack];   //[nParticle]
   Float_t         mtrkPt[maxEntryTrack];   //[nParticle]
   Float_t         mtrkPtError[maxEntryTrack];   //[nParticle]
   Int_t           mtrkNHit[maxEntryTrack];   //[nParticle]
   Int_t           mtrkNlayer[maxEntryTrack];   //[nParticle]
   Int_t           mtrkNlayer3D[maxEntryTrack];   //[nParticle]
   Int_t           mtrkQual[maxEntryTrack];   //[nParticle]
   Float_t         mtrkChi2[maxEntryTrack];   //[nParticle]
   Float_t         mtrkNdof[maxEntryTrack];   //[nParticle]
   Float_t         mtrkDz1[maxEntryTrack];   //[nParticle]
   Float_t         mtrkDzError1[maxEntryTrack];   //[nParticle]
   Float_t         mtrkDxy1[maxEntryTrack];   //[nParticle]
   Float_t         mtrkDxyError1[maxEntryTrack];   //[nParticle]
   Float_t         mtrkAlgo[maxEntryTrack];   //[nParticle]
   Int_t           mtrkPfType[maxEntryTrack];   //[nParticle]
   Float_t         mtrkPfCandPt[maxEntryTrack];   //[nParticle]
   Float_t         mtrkPfSumEcal[maxEntryTrack];   //[nParticle]
   Float_t         mtrkPfSumHcal[maxEntryTrack];   //[nParticle]

   // List of branches
   TBranch        *b_nEv;   //!
   TBranch        *b_nLumi;   //!
   TBranch        *b_nBX;   //!
   TBranch        *b_nRun;   //!
   TBranch        *b_nv;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_nTrk;   //!
   TBranch        *b_trkPt;   //!
   TBranch        *b_trkPtError;   //!
   TBranch        *b_trkNHit;   //!
   TBranch        *b_trkNlayer;   //!
   TBranch        *b_trkNlayer3D;   //!
   TBranch        *b_trkEta;   //!
   TBranch        *b_trkPhi;   //!
   TBranch        *b_dedx;   //!
   TBranch        *b_highPurity;   //!
   TBranch        *b_highPuritySetWithPV;   //!
   TBranch        *b_trkChi2;   //!
   TBranch        *b_trkChi2hit1D;   //!
   TBranch        *b_trkNdof;   //!
   TBranch        *b_trkDz;   //!
   TBranch        *b_trkDzError;   //!
   TBranch        *b_trkDzError1;   //!
   TBranch        *b_trkDzError2;   //!
   TBranch        *b_trkDxy;   //!
   TBranch        *b_trkDxyBS;   //!
   TBranch        *b_trkDxy1;   //!
   TBranch        *b_trkDxy2;   //!
   TBranch        *b_trkDxyError;   //!
   TBranch        *b_trkDxyErrorBS;   //!
   TBranch        *b_trkDxyError1;   //!
   TBranch        *b_trkDxyError2;   //!
   TBranch        *b_trkDz1;   //!
   TBranch        *b_trkDz2;   //!
   TBranch        *b_trkVx;   //!
   TBranch        *b_trkVy;   //!
   TBranch        *b_trkVz;   //!
   TBranch        *b_trkFake;   //!
   TBranch        *b_trkAlgo;   //!
   TBranch        *b_pfType;   //!
   TBranch        *b_pfCandPt;   //!
   TBranch        *b_pfSumEcal;   //!
   TBranch        *b_pfSumHcal;   //!

   TBranch        *b_nParticle;   //!
   TBranch        *b_pStatus;   //!
   TBranch        *b_pPId;   //!
   TBranch        *b_pEta;   //!
   TBranch        *b_pPhi;   //!
   TBranch        *b_pPt;   //!
   TBranch        *b_pAcc;   //!
   TBranch        *b_pNRec;   //!
   TBranch        *b_pNHit;   //!
   TBranch        *b_mtrkPt;   //!
   TBranch        *b_mtrkPtError;   //!
   TBranch        *b_mtrkNHit;   //!
   TBranch        *b_mtrkNlayer;   //!
   TBranch        *b_mtrkNlayer3D;   //!
   TBranch        *b_mtrkQual;   //!
   TBranch        *b_mtrkChi2;   //!
   TBranch        *b_mtrkNdof;   //!
   TBranch        *b_mtrkDz1;   //!
   TBranch        *b_mtrkDzError1;   //!
   TBranch        *b_mtrkDxy1;   //!
   TBranch        *b_mtrkDxyError1;   //!
   TBranch        *b_mtrkAlgo;   //!
   TBranch        *b_mtrkPfType;   //!
   TBranch        *b_mtrkPfCandPt;   //!
   TBranch        *b_mtrkPfSumEcal;   //!
   TBranch        *b_mtrkPfSumHcal;   //!

};


void setupTrackTree(TTree *t,Tracks &tTracks,bool doCheck = 0)
{
   // Set branch addresses and branch pointers
   t->SetBranchAddress("nEv", &tTracks.nEv, &tTracks.b_nEv);
   t->SetBranchAddress("nLumi", &tTracks.nLumi, &tTracks.b_nLumi);
   t->SetBranchAddress("nBX", &tTracks.nBX, &tTracks.b_nBX);
   t->SetBranchAddress("nRun", &tTracks.nRun, &tTracks.b_nRun);
   t->SetBranchAddress("nv", &tTracks.nv, &tTracks.b_nv);
   t->SetBranchAddress("vx", tTracks.vx, &tTracks.b_vx);
   t->SetBranchAddress("vy", tTracks.vy, &tTracks.b_vy);
   t->SetBranchAddress("vz", tTracks.vz, &tTracks.b_vz);
   t->SetBranchAddress("nTrk", &tTracks.nTrk, &tTracks.b_nTrk);
   t->SetBranchAddress("trkPt", tTracks.trkPt, &tTracks.b_trkPt);
   t->SetBranchAddress("trkPtError", tTracks.trkPtError, &tTracks.b_trkPtError);
   t->SetBranchAddress("trkNHit", tTracks.trkNHit, &tTracks.b_trkNHit);
   t->SetBranchAddress("trkNlayer", tTracks.trkNlayer, &tTracks.b_trkNlayer);
   t->SetBranchAddress("trkNlayer3D", tTracks.trkNlayer3D, &tTracks.b_trkNlayer3D);
   t->SetBranchAddress("trkEta", tTracks.trkEta, &tTracks.b_trkEta);
   t->SetBranchAddress("trkPhi", tTracks.trkPhi, &tTracks.b_trkPhi);
   t->SetBranchAddress("dedx", tTracks.dedx, &tTracks.b_dedx);
   t->SetBranchAddress("highPurity", tTracks.highPurity, &tTracks.b_highPurity);
   t->SetBranchAddress("highPuritySetWithPV", tTracks.highPuritySetWithPV, &tTracks.b_highPuritySetWithPV);
   t->SetBranchAddress("trkChi2", tTracks.trkChi2, &tTracks.b_trkChi2);
   t->SetBranchAddress("trkChi2hit1D", tTracks.trkChi2hit1D, &tTracks.b_trkChi2hit1D);
   t->SetBranchAddress("trkNdof", tTracks.trkNdof, &tTracks.b_trkNdof);
   t->SetBranchAddress("trkDz", tTracks.trkDz, &tTracks.b_trkDz);
   t->SetBranchAddress("trkDzError", tTracks.trkDzError, &tTracks.b_trkDzError);
   t->SetBranchAddress("trkDzError1", tTracks.trkDzError1, &tTracks.b_trkDzError1);
   t->SetBranchAddress("trkDzError2", tTracks.trkDzError2, &tTracks.b_trkDzError2);
   t->SetBranchAddress("trkDxy", tTracks.trkDxy, &tTracks.b_trkDxy);
   t->SetBranchAddress("trkDxyBS", tTracks.trkDxyBS, &tTracks.b_trkDxyBS);
   t->SetBranchAddress("trkDxy1", tTracks.trkDxy1, &tTracks.b_trkDxy1);
   t->SetBranchAddress("trkDxy2", tTracks.trkDxy2, &tTracks.b_trkDxy2);
   t->SetBranchAddress("trkDxyError", tTracks.trkDxyError, &tTracks.b_trkDxyError);
   t->SetBranchAddress("trkDxyErrorBS", tTracks.trkDxyErrorBS, &tTracks.b_trkDxyErrorBS);
   t->SetBranchAddress("trkDxyError1", tTracks.trkDxyError1, &tTracks.b_trkDxyError1);
   t->SetBranchAddress("trkDxyError2", tTracks.trkDxyError2, &tTracks.b_trkDxyError2);
   t->SetBranchAddress("trkDz1", tTracks.trkDz1, &tTracks.b_trkDz1);
   t->SetBranchAddress("trkDz2", tTracks.trkDz2, &tTracks.b_trkDz2);
   t->SetBranchAddress("trkVx", tTracks.trkVx, &tTracks.b_trkVx);
   t->SetBranchAddress("trkVy", tTracks.trkVy, &tTracks.b_trkVy);
   t->SetBranchAddress("trkVz", tTracks.trkVz, &tTracks.b_trkVz);
   t->SetBranchAddress("trkFake", tTracks.trkFake, &tTracks.b_trkFake);
   t->SetBranchAddress("trkAlgo", tTracks.trkAlgo, &tTracks.b_trkAlgo);
   t->SetBranchAddress("pfType", tTracks.pfType, &tTracks.b_pfType);
   t->SetBranchAddress("pfCandPt", tTracks.pfCandPt, &tTracks.b_pfCandPt);
   t->SetBranchAddress("pfSumEcal", tTracks.pfSumEcal, &tTracks.b_pfSumEcal);
   t->SetBranchAddress("pfSumHcal", tTracks.pfSumHcal, &tTracks.b_pfSumHcal);
   // Genp
   t->SetBranchAddress("nParticle", &tTracks.nParticle, &tTracks.b_nParticle);
   t->SetBranchAddress("pStatus", tTracks.pStatus, &tTracks.b_pStatus);
   t->SetBranchAddress("pPId", tTracks.pPId, &tTracks.b_pPId);
   t->SetBranchAddress("pEta", tTracks.pEta, &tTracks.b_pEta);
   t->SetBranchAddress("pPhi", tTracks.pPhi, &tTracks.b_pPhi);
   t->SetBranchAddress("pPt", tTracks.pPt, &tTracks.b_pPt);
   t->SetBranchAddress("pAcc", tTracks.pAcc, &tTracks.b_pAcc);
   t->SetBranchAddress("pNRec", tTracks.pNRec, &tTracks.b_pNRec);
   t->SetBranchAddress("pNHit", tTracks.pNHit, &tTracks.b_pNHit);
   t->SetBranchAddress("mtrkPt", tTracks.mtrkPt, &tTracks.b_mtrkPt);
   t->SetBranchAddress("mtrkPtError", tTracks.mtrkPtError, &tTracks.b_mtrkPtError);
   t->SetBranchAddress("mtrkNHit", tTracks.mtrkNHit, &tTracks.b_mtrkNHit);
   t->SetBranchAddress("mtrkNlayer", tTracks.mtrkNlayer, &tTracks.b_mtrkNlayer);
   t->SetBranchAddress("mtrkNlayer3D", tTracks.mtrkNlayer3D, &tTracks.b_mtrkNlayer3D);
   t->SetBranchAddress("mtrkQual", tTracks.mtrkQual, &tTracks.b_mtrkQual);
   t->SetBranchAddress("mtrkChi2", tTracks.mtrkChi2, &tTracks.b_mtrkChi2);
   t->SetBranchAddress("mtrkNdof", tTracks.mtrkNdof, &tTracks.b_mtrkNdof);
   t->SetBranchAddress("mtrkDz1", tTracks.mtrkDz1, &tTracks.b_mtrkDz1);
   t->SetBranchAddress("mtrkDzError1", tTracks.mtrkDzError1, &tTracks.b_mtrkDzError1);
   t->SetBranchAddress("mtrkDxy1", tTracks.mtrkDxy1, &tTracks.b_mtrkDxy1);
   t->SetBranchAddress("mtrkDxyError1", tTracks.mtrkDxyError1, &tTracks.b_mtrkDxyError1);
   t->SetBranchAddress("mtrkAlgo", tTracks.mtrkAlgo, &tTracks.b_mtrkAlgo);
   t->SetBranchAddress("mtrkPfType", tTracks.mtrkPfType, &tTracks.b_mtrkPfType);
   t->SetBranchAddress("mtrkPfCandPt", tTracks.mtrkPfCandPt, &tTracks.b_mtrkPfCandPt);
   t->SetBranchAddress("mtrkPfSumEcal", tTracks.mtrkPfSumEcal, &tTracks.b_mtrkPfSumEcal);
   t->SetBranchAddress("mtrkPfSumHcal", tTracks.mtrkPfSumHcal, &tTracks.b_mtrkPfSumHcal);
   if (doCheck) {
      if (t->GetMaximum("nv")>4) cout <<"FATAL ERROR: Arrary size of nv too small!!!  "<<t->GetMaximum("nv")<<endl;
      if (t->GetMaximum("nTrk")>maxEntryTrack) cout <<"FATAL ERROR: Arrary size of nTrk too small!!!  "<<t->GetMaximum("nTrk")<<endl;
   }
}

