//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 28 23:50:13 2011 by ROOT version 5.27/06b
// from TTree photon/Tree of Rechits around photon
// found on file: ../../ntout/GenHiForest_HyPhton50_all_v2.root
//////////////////////////////////////////////////////////
#include "commonSetup.h"
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class Genps {
public :
   Genps(){};
   ~Genps(){};

   // Declaration of leaf types
   Int_t           cBin;
   Float_t         hf;
   Int_t           nPar;
   Float_t         recoVtxZ;
   Float_t         et[maxEntry];   //[nPar]
   Float_t         eta[maxEntry];   //[nPar]
   Float_t         phi[maxEntry];   //[nPar]
   Int_t           id[maxEntry];   //[nPar]
   Int_t           momId[maxEntry];   //[nPar]
   Int_t           status[maxEntry];   //[nPar]
   Int_t           collId[maxEntry];   //[nPar]

   // List of branches
   TBranch        *b_cBin;   //!
   TBranch        *b_hf;   //!
   TBranch        *b_nPar;   //!
   TBranch        *b_recoVtxZ;   //!
   TBranch        *b_et;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_id;   //!
   TBranch        *b_momId;   //!
   TBranch        *b_status;   //!
   TBranch        *b_collId;   //!

};


void setupGenpTree(TTree *t,Genps &tGenps,bool doCheck = 0)
{
   // Set branch addresses and branch pointers
   t->SetBranchAddress("cBin", &tGenps.cBin, &tGenps.b_cBin);
   t->SetBranchAddress("hf", &tGenps.hf, &tGenps.b_hf);
   t->SetBranchAddress("nPar", &tGenps.nPar, &tGenps.b_nPar);
   t->SetBranchAddress("recoVtxZ", &tGenps.recoVtxZ, &tGenps.b_recoVtxZ);
   t->SetBranchAddress("et", tGenps.et, &tGenps.b_et);
   t->SetBranchAddress("eta", tGenps.eta, &tGenps.b_eta);
   t->SetBranchAddress("phi", tGenps.phi, &tGenps.b_phi);
   t->SetBranchAddress("id", tGenps.id, &tGenps.b_id);
   t->SetBranchAddress("momId", tGenps.momId, &tGenps.b_momId);
   t->SetBranchAddress("status", tGenps.status, &tGenps.b_status);
   t->SetBranchAddress("collId", tGenps.collId, &tGenps.b_collId);
   if (doCheck) {
     if (t->GetMaximum("nPar")>maxEntry) cout <<"FATAL ERROR: Arrary size of nPar too small!!!  "<<t->GetMaximum("nPar")<<endl;
   }
}

