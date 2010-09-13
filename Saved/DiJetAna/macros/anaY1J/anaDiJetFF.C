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
    TString AnaVersion = "a7j2t0",
    TString modName = "dijetAna_mc_calojet_genp",
    Double_t NrJEtMin = 100,
    Double_t NrJEtMax = 200,
    Double_t AwJEtMin = 50,
    Double_t AwJEtaMax = 2.,
    TString DJCutType = "Ana",
    TString TrkCutType = "Ana",
    TString varXiNrJES = "log(1./zn)",
    TString varXiAwJES = "log(1./za)",
    const char * inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root",
    TString SrcName = "HydUQDJ80",
    TString header = "Hydjet+UQDJ80",
    TString AnaType = "ff")
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
  selectionCut anaSel(SrcName,doMC,"S1",NrJEtMin,NrJEtMax,AwJEtMin,2.5);
  anaSel.AwJEtaMax = AwJEtaMax;
  anaSel.DJCutType = DJCutType;
  anaSel.TrkCutType = TrkCutType;
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
  AnaFrag NrXi("Xi","Nr",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),varXiNrJES,"pndr<0.5","pndrbg<0.5",anaSel.numXiBins,anaSel.hisXiMin,anaSel.hisXiMax,&anaSel);
  AnaFrag AwXi("Xi","Aw",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),varXiAwJES,"padr<0.5","padrbg<0.5",anaSel.numXiBins,anaSel.hisXiMin,anaSel.hisXiMax,&anaSel);
  HisTGroup<TH1D> hgXi("Xi");
  hgXi.Add(NrXi.hSig,"Nr");
  hgXi.Add(AwXi.hSig,"Aw");
  hgXi.Average();

  AnaFrag AwXiE1("XiE1","Aw",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),varXiNrJES,"padr<0.5","padrbg<0.5",anaSel.numXiBins,anaSel.hisXiMin,anaSel.hisXiMax,&anaSel);
  HisTGroup<TH1D> hgXiE1("XiE1");
  hgXiE1.Add(NrXi.hSig,"Nr");
  hgXiE1.Add(AwXiE1.hSig,"Aw");
  hgXiE1.Average();

  AnaFrag NrPPt("PPt","Nr",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"ppt","pndr<0.5","pndrbg<0.5",anaSel.numTrkPtBins,anaSel.hisTrkPtMin,anaSel.hisTrkPtMax,&anaSel);
  AnaFrag AwPPt("PPt","Aw",djTree,anaSel.FinDJCut(),anaSel.TrkCut(),"ppt","padr<0.5","padrbg<0.5",anaSel.numTrkPtBins,anaSel.hisTrkPtMin,anaSel.hisTrkPtMax,&anaSel);
  HisTGroup<TH1D> hgPPt("PPt");
  hgPPt.Add(NrPPt.hSig,"Nr");
  hgPPt.Add(AwPPt.hSig,"Aw");
  hgPPt.Average();

  // All done, save and exit
  outf->Write();
}
