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

void anaDiJet(int doMC=0,
    const char * inFile0Name="/net/hisrv0001/home/frankma/scratch01/ana/JEx_Hard_GSR_ZS/mc0824_hlt/dijetAna_*.root",
    TString SrcName = "JExHdMc",
    TString AnaVersion = "a0",
    TString AnaType = "dj",
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
  anaSel.DJCutType = "Ref";
  anaSel.TrkCutType = "Ana";
  anaSel.SetCut();
  //anaSel.DJAnd("aljet/nljet<0.7");
  // loose
  selectionCut anaSelLoose(SrcName,doMC,"S0",20,200,20,2.5);
  anaSelLoose.DJCutType = "Ref";
  anaSelLoose.SetCut();

  // -- analysis selections --
  cout << endl << "====== DJ Selection: " << anaSel.DJCutType << " ======" << endl;
  cout << " DJ selection: " << TString(anaSel.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(anaSel.FinDJCut()) << endl;
  cout << " DJ selection: " << TString(anaSelLoose.FinDJCut()) << endl;
  cout << "# DJ events passed: " << djTree->GetEntries(anaSelLoose.FinDJCut()) << endl;

  // === Define Output ===
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("out/%s/%s/%s",SrcName.Data(),anaSel.Tag.Data(),AnaVersion.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir+"/"+AnaType;
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJet.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  cout << endl << "======= anaDiJet ========" << endl;
  AnaFrag djTreeJDPhi("djTree","JDPhi",djTree,anaSel.FinDJCut(),"","jdphi","","",30,0,TMath::Pi());
  AnaFrag djTreeLooseJDPhi("djTreeLoose","JDPhi",djTree,anaSelLoose.FinDJCut(),"","jdphi","","",30,0,TMath::Pi());
  AnaFrag djTreeBalance("djTree","Balance",djTree,anaSel.FinDJCut(),"","aljet/nljet","","",30,0,1.);
  AnaFrag djTreeLooseBalance("djTreeLoose","Balance",djTree,anaSelLoose.FinDJCut(),"","aljet/nljet","","",30,0,1.);

  // -- plot --
  TCanvas * cCompJDPhi = new TCanvas("cCompJDPhi","cCompJDPhi",500,500);
  CPlot cpCompJDPhi("CompJDPhi","CompJDPhi","Leading Di-Jet d #phi","pdf");
  cpCompJDPhi.SetYRange(0,5);
  cpCompJDPhi.AddHist1D(djTreeLooseJDPhi.hRaw,"Data DiJet Loose","E",kBlue,kOpenCircle);
  cpCompJDPhi.AddHist1D(djTreeJDPhi.hRaw,"Data DiJet Tight","E",kBlack,kFullCircle);
  cpCompJDPhi.SetLegend(0.21,0.75,0.54,0.87);
  cpCompJDPhi.SetLegendHeader(header);
  cpCompJDPhi.Draw(cCompJDPhi,true);

  TCanvas * cCompBalance = new TCanvas("cCompBalance","cCompBalance",500,500);
  CPlot cpCompBalance("CompBalance","CompBalance","E_{T}^{j2}/E_{T}^{j1}","pdf");
  cpCompBalance.SetYRange(0,7.5);
  cpCompBalance.AddHist1D(djTreeLooseBalance.hRaw,"Data DiJet Loose","E",kBlue,kOpenCircle);
  cpCompBalance.AddHist1D(djTreeBalance.hRaw,"Data DiJet Tight","E",kBlack,kFullCircle);
  cpCompBalance.SetLegendHeader(header);
  cpCompBalance.Draw(cCompBalance,true);

  // ============== Dijet Scales ===============
  TString DJEtName0 = modName+"_"+anaSel.DJCutType;
  HisTGroup<TH2F> hgDJEt("DJEt",50,0,200,50,0,200);
  hgDJEt.Add2D(DJEtName0);

  djTree->Draw(Form("aljet:nljet>>%s",hgDJEt.GetH(DJEtName0)->GetName()),anaSelLoose.FinDJCut(),"goff");

  // -- plot --
  TCanvas * cDJEt = new TCanvas("cDJEt","cDJEt",500,500);
  cDJEt->SetLogz();
  CPlot cpDJEt("DataDJEt","DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpDJEt.AddHist2D(hgDJEt.H(DJEtName0),"colz");
  cpDJEt.Draw(cDJEt,true);

  // DJ vs Centrality
  anaSel.Trigger = "";
  anaSel.CentCut = "cent<100";
  anaSel.VtxCut = "";
  anaSel.SetCut();
  AnaFrag djTreeDJCent("djTree","DJCent",djTree,anaSel.FinDJCut(),"","cent","","",20,0,100);
  TCanvas * cDJCent = new TCanvas("cDJCent","cDJCent",500,500);
  CPlot cpDJCent("DJCent","DJCent","Centrality [%]","frac of (tight) DiJets");
  cpDJCent.SetYRange(0,0.07);
  cpDJCent.AddHist1D(djTreeDJCent.hRaw,"HF based Centrality","E",kBlack,kFullCircle);
  cpDJCent.SetLegendHeader(header);
  cpDJCent.Draw(cDJCent,true);

  // All done, save and exit
  outf->Write();
}
