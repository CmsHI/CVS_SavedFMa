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
    selectionCut(TString name, int doMC, TString base="S0", float NrJetMin=100, float NrJetMax=170, float AwJetMin=50, float jdphi=2.5);
    ~selectionCut(){}

    // memeber functions
    void And(std::map<TString,TCut> & mp, TCut cut);
    void DJAnd(TCut cut) { And(DJ,cut); }
    void EvtAnd(TCut cut) { And(Evt,cut); }
    void Print(int verbosity=2);
    void SetCut();
    // accessors
    TCut DJCut() { return DJ[DJCutType]; }
    TCut TrkCut() { return Trk[TrkCutType]; }
    TCut FinDJCut() { return BaseCut && DJCut(); }
    TCut FinDJTrkCut() { return FinDJCut() && TrkCut(); }

    // ana setup
    TString Name;
    TString Tag;
    int selType;
    int doMC;
    TString BaseCutType; 
    TString DJCutType;
    TString TrkCutType;

    // Cuts
    TCut BaseCut;
    TCut RunCut;
    TCut Trigger;
    TCut CentCut;
    TCut VtxCut;

    // run info
    int RunNum;

    // evt
    float VzMin;
    float VzMax;

    // jet
    // -- ana ranges --
    float NrJEtMin;
    float NrJEtMax;
    float AwJEtMin;
    float JEtaMin;
    float JEtaMax;
    float DjDPhiMin;
    // -- draw ranges --
    int	  numJEtBins;
    float hisJEtMin;
    float hisJEtMax;
    int	  numJEtaBins;
    float hisJEtaMin;
    float hisJEtaMax;
    int	  hisJPhiBins;
    float hisJPhiMin;
    float hisJPhiMax;

    // trk
    int	  numTrkPtBins;
    float hisTrkPtMin;
    float hisTrkPtMax;

    // xi
    int numXiBins;
    float hisXiMin;
    float hisXiMax;

  protected:
    std::map<TString,TCut> Evt;
    std::map<TString,TCut> DJ;
    std::map<TString,TCut> Trk;
};

selectionCut::selectionCut(TString name, int mc, TString base, float NrEtMin, float NrEtMax, float AwEtMin, float jdphi) :
  // setup
  Name(name),
  doMC(mc),
  BaseCutType(base),
  DJCutType("Ana"),
  TrkCutType("Ana"),
  // default cuts
  Trigger("hlt[0]"),
  CentCut("cent<20"),
  // evt
  RunNum(-1),
  VzMin(-30),
  VzMax(30),
  // jet
  NrJEtMin(NrEtMin),
  NrJEtMax(NrEtMax),
  AwJEtMin(AwEtMin),
  JEtaMin(0.),
  JEtaMax(2.),
  DjDPhiMin(jdphi),
  // plot
  numJEtBins(50),
  hisJEtMin(0),
  hisJEtMax(200),
  numJEtaBins(40),
  hisJEtaMin(-3.0),
  hisJEtaMax(3.0),
  hisJPhiBins(40),
  hisJPhiMin(-1*TMath::Pi()),
  hisJPhiMax(TMath::Pi()),
  numXiBins(10),
  hisXiMin(0),
  hisXiMax(6)
{
  TH1::SetDefaultSumw2();

  RunCut = Form("run==%d",RunNum);
  VtxCut = Form("vz>%.1f&&vz<%.1f",VzMin,VzMax);
}

void selectionCut::SetCut()
{
  // Event level selections
  Evt["S0"] = Trigger&&CentCut&&VtxCut;

  BaseCut = Evt[BaseCutType];

  // Dijet selections
  DJ["Ana"] = Form("nljet>%.1f&&nljet<%.1f&&aljet>%.1f&&abs(nljeta)<%.1f&&abs(aljeta)<%.1f&&jdphi>%.2f",
      NrJEtMin,NrJEtMax,AwJEtMin,JEtaMax,JEtaMax,DjDPhiMin);
  DJ["AnaMatRef"] = DJ["Ana"] && "nlrjet>20&&alrjet>20";
  DJ["AnaOrderRef"] = TCut(Form("nrljet>%.1f&&nrljet<%.1f&&arljet>%.1f&&abs(nrljeta)<%.1f&&abs(arljeta)<%.1f&&jdphi>%.2f",
      NrJEtMin,NrJEtMax,AwJEtMin,JEtaMax,JEtaMax,DjDPhiMin)) && "nlrjet>20&&alrjet>20";
  DJ["Ref"] = Form("nlrjet>%.1f&&nlrjet<%.1f&&alrjet>%.1f&&abs(nlrjeta)<%.1f&&abs(alrjeta)<%.1f&&rjdphi>%.2f",
      NrJEtMin,NrJEtMax,AwJEtMin,JEtaMax,JEtaMax,DjDPhiMin);
  DJ["RefOrderRef"] = Form("nrlrjet>%.1f&&nrlrjet<%.1f&&arlrjet>%.1f&&abs(nrlrjeta)<%.1f&&abs(arlrjeta)<%.1f&&rjdphi>%.2f",
      NrJEtMin,NrJEtMax,AwJEtMin,JEtaMax,JEtaMax,DjDPhiMin);
  // - sysetmatics -
  DJ["AnaUpper"] = Form("nljet*1.14>%.1f&&nljet*1.14<%.1f&&aljet*1.14>%.1f&&abs(nljeta)<%.1f&&abs(aljeta)<%.1f&&jdphi>%.2f",
      NrJEtMin,NrJEtMax,AwJEtMin,JEtaMax,JEtaMax,DjDPhiMin);
  DJ["AnaLower"] = Form("nljet*0.86>%.1f&&nljet*0.86<%.1f&&aljet*0.86>%.1f&&abs(nljeta)<%.1f&&abs(aljeta)<%.1f&&jdphi>%.2f",
      NrJEtMin,NrJEtMax,AwJEtMin,JEtaMax,JEtaMax,DjDPhiMin);

  // Track Selections
  Trk["Ana"] = ("ppt>1.2&&ppt<nljet");
  Trk["Tight3"] = ("ppt>3.&&ppt<nljet");
  Trk["Tight5"] = ("ppt>5.&&ppt<nljet");
  Trk["TightNH"] = ("ppt>1.5&&ppt<nljet&&trkNHits>14");
  Trk["AnaSig"] = Trk["Ana"]&&"psube==0";

  Tag = Form("%s_%0.f_%.0f_%.0f_%.0f",BaseCutType.Data(),NrJEtMin,NrJEtMax,AwJEtMin,DjDPhiMin*10);
  Print(1);
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

void selectionCut::Print(int verbosity)
{
  std::cout << std::endl << "Ana: " << Name << "/" << Tag << std::endl;
  std::cout << std::setw(15) << "Evt" << ": " << TString(BaseCut) << std::endl;
  if (verbosity==1) {
    std::cout << std::setw(15) << DJCutType << ": " << TString(DJCut()) << std::endl;
    std::cout << std::setw(15) << TrkCutType << ": " << TString(TrkCut()) << std::endl;
  }
  if (verbosity==2) {
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
}
