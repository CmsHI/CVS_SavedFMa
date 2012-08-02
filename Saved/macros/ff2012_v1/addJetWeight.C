#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "../forest/hiForest.h"
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

#define PI 3.141592653589
static const int MAXTRK = 30000;



void addJetWeight(std::string inputFile_="yskim_qcdAllPhoton30_allCent_nMix10_v7.root") {
 
 
  TFile *inf = new TFile(inputFile_.data());
  
  std::string remword=".root";
  size_t pos = inputFile_.find(remword);
  std::string forOutput = inputFile_;
  if(pos!= std::string::npos)
    forOutput.swap(forOutput.erase(pos,remword.length()));
  std::string endfix = "_jetWeightAdded.root";
  std::string outputFile = forOutput + endfix;
  
  // now open new root file                                                                                                                      
  TFile* newfile_data = new TFile(outputFile.data(),"recreate");
  cout << "Output file :" << outputFile << endl;
  
  
  TTree *yongsunTrack = (TTree*)inf->Get("yongsunTrack");
  TTree *tmixTrk      = (TTree*)inf->Get("mTrk2");
  TTree *tmixTrkMoM      = (TTree*)inf->Get("mTrk1");
  TTree *yongsunPhotonTree  = (TTree*)inf->Get("yongsunPhotonTree");
  TTree *yongsunSkimTree = (TTree*)inf->Get("yongsunSkimTree");
  TTree *yongsunHiEvt  = (TTree*)inf->Get("yongsunHiEvt");
  TTree *yongsunJetakPu3PF  = (TTree*)inf->Get("yongsunJetakPu3PF");
  TTree *tgj  = (TTree*)inf->Get("tgj");
  TTree *tmixJet  = (TTree*)inf->Get("tmixJet");
  
  TTree *newyongsunTrack = (TTree*)yongsunTrack->CloneTree(0);
  newyongsunTrack->SetMaxTreeSize(20000000000);

  TTree *newtmixTrk = (TTree*)tmixTrk->CloneTree(0);
  newtmixTrk->SetMaxTreeSize(20000000000);

  TTree *newtmixTrkMoM = (TTree*)tmixTrkMoM->CloneTree(0);
  newtmixTrkMoM->SetMaxTreeSize(20000000000);

  TTree *newyongsunPhotonTree = (TTree*)yongsunPhotonTree->CloneTree(0);
  newyongsunPhotonTree->SetMaxTreeSize(20000000000);

  TTree *newyongsunSkimTree = (TTree*)yongsunSkimTree->CloneTree(0);
  newyongsunSkimTree->SetMaxTreeSize(20000000000);

  TTree *newyongsunHiEvt = (TTree*)yongsunHiEvt->CloneTree(0);
  newyongsunHiEvt->SetMaxTreeSize(20000000000);

  TTree *newyongsunJetakPu3PF = (TTree*)yongsunJetakPu3PF->CloneTree(0);
  newyongsunJetakPu3PF->SetMaxTreeSize(20000000000);

  TTree *newtgj = (TTree*)tgj->CloneTree(0);
  newtgj->SetMaxTreeSize(20000000000);

  TTree *newtmixJet = (TTree*)tmixJet->CloneTree(0);
  newtmixJet->SetMaxTreeSize(20000000000);


  EvtSel evt;
  GammaJet gj;

  TBranch        *b_evt;
  TBranch        *b_gj;
  tgj->SetBranchAddress("evt", &evt, &b_evt);
  tgj->SetBranchAddress("lpho", &gj, &b_gj);
  
  //////////// usual track ///////////////
  Int_t           nTrk;
  TBranch        *b_nTrk;   //!                                                                                                                      
  Float_t         injetPt[50000];   //[nTrk]                                                                                         
  TBranch        *b_injetPt;   //!                                                                                                                   
  Float_t         injetWeight[50000];   //[nTrk]                                                                                                          
  yongsunTrack->SetBranchAddress("nTrk", &nTrk, &b_nTrk);
  yongsunTrack->SetBranchAddress("injetPt", injetPt, &b_injetPt);
  newyongsunTrack->Branch("injetWeight",injetWeight,"injetWeight[nTrk]");
  ///////////  mixed track //////////////
  Int_t           nTrkm;
  TBranch        *b_nTrkm;   //!                                                                                                                                                            
  Float_t         injetPtm[50000];   //[nTrk]                                                                                                                                               
  TBranch        *b_injetPtm;   //!                                                                                                                                                         
  Float_t         injetWeightm[50000];   //[nTrk]                                                                                                                                       
  Float_t         xjetPtm[50000];   //[nTrk]                                                                                                                                               
  TBranch        *b_xjetPtm;   //!                                                                                                                                                         
  Float_t         xjetWeightm[50000];   //[nTrk]    

  tmixTrk->SetBranchAddress("ntrk", &nTrkm, &b_nTrkm);
  tmixTrk->SetBranchAddress("injetPt", injetPtm, &b_injetPtm);
  tmixTrk->SetBranchAddress("xjetPt",  xjetPtm,  &b_xjetPtm);
  newtmixTrk->Branch("injetWeight",injetWeightm,"injetWeight[ntrk]");
  newtmixTrk->Branch("xjetWeight",xjetWeightm,"xjetWeight[ntrk]");

  ///////////// second mix track ////////////
  Int_t           nTrkMoM;
  TBranch        *b_nTrkMoM;  
  Float_t         xjetPtMoM[50000];   //[nTrk]                                                                                                                                      
  TBranch        *b_xjetPtMoM;   //!                                                                                                                               
  Float_t         xjetWeightMoM[50000];   //[nTrk]                                                                                                                               
  tmixTrkMoM->SetBranchAddress("ntrk", &nTrkMoM, &b_nTrkMoM);
  tmixTrkMoM->SetBranchAddress("xjetPt",  xjetPtMoM,  &b_xjetPtMoM);
  newtmixTrkMoM->Branch("xjetWeight",xjetWeightMoM,"xjetWeight[ntrk]");
  
  ////////////////////////////////////////// 
  

  
  
  /// Get the weight table!! //
  int cutOpt = 9999;
  TFile *infTable = new TFile(Form("jetReweightingTable_cutOpt%d.root",cutOpt));
  TH3D* corrTable = (TH3D*)infTable->Get("hRatio3D");
  ///////////////////////////

  Long64_t nentries = tgj->GetEntries();
  for (Long64_t jentry=0;  jentry<nentries;jentry++) {
  //  for (Long64_t jentry=0;  jentry<1000 ; jentry++) {
    if (jentry% 500 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
    yongsunTrack->GetEntry(jentry);
    tmixTrk->GetEntry(jentry);
    tmixTrkMoM->GetEntry(jentry);
    yongsunPhotonTree->GetEntry(jentry);
    yongsunSkimTree->GetEntry(jentry);
    yongsunHiEvt->GetEntry(jentry);
    yongsunJetakPu3PF->GetEntry(jentry);
    tgj->GetEntry(jentry);
    tmixJet->GetEntry(jentry);
    
    // normal tracks
    for ( int it = 0 ; it<=nTrk ; it++) { 
      if ( injetPt[it] < 20 ) {  /// change into flexible cut 
	injetWeight[it] = 0 ;
      }
      else {
	injetWeight[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, injetPt[it] )) ; 
      }
    }
    
    //tmixjet tracks
    for ( int it = 0 ; it<=nTrkm ; it++) {
      if ( injetPtm[it] < 20 ) {  /// change into flexible cut                                                                                                                    
	injetWeightm[it] = 0 ;
      }
      else {
        injetWeightm[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, injetPtm[it] )) ;
      }
      
      if ( xjetPtm[it] < 20 ) {  /// change into flexible cut                                                                                                                        
	xjetWeightm[it] = 0 ;
      }
      else {
        xjetWeightm[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, xjetPtm[it] )) ;
      }
      
    }
    
    for ( int it = 0 ; it<=nTrkMoM ; it++) {
      if ( xjetPtMoM[it] < 20 ) {  /// change into flexible cut                                                                                                                          
        xjetWeightMoM[it] = 0 ;
      }
      else {
        xjetWeightMoM[it] = corrTable->GetBinContent(  corrTable->FindBin( evt.cBin, gj.photonEt, xjetPtMoM[it] )) ;
      }
      
      
    }
    
    newyongsunTrack->Fill();
    newtmixTrk->Fill();
    newtmixTrkMoM->Fill();
    newyongsunPhotonTree->Fill();
    newyongsunSkimTree->Fill();
    newyongsunHiEvt->Fill();
    newyongsunJetakPu3PF->Fill();
    newtgj->Fill();
    newtmixJet->Fill();

    
  }
  
  newyongsunTrack->AutoSave();
  newtmixTrk->AutoSave();
  newtmixTrkMoM->AutoSave();
  newyongsunPhotonTree->AutoSave();
  newyongsunSkimTree->AutoSave();
  newyongsunHiEvt->AutoSave();
  newyongsunJetakPu3PF->AutoSave();
  newtgj->AutoSave();
  newtmixJet->AutoSave();

  //   newfile_data->Close();   // <<=== If there is close() function. writing stucks in the middle of looping.. I don't know why!!
  cout << " Done! "<< endl;
}



