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

void anaDiJetFF(int doMC=0,
    const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/JEx_Hard_GSR_ZS/mc0827_hltall/dijetAna_*.root",
    TString SrcName = "JExHdMcV2",
    TString AnaVersion = "a5",
    TString AnaType = "ff",
    TString header = "July Data (Hard Triggered)",
    TString modName = "dijetAna_mc")
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
  selectionCut anaSel(SrcName,doMC,"S1",100,200,50,2.5);
  anaSel.DJCutType = "Ana";
  anaSel.TrkCutType = "Ana";
  anaSel.SetCut();

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << anaSel.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(anaSel.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(anaSel.FinDJCut()) << endl;

  // === Define Output ===
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("out/%s/%s/%s",SrcName.Data(),anaSel.Tag.Data(),AnaVersion.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  // Save output
  TFile * outf = new TFile(Form("%s/%s.root",anaoutdir.Data(),AnaType.Data()),"RECREATE");

  // ============== ana ===============
  cout << endl << "======= anaDiJetFF ========" << endl;
  AnaFrag NrXi("Xi","Nr",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"log(1./zn)","pndr<0.5","pndrbg<0.5",anaSel.numXiBins,anaSel.hisXiMin,anaSel.hisXiMax);
  AnaFrag AwXi("Xi","Aw",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"log(1./za)","padr<0.5","padrbg<0.5",anaSel.numXiBins,anaSel.hisXiMin,anaSel.hisXiMax);
  HisTGroup<TH1D> hgXi("Xi");
  hgXi.Add(NrXi.hSig,"Nr");
  hgXi.Add(AwXi.hSig,"Aw");
  hgXi.Average();

  AnaFrag AwXi2("Xi2","Aw",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"log(1./zn)","padr<0.5","padrbg<0.5",anaSel.numXiBins,anaSel.hisXiMin,anaSel.hisXiMax);
  HisTGroup<TH1D> hgXi2("Xi2");
  hgXi2.Add(NrXi.hSig,"Nr");
  hgXi2.Add(AwXi2.hSig,"Aw");
  hgXi2.Average();

  AnaFrag NrTrk("Trk","Nr",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"ppt","pndr<0.5","pndrbg<0.5",anaSel.numTrkPtBins,anaSel.hisTrkPtMin,anaSel.hisTrkPtMax);
  AnaFrag AwTrk("Trk","Aw",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"ppt","padr<0.5","padrbg<0.5",anaSel.numTrkPtBins,anaSel.hisTrkPtMin,anaSel.hisTrkPtMax);
  HisTGroup<TH1D> hgTrk("Trk");
  hgTrk.Add(NrTrk.hSig,"Nr");
  hgTrk.Add(AwTrk.hSig,"Aw");
  hgTrk.Average();

  // All done, save and exit
  outf->Write();
}
