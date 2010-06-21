#include "TH1.h"
#include <TCut.h>
#include <TString.h>
#include <iostream>
#include <iomanip>
#include <map>

class selectionCut
{  
  public:
    selectionCut(TString name, bool isMC, int sel, float NrJetMin=80, float NrJetMax=120, float AwJetMin=70, int runnum=-1, int nLumiL=0, int nLumiH=10000);
    ~selectionCut(){}
    void Print();

    // ana setup
    int selType;

    TCut Cut; 
    TCut runCut;
    TString Evt;
    TCut CutWOVtxCut;
    TString vtxCut;
    TCut trigger;
    TCut noHalo;
    TCut noMonster;
    TCut hfCoinc;
    std::map<TString,TCut> DJ;
    std::map<TString,TCut> Trk;

    // evt sel
    int numVzBin;
    int VzRangeL;
    int VzRangeH;

    // run info
    int runNum;

    // ana info
    TString Name;
    TString AnaTag;

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

selectionCut::selectionCut(TString name, bool isMC, int sel, float NrJetMin, float NrJetMax, float AwJetMin, int runnum, int nLumiL, int nLumiH) :
  Name(name),
  selType(sel),
  runNum(runnum),
  histJetPtBins(50),
  histJetPtMin(0),
  histJetPtMax(200),
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

  // Dijet selections
  DJ["Ana"] = Form("nljet>%.1f&&nljet<%.1f&&aljet>%.1f&&abs(nljeta)<%.1f&&abs(aljeta)<%.1f&&jdphi>%.2f",
      nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
  DJ["AnaMatRef"] = DJ["Ana"] && "nlrjet>50&&alrjet>50";
  DJ["AnaOrderRef"] = Form("nrljet>%.1f&&nrljet<%.1f&&arljet>%.1f&&abs(nrljeta)<%.1f&&abs(arljeta)<%.1f&&jdphi>%.2f",
      nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
  DJ["Ref"] = Form("nlrjet>%.1f&&nlrjet<%.1f&&alrjet>%.1f&&abs(nlrjeta)<%.1f&&abs(alrjeta)<%.1f&&rjdphi>%.2f",
      nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);
  DJ["RefOrderRef"] = Form("nrlrjet>%.1f&&nrlrjet<%.1f&&arlrjet>%.1f&&abs(nrlrjeta)<%.1f&&abs(arlrjeta)<%.1f&&rjdphi>%.2f",
      nrJetPtMin,nrJetPtMax,awJetPtMin,jetEtaMax,jetEtaMax,djDPhiMin);

  // Track Selections
  Trk["Ana"] = ("ppt>0.3 && ppt<nljet");

  CutWOVtxCut = TCut(Evt);
  Cut = TCut(Evt);

  // Analysis Tag
  AnaTag = Form("Sel%d_N%0.fto%.0f_A%.0f",selType,nrJetPtMin,nrJetPtMax,awJetPtMin);

  // Check
  Print();
}

void selectionCut::Print()
{
  cout << endl << "Ana: " << Name << "/" << AnaTag << endl;
  cout << " -- DJ cut -- " << endl;
  for (std::map<TString, TCut>::iterator 
      iter=DJ.begin(); iter != DJ.end(); ++iter) {
    std::cout << std::setw(15) << iter->first << ": " << TString(iter->second) << std::endl;
  }
  cout << " -- Trk cut -- " << std::endl;
  for (std::map<TString, TCut>::iterator 
      iter=Trk.begin(); iter != Trk.end(); ++iter) {
    std::cout << std::setw(15) << iter->first << ": " << TString(iter->second) << std::endl;
  }
}
