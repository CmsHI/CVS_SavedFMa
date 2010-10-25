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
    TString AnaVersion = "a1025c",
    TString CmpVersion = "c0",
    TString modName = "djcalo_genp",
    Double_t NrJEtMin = 80,
    Double_t NrJEtMax = 100,
    TString DJCutType = "Ana",
    TString fragVar = "lppt[0]/nljet",
    TString jetPlot = "nljet",
    TString jextraCut = "(ljcpt[0]-ljcptbg[0])/nljet>0.5",
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
  cout << " # entries: " << djTree->GetEntries() << endl;

  // === Declare selection ===
  selectionCut anaSel(SrcName,doMC,"S1",NrJEtMin,NrJEtMax,NrJEtMin,2.5);
  anaSel.DJCutType = DJCutType;
  anaSel.TrkCutType = "Ana";
  anaSel.LJExtraCut = jextraCut;
  anaSel.AJExtraCut = anaSel.NrCut2AwCut(jextraCut);
  anaSel.Tag2+=TString("_"+modName);
  anaSel.SetCut();
  anaSel.Print(1);

  // -- analysis selections --
  anaSel.PreviewCuts(djTree,2);

  // === Define Output ===
  CPlot::sOutDir = outdir;
  gSystem->mkdir(outdir.Data(),kTRUE);

  // === ana ===
  HisTGroup<TH2F> hgPPt_JEt("PPT_JEt",50,0,250,50,0,250);
  hgPPt_JEt.Add2D("Nr");
  hgPPt_JEt.Add2D("GJNr");
  hgPPt_JEt.Add2D("Aw");
  hgPPt_JEt.Add2D("GJAw");
  TH2F * hLPPtVsGJEt = new TH2F("hLPPtVsGJEt","hLPPtVsJEt",50,0,250,50,0,250);
  TH2F * hLPPtVsJEt = new TH2F("hLPPtVsJEt","hLPPtVsJEt",50,0,250,50,0,250);
  TProfile * hLzVsGJEtProf = new TProfile("hLzVsGJEtProf","hLzVsJEt",50,0,250);
  TProfile * hLzVsJEtProf = new TProfile("hLzVsJEtProf","hLzVsJEt",50,0,250);
  TProfile * hRespVsLzProf = new TProfile("hRespVsLzProf","hLzVsJEt",50,0,1);
  TProfile * hRespVsAzProf = new TProfile("hRespVsAzProf","hAzVsJEt",50,0,1);
  djTree->Draw("lppt[0]:nlrjet>>hLPPtVsGJEt",anaSel.FinLJCut(),"goff");
  djTree->Draw("lppt[0]:nljet>>hLPPtVsJEt",anaSel.FinLJCut(),"goff");
  djTree->Draw("lppt[0]/nlrjet:nlrjet>>hLzVsGJEtProf",anaSel.FinLJCut(),"prof goff");
  djTree->Draw("lppt[0]/nlrjet:nljet>>hLzVsJEtProf",anaSel.FinLJCut(),"prof goff");
  djTree->Draw("nljet/nlrjet:lppt[0]/nlrjet>>hRespVsLzProf",anaSel.FinLJCut(),"prof goff");
  djTree->Draw("aljet/alrjet:lppt[1]/alrjet>>hRespVsAzProf",anaSel.FinAJCut(),"prof goff");

  // -- plot --
  TCanvas * cLPPtVsGJEt = new TCanvas("cLPPtVsGJEt","cLPPtVsGJEt",500,500);
  cLPPtVsGJEt->SetLogz();
  CPlot cpLPPtVsGJEt("LPPtVsGJEt"+anaSel.Tag2,"DJ","Leading E_{T}^{genjet} [GeV]","Leading p_{T}^{trk} (in Jet Cone) [GeV]");
  cpLPPtVsGJEt.SetYRange(0,160);
  cpLPPtVsGJEt.AddHist2D(hLPPtVsGJEt,"colz");
  cpLPPtVsGJEt.Draw(cLPPtVsGJEt,true);

  TCanvas * cLzVsGJEtProf = new TCanvas("LzVsGJEtProf","LzVsGJEtProf",500,500);
  CPlot cpLzVsGJEtProf("LzVsGJEtProf"+anaSel.Tag2,"LzVsGJEtProf","Leading E_{T}^{genjet} [GeV]","z^{lead trk} (in Jet Cone)");
  cpLzVsGJEtProf.SetYRange(0,0.3);
  cpLzVsGJEtProf.AddProfile(hLzVsGJEtProf,"j1","E");
  cpLzVsGJEtProf.SetLegend(0.53,0.31,0.86,0.37);
  cpLzVsGJEtProf.Draw(cLzVsGJEtProf,true);

  TCanvas * cLzVsJEtProf = new TCanvas("LzVsJEtProf","LzVsJEtProf",500,500);
  CPlot cpLzVsJEtProf("LzVsJEtProf"+anaSel.Tag2,"LzVsJEtProf","Leading E_{T}^{calojet} [GeV]","z^{lead trk} (in Jet Cone)");
  cpLzVsJEtProf.SetYRange(0,0.3);
  cpLzVsJEtProf.AddProfile(hLzVsJEtProf,"j1","E");
  cpLzVsJEtProf.SetLegend(0.53,0.31,0.86,0.37);
  cpLzVsJEtProf.Draw(cLzVsJEtProf,true);

  // -- response --
  TCanvas * cRespVsLzProf = new TCanvas("RespVsLzProf","RespVsLzProf",500,500);
  CPlot cpRespVsLzProf("RespVsLzProf"+anaSel.Tag2,"RespVsLzProf","z^{lead trk} (in Jet Cone)","E_{T}^{calojet}/E_{T}^{genjet}");
  cpRespVsLzProf.SetYRange(0,1.5);
  cpRespVsLzProf.AddProfile(hRespVsLzProf,"j1","E");
  cpRespVsLzProf.AddProfile(hRespVsAzProf,"j2","E",kBlue,kOpenSquare);
  cpRespVsLzProf.SetLegend(0.53,0.31,0.86,0.37);
  cpRespVsLzProf.Draw(cRespVsLzProf,true);
}
