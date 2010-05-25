#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int sel, int runnum=-1, int nLumiL=0, int nLumiH=10000);
    ~selectionCut(){}
    
    // ana setup
    int selType;

    TCut Cut; 
    TCut runCut;
    TString Evt;
    TCut CutWOVtxCut;
    TString vtxCut;
    TString myCut;
    TCut trigger;
    TCut noHalo;
    TCut noMonster;
    TCut hfCoinc;
    TCut DJ;
    TCut Trk;
    TCut DJTrk;

    // evt sel
    int numVzBin;
    int VzRangeL;
    int VzRangeH;

    // run info
    int runNum;
    
    // ana info
    TString AnaTag;
    TString Tag;

    // jet
    int histJetPtBins;
    float histJetPtMin;
    float histJetPtMax;
    int numJetPtBin;
    float nrJetPtMin;
    float nrJetPtMax;
    float awJetPtMin;
    float djDPhiMin;
    int histJetEtaBins;
    float histJetEtaMin;
    float histJetEtaMax;
    float jetEtaMin;
    float jetEtaMax;
    int histJetPhiBins;
    float histJetPhiMin;
    float histJetPhiMax;

    // trk
    int numTrkPtBin;
    float trkPtMin;
    float trkPtMax;
};

selectionCut::selectionCut(bool isMC, int sel, int runnum, int nLumiL, int nLumiH) :
  selType(sel),
  runNum(runnum),
  histJetPtBins(30),
  histJetPtMin(0),
  histJetPtMax(150),
  numJetPtBin(20),
  nrJetPtMin(80),
  nrJetPtMax(120),
  awJetPtMin(70),
  djDPhiMin(3.14*5/6),
  histJetEtaBins(40),
  histJetEtaMin(-3.0),
  histJetEtaMax(3.0),
  jetEtaMin(0.),
  jetEtaMax(2.),
  histJetPhiBins(40),
  histJetPhiMin(-3.14),
  histJetPhiMax(3.14)
{
   VzRangeL = -30; 
   VzRangeH = 30;
   vtxCut = Form("vz[1]<%d&&vz[1]>%d",VzRangeH,VzRangeL);
   runCut = Form("nRun==%d",runNum);

   // define selections here
   trigger = "L1A[124]";
   noHalo = "(!L1T[36] && !L1T[37] && !L1T[38] && !L1T[39])";
   noMonster = "(vtxqual>(0.0045*npxhits) || npxhits<150 || vtxqual>2)";
   hfCoinc = "nHFp>0&&nHFn>0";

   // Basic Dijet selection
   if (selType==1) {
     DJ = Form("nljet>%.1f&&nljet<%.1f&&aljet>%.1f&&abs(nljeta)<%.1f&&abs(aljeta)<%.1f&&jdphi>%f",
	 nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
     Evt = TString(DJ);
     Trk = ("ppt>0.3 && ppt<nljet");
     DJTrk = DJ&&Trk;
     Tag = "ana02";
   }

   CutWOVtxCut = TCut(Evt);
   Cut = TCut(Evt);

   AnaTag = Form("Sel%d_Nr_%0.f_%.0f_Aw_%.0f_DPhi_%.1f",selType,nrJetPtMin,nrJetPtMax,awJetPtMin,djDPhiMin);
}
