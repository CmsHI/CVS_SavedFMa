#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "../HiForest/HIN-12-013/hiForest.h"
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>
#include "SmearingFactors.h"

using namespace std;

#define PI 3.141592653589
static const int MAXTRK  = 15000;   // This is very enough.
static const int MAXMTRK = 50000;   // Again this is very enough for 10 mixing  
static const int MAXMGenPar = 80000;   // Again this is very enough for 10 mixing                                                               

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
      
      TCut addCut = "jtpt[0]>100 && abs(jteta[0])<1.6";

      TChain * tdata = new TChain("akPu3PFJetAnalyzer/t");
      tdata->Add(datafname);
      tdata->AddFriend("yEvt=hiEvtAnalyzer/HiTree",datafname.Data());
      tdata->AddFriend("ySkim=skimanalysis/HltTree",datafname.Data());
           
      tdata->Draw("yEvt.hiBin>>hCentData","ySkim.pHBHENoiseFilter>0 && ySkim.pcollisionEventSelection>0" && addCut);

      TChain * tmc = new TChain("ak5CaloJetAnalyzer/t");
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


TH1D* getCentDistDj();


void forest2jetSkim(TString inputFile_="mc/photon50_25k.root", std::string outname = "barrelPhoton50_25k.root", bool needReweight=true, int maxEvents=-1, TString MinbiasFname="", bool isMC=false, TString jetAlgo="akPu3PF", 
		    bool isPP = false,   int smearCentBin = 0,bool useGenJet=false){ 
  // smear Cent Bin = 0 : no smearing     1,2,3,4 corresponds to 0-10%, 30%,50%,100%
  TH1D* smearingCents = getCentDistDj();
  TH1D* smearingCents2;
  cout << " smearCentBin  = " << smearCentBin << endl;
  if ( smearCentBin > 0 ) {
    for ( int icent40=0;icent40<40;icent40++ ) {
      cout << " centBin1[smearCentBin] =" << centBin1[smearCentBin];
      cout << " centBin1[smearCentBin-1] =" << centBin1[smearCentBin-1];
      if ( (icent40 >= centBin1[smearCentBin] ) || ( icent40 < centBin1[smearCentBin-1] ) ) 
	smearingCents->SetBinContent(icent40+1,0);      
      cout << " bin = " << smearingCents->GetBinContent(icent40+1) << endl;
    }
    
    smearingCents2 = new TH1D("smearingCents2","",centBin1[smearCentBin]-centBin1[smearCentBin-1],centBin1[smearCentBin-1],centBin1[smearCentBin]);
    for ( int icent40 = centBin1[smearCentBin-1]; icent40<=centBin1[smearCentBin]; icent40++ ) {
      smearingCents2->SetBinContent(icent40- centBin1[smearCentBin-1] +1,  smearingCents->GetBinContent(icent40+1) );
    }
  }
  TCanvas *c3 = new TCanvas("c3","",400,400);
  if ( smearCentBin > 0 ) 
    smearingCents2->Draw();
  c3->Draw("cent2.gif");
  TCanvas *c4 = new TCanvas("c4","",400,400);
  
  //  float jetDPhiCut = PI * 7./8.;
  float leadingJetPtCut = 100;
  float sublingJetPtCut = -1;
  float slJetPtCutMatch = 100;
  bool doIcPu5CaloSkim = false;

  float tempJetEtaCut  = 2;
  
  float drMatching = 0.3;
  
  bool saveOnlyJetConeTrks = true;
  bool saveMixTracks = true; 
  TFile* cent2F = new TFile("percentBins.root");
  TH1D* cent2Hist;
  if ( isMC ) 
    cent2Hist = (TH1D*)cent2F->Get("hresult_mc");
  else
    cent2Hist = (TH1D*)cent2F->Get("hresult_data");
  
  int nCent2 = 40 ; // cent2Hist->GetNbinsX() + 1 ;  // ok back to 40 centrality bins                                               

  cout << " number of mixing centrality bins = " << nCent2 << endl; //cent2Hist->GetNbinsX() << endl;
  bool takeCarePBin = false;
  TString pbinFlag = "yesPbin";
  if ( !takeCarePBin ) pbinFlag = "noPbin";
  int seconds = time(NULL);
  //  cout << " time = " <<seconds%10000<< endl;
  TRandom3 rand(seconds%10000);
   
  TString datafname  = "/net/hidsk0001/d00/scratch/yjlee/merge/pbpb/mergedFile.root";
  
  float preCutPhotonEt = 30;
   
  const int nMaxPho = 1000;
  
  HiForest *c = new HiForest(inputFile_.Data(),jetAlgo.Data(),isPP);
  c->doTrackingSeparateLeadingSubleading = false;
  c->doTrackCorrections = 1;
  c->InitTree();
  
//   c->GetEnergyScaleTable("../forest2/photonEnergyScaleTable_Hydjet_GammaJet.root");
  

  
  
  // now open new root file
  TFile* newfile_data = new TFile(Form("%s_jetPt_%.0f_%.0f_jetEtaCut_%.2f_%s_smearCentBin%d_%s_useGenJet%d.root",outname.data(),leadingJetPtCut,sublingJetPtCut,tempJetEtaCut,pbinFlag.Data(),smearCentBin,jetAlgo.Data(),useGenJet),"recreate");
  
  TH1D* smearingHist ;
  if ( smearCentBin > 0 ) {
    smearingHist = new TH1D("smearingH","",100,-2,2);
  }
  
     
  TTree* newtreehlt;
  if  (MinbiasFname != "") {
    if ( c->hltTree != 0 ) {
      newtreehlt = c->hltTree->CloneTree(0);
      newtreehlt->SetName("yongsunHltTree");
      newtreehlt->SetMaxTreeSize(30000000000);
    }
  }
  
  TTree* newtreeSkim;
  if (MinbiasFname != "") {
    newtreeSkim = c->skimTree->CloneTree(0);
    newtreeSkim->SetName("yongsunSkimTree");
    newtreeSkim->SetMaxTreeSize(30000000000);
  }

  TTree* newtreeEvt;
  if  (MinbiasFname != "") {
    newtreeEvt = c->evtTree->CloneTree(0);
    newtreeEvt->SetName("yongsunHiEvt");
    newtreeEvt->SetMaxTreeSize(30000000000);
  }

  TTree* newtreeAk3pfJet;
  if  (MinbiasFname != "") {
    newtreeAk3pfJet =  c->akPu3jetTree->CloneTree(0);
    newtreeAk3pfJet->SetName("yongsunAk3PfJet");
    newtreeAk3pfJet->SetMaxTreeSize(30000000000);
  }
  
  
  
  //  TTree* newtreeTrack = c->trackTree->CloneTree(0);
  //  newtreeTrack->SetName("yongsunTrack");
  
   
  int isGen(false);

  TTree* newtreeGen;  // Let's not save gen particles at this moment
  if ( c->genpTree !=0 ) {   
    newtreeGen = c->genpTree->CloneTree(0);
    newtreeGen->SetName("yongsunGen");
    newtreeGen->SetMaxTreeSize(30000000000);
    isGen = true;
  }
  

  // Track tree
  int nTrk;
  float trkPt[MAXTRK];
  float trkEta[MAXTRK];
  float trkPhi[MAXTRK];
  float trkWeight[MAXTRK];
  float trkWeightPara[MAXTRK];
  int trkJetMatch[MAXTRK];
  float trkJetDr[MAXTRK];

  TTree *newtreeTrack;                                                                                        
  if ( MinbiasFname != "") { 
    newtreeTrack = new TTree("yongsunTrack","merged tracks");
    newtreeTrack->SetMaxTreeSize(30000000000);                  
    newtreeTrack->Branch("ntrk",&nTrk,"ntrk/I");
    newtreeTrack->Branch("pt",trkPt,"pt[ntrk]/F");
    newtreeTrack->Branch("eta",trkEta,"eta[ntrk]/F");
    newtreeTrack->Branch("phi",trkPhi,"phi[ntrk]/F");
    newtreeTrack->Branch("jetMatch",trkJetMatch,"jetMatch[ntrk]/I");
    newtreeTrack->Branch("trkWeight",trkWeight,"trkWeight[ntrk]/F");
    newtreeTrack->Branch("trkWeightPara",trkWeightPara,"trkWeightPara[ntrk]/F");
    newtreeTrack->Branch("jetDr",trkJetDr,"jetDr[ntrk]/F");
    
  }
  
  const int maxGenPar = 25000;
  int nGenPar;
  int   genParSube[maxGenPar];
  int   genParChg[maxGenPar];
  float genParPt[maxGenPar];
  float genParEta[maxGenPar];
  float genParPhi[maxGenPar];
  int   genParJetMatch[maxGenPar];
  float genParJetDr[maxGenPar];
  
  TTree *newtreeGenPar;
  if ( isMC ) {
    newtreeGenPar = new TTree("genPar","gen chg particle");
    newtreeGenPar->SetMaxTreeSize(30000000000);
    newtreeGenPar->Branch("nGenPar",&nGenPar,"nGenPar/I");
    newtreeGenPar->Branch("sube",genParSube,"sube[nGenPar]/I");
    newtreeGenPar->Branch("chg",genParChg,"chg[nGenPar]/I");
    newtreeGenPar->Branch("pt",genParPt,"pt[nGenPar]/F");
    newtreeGenPar->Branch("eta",genParEta,"eta[nGenPar]/F");
    newtreeGenPar->Branch("phi",genParPhi,"phi[nGenPar]/F");
    newtreeGenPar->Branch("jetMatch",genParJetMatch,"jetMatch[nGenPar]/I");
    newtreeGenPar->Branch("jetDr",genParJetDr,"jetDr[nGenPar]/F");
  }

  // PF
  int nPF;
  int   pfId[MAXTRK];
  float pfPt[MAXTRK];
  float pfEta[MAXTRK];
  float pfPhi[MAXTRK];
  int   pfJetMatch[MAXTRK];
  float pfJetDr[MAXTRK];
  
  TTree *newtreePF;
  newtreePF = new TTree("pf","pf candidates");
  newtreePF->SetMaxTreeSize(30000000000);
  newtreePF->Branch("nPF",&nPF,"nPF/I");
  newtreePF->Branch("pfId",pfId,"pfId[nPF]/I");
  newtreePF->Branch("pt",pfPt,"pt[nPF]/F");
  newtreePF->Branch("eta",pfEta,"eta[nPF]/F");
  newtreePF->Branch("phi",pfPhi,"phi[nPF]/F");
  newtreePF->Branch("jetMatch",pfJetMatch,"jetMatch[nPF]/I");
  newtreePF->Branch("jetDr",pfJetDr,"jetDr[nPF]/F");
  
  // For centrality reweighting
  cout << " change the centrality reweighting table for dijets" << endl;
  CentralityReWeight cw(datafname,inputFile_,"");
  if (needReweight ) cw.Init();
    
  DiJet dj;
  EvtSel evt;
  TTree *tdj;
  if  (MinbiasFname != "") {
    tdj = new TTree("tdj","Dijet tree");
    tdj->SetMaxTreeSize(30000000000);
    tdj->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:reweight/F"); // todo : add ncoll later
    tdj->Branch("dijet",&dj.pthat,"pthat/F:lJetPt:slJetPt:lJetRawPt:slJetRawPt:lJetEta:slJetEta:lJetPhi:slJetPhi:lJetPtGM:slJetPtGM:lJetEtaGM:slJetEtaGM:lJetPhiGM:slJetPhiGM:lChgSum:slChgSum:lPhoSum:slPhoSum:lNtrSum:slNtrSum:lJetRecoEoH:slJetRecoEoH:lJetGenEoH:slJetGenEoH:lJetFlav:slJetFlav");
  }
   
  int   nMtrk=0;
  float ptMtrk[MAXMTRK];
  float etaMtrk[MAXMTRK];
  float phiMtrk[MAXMTRK];
  float weightMtrk[MAXMTRK];
  float weightParaMtrk[MAXMTRK];
  int   jetMatchMtrk[MAXMTRK];   // 0: no match, 1: leading jet, 2 : sub leading jet
  float jetDrMtrk[MAXTRK];

  
  TTree * tmixTrk;
  if  ( (saveMixTracks) && (MinbiasFname !="")) {
    tmixTrk = new TTree("mTrk","Track from second minbias events");
    tmixTrk->SetMaxTreeSize(30000000000);
    tmixTrk->Branch("ntrk",&nMtrk,"ntrk/I");
    tmixTrk->Branch("pt",ptMtrk,"pt[ntrk]/F");
    tmixTrk->Branch("eta",etaMtrk,"eta[ntrk]/F");
    tmixTrk->Branch("phi",phiMtrk,"phi[ntrk]/F");
    tmixTrk->Branch("trkWeight",weightMtrk,"trkWeight[ntrk]/F");
    tmixTrk->Branch("trkWeightPara",weightParaMtrk,"trkWeightPara[ntrk]/F");
    tmixTrk->Branch("jetMatch",jetMatchMtrk,"jetMatch[ntrk]/I");
    tmixTrk->Branch("jetDr",jetDrMtrk,"jetDr[ntrk]/F");
  }

  int   nMgenPar=0;
  int   subeMgenPar[MAXMGenPar];  
  int   chgMgenPar[MAXMGenPar]; 
  float ptMgenPar[MAXMGenPar];
  float etaMgenPar[MAXMGenPar];
  float phiMgenPar[MAXMGenPar];
  int   jetMatchMgenPar[MAXMGenPar];   // 0: no match, 1: leading jet, 2 : sub leading jet           
  float jetDrMgenPar[MAXMGenPar];


  TTree * tmixGenPar;
  if  ( (MinbiasFname !="") && ( isMC)) {
    tmixGenPar = new TTree("mGenPar","Gen Particles in mixed events");
    tmixGenPar->SetMaxTreeSize(30000000000);
    tmixGenPar->Branch("nGenPar",&nMgenPar,"nGenPar/I");
    tmixGenPar->Branch("sube",subeMgenPar,"sube[nGenPar]/I");
    tmixGenPar->Branch("chg",chgMgenPar,"chg[nGenPar]/I");
    tmixGenPar->Branch("pt",ptMgenPar,"pt[nGenPar]/F");
    tmixGenPar->Branch("eta",etaMgenPar,"eta[nGenPar]/F");
    tmixGenPar->Branch("phi",phiMgenPar,"phi[nGenPar]/F");
    tmixGenPar->Branch("jetMatch",jetMatchMgenPar,"jetMatch[nGenPar]/I");
    tmixGenPar->Branch("jetDr",jetDrMgenPar,"jetDr[nGenPar]/F");
  }

  
    
  /// input from MB events for MB on MB mixing!  
  
   TChain         *tjmb[100][nVtxBin];
   Int_t           nimbTrk;
   Float_t         imbTrkpt[10000];
   Float_t         imbTrketa[10000];
   Float_t         imbTrkphi[10000];
   Float_t         imbTrkWeight[10000];
   Float_t         imbTrkWeightPara[10000];
   EvtSel          imbEvt;
   TBranch        *b_nimbTrk;
   TBranch        *b_imbTrkpt;
   TBranch        *b_imbTrketa;
   TBranch        *b_imbTrkphi;
   TBranch        *b_imbTrkWeight;
   TBranch        *b_imbTrkWeightPara;
   TBranch        *b_imbEvt ;
   
   Int_t           nimbGenPar;
   Int_t           imbGenParSube[10080];   //[nGenPar]                                                                                                      
   Float_t         imbGenParPt[10080];   //[nGenPar]                                                                                                         
   Float_t         imbGenParEta[10080];   //[nGenPar]                                                                                                 
   Float_t         imbGenParPhi[10080];   //[nGenPar]                                                                                              
   Int_t           imbGenParChg[10080];   //[nGenPar]                                                                                    
   TBranch         *b_nimbGenPar;
   TBranch         *b_imbGenParSube;
   TBranch         *b_imbGenParPt;
   TBranch         *b_imbGenParEta;
   TBranch         *b_imbGenParPhi;
   TBranch         *b_imbGenParChg;
   
   
 
   int nMB[100][nVtxBin] ; //= 199109;                                                                                          
   int mbItr[100][nVtxBin];
   if  ( (saveMixTracks) && (MinbiasFname !="") ) {
     for( int icent = 0 ; icent<nCent2 ; icent++) {
       for( int ivz = 0 ; ivz<nVtxBin ; ivz++) {
	 tjmb[icent][ivz] = new TChain(Form("trackAndJet_first_cBin2icent%d_ivz%d",icent,ivz));
	 tjmb[icent][ivz]->Add(MinbiasFname);
	 tjmb[icent][ivz]->SetBranchAddress("evt", &imbEvt, &b_imbEvt);
	 tjmb[icent][ivz]->SetBranchAddress("nTrk", &nimbTrk ,&b_nimbTrk);
	 tjmb[icent][ivz]->SetBranchAddress("trkPt", imbTrkpt, &b_imbTrkpt);
	 tjmb[icent][ivz]->SetBranchAddress("trkEta", imbTrketa, &b_imbTrketa);
	 tjmb[icent][ivz]->SetBranchAddress("trkPhi", imbTrkphi, &b_imbTrkphi);
	 tjmb[icent][ivz]->SetBranchAddress("trkWeight", imbTrkWeight, &b_imbTrkWeight);
	 tjmb[icent][ivz]->SetBranchAddress("trkWeightPara", imbTrkWeightPara, &b_imbTrkWeightPara);
	 if ( isMC ) {
	   tjmb[icent][ivz]->SetBranchAddress("nGenPar", &nimbGenPar, &b_nimbGenPar);
	   tjmb[icent][ivz]->SetBranchAddress("genParSube", imbGenParSube, &b_imbGenParSube);
	   tjmb[icent][ivz]->SetBranchAddress("genParPt", imbGenParPt, &b_imbGenParPt);
	   tjmb[icent][ivz]->SetBranchAddress("genParEta", imbGenParEta, &b_imbGenParEta); 
	   tjmb[icent][ivz]->SetBranchAddress("genParPhi", imbGenParPhi, &b_imbGenParPhi);
	   tjmb[icent][ivz]->SetBranchAddress("genParChg", imbGenParChg, &b_imbGenParChg);
	 }
	 nMB[icent][ivz] = tjmb[icent][ivz]->GetEntries();
	 cout << "number of evetns in icent = " << icent << ", ivz "<<ivz<<" = " << nMB[icent][ivz] << endl;
	 if ( nMB[icent][ivz] == 0 )  {
	   cout << "ERROR!!!! nMB = 0 bin" << endl << endl << endl << endl;
	   mbItr[icent][ivz] = -1;
	 }
	 else {
	   int primeSeed = rand.Integer(197314);
	   mbItr[icent][ivz] = primeSeed%(nMB[icent][ivz]);
	   cout <<" initial itr = " << mbItr[icent][ivz] << endl;
	 }
	 }
     }
   }
   
   // verte binning 
   TH1F* hvz = new TH1F("hvz","",nVtxBin,-vtxCut,vtxCut);
   // event plane hitogram
   TH1F* hEvtPlnBin = new TH1F("hEvtPlnBin", "", nPlnBin*5, -4.*PI, PI);
   const int theEvtPlNumber = 21;
   
   ////////////////////////
   // Smearing Setup
   ////////////////////////
   if (smearCentBin>0) {
     LoadParameters();
   }

   /// LOOP!!
   
   int nentries = c->GetEntries();
   if (maxEvents > 0 ) 
     nentries = maxEvents;
   cout << "number of entries = " << nentries << endl;
   int startEntry = 0;
   if ( startEntry > nentries ) {
     cout << "error! startEntry > nentries!!!" << endl;
     return;
   }
   
   for (Long64_t jentry=startEntry ; jentry<nentries;jentry++) {
     
     if (jentry% 1000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
     
     c->GetEntry(jentry);
     
     //here
     evt.run  = c->evt.run;
     evt.evt  = c->evt.evt;
     evt.cBin = c->evt.hiBin;
     evt.pBin = hEvtPlnBin->FindBin( c->evt.hiEvtPlanes[theEvtPlNumber] ) ;
     evt.nG   = c->photon.nPhotons;
     evt.nJ   = c->icPu5.nref;
     evt.nT   = c->track.nTrk;
     evt.reweight = 1;
     if (needReweight) evt.reweight = cw.GetWeight(c->evt.hiBin);
     evt.trig = (c->hlt.HLT_HISinglePhoton30_v2 > 0);
     evt.offlSel = (c->skim.pcollisionEventSelection > 0);
     evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
     evt.anaEvtSel = c->selectEvent() && evt.trig;
     evt.vz = c->evt.vz;

     int vzBin = hvz->FindBin(evt.vz) -1 ;
     if ( (vzBin<0) || ( vzBin >= nVtxBin) )
       continue;

     // New centrality bin 
     int cBin2 = evt.cBin;     //   cent2Hist->FindBin(c->evt.hiHF) -1 ; // ok.. go back to 40 centrality bins                           
     // calculate corrected pt
     
     
     
     if (  c->skim.pcollisionEventSelection != 1 ) // Now, let's save the collision events only
       continue;
     if ( (!isMC) && (c->skim.pHBHENoiseFilter != 1) ) 
       continue; 
     if (doIcPu5CaloSkim && !(c->icPu5.jtpt[0]>90 && fabs(c->icPu5.jteta[0])<3) )
       continue;

     dj.clear();
     if (isMC)    dj.pthat = c->photon.ptHat;
     
     Jets* theJet = &(c->akPu3PF) ;
//      if ( jetAlgo == "akPu3PF" ) theJet = &(c->akPu3PF) ;
//      else if ( jetAlgo == "icPu5" ) theJet = &(c->icPu5) ;
//      else if ( jetAlgo == "akPu3Calo" ) theJet = &(c->akPu3Calo) ;
//      else {
//        cout << "jet algo =" << jetAlgo << " : no such jet algo!!" << endl ;
//        continue;
//      }
     
     int lJetIndex(-99);
     int nJets                          = theJet->nref; 
     if ( useGenJet && isMC )     nJets = theJet->ngen;

     ////////////////////////
     // Smearing Setup
     ////////////////////////
     if (smearCentBin>0) {	
       for (int ij=0; ij< nJets ; ij++) {
         if (theJet->jtpt[ij]>200) cout << "Before smearing: " << theJet->jtpt[ij] << endl;
         int tempCentBin=-100;
	 while (  (tempCentBin >= centBin1[smearCentBin] ) || ( tempCentBin < centBin1[smearCentBin-1] ) ) {
	   tempCentBin = smearingCents2->GetRandom() ;
	   if ( (tempCentBin >= centBin1[smearCentBin] ) || ( tempCentBin < centBin1[smearCentBin-1] ) ) {
	     cout << "smearCentBin = " << smearCentBin << "   and tempCentBin= " << tempCentBin << endl;
	     cout << " NO!!! the centrality bin from dice throw is out of the range.  Trying once again" << endl;
	   }
	 }
	 int smCentBin = GetCBin( tempCentBin );
	 float tttejej = theJet->jtpt[ij] ;
	 theJet->jtpt[ij]  = GetSmearedPtData(2,smCentBin,theJet->jtpt[ij],0,"");
         if (theJet->jtpt[ij]>200) cout << "After smearing: " << theJet->jtpt[ij] << endl;
	 
	 if ( (tttejej > 100) && (fabs(theJet->jteta[ij])<2.0))
	   smearingHist->Fill( (theJet->jtpt[ij] - tttejej )/tttejej );
       }
     }
     for (int ij=0; ij< nJets ; ij++) {
       float theJtpt ;
       float theJteta;
       float theJtphi;
       if( useGenJet && isMC ) {
         theJtpt = theJet->genpt[ij];
         theJteta = theJet->geneta[ij];
         theJtphi = theJet->genphi[ij];
       }
       else {   
         theJtpt  = theJet->jtpt[ij];
         theJteta = theJet->jteta[ij];
         theJtphi = theJet->jtphi[ij];
       }
              
       if ( fabs( theJteta ) > tempJetEtaCut ) 
	 continue;
       
       if ( (!useGenJet) && ((theJet->trackMax[ij]/theJet->jtpt[ij])<0.01) )
	 continue;
	    
       
       //   if ( useGenJet && isMC )          theJtpt = theJet->matchedPt[ij];
       if ( theJtpt > dj.lJetPt ) { 
	 lJetIndex = ij;
	 dj.lJetPt = theJtpt;
	 dj.lJetRawPt = theJet->rawpt[ij];
	 dj.lJetEta = theJteta;
	 dj.lJetPhi = theJtphi;
	 if ( useGenJet) 
	   dj.lJetPtGM = dj.lJetPt; 
	 else
	   dj.lJetPtGM = theJet->refpt[ij];
	 
         dj.lJetFlav  = theJet->refparton_flavor[ij];
	 //dj.lJetChgSum = theJet->chargedSum[ij];
	 // dj.lJetPhoSum = theJet->photonSum[ij];
	 //dj.lJetNtrSum = theJet->neutralSum[ij];
	 //dj.lJetRecoEoH = dj.lJetPhoSum / (dj.lJetChgSum + dj.lJetNtrSum) ;
		 
       }
     }

     for (int ij=0; ij< nJets ; ij++) {
       float theJtpt ;
       float theJteta;
       float theJtphi;
       if( useGenJet && isMC ) {
         theJtpt = theJet->genpt[ij];
         theJteta = theJet->geneta[ij];
         theJtphi = theJet->genphi[ij];
       }
       else {
	 theJtpt  = theJet->jtpt[ij];
	 theJteta = theJet->jteta[ij];
	 theJtphi = theJet->jtphi[ij];
       }
       if ( ij == lJetIndex ) 
	 continue;
       if ( fabs( theJteta ) > tempJetEtaCut )
	 continue;

       if ( theJtpt > dj.slJetPt ) {
         dj.slJetPt = theJtpt;
	 dj.slJetRawPt = theJet->rawpt[ij];
	 dj.slJetEta = theJteta;
         dj.slJetPhi = theJtphi;
	 dj.slJetPtGM = theJet->refpt[ij];

	 dj.slJetFlav  = theJet->refparton_flavor[ij];
	 //dj.slJetChgSum = theJet->chargedSum[ij];
	 //dj.slJetPhoSum = theJet->photonSum[ij];
	 //dj.slJetNtrSum = theJet->neutralSum[ij];
         //dj.slJetRecoEoH= dj.slJetPhoSum/ (dj.slJetChgSum + dj.slJetNtrSum) ;
       }
     }
     
     if ( dj.lJetPt < leadingJetPtCut ) 
       continue;
     if ( sublingJetPtCut>0 && dj.slJetPt < sublingJetPtCut ) 
       continue;
     //     if (  getDPHI( dj.lJetPhi,  dj.slJetPhi ) < jetDPhiCut  )
     //   continue;
   
     //// Jet is done.  now let's go to tracks 
     nTrk = 0; 

     for (int it=0; it < c->track.nTrk; it++ ) { 
       if ( c->track.trkPt[it] < cuttrkPt ) continue;
       if ( fabs( c->track.trkEta[it]) > cuttrkEta ) continue;
       if ( !c->track.highPurity[it])  continue;   // only high purity high pt tracks and all pixel track
       
       float drFromLjet  = getDR( c->track.trkEta[it], c->track.trkPhi[it], dj.lJetEta,  dj.lJetPhi);
       
       float drFromSljet = 100 ; 
       if ( dj.slJetPt > 20 ) 
	 drFromSljet = getDR( c->track.trkEta[it], c->track.trkPhi[it], dj.slJetEta, dj.slJetPhi);
       // float matchedJetPt=0;
       trkJetMatch[nTrk] = 0;
       if  (drFromLjet<=drMatching) {	 // matchedJetPt = dj.lJetPt;
	 trkJetDr[nTrk] = drFromLjet;
	 trkJetMatch[nTrk] = 1;
       }
       else if ( (dj.slJetPt > slJetPtCutMatch) && (drFromSljet<=drMatching)) {	 // matchedJetPt =dj.slJetPt;
	 trkJetDr[nTrk] = drFromSljet;
	 trkJetMatch[nTrk] = 2;
       }
       else if ( getDR(c->track.trkEta[it],  c->track.trkPhi[it], -dj.lJetEta,  dj.lJetPhi) <= drMatching  ) {
	 trkJetDr[nTrk] = getDR(c->track.trkEta[it], c->track.trkPhi[it], -dj.lJetEta, dj.lJetPhi) ; 
	 trkJetMatch[nTrk] = -1;
       }
       else if (  (dj.slJetPt > slJetPtCutMatch) && (getDR(c->track.trkEta[it], c->track.trkPhi[it], -dj.slJetEta, dj.slJetPhi) <= drMatching)) {
	 trkJetDr[nTrk] = getDR(c->track.trkEta[it], c->track.trkPhi[it], -dj.slJetEta, dj.slJetPhi) ;
	 trkJetMatch[nTrk] = -2;
       }
       else {
	 trkJetMatch[nTrk] = 0;  	 // matchedJetPt = 0 ;
	 trkJetDr[nTrk] = 999;
	 if ( saveOnlyJetConeTrks)  continue;  // just tantatively... July 8th 2012
       }
       trkPt[nTrk]  = c->track.trkPt[it];
       trkEta[nTrk] = c->track.trkEta[it];
       trkPhi[nTrk] = c->track.trkPhi[it]; 
       trkWeight[nTrk] = c->getTrackCorrection(it);
       trkWeightPara[nTrk] = c->getTrackCorrectionPara(it);
       
       nTrk++;
     }

     ///////////////////////////////////////////
     /// track is done now it's pf candidates
     ///////////////////////////////////////////
     nPF = 0;
     for ( int ic = 0 ; ic < c->pf.nPFpart ; ic++) {
       if ( c->pf.pfPt[ic] < cuttrkPt ) continue;
       if ( fabs( c->pf.pfEta[ic] ) > cuttrkEta ) continue;
       
       float drFromLjet  = getDR( c->pf.pfEta[ic], c->pf.pfPhi[ic], dj.lJetEta,  dj.lJetPhi);
       float drFromSljet = 100;
       if ( dj.slJetPt > 20 ) drFromSljet = getDR( c->pf.pfEta[ic], c->pf.pfPhi[ic], dj.slJetEta, dj.slJetPhi);
       
       pfJetMatch[nPF] =0;
       if  (drFromLjet<=drMatching) {	    //  matchedJetPt = dj.lJetPt;
         pfJetMatch[nPF] = 1;
         pfJetDr[nPF] = drFromLjet;
       }
       else if ( (dj.slJetPt > slJetPtCutMatch) && (drFromSljet<=drMatching)) {  //            matchedJetPt = dj.slJetPt;
         pfJetMatch[nPF] = 2;
         pfJetDr[nPF] = drFromSljet;
       }
       else if ( getDR(c->pf.pfEta[ic], c->pf.pfPhi[ic], -dj.lJetEta,  dj.lJetPhi) <=drMatching ) {
         pfJetMatch[nPF] = -1; 
         pfJetDr[nPF] =  getDR(c->pf.pfEta[ic], c->pf.pfPhi[ic], -dj.lJetEta,  dj.lJetPhi);
       }
       else if (  (dj.slJetPt > slJetPtCutMatch) && (getDR(c->pf.pfEta[ic], c->pf.pfPhi[ic], -dj.slJetEta, dj.slJetPhi) <=drMatching) ) {
         pfJetMatch[nPF] = -2;
         pfJetDr[nPF] =  getDR(c->pf.pfEta[ic], c->pf.pfPhi[ic], -dj.slJetEta, dj.slJetPhi);
       }
       else {        //        matchedJetPt = 0;
         pfJetMatch[nPF] = 0;
         pfJetDr[nPF] = 999;
         if (saveOnlyJetConeTrks) continue;  // just tantatively... July 8th 2012  
       }
       
       pfPt[nPF]  = c->pf.pfPt[ic];
       pfEta[nPF] = c->pf.pfEta[ic];
       pfPhi[nPF] = c->pf.pfPhi[ic];	
       pfId[nPF] = c->pf.pfId[ic];       
       nPF++;
     }
     
     /// pf candidate is done now it's gen particie
     nGenPar = 0;
     if ( isMC ) {
       float lGenESum(0), slGenESum(0), lGenHSum(0), slGenHSum(0);

       for ( int ig = 0 ; ig < c->genparticle.mult ; ig++) {
	 	 
	 if ( c->genparticle.pt[ig] < cuttrkPt ) 
	   continue;
	 if ( fabs( c->genparticle.eta[ig] ) > cuttrkEta ) 
	   continue;
	 
	 
	  float drFromLjet  = getDR( c->genparticle.eta[ig], c->genparticle.phi[ig], dj.lJetEta,  dj.lJetPhi);
	  float drFromSljet = 100 ;
	  if ( dj.slJetPt > 20 )
	    drFromSljet = getDR( c->genparticle.eta[ig], c->genparticle.phi[ig], dj.slJetEta, dj.slJetPhi);
	  
	  //	  float matchedJetPt=0;
	  genParJetMatch[nGenPar] =0;
	  if  (drFromLjet<=drMatching) {	    //  matchedJetPt = dj.lJetPt;
	    genParJetMatch[nGenPar] = 1;
	    genParJetDr[nGenPar] = drFromLjet;
	  }
	  else if ( (dj.slJetPt > slJetPtCutMatch) && (drFromSljet<=drMatching)) {  //            matchedJetPt = dj.slJetPt;
            genParJetMatch[nGenPar] = 2;
            genParJetDr[nGenPar] = drFromSljet;
	  }
	  else if ( getDR(c->genparticle.eta[ig], c->genparticle.phi[ig], -dj.lJetEta,  dj.lJetPhi) <=drMatching ) {
	    genParJetMatch[nGenPar] = -1; 
	    genParJetDr[nGenPar] =  getDR(c->genparticle.eta[ig], c->genparticle.phi[ig], -dj.lJetEta,  dj.lJetPhi);
	  }
	  else if (  (dj.slJetPt > slJetPtCutMatch) && (getDR(c->genparticle.eta[ig], c->genparticle.phi[ig], -dj.slJetEta, dj.slJetPhi) <=drMatching) ) {
	    genParJetMatch[nGenPar] = -2;
	    genParJetDr[nGenPar] =  getDR(c->genparticle.eta[ig], c->genparticle.phi[ig], -dj.slJetEta, dj.slJetPhi);
	  }
	  else {        //        matchedJetPt = 0;
            genParJetMatch[nGenPar] = 0;
            genParJetDr[nGenPar] = 999;
	    if (saveOnlyJetConeTrks) continue;  // just tantatively... July 8th 2012  
	  }
	  
	  genParPt[nGenPar]  = c->genparticle.pt[ig];
	  genParEta[nGenPar] = c->genparticle.eta[ig];
	  genParPhi[nGenPar] = c->genparticle.phi[ig];	
	  genParSube[nGenPar] = c->genparticle.sube[ig];
	  genParChg[nGenPar] = c->genparticle.chg[ig];
	  
	  nGenPar++;
       }
       
       dj.lJetGenEoH =  slGenESum / lGenHSum ;
       dj.slJetGenEoH = slGenESum / slGenHSum;
     }
     

     // track mixing 

     int iCounter=0;
     int nMixing = nMixing1;
     nMtrk =0;
     nMgenPar =0;
     int zeroBinFinder=0;
     bool noSuchEvent = false;
     while ( saveMixTracks && (MinbiasFname !="") && (iCounter<nMixing) ) {
       if ( mbItr[cBin2][vzBin] == -1 ) {
	 iCounter = 99999;
	 noSuchEvent = true; 
	 continue;
       }
       
       mbItr[cBin2][vzBin] = mbItr[cBin2][vzBin] + 1 ;
       if ( mbItr[cBin2][vzBin] == nMB[cBin2][vzBin] )
 	 mbItr[cBin2][vzBin] =  mbItr[cBin2][vzBin] - nMB[cBin2][vzBin];
       
       tjmb[cBin2][vzBin]->GetEntry(mbItr[cBin2][vzBin]);
       
       zeroBinFinder++;
       if (zeroBinFinder > 1000 ) {
	 cout <<" zeroBinFinder > 1000 !!!  There is no such a event with cent, vzbin, evt pln : " << cBin2 <<", "<<vzBin<<", "<<evt.pBin<<endl;
	 noSuchEvent = true;	 //	 cout <<" we move to the next vertex bin" << endl;
	 iCounter = 99999;
	 //	 if ( vzBin >= nVtxBin/2. )	   vzBin = vzBin - 1 ;
	 //	 else                   	   vzBin = vzBin + 1 ;
	 //	 zeroBinFinder = 0;
       }
       
       if ( (takeCarePBin) && ( evt.pBin != imbEvt.pBin )  )   // Only applied for data.
	 continue;
       ////////////////// passed the cut!!       
       iCounter++;

       for (int it = 0 ; it < nimbTrk ; it++) {
	 if ( imbTrkpt[it] < cuttrkPt )
	   continue;
	 if ( fabs(imbTrketa[it]) > cuttrkEta )
	   continue;
	 
	 ptMtrk[nMtrk]  =   imbTrkpt[it];
	 etaMtrk[nMtrk] =   imbTrketa[it];
	 phiMtrk[nMtrk] =   imbTrkphi[it];
	 weightMtrk[nMtrk] = imbTrkWeight[it];
	 weightParaMtrk[nMtrk] = imbTrkWeightPara[it];
	 
	 //	 cout << " phiMtrk[nMtrk] = " << phiMtrk[nMtrk] << endl;
	 float drFromLjet  = getDR( etaMtrk[nMtrk], phiMtrk[nMtrk], dj.lJetEta,  dj.lJetPhi);
	 float drFromSljet = 100 ;
	 if ( dj.slJetPt > 20 )
	   drFromSljet = getDR( etaMtrk[nMtrk], phiMtrk[nMtrk], dj.slJetEta, dj.slJetPhi);        
	 //	 float matchedJetPt=0;
	 
	 jetMatchMtrk[nMtrk] = 0;
	 if  (drFromLjet<=drMatching) {             //	   matchedJetPt = dj.lJetPt;
	   jetDrMtrk[nMtrk] = drFromLjet;
	   jetMatchMtrk[nMtrk] = 1;
	 }
	 else if (  (dj.slJetPt > slJetPtCutMatch) && (drFromSljet<=drMatching)) {        //	   matchedJetPt =dj.slJetPt;
	   jetDrMtrk[nMtrk] = drFromSljet;
           jetMatchMtrk[nMtrk] = 2;
	 }
	 else if (  getDR( etaMtrk[nMtrk], phiMtrk[nMtrk], -dj.lJetEta,  dj.lJetPhi) <=drMatching ){       
	   jetDrMtrk[nMtrk] =  getDR( etaMtrk[nMtrk], phiMtrk[nMtrk], -dj.lJetEta,  dj.lJetPhi);
           jetMatchMtrk[nMtrk] = -1;
         }
	 else if (  (dj.slJetPt > slJetPtCutMatch) && (getDR( etaMtrk[nMtrk], phiMtrk[nMtrk], -dj.slJetEta, dj.slJetPhi) <= drMatching) ) {
	   jetDrMtrk[nMtrk] =  getDR( etaMtrk[nMtrk], phiMtrk[nMtrk], -dj.slJetEta, dj.slJetPhi);
           jetMatchMtrk[nMtrk] = -2;
         }
	 else {                              //    matchedJetPt = 0 ;
	   jetDrMtrk[nMtrk] = 999;
           jetMatchMtrk[nMtrk] = 0;
	   if ( saveOnlyJetConeTrks)  continue;  // just tantatively... July 8th 2012
	 }
	 
	 
	 nMtrk++;
       }
       //////////////////////////................................................. 
       //////////////////////////.................................................       
       //////////////////////////.................................................
       if ( isMC ) {
	 for (int ig = 0 ; ig < nimbGenPar ; ig++) {
	   if ( imbGenParPt[ig] < cuttrkPt )
	     continue;
	   if ( fabs(imbGenParEta[ig]) > cuttrkEta )
	     continue;
	   
	   ptMgenPar[nMgenPar]  = imbGenParPt[ig];
	   etaMgenPar[nMgenPar] = imbGenParEta[ig];
	   phiMgenPar[nMgenPar] = imbGenParPhi[ig];
	   subeMgenPar[nMgenPar]= imbGenParSube[ig];
	   chgMgenPar[nMgenPar] = imbGenParChg[ig];
	   
	   //      cout << " phiMtrk[nMtrk] = " << phiMtrk[nMtrk] << endl;                                                     
	   
	   float drFromLjet  = getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar] , dj.lJetEta,  dj.lJetPhi);
	   float drFromSljet = 100 ;
	   if ( dj.slJetPt > 20 )
	     drFromSljet = getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar] , dj.slJetEta, dj.slJetPhi);
	   
	   jetMatchMgenPar[nMgenPar] = 0;
	   if      (drFromLjet<=drMatching) {
	     jetDrMgenPar[nMgenPar] = drFromLjet;
	     jetMatchMgenPar[nMgenPar] = 1;
	   }
	   else if (  (dj.slJetPt > slJetPtCutMatch) && (drFromSljet<=drMatching) ){
	     jetDrMgenPar[nMgenPar] = drFromSljet;
	     jetMatchMgenPar[nMgenPar] = 2;
	   }
	   ////////////////////////////
	   else if ( getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar] , -dj.lJetEta,  dj.lJetPhi) <= drMatching) {
	     jetDrMgenPar[nMgenPar] = getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar] , -dj.lJetEta,  dj.lJetPhi);
	     jetMatchMgenPar[nMgenPar] = -1;
	   }
	   else if (  (dj.slJetPt > slJetPtCutMatch) && (getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar] , -dj.slJetEta, dj.slJetPhi) < drMatching)) {
	     jetDrMgenPar[nMgenPar] =  getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar] , -dj.slJetEta, dj.slJetPhi);
	     jetMatchMgenPar[nMgenPar] = -22;
	   }
	   ///////////////////
	   else {
	     jetDrMgenPar[nMgenPar] = 999;
	     jetMatchMgenPar[nMgenPar] = 0;
	     if ( saveOnlyJetConeTrks)  continue;
	   }
	   nMgenPar++;
	 }
	 /////////////////////////////////////////////////
       }
     }
     
     if (noSuchEvent == true ) continue;
     
     if  (MinbiasFname != "")  {
       tdj->Fill();
       newtreeSkim->Fill();
       newtreeEvt->Fill();
       //      newtreeAk3pfJet->Fill();
       newtreeTrack->Fill();
       newtreePF->Fill();
       if (saveMixTracks) tmixTrk->Fill();
       if ( isMC ) {
	 newtreeGenPar->Fill();
	 tmixGenPar->Fill();
       }
     }
     
     //      if ( isGen )
     //	newtreeGen->Fill();
   }
   if (smearCentBin>0) {
     smearingHist->Draw();
     smearingHist->Fit("gaus");
   }

   newfile_data->Write();
   cout << " Done! "<< endl;
   

   
}




TH1D* getCentDistDj() {
  TH1D *hCentData = new TH1D("hCentData","",40,-.5,39.5);
  hCentData->SetBinContent(1,13132);
  hCentData->SetBinContent(2,11934);
  hCentData->SetBinContent(3,10585);
  hCentData->SetBinContent(4,9658);
  hCentData->SetBinContent(5,9014);
  hCentData->SetBinContent(6,8068);
  hCentData->SetBinContent(7,7248);
  hCentData->SetBinContent(8,6495);
  hCentData->SetBinContent(9,5918);
  hCentData->SetBinContent(10,5403);
  hCentData->SetBinContent(11,4710);
  hCentData->SetBinContent(12,4399);
  hCentData->SetBinContent(13,3879);
  hCentData->SetBinContent(14,3417);
  hCentData->SetBinContent(15,2931);
  hCentData->SetBinContent(16,2623);
  hCentData->SetBinContent(17,2315);
  hCentData->SetBinContent(18,2043);
  hCentData->SetBinContent(19,1715);
  hCentData->SetBinContent(20,1316);
  hCentData->SetBinContent(21,1247);
  hCentData->SetBinContent(22,1007);
  hCentData->SetBinContent(23,867);
  hCentData->SetBinContent(24,754);
  hCentData->SetBinContent(25,571);
  hCentData->SetBinContent(26,520);
  hCentData->SetBinContent(27,366);
  hCentData->SetBinContent(28,316);
  hCentData->SetBinContent(29,290);
  hCentData->SetBinContent(30,200);
  hCentData->SetBinContent(31,169);
  hCentData->SetBinContent(32,118);
  hCentData->SetBinContent(33,103);
  hCentData->SetBinContent(34,80);
  hCentData->SetBinContent(35,39);
  hCentData->SetBinContent(36,41);
  hCentData->SetBinContent(37,26);
  hCentData->SetBinContent(38,10);
  hCentData->SetBinContent(39,7);
  hCentData->SetBinContent(40,3);
  
  return hCentData;
}
