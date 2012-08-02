#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "../HiForest_V2_latest/hiForest.h"
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

#define PI 3.141592653589
static const int MAXTRK  = 15000;   // This is very enough.
static const int MAXMTRK = 50000;   // Again this is very enough for 10 mixing  
static const long MAXTREESIZE = 10000000000;


class CentralityReWeight {
public:
  CentralityReWeight(TString data, TString mc,TCut s) :
    datafname(data),mcfname(mc),sel(s) {}
  void Init()
  {
    cout << "Reweight Centrality: " << endl;
    cout << "Data file: " << datafname << endl;
    cout << "MC file:   " << mcfname << endl;
    
    hCentData = new TH1D("hCentData","",40,0,40);
    hCentMc = new TH1D("hCentMc","",40,0,40);
    hReWt = new TH1D("hReWt","",40,0,40);
    
    TCut addCut = "pt[0]>50 && abs(eta[0])<1.44 && swissCrx[0]<0.9";

    
      TChain * tdata = new TChain("yongsunPhotonTree");
      tdata->Add(datafname);
      tdata->AddFriend("yEvt=yongsunHiEvt",datafname.Data());
      tdata->AddFriend("ySkim=yongsunSkimTree",datafname.Data());
           
      tdata->Draw("yEvt.hiBin>>hCentData","ySkim.pHBHENoiseFilter>0 && ySkim.pcollisionEventSelection>0" && addCut);

      TChain * tmc = new TChain("multiPhotonAnalyzer/photon");
      tmc->Add(mcfname);
      tmc->AddFriend("yEvt=hiEvtAnalyzer/HiTree",mcfname.Data());
      tmc->AddFriend("ySkim=skimanalysis/HltTree",mcfname.Data());
      tmc->Draw("yEvt.hiBin>>hCentMc","ySkim.pcollisionEventSelection>0" && addCut);
   
      //      hCentData->Scale(1./hCentData->Integral());                       
      //      hCentMc->Scale(1./hCentMc->Integral());
      hReWt->Divide(hCentData,hCentMc);
      TCanvas* c1 = new TCanvas("c_centrality","",500,1000);
      c1->Divide(1,2);
      c1->cd(1);
      handsomeTH1(hCentData,2);
      handsomeTH1(hCentMc,1);
      double maxRang = cleverRange(hCentData,hCentMc); 
      hCentMc->SetAxisRange(-1,maxRang,"Y");
      hCentMc->Draw("hist");
      hCentData->Draw("same");
      c1->cd(2);
      hReWt->Draw();
      c1->SaveAs("centrality.gif");
   }
  float GetWeight(int cBin) {
    int bin=cBin+1;
    if (hCentData->GetBinContent(bin)==0 || hCentMc->GetBinContent(bin)==0) {
      return 0;
    }
    return hCentData->GetBinContent(bin)/hCentMc->GetBinContent(bin);
  }
  TString datafname,mcfname;
  TCut sel;
  TH1D * hCentData;
  TH1D * hCentMc;
  TH1D * hReWt;
};



class Isolation{
public:
  float cc1,cc2,cc3,cc4,cc5;
  float cr1,cr2,cr3,cr4,cr5;
  float ct1PtCut20,ct2PtCut20,ct3PtCut20,ct4PtCut20,ct5PtCut20;
  void Set(HiForest * c, int j) {
    cc1=c->photon.cc1[j];
    cc2=c->photon.cc2[j];
    cc3=c->photon.cc3[j];
    cc4=c->photon.cc4[j];
    cc5=c->photon.cc5[j];
    cr1=c->photon.cr1[j];
    cr2=c->photon.cr2[j];
    cr3=c->photon.cr3[j];
    cr4=c->photon.cr4[j];
    cr5=c->photon.cr5[j];
    ct1PtCut20=c->photon.ct1PtCut20[j];
    ct2PtCut20=c->photon.ct2PtCut20[j];
    ct3PtCut20=c->photon.ct3PtCut20[j];
    ct4PtCut20=c->photon.ct4PtCut20[j];
    ct5PtCut20=c->photon.ct5PtCut20[j];
  }
};



void forest2yskim(TString inputFile_="mc/photon50_25k.root", std::string outname = "barrelPhoton50_25k.root",float cutphotonPt  = 45, bool needReweight=true, int maxEvents=-1, TString MinbiasFname="", bool isMC=false, bool minBiasOnminBias=false) { 
  
  TFile* cent2F = new TFile("percentBins.root");
  TH1D* cent2Hist;
  if ( isMC ) 
    cent2Hist = (TH1D*)cent2F->Get("hresult_mc");
  else
    cent2Hist = (TH1D*)cent2F->Get("hresult_data");
  
  int nCent2 = 40 ; // cent2Hist->GetNbinsX() + 1 ;  // ok back to 40 centrality bins
  cout << " number of mixing centrality bins = " << nCent2 << endl;// 
  
  bool takeCarePBin = true;
  
  int seconds = time(NULL);
  cout << " here 0" << endl ;
  cout << " time = " <<seconds%10000<< endl;
  TRandom3 rand(seconds%10000);
  cout << " here 1" << endl ;
  TString datafname  = "skimmed/yskim_for_dataCentrality_yskim_HiForestPhoton-v7-noDuplicate_nMinBisMixedQuarterPI.root"; // used for centrality reweighting
  float cutphotonEta = 1.44;
  float preCutPhotonEt = 30;
   
  const int nMaxPho = 1000;
  
  HiForest *c = new HiForest(inputFile_.Data());
  c->doTrackingSeparateLeadingSubleading = true;
  c->GetEnergyScaleTable("../forest2/photonEnergyScaleTable_Hydjet_GammaJet.root");
  cout << " here 1" << endl ;
  
  c->doTrackCorrections = 1;
  c->InitTree();
  cout << " here 1" << endl ;
  
  // now open new root file
  TFile* newfile_data = new TFile(outname.data(),"recreate");
  cout << " here 1" << endl ;
  
  TTree* newtree;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    newtree = c->photonTree->CloneTree(0);
    newtree->SetName("yongsunPhotonTree");
    newtree->SetMaxTreeSize(MAXTREESIZE);
  }

  TTree* newtreehlt;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    if ( c->hltTree != 0 ) {
      newtreehlt = c->hltTree->CloneTree(0);
      newtreehlt->SetName("yongsunHltTree");
      newtreehlt->SetMaxTreeSize(MAXTREESIZE);
    }
  }
  

  TTree* newtreeSkim;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    newtreeSkim = c->skimTree->CloneTree(0);
    newtreeSkim->SetName("yongsunSkimTree");
    newtreeSkim->SetMaxTreeSize(MAXTREESIZE);
  }

  TTree* newtreeEvt;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    newtreeEvt = c->evtTree->CloneTree(0);
    newtreeEvt->SetName("yongsunHiEvt");
    newtreeEvt->SetMaxTreeSize(MAXTREESIZE);
  }
  
  TTree* newtreeAk3Jet;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    newtreeAk3Jet = c->akPu3jetTree->CloneTree(0);
    newtreeAk3Jet->SetName("yongsunJetakPu3PF");
    newtreeAk3Jet->SetMaxTreeSize(MAXTREESIZE);
  }
  
  
  //  TTree* newtreeTrack = c->trackTree->CloneTree(0);
  //  newtreeTrack->SetName("yongsunTrack");
  
  float trkBoundary = 0. ; // Now, it's merged track collection 2GeV boundary 
  cout << " here 1" << endl ;
  
  
  int isGen(false);

  TTree* newtreeGen;  // Let's not save gen particles at this moment
  if ( c->genpTree !=0 ) {   
    newtreeGen = c->genpTree->CloneTree(0);
    newtreeGen->SetName("yongsunGen");
    newtreeGen->SetMaxTreeSize(MAXTREESIZE);
    isGen = true;
  }
  
  // more bits for photon tree
  float newPt[nMaxPho];      //     <==  temporary data storage
  int isLeading[nMaxPho];
  int order[nMaxPho];
  float corrPt[nMaxPho];
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    newtree->Branch("order",  order, "order[nPhotons]/I");
    newtree->Branch("corrPt", corrPt,"corrPt[nPhotons]/F");
    newtree->Branch("leading",isLeading,"leading[nPhotons]/I");
  }
  cout << " here 1" << endl ;
  

  // Track tree
  int nTrk;
  float trkPt[MAXTRK];
  float trkEta[MAXTRK];
  float trkPhi[MAXTRK];
  float trkWeight[MAXTRK];
  float trkWeightPara[MAXTRK];
  float injetPt[MAXTRK];
  float injetEta[MAXTRK];
  float injetPhi[MAXTRK];
  float injetDphi[MAXTRK];
  float injetCR[MAXTRK];  // CR = track to cone axis distance
  float dphiFg[MAXTRK];

  TTree *newtreeTrack;                                                                                                                       

  const int maxGenPar = 25000;
  int nGenPar;
  int   genParSube[maxGenPar];
  float genParPt[maxGenPar];
  float genParEta[maxGenPar];
  float genParPhi[maxGenPar];
  int   genParChg[maxGenPar];
  float genParInjetPt[maxGenPar];
  float genParInjetEta[maxGenPar];
  float genParInjetPhi[maxGenPar];
  float genParInjetCR[maxGenPar];  // CR = track to cone axis distance                                             
  float genParInjetDphi[maxGenPar];
  
  TTree *newtreeGenPar;
  
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    newtreeTrack = new TTree("yongsunTrack","merged tracks");
    newtreeTrack->SetMaxTreeSize(MAXTREESIZE);
    newtreeTrack->Branch("ntrk",&nTrk,"ntrk/I");
    newtreeTrack->Branch("pt",trkPt,"pt[ntrk]/F");
    newtreeTrack->Branch("eta",trkEta,"eta[ntrk]/F");
    newtreeTrack->Branch("phi",trkPhi,"phi[ntrk]/F");
    newtreeTrack->Branch("dphi", dphiFg, "dphi[ntrk]/F");
    newtreeTrack->Branch("trkWeight",trkWeight,"trkWeight[ntrk]/F");
    newtreeTrack->Branch("trkWeightPara",trkWeightPara,"trkWeightPara[ntrk]/F");
    newtreeTrack->Branch("injetPt", injetPt, "injetPt[ntrk]/F");
    newtreeTrack->Branch("injetEta", injetEta, "injetEta[ntrk]/F");
    newtreeTrack->Branch("injetPhi", injetPhi, "injetPhi[ntrk]/F");
    newtreeTrack->Branch("injetDphi", injetDphi, "injetDphi[ntrk]/F");
    newtreeTrack->Branch("injetCR", injetCR, "injetCR[ntrk]/F");
    
    if ( isMC ) {
      newtreeGenPar = new TTree("chg","gen chg particle");
      newtreeGenPar->SetMaxTreeSize(MAXTREESIZE);
      newtreeGenPar->Branch("nGenPar",&nGenPar,"nGenPar/I");
      newtreeGenPar->Branch("sube",genParSube,"sube[nGenPar]/I");
      newtreeGenPar->Branch("pt",genParPt,"pt[nGenPar]/F");
      newtreeGenPar->Branch("eta",genParEta,"eta[nGenPar]/F");
      newtreeGenPar->Branch("phi",genParPhi,"phi[nGenPar]/F");
      newtreeGenPar->Branch("chg",genParChg,"chg[nGenPar]/I");
      newtreeGenPar->Branch("injetPt",genParInjetPt,"injetPt[nGenPar]/F");
      newtreeGenPar->Branch("injetEta",genParInjetEta,"injetEta[nGenPar]/F");
      newtreeGenPar->Branch("injetPhi",genParInjetPhi,"injetPhi[nGenPar]/F");
      newtreeGenPar->Branch("injetDphi",genParInjetDphi,"injetDphi[nGenPar]/F");
      newtreeGenPar->Branch("injetCR",genParInjetCR,"injetCR[nGenPar]/F");
    }
    
  }
  
  
  int nJet;            // 300 is very enough!!!
  float jetPt[300];
  float jetEta[300];
  float jetPhi[300];
  
  float jetTs0[300];
  float jetTs2[300];
  float jetTs4[300];
  float jetTs0r[300];
  float jetTs2r[300];
  float jetTs4r[300];
  int jetLeading[300];

  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    
    newtreeAk3Jet->Branch("jetTs0",  jetTs0,  "jetTs0[nref]/F");
    newtreeAk3Jet->Branch("jetTs2",  jetTs2,  "jetTs2[nref]/F");
    newtreeAk3Jet->Branch("jetTs4",  jetTs4,  "jetTs4[nref]/F");
    newtreeAk3Jet->Branch("jetLeading", jetLeading,  "jetLeading[nref]/I");
    newtreeAk3Jet->Branch("jetTs0r",  jetTs0r,  "jetTs0r[nref]/F");
    newtreeAk3Jet->Branch("jetTs2r",  jetTs2r,  "jetTs2r[nref]/F");
    newtreeAk3Jet->Branch("jetTs4r",  jetTs4r,  "jetTs4r[nref]/F");
  }
  
  

  
  
  kinem goodJetf[100]; // 
  
  // For centrality reweighting
  CentralityReWeight cw(datafname,inputFile_,"");
  if (needReweight ) cw.Init();
  
  GammaJet gj;
  Isolation isol;
  EvtSel evt;
  TTree *tgj;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    tgj = new TTree("tgj","gamma jet tree");
    tgj->SetMaxTreeSize(MAXTREESIZE);
    tgj->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:reweight/F"); // todo : add ncoll later
    tgj->Branch("lpho",&gj.photonEt,"photonEt/F:photonRawEt:photonEta:photonPhi:hovere:sigmaIetaIeta:sumIso:genIso:genMomId/I:lJetPt/F:lJetEta/F:lJetPhi/I");
    tgj->Branch("isolation",&isol.cc1,"cc1:cc2:cc3:cc4:cc5:cr1:cr2:cr3:cr4:cr5:ct1PtCut20:ct2PtCut20:ct3PtCut20:ct4PtCut20:ct5PtCut20");
  }
  
  int nmjet;
  float ptMjet[10000];
  float etaMjet[10000];
  float phiMjet[10000];
  float ts0Mjet[10000];
  float ts2Mjet[10000];
  float ts4Mjet[10000];
  float ts0rMjet[10000];
  float ts2rMjet[10000];
  float ts4rMjet[10000];

  TTree *tmixJet;
  if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
    tmixJet = new TTree("tmixJet","jets from minbias events");
    tmixJet->SetMaxTreeSize(MAXTREESIZE);
    tmixJet->Branch("njet",&nmjet,"njet/I");
    tmixJet->Branch("pt",ptMjet,"pt[njet]/F");
    tmixJet->Branch("eta",etaMjet,"eta[njet]/F");
    tmixJet->Branch("phi",phiMjet,"phi[njet]/F");
    tmixJet->Branch("ts0",ts0Mjet,"ts0[njet]/F");
    tmixJet->Branch("ts2",ts2Mjet,"ts2[njet]/F");
    tmixJet->Branch("ts4",ts4Mjet,"ts4[njet]/F");
    tmixJet->Branch("ts0r",ts0rMjet,"ts0r[njet]/F");
    tmixJet->Branch("ts2r",ts2rMjet,"ts2r[njet]/F");
    tmixJet->Branch("ts4r",ts4rMjet,"ts4r[njet]/F");
  }
  
  int nMtrk;
  float ptMtrk[MAXMTRK];
  float etaMtrk[MAXMTRK];
  float phiMtrk[MAXMTRK];
  float weightMtrk[MAXMTRK];
  float dphiFgMtrk[MAXMTRK];
  float injetPtMtrk[MAXMTRK];
  float injetEtaMtrk[MAXMTRK];
  float injetPhiMtrk[MAXMTRK];
  float injetDphiMtrk[MAXMTRK];
  float injetCRMtrk[MAXMTRK];
  float xjetPtMtrk[MAXMTRK];
  float xjetEtaMtrk[MAXMTRK];
  float xjetPhiMtrk[MAXMTRK];
  float xjetDphiMtrk[MAXMTRK];
  float xjetCRMtrk[MAXMTRK];
  
  
  TTree * tmixTrk;
  if  ( (MinbiasFname !="") && ( minBiasOnminBias == false )) {
    tmixTrk = new TTree("mTrk2","Track from second minbias events");
    tmixTrk->SetMaxTreeSize(MAXTREESIZE);
    tmixTrk->Branch("ntrk",&nMtrk,"ntrk/I");
    tmixTrk->Branch("pt",ptMtrk,"pt[ntrk]/F");
    tmixTrk->Branch("eta",etaMtrk,"eta[ntrk]/F");
    tmixTrk->Branch("phi",phiMtrk,"phi[ntrk]/F");
    tmixTrk->Branch("trkWeight",weightMtrk,"trkWeight[ntrk]/F");
    tmixTrk->Branch("dphi", dphiFgMtrk, "dphi[ntrk]/F");
    
    tmixTrk->Branch("injetPt",injetPtMtrk,"injetPt[ntrk]/F");
    tmixTrk->Branch("injetEta",injetEtaMtrk,"injetEta[ntrk]/F");
    tmixTrk->Branch("injetPhi",injetPhiMtrk,"injetPhi[ntrk]/F");
    tmixTrk->Branch("injetDphi",injetDphiMtrk,"injetDphi[ntrk]/F");
    tmixTrk->Branch("injetCR",injetCRMtrk,"injetCR[ntrk]/F");
 
    // We don't need this anymore
    // tmixTrk->Branch("xjetPt",xjetPtMtrk,"xjetPt[ntrk]/F");
    // tmixTrk->Branch("xjetEta",xjetEtaMtrk,"xjetEta[ntrk]/F");
    // tmixTrk->Branch("xjetPhi",xjetPhiMtrk,"xjetPhi[ntrk]/F");
    // tmixTrk->Branch("xjetDphi",xjetDphiMtrk,"xjetDphi[ntrk]/F");
    
    
    
  }
  
  TTree *tmixTrk2;
  int nMtrk2;
  float ptMtrk2[MAXMTRK];
  float etaMtrk2[MAXMTRK];
  float phiMtrk2[MAXMTRK];
  float weightMtrk2[MAXMTRK];
  float dphiFgMtrk2[MAXMTRK];
 
  float xmjetPtMtrk[MAXMTRK];
  float xmjetEtaMtrk[MAXMTRK];
  float xmjetPhiMtrk[MAXMTRK];
  float xmjetDphiMtrk[MAXMTRK];
  float xmjetCRMtrk[MAXMTRK];

  float xjetPtMtrk2[MAXMTRK];
  float xjetEtaMtrk2[MAXMTRK];
  float xjetPhiMtrk2[MAXMTRK];
  float xjetCRMtrk2[MAXMTRK];
  float xjetDphiMtrk2[MAXMTRK];
  
  
  if  ( (MinbiasFname !="") && ( minBiasOnminBias == false )) {
    tmixTrk2 = new TTree("mTrk1","Tracks from the first - second deep -  minbias events");
    tmixTrk2->SetMaxTreeSize(MAXTREESIZE);
    tmixTrk2->Branch("ntrk",&nMtrk2,"ntrk/I");
    tmixTrk2->Branch("pt",ptMtrk2,"pt[ntrk]/F");
    tmixTrk2->Branch("eta",etaMtrk2,"eta[ntrk]/F");
    tmixTrk2->Branch("phi",phiMtrk2,"phi[ntrk]/F");
    tmixTrk2->Branch("trkWeight",weightMtrk2,"trkWeight[ntrk]/F");
    tmixTrk2->Branch("dphi", dphiFgMtrk2, "dphi[ntrk]/F");
    
    tmixTrk2->Branch("xmjetPt",xmjetPtMtrk,"xmjetPt[ntrk]/F");
    tmixTrk2->Branch("xmjetEta",xmjetEtaMtrk,"xmjetEta[ntrk]/F");
    tmixTrk2->Branch("xmjetPhi",xmjetPhiMtrk,"xmjetPhi[ntrk]/F");
    tmixTrk2->Branch("xmjetDphi",xmjetDphiMtrk,"xmjetDphi[ntrk]/F");
    tmixTrk2->Branch("xmjetCR",xmjetCRMtrk,"xmjetCR[ntrk]/F");

    tmixTrk2->Branch("xjetPt",xjetPtMtrk2,"xjetPt[ntrk]/F");                                                                     
    tmixTrk2->Branch("xjetEta",xjetEtaMtrk2,"xjetEta[ntrk]/F");                                                            
    tmixTrk2->Branch("xjetPhi",xjetPhiMtrk2,"xjetPhi[ntrk]/F");                                                             
    tmixTrk2->Branch("xjetCR",xjetCRMtrk2,"xjetCR[ntrk]/F");
    tmixTrk2->Branch("xjetDphi",xjetDphiMtrk2,"xjetDphi[ntrk]/F");                                                              
    
  }



  

  
  TTree* newtreeTrkJet[100][nVtxBin];
  if ( (MinbiasFname == "") || (minBiasOnminBias==true) ) {
    for( int icent = 0 ; icent< nCent2 ; icent++) { 
      for( int ivz = 0 ; ivz<nVtxBin ; ivz++) {
	
	if (minBiasOnminBias==false) 
	  newtreeTrkJet[icent][ivz] = new TTree(Form("trackAndJet_first_cBin2icent%d_ivz%d",icent,ivz),"track and jets");
	else
	  newtreeTrkJet[icent][ivz] = new TTree(Form("trackAndJet_cBin2icent%d_ivz%d",icent,ivz),"track and jets");
	
	newtreeTrkJet[icent][ivz]->SetMaxTreeSize(MAXTREESIZE);
	
	newtreeTrkJet[icent][ivz]->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:reweight/F");      
	newtreeTrkJet[icent][ivz]->Branch("nTrk",   &nTrk,  "nTrk/I");
	newtreeTrkJet[icent][ivz]->Branch("trkPt",  trkPt,  "trkPt[nTrk]/F");
	newtreeTrkJet[icent][ivz]->Branch("trkEta", trkEta, "trkEta[nTrk]/F");
	newtreeTrkJet[icent][ivz]->Branch("trkPhi", trkPhi, "trkPhi[nTrk]/F");
	newtreeTrkJet[icent][ivz]->Branch("trkWeight", trkWeight, "trkWeight[nTrk]/F");
	newtreeTrkJet[icent][ivz]->Branch("trkWeightPara", trkWeightPara, "trkWeightPara[nTrk]/F");

	//genparticle
	newtreeTrkJet[icent][ivz]->Branch("nGenPar",&nGenPar,  "nGenPar/I");
	newtreeTrkJet[icent][ivz]->Branch("genParSube", genParSube, "genParSube[nGenPar]/I");
	newtreeTrkJet[icent][ivz]->Branch("genParPt", genParPt, "genParPt[nGenPar]/F");
	newtreeTrkJet[icent][ivz]->Branch("genParEta", genParEta, "genParEta[nGenPar]/F");
	newtreeTrkJet[icent][ivz]->Branch("genParPhi", genParPhi, "genParPhi[nGenPar]/F");
	newtreeTrkJet[icent][ivz]->Branch("genParChg", genParChg, "genParChg[nGenPar]/I");


	if (minBiasOnminBias) {
	  newtreeTrkJet[icent][ivz]->Branch("nJet", &nJet, "nJet/I");
	  newtreeTrkJet[icent][ivz]->Branch("jetPt", jetPt, "jetPt[nJet]/F");  
	  newtreeTrkJet[icent][ivz]->Branch("jetEta", jetEta, "jetEta[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetPhi", jetPhi, "jetPhi[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetTs0",  jetTs0,  "jetTs0[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetTs2",  jetTs2,  "jetTs2[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetTs4",  jetTs4,  "jetTs4[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetTs0r",  jetTs0r,  "jetTs0r[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetTs2r",  jetTs2r,  "jetTs2r[nJet]/F");
	  newtreeTrkJet[icent][ivz]->Branch("jetTs4r",  jetTs4r,  "jetTs4r[nJet]/F");
	
	  newtreeTrkJet[icent][ivz]->Branch("injetPt", injetPt, "injetPt[nTrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("injetEta", injetEta, "injetEta[nTrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("injetPhi", injetPhi, "injetPhi[nTrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("injetCR", injetCR, "injetCR[nTrk]/F");

	  /// mixed part from MB 
	  newtreeTrkJet[icent][ivz]->Branch("nMtrk",&nMtrk,"nMtrk/I");
	  newtreeTrkJet[icent][ivz]->Branch("ptMtrk",ptMtrk,"ptMtrk[nMtrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("weightMtrk",weightMtrk,"weightMtrk[nMtrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("etaMtrk",etaMtrk,"etaMtrk[nMtrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("phiMtrk",phiMtrk,"phiMtrk[nMtrk]/F");
	  
	  newtreeTrkJet[icent][ivz]->Branch("xjetPtMtrk",xjetPtMtrk,"xjetPtMtrk[nMtrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("xjetEtaMtrk",xjetEtaMtrk,"xjetEtaMtrk[nMtrk]/F");
	  newtreeTrkJet[icent][ivz]->Branch("xjetPhiMtrk",xjetPhiMtrk,"xjetPhiMtrk[nMtrk]/F");	
	  newtreeTrkJet[icent][ivz]->Branch("xjetCRMtrk",xjetCRMtrk,"xjetCRMtrk[nMtrk]/F");
	}
	
      }
    }
  }
  
  
  /// input from MB events for MB on MB mixing!  
  
   TChain         *tjmb[100][nVtxBin];

   Int_t           nimbj;
   Float_t         imbjpt[1000];
   Float_t         imbjeta[1000];
   Float_t         imbjphi[1000];
   Float_t         imbjts0[1000];
   Float_t         imbjts2[1000];
   Float_t         imbjts4[1000];
   Float_t         imbjts0r[1000];
   Float_t         imbjts2r[1000];
   Float_t         imbjts4r[1000];

   Int_t           nimbTrk;
   Float_t         imbTrkpt[10000];
   Float_t         imbTrketa[10000];
   Float_t         imbTrkphi[10000];
   Float_t         imbTrkWeight[10000];
   float          imbTrkInjetPt[10000];
   float          imbTrkInjetPhi[10000];
   float          imbTrkInjetEta[10000];
   float          imbTrkInjetCR[10000];
   EvtSel          imbEvt;
   
  
   // should be taken care of
   Int_t           nMtrkImb;
   Float_t         ptMtrkImb[12000];   //[nMtrk]                      
   Float_t         etaMtrkImb[12000];   //[nMtrk]                                                            
   Float_t         phiMtrkImb[12000];   //[nMtrk]     
   Float_t         weightMtrkImb[12000];   //[nMtrk]                                                   
   Float_t         xjetPtMtrkImb[12000];
   Float_t         xjetEtaMtrkImb[12000];
   Float_t         xjetPhiMtrkImb[12000];
   Float_t         xjetCRMtrkImb[12000];
   
   TBranch        *b_nimbj;
   TBranch        *b_imbjpt;
   TBranch        *b_imbjphi;
   TBranch        *b_imbjeta;
   TBranch        *b_imbjts0;
   TBranch        *b_imbjts2;
   TBranch        *b_imbjts4;
   TBranch        *b_imbjts0r;
   TBranch        *b_imbjts2r;
   TBranch        *b_imbjts4r;
   TBranch        *b_imbTrkWeight;

   TBranch        *b_nimbTrk;
   TBranch        *b_imbTrkpt;
   TBranch        *b_imbTrketa;
   TBranch        *b_imbTrkphi;
   TBranch        *b_imbTrkWeigh;
   TBranch        *b_imbTrkInjetPt;
   TBranch        *b_imbTrkInjetEta;
   TBranch        *b_imbTrkInjetPhi;
   TBranch        *b_imbTrkInjetCR;
   TBranch        *b_imbEvt;   
   TBranch        *b_nMtrkImb;   //!                                               
   TBranch        *b_ptMtrkImb;   //!                                              
   TBranch        *b_etaMtrkImb;   //!                                             
   TBranch        *b_phiMtrkImb;   //!                                             
   TBranch        *b_weightMtrkImb;   //!                                                                                                                                              
   TBranch        *b_xjetPtMtrkImb;   //!                                                                   
   TBranch        *b_xjetEtaMtrkImb;   //!                                         
   TBranch        *b_xjetPhiMtrkImb;   //!                                         
   TBranch        *b_xjetCRMtrkImb;
   
   
   int nMB[100][nVtxBin] ; //= 199109;                                                                                          
   int mbItr[100][nVtxBin];
   if (MinbiasFname !="") {
     for( int icent = 0 ; icent<nCent2 ; icent++) {
       for( int ivz = 0 ; ivz<nVtxBin ; ivz++) {
	 if (minBiasOnminBias==true)
	   tjmb[icent][ivz] = new TChain(Form("trackAndJet_first_cBin2icent%d_ivz%d",icent,ivz));
	 else
	   tjmb[icent][ivz] = new TChain(Form("trackAndJet_cBin2icent%d_ivz%d",icent,ivz));
	 
	 
	 tjmb[icent][ivz]->Add(MinbiasFname);
	 
	 tjmb[icent][ivz]->SetBranchAddress("evt", &imbEvt, &b_imbEvt);
	 tjmb[icent][ivz]->SetBranchAddress("nTrk", &nimbTrk ,&b_nimbTrk);
	 tjmb[icent][ivz]->SetBranchAddress("trkPt", imbTrkpt, &b_imbTrkpt);
	 tjmb[icent][ivz]->SetBranchAddress("trkEta", imbTrketa, &b_imbTrketa);
	 tjmb[icent][ivz]->SetBranchAddress("trkPhi", imbTrkphi, &b_imbTrkphi);
	 tjmb[icent][ivz]->SetBranchAddress("trkWeight", imbTrkWeight, &b_imbTrkWeight);
	 
	 if ( minBiasOnminBias == false ) {  // is this is minbias + gammajet mixing                            
	   tjmb[icent][ivz]->SetBranchAddress("nJet", &nimbj ,&b_nimbj);
	   tjmb[icent][ivz]->SetBranchAddress("jetPt", imbjpt, &b_imbjpt);
	   tjmb[icent][ivz]->SetBranchAddress("jetEta", imbjeta, &b_imbjeta);
	   tjmb[icent][ivz]->SetBranchAddress("jetPhi", imbjphi, &b_imbjphi);
	   tjmb[icent][ivz]->SetBranchAddress("jetTs0", imbjts0, &b_imbjts0);
	   tjmb[icent][ivz]->SetBranchAddress("jetTs2", imbjts2, &b_imbjts2);
	   tjmb[icent][ivz]->SetBranchAddress("jetTs4", imbjts4, &b_imbjts4);
	   
	   tjmb[icent][ivz]->SetBranchAddress("jetTs0r", imbjts0r, &b_imbjts0r);
	   tjmb[icent][ivz]->SetBranchAddress("jetTs2r", imbjts2r, &b_imbjts2r);
	   tjmb[icent][ivz]->SetBranchAddress("jetTs4r", imbjts4r, &b_imbjts4r);
	   tjmb[icent][ivz]->SetBranchAddress("nMtrk", &nMtrkImb, &b_nMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("ptMtrk", ptMtrkImb, &b_ptMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("etaMtrk", etaMtrkImb, &b_etaMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("phiMtrk", phiMtrkImb, &b_phiMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("weightMtrk", weightMtrkImb, &b_weightMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("injetPt", imbTrkInjetPt, &b_imbTrkInjetPt);    // should be removed
	   tjmb[icent][ivz]->SetBranchAddress("injetEta", imbTrkInjetEta, &b_imbTrkInjetEta);
	   tjmb[icent][ivz]->SetBranchAddress("injetPhi", imbTrkInjetPhi, &b_imbTrkInjetPhi);
	   tjmb[icent][ivz]->SetBranchAddress("injetCR",  imbTrkInjetCR, &b_imbTrkInjetCR);
	   
	   tjmb[icent][ivz]->SetBranchAddress("xjetPtMtrk",  xjetPtMtrkImb,  &b_xjetPtMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("xjetEtaMtrk", xjetEtaMtrkImb, &b_xjetEtaMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("xjetPhiMtrk", xjetPhiMtrkImb, &b_xjetPhiMtrkImb);
	   tjmb[icent][ivz]->SetBranchAddress("xjetCRMtrk",  xjetCRMtrkImb, &b_xjetCRMtrkImb);
	   
	 }
	 
	 nMB[icent][ivz] = tjmb[icent][ivz]->GetEntries();
	 cout << "number of evetns in icent = " << icent << ", vz = "<< ivz << "= " << nMB[icent][ivz] << endl;
	 int primeSeed = rand.Integer(397324);
	 mbItr[icent][ivz] = primeSeed%(nMB[icent][ivz]);
	 cout <<" initial itr = " << mbItr[icent][ivz] << endl;
       }
     }
   }
   
 
   // vertex histogram 
   TH1F* hvz = new TH1F("hvz","",nVtxBin,-vtxCut,vtxCut); 
   // event plane hitogram
   TH1F* hEvtPlnBin = new TH1F("hEvtPlnBin", "", nPlnBin*5, -4.*PI, PI); 
   const int theEvtPlNumber = 21;
   
   /// LOOP!!
   
   int nentries = c->GetEntries();
   if (maxEvents > 0 ) 
     nentries = maxEvents;
   cout << "number of entries = " << nentries << endl;
   int startEntry = 0;
   //   if  ( (MinbiasFname =="") && ( isMC == false ) ) 
   //   if ( (MinbiasFname =="") && (!isMC) )
   //   startEntry = (int)c->GetEntries()/2  ;
   if ( startEntry > nentries ) {
     cout << "error! startEntry > nentries!!!" << endl;
     return;
   }
   
   for (Long64_t jentry=startEntry ; jentry<nentries;jentry++) {
     
     if (jentry% 1000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
     
     c->GetEntry(jentry);
     
     // For MinBias events!!! 

     if (   (isMC==false)  && ((MinbiasFname == "")||(minBiasOnminBias==true))  && (  c->hlt.HLT_HIMinBiasHfOrBSC_v1 == false ) )
       continue;

     //here
     evt.run   = c->evt.run;
     evt.evt = c->evt.evt;
     evt.cBin = c->evt.hiBin;
     evt.pBin   = hEvtPlnBin->FindBin( c->evt.hiEvtPlanes[theEvtPlNumber] ) ;
     evt.nG = c->photon.nPhotons;
     evt.nJ = c->icPu5.nref;
     evt.nT = c->track.nTrk;
     evt.reweight = 1;
     if (needReweight) evt.reweight = cw.GetWeight(c->evt.hiBin);
     evt.trig = (c->hlt.HLT_HISinglePhoton30_v2 > 0);
     evt.offlSel = (c->skim.pcollisionEventSelection > 0);
     evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
     evt.anaEvtSel = c->selectEvent() && evt.trig;
     evt.vz = c->evt.vz;

     // vertex bin and cut!! 

     int vzBin = hvz->FindBin(evt.vz) -1 ;
     if ( (vzBin<0) || ( vzBin >= nVtxBin) ) 
       continue;;

     //restart from here
     // New centrality bin 
     int cBin2 = evt.cBin;     //   cent2Hist->FindBin(c->evt.hiHF) -1 ; // ok.. go back to 40 centrality bins
     // calculate corrected pt
     for (int j=0;j< c->photon.nPhotons;j++) {
       if (  ( c->photon.pt[j] > preCutPhotonEt ) && ( fabs( c->photon.eta[j] ) < cutphotonEta ) )
	 newPt[j] = c->getCorrEt(j);
       else 
	 newPt[j]  =  c->photon.pt[j] - 10000;
       corrPt[j] = newPt[j];
       
       //  Count out spikes, HoE < 0.2   These should be always synchrinized with the loose cut below. 
       if ( (c->isSpike(j)) || (c->photon.hadronicOverEm[j]>0.2) ||  (c->photon.isEle[j]) )
	  newPt[j] = newPt[j] - 5000;
     }

      // sort the order of photon pt
      TMath::Sort(c->photon.nPhotons, newPt, order);
      
 
      int leadingIndex=-1;
      // Good jet not from photon!!!!
      gj.clear();
      for (int j=0;j<c->photon.nPhotons;j++) {
	
	// loose cut!!! 
	if ( c->photon.pt[j]  < preCutPhotonEt ) continue; 
	if ( fabs(c->photon.eta[j]) > cutphotonEta ) continue;
	if (c->isSpike(j)) continue;       
	if (!(c->isLoosePhoton(j))) continue;
	
	// sort using corrected photon pt                                                                                                                                                     
	float theCorrPt= corrPt[j];
	if ( theCorrPt > gj.photonEt) {
	  gj.photonEt = theCorrPt;
	  leadingIndex = j;
	}
      }

      if ( ( cutphotonPt > 0 )  && ( gj.photonEt < cutphotonPt ) )   // then we don't save the events.
	continue;
      if ( ( cutphotonPt > 0 ) && (c->photon.hadronicOverEm[leadingIndex] > 0.1 ) ) // Only H/E<0.1 allowed
	continue;
      if ( c->skim.pcollisionEventSelection != 1 ) // Now, let's save the collision events only
	continue;
      if ( (!isMC) && (c->skim.pHBHENoiseFilter != 1) ) 
	continue; 
      
   
      // give the leading bit to the photon candidates 
      for (int j=0;j<c->photon.nPhotons;j++) {
	if ( j==leadingIndex)       isLeading[j] = 1; 
	else                        isLeading[j] = 0;
      }
      /// Save leading photons 
      if (leadingIndex!=-1) {
	gj.photonRawEt=c->photon.pt[leadingIndex];
	gj.photonEta=c->photon.eta[leadingIndex];
	gj.photonPhi=c->photon.phi[leadingIndex];
	gj.hovere=c->photon.hadronicOverEm[leadingIndex];
	gj.sigmaIetaIeta=c->photon.sigmaIetaIeta[leadingIndex];
        gj.sumIsol=(c->photon.cr4[leadingIndex]+c->photon.cc4[leadingIndex]+c->photon.ct4PtCut20[leadingIndex])/0.9;
	gj.genIso   = c->photon.genCalIsoDR04[leadingIndex];
	gj.genMomId = c->photon.genMomId[leadingIndex];
	isol.Set(c,leadingIndex);
      }
      else {
	gj.clear();
      }
      
      ///////// Leading photon is all set
      /// Now jet and tracks

      // make the list of high pt jets 
      nJet = c->akPu3PF.nref; 
      int nGoodj=0;
      float maxJpt = -1;
      int jetLeadingIndex = -1;
      for (int ij=0; ij< nJet ; ij++) {
	jetPt[ij] = c->akPu3PF.jtpt[ij];
	jetEta[ij] = c->akPu3PF.jteta[ij];
	jetPhi[ij] = c->akPu3PF.jtphi[ij];
	jetLeading[ij] = 0;
		
	
	if ( c->akPu3PF.jtpt[ij] <  cutjetPtPre)
	  continue;
	if ( fabs(c->akPu3PF.jteta[ij]) > 2.0 ) // Used for the good jet.... should be within |eta|<2 
	  continue;
        if ( ((MinbiasFname != "") && (minBiasOnminBias==false)) &&  ( getDR( c->akPu3PF.jteta[ij],  c->akPu3PF.jtphi[ij], gj.photonEta, gj.photonPhi) < 0.5 ) ) 
	  continue;
	  
	
	
	goodJetf[nGoodj].pt = c->akPu3PF.jtpt[ij] ;
	goodJetf[nGoodj].eta = c->akPu3PF.jteta[ij] ;
	goodJetf[nGoodj].phi = c->akPu3PF.jtphi[ij] ;
	nGoodj++;

	// Find the leading jet
	if ( gj.photonEt > 0 ) { 
	  if ( acos(cos ( gj.photonPhi - c->akPu3PF.jtphi[ij]) ) < 0.3 )
	    continue; // not a leading jet candidate
	  if ( c->akPu3PF.jtpt[ij] > maxJpt) {
	    maxJpt = c->akPu3PF.jtpt[ij] ;
	    jetLeadingIndex = ij;
	  }
	} 
	else {
	  jetLeadingIndex = -1 ; 
	}

      }
      if ( jetLeadingIndex > -1 ) {
	jetLeading[jetLeadingIndex] = 1;
      	gj.lJetPt = c->akPu3PF.jtpt[jetLeadingIndex];
        gj.lJetEta = c->akPu3PF.jteta[jetLeadingIndex];
        gj.lJetPhi = c->akPu3PF.jtphi[jetLeadingIndex];
      }
      

      ////////// Add track sum pT of jet //////
       for (int ij=0; ij< c->akPu3PF.nref ; ij++) {
	jetTs0[ij] = 0;
	jetTs2[ij] = 0;
        jetTs4[ij] = 0;
	for (int it=0; it < c->track.nTrk; it++ ) {
	  if (  (c->track.trkNHit[it] > 8) && ( c->track.highPurity[it] == 0) )
	    continue;   // only high purity high pt tracks and all pixel track                 
	  
	  if ( getDR( c->track.trkEta[it],  c->track.trkPhi[it], c->akPu3PF.jteta[ij] , c->akPu3PF.jtphi[ij] ) > drCutTJ )
	    continue;

	  jetTs0[ij]  = jetTs0[ij] +  c->track.trkPt[it];
	  if ( c->track.trkPt[it] > 2 )   jetTs2[ij]  =jetTs2[ij] +  c->track.trkPt[it];
	  if ( c->track.trkPt[it] > 4 )   jetTs4[ij]  =jetTs4[ij] +  c->track.trkPt[it];
	}
      }
       //      cout <<"  here 1g "<< endl;

      //      cout << " track.nTrk = " << c->track.nTrk << endl;

      //////////////////////////////////////////// New collection of tracks
       nTrk = 0; 
       for (int it=0; it < c->track.nTrk; it++ ) { 
	 //if ( c->track.trkPt[it] < trkBoundary )   continue;
	 if (  (c->track.trkNHit[it] > 8) && ( c->track.highPurity[it] == 0) ) 
	   continue;   // only high purity high pt tracks and all pixel track

	 trkPt[nTrk]  = c->track.trkPt[it];
	 trkEta[nTrk] = c->track.trkEta[it];
	 trkPhi[nTrk] = c->track.trkPhi[it]; 
	 

	 if ( trkPt[nTrk] < cuttrkPt ) 
	   continue;
	 if ( fabs(trkEta[nTrk]) > cuttrkEta )
           continue;

	 float assoJpt=0;
	 for ( int ij = 0 ; ij < nGoodj ; ij++) {
	   if ( getDR( trkEta[nTrk], trkPhi[nTrk], goodJetf[ij].eta, goodJetf[ij].phi ) < 0.5 ) {
	     if ( assoJpt <  goodJetf[ij].pt ) {
	       assoJpt = goodJetf[ij].pt;
	     }
	   } 
	 }

	 trkWeight[nTrk] = c->getTrackCorrection(it);
	 trkWeightPara[nTrk]= c->getTrackCorrectionPara(it);
	 nTrk++;
       }

      ////////////////////////////////////////////////////
      for (int it=0; it < nTrk ; it++ ) {
	
	dphiFg[it] = PI - fabs(PI - fabs(trkPhi[it] - gj.photonPhi) );
	injetPt[it]   =  -99999;
        injetEta[it]  =  -99999; 
        injetPhi[it]  =  -99999;
        injetDphi[it] =  -99999;
	injetCR[it]   =   99999;

	for ( int ij = 0 ; ij < nGoodj ; ij++) {
	  if ( getDR( trkEta[it], trkPhi[it], goodJetf[ij].eta, goodJetf[ij].phi ) < drCutTJ ) {
	    if ( injetPt[it] < goodJetf[ij].pt ) {
	      injetPt[it]  = goodJetf[ij].pt; // here
              injetEta[it]  = goodJetf[ij].eta; // here                             
	      injetPhi[it]  = goodJetf[ij].phi; // here                                            
	      injetDphi[it] = acos( cos ( injetPhi[it] - gj.photonPhi ) ); 
	      injetCR[it]   = getDR( trkEta[it], trkPhi[it], goodJetf[ij].eta, goodJetf[ij].phi );
	    }
	  }
	}
	
      }
      
      
      
      

      
      
      /////////////////////////// Add rundom jet anlaysis
      
      for (int ij=0; ij< c->akPu3PF.nref ; ij++) {
	jetTs0r[ij] = -9999999;
	jetTs2r[ij] = -9999999;
	jetTs4r[ij] = -9999999;
	continue;
	
	if ( fabs(c->akPu3PF.jteta[ij]) > cutjetEtaPre ) 
	  continue;
	
	jetTs0r[ij] = 0;
	jetTs2r[ij] = 0;
        jetTs4r[ij] = 0;
	int rCounter=0;
	while ( (MinbiasFname !="") && (rCounter<nMixing2) ) {
	  
	  mbItr[cBin2][vzBin] = mbItr[cBin2][vzBin] + 1 ;
          if ( mbItr[cBin2][vzBin] == nMB[cBin2][vzBin] )
            mbItr[cBin2][vzBin] =  mbItr[cBin2][vzBin] - nMB[cBin2][vzBin];
	  tjmb[cBin2][vzBin]->GetEntry(mbItr[cBin2][vzBin]);

	  // if ( (takeCarePBin) && (!isMC) && ( evt.pBin != imbEvt.pBin )  )   // Only applied for data. 
	  if ( (takeCarePBin) && ( evt.pBin != imbEvt.pBin )  )   // Only applied for data. 
	    continue;
	  
	  for (int it = 0 ; it < nimbTrk ; it++) {
	    if ( getDR( imbTrketa[it],  imbTrkphi[it], c->akPu3PF.jteta[ij] , c->akPu3PF.jtphi[ij] ) > drCutTJ )
	      continue;

	    jetTs0r[ij] = jetTs0r[ij] + imbTrkpt[it] ;
	    if ( imbTrkpt[it] > 2)   jetTs2r[ij] = jetTs2r[ij] + imbTrkpt[it] ;
	    if ( imbTrkpt[it] > 4)   jetTs4r[ij] = jetTs4r[ij] +imbTrkpt[it] ;
	  }
	  rCounter++;
	}
	jetTs0r[ij] = jetTs0r[ij]/(float)nMixing2;
	jetTs2r[ij] = jetTs2r[ij]/(float)nMixing2;
	jetTs4r[ij] = jetTs4r[ij]/(float)nMixing2;
      }
      ////////////////////////////////////////////      
       

      /// Gen Particle 
      nGenPar = 0;
      if ( isMC ) {
	for ( int ig = 0 ; ig < c->genparticle.mult ; ig++) {
	  if ( c->genparticle.pt[ig] < 0.5 )  /// not possibly though..
	    continue;
	  if ( fabs( c->genparticle.eta[ig] ) > 2.0 ) 
	    continue;
	  
	  genParChg[nGenPar] =  c->genparticle.chg[ig] ;
	  genParPt[nGenPar]  = c->genparticle.pt[ig];
	  genParEta[nGenPar] = c->genparticle.eta[ig];
	  genParPhi[nGenPar] = c->genparticle.phi[ig];	
	  genParSube[nGenPar] = c->genparticle.sube[ig];
	  nGenPar++;
	}
      }
      
      for (int it=0; it < nGenPar ; it++ ) {

	genParInjetPt[it]   =  -99999;
        genParInjetEta[it]  =  -99999;
        genParInjetPhi[it]  =  -99999;
        genParInjetDphi[it] =  -99999;
        genParInjetCR[it]   =   99999;

        for ( int ij = 0 ; ij < nGoodj ; ij++) {
	  float tempDR = getDR( genParEta[it], genParPhi[it], goodJetf[ij].eta, goodJetf[ij].phi) ;
	  if ( tempDR < drCutTJ ) {
            if ( genParInjetPt[it] < goodJetf[ij].pt ) {
              genParInjetPt[it]  = goodJetf[ij].pt; // here                                                                       
	      genParInjetEta[it]  = goodJetf[ij].eta; // here                                                                      
	      genParInjetPhi[it]  = goodJetf[ij].phi; // here                                                                  
	      genParInjetDphi[it] = acos( cos ( genParInjetPhi[it] - gj.photonPhi ) );
              genParInjetCR[it]   = tempDR ;
            }
          }
	}
	
      }
      
      

      
      int iCounter=0;
      int nMixing = nMixing1;
      if ( minBiasOnminBias == true ) 
	nMixing = nMixing2;
      
      nmjet =0;
      nMtrk =0;
      nMtrk2 = 0;

      while ( (MinbiasFname !="") && (iCounter<nMixing) ) {

	mbItr[cBin2][vzBin] = mbItr[cBin2][vzBin] + 1 ;
	if ( mbItr[cBin2][vzBin] == nMB[cBin2][vzBin] )
	  mbItr[cBin2][vzBin] =  mbItr[cBin2][vzBin] - nMB[cBin2][vzBin];
	
	tjmb[cBin2][vzBin]->GetEntry(mbItr[cBin2][vzBin]);
	
	//if ( (!isMC) && (takeCarePBin) && ( evt.pBin != imbEvt.pBin )  )   // Only applied for data.
	if ( (takeCarePBin) && ( evt.pBin != imbEvt.pBin )  )   // Only applied for data.
	  continue;
	
       	
	for (int ij = 0 ; ij<nimbj ; ij++) {
	  if ( imbjpt[ij] < cutjetPtPre)
	    continue;
	  if ( fabs(imbjeta[ij]) > cutjetEtaPre)
	    continue;
	  
	  ptMjet[nmjet]  =  imbjpt[ij];
	  etaMjet[nmjet] =  imbjeta[ij];
          phiMjet[nmjet] =  imbjphi[ij];
	  
	  ts0Mjet[nmjet] =  imbjts0[ij];
          ts2Mjet[nmjet] =  imbjts2[ij];
          ts4Mjet[nmjet] =  imbjts4[ij];
          
	  if ( minBiasOnminBias == false ) {  // is this is minbias + gammajet mixing     
	    ts0rMjet[nmjet] =  imbjts0r[ij];
	    ts2rMjet[nmjet] =  imbjts2r[ij];
	    ts4rMjet[nmjet] =  imbjts4r[ij];
	  }
	  
	  nmjet++;
	}
	
        for (int it = 0 ; it < nimbTrk ; it++) {
	  if ( imbTrkpt[it] < cuttrkPt )
	    continue;
	  if ( fabs(imbTrketa[it]) > cuttrkEta ) 
	    continue;
	  
	  ptMtrk[nMtrk]  =   imbTrkpt[it];
          etaMtrk[nMtrk] =   imbTrketa[it];
          phiMtrk[nMtrk] =   imbTrkphi[it];
	  weightMtrk[nMtrk] = imbTrkWeight[it];

          dphiFgMtrk[nMtrk] = PI - fabs(PI - fabs( phiMtrk[nMtrk] - gj.photonPhi ) );

	  injetPtMtrk[nMtrk] = imbTrkInjetPt[it];
	  injetEtaMtrk[nMtrk] = imbTrkInjetEta[it];
          injetPhiMtrk[nMtrk] = imbTrkInjetPhi[it];
	  injetCRMtrk[nMtrk]  = imbTrkInjetCR[it];
	  injetDphiMtrk[nMtrk] = PI - fabs(PI - fabs( injetPhiMtrk[nMtrk] - gj.photonPhi ) );

	  xjetPtMtrk[nMtrk] =  -99999;
          xjetEtaMtrk[nMtrk] = -99999;// etaMtrk[nMtrk];
          xjetPhiMtrk[nMtrk] = -99999;// phiMtrk[nMtrk];
	  xjetDphiMtrk[nMtrk]= -99999;// PI - fabs(PI - fabs( xjetPhiMtrk[nMtrk] - gj.photonPhi ) );
	  xjetCRMtrk[nMtrk]  =  99999;
	  for ( int ij = 0 ; ij < nGoodj ; ij++) {
	    if ( getDR( etaMtrk[nMtrk],  phiMtrk[nMtrk], goodJetf[ij].eta, goodJetf[ij].phi ) < drCutTJ ) {
	      if ( xjetPtMtrk[nMtrk] < goodJetf[ij].pt ) {
		xjetPtMtrk[nMtrk]   = goodJetf[ij].pt; // here                                              
		xjetEtaMtrk[nMtrk]  = goodJetf[ij].eta; // here                                 
		xjetPhiMtrk[nMtrk]  = goodJetf[ij].phi; // here                             
		xjetDphiMtrk[nMtrk] = PI - fabs(PI - fabs( xjetPhiMtrk[nMtrk] - gj.photonPhi ) );
		xjetCRMtrk[nMtrk]    =  getDR( etaMtrk[nMtrk],  phiMtrk[nMtrk], goodJetf[ij].eta, goodJetf[ij].phi ) ;
	      }
	    }
	  }
	  
	  nMtrk++;
	}
	
	if ( (MinbiasFname != "") && (minBiasOnminBias==false) ) {
	  for (int it = 0 ; it < nMtrkImb ; it++) {
	    if ( ptMtrkImb[it] < cuttrkPt )
	      continue;
	    if ( fabs(etaMtrkImb[it]) > cuttrkEta )
	      continue;
	    
	    ptMtrk2[nMtrk2]  = ptMtrkImb[it];
            etaMtrk2[nMtrk2]  = etaMtrkImb[it];
	    phiMtrk2[nMtrk2]  = phiMtrkImb[it];
	    weightMtrk2[nMtrk2]  = weightMtrkImb[it];
	    
	    dphiFgMtrk2[nMtrk2] = PI - fabs(PI - fabs( phiMtrk2[nMtrk2] - gj.photonPhi ) );
	    xmjetPtMtrk[nMtrk2] = xjetPtMtrkImb[it];
            xmjetEtaMtrk[nMtrk2] = xjetEtaMtrkImb[it];
	    xmjetPhiMtrk[nMtrk2] = xjetPhiMtrkImb[it];
	    xmjetCRMtrk[nMtrk2]  = xjetCRMtrkImb[it];
	    xmjetDphiMtrk[nMtrk2] = PI - fabs(PI - fabs( xmjetPhiMtrk[nMtrk2] - gj.photonPhi ) );

	    
	    xjetPtMtrk2[nMtrk2] = -99999;
	    xjetEtaMtrk2[nMtrk2]= -99999;
            xjetPhiMtrk2[nMtrk2]= -99999;
            xjetCRMtrk2[nMtrk2] =  99999;
	    xjetDphiMtrk2[nMtrk2]= -99999;
	    
	    for ( int ij = 0 ; ij < nGoodj ; ij++) {
	      if ( getDR(  etaMtrk2[nMtrk2],  phiMtrk2[nMtrk2], goodJetf[ij].eta, goodJetf[ij].phi ) < drCutTJ ) {
		if ( xjetPtMtrk2[nMtrk2] < goodJetf[ij].pt ) {
		  xjetPtMtrk2[nMtrk2]  = goodJetf[ij].pt;
		  xjetEtaMtrk2[nMtrk2] = goodJetf[ij].eta;
		  xjetPhiMtrk2[nMtrk2] = goodJetf[ij].phi;
		  xjetCRMtrk2[nMtrk2]  = getDR(  etaMtrk2[nMtrk2],  phiMtrk2[nMtrk2], goodJetf[ij].eta, goodJetf[ij].phi ) ;
		  xjetDphiMtrk2[nMtrk2] = PI - fabs(PI - fabs( xjetPhiMtrk2[nMtrk2] - gj.photonPhi ) );
		}
	      }
	    }
	    
	    
	    nMtrk2++;
	  }
	}
	
	
	iCounter++;
      }
      

      if ( (MinbiasFname != "") && (minBiasOnminBias==false) )     { // This saves the file size (by factor of 5 ) 
	tgj->Fill();
	//	newtreehlt->Fill();
	newtreeSkim->Fill();
	newtreeEvt->Fill();
	newtree->Fill();
	newtreeAk3Jet->Fill();
	newtreeTrack->Fill();
	tmixJet->Fill();
	tmixTrk->Fill();
	tmixTrk2->Fill();
	
	if ( isMC ) 
	  newtreeGenPar->Fill();
      }

      
      //      if ( isGen )
      //	newtreeGen->Fill();
      
      if ( (MinbiasFname == "") || ( minBiasOnminBias ) ) 
	{
	  
	  if ( (isMC ==1 ) || ((evt.offlSel==1)&&(evt.noiseFilt==1)) )  {// ONLY clean MB events 
	    newtreeTrkJet[cBin2][vzBin]->Fill();
	  }
	}
      
   }
   
   
   // After Event Loop                                                                   
   /*
   if ( (MinbiasFname == "") || ( minBiasOnminBias ) ) {
     for( int icent = 0 ; icent<40 ; icent++) 
       newtreeTrkJet[icent][ivz]->Write() ;
   }
   else {
     tgj->Write();
     newtree->Write();
     newtreeAk3Jet->Write();
     newtreeTrack->Write();
     tmixJet->Write();
     tmixTrk->Write();
     tmixTrk2->Write();
     newtreehlt->Write();
     newtreeSkim->Write();
     newtreeEvt->Write();
   }
   
   */
   

   newfile_data->Write();
   //   newfile_data->Close();   // <<=== If there is close() function. writing stucks in the middle of looping.. I don't know why!!
   cout << " Done! "<< endl;
}



