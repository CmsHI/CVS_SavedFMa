#include "TH1.h"
#include <TCut.h>
#include <TString.h>
#include "TMath.h"
#include <iostream>
#include <iomanip>
#include <map>

class selectionCut
{  
  public:
    selectionCut(TString name, bool isMC, int sel, float NrJetMin=80, float NrJetMax=120, float AwJetMin=70, float jdphi=2.5, int runnum=-1, int nLumiL=0, int nLumiH=10000);
    ~selectionCut(){}

    // memeber functions
    void And(std::map<TString,TCut> & mp, TCut cut);
    void DJAnd(TCut cut) { And(DJ,cut); }
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
    // -- ana ranges --
    float nrJEtMin;
    float nrJEtMax;
    float awJEtMin;
    float jetEtaMin;
    float jetEtaMax;
    float djDPhiMin;
    // -- draw ranges --
    int	  hisJEtBins;
    float hisJEtMin;
    float hisJEtMax;
    int	  hisJEtaBins;
    float hisJEtaMin;
    float hisJEtaMax;
    int	  hisJPhiBins;
    float hisJPhiMin;
    float hisJPhiMax;

    // trk
    int numTrkPtBin;
    float trkPtMin;
    float trkPtMax;
};

selectionCut::selectionCut(TString name, bool isMC, int sel, float NrJetMin, float NrJetMax, float AwJetMin, float jdphi, int runnum, int nLumiL, int nLumiH) :
  Name(name),
  selType(sel),
  runNum(runnum),
  nrJEtMin(NrJetMin),
  nrJEtMax(NrJetMax),
  awJEtMin(AwJetMin),
  jetEtaMin(0.),
  jetEtaMax(2.),
  djDPhiMin(jdphi),
  hisJEtBins(50),
  hisJEtMin(0),
  hisJEtMax(200),
  hisJEtaBins(40),
  hisJEtaMin(-3.0),
  hisJEtaMax(3.0),
  hisJPhiBins(40),
  hisJPhiMin(-1*TMath::Pi()),
  hisJPhiMax(TMath::Pi())
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
      nrJEtMin,nrJEtMax,awJEtMin,jetEtaMax,jetEtaMax,djDPhiMin);
  DJ["AnaMatRef"] = DJ["Ana"] && "nlrjet>20&&alrjet>20";
  DJ["AnaOrderRef"] = TCut(Form("nrljet>%.1f&&nrljet<%.1f&&arljet>%.1f&&abs(nrljeta)<%.1f&&abs(arljeta)<%.1f&&jdphi>%.2f",
      nrJEtMin,nrJEtMax,awJEtMin,jetEtaMax,jetEtaMax,djDPhiMin)) && "nlrjet>20&&alrjet>20";
  DJ["Ref"] = Form("nlrjet>%.1f&&nlrjet<%.1f&&alrjet>%.1f&&abs(nlrjeta)<%.1f&&abs(alrjeta)<%.1f&&rjdphi>%.2f",
      nrJEtMin,nrJEtMax,awJEtMin,jetEtaMax,jetEtaMax,djDPhiMin);
  DJ["RefOrderRef"] = Form("nrlrjet>%.1f&&nrlrjet<%.1f&&arlrjet>%.1f&&abs(nrlrjeta)<%.1f&&abs(arlrjeta)<%.1f&&rjdphi>%.2f",
      nrJEtMin,nrJEtMax,awJEtMin,jetEtaMax,jetEtaMax,djDPhiMin);

  // Track Selections
  Trk["Ana"] = ("ppt>1.&&ppt<nljet");
  Trk["TightNH"] = ("ppt>1.5&&ppt<nljet&&trkNHits>14");
  Trk["Tight5"] = ("ppt>5.&&ppt<nljet");
  Trk["Tight7"] = ("ppt>7.&&ppt<nljet");
  Trk["AnaSig"] = Trk["Ana"]&&"psube==0";

  CutWOVtxCut = TCut(Evt);
  Cut = TCut(Evt);

  // Analysis Tag
  AnaTag = Form("Sel%d_N%0.fto%.0f_A%.0f",selType,nrJEtMin,nrJEtMax,awJEtMin);
}

void selectionCut::And(std::map<TString,TCut> & mp, TCut cut)
{
  //cout << "add cut: " << TString(cut) << " to: " << endl;
  for (std::map<TString, TCut>::iterator 
      iter=mp.begin(); iter != mp.end(); ++iter) {
    //std::cout << std::setw(15) << iter->first << ": " << TString(iter->second) << std::endl;
    iter->second = iter->second && cut;
  }
}

void selectionCut::Print()
{
  std::cout << std::endl << "Ana: " << Name << "/" << AnaTag << std::endl;
  std::cout << " -- DJ cut -- " << std::endl;
  for (std::map<TString, TCut>::iterator 
      iter=DJ.begin(); iter != DJ.end(); ++iter) {
    std::cout << std::setw(15) << iter->first << ": " << TString(iter->second) << std::endl;
  }
  std::cout << " -- Trk cut -- " << std::endl;
  for (std::map<TString, TCut>::iterator 
      iter=Trk.begin(); iter != Trk.end(); ++iter) {
    std::cout << std::setw(15) << iter->first << ": " << TString(iter->second) << std::endl;
  }
}
