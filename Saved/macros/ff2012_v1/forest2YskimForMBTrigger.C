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
using namespace std;

#define PI 3.141592653589
static const int MAXTRK = 20000;

int getNcoll(int cBin=0);

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
      
      TCut addCut = "pt[0]>60 && abs(eta[0])<1.44 && swissCrx[0]<0.9";
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
   
      hCentData->Scale(1./hCentData->Integral());
      hCentMc->Scale(1./hCentMc->Integral());
      hReWt->Divide(hCentData,hCentMc);
      
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



class EvtSel {
public:
  int run;
  int evt;
  int cBin;
  int pBin;
  int nG;
  int nJ;
  int nT;
  bool trig;
  bool offlSel;
  bool noiseFilt;
  bool anaEvtSel;
  float vz;
  float reweight;
  
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

class GammaJet {
public:
  GammaJet() :
    photonEt(-99),photonRawEt(-99),
    photonEta(0),photonPhi(0),
    hovere(10000),
    sigmaIetaIeta(-99),
    sumIsol(-9999),
    genIso(99999),
    genMomId(-999999)
  {}
  float photonEt,photonRawEt;
  float photonEta;
  float photonPhi;
  float hovere,sigmaIetaIeta, sumIsol;
  float genIso;
  int genMomId;

  void clear() {
    photonRawEt=-99; photonEt = -99999; photonEta=100; photonPhi=1000;
    sigmaIetaIeta=-99;
    sumIsol = -99;
    genIso = 9990;
    genMomId = -999999;
    hovere  =  1000;
  }
};




void forest2YskimForMBTrigger(TString inputFile_="mc/photon50_25k.root", std::string outname = "barrelPhoton50_25k.root",float cutphotonPt  = 45, bool needReweight=true, int maxEvents=-1, TString MinbiasFname="", bool isMC=false) {  // isMC is used for the HbHe noise in MB mixing
  //   cout << "Output file " << outname << endl;                             
  // Data weighting funcition                                                                                  
  
  TString datafname  = "yskim_HiForestPhoton-v7-noDuplicate_nMinBisMixedQuarterPI.root";   //used for reweighting
  float cutphotonEta = 1.44;
  float preCutPhotonEt = 10;
   
  const int nMaxPho = 300;
  
  HiForest *c = new HiForest(inputFile_.Data());
  c->GetEnergyScaleTable("../forest/photonEnergyScaleTable_Hydjet_GammaJet.root");
  
  // now open new root file
  TFile* newfile_data = new TFile(outname.data(),"recreate");
   
  TTree* newtree = c->photonTree->CloneTree(0);
  newtree->SetName("yongsunPhotonTree");
  
  TTree* newtreehlt = c->hltTree->CloneTree(0);
  newtreehlt->SetName("yongsunHltTree");
  
  TTree* newtreeSkim = c->skimTree->CloneTree(0);
  newtreeSkim->SetName("yongsunSkimTree");
 
  TTree* newtreeEvt = c->evtTree->CloneTree(0);
  newtreeEvt->SetName("yongsunHiEvt");
  
  TTree* newtreeAk3Jet = c->akPu3jetTree->CloneTree(0);
  newtreeAk3Jet->SetName("yongsunJetakPu3PF");
  
  TTree* newtreeTrack = c->trackTree->CloneTree(0);
  newtreeTrack->SetName("yongsunTrack");
    
   
  
  int isGen(false);
  TTree* newtreeGen;
  if ( c->genpTree !=0 ) { 
    newtreeGen = c->genpTree->CloneTree(0);
    newtreeGen->SetName("yongsunGen");
    isGen = true;
  }
  newtree->SetMaxTreeSize(4000000000);
  newtreehlt->SetMaxTreeSize(4000000000);
  newtreeSkim->SetMaxTreeSize(4000000000);
  newtreeEvt->SetMaxTreeSize(4000000000);
  newtreeAk3Jet->SetMaxTreeSize(4000000000);
  newtreeTrack->SetMaxTreeSize(4000000000);
  if ( isGen)     newtreeGen->SetMaxTreeSize(4000000000);
   

  
  // more bits for photon tree
  float newPt[nMaxPho];      //     <==  temporary data storage
  int isLeading[nMaxPho];
  int order[nMaxPho];
  float corrPt[nMaxPho];
  newtree->Branch("order",  order, "order[nPhotons]/I");
  newtree->Branch("corrPt", corrPt,"corrPt[nPhotons]/F");
  newtree->Branch("leading",isLeading,"leading[nPhotons]/I");
  

  float injetPt[MAXTRK];
  float injetEta[MAXTRK];
  float injetPhi[MAXTRK];

  newtreeTrack->Branch("injetPt", injetPt, "injetPt[nTrk]/F");
  newtreeTrack->Branch("injetEta", injetEta, "injetEta[nTrk]/F");
  newtreeTrack->Branch("injetPhi", injetPhi, "injetPhi[nTrk]/F");

  
  kinem goodJetf[100]; // 
  
  // For centrality reweighting
  CentralityReWeight cw(datafname,inputFile_,"");
  if (needReweight ) cw.Init();
  
  TTree * tgj = new TTree("tgj","gamma jet tree");
   GammaJet gj;
   Isolation isol;
   EvtSel evt;
   tgj->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:reweight/F"); // todo : add ncoll later
   tgj->Branch("lpho",&gj.photonEt,"photonEt/F:photonRawEt:photonEta:photonPhi:hovere:sigmaIetaIeta:sumIso:genIso:genMomId/I");
   tgj->Branch("isolation",&isol.cc1,"cc1:cc2:cc3:cc4:cc5:cr1:cr2:cr3:cr4:cr5:ct1PtCut20:ct2PtCut20:ct3PtCut20:ct4PtCut20:ct5PtCut20");
   // todo : Add reaction plane and bin into 20 reaction plane
   const int nMixing = 10;
      
   TTree * tmixJet = new TTree("tmixJet","jets from minbias events");
   int nmjet;
   float ptMjet[2000];
   float etaMjet[2000];
   float phiMjet[2000];
   tmixJet->Branch("njet",&nmjet,"njet/I");
   tmixJet->Branch("pt",ptMjet,"pt[njet]/F");
   tmixJet->Branch("eta",etaMjet,"eta[njet]/F");
   tmixJet->Branch("phi",phiMjet,"phi[njet]/F");
   
   TTree * tmixTrk = new TTree("tmixTrk","Track from minbias events");
   int nMtrk;
   float ptMtrk[20000];
   float etaMtrk[20000];
   float phiMtrk[20000];
   float injetPtMtrk[20000];
   float injetEtaMtrk[20000];
   float injetPhiMtrk[20000];
   float xjetPtMtrk[20000];
   float xjetEtaMtrk[20000];
   float xjetPhiMtrk[20000];


   tmixTrk->Branch("ntrk",&nMtrk,"ntrk/I");
   tmixTrk->Branch("pt",ptMtrk,"pt[ntrk]/F");
   tmixTrk->Branch("eta",etaMtrk,"eta[ntrk]/F");
   tmixTrk->Branch("phi",phiMtrk,"phi[ntrk]/F");
   tmixTrk->Branch("injetPt",injetPtMtrk,"injetPt[ntrk]/F");
   tmixTrk->Branch("injetEta",injetEtaMtrk,"injetEta[ntrk]/F");
   tmixTrk->Branch("injetPhi",injetPhiMtrk,"injetPhi[ntrk]/F");
   tmixTrk->Branch("xjetPt",xjetPtMtrk,"xjetPt[ntrk]/F");
   tmixTrk->Branch("xjetEta",xjetEtaMtrk,"xjetEta[ntrk]/F");
   tmixTrk->Branch("xjetPhi",xjetPhiMtrk,"xjetPhi[ntrk]/F");
   
   //For the MB mixing
     
   TChain         *tjmb;

   Int_t           nimbj;
   Float_t         imbjpt[300];
   Float_t         imbjeta[300];
   Float_t         imbjphi[300];

   Int_t           nimbTrk;
   Float_t         imbTrkpt[1000];
   Float_t         imbTrketa[1000];
   Float_t         imbTrkphi[1000];
   float          imbTrkInjetPt[1000];
   float          imbTrkInjetPhi[1000];
   float          imbTrkInjetEta[1000];

   Int_t           ipcoll;
   Int_t           iphcal;
   Int_t           imbcent;
   Int_t             imbPBin;
   EvtSel          imbEvt;
   
   TBranch        *b_nimbj;
   TBranch        *b_imbjpt;
   TBranch        *b_imbjphi;
   TBranch        *b_imbjeta;
   
   TBranch        *b_nimbTrk;
   TBranch        *b_imbTrkpt;
   TBranch        *b_imbTrketa;
   TBranch        *b_imbTrkphi;
   TBranch        *b_imbTrkInjetPt;
   TBranch        *b_imbTrkInjetEta;
   TBranch        *b_imbTrkInjetPhi;

   TBranch        *b_ipcoll;
   TBranch        *b_iphcal;
   TBranch        *b_imbcent;
   TBranch        *b_imbEvt;
   
   int nMB;
   
   if ( MinbiasFname !="") {
     tjmb = new TChain("yongsunJetakPu3PF");
     tjmb->Add(MinbiasFname);
     tjmb->AddFriend("yongsunSkimTree",MinbiasFname.Data());
     tjmb->AddFriend("yongsunHiEvt",MinbiasFname.Data());
     tjmb->AddFriend("yongsunTrack",MinbiasFname.Data());
     tjmb->AddFriend("tgj",MinbiasFname.Data());
     
     tjmb->SetBranchAddress("yongsunSkimTree.pcollisionEventSelection", &ipcoll, &b_ipcoll);
     tjmb->SetBranchAddress("yongsunSkimTree.pHBHENoiseFilter",         &iphcal, &b_iphcal);
     tjmb->SetBranchAddress("yongsunHiEvt.hiBin",                  &imbcent,    &b_imbcent);
     tjmb->SetBranchAddress("tgj.evt", &imbEvt, &b_imbEvt);

     tjmb->SetBranchAddress("nref", &nimbj ,&b_nimbj);
     tjmb->SetBranchAddress("jtpt", imbjpt, &b_imbjpt);
     tjmb->SetBranchAddress("jteta", imbjeta, &b_imbjeta);
     tjmb->SetBranchAddress("jtphi", imbjphi, &b_imbjphi);

     tjmb->SetBranchAddress("yongsunTrack.nTrk", &nimbTrk ,&b_nimbTrk);
     tjmb->SetBranchAddress("yongsunTrack.trkPt", imbTrkpt, &b_imbTrkpt);
     tjmb->SetBranchAddress("yongsunTrack.trkEta", imbTrketa, &b_imbTrketa);
     tjmb->SetBranchAddress("yongsunTrack.trkPhi", imbTrkphi, &b_imbTrkphi);
     tjmb->SetBranchAddress("yongsunTrack.injetPt", imbTrkInjetPt, &b_imbTrkInjetPt);
     tjmb->SetBranchAddress("yongsunTrack.injetEta", imbTrkInjetEta, &b_imbTrkInjetEta);
     tjmb->SetBranchAddress("yongsunTrack.injetPhi", imbTrkInjetPhi, &b_imbTrkInjetPhi);

     nMB = tjmb->GetEntries();                                                                                           
     cout << "mb Entries = " << nMB;
   }
   
   int thePrimeN ; //= 199109;
   

   // event plane hitogram
   TH1F* hEvtPlnBin = new TH1F("hEvtPlnBin", "", 100, -4.*PI, PI); // 20 for +-PI/2.
   const int theEvtPlNumber = 21;


   /// LOOP!!
   int nentries = c->GetEntries();
   if (maxEvents > 0 ) 
     nentries = maxEvents;
   cout << "number of entries = " << nentries << endl;
   int mbItr(0) ;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if (jentry% 500 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
     
     c->GetEntry(jentry);
     evt.run   = c->evt.run;
     evt.evt = c->evt.evt;
     evt.cBin = c->evt.hiBin;
     float gjEvtPln = c->evt.hiEvtPlanes[theEvtPlNumber];
     evt.pBin   = hEvtPlnBin->FindBin(gjEvtPln);
     evt.nG = c->photon.nPhotons;
     evt.nJ = c->icPu5.nref;
     evt.nT = c->track.nTrk;
     evt.reweight = 1;
     if (needReweight) evt.reweight = cw.GetWeight(c->evt.hiBin);
     evt.trig = (c->hlt.HLT_HISinglePhoton30_v2 > 0);
     evt.offlSel = (c->skim.pcollisionEventSelection > 0);
     evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
     evt.anaEvtSel = c->selectEvent() && evt.trig;
     evt.vz = c->track.vz[1];
      
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
            
      gj.clear();
      for (int j=0;j<c->photon.nPhotons;j++) {
	
	// loose cut!!! 
	if ( c->photon.pt[j]  < preCutPhotonEt ) continue; 
	if ( fabs(c->photon.eta[j]) > cutphotonEta ) continue;
	if (c->isSpike(j)) continue;       
	//	if (!(c->isLoosePhoton(j))) continue;
	// sort using corrected photon pt                                                                                                                                                     
	/// This is changed!!!
	float theCorrPt= c->photon.pt[j] ; //corrPt[j];
	if ( theCorrPt > gj.photonEt) {
	  gj.photonEt = theCorrPt;
	  leadingIndex = j;
	}
      }
      
      if ( ( cutphotonPt > 0 )  && ( gj.photonEt < cutphotonPt ) ) 
	continue;    // otherwise we don't cut any events!!
      
      
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
      
      // add injet tag to the tracks in jet
      // getDr function
      // make the list of high pt jets 
      int nGoodj=0;
      for (int ij=0; ij< c->akPu3PF.nref ;ij++) {
	if ( c->akPu3PF.jtpt[ij] <  cutjetPt)
	  continue;
	if ( fabs(c->akPu3PF.jteta[ij]) > cutjetEta)
	  continue;
	goodJetf[nGoodj].pt = c->akPu3PF.jtpt[ij] ;
	goodJetf[nGoodj].eta = c->akPu3PF.jteta[ij] ;
	goodJetf[nGoodj].phi = c->akPu3PF.jtphi[ij] ;
	nGoodj++;
      }
      
      
      for (int it=0; it < c->track.nTrk; it++ ) { 
	injetPt[it] = 0;
        injetEta[it] = 100000;
        injetPhi[it] = 100000;

	for ( int ij = 0 ; ij < nGoodj ; ij++) {
	  if ( getDR( c->track.trkEta[it],  c->track.trkPhi[it], goodJetf[ij].eta, goodJetf[ij].phi ) < drCutTJ )
	    if ( injetPt[it] < goodJetf[ij].pt ) {
	      injetPt[it]  = goodJetf[ij].pt; // here
              injetEta[it]  = goodJetf[ij].eta; // here                             
	      injetPhi[it]  = goodJetf[ij].phi; // here                                            
	    }
	}
      }
      
      
      // todo make getDR function
      ///////////////////////
      
      int iCounter=0;
      nmjet =0;
      nMtrk =0;
      
      while ( (MinbiasFname !="") && (iCounter<nMixing) ) {
	
	//	if ( thePrimeN > tjmb->GetEntries() ) {
	//	  cout << "error!! the prime Number is larger than the number of entries of MB events" << endl;
	thePrimeN =  tjmb->GetEntries() - 10;
	//	}
	
	mbItr++;
	//cout << " mbItr = " << mbItr << endl;
	if (mbItr == thePrimeN)   mbItr = mbItr - thePrimeN;	
	tjmb->GetEntry(mbItr);
	
	if ( ipcoll == 0 )   
	  continue;
	if ( (!isMC) && ( iphcal == 0  )) 
	  continue;  
	if ( evt.cBin != imbcent ) 
	  continue;
	//	if ( evt.pBin != imbEvt.pBin ) 
	//	  continue;
	
	//	cout << "the centrality = " << evt.cBin << " == " << imbcent << endl;
	// Now, this is clean minbias events with centrality matched and event plane also matched

	for (int ij = 0 ; ij<nimbj ; ij++) {
	  if ( imbjpt[ij] < cutjetPt) 
	    continue;
	  if ( fabs(imbjeta[ij]) > cutjetEta )
	    continue;
	  
	  ptMjet[nmjet] =  imbjpt[ij];
	  etaMjet[nmjet] =  imbjeta[ij];
          phiMjet[nmjet] =  imbjphi[ij];
	  nmjet++;
	  }
	
	
        for (int it = 0 ; it < nimbTrk ; it++) {
	  if ( imbTrkpt[it] < cuttrkPt )
	    continue;
	  if ( fabs(imbTrketa[it]) > cuttrkEta ) 
	    continue;
	  
	  ptMtrk[nMtrk] =    imbTrkpt[it];
          etaMtrk[nMtrk] =   imbTrketa[it];
          phiMtrk[nMtrk] =   imbTrkphi[it];
          injetPtMtrk[nMtrk] = imbTrkInjetPt[it];
	  injetEtaMtrk[nMtrk] = imbTrkInjetEta[it];
          injetPhiMtrk[nMtrk] = imbTrkInjetPhi[it];
	  xjetPtMtrk[nMtrk] = 0;
          xjetEtaMtrk[nMtrk] = 100000;
          xjetPhiMtrk[nMtrk] = 100000;
	  
	  for ( int ij = 0 ; ij < nGoodj ; ij++) {
	    if ( getDR( etaMtrk[nMtrk],  phiMtrk[nMtrk], goodJetf[ij].eta, goodJetf[ij].phi ) < drCutTJ )
	      if ( xjetPtMtrk[nMtrk] < goodJetf[ij].pt ) {
		xjetPtMtrk[nMtrk]   = goodJetf[ij].pt; // here                                              
		xjetEtaMtrk[nMtrk]  = goodJetf[ij].eta; // here                                                             
		xjetPhiMtrk[nMtrk]  = goodJetf[ij].phi; // here                             
	      }
	  }
	  	  
	  nMtrk++;
	}
	
	iCounter++;
      }
      
     
      
      tgj->Fill();
      newtree->Fill();;
      newtreehlt->Fill();
      newtreeSkim->Fill();
      newtreeEvt->Fill();
      newtreeAk3Jet->Fill();
      newtreeTrack->Fill();

      if ( isGen )
	newtreeGen->Fill();
      if (MinbiasFname !="") {
	tmixJet->Fill();
	tmixTrk->Fill();
      }
      
   }
   // After Event Loop                                                                                                                       
   tgj->SetAlias("optIsol","(6.5481e-01 +cc5*8.127033e-03 +cc4*-1.275908e-02 +cc3*-2.24332e-02 +cc2*-6.96778e-02 +cc1*4.682052e-02 +cr5*-2.35164e-02 +cr4*1.74567e-03 +cr3*-2.39334e-04 +cr2*-3.1724e-02 +cr1*-3.65306e-02 +ct4PtCut20*1.8335e-02 +ct3PtCut20*-2.609068e-02 +ct2PtCut20*-4.523171e-02 +ct1PtCut20*-1.270661e-02 +ct5PtCut20*9.218723e-03)");
   
   newfile_data->Write();
   //newfile_data->Close();
   tgj->Draw("offlSel");
   cout << " Done! "<< endl;
}




int getNcoll(int cBin) { 
   if (cBin == 0) return  1747.86 ;
   if (cBin == 1) return  1567.53 ;
   if (cBin == 2) return  1388.39 ;
   if (cBin == 3) return  1231.77 ;
   if (cBin == 4) return  1098.2 ;
   if (cBin == 5) return  980.439 ;
   if (cBin == 6) return  861.609 ;
   if (cBin == 7) return  766.042 ;
   if (cBin == 8) return  676.515 ;
   if (cBin == 9) return  593.473 ;
   if (cBin == 10) return  521.912 ;
   if (cBin == 11) return  456.542 ;
   if (cBin == 12) return  398.546 ;
   if (cBin == 13) return  346.647 ;
   if (cBin == 14) return  299.305 ;
   if (cBin == 15) return  258.344 ;
   if (cBin == 16) return  221.216 ;
   if (cBin == 17) return  188.677 ;
   if (cBin == 18) return  158.986 ;
   if (cBin == 19) return  134.7 ;
   if (cBin == 20) return  112.547 ;
   if (cBin == 21) return  93.4537 ;
   if (cBin == 22) return  77.9314 ;
   if (cBin == 23) return  63.5031 ;
   if (cBin == 24) return  52.0469 ;
   if (cBin == 25) return  42.3542 ;
   if (cBin == 26) return  33.9204 ;
   if (cBin == 27) return  27.3163 ;
   if (cBin == 28) return  21.8028 ;
   if (cBin == 29) return  17.2037 ;
   if (cBin == 30) return  13.5881 ;
   if (cBin == 31) return  10.6538 ;
   if (cBin == 32) return  8.35553 ;
   if (cBin == 33) return  6.40891 ;
   if (cBin == 34) return  5.13343 ;
   if (cBin == 35) return  3.73215 ;
   if (cBin == 36) return  3.06627 ;
   if (cBin == 37) return  2.41926 ;
   if (cBin == 38) return  2.11898 ;
   if (cBin == 39) return  1.76953 ;
   return -100000;
}
