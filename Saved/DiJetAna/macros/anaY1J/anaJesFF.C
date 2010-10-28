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
    TString SrcName = "su10Qcd80",
    TString AnaVersion = "a1028",
    TString CmpVersion = "c0",
    TString modName = "djcalo_genp",
    Double_t NrJEtMin = 50,
    Double_t NrJEtMax = 200,
    TString DJCutType = "Ref",
    TString fragVar = "lppt[0]/nlrjet",
    TString fragVarTag = "Lz",
    TString fragVarTitle = "z^{lead} = p_{T}^{trk}/E_{T}^{RefJet}",
    Double_t fragVarMin = 0,
    Double_t fragVarMax = 1.5,
    //TString fragVar = "nlrjet",
    //TString fragVarTag = "RefJEt",
    //TString fragVarTitle = "E_{T}^{RefJet}",
    //Double_t fragVarMin = 0,
    //Double_t fragVarMax = 250,
    TString jextraCut = "",//"(ljcpt[0]-ljcptbg[0])/nlrjet>0.5",
    const char * inFile0Name="~/scratch01/mc/QCD/su10-qcd80-startup36v9_f500_proc1022_final/trkhists_*.root",
    TString header = "QCD-DiJet80",
    TString AnaType = "dj")
{
  // Define Inputs
  TString outdir="out/"+SrcName+"/compare/"+AnaVersion+CmpVersion;
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  cout << "Analyze: " << modName << endl;
  TChain * djTree = new TChain(modName+"/djTree","dijet Tree");
  djTree->Add(inFile0Name);
  aliases_dijet(djTree,doMC);

  // === Declare selection ===
  selectionCut anaSel(SrcName,doMC,"S1",NrJEtMin,NrJEtMax,NrJEtMin,2.5);
  anaSel.DJCutType = DJCutType;
  anaSel.TrkCutType = "Ana";
  anaSel.LJExtraCut = jextraCut;
  anaSel.AJExtraCut = anaSel.Nr2Aw(jextraCut);
  anaSel.Tag2+=TString("_"+modName);
  anaSel.SetCut();
  anaSel.Print(1);
  anaSel.PreviewCuts(djTree,2);

  // === Define Output ===
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);

  // === ana ===
  // frag var
  Int_t numFragVarBins=40;
  AnaFrag anaFragVarNr(fragVarTag,"Nr",djTree,anaSel.FinLJCut(),"",fragVar,"","",numFragVarBins,fragVarMin,fragVarMax);
  AnaFrag anaFragVarAw(fragVarTag,"Aw",djTree,anaSel.FinAJCut(),"",anaSel.Nr2Aw(fragVar),"","",numFragVarBins,fragVarMin,fragVarMax);

  cout << "=== Correlation Analysis ===" << endl;
  // correlations
  AnaFrag anaFragVar_AnaJEt(fragVarTag,"JEt",djTree,numFragVarBins,fragVarMin,fragVarMax);
  anaFragVar_AnaJEt.xtitle = "E_{t}^{Jet}";
  anaFragVar_AnaJEt.ytitle = fragVarTitle;
  anaFragVar_AnaJEt.PlotCorrelations(anaSel,fragVar,"nljet",anaSel.numJEtBins,anaSel.hisJEtMin,anaSel.hisJEtMax);

  AnaFrag anaFragVar_RefJEt(fragVarTag,"RefJEt",djTree,numFragVarBins,fragVarMin,fragVarMax);
  anaFragVar_RefJEt.xtitle = "E_{t}^{RefJet}";
  anaFragVar_RefJEt.ytitle = fragVarTitle;
  anaFragVar_RefJEt.PlotCorrelations(anaSel,fragVar,"nlrjet",anaSel.numJEtBins,anaSel.hisJEtMin,anaSel.hisJEtMax);

  AnaFrag anaFragVar_JCPtDivJEt(fragVarTag,"JCPtDivJEt",djTree,numFragVarBins,fragVarMin,fragVarMax);
  anaFragVar_JCPtDivJEt.xtitle = "#Sigma_{jet cone} p_{T}^{Trk}/E_{t}^{Jet}";
  anaFragVar_JCPtDivJEt.ytitle = fragVarTitle;
  anaFragVar_JCPtDivJEt.PlotCorrelations(anaSel,fragVar,"(ljcpt[0]-ljcptbg[0])/nljet",numFragVarBins,0,2);

  AnaFrag anaFragVar_JCPtDivRefJEt(fragVarTag,"JCPtDivRefJEt",djTree,numFragVarBins,fragVarMin,fragVarMax);
  anaFragVar_JCPtDivRefJEt.xtitle = "#Sigma_{jet cone} p_{T}^{Trk}/E_{t}^{RefJet}";
  anaFragVar_JCPtDivRefJEt.ytitle = fragVarTitle;
  anaFragVar_JCPtDivRefJEt.PlotCorrelations(anaSel,fragVar,"(ljcpt[0]-ljcptbg[0])/nlrjet",numFragVarBins,0,2);

  AnaFrag anaFragVar_JResp("JResp",fragVarTag,djTree,numFragVarBins,0,2);
  anaFragVar_JResp.xtitle = fragVarTitle;
  anaFragVar_JResp.ytitle = "E_{T}^{RecoJet}/E_{t}^{RefJet}";
  anaFragVar_JResp.PlotCorrelations(anaSel,"nljet/nlrjet",fragVar,numFragVarBins,fragVarMin,fragVarMax);

  // -- plot frag var--
  TCanvas * cFragVar = new TCanvas("c"+fragVarTag,"c"+fragVarTag,500,500);
  CPlot cpFragVar(fragVarTag+anaSel.Tag2,fragVarTag,fragVarTitle,"unit normalization");
  cpFragVar.SetLogy();
  cpFragVar.AddHist1D(anaFragVarNr.hRaw,"Leading Jet","E",kRed,kOpenCircle);
  cpFragVar.AddHist1D(anaFragVarAw.hRaw,"Away Jet","E",kBlue,kOpenSquare);
  cpFragVar.Draw(cFragVar,true);
}
