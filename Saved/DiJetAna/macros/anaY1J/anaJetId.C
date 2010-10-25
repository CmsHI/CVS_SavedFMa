#include <iostream>
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TMath.h"

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/selectionCut.h"
#include "Saved/DiJetAna/macros/AnaFrag.h"
#include "Saved/Utilities/macros/cplot/CPlot.h"           // helper class for plots
#include "Saved/Utilities/macros/histogram/HisTGroup.h"

using namespace std;

void anaJetId(int doMC=1,
    TString SrcName = "su10Qcd80",
    TString AnaVersion = "a1025",
    TString CmpVersion = "c0",
    TString modName = "djcalo_genp",
    const char * inFile0Name="~/scratch01/mc/QCD/su10-qcd80-startup36v9_f500_proc1022_final/trkhists_*.root",
    TString header = "QCD-DiJet80",
    TString AnaType = "jid")
{
  // Define Inputs
  TString outdir="out/"+SrcName+"/compare/"+AnaVersion+CmpVersion;
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  djTree->Add(inFile0Name);
  aliases_dijet(djTree,doMC);
  cout << " # entries: " << djTree->GetEntries() << endl;

  // === Declare selection ===
  selectionCut anaSel(SrcName,doMC,"S1",100,2000,40,2.5);
  anaSel.DJCutType = "Ana";
  anaSel.TrkCutType = "Ana";
  anaSel.Tag2+=TString("_"+modName);
  anaSel.SetCut();


  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << anaSel.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(anaSel.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(anaSel.FinDJCut()) << endl;

  // === Define Output ===
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);

  // === ana ===
  HisTGroup<TH1D> hgJEmf("JEmf",400,-0.51,1.51);
  hgJEmf.Add1D("LUncut");
  hgJEmf.Add1D("LDJ");
  hgJEmf.Add1D("LJId");
  HisTGroup<TH1D> hgJN90hits("JN90hits",500,0,500);
  hgJN90hits.Add1D("LUncut");
  hgJN90hits.Add1D("LDJ");
  hgJN90hits.Add1D("LJId");
  HisTGroup<TH1D> hgJFHpd("JFHpd",100,-2,2.05);
  hgJFHpd.Add1D("LUncut");
  hgJFHpd.Add1D("LDJ");
  hgJFHpd.Add1D("LJId");
  HisTGroup<TH1D> hgJEt("JEt",150,0,1500);
  hgJEt.Add1D("LUncut");
  hgJEt.Add1D("LDJ");
  hgJEt.Add1D("LJId");

  // Ana
  djTree->Draw("nljemf>>hJEmfLUncut",anaSel.BaseCut && "nljet>100","goff");
  djTree->Draw("nljemf>>hJEmfLDJ",anaSel.BaseCut && "nljet>100&&aljet>30","goff");
  djTree->Draw("nljemf>>hJEmfLJId",anaSel.BaseCut && "nljet>100&&nljemf>0.01&&nljN90hits>1&&nljfhpd<0.98","goff");

  djTree->Draw("nljN90hits>>hJN90hitsLUncut",anaSel.BaseCut && "nljet>100","goff");
  djTree->Draw("nljN90hits>>hJN90hitsLDJ",anaSel.BaseCut && "nljet>100&&aljet>30","goff");
  djTree->Draw("nljN90hits>>hJN90hitsLJId",anaSel.BaseCut && "nljet>100&&nljemf>0.01&&nljN90hits>1&&nljfhpd<0.98","goff");

  djTree->Draw("nljfhpd>>hJFHpdLUncut",anaSel.BaseCut && "nljet>100","goff");
  djTree->Draw("nljfhpd>>hJFHpdLDJ",anaSel.BaseCut && "nljet>100&&aljet>30","goff");
  djTree->Draw("nljfhpd>>hJFHpdLJId",anaSel.BaseCut && "nljet>100&&nljemf>0.01&&nljN90hits>1&&nljfhpd<0.98","goff");

  cout << endl << "leading et cut: " <<
    djTree->Draw("nljet>>hJEtLUncut",anaSel.BaseCut && "nljet>100","goff") << endl;
  cout << endl << "leading et from DJ cut: " <<
  djTree->Draw("nljet>>hJEtLDJ",anaSel.BaseCut && "nljet>100&&aljet>30","goff") << endl;
  cout << endl << "leading et cut w/ Jet Id: " <<
  djTree->Draw("nljet>>hJEtLJId",anaSel.BaseCut && "nljet>100&&nljemf>0.01&&nljN90hits>1&&nljfhpd<0.98","goff") << endl;

  // -- plot --
  TCanvas * cLJEmf = new TCanvas("LJEmf","LJEmf",500,500);
  CPlot cpLJEmf("LJEmf"+anaSel.Tag2,"LJEmf","Leading Jet EMF","#");
  cpLJEmf.AddHist1D(hgJEmf.H("LUncut"),"Uncut","hist");
  cpLJEmf.AddHist1D(hgJEmf.H("LJId"),"Jet ID Cut","E",kRed,kOpenStar);
  cpLJEmf.AddHist1D(hgJEmf.H("LDJ"),"In DiJet (j2>30GeV)","E",kBlue,kOpenSquare);
  cpLJEmf.SetLegend(0.26,0.77,0.59,0.89);
  cpLJEmf.Draw(cLJEmf,true);

  TCanvas * cLJN90hits = new TCanvas("LJN90hits","LJN90hits",500,500);
  CPlot cpLJN90hits("LJN90hits"+anaSel.Tag2,"LJN90hits","Leading Jet N90Hits","#");
  cpLJN90hits.SetLogx();
  cpLJN90hits.SetLogy();
  cpLJN90hits.AddHist1D(hgJN90hits.H("LUncut"),"Uncut","hist");
  cpLJN90hits.AddHist1D(hgJN90hits.H("LJId"),"Jet ID Cut","E",kRed,kOpenStar);
  cpLJN90hits.AddHist1D(hgJN90hits.H("LDJ"),"In DiJet (j2>30GeV)","E",kBlue,kOpenSquare);
  cpLJN90hits.SetLegend(0.52,0.21,0.85,0.33);
  cpLJN90hits.Draw(cLJN90hits,true);

  TCanvas * cLJFHpd = new TCanvas("LJFHpd","LJFHpd",500,500);
  CPlot cpLJFHpd("LJFHpd"+anaSel.Tag2,"LJFHpd","Leading Jet fHPD","#");
  cpLJFHpd.SetLogy();
  cpLJFHpd.AddHist1D(hgJFHpd.H("LUncut"),"Uncut","hist");
  cpLJFHpd.AddHist1D(hgJFHpd.H("LJId"),"Jet ID Cut","E",kRed,kOpenStar);
  cpLJFHpd.AddHist1D(hgJFHpd.H("LDJ"),"In DiJet (j2>30GeV)","E",kBlue,kOpenSquare);
  cpLJFHpd.SetLegend(0.21,0.79,0.54,0.91);
  cpLJFHpd.Draw(cLJFHpd,true);

  TCanvas * cLJEt = new TCanvas("LJEt","LJEt",500,500);
  CPlot cpLJEt("LJEt"+anaSel.Tag2,"LJEt","Leading E_{T}^{jet} [GeV]","# / 10GeV");
  cpLJEt.SetLogy();
  cpLJEt.AddHist1D(hgJEt.H("LUncut"),"Uncut","hist");
  cpLJEt.AddHist1D(hgJEt.H("LJId"),"Jet ID Cut","E",kRed,kOpenStar);
  cpLJEt.AddHist1D(hgJEt.H("LDJ"),"In DiJet (j2>30GeV)","E",kBlue,kOpenSquare);
  cpLJEt.Draw(cLJEt,true);
}
