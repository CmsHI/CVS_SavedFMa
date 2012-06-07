#ifndef hiForest_h
#define hiForest_h

#include <iostream>
#include <vector>
#include <algorithm>

#include "commonTool.h"
#include "SetupPhotonTree.h"
#include "SetupJetTree.h"
#include "SetupHltTree.h"
#include "SetupSkimTree.h"
#include "SetupTrackTree.h"
#include "SetupHitTree.h"
#include "SetupEvtTree.h"
#include "SetupMetTree.h"
#include "SetupGenpTree.h"
#include "SetupPFTree.h"
#include "SetupGenParticleTree.h"
#include "TrackingCorrections2012.h"

#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TF1.h>
#include <TCut.h>

#include "DummyJetCorrector.h"

// ==========================================================
// Main class which can be used to read the hiForest trees
//
// Author: Yen-Jie Lee
//
// ==========================================================

namespace names{
  enum Algo{
     icPu5calo,         ic5calo,         akPu3calo,         ak3calo,         akPu3PF,             ak3PF,             akPu5calo,         ak5calo,         akPu5PF,             ak5PF};
  string AlgoRename[100] = {
    "icPu5calo",       "ic5calo",       "akPu3calo",       "ak3calo",       "akPu3PF",           "ak3PF",           "akPu5calo",       "ak5calo",       "akPu5PF",           "ak5PF"};
  string AlgoAnalyzer[100] = {
    "icPu5JetAnalyzer","ic5JetAnalyzer","akPu3JetAnalyzer","ak3JetAnalyzer","akPu3PFJetAnalyzer","ak3PFJetAnalyzer","akPu5JetAnalyzer","ak5JetAnalyzer","akPu5PFJetAnalyzer","ak5PFJetAnalyzer"};
}

class HiForest : public TNamed
{

  public: 
   HiForest(const char *file, const char *name="forest", bool ispp = 0, bool ismc = 0, bool isrecorrected = 0, TString jetAlgo="akPu3PF");
  ~HiForest();

  // Utility functions
  void GetEntry(int i);
  int  GetEntries();  						// Get the number of entries 
  void InitTree();
  void CheckTree(TTree *t,const char *title);				// Check the status of a tree

  void CheckArraySizes();
  void PrintStatus();						// Print the status of the hiForest
  void SetOutputFile(const char *name);               		// Set output file name for skim
  void AddCloneTree(TTree* t, const char *dirName, const char *treeName);   // Add a clone tree to the clone forest
  void FillOutput();						// Fill output forest  
  
  void GetEnergyScaleTable(char *fileNameTable);
  float getCorrEt(int j);
  Long64_t Draw(const char* varexp, const char* selection, Option_t* option = "", Long64_t nentries = 1000000000, Long64_t firstentry = 0){
     return tree->Draw(varexp,selection,option,nentries,firstentry);
  }

  // Event filtering utility functions
  bool selectEvent();
  TCut eventSelection();

  // Photon utility functions
  bool isSpike(int i);                          // return true if it is considered as a spike candidate
  bool isLooseEGamma(int i);
  bool isLoosePhoton(int i);
  bool isGoodPhoton(int i);                     // return true if it is considered as a hiGoodPhoton candidate
  bool isIsolatedPhoton(int i);                     // return true if it is considered as a hiGoodPhoton candidate
  bool isMCSignal(int i);
  bool isDirectPhoton(int i);
  bool isFragPhoton(int i);


  //bool isSidebandPhoton(int i);                     // return true if it is considered as a photon sideband

  // Jet utility functions
  void sortJets(TTree* jetTree, Jets& jets, double etaMax = 2, double ptMin = 40, bool allEvents = 1, int smearType = -1);
  int leadingJet();
  int subleadingJet();
  int thirdJet();
  double deltaPhiDijet(Jets& jets);
  bool hasDiJet(Jets& jets, double pt1 = 100, double pt2 = 40, double dphiMin = 2.*3.1415926/3.);
  void fakeRejection(TTree *jetTree, Jets &jets, bool allEvents);
  
  // Get track-jet correlated variables. Not needed if correlatePF is run.
  void correlateTracks(TTree* jetTree, Jets& jets, bool allEvents = 1, bool smeared = 0);
  // Build correlations between jet & its constituents, builds jetIDs
  void correlatePF(TTree* jetTree, Jets& jets, bool allEvents = 1){return;}

  double jetFracChg(int i);
  double jetFracNeut(int i);
  double jetFracEM(int i);

  // Track utility functions
  int getMatchedCaloTowerAllowReuse(int j);
  int getMatchedHBHEAllowReuse(int j);
  void matchTrackCalo(bool allEvents = 1);

  // TFile
  TFile *inf; 					// Input file 
  TFile *outf;                                  // Output file

  // Trees
  TTree *photonTree;				// Photon Tree, see branches in SetupPhotonTree.h
  TTree *icPu5jetTree;				// Jet Tree with icPu5 algorithm, see branches in SetupJetTree.h
  TTree *akPu3jetTree;				// Jet Tree with akPu3PF algorithm, see branches in SetupJetTree.h
  TTree *hltTree;				// OpenHLT Tree, see branches in SetupHltTree.h
  TTree *trackTree;				// Track Tree, see branches in SetupTrackTree.h
  TTree *pixtrackTree;				// Track Tree, see branches in SetupTrackTree.h
  TTree *skimTree;				// Skim Tree, contains event selection info, see branches in SetupSkimTree.h
  TTree *towerTree;                             // Tower Tree
  TTree *hbheTree;                              // HCAL HBHE Tree
  TTree *ebTree;                                // ECAL eb Tree
  TTree *evtTree;                               // Event Tree
  TTree *metTree;                               // MET Tree
  TTree *genpTree;                               // Gen photon of the signal event Tree
  TTree *genParticleTree;                        // Stable Gen particles

  TTree *tree;					// Pointer to the available tree, all trees in the forest are friended to each other

  vector<TTree*> jetTrees;
  vector<TTree*> cloneForest;                   // Vector of clones for skim

  TF1* fGauss;

  // Branches
  Hlts hlt;
  Skims skim;

  vector<Jets> alljets;

  Jets icPu5;
  Jets akPu3PF;

  Photons photon;
  Tracks track;
  Tracks pixtrack;
  Hits tower;
  Hits hbhe;
  Hits eb;
  Evts evt;
  Mets met;
  Genps genp;
  GenParticles genparticle;
    
  // Booleans
  bool hasPhotonTree;
  bool hasEvtTree;
  bool hasMetTree;
  bool hasIcPu5JetTree;
  bool hasAkPu3JetTree;
  bool hasHltTree;
  bool hasTrackTree;
  bool hasPixTrackTree;
  bool hasSkimTree;
  bool hasTowerTree;
  bool hasHbheTree;
  bool hasEbTree;
  bool hasGenpTree;
  bool hasGenParticleTree;

  bool setupOutput;
  bool verbose;
  bool pp;
  bool mc;
  bool doJetCorrection;
  bool doTrackCorrections;

  // Extra variables
  Float_t* towerEt;
  Float_t* towerdR;
  Float_t* hbheEt;
  Float_t* hbhedR;

  Float_t* jtChg;
  Float_t* jtNeut;
  Float_t* jtEM;

  Float_t* jtChgGen;
  Float_t* jtNeutGen;
  Float_t* jtEMGen;

  Float_t* jtPtMax;
  Float_t* jtPtMean;
  Float_t* jtPtMeanW;

  Int_t* jtLeadType;

  Int_t jtLead;
  Int_t jtSubLead;
  bool jtHasDijet;
  bool jtHasLeadingJet;

  Float_t* tjDeltaEtaLead;
  Float_t* tjDeltaPhiLead;
  Float_t* zLead;

  Float_t* tjDeltaEtaSubLead;
  Float_t* tjDeltaPhiSubLead;
  Float_t* zSubLead;

  Float_t* zOldLead;
  Float_t* zOldSubLead;

  Float_t* zSingleLead;
  Float_t* zLabLead;
  Float_t* zSingleSubLead;
  Float_t* zLabSubLead;
  Float_t* tjDeltaThetaLead;
  Float_t* tjDeltaThetaLabLead;
  Float_t* tjDeltaThetaSingleLead;

  Float_t* tjDeltaThetaSubLead;
  Float_t* tjDeltaThetaLabSubLead;
  Float_t* tjDeltaThetaSingleSubLead;

  Float_t* corrLead;
  Float_t* corrSubLead;


  int nEntries;
  int currentEvent;
  double cone;

  vector<JetCorrectorParameters> vpar_HI310x;
  FactorizedJetCorrector *_JEC_HI310X;

  vector<TrackingCorrections*> trackCorrections;
  TF1* fEnergyScale[2][10];  // [a][b],  a =0 for unconverted,  a=1 for converted.   b: 1,2,3 is centrality bin. b=0 is empty                                                                                       
  
 private:
  
  
  
};

HiForest::HiForest(const char *infName, const char* name, bool ispp, bool ismc, bool recjec, TString jetAlgo):
   tree(0),
   fGauss(0),
   verbose(0),
   pp(ispp),
   mc(ismc),
   nEntries(0),
   currentEvent(0)
{
   tree = new TTree("tree","");
  SetName(name);
  // Input file
  inf = TFile::Open(infName);

  cone = 0.3;
  doTrackCorrections = 0;

  // Load trees. Hard coded for the moment
  hltTree      = (TTree*) inf->Get("hltanalysis/HltTree");
  skimTree     = (TTree*) inf->Get("skimanalysis/HltTree");
  photonTree   = (TTree*) inf->Get("multiPhotonAnalyzer/photon");
  if (photonTree==0)  photonTree   = (TTree*) inf->Get("NTuples/Analysis");
  trackTree    = (TTree*) inf->Get("anaTrack/trackTree");
  //pixtrackTree = (TTree*) inf->Get("pixelTrack/trackTree");
  pixtrackTree = (TTree*) inf->Get("anaPixTrack/trackTree");
  towerTree    = (TTree*) inf->Get("rechitanalyzer/tower");
  icPu5jetTree = (TTree*) inf->Get("icPu5JetAnalyzer/t");
  akPu3jetTree = (TTree*) inf->Get(jetAlgo+"JetAnalyzer/t");
  hbheTree     = (TTree*) inf->Get("rechitanalyzer/hbhe");
  ebTree       = (TTree*) inf->Get("rechitanalyzer/eb");
  evtTree      = (TTree*) inf->Get("hiEvtAnalyzer/HiTree");
  metTree      = (TTree*) inf->Get("anaMET/metTree");
  genpTree     = (TTree*) inf->Get("genpana/photon");
  genParticleTree     = (TTree*) inf->Get("HiGenParticleAna/hi");
/*
  if(pp){
    icPu5jetTree = 0;//(TTree*) inf->Get(Form("%s/t",names::AlgoAnalyzer[names::icPu5calo].data()));
    akPu3jetTree = (TTree*) inf->Get(Form("%s/t",names::AlgoAnalyzer[names::ak3PF].data()));
  }else{
    icPu5jetTree = 0;//(TTree*) inf->Get(Form("%s/t",names::AlgoAnalyzer[names::icPu5calo].data()));
    if(recjec){
       akPu3jetTree = (TTree*) inf->Get(Form("%s/t",(names::AlgoAnalyzer[names::akPu3PF]+"_recorrected").data()));
       cout<<"HEEY, the jets are: "<<akPu3jetTree->GetName()<<endl;
    }else{
       akPu3jetTree = (TTree*) inf->Get(Form("%s/t",names::AlgoAnalyzer[names::akPu3PF].data()));
    }
  }
*/
  // Check the validity of the trees.
  hasPhotonTree    = (photonTree   != 0);
  hasEvtTree       = (evtTree   != 0);
  hasMetTree       = (metTree   != 0);
  hasIcPu5JetTree  = (icPu5jetTree != 0);
  hasAkPu3JetTree  = (akPu3jetTree != 0);
  hasTrackTree     = (trackTree    != 0);
  hasPixTrackTree  = (pixtrackTree    != 0);
  hasHltTree       = (hltTree      != 0);
  hasSkimTree      = (skimTree     != 0);
  hasTowerTree     = (towerTree    != 0);
  hasHbheTree      = (hbheTree     != 0);
  hasEbTree        = (ebTree     != 0);
  hasGenpTree	   = (genpTree   !=0);
  hasGenParticleTree = (genParticleTree   !=0);
  setupOutput = false;
  
  // Setup branches. See also Setup*.h
  if (hasPhotonTree) {
    photonTree->SetName("photon");
    if (tree == 0) tree = photonTree; else tree->AddFriend(photonTree);
    setupPhotonTree(photonTree,photon);
  }

  if (hasEvtTree) {
    evtTree->SetName("evtTree");
    if (tree == 0) tree = evtTree; else tree->AddFriend(evtTree);
    setupEvtTree(evtTree,evt);
  }

  if (hasMetTree) {
    metTree->SetName("met");
    if (tree == 0) tree = metTree; else tree->AddFriend(metTree);
    setupMetTree(metTree,met);
  }

  if (hasHltTree) {
    hltTree->SetName("hltTree");
    if (tree == 0) tree = hltTree; else tree->AddFriend(hltTree);
    setupHltTree(hltTree,hlt);
  }

  if (hasIcPu5JetTree) {
    icPu5jetTree->SetName("icPu5");
    if (tree == 0) tree = icPu5jetTree; else tree->AddFriend(icPu5jetTree);
    setupJetTree(icPu5jetTree,icPu5);
  }

  if (hasAkPu3JetTree) {
    akPu3jetTree->SetName("akPu3PF");
    if (tree == 0) tree = akPu3jetTree; else tree->AddFriend(akPu3jetTree);
    setupJetTree(akPu3jetTree,akPu3PF);
  }

  if (hasTrackTree) {
    trackTree->SetName("track");
    if (tree == 0) tree = trackTree; else tree->AddFriend(trackTree);
    setupTrackTree(trackTree,track);
  }
   
  if (hasPixTrackTree) {
    pixtrackTree->SetName("pixtrack");
    if (tree == 0) tree = pixtrackTree; else tree->AddFriend(pixtrackTree);
    setupTrackTree(pixtrackTree,pixtrack);
  }
   
  if (hasSkimTree) {
    skimTree->SetName("skim");
    if (tree == 0) tree = skimTree; else tree->AddFriend(skimTree);
    setupSkimTree(skimTree,skim);
  }

  if (hasTowerTree) {
    towerTree->SetName("tower");
    if (tree == 0) tree = towerTree; else tree->AddFriend(towerTree);
    setupHitTree(towerTree,tower);
  }

  if (hasHbheTree) {
    hbheTree->SetName("hbhe");
    if (tree == 0) tree = hbheTree; else tree->AddFriend(hbheTree);
    setupHitTree(hbheTree,hbhe);
  }

  if (hasEbTree) {
    ebTree->SetName("eb");
    if (tree == 0) tree = ebTree; else tree->AddFriend(ebTree);
    setupHitTree(ebTree,eb);
  }
  
  if (hasGenpTree) {
    genpTree->SetName("genp");
    if (tree == 0) tree = genpTree; else tree->AddFriend(genpTree);
    setupGenpTree(genpTree,genp);
  }

  if (hasGenParticleTree) {
    genParticleTree->SetName("genParticle");
    if (tree == 0) tree = genParticleTree; else tree->AddFriend(genParticleTree);
    setupGenParticleTree(genParticleTree,genparticle);
  }

  tree->SetMarkerStyle(20);

  // Print the status of thre forest
  PrintStatus();

  //fakeRejection(icPu5jetTree, icPu5, 1);
  //fakeRejection(akPu3jetTree, akPu3PF, 1);
  //  CheckArraySizes();
}

HiForest::~HiForest()
{
  if (setupOutput) {
    for (unsigned int i=0; i<cloneForest.size(); i++)
    {
      cloneForest[i]->AutoSave();
    }
    outf->Close();
  }
}

void HiForest::GetEntry(int i)
{

   currentEvent = i;
  // get the entry of the available trees
  if (hasPhotonTree)   photonTree   ->GetEntry(i);
  if (hasEvtTree)      evtTree   ->GetEntry(i);
  if (hasMetTree)      metTree   ->GetEntry(i);
  if (hasHltTree)      hltTree      ->GetEntry(i);
  if (hasSkimTree)     skimTree     ->GetEntry(i);
  if (hasIcPu5JetTree) icPu5jetTree ->GetEntry(i);
  if (hasAkPu3JetTree) akPu3jetTree ->GetEntry(i);
  if (hasTrackTree)    trackTree    ->GetEntry(i);
  if (hasPixTrackTree) pixtrackTree ->GetEntry(i);
  if (hasTowerTree)    towerTree    ->GetEntry(i);
  if (hasHbheTree)     hbheTree     ->GetEntry(i);
  if (hasEbTree)       ebTree     ->GetEntry(i);
  if (hasGenpTree)     genpTree   ->GetEntry(i);
  if (hasGenParticleTree) genParticleTree   ->GetEntry(i);
}

int HiForest::GetEntries()
{
  // get the entries of the available trees
  return nEntries;
}

void HiForest::InitTree()
{
   // Setup Track Corrections 	 
   if(doTrackCorrections){
//       trackCorrections.push_back(new TrackingCorrections("QM2011","hitrkEffAnalyzer_akpu3pf")); 	 
//       trackCorrections.push_back(new TrackingCorrections("QM2012IterTrk","hitrkEffAnalyzer_GeneralCalo")); 	 
      trackCorrections.push_back(new TrackingCorrections("QM2012IterTrk","hitrkEffAnalyzer_Selected")); 	 
      trackCorrections.push_back(new TrackingCorrections("QM2012PixTrk","hitrkEffAnalyzer_PixTrk")); 	 

      for(int i = 0; i < trackCorrections.size(); ++i){
         if (pp) {
            trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq30_ppcorrpthgtv4.root",30);
            trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq50_ppcorrpthgtv4.root",50);
            trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq80_ppcorrpthgtv4.root",80);
            trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq110_ppcorrpthgtv4.root",110);
            trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq170_ppcorrpthgtv4.root",170);
         } else {
//             if (i==0) {
//               trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq30_tev9hgtv4_3.root",30);
//               trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq50_tev9hgtv4_3.root",50);
//               trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq80_tev9hgtv4_3.root",80);
//               trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq110_tev9hgtv4_3.root",110);
//               trackCorrections[i]->AddSample("trkcorr/trkCorrHisAna_djuq170_tev9hgtv4_3.root",170);

//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj30_v1.root",30);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj50_v1.root",50);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj80_v1.root",80);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj120_v1.root",120);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj200_v1.root",200);

              trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj50_iterTrkPixTrk_v1_full.root",50);
              trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj120_iterTrkPixTrk_v1_full.root",120);
              trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj170_iterTrkPixTrk_v1_full.root",170);
              trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj200_iterTrkPixTrk_v1_full.root",200);
//             } else if (i==1) {
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj30_v1.root",30);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj50_v1.root",50);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj80_v1.root",80);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj120_v1.root",120);
//               trackCorrections[i]->AddSample("trkcorr/itertrkpixtrk/trkcorr_hy18dj200_v1.root",200);
//             }
         }
         trackCorrections[i]->smoothLevel_ = 1; 	 
         trackCorrections[i]->Init(); 	 
      }
   }
}

void HiForest::CheckTree(TTree *t,const char *title)
{
  int entries = t->GetEntries();
  if (nEntries==0) nEntries = entries;
  cout <<title<<": "<<entries<<" entries loaded.";
  if (entries != nEntries) {
    cout <<" Inconsistent number of entries!!"<<endl;
  } else {
    cout <<endl;
  }
}


void HiForest::CheckArraySizes(){

  vector<int> trackOverflow(0);
  vector<int> objectOverflow(0);

  for(int ie = 0; ie < nEntries; ++ie){
    GetEntry(ie);
    if(track.nTrk > maxEntryTrack) trackOverflow.push_back(track.nTrk);
    if(akPu3PF.nref > maxEntry) objectOverflow.push_back(akPu3PF.nref);
    if(icPu5.nref > maxEntry) objectOverflow.push_back(icPu5.nref);
  }

  for(int i = 0; i < trackOverflow.size(); ++i){
    cout<<trackOverflow[i]<<endl;
  }

  if(trackOverflow.size() == 0) cout<<"Track sizes OK"<<endl;
  else cout<<"tracks crash"<<endl; // TODO : really crash

  for(int i = 0; i < objectOverflow.size(); ++i){
    cout<<objectOverflow[i]<<endl;
  }

  if(objectOverflow.size() == 0) cout<<"Object sizes OK"<<endl;
  else cout<<"objects crash"<<endl; // TODO : really crash

}



void HiForest::PrintStatus()
{
  if (hasHltTree)      CheckTree(hltTree,      "HltTree");
  if (hasSkimTree)     CheckTree(skimTree,     "SkimTree");
  if (hasIcPu5JetTree) CheckTree(icPu5jetTree, "IcPu5jetTree");
  if (hasAkPu3JetTree) CheckTree(akPu3jetTree, "AkPu3jetTree");
  if (hasTrackTree)    CheckTree(trackTree,    "TrackTree");
  if (hasPixTrackTree) CheckTree(pixtrackTree, "PixTrackTree");
  if (hasPhotonTree)   CheckTree(photonTree,   "PhotonTree");
  if (hasMetTree)      CheckTree(metTree,   "MetTree");
  if (hasTowerTree)    CheckTree(towerTree,    "TowerTree");
  if (hasHbheTree)     CheckTree(hbheTree,     "HbheTree");
  if (hasEbTree)       CheckTree(ebTree,     "EbTree");
  if (hasGenpTree)      CheckTree(genpTree,   "GenpTree");
  if (hasGenParticleTree)      CheckTree(genParticleTree,   "GenParticleTree");

}

void HiForest::SetOutputFile(const char *name)
{
  outf = new TFile(name,"recreate");
  if (hasHltTree)      AddCloneTree(hltTree,      "hltanalysis",        "HltTree");
  if (hasSkimTree)     AddCloneTree(skimTree,     "skimanalysis",       "HltTree");

     if(pp){
	if (hasIcPu5JetTree) AddCloneTree(icPu5jetTree, names::AlgoAnalyzer[names::icPu5calo].data(),   "t");
	if (hasAkPu3JetTree) AddCloneTree(akPu3jetTree, names::AlgoAnalyzer[names::ak3PF].data(), "t");
     }else{
	if (hasIcPu5JetTree) AddCloneTree(icPu5jetTree, names::AlgoAnalyzer[names::icPu5calo].data(),   "t");
	if (hasAkPu3JetTree) AddCloneTree(akPu3jetTree, names::AlgoAnalyzer[names::akPu3PF].data(), "t");
     }

  if (hasTrackTree)    AddCloneTree(trackTree,    "anaTrack",           "trackTree");
  if (hasPixTrackTree) AddCloneTree(pixtrackTree, "anaPixTrack",        "trackTree");
  if (hasPhotonTree)   AddCloneTree(photonTree,   "multiPhotonAnalyzer",            "photon");
  if (hasEvtTree)      AddCloneTree(evtTree,      "hiEvtAnalyzer",            "HiTree");
  if (hasMetTree)      AddCloneTree(metTree,      "anaMET",            "metTree");
  if (hasTowerTree)    AddCloneTree(towerTree,    "rechitanalyzer",              "tower");
  if (hasHbheTree)     AddCloneTree(hbheTree,     "rechitanalyzer",               "hbhe");
  if (hasEbTree)       AddCloneTree(ebTree,       "rechitanalyzer",               "eb");
  setupOutput = true;
}

void HiForest::AddCloneTree(TTree* t, const char *dirName, const char *treeName)
{
  // Make directory
  outf->cd();
  outf->mkdir(dirName);
  outf->cd(dirName);

  // Add a clone tree to the clone forest
  TTree *tClone = t->CloneTree(0);
  tClone->SetMaxTreeSize(4000000000);
  tClone->SetName(treeName);
  
  cloneForest.push_back(tClone);
}

void HiForest::FillOutput()
{
  if (setupOutput) {
     for (unsigned int i=0; i<cloneForest.size(); i++)
     {
       cloneForest[i]->Fill();
     } 
  } else {
       cout <<"ERROR: Specify an output file by hiForest.SetOutputFile(filename)!"<<endl;
  }
}

// ====================== Event Utilities ========================

bool HiForest::selectEvent(){
  /*
   bool select = skim.phbheReflagNewTimeEnv 
      && 
      skim.phcalTimingFilter 
      && 
      skim.pHBHENoiseFilter 
      && 
      skim.phiEcalRecHitSpikeFilter;
  */
  bool select = skim.pHBHENoiseFilter;
   if(!pp){
      select = select && skim.pcollisionEventSelection;
   }else{
     //      select = select && skim.phfCoincFilter && skim.ppurityFractionFilter;
   }
   return select;
}

TCut HiForest::eventSelection(){
  //   TCut select("skim.phbheReflagNewTimeEnv && skim.phcalTimingFilter && skim.pHBHENoiseFilter && skim.phiEcalRecHitSpikeFilter");
  TCut select("skim.pHBHENoiseFilter");
  if(!pp){
    select = select && "skim.pcollisionEventSelection";
  }else{
    //      select = select && "skim.phfCoincFilter && skim.ppurityFractionFilter";
  }
   return select;
}


void HiForest::GetEnergyScaleTable(char *fileNameTable) {
   TFile* f = new TFile(fileNameTable);
   fEnergyScale[0][1] = (TF1*)f->Get("fit_hscale_r9gt94_1");
   fEnergyScale[0][2] = (TF1*)f->Get("fit_hscale_r9gt94_2");
   fEnergyScale[0][3] = (TF1*)f->Get("fit_hscale_r9gt94_3");
   fEnergyScale[1][1] = (TF1*)f->Get("fit_hscale_r9lt94_1");
   fEnergyScale[1][2] = (TF1*)f->Get("fit_hscale_r9lt94_2");
   fEnergyScale[1][3] = (TF1*)f->Get("fit_hscale_r9lt94_3");
}


// ====================== Track Utilities ========================
#include "TrackUtilities.C"

// ======================= Jet Utilities =========================
#include "JetUtilities.C"

// ====================== Photon Utilities ========================
#include "PhotonUtilities.C"


#endif
