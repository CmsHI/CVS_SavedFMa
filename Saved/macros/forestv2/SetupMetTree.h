//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec  7 12:57:09 2011 by ROOT version 5.27/06b
// from TTree metTree/v1
// found on file: ../HiForestMinBias_v2.root
//////////////////////////////////////////////////////////
#include "commonSetup.h"
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class Mets {
public :
   Mets(){};
   ~Mets(){};

   // Declaration of leaf types
   Int_t           nEv;
   Int_t           nLumi;
   Int_t           nBX;
   Int_t           nRun;
   Int_t           nMET;
   Float_t         MET[2];   //[nMET]
   Float_t         METPhi[2];   //[nMET]
   Float_t         SumEt[2];   //[nMET]

   // List of branches
   TBranch        *b_nEv;   //!
   TBranch        *b_nLumi;   //!
   TBranch        *b_nBX;   //!
   TBranch        *b_nRun;   //!
   TBranch        *b_nMET;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_SumEt;   //!

};


void setupMetTree(TTree *t,Mets &tMets,bool doCheck = 0)
{
   // Set branch addresses and branch pointers
   t->SetBranchAddress("nEv", &tMets.nEv, &tMets.b_nEv);
   t->SetBranchAddress("nLumi", &tMets.nLumi, &tMets.b_nLumi);
   t->SetBranchAddress("nBX", &tMets.nBX, &tMets.b_nBX);
   t->SetBranchAddress("nRun", &tMets.nRun, &tMets.b_nRun);
   t->SetBranchAddress("nMET", &tMets.nMET, &tMets.b_nMET);
   t->SetBranchAddress("MET", tMets.MET, &tMets.b_MET);
   t->SetBranchAddress("METPhi", tMets.METPhi, &tMets.b_METPhi);
   t->SetBranchAddress("SumEt", tMets.SumEt, &tMets.b_SumEt);
   if (doCheck) {
      if (t->GetMaximum("nMET")>2) cout <<"FATAL ERROR: Arrary size of nMET too small!!!  "<<t->GetMaximum("nMET")<<endl;
   }
}

