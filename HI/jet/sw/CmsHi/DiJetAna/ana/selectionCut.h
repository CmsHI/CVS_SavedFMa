#include "TH1.h"
#include <TCut.h>
#include <TString.h>
#include <iostream>

class selectionCut
{  
    public:
    selectionCut(bool isMC, int sel, float NrJetMin=80, float NrJetMax=120, float AwJetMin=70, int runnum=-1, int nLumiL=0, int nLumiH=10000);
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

selectionCut::selectionCut(bool isMC, int sel, float NrJetMin, float NrJetMax, float AwJetMin, int runnum, int nLumiL, int nLumiH) :
  selType(sel),
  runNum(runnum),
  histJetPtBins(30),
  histJetPtMin(0),
  histJetPtMax(150),
  numJetPtBin(20),
  nrJetPtMin(NrJetMin),
  nrJetPtMax(NrJetMax),
  awJetPtMin(AwJetMin),
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
   // some default setup
   VzRangeL = -30; 
   VzRangeH = 30;
   vtxCut = Form("vz[1]<%d&&vz[1]>%d",VzRangeH,VzRangeL);
   runCut = Form("nRun==%d",runNum);
   TH1::SetDefaultSumw2();

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
     Tag = "SelAna";
   } else if (selType==11) {
     DJ = Form("nljet>%.1f&&nljet<%.1f&&aljet>%.1f&&abs(nljeta)<%.1f&&abs(aljeta)<%.1f&&jdphi>%f",
	 nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
     DJ = DJ && "nlrjet>50&&alrjet>50";
     Evt = TString(DJ);
     Trk = ("ppt>0.3 && ppt<nljet");
     DJTrk = DJ&&Trk;
     Tag = "SelAna_GenMatchCalo";
   } else if (selType==101) {
     DJ = Form("nlrjet>%.1f&&nlrjet<%.1f&&alrjet>%.1f&&abs(nlrjeta)<%.1f&&abs(alrjeta)<%.1f&&rjdphi>%f",
	 nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
     Evt = TString(DJ);
     Trk = ("ppt>0.3 && ppt<nljet");
     DJTrk = DJ&&Trk;
     Tag = "SelRef";
   } else if (selType==111) {
     DJ = Form("nlrjet>%.1f&&nlrjet<%.1f&&alrjet>%.1f&&abs(nlrjeta)<%.1f&&abs(alrjeta)<%.1f&&rjdphi>%f",
	 nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
     DJ = DJ && "nlrjet>50&&alrjet>50";
     Evt = TString(DJ);
     Trk = ("ppt>0.3 && ppt<nljet");
     DJTrk = DJ&&Trk;
     Tag = "SelRef_GenMatchCalo";
   } else if (selType==1001) {
     DJ = Form("nrlrjet>%.1f&&nrlrjet<%.1f&&arlrjet>%.1f&&abs(nrlrjeta)<%.1f&&abs(arlrjeta)<%.1f&&rjdphi>%f",
	 nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
     Evt = TString(DJ);
     Trk = ("ppt>0.3 && ppt<nljet");
     DJTrk = DJ&&Trk;
     Tag = "SelRef";
   } else if (selType==1011) {
     DJ = Form("nrlrjet>%.1f&&nrlrjet<%.1f&&arlrjet>%.1f&&abs(nrlrjeta)<%.1f&&abs(arlrjeta)<%.1f&&rjdphi>%f",
	 nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
     DJ = DJ && "nlrjet>50&&alrjet>50";
     Evt = TString(DJ);
     Trk = ("ppt>0.3 && ppt<nljet");
     DJTrk = DJ&&Trk;
     Tag = "SelRef_GenMatchCalo";
   }

   CutWOVtxCut = TCut(Evt);
   Cut = TCut(Evt);

   AnaTag = Form("Sel%d_N%0.fto%.0f_A%.0f_DPhi%.0f",selType,nrJetPtMin,nrJetPtMax,awJetPtMin,djDPhiMin*10);
   cout << "Ana: " << AnaTag << endl;
   cout << " w/ DJ cut: " << TString(DJ) << endl;
}
