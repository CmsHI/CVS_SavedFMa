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

  // === ana ===
  TH2F * hLPPtVsGJEt = new TH2F("hLPPtVsGJEt","hLPPtVsJEt",50,0,250,50,0,250);
  TH2F * hLPPtVsJEt = new TH2F("hLPPtVsJEt","hLPPtVsJEt",50,0,250,50,0,250);
  TProfile * hLzVsGJEtProf = new TProfile("hLzVsGJEtProf","hLzVsJEt",50,0,250);
  TProfile * hLzVsJEtProf = new TProfile("hLzVsJEtProf","hLzVsJEt",50,0,250);
  TProfile * hRespVsLzProf = new TProfile("hRespVsLzProf","hLzVsJEt",50,0,1);
  TProfile * hRespVsAzProf = new TProfile("hRespVsAzProf","hAzVsJEt",50,0,1);
  djTree->Draw("lppt[0]:nlrjet>>hLPPtVsGJEt",anaSel.FinDJCut(),"goff");
  djTree->Draw("lppt[0]:nljet>>hLPPtVsJEt",anaSel.FinDJCut(),"goff");
  djTree->Draw("lppt[0]/nlrjet:nlrjet>>hLzVsGJEtProf",anaSel.FinDJCut(),"prof goff");
  djTree->Draw("lppt[0]/nlrjet:nljet>>hLzVsJEtProf",anaSel.FinDJCut(),"prof goff");
  djTree->Draw("nljet/nlrjet:lppt[0]/nlrjet>>hRespVsLzProf",anaSel.FinDJCut(),"prof");
  djTree->Draw("aljet/alrjet:lppt[1]/alrjet>>hRespVsAzProf",anaSel.FinDJCut(),"prof");

  // -- plot --
  TCanvas * cLPPtVsGJEt = new TCanvas("cLPPtVsGJEt","cLPPtVsGJEt",500,500);
  cLPPtVsGJEt->SetLogz();
  CPlot cpLPPtVsGJEt("LPPtVsGJEt"+anaSel.Tag2,"DJ","Leading E_{T}^{genjet} [GeV]","Leading p_{T}^{trk} (in Jet Cone) [GeV]");
  cpLPPtVsGJEt.SetYRange(0,160);
  cpLPPtVsGJEt.AddHist2D(hLPPtVsGJEt,"colz");
  cpLPPtVsGJEt.Draw(cLPPtVsGJEt,true);

  TCanvas * cLzVsGJEtProf = new TCanvas("LzVsGJEtProf","LzVsGJEtProf",500,500);
  CPlot cpLzVsGJEtProf("LzVsGJEtProf","LzVsGJEtProf","Leading E_{T}^{genjet} [GeV]","z^{lead trk} (in Jet Cone)");
  cpLzVsGJEtProf.SetYRange(0,0.3);
  cpLzVsGJEtProf.AddProfile(hLzVsGJEtProf,"j1","E");
  cpLzVsGJEtProf.SetLegend(0.53,0.31,0.86,0.37);
  cpLzVsGJEtProf.Draw(cLzVsGJEtProf,true);

  TCanvas * cLzVsJEtProf = new TCanvas("LzVsJEtProf","LzVsJEtProf",500,500);
  CPlot cpLzVsJEtProf("LzVsJEtProf","LzVsJEtProf","Leading E_{T}^{calojet} [GeV]","z^{lead trk} (in Jet Cone)");
  cpLzVsJEtProf.SetYRange(0,0.3);
  cpLzVsJEtProf.AddProfile(hLzVsJEtProf,"j1","E");
  cpLzVsJEtProf.SetLegend(0.53,0.31,0.86,0.37);
  cpLzVsJEtProf.Draw(cLzVsJEtProf,true);

  // -- response --
  TCanvas * cRespVsLzProf = new TCanvas("RespVsLzProf","RespVsLzProf",500,500);
  CPlot cpRespVsLzProf("RespVsLzProf","RespVsLzProf","Leading E_{T}^{calojet} [GeV]","z^{lead trk} (in Jet Cone)");
  cpRespVsLzProf.SetYRange(0,1.5);
  cpRespVsLzProf.AddProfile(hRespVsLzProf,"j1","E");
  cpRespVsLzProf.AddProfile(hRespVsAzProf,"j2","E",kBlue);
  cpRespVsLzProf.SetLegend(0.53,0.31,0.86,0.37);
  cpRespVsLzProf.Draw(cRespVsLzProf,true);
}
