#ifndef CutAndBinCollection_C
#define CutAndBinCollection_C
double a1, a2, a3, a4, a5, a6;


#include <TF1.h>
#include <TCut.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "commonUtility.h"
#include "multiTreeUtilPhoton2011.h"
#include "histFunctionD.C"
#define PI 3.141592653589


const float awayRange= PI * 7./8.;


// DON'T FORGET TO APPLY HOE CUT SEPARATELY    
// Convinient Output Classes                                                                                                                 

// sample typle
const int kHIDATA =1;
const int kHIMC   =2;
const int kPPDATA =3;
const int kPPMC   =4;
const int nMixing1 = 10;
const int nMixing2 = 1;

const int kHIMCNoJetWeight = 2001;

struct valPair {
  double val;
  double err;
};




class GammaJet {
  public:
    GammaJet() :
      photonEt(-99),photonRawEt(-99),
      photonEta(9999),photonPhi(9999),
      hovere(10000),
      sigmaIetaIeta(-99),
      sumIsol(99999),
      genIso(99999),
      genMomId(-999999),
      lJetPt(-999990),
      lJetEta(-999990),
      lJetPhi(-999990)


  {}
    float photonEt,photonRawEt;
    float photonEta;
    float photonPhi;
    float hovere,sigmaIetaIeta, sumIsol;
    float genIso;
    int genMomId;
    float lJetPt;
    float lJetEta;
    float lJetPhi;

    void clear() {
      photonRawEt=-99; photonEt = -99999; photonEta=100; photonPhi=1000;
      sigmaIetaIeta=-99;
      sumIsol = -99;
      genIso = 9990;
      genMomId = -999999;
      hovere  =  1000;
      lJetPt = -99999;
      lJetEta = -99999;
      lJetPhi = -99999;

    }
};

class DiJet {
  public:
    DiJet() :
      pthat(-99),
      lJetPt(-99), slJetPt(-99),
      lJetRawPt(-99), slJetRawPt(-99),
      lJetEta(-99), slJetEta(-99),
      lJetPhi(-99), slJetPhi(-99),
      lJetPtGM(-99), slJetPtGM(-99),
      lJetEtaGM(-99), slJetEtaGM(-99),
      lJetPhiGM(-99), slJetPhiGM(-99),
      lJetChgSum(-99), slJetChgSum(-99),
      lJetPhoSum(-99), slJetPhoSum(-99),
      lJetNtrSum(-99), slJetNtrSum(-99),
      lJetRecoEoH(-99), slJetRecoEoH(-99),
      lJetGenEoH(-99), slJetGenEoH(-99),
      lJetFlav(-99),    slJetFlav(-99)


  {}
    float pthat;
    float lJetPt, slJetPt;
    float lJetRawPt, slJetRawPt;
    float lJetEta, slJetEta;
    float lJetPhi, slJetPhi;
    float lJetPtGM, slJetPtGM;
    float lJetEtaGM, slJetEtaGM;
    float lJetPhiGM, slJetPhiGM;
    float lJetChgSum,   slJetChgSum;
    float lJetPhoSum,   slJetPhoSum;
    float lJetNtrSum,   slJetNtrSum;
    float lJetRecoEoH, slJetRecoEoH;
    float lJetGenEoH, slJetGenEoH;
    float lJetFlav,    slJetFlav;



    void clear() {
      pthat=-99;
      lJetPt = -99; slJetPt = -99;
      lJetRawPt = -99; slJetRawPt = -99;
      lJetEta = -99; slJetEta = -99;
      lJetPhi = -99; slJetPhi = -99;
      lJetPtGM = -99; slJetPtGM = -99;
      lJetEtaGM = -99; slJetEtaGM = -99;
      lJetPhiGM = -99; slJetPhiGM = -99;
      lJetChgSum = -99; slJetChgSum = -99; 
      lJetPhoSum = -99; slJetPhoSum = -99;
      lJetNtrSum = -99; slJetNtrSum = -99;
      lJetRecoEoH = -99; slJetRecoEoH = -99;
      lJetGenEoH = -99; slJetGenEoH = -99;
      lJetFlav      = -99; slJetFlav = -99;  

    }
};


class IndividualJet {
  public:
    IndividualJet() :
      pthat(-99),
      jetPt(-99),
      jetEta(-99),
      jetPhi(-99),
      jetPtGM(-99),
      jetEtaGM(-99),
      jetPhiGM(-99),
      jetInd(-99)

  {}
    float  pthat;
    float  jetPt;
    float  jetEta;
    float  jetPhi;
    float  jetPtGM;
    float  jetEtaGM;
    float  jetPhiGM;
    int jetInd;


    void clear() {
      pthat=-99;
      jetPt=-99;
      jetEta=-99;
      jetPhi=-99;
      jetPtGM=-99;
      jetEtaGM=-99;
      jetPhiGM=-99;
      jetInd=-99;
    }
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

class fitResult {
  public:
    double nSig;
    double nSigErr;
    double purity010;
    double chisq;
};

TCut isoSumCut  = "(cc4+cr4+ct4PtCut20)/0.9 <1";

TCut sbIsoCut =" (cc4+cr4+ct4PtCut20)/0.9>10 && (cc4+cr4+ct4PtCut20)/0.9 < 20 ";
TCut sbIsoPPCut = sbIsoCut;


float isolationCut = 5.0;

TCut isFragment = "abs(genMomId)<22";
TCut isPrompt = "abs(genMomId)==22";

TCut genMatchCut0      = "isGenMatched && abs(genMomId)<=22";
TCut genMatchCut1      = Form("isGenMatched && genMomId==22 && genCalIsoDR04 < %.1f",isolationCut);
TCut genMatchCut      = Form("(isGenMatched && abs(genMatchedEta)<1.44 && abs(etCorrected/genMatchedPt-1)<.3 && abs(genMomId) <= 22 && genCalIsoDR04 < %.1f)",isolationCut);
TCut genMatchCutBkg      = "(isGenMatched && abs(genMatchedEta)<1.44 && abs(etCorrected/genMatchedPt-1)<.6)  &&  ( (abs(genMomId) > 22) || (genCalIsoDR04 > 5.0) ) ";


TCut genPhotonCut     = Form("( abs(gpEta) < 1.44 && abs(gpId)==22 && abs(gpMomId) <= 22 && gpCollId ==0  && gpIsoDR04 < %.3f)",isolationCut);


TString swissCrx      = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
TCut hiSpikeCutMC     = Form("(  %s < 0.90 && sigmaIetaIeta>0.002 && sigmaIphiIphi>0.002)",swissCrx.Data());
TCut ppSpikeCutMC     = Form("(  %s < 0.95 && sigmaIetaIeta>0.002 && sigmaIphiIphi>0.002)",swissCrx.Data());

//TCut hiSpikeCutNoPhi  = Form("( ( %s < 0.90 && sigmaIetaIeta>0.002) ",swissCrx.Data());      

//TCut hiSpikeCutNoPhi  = Form("( ( %s < 0.90 && sigmaIetaIeta>0.002) ",swissCrx.Data());


TCut seedTimeCut      = "abs(seedTime)<3";
TCut hiSpikeCutData   = hiSpikeCutMC && seedTimeCut;
TCut ppSpikeCutData   = ppSpikeCutMC && seedTimeCut;

TCut etaCut       = "abs(eta)<1.44 && abs(scEta)<1.479";

//TCut etaCut           = " (abs(scEta) < 1.479 && abs(eta)<1.44) && (rawEnergy/energy > 0.5)";// && (!isEBGap&&!isEEGap&&!isEBEEGap)";

TCut genEtaCut  =       "(abs(eta) < 1.44)";
TCut vtxCut1     = "abs(vtxZ)<15";
TCut finalCutSigHI  = genMatchCut     &&  hiSpikeCutMC && etaCut && vtxCut1 ;
TCut finalCutBkgHI  = !genMatchCut  &&  hiSpikeCutMC && etaCut && vtxCut1 ;
TCut finalCutDataHI =                     hiSpikeCutData && etaCut  && vtxCut1;

TCut finalCutSigPP  = genMatchCut     &&  ppSpikeCutMC && etaCut && vtxCut1 ;
TCut finalCutBkgPP  = !genMatchCut  &&  ppSpikeCutMC && etaCut && vtxCut1;
TCut finalCutDataPP =                     ppSpikeCutData && etaCut  && vtxCut1;



TCut finalCutGen  =  genPhotonCut && vtxCut1 ;



// Cuts for track and jets
double cutjetPtPre = 20;
double cutjetEtaPre = 2;
double cutjetEta = 2;
double cuttrkPt =  1; //moved to anaTrack
double cuttrkEta = 2.4;
double cuttrkEtaForAna = 2.4;
double drCutTJ = 0.5 ;  // cut for tracks in jets    


int ycolor[9] =  {0,1,2,4,8,1,1,1,1};  // the for centrality bins...
int ycolorEt[9]= {0,1,2,4,8,1,1,1,1};

// temporary!!!
double centBin1[5] = {0,4,12,20,40};
double rjBin1[5] = {0, 0.52, 0.7, 0.82,1} ;
const int nVtxBin = 8;
float vtxCut = 15;
const int nPlnBin = 24;
float jetDPhiCut = PI * 7./8.;



void mcStyle(TH1* h=0) {
  h->SetLineColor(kPink);
  h->SetFillColor(kOrange+7);
  h->SetFillStyle(3004);
}

void sbStyle(TH1* h=0) {
  h->SetLineColor(kGreen+4);
  h->SetFillColor(kGreen+1);
  h->SetFillStyle(3001);
}

void dijetStyle(TH1* h=0) {
  h->SetLineColor(kBlue);
  h->SetFillColor(kAzure-8);
  h->SetFillStyle(3001);
}

double getNoEmc (TString theFname="", TCut theCut="") {
  TFile *fd = new TFile(theFname.Data());
  TTree *ana = (TTree*) fd->FindObjectAny("Analysis");
  cout << "number of events of " << theCut.GetTitle() << "    : " << ana->GetEntries( theCut ) << endl;
  return ana->GetEntries( theCut ) ;
}


void setupMTU(multiTreeUtil* photon1=0 , int sampleType = kHIDATA){

  if ( sampleType == kHIDATA) {
    photon1->addFile("skimmed/yskim_HiForestPhoton-v7-noDuplicate_nMix10_YesEvtPlReq_v6Mixed_mrgTrkCollection.root","tgj","",1);
  }
  else if ( sampleType == kPPDATA) {
    photon1->addFile("skimmed/yskim_HiForestPhoton-pp_photon50GeV_nMix10_YesEvtPlReq_v6Mixed_mrgTrkCollection.root","tgj","",1);
  }
  else if ( sampleType == kHIMC) {
    photon1->addFile("skimmed/yskim_qcdAllPhoton30_allCent_nMix10_v7_mrgTrkCollection.root",
	"tgj", "yPhoton.ptHat>30 && yPhoton.ptHat<50" , 1.59);
    photon1->addFile("skimmed/yskim_qcdAllPhoton50_allCent_nMix10_v7_mrgTrkCollection.root",
	"tgj", "yPhoton.ptHat>50" , 0.767);
    // photon1->addFile("skimmed/yskim_qcdAllPhoton80_allCent_nMix10_v5.root" ,
    //		     "tgj", "yPhoton.ptHat>80" , 1.72e-7);
  }

  photon1->AddFriend("yEvt=yongsunHiEvt");
  photon1->AddFriend("ySkim=yongsunSkimTree");
  //  photon1->AddFriend("yHlt=yongsunHltTree");
  photon1->AddFriend("yTrk=yongsunTrack");
  photon1->AddFriend("yJet=yongsunJetakPu3PF");
  photon1->AddFriend("yPhoton=yongsunPhotonTree");
  photon1->AddFriend("mTrk1");
  photon1->AddFriend("mTrk2");
  photon1->AddFriend("ymJet=tmixJet");
  if ( sampleType == kHIMC) {
    photon1->AddFriend("yGen=yGenParticle");
  }
  // photon1->SetAlias("yTrkInjetDr", "sqrt( (acos(cos(yTrk.injetPhi-yTrk.phi)))^2 + (yTrk.injetEta-yTrk.eta)^2)");
  // photon1->SetAlias("mTrk1xjetDr", "sqrt( (acos(cos(mTrk1.xjetPhi-mTrk1.phi)))^2 + (mTrk1.xjetEta-mTrk1.eta)^2)");
  // photon1->SetAlias("mTrk2injetDr","sqrt( (acos(cos(mTrk2.injetPhi-mTrk2.phi)))^2 + (mTrk2.injetEta-mTrk2.eta)^2)");
  // photon1->SetAlias("mTrk1xmjetDr","sqrt( (acos(cos(mTrk1.xmjetPhi-mTrk1.phi)))^2 + (mTrk1.xmjetEta-mTrk1.eta)^2)");
  //  photon1->SetAlias("dphigt","acos(cos(photonPhi-yTrk.trkPhi))");
  //  photon1->SetAlias("dphiginj","acos(cos(photonPhi-yTrk.injetPhi))");
  //  photon1->SetAlias("dphigmt","acos(cos(photonPhi-ymTrk.phi)) ");
  //  photon1->SetAlias("dphigminj","acos(cos(photonPhi-ymTrk.injetPhi))");
  //  photon1->SetAlias("dphigxinj","acos(cos(photonPhi-ymTrk.xjetPhi))");
  //  photon1->SetAlias("dphigxmt","acos(cos(photonPhi-ymmTrk.phi))");
  //  photon1->SetAlias("dphigxminj","acos(cos(photonPhi-ymmTrk.xjetPhi))");
  //  photon1->SetAlias("dphigj","acos(cos(photonPhi-yJet.jtphi))");
  // photon1->SetAlias("dphigmj","acos(cos(photonPhi-ymJet.phi))");

}







class CutAndBinCollection
{
  public:
    CutAndBinCollection() {
    }
    ~CutAndBinCollection() {

    }
    //   float getHoECut( float percentBin=1, float et=30 ); 
    TString getTmplName()  { return tmplName_;};
    TString getTmplVar()   { return tmplVar_;};
    TString getTmplVarMC() { return tmplVarMC_;};
    TString getTmplVarBkg() { return tmplVarBkg_;};

    TString getTmplLeg()   { return tmplLeg_;};
    TString getTmplXTitle(){ return tmplXTitle_;};

    int  getNTmplBin() {     return nTmplBin_;};
    float getTmplMin() {     return tmplMin_;};
    float getTmplMax() {     return tmplMax_;};
    float getConeSize(){     return coneSize_;};
    float getFitMin()  {     return fitMin_;};
    float getFitMax()  {     return fitMax_;};

    bool isHI()      {       return hiOrPp_;};

    bool doJetSpec() {   return doJetSpec_;};

    bool doFragPt()  {   return doFragPt_;};
    bool doFragX()   {   return doFragX_;};
    bool doFragXi()  {   return doFragXi_;};
    bool doFragXij() {   return doFragXij_;};
    bool doIncXi05() {   return doIncXi05_;};
    bool doIncXi10() {   return doIncXi10_;};
    bool doIncKt05() {   return doIncKt05_;};
    bool doIncKt10() {   return doIncKt10_;};

    //   bool doFragPt_, doFragX_, doFragXij_, doFragXi_, doIncXi05_, doIncXi10_;


    int getNPtBin() { return ptBin_.size()-1;};
    int getNJetPtBin() { return jetPtBin_.size()-1;};
    int getNPtBinAssoc() { return ptBinAssoc_.size()-1;};
    int getNCentBin() { return centBin_.size()-1;};

    vector<double> getPtBin() { return ptBin_;};
    vector<double> getCentBin() { return centBin_;};
    vector<double> getPtBinAssoc() { return ptBinAssoc_;};
    vector<double> getJetPtBin() { return jetPtBin_;};
    TString getPurityFileName() { return purityFname_;};
    void setCuts(int cutOpt);
  private:

    TString tmplName_;
    TString tmplVar_;
    TString tmplVarMC_;
    TString tmplVarBkg_;

    TString tmplLeg_;
    TString tmplXTitle_;
    TCut IBCutWOEleVeto_;
    TCut IBCut_;
    TCut SBCut_;
    int nTmplBin_;
    float tmplMin_;
    float tmplMax_;
    float fitMin_;
    float fitMax_;
    bool hiOrPp_;
    float coneSize_;
    bool doJetSpec_, doFragPt_, doFragX_, doFragXij_, doFragXi_, doIncXi05_, doIncXi10_, doIncKt05_, doIncKt10_;


    vector<double> ptBin_;
    vector<double> ptBinAssoc_; // tracks
    vector<double> centBin_;
    vector<double> jetPtBin_;   // jet pt

    TString purityFname_;
    float jetPTCut_;

};

void CutAndBinCollection::setCuts( int cutOpt ) { 
  purityFname_ = Form("photonPurityCollection_cutOption%d.root",cutOpt);
  centBin_.clear();
  ptBin_.clear();
  ptBinAssoc_.clear();
  jetPtBin_.clear();

  doJetSpec_ =false;
  doFragPt_  =false;
  doFragX_   =false; 
  doFragXij_ =false; 
  doFragXi_  =false; 
  doIncXi05_ =false;
  doIncXi10_  =false;
  doIncKt05_ = false;
  doIncKt10_ = false;

  if ( cutOpt == 602003) {
    coneSize_ = 0.3;

    centBin_.clear();
    centBin_.push_back(0);
    centBin_.push_back(4);
    centBin_.push_back(12);
    centBin_.push_back(20);
    centBin_.push_back(40);

    ptBin_.push_back(60);
    ptBin_.push_back(1000);

    jetPtBin_.push_back(20);
    jetPtBin_.push_back(30);
    jetPtBin_.push_back(40);
    jetPtBin_.push_back(50);
    jetPtBin_.push_back(60);
    jetPtBin_.push_back(1000);

    ptBinAssoc_.push_back(0.5);
    ptBinAssoc_.push_back(1000);

    doFragPt_ = true;
    doFragX_   =true;
    doFragXij_ =true;
    doFragXi_  =true;
  }

  else if ( cutOpt == 500000) {
    coneSize_ = 0.3;
    centBin_.clear();
    centBin_.push_back(0);
    centBin_.push_back(4);
    centBin_.push_back(12);
    centBin_.push_back(20);
    centBin_.push_back(40);

    ptBin_.push_back(50);
    ptBin_.push_back(10000);

    jetPtBin_.push_back(20);
    jetPtBin_.push_back(10000);

    ptBinAssoc_.push_back(0.5);
    ptBinAssoc_.push_back(1000);

    doJetSpec_ = true;
    doFragXi_   = false;
    doIncXi05_  = false;
    doIncXi10_  = false;
    doIncKt05_  = false; 
    doIncKt10_  = false; 
  }
  else if ( cutOpt == 500020) {
    coneSize_ = 0.3;

    centBin_.clear();
    centBin_.push_back(0);
    centBin_.push_back(4);
    centBin_.push_back(12);
    centBin_.push_back(20);
    centBin_.push_back(40);

    ptBin_.push_back(50);
    ptBin_.push_back(10000);

    jetPtBin_.push_back(20);
    jetPtBin_.push_back(10000);

    ptBinAssoc_.push_back(0.5);
    ptBinAssoc_.push_back(1000);

    doFragXi_   = true;
    doIncXi05_  = false;
    doIncXi10_  = false;
    doIncKt05_  = false; 
    doIncKt10_  = false; 
  }
  else if ( cutOpt == 500025) {
    coneSize_ = 0.3;

    centBin_.clear();
    centBin_.push_back(0);
    centBin_.push_back(4);
    centBin_.push_back(12);
    centBin_.push_back(20);
    centBin_.push_back(40);

    ptBin_.push_back(50);
    ptBin_.push_back(10000);

    jetPtBin_.push_back(25);
    jetPtBin_.push_back(10000);

    ptBinAssoc_.push_back(0.5);
    ptBinAssoc_.push_back(1000);

    doFragXi_   = true;
    doIncXi05_  = false;
    doIncXi10_  = false;
    doIncKt05_  = false;
    doIncKt10_  = false;
  }

  else if ( cutOpt == 500030) {
    coneSize_ = 0.3;

    centBin_.clear();
    centBin_.push_back(0);
    centBin_.push_back(4);
    centBin_.push_back(12);
    centBin_.push_back(20);
    centBin_.push_back(40);

    ptBin_.push_back(50);
    ptBin_.push_back(10000);

    jetPtBin_.push_back(30);
    jetPtBin_.push_back(10000);

    ptBinAssoc_.push_back(0.5);
    ptBinAssoc_.push_back(1000);

    doFragXi_   = true;
    doIncXi05_  = false;
    doIncXi10_  = false;
    doIncKt05_  = false;
    doIncKt10_  = false;
  }

  else if ( cutOpt == 600000) {
    coneSize_ = 0.5;

    centBin_.clear();
    centBin_.push_back(0);
    centBin_.push_back(4);
    centBin_.push_back(10);
    centBin_.push_back(20);
    centBin_.push_back(40);

    ptBin_.push_back(60);
    ptBin_.push_back(10000);

    jetPtBin_.push_back(20);
    jetPtBin_.push_back(10000);

    ptBinAssoc_.push_back(0.5);
    ptBinAssoc_.push_back(1000);

    doFragXi_   = false;
    doIncXi05_  = true;
    doIncXi10_  = true;
    doIncKt05_  = false;
    doIncKt10_  = false;
  }



  else 
    cout << "  we dont have such template option " << endl;



}




/*
   float CutAndBinCollection::getHoECut(   float percentBin=false; float et ) {  // 100% centrality used   
   float thehoe = 0.1;
   if ( percentBin <= 20 ) {   // should change univHoEnCut  accordingly
   if         ( et <=30 )                thehoe = 0.15;
   else                                  thehoe = 0.1;
   }
   if ( percentBin > 20 ) {
   if         ( et <=30 )                thehoe = 0.1;
   `else                                  thehoe = 0.05;
   }

   cout << endl << " centrality = " << percentBin << "%" << "    et = " << et << "     hoeCut = " << thehoe << endl;
   return thehoe;
   }

 */


void addCentralityFriend(TTree *tSig, TTree *tData,TCut selectionCut)
{
  //copied from /d101/yjlee/HIPhoton/ana/photonAna2011/common.h
  static int counter=0;
  TH1D *hSigCent = new TH1D("hSigCent","",40,-0.5,39.5);
  TH1D *hDataCent = new TH1D("hDataCent","",40,-0.5,39.5);

  hSigCent->SetLineColor(2);
  tSig->Project("hSigCent","cBin",selectionCut);
  tData->Project("hDataCent","cBin",selectionCut);
  hDataCent->Scale(1./hDataCent->GetEntries());
  hSigCent->Scale(1./hSigCent->GetEntries());
  hDataCent->Divide(hSigCent);
  TNtuple *nt = new TNtuple(Form("ntCentFriend%d",counter),"","cBinWeight");

  Int_t cBin;
  tSig->SetBranchAddress("cBin",&cBin);

  for (int i=0;i<tSig->GetEntries();i++)
  {
    tSig->GetEntry(i);
    int bin = hDataCent->FindBin(cBin);
    //cout <<cBin<<" "<<hDataCent->GetBinContent(bin)<<endl;                                                                                                                                                    
    nt->Fill(hDataCent->GetBinContent(bin));
  }
  counter++;
  delete hSigCent;
  delete hDataCent;
  tSig->AddFriend(nt);
}


void getNColl( float* ncoll) {

  ncoll[0] = 1747.86 ; 
  ncoll[1] = 1567.53 ; 
  ncoll[2] = 1388.39 ; 
  ncoll[3] = 1231.77 ; 
  ncoll[4] = 1098.2 ; 
  ncoll[5] = 980.439 ; 
  ncoll[6] = 861.609 ; 
  ncoll[7] = 766.042 ; 
  ncoll[8] = 676.515 ; 
  ncoll[9] = 593.473 ; 
  ncoll[10] = 521.912 ; 
  ncoll[11] = 456.542 ; 
  ncoll[12] = 398.546 ; 
  ncoll[13] = 346.647 ; 
  ncoll[14] = 299.305 ; 
  ncoll[15] = 258.344 ; 
  ncoll[16] = 221.216 ; 
  ncoll[17] = 188.677 ; 
  ncoll[18] = 158.986 ; 
  ncoll[19] = 134.7 ; 
  ncoll[20] = 112.547 ; 
  ncoll[21] = 93.4537 ; 
  ncoll[22] = 77.9314 ; 
  ncoll[23] = 63.5031 ; 
  ncoll[24] = 52.0469 ; 
  ncoll[25] = 42.3542 ; 
  ncoll[26] = 33.9204 ; 
  ncoll[27] = 27.3163 ; 
  ncoll[28] = 21.8028 ; 
  ncoll[29] = 17.2037 ; 
  ncoll[30] = 13.5881 ; 
  ncoll[31] = 10.6538 ; 
  ncoll[32] = 8.35553 ; 
  ncoll[33] = 6.40891 ; 
  ncoll[34] = 5.13343 ; 
  ncoll[35] = 3.73215 ; 
  ncoll[36] = 3.06627 ; 
  ncoll[37] = 2.41926 ; 
  ncoll[38] = 2.11898 ; 
  ncoll[39] = 1.76953 ; 

}



fitResult doFit(TH1D* hSig=0, TH1D* hBkg=0, TH1D* hData1=0, float varLow=0.001, float varHigh=0.028, bool drawLeg=true) {

  TH1D* hDatatmp = (TH1D*)hData1->Clone(Form("%s_datatmp",hData1->GetName()));
  int nBins = hDatatmp->GetNbinsX();
  histFunction2 *myFits = new histFunction2(hSig,hBkg);
  TF1 *f = new TF1("f",myFits,&histFunction2::evaluate,varLow,varHigh,2);
  f->SetParameters( hDatatmp->Integral(1,nBins+1), 0.3);
  f->SetParLimits(1,0,1);
  hDatatmp->Fit("f","LL M O Q","",varLow,varHigh);
  hDatatmp->Fit("f","LL M O Q","",varLow,varHigh);

  fitResult res;
  res.nSig =0;
  double nev = f->GetParameter(0);
  double ratio = f->GetParameter(1);
  double ratioErr = f->GetParError(1);
  res.nSig    = nev * ratio;
  res.nSigErr = nev * ratioErr;
  res.chisq = (double)f->GetChisquare()/ f->GetNDF();

  TH1F *hSigPdf = (TH1F*)hSig->Clone(Form("%s_tmp",hSig->GetName()));
  hSigPdf->Scale(res.nSig/hSigPdf->Integral(1,nBins+1));

  TH1F *hBckPdf = (TH1F*)hBkg->Clone(Form("%s_tmp",hBkg->GetName()));
  hBckPdf->Scale((nev-res.nSig)/hBckPdf->Integral(1,nBins+1));

  double ss1 = hSigPdf->Integral(1, hSigPdf->FindBin(0.00999),"width");
  double bb1 = hBckPdf->Integral(1, hBckPdf->FindBin(0.00999),"width");
  //   cout <<"  hte bin = " <<hSigPdf->FindBin(0.00999) << endl;
  res.purity010 = ss1/(ss1+bb1);
  cout << "purity = " << res.purity010 << endl;
  hSigPdf->Add(hBckPdf);
  handsomeTH1(hSigPdf);
  mcStyle(hSigPdf);
  sbStyle(hBckPdf);
  cleverRange(hSigPdf,1.5);
  hSigPdf->SetNdivisions(510);

  hSigPdf->SetYTitle("Entries");
  hSigPdf->DrawCopy("hist");
  hBckPdf->DrawCopy("same hist");
  hData1->DrawCopy("same");
  TH1D* temphSigPdf = (TH1D*)hSigPdf->Clone("temp1");
  TH1D* temphBckPdf = (TH1D*)hBckPdf->Clone("temp2");
  if(drawLeg){
    TLegend *t3=new TLegend(0.5402006,0.5963235,0.9186019,0.7853466,NULL,"brNDC");
    t3->AddEntry(hData1,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    t3->AddEntry(temphSigPdf,"Signal","lf");
    t3->AddEntry(temphBckPdf,"Background","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
    drawCMS2011(0.53,0.9,150,16);
  }


  //   delete hSigPdf;                                                                                                                                                                                             
  //   delete hBckPdf;                                                                                                                                                                                             

  return res;

}



#endif
