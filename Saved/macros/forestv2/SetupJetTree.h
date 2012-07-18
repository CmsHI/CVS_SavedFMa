//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Dec 19 11:47:28 2011 by ROOT version 5.27/06b
// from TTree t/akPu3PFpatJets Jet Analysis Tree
// found on file: /d101/kimy/macro/hiPhotonAna2011/rootFiles/photon50_corrCentralityv12.root
//////////////////////////////////////////////////////////
#include "commonSetup.h"
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class Jets {
public :
   Jets(){};
   ~Jets(){};

   // Declaration of leaf types
   Int_t           evt;
   Float_t         b;
   Int_t           nref;
   Float_t         rawpt[maxEntry];   //[nref]
   Float_t         jtpt[maxEntry];   //[nref]
   Float_t         jteta[maxEntry];   //[nref]
   Float_t         jty[maxEntry];   //[nref]
   Float_t         jtphi[maxEntry];   //[nref]
   Float_t         jtpu[maxEntry];   //[nref]
   Float_t         pthat;
   Float_t         refpt[maxEntry];   //[nref]
   Float_t         refeta[maxEntry];   //[nref]
   Float_t         refy[maxEntry];   //[nref]
   Float_t         refphi[maxEntry];   //[nref]
   Float_t         refdphijt[maxEntry];   //[nref]
   Float_t         refdrjt[maxEntry];   //[nref]
   Float_t         refparton_pt[maxEntry];   //[nref]
   Int_t           refparton_flavor[maxEntry];   //[nref]
   Int_t           ngen;
   Int_t           genmatchindex[maxEntry];   //[ngen]
   Float_t         genpt[maxEntry];   //[ngen]
   Float_t         geneta[maxEntry];   //[ngen]
   Float_t         geny[maxEntry];   //[ngen]
   Float_t         genphi[maxEntry];   //[ngen]
   Float_t         gendphijt[maxEntry];   //[ngen]
   Float_t         gendrjt[maxEntry];   //[ngen]
   Float_t         smpt[maxEntry];   //[nref]
   Float_t         fr01Chg[maxEntry];   //[nref]
   Float_t         fr01EM[maxEntry];   //[nref]
   Float_t         fr01[maxEntry];   //[nref]

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_b;   //!
   TBranch        *b_nref;   //!
   TBranch        *b_rawpt;   //!
   TBranch        *b_jtpt;   //!
   TBranch        *b_jteta;   //!
   TBranch        *b_jty;   //!
   TBranch        *b_jtphi;   //!
   TBranch        *b_jtpu;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_refpt;   //!
   TBranch        *b_refeta;   //!
   TBranch        *b_refy;   //!
   TBranch        *b_refphi;   //!
   TBranch        *b_refdphijt;   //!
   TBranch        *b_refdrjt;   //!
   TBranch        *b_refparton_pt;   //!
   TBranch        *b_refparton_flavor;   //!
   TBranch        *b_ngen;   //!
   TBranch        *b_genmatchindex;   //!
   TBranch        *b_genpt;   //!
   TBranch        *b_geneta;   //!
   TBranch        *b_geny;   //!
   TBranch        *b_genphi;   //!
   TBranch        *b_gendphijt;   //!
   TBranch        *b_gendrjt;   //!
   TBranch        *b_smpt;   //!
   TBranch        *b_fr01Chg;   //!
   TBranch        *b_fr01EM;   //!
   TBranch        *b_fr01;   //!

};


void setupJetTree(TTree *t,Jets &tJets,bool doCheck = 0)
{
   // Set branch addresses and branch pointers
   t->SetBranchAddress("evt", &tJets.evt, &tJets.b_evt);
   t->SetBranchAddress("b", &tJets.b, &tJets.b_b);
   t->SetBranchAddress("nref", &tJets.nref, &tJets.b_nref);
   t->SetBranchAddress("rawpt", tJets.rawpt, &tJets.b_rawpt);
   t->SetBranchAddress("jtpt", tJets.jtpt, &tJets.b_jtpt);
   t->SetBranchAddress("jteta", tJets.jteta, &tJets.b_jteta);
   t->SetBranchAddress("jty", tJets.jty, &tJets.b_jty);
   t->SetBranchAddress("jtphi", tJets.jtphi, &tJets.b_jtphi);
   t->SetBranchAddress("jtpu", tJets.jtpu, &tJets.b_jtpu);
   t->SetBranchAddress("pthat", &tJets.pthat, &tJets.b_pthat);
   t->SetBranchAddress("refpt", tJets.refpt, &tJets.b_refpt);
   t->SetBranchAddress("refeta", tJets.refeta, &tJets.b_refeta);
   t->SetBranchAddress("refy", tJets.refy, &tJets.b_refy);
   t->SetBranchAddress("refphi", tJets.refphi, &tJets.b_refphi);
   t->SetBranchAddress("refdphijt", tJets.refdphijt, &tJets.b_refdphijt);
   t->SetBranchAddress("refdrjt", tJets.refdrjt, &tJets.b_refdrjt);
   t->SetBranchAddress("refparton_pt", tJets.refparton_pt, &tJets.b_refparton_pt);
   t->SetBranchAddress("refparton_flavor", tJets.refparton_flavor, &tJets.b_refparton_flavor);
   t->SetBranchAddress("ngen", &tJets.ngen, &tJets.b_ngen);
   t->SetBranchAddress("genmatchindex", tJets.genmatchindex, &tJets.b_genmatchindex);
   t->SetBranchAddress("genpt", tJets.genpt, &tJets.b_genpt);
   t->SetBranchAddress("geneta", tJets.geneta, &tJets.b_geneta);
   t->SetBranchAddress("geny", tJets.geny, &tJets.b_geny);
   t->SetBranchAddress("genphi", tJets.genphi, &tJets.b_genphi);
   t->SetBranchAddress("gendphijt", tJets.gendphijt, &tJets.b_gendphijt);
   t->SetBranchAddress("gendrjt", tJets.gendrjt, &tJets.b_gendrjt);
   t->SetBranchAddress("smpt", tJets.smpt, &tJets.b_smpt);
   t->SetBranchAddress("fr01Chg", tJets.fr01Chg, &tJets.b_fr01Chg);
   t->SetBranchAddress("fr01EM", tJets.fr01EM, &tJets.b_fr01EM);
   t->SetBranchAddress("fr01", tJets.fr01, &tJets.b_fr01);
   if (doCheck) {
      if (t->GetMaximum("nref")>maxEntry) cout <<"FATAL ERROR: Arrary size of nref too small!!!  "<<t->GetMaximum("nref")<<endl;
      if (t->GetMaximum("ngen")>maxEntry) cout <<"FATAL ERROR: Arrary size of ngen too small!!!  "<<t->GetMaximum("ngen")<<endl;
   }
}

