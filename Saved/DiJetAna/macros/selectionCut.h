#ifndef SelectionCut_H
#define SelectionCut_H

#include "TH1.h"
#include <TCut.h>
#include <TString.h>
#include "TMath.h"
#include "TTree.h"
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

class selectionCut
{  
  public:
    selectionCut(TString name, int doMC, TString base="S1", double NrJetMin=120, double NrJetMax=500, double AwJetMin=50, double jdphi=2.5);
    ~selectionCut(){}

    // memeber functions
    void And(std::map<TString,TCut> & mp, TCut cut);
    void DJAnd(TCut cut) { And(DJ,cut); }
    void EvtAnd(TCut cut) { And(Evt,cut); }
    void Print(int verbosity=2);
    void SetCut();
    // accessors
    TCut LJCut() const { return LJ.find(DJCutType)->second; }
    TCut AJCut() const { return AJ.find(DJCutType)->second; }
    TCut DJCut() const { return DJ.find(DJCutType)->second; }
    TCut TrkCut() const { return Trk.find(TrkCutType)->second; }
    TCut TrkCut(TString type) const { return Trk.find(type)->second; }
    TCut FinLJCut() const { return BaseCut && LJCut(); }
    TCut FinAJCut() const { return BaseCut && AJCut(); }
    TCut FinDJCut() const { return BaseCut && DJCut(); }
    TCut FinDJTrkCut() const { return FinDJCut() && TrkCut(); }
    // mutators
    void SetDJEt(double nrMin, double nrMax, double awMin, double awMax, double dPhiMin)
    { NrJEtMin=nrMin; NrJEtMax=nrMax; AwJEtMin=awMin; AwJEtMax=awMax; DjDPhiMin=dPhiMin; }
    // External
    TString Nr2Aw(TString s) {
      s.ReplaceAll("nlrj","alrj");
      s.ReplaceAll("nlj","alj");
      s.ReplaceAll("[0]","[1]");
      s.ReplaceAll("Nr","Aw");
      return s;
    }
    void PreviewCuts(TTree * djTree, int level=1) {
      cout << " # entries: " << djTree->GetEntries() << endl;
      cout << endl << "====== DJ Selection: " << DJCutType << " ======" << endl;
      cout << " DJ selection: " << TString(FinDJCut()) << endl;
      cout << "# DJ events passed: " << djTree->GetEntries(FinDJCut()) << endl;
      if (level>=2) {
	cout << " LJ selection: " << TString(FinLJCut()) << endl;
	cout << "# LJ events passed: " << djTree->GetEntries(FinLJCut()) << endl;
	cout << " AJ selection: " << TString(FinAJCut()) << endl;
	cout << "# AJ events passed: " << djTree->GetEntries(FinAJCut()) << endl;
      }
    }

    // ana setup
    TString Name;
    TString Tag;
    int selType;
    int doMC;
    TString BaseCutType; 
    TString DJCutType;
    TString TrkCutType;
    TString BkgSubType;
    TString Tag2;

    // Cuts
    TCut BaseCut;
    TCut RunCut;
    TCut Trigger;
    TCut CentCut;
    TCut VtxCut;
    TCut LJExtraCut;
    TCut AJExtraCut;

    // run info
    int RunNum;

    // evt
    double CentMin;
    double CentMax;
    double VzMin;
    double VzMax;

    // jet
    // -- ana ranges --
    double NrJEtMin;
    double NrJEtMax;
    double AwJEtMin;
    double AwJEtMax;
    double JEtaMin;
    double NrJEtaMax;
    double AwJEtaMax;
    double DjDPhiMin;
    // trk
    double TrkPtMin;
    // -- draw ranges --
    int	  numJEtBins;
    double hisJEtMin;
    double hisJEtMax;
    int	  numJEtaBins;
    double hisJEtaMin;
    double hisJEtaMax;
    int	  hisJPhiBins;
    double hisJPhiMin;
    double hisJPhiMax;
    int	  numC5NPBins;
    double hisC5NPMin;
    double hisC5NPMax;
    int	  numC5NPSubBins;
    double hisC5NPSubMin;
    double hisC5NPSubMax;

    // trk
    int	  numTrkPtBins;
    double hisTrkPtMin;
    double hisTrkPtMax;

    // xi
    int numXiBins;
    double hisXiMin;
    double hisXiMax;

  protected:
    std::map<TString,TCut> Evt;
    std::map<TString,TCut> LJ;
    std::map<TString,TCut> AJ;
    std::map<TString,TCut> DJ;
    std::map<TString,TCut> Trk;
};

selectionCut::selectionCut(TString name, int mc, TString base, double NrEtMin, double NrEtMax, double AwEtMin, double jdphi) :
  // setup
  Name(name),
  doMC(mc),
  BaseCutType(base),
  DJCutType("Ana"),
  TrkCutType("Ana"),
  BkgSubType("EtaRefl"),
  // default cuts
  Trigger("hlt[2]"),
  // evt
  RunNum(-1),
  CentMin(0),
  CentMax(10),
  VzMin(-20),
  VzMax(20),
  // jet
  NrJEtMin(NrEtMin),
  NrJEtMax(NrEtMax),
  AwJEtMin(AwEtMin),
  AwJEtMax(NrEtMax),
  JEtaMin(0.),
  NrJEtaMax(2.),
  AwJEtaMax(2.),
  DjDPhiMin(jdphi),
  // trk
  TrkPtMin(1.2),
  // plot
  numJEtBins(50),
  hisJEtMin(0),
  hisJEtMax(250),
  numJEtaBins(50),
  hisJEtaMin(-3.0),
  hisJEtaMax(3.0),
  hisJPhiBins(40),
  hisJPhiMin(-1*TMath::Pi()),
  hisJPhiMax(TMath::Pi()),
  numC5NPBins(100),
  hisC5NPMin(0),
  hisC5NPMax(100),
  numC5NPSubBins(150),
  hisC5NPSubMin(-50),
  hisC5NPSubMax(100),
  // trk
  numTrkPtBins(30),
  hisTrkPtMin(0),
  hisTrkPtMax(60),
  // FF
  numXiBins(10),
  hisXiMin(0),
  hisXiMax(6)
{
  TH1::SetDefaultSumw2();

  RunCut = Form("run==%d",RunNum);
  CentCut = Form("cent>=%.1f&&cent<%.1f",CentMin,CentMax);
  VtxCut = Form("vz>%.1f&&vz<%.1f",VzMin,VzMax);
}

void selectionCut::SetCut()
{
  // Event level selections
  Evt["S0"] = VtxCut;
  Evt["S1"] = CentCut&&VtxCut;
  Evt["S2"] = Trigger&&CentCut&&VtxCut;

  BaseCut = Evt[BaseCutType];

  // Single Jet Selections
  LJ["Ana"] = Form("nljet>=%.1f&&nljet<%.1f&&abs(nljeta)<%.1f",NrJEtMin,NrJEtMax,NrJEtaMax);
  AJ["Ana"] = Form("aljet>=%.1f&&aljet<%.1f&&abs(aljeta)<%.1f",AwJEtMin,AwJEtMax,AwJEtaMax);

  LJ["AnaMatRef"] = LJ["Ana"] && "nlrjet>10";
  AJ["AnaMatRef"] = AJ["Ana"] && "alrjet>10";

  LJ["Ref"] = Form("nlrjet>%.1f&&nlrjet<%.1f&&abs(nlrjeta)<%.1f",NrJEtMin,NrJEtMax,NrJEtaMax);
  AJ["Ref"] = Form("alrjet>%.1f&&alrjet<%.1f&&abs(alrjeta)<%.1f",AwJEtMin,AwJEtMax,AwJEtaMax);

  LJ["AnaOrderRef"] = Form("nrljet>%.1f&&nrljet<%.1f&&abs(nrljeta)<%.1f && nlrjet>10",NrJEtMin,NrJEtMax,NrJEtaMax);
  AJ["AnaOrderRef"] = Form("arljet>%.1f&&arljet<%.1f&&abs(arljeta)<%.1f && alrjet>10",AwJEtMin,AwJEtMax,AwJEtaMax);

  LJ["RefOrderRef"] = Form("nrlrjet>%.1f&&nrlrjet<%.1f&&abs(nrlrjeta)<%.1f",NrJEtMin,NrJEtMax,NrJEtaMax);
  AJ["RefOrderRef"] = Form("arlrjet>%.1f&&arlrjet<%.1f&&abs(arlrjeta)<%.1f",AwJEtMin,AwJEtMax,AwJEtaMax);

  LJ["AnaUpper"] = Form("nljet*1.14>%.1f&&nljet*1.14<%.1f&&abs(nljeta)<%.1f",NrJEtMin,NrJEtMax,NrJEtaMax);
  AJ["AnaUpper"] = Form("aljet*1.14>%.1f&&aljet*1.14<%.1f&&abs(aljeta)<%.1f",AwJEtMin,AwJEtMax,AwJEtaMax);

  LJ["AnaLower"] = Form("nljet*0.86>%.1f&&nljet*0.86<%.1f&&abs(nljeta)<%.1f",NrJEtMin,NrJEtMax,NrJEtaMax);
  AJ["AnaLower"] = Form("aljet*0.86>%.1f&&aljet*0.86<%.1f&&abs(aljeta)<%.1f",AwJEtMin,AwJEtMax,AwJEtaMax);

  if (!TString(LJExtraCut).IsWhitespace()) And(LJ,LJExtraCut);
  if (!TString(AJExtraCut).IsWhitespace()) And(AJ,AJExtraCut);

  // Dijet selections
  TCut JDPhiCut(Form("jdphi>%.2f",DjDPhiMin));
  TCut RefJDPhiCut(Form("rjdphi>%.2f",DjDPhiMin));
  DJ["Ana"]	      = LJ["Ana"]	  && AJ["Ana"]		&& JDPhiCut;
  DJ["AnaMatRefNr"]   = LJ["AnaMatRef"]	  && AJ["Ana"]		&& JDPhiCut;
  DJ["AnaMatRefAw"]   = LJ["Ana"]	  && AJ["AnaMatRef"]	&& JDPhiCut;
  DJ["AnaMatRef"]     = LJ["AnaMatRef"]	  && AJ["AnaMatRef"]	&& JDPhiCut;
  DJ["RefAwAnaMatNr"] = LJ["AnaMatRef"]	  && AJ["Ref"]		&& JDPhiCut;
  DJ["RefNrAnaMatAw"] = LJ["Ref"]	  && AJ["AnaMatRef"]	&& JDPhiCut;
  DJ["Ref"]	      = LJ["Ref"]	  && AJ["Ref"]		&& RefJDPhiCut;
  // reorder lead vs aw
  DJ["AnaOrderRef"]   = LJ["AnaOrderRef"] && AJ["AnaOrderRef"]	&& JDPhiCut;
  DJ["RefOrderRef"]   = LJ["RefOrderRef"] && AJ["RefOrderRef"]	&& RefJDPhiCut;
  // - sysetmatics -
  DJ["AnaUpper"]      = LJ["AnaUpper"]	  && AJ["AnaUpper"]	&& JDPhiCut;
  DJ["AnaLower"]      = LJ["AnaLower"]	  && AJ["AnaLower"]	&& JDPhiCut;
  // FF aware
  DJ["AnaLz0"]	      = LJ["Ana"]	  && AJ["Ana"]		&& JDPhiCut && "(lppt[0]/nljet)>0.0&&(lppt[0]/nljet)<0.3";
  DJ["AnaLz1"]	      = LJ["Ana"]	  && AJ["Ana"]		&& JDPhiCut && "(lppt[0]/nljet)>0.3&&(lppt[0]/nljet)<0.6";
  DJ["AnaLz2"]	      = LJ["Ana"]	  && AJ["Ana"]		&& JDPhiCut && "(lppt[0]/nljet)>0.6&&(lppt[0]/nljet)<0.9";

  // Track Selections
  Trk["Ana"] = Form("ppt>%.1f&&ppt<1.5*nljet",TrkPtMin);
  Trk["Tight3"] = ("ppt>3.&&ppt<1.5*nljet");
  Trk["Tight5"] = ("ppt>5.&&ppt<1.5*nljet");
  Trk["TightNH"] = ("ppt>1.5&&ppt<nljet&&trkNHits>14");
  Trk["AnaSig"] = Trk["Ana"]&&"psube==0";

  Tag = Form("%s_%0.f_%.0f_%.0f_%.0f",BaseCutType.Data(),NrJEtMin,NrJEtMax,AwJEtMin,DjDPhiMin*10);
  if (TMath::Abs(AwJEtMax-NrJEtMax)>0.1)
    Tag = Form("%s_%0.f_%.0f_%.0f_%.0f_%.0f",BaseCutType.Data(),NrJEtMin,NrJEtMax,AwJEtMin,AwJEtMax,DjDPhiMin*10);
  Tag2 = "_J"+DJCutType+"T"+TrkCutType;
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
  std::cout << std::endl << "Ana: " << Name << "/" << Tag << "  Tag2: " << Tag2 << std::endl;
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

#endif
