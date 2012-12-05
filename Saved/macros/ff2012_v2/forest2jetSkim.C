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


void forest2jetSkim(TString inputFile_="/net/hidsk0001/d00/scratch/yjlee/merge/pbpbDijet_v20/promptskim-hihighpt-hltjet80-pt90-v20.root", std::string outname = "jskim.root", bool needReweight=false, int maxEvents=-1, TString MinbiasFname="/mnt/hadoop/cms/store/user/jazzitup/hiForest/skimmed/yskim_HiForestMinBias_v6_first_July-v8-8VtxBin-24PlnBin-40CentBin.root", bool isMC=false, TString jetAlgo="akPu3PF", 
    bool isPP = false,   int smearCentBin = 0,bool useGenJet=false)
{
  // Print setup
  cout << "needReweight: " << needReweight << " isMC: " << isMC << " maxEvents: " << maxEvents << " isPP: " << isPP << " smearCentBin: " << smearCentBin << " useGenJet: " << useGenJet << endl;
    
  //////////////////////////////////////////////////////////////////////////
  // Setup Centrality for smearing, centrality reweighting
  //////////////////////////////////////////////////////////////////////////
  // smear Cent Bin = 0 : no smearing     1,2,3,4 corresponds to 0-10%, 30%,50%,100%
  TH1D* smearingCents = getCentDistDj();
  TH1D* smearingCents2=0;
  if ( smearCentBin > 0 ) {
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
    smearingCents2->Draw();
    c3->Print(Form("centSm_%d.gif",smearCentBin));
    // Finally setup smearing functions
    LoadParameters();
  }

  // For Centrality reweighting of mc
  int nCent2 = 40 ; // cent2Hist->GetNbinsX() + 1 ;  // ok back to 40 centrality bins                                               
  cout << " number of mixing centrality bins = " << nCent2 << endl; //cent2Hist->GetNbinsX() << endl;
  bool takeCarePBin = false;
  TString pbinFlag = "yesPbin";
  if ( !takeCarePBin ) pbinFlag = "noPbin";
  int seconds = time(NULL);
  //  cout << " time = " <<seconds%10000<< endl;
  TRandom3 rand(seconds%10000);
  TString datafname  = "/net/hidsk0001/d00/scratch/yjlee/merge/pbpb/mergedFile.root";
  // For centrality reweighting
  cout << " change the centrality reweighting table for dijets" << endl;
  CentralityReWeight cw(datafname,inputFile_,"");
  if (needReweight ) cw.Init();

  //////////////////////////////////////////////////////////////////////////
  // Analysis Options
  //////////////////////////////////////////////////////////////////////////
  float InclusiveJetPtCut = 50;
  float smearingPtMin=40;
  bool doIcPu5CaloSkim = false;
  float tempJetEtaCut  = 2;
  float drMatching = 0.3;

  bool saveOnlyJetConeTrks = true;
  bool saveMixTracks = true; 

  //////////////////////////////////////////////////////////////////////////
  // Run Forest Run!
  //////////////////////////////////////////////////////////////////////////
  HiForest *c = new HiForest(inputFile_.Data(),jetAlgo.Data(),isPP);
  c->doTrackingSeparateLeadingSubleading = false;
  c->doTrackCorrections = 1;
  c->InitTree();
  // top level overwrites to save speed
  if (c->hasPFTree) c->hasPFTree            = false;
  if (c->hasMetTree) c->hasMetTree           = false;
  if (c->hasAkPu2JetTree) c->hasAkPu2JetTree      = false;
  if (c->hasAkPu4JetTree) c->hasAkPu4JetTree      = false;
  if (c->hasAkPu2CaloJetTree) c->hasAkPu2CaloJetTree  = false;
  if (c->hasAkPu3CaloJetTree) c->hasAkPu3CaloJetTree  = false;
  if (c->hasAkPu4CaloJetTree) c->hasAkPu4CaloJetTree  = false;
  if (c->hasPixTrackTree) c->hasPixTrackTree      = false;
  if (c->hasTowerTree) c->hasTowerTree         = false;
  if (c->hasHbheTree) c->hasHbheTree          = false;
  if (c->hasEbTree) c->hasEbTree            = false;
  
  //////////////////////////////////////////////////////////////////////////
  // Analysis Output
  //////////////////////////////////////////////////////////////////////////
  TFile* newfile_data = new TFile(Form("%s_jetPt_%.0f_jetEtaCut_%.2f_%s_sm%d_%s_gj%d.root",outname.data(),InclusiveJetPtCut,tempJetEtaCut,pbinFlag.Data(),smearCentBin,jetAlgo.Data(),useGenJet),"recreate");
  // XCheck Histograms
  TH1D * hEvtCentNoSkim = new TH1D("hEvtCentNoSkim",";Centrality Bin;",40,0,40);
  TH1D * hEvtCent = new TH1D("hEvtCent",";Centrality Bin;",40,0,40);
  TH1D * hEvtVz = new TH1D("hEvtVz",";vz (cm);",240,-30,30);
  TH1D * hEvtPtHat = new TH1D("hEvtPtHat",";#hat{p}_{T} (GeV/c);",100,0,500);
  TH1D * hSmJetPtRaw = new TH1D("hSmJetPtRaw",";Leading Jet p_{T} (GeV/c);",100,0,300);
  TH1D * hSmJetPtSm = new TH1D("hSmJetPtSm",";Leading Jet p_{T} (GeV/c);",100,0,300);
  TH1D * hJet1Pt = new TH1D("hJet1Pt",";Leading Jet p_{T} (GeV/c);",100,0,300);
  TH1D * hJet2Pt = new TH1D("hJet2Pt",";Subleading Jet p_{T} (GeV/c);",100,0,300);
  TH1D* smearingHist=0;
  TH2D* smearingBin=0, *smearingvRawPt=0, *smearingvSmPt=0, *smearingPtBin0=0, *smPtvRawPt=0;
  if ( smearCentBin > 0 ) {  
    // the actual smearing hist
    smearingHist = new TH1D("smearingH","",100,-2,2);
    smearingBin = new TH2D("smearingBin",";bin;factor",10,0,10,100,-2,2);
    smearingvRawPt = new TH2D("smearingvRawPt",";p_{T} (GeV/c);factor",30,0,300,100,-2,2);
    smearingvSmPt = new TH2D("smearingvSmPt",";p_{T} (GeV/c);factor",30,0,300,100,-2,2);
    smearingPtBin0 = new TH2D("smearingPtBin0",";p_{T} (GeV/c);factor",30,0,300,100,-2,2);
    smPtvRawPt = new TH2D("smPtvRawPt",";Raw p_{T} (GeV/c);Smeared p_{T} (GeV/c)",30,0,300,30,0,300);
  }
  
  // Analysis ntuples
  TTree* newtreehlt;
  if ( c->hltTree != 0 ) {
    newtreehlt = c->hltTree->CloneTree(0);
    newtreehlt->SetName("yongsunHltTree");
    newtreehlt->SetMaxTreeSize(30000000000);
  }

  TTree* newtreeSkim;
  newtreeSkim = c->skimTree->CloneTree(0);
  newtreeSkim->SetName("yongsunSkimTree");
  newtreeSkim->SetMaxTreeSize(30000000000);

  TTree* newtreeEvt;
  newtreeEvt = c->evtTree->CloneTree(0);
  newtreeEvt->SetName("yongsunHiEvt");
  newtreeEvt->SetMaxTreeSize(30000000000);

  TTree* newtreeAk3pfJet;
  newtreeAk3pfJet =  c->akPu3jetTree->CloneTree(0);
  newtreeAk3pfJet->SetName("yongsunAk3PfJet");
  newtreeAk3pfJet->SetMaxTreeSize(30000000000);

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

  IndividualJet dj;
  EvtSel evt;
  TTree *tdj;
  tdj = new TTree("tdj","Dijet tree");
  tdj->SetMaxTreeSize(30000000000);
  tdj->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:nG:nJ:nT:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:reweight/F"); // todo : add ncoll later
  tdj->Branch("indiJet",&dj.pthat,"pthat/F:jetPt:jetEta:jetPhi:jetPtGM:jetEtaGM:jetPhiGM:jetUnSmPt:jetInd/I");

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

  //////////////////////////////////////////////////////////////////////////
  // input from MB events for MB on MB mixing!  
  //////////////////////////////////////////////////////////////////////////
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

  //////////////////////////////////////////////////////////////////////////
  // LOOP!!
  //////////////////////////////////////////////////////////////////////////
  int nentries = c->GetEntries();
  if (maxEvents > 0 ) nentries = maxEvents;
  cout << "number of entries = " << nentries << endl;
  int startEntry = 0;
  if ( startEntry > nentries ) {
    cout << "error! startEntry > nentries!!!" << endl;
    return;
  }

  for (Long64_t jentry=startEntry ; jentry<nentries;jentry++) {

    if (jentry% 1000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(4)<<(double)jentry/nentries*100<<endl;

    c->GetEntry(jentry);

    ////////////////////////
    // Event Level Info
    ////////////////////////    
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
    // check
    hEvtCentNoSkim->Fill(evt.cBin);
        
    ////////////////////////
    // Skim
    ////////////////////////
    if (  c->skim.pcollisionEventSelection != 1 ) continue;
    if ( (!isMC) && (c->skim.pHBHENoiseFilter != 1) ) continue; 
    if (doIcPu5CaloSkim && !(c->icPu5.jtpt[0]>90 && fabs(c->icPu5.jteta[0])<3) ) continue;
    // vz cut
    int vzBin = hvz->FindBin(evt.vz) -1;
    if ( (vzBin<0) || ( vzBin >= nVtxBin) ) continue;
    // New centrality bin 
    int cBin2 = evt.cBin;     //   cent2Hist->FindBin(c->evt.hiHF) -1 ; // ok.. go back to 40 centrality bins                           
    // check
    hEvtCent->Fill(evt.cBin);
    hEvtVz->Fill(evt.vz);
    hEvtPtHat->Fill(c->akPu3PF.pthat);

    ////////////////////////
    // Jet
    ////////////////////////
    Jets* theJet = &(c->akPu3PF) ;

    int nJets                          = theJet->nref; 
    if ( useGenJet && isMC )     nJets = theJet->ngen;

    ////////////////////////
    // Apply Smearing
    ////////////////////////
    float jetUnSmPt[MAXMTRK];  
    if (smearCentBin>0) {	
      for (int ij=0; ij< nJets ; ij++) {
        hSmJetPtRaw->Fill(theJet->jtpt[ij]);
        int tempCentBin=-100;
        while (  (tempCentBin >= centBin1[smearCentBin] ) || ( tempCentBin < centBin1[smearCentBin-1] ) ) {
          tempCentBin = smearingCents2->GetRandom() ;
          if ( (tempCentBin >= centBin1[smearCentBin] ) || ( tempCentBin < centBin1[smearCentBin-1] ) ) {
            cout << "smearCentBin = " << smearCentBin << "   and tempCentBin= " << tempCentBin << endl;
            cout << " NO!!! the centrality bin from dice throw is out of the range.  Trying once again" << endl;
          }
        }
        int smCentBin = GetCBin( tempCentBin );
        jetUnSmPt[ij]=theJet->jtpt[ij];
        if (jetUnSmPt[ij]>smearingPtMin) theJet->jtpt[ij]  = GetSmearedPtData(2,smCentBin,theJet->jtpt[ij],0,"");

        float sm=(theJet->jtpt[ij] - jetUnSmPt[ij] )/jetUnSmPt[ij];
//         if ((jetUnSmPt[ij] > 100) && (fabs(theJet->jteta[ij])<2.0)) cout << "smCentBin: " << smCentBin << " Pt Before: " << jetUnSmPt[ij] << " After smearing: " << theJet->jtpt[ij] << " factor: " << sm << endl;
      
        if ( (jetUnSmPt[ij] > 100) && (fabs(theJet->jteta[ij])<2.0)) {
          smearingHist->Fill( sm );
          smearingBin->Fill( smCentBin, sm );
          smearingvRawPt->Fill( jetUnSmPt[ij], sm );
          smearingvSmPt->Fill( theJet->jtpt[ij], sm );
          smPtvRawPt->Fill(jetUnSmPt[ij],theJet->jtpt[ij]);
          if (smCentBin==0) smearingPtBin0->Fill( jetUnSmPt[ij], sm );
        }
        hSmJetPtSm->Fill(theJet->jtpt[ij]);
      }
    }

    dj.clear();

    /// Major update on Nov 12th 2012.  The loop on jets start here.  tree->Fill() function will used at the end of each loop
    dj.jetInd=0;
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


      // Jet selection
      if ( fabs( theJteta ) > tempJetEtaCut ) continue;
      if ( (!useGenJet) && ((theJet->trackMax[ij]/theJet->jtpt[ij])<0.01) ) continue;
      if (  theJtpt < InclusiveJetPtCut ) continue;
      // check jet selection
      if (dj.jetInd==0) hJet1Pt->Fill(theJtpt);
      else if (dj.jetInd==1) hJet2Pt->Fill(theJtpt);

      /// Jet selection is done, Let's fill dj ntuple
      if (isMC)    dj.pthat = theJet->pthat;

      dj.jetPt = theJtpt;
      dj.jetEta = theJteta;
      dj.jetPhi = theJtphi;
      if ( useGenJet) dj.jetPtGM = 0;
      else dj.jetPtGM = theJet->refpt[ij];
      if (smearCentBin>0) dj.jetUnSmPt = jetUnSmPt[ij];
      
      ////////////////////////
      //  Jet business is done..  loop on tracks
      ////////////////////////
      nTrk = 0; 
      for (int it=0; it < c->track.nTrk; it++ ) {
      // track selection
        if ( c->track.trkPt[it] < cuttrkPt ) continue;
        if ( fabs( c->track.trkEta[it]) > cuttrkEta ) continue;
        if ( !c->track.highPurity[it])  continue;   // only high purity high pt tracks and all pixel track
      
        float drFromIndiJet  = getDR( c->track.trkEta[it], c->track.trkPhi[it], dj.jetEta , dj.jetPhi);
      
        if  (drFromIndiJet < drMatching) {
          trkJetDr[nTrk] = drFromIndiJet; 
          trkJetMatch[nTrk] = 1;
        }
        else if ( getDR(c->track.trkEta[it],  c->track.trkPhi[it], -dj.jetEta,   dj.jetPhi ) <= drMatching  ) {
          trkJetDr[nTrk] = getDR(c->track.trkEta[it],  c->track.trkPhi[it], - dj.jetEta,   dj.jetPhi ) ; 
          trkJetMatch[nTrk] = -1;
        }
        else {
          trkJetMatch[nTrk] = 0;  	 // matchedJetPt = 0 ;
          trkJetDr[nTrk] = 999;
          if ( saveOnlyJetConeTrks)  continue;  // Save only the tracks in jet cones and reflected cones
        }
      
        trkPt[nTrk]  = c->track.trkPt[it];
        trkEta[nTrk] = c->track.trkEta[it];
        trkPhi[nTrk] = c->track.trkPhi[it]; 
        trkWeight[nTrk] = c->getTrackCorrection(it);
        trkWeightPara[nTrk] = c->getTrackCorrectionPara(it);
        nTrk++;
      }

      ////////////////////////
      /// Tracks are done now it's gen particie
      // Yongsun removed ESum, HSum etc..
      ////////////////////////
      nGenPar = 0;
      if ( isMC ) {
        for ( int ig = 0 ; ig < c->genparticle.mult ; ig++) {
          // gen particle selection
          if ( c->genparticle.pt[ig] < cuttrkPt ) continue;
          if ( fabs( c->genparticle.eta[ig] ) > cuttrkEta ) continue;
      
          float drFromIndiJet  = getDR( c->genparticle.eta[ig], c->genparticle.phi[ig],  dj.jetEta , dj.jetPhi);
      
          if  ( drFromIndiJet <= drMatching) {	
            genParJetMatch[nGenPar] = 1;
            genParJetDr[nGenPar] = drFromIndiJet; 
          }
          else if ( getDR( c->genparticle.eta[ig], c->genparticle.phi[ig],  -dj.jetEta , dj.jetPhi) <=drMatching ) {
            genParJetMatch[nGenPar] = -1; 
            genParJetDr[nGenPar] =  getDR( c->genparticle.eta[ig], c->genparticle.phi[ig],  - dj.jetEta , dj.jetPhi);
          }
          else {  
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
      }

      ////////////////////////
      // track mixing 
      ////////////////////////
      if  (MinbiasFname != "") {
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
            // track selection
            if ( imbTrkpt[it] < cuttrkPt ) continue;
            if ( fabs(imbTrketa[it]) > cuttrkEta ) continue;
        
            ptMtrk[nMtrk]  =   imbTrkpt[it];
            etaMtrk[nMtrk] =   imbTrketa[it];
            phiMtrk[nMtrk] =   imbTrkphi[it];
            weightMtrk[nMtrk] = imbTrkWeight[it];
            weightParaMtrk[nMtrk] = imbTrkWeightPara[it];
        
            float drFromIndiJet  = getDR( etaMtrk[nMtrk], phiMtrk[nMtrk],  dj.jetEta , dj.jetPhi);
        
            if  (drFromIndiJet<=drMatching) {             //	   matchedJetPt = dj.lJetPt;
              jetMatchMtrk[nMtrk] = 1;
              jetDrMtrk[nMtrk] = drFromIndiJet;
            } else if (  getDR( etaMtrk[nMtrk], phiMtrk[nMtrk],  -dj.jetEta , dj.jetPhi) <=drMatching ){       
              jetMatchMtrk[nMtrk] = -1;
              jetDrMtrk[nMtrk] =  getDR( etaMtrk[nMtrk], phiMtrk[nMtrk],  -dj.jetEta , dj.jetPhi);
            } else {                              //    matchedJetPt = 0 ;
              jetMatchMtrk[nMtrk] = 0;
              jetDrMtrk[nMtrk] = 999;
              if ( saveOnlyJetConeTrks)  continue;  // just tantatively... July 8th 2012
            }
            nMtrk++;
          }
        
          //////////////////////////.................................................
          if ( isMC ) {
            for (int ig = 0 ; ig < nimbGenPar ; ig++) {
              // gen particle selection
              if ( imbGenParPt[ig] < cuttrkPt ) continue;
              if ( fabs(imbGenParEta[ig]) > cuttrkEta ) continue;
        
              ptMgenPar[nMgenPar]  = imbGenParPt[ig];
              etaMgenPar[nMgenPar] = imbGenParEta[ig];
              phiMgenPar[nMgenPar] = imbGenParPhi[ig];
              subeMgenPar[nMgenPar]= imbGenParSube[ig];
              chgMgenPar[nMgenPar] = imbGenParChg[ig];
        
              float drFromIndiJet  = getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar], dj.jetEta , dj.jetPhi);
        
              if (drFromIndiJet<=drMatching) {
                jetDrMgenPar[nMgenPar] = drFromIndiJet;
                jetMatchMgenPar[nMgenPar] = 1;
              } else if ( getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar], -dj.jetEta , dj.jetPhi) <= drMatching) {
                jetDrMgenPar[nMgenPar] = getDR( etaMgenPar[nMgenPar], phiMgenPar[nMgenPar], -dj.jetEta , dj.jetPhi);
                jetMatchMgenPar[nMgenPar] = -1;
              } else {
                jetDrMgenPar[nMgenPar] = 999;
                jetMatchMgenPar[nMgenPar] = 0;
                if ( saveOnlyJetConeTrks)  continue;
              }
              nMgenPar++;
            }
          }
        }
        if (noSuchEvent == true ) continue;
      }

      ////////////////////////
      // Fill Trees
      ////////////////////////
      if (tdj) tdj->Fill();
      if (newtreeSkim) newtreeSkim->Fill();
      if (newtreeEvt) newtreeEvt->Fill();
      if (newtreeTrack) newtreeTrack->Fill();
      if (newtreeGenPar) newtreeGenPar->Fill();
      if (tmixTrk) tmixTrk->Fill();
      if (tmixGenPar) tmixGenPar->Fill();
      ++dj.jetInd;
    } // Jet loop done!
  } // Event loop done

  //////////////////////////////////////////////////////////////////////////
  // All Done Take care of finishing business
  //////////////////////////////////////////////////////////////////////////
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
