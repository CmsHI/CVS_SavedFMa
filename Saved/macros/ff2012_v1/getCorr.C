#include "CutAndBinCollection2011.h"
#include "photonIDEfficiency.C"
const int kData =  0;
const int kSig  =  1;
const int kSBB  =  2;
const int kMCBsr = 3;
const int kMCBsb = 4;

const int kSumIso = 1;
const int k3dIso =  2;
const int kFisher = 3;
const int kNoIso  = 4;
const int kSumIso2= 5;
const int kSumIso3= 6;

TCut getIsoCut( int isoChoice=0, float isoCut = -100 ) {
   
   if ( (isoChoice== kSumIso2) && ( isoCut > -99))
      return Form("(cc4+cr4+ct4PtCut20)/0.9 < %f",isoCut);
   if         (isoChoice == kSumIso) return isoSumCut;
   if         (isoChoice == kSumIso3) return isoSumCut;
   else    if (isoChoice == k3dIso ) return iso3dCut;
   else    if (isoChoice == kFisher) return FisherCut;
   else    if (isoChoice == kNoIso) return "";
   else   cout << "!!!!!!! No such isolation choice" << endl;
}
TString getIsoLabel ( int isoChoice=0) {
   if ( isoChoice == kSumIso) return "sumIso";
   if ( isoChoice == k3dIso)  return "3dIso";
   if ( isoChoice == kFisher) return "fisherIso";
   if ( isoChoice == kSumIso2) return "sumIso";
   if ( isoChoice == kSumIso3) return "sumIso";

   else   cout << "!!!!!!! No such isolation choice" << endl;
}



void getCorr(TString fname1="../forest/barrelHiForestPhotonV7.root", TCut isoCut = isoSumCut, int lowCent=0, int highCent=11, TCut addCut="") {
  
  char* fnamePho50 = "barrelHiForestPhoton_MCphoton50_37k.root";
  float nEvtPho50     = 37188;
  char* fnamePho80 = "barrelHiForestPhoton_MCphoton80_25k.root";
  float nEvtPho80     = 25308;
  
  TCut evtSelCut = "tgj.anaEvtSel";
  TCut centCut     = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
  
  TCut photonJetCut  = "tgj.photonEt>100  &&  tgj.jetEt>50 && abs(tgj.jetEta)<1.44";
  TCut dphiCut= "acos(cos(tgj.photonPhi-tgj.jetPhi))>2.0944";
  TCut goodGJCut = "sigmaIetaIeta < 0.011";
  
  TCut generalCut   = photonJetCut && dphiCut && centCut && addCut;
  TCut photon2Cut  = "sqrt(  (acos(cos (phi - tgj.jetPhi) ) )^2 + ( eta - tgj.jetEta )^2  ) < 0.4";

  TCut finalCut= generalCut  && goodGJCut && photon2Cut;
  bool isData(true);
  if (isData)
    finalCut = finalCut && evtSelCut;
  TFile* f1 = new TFile(fname1.Data());
  TTree* photon1 = (TTree*)f1->Get("yongsunPhotonTree");
  photon1->AddFriend("yEvt=yongsunHiEvt");
  photon1->AddFriend("tgj");
  
  TH1D* h1 = new TH1D("hpt1",";(GeV);Entries",200,0,200);
  h1->Sumw2();
  photon1->Draw("corrPt>>hpt1",   finalCut);
  int nScale = photon1->GetEntries(generalCut);
  cout << " n = " << nScale << endl;
  h1->Draw();
}
