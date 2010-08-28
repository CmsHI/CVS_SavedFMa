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

void anaDiJetTrk(int doMC=0,
    const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/JEx_Hard_GSR_ZS/mc0824_hlt/dijetAna_*.root",
    TString SrcName = "JExHdMc",
    TString AnaVersion = "a1",
    TString AnaType = "djtrk",
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
  selectionCut anaSel(SrcName,doMC,"S0",100,200,50,2.5);
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

  // ============== pdf comparisons ===============
  cout << endl << "======= anaDiJetTrk ========" << endl;

  Double_t histTrkNHitsMax=30;
  // === Inclusive ===
  AnaFrag TrkPt("Ana","TrkPt",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),
      "ppt","","",anaSel.numTrkPtBins,0,anaSel.hisTrkPtMax);
  AnaFrag TrkNHits("Ana","TrkNHits",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),
      "trkNHits","","",histTrkNHitsMax,0,histTrkNHitsMax);

  // === In Jet Eta Strip ===
  AnaFrag JTrkDPhi("Ana","JTrkDPhi",djTree,anaSel.FinDJCut(),anaSel.TrkCut()&&"abs(peta-nljeta)<0.5||abs(peta-aljeta)<0.5",
      "pndphi","","",20,-1*TMath::Pi(),TMath::Pi());
  AnaFrag JTrkTight3DPhi("Ana","JTrkTight3DPhi",djTree,anaSel.FinDJCut(),anaSel.TrkCut("Tight3")&&"abs(peta-nljeta)<0.5||abs(peta-aljeta)<0.5",
      "pndphi","","",20,-1*TMath::Pi(),TMath::Pi());

  // === In Jet Cone ===
  AnaFrag JCTrkPt("Ana","JCTrkPt",djTree,anaSel.FinDJCut(),anaSel.TrkCut()&&"pndr<0.5||padr<0.5",
      "ppt","","",anaSel.numTrkPtBins,0,anaSel.hisTrkPtMax);
  //  - bg subtracted -
  AnaFrag NrTrkPt("Ana","NrTrkPt",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),
      "ppt","pndr<0.5","pndrbg<0.5",anaSel.numTrkPtBins,0,anaSel.hisTrkPtMax);
  AnaFrag AwTrkPt("Ana","AwTrkPt",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),
      "ppt","padr<0.5","padrbg<0.5",anaSel.numTrkPtBins,0,anaSel.hisTrkPtMax);

  // All done, save and exit
  outf->Write();
}
