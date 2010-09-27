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
    const char * inFile0Name="/net/hibat0003/d00/scratch/frankma/HI/jet/Y1JAna_CMSSW_3_8_4/process_aod_mc/output/dijetAna_patJets_dijet30.root",
    TString AnaVersion = "a1",
    TString modName = "dijetAna_mc_genjet_genp",
    TString SrcName = "HydjQDJQ30",
    TString header = "HydjetQ+DJQuen30",
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
  selectionCut anaSel(SrcName,doMC,"S1",50,200,20,2.5);
  anaSel.DJCutType = "Ana";
  anaSel.TrkCutType = "Ana";
  anaSel.Tag2+=TString("_"+modName);
  anaSel.SetCut();
  //anaSel.DJAnd("aljet/nljet<0.7");
  // loose
  selectionCut anaSelLoose(SrcName,doMC,"S1",20,200,20,2.5);
  anaSelLoose.DJCutType = "Ana";
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
  CPlot::sOutDir = anaoutdir;
  // Save output
  TFile * outf = new TFile(anaoutdir+"/"+AnaType+anaSel.Tag2+".root","RECREATE");

  // ============== pdf comparisons ===============
  cout << endl << "======= anaDiJet ========" << endl;
  AnaFrag JDPhi("Ana","JDPhi",djTree,anaSel.FinDJCut(),"","jdphi","","",30,0,TMath::Pi());
  AnaFrag LooseJDPhi("Loose","JDPhi",djTree,anaSelLoose.FinDJCut(),"","jdphi","","",30,0,TMath::Pi());
  AnaFrag Balance("Ana","Balance",djTree,anaSel.FinDJCut(),"","aljet/nljet","","",30,0,1.);
  AnaFrag LooseBalance("Loose","Balance",djTree,anaSelLoose.FinDJCut(),"","aljet/nljet","","",30,0,1.);

  // -- plot --
  TCanvas * cCompJDPhi = new TCanvas("cCompJDPhi","cCompJDPhi",500,500);
  CPlot cpCompJDPhi("CompJDPhi"+anaSel.Tag2,"CompJDPhi","Leading Di-Jet d #phi","pdf");
  cpCompJDPhi.SetYRange(0,5);
  cpCompJDPhi.AddHist1D(LooseJDPhi.hRaw,"Data DiJet Loose","E",kBlue,kOpenCircle);
  cpCompJDPhi.AddHist1D(JDPhi.hRaw,"Data DiJet Tight","E",kBlack,kFullCircle);
  cpCompJDPhi.SetLegend(0.21,0.75,0.54,0.87);
  cpCompJDPhi.SetLegendHeader(header);
  cpCompJDPhi.Draw(cCompJDPhi,true);

  TCanvas * cCompBalance = new TCanvas("cCompBalance","cCompBalance",500,500);
  CPlot cpCompBalance("CompBalance"+anaSel.Tag2,"CompBalance","E_{T}^{j2}/E_{T}^{j1}","pdf");
  cpCompBalance.SetYRange(0,7.5);
  cpCompBalance.AddHist1D(LooseBalance.hRaw,"Data DiJet Loose","E",kBlue,kOpenCircle);
  cpCompBalance.AddHist1D(Balance.hRaw,"Data DiJet Tight","E",kBlack,kFullCircle);
  cpCompBalance.SetLegendHeader(header);
  cpCompBalance.Draw(cCompBalance,true);

  // ============== Dijet Scales ===============
  HisTGroup<TH2F> hgDJEt("DJEt",50,0,200,50,0,200);
  hgDJEt.Add2D(anaSel.DJCutType);

  djTree->Draw(Form("aljet:nljet>>%s",hgDJEt.GetH(anaSel.DJCutType)->GetName()),anaSelLoose.FinDJCut(),"goff");

  // -- plot --
  TCanvas * cDJEt = new TCanvas("cDJEt","cDJEt",500,500);
  cDJEt->SetLogz();
  CPlot cpDJEt("DataDJEt"+anaSel.Tag2,"DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpDJEt.AddHist2D(hgDJEt.H(anaSel.DJCutType),"colz");
  cpDJEt.Draw(cDJEt,true);

  // DJ vs Centrality
  selectionCut centSel(anaSelLoose);
  centSel.Trigger = "";
  centSel.CentCut = "cent<100";
  centSel.VtxCut = "";
  centSel.SetDJEt(80,200,40);
  centSel.SetCut();
  AnaFrag DJCent("Ana","DJCent",djTree,centSel.FinDJCut(),"","cent","","",20,0,100);
  TCanvas * cDJCent = new TCanvas("cDJCent","cDJCent",500,500);
  CPlot cpDJCent("DJCent"+anaSel.Tag2,"DJCent","Centrality [%]","frac of (tight) DiJets");
  cpDJCent.SetYRange(0,0.07);
  cpDJCent.AddHist1D(DJCent.hRaw,"HF based Centrality","E",kBlack,kFullCircle);
  cpDJCent.SetLegendHeader(header);
  cpDJCent.Draw(cDJCent,true);

  // All done, save and exit
  outf->Write();
}
