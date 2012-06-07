//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr 18 09:21:09 2012 by ROOT version 5.27/06b
// from TTree hi/Tree of Hi gen Event
// found on file: /net/hisrv0001/home/icali/hadoop/Pythia/Z2/ppDijet_merged/pp276Dijet80_merged.root
//////////////////////////////////////////////////////////
#include "commonSetup.h"
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class GenParticles {
public :
   GenParticles(){};
   ~GenParticles(){};

   // Declaration of leaf types
   Int_t           event;
   Float_t         b;
   Float_t         npart;
   Float_t         ncoll;
   Float_t         nhard;
   Float_t         phi0;
   Float_t         scale;
   Int_t           n[3];
   Float_t         ptav[3];
   Int_t           mult;
   Float_t         pt[maxEntryTrack];   //[mult]
   Float_t         eta[maxEntryTrack];   //[mult]
   Float_t         phi[maxEntryTrack];   //[mult]
   Int_t           pdg[maxEntryTrack];   //[mult]
   Int_t           chg[maxEntryTrack];   //[mult]
   Int_t           sube[maxEntryTrack];   //[mult]
   Float_t         vx;
   Float_t         vy;
   Float_t         vz;
   Float_t         vr;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_b;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_ncoll;   //!
   TBranch        *b_nhard;   //!
   TBranch        *b_phi0;   //!
   TBranch        *b_scale;   //!
   TBranch        *b_n;   //!
   TBranch        *b_ptav;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_chg;   //!
   TBranch        *b_sube;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_vr;   //!

};


void setupGenParticleTree(TTree *t,GenParticles &tGenParticles,bool doCheck = 0)
{
   // Set branch addresses and branch pointers
   t->SetBranchAddress("event", &tGenParticles.event, &tGenParticles.b_event);
   t->SetBranchAddress("b", &tGenParticles.b, &tGenParticles.b_b);
   t->SetBranchAddress("npart", &tGenParticles.npart, &tGenParticles.b_npart);
   t->SetBranchAddress("ncoll", &tGenParticles.ncoll, &tGenParticles.b_ncoll);
   t->SetBranchAddress("nhard", &tGenParticles.nhard, &tGenParticles.b_nhard);
   t->SetBranchAddress("phi0", &tGenParticles.phi0, &tGenParticles.b_phi0);
   t->SetBranchAddress("scale", &tGenParticles.scale, &tGenParticles.b_scale);
   t->SetBranchAddress("n", tGenParticles.n, &tGenParticles.b_n);
   t->SetBranchAddress("ptav", tGenParticles.ptav, &tGenParticles.b_ptav);
   t->SetBranchAddress("mult", &tGenParticles.mult, &tGenParticles.b_mult);
   t->SetBranchAddress("pt", tGenParticles.pt, &tGenParticles.b_pt);
   t->SetBranchAddress("eta", tGenParticles.eta, &tGenParticles.b_eta);
   t->SetBranchAddress("phi", tGenParticles.phi, &tGenParticles.b_phi);
   t->SetBranchAddress("pdg", tGenParticles.pdg, &tGenParticles.b_pdg);
   t->SetBranchAddress("chg", tGenParticles.chg, &tGenParticles.b_chg);
   t->SetBranchAddress("sube", tGenParticles.sube, &tGenParticles.b_sube);
   t->SetBranchAddress("vx", &tGenParticles.vx, &tGenParticles.b_vx);
   t->SetBranchAddress("vy", &tGenParticles.vy, &tGenParticles.b_vy);
   t->SetBranchAddress("vz", &tGenParticles.vz, &tGenParticles.b_vz);
   t->SetBranchAddress("vr", &tGenParticles.vr, &tGenParticles.b_vr);
   if (doCheck) {
      if (t->GetMaximum("mult")>maxEntryTrack) cout <<"FATAL ERROR: Arrary size of mult too small!!!  "<<t->GetMaximum("mult")<<endl;
   }
}

