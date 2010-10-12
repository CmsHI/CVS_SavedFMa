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

void anaJesFF(int doMC=1,
    TString AnaVersion = "a1012",
    TString modName = "djcalo_genp",
    TString outdir="out/s10Qcd80/compare/a1012c1",
    const char * inFile0Name="~/scratch01/ana/s10-dj80to120/dj1009_Y1JAna011/dj_*.root",
    TString SrcName = "s10Qcd80",
    TString header = "QCD-DiJet80to120",
    TString AnaType = "dj")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  djTree->Add(inFile0Name);
  aliases_dijet(djTree,doMC);
  cout << " # entries: " << djTree->GetEntries() << endl;

  // === Declare selection ===
  selectionCut anaSel(SrcName,doMC,"S1",50,200,30,2.5);
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

  // ============== Dijet Scales ===============
  TH2F * hLPPtVsJEt = new TH2F("hLPPtVsJEt","hLPPtVsJEt",50,0,250,50,0,250);
  TH2F * hLzVsJEt = new TH2F("hLzVsJEt","hLzVsJEt",50,0,250,50,-1,2);
  djTree->Draw("lppt[0]:nlrjet>>hLPPtVsJEt",anaSel.FinDJCut(),"goff");
  djTree->Draw("lppt[0]/nljet:nlrjet>>hLzVsJEt",anaSel.FinDJCut(),"prof");

  // -- plot --
  TCanvas * cLzVsJEt = new TCanvas("cLzVsJEt","cLzVsJEt",500,500);
  cLzVsJEt->SetLogz();
  CPlot cpLzVsJEt("LzVsJEt"+anaSel.Tag2,"DJ","Leading E_{T}^{genjet} [GeV]","Leading p_{T}^{trk} in Jet Cone [GeV]");
  cpLzVsJEt.SetYRange(0,160);
  cpLzVsJEt.AddHist2D(hLPPtVsJEt,"colz");
  cpLzVsJEt.Draw(cLzVsJEt,true);
}
