#include <iostream>
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
#include "Saved/DiJetAna/macros/AnaFrag.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"

#include "Saved/DiJetAna/anaJEx/selectionCut.h"
using namespace std;

void anaDiJetTrk(int doMC=0,
    /*
    const char * inFile0Name="../process_aod/outputs/dijetaAna_JulyMb4_try4.root",
    TString AnaName = "dataMb4p4/a0",
    TString header="July Data (MB)",
    */
    const char * inFile0Name="../process_aod/outputs/dijetaAna_JulyHard4_try4.root",
    TString AnaName = "dataHd4p4/a0",
    TString header="July Data (Hard Triggered)",
    //
    TString title1="Data",
    TString title2="MC")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();

  // === Define dijet selection ===
  selectionCut mcAna(AnaName,doMC,1,70,120,60);
  mcAna.DJAnd(TCut("(5*cbin)<20"));
  // check
  mcAna.Print();
  // loose
  selectionCut mcAnaLoose(AnaName,doMC,1,20,200,20);
  mcAnaLoose.DJAnd(TCut("(5*cbin)<20"));

  // === Get Trees ===
  TTree *j2t3, *j2t0, *j1t0;
  if (!doMC) {
    inFile0->GetObject("dijetAna_data/djTree",j2t3);
  }
  else {
    inFile0->GetObject("dijetAna_mc/djTree",j2t3);
    // MC Sysemtics
    inFile0->GetObject("dijetAna_mc_calojet_genp/djTree",j2t0); aliases_dijet(j2t0,1);
    inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",j1t0); aliases_dijet(j1t0,1);
  }
  aliases_dijet(j2t3,doMC);

  TString djSelTag("Ana");
  TString trkSelTag("Ana");
  cout << endl << "====== DJ Selection: " << djSelTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna j2t3 # entries: " << j2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << j2t3->GetEntries(mcAna.DJ[djSelTag]) << endl;
  if (doMC) {
    cout << "dijetAna_mc/j2t0 # entries: " << j2t0->GetEntries() << endl;
    cout << "# DJ events passed: " << j2t0->GetEntries(mcAna.DJ[djSelTag]) << endl;
    cout << "dijetAna_mc/j1t0 # entries: " << j1t0->GetEntries() << endl;
    cout << "# DJ events passed: " << j1t0->GetEntries(mcAna.DJ[djSelTag]) << endl;
  }

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir+"/djtrk";
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJetTrk.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  AnaFrag j2t3JTrkDPhi("j2t3","JTrkDPhi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
  AnaFrag j2t3JTrkTight5DPhi("j2t3","JTrkTight5DPhi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Tight5"],"pndphi","","",30,0,TMath::Pi());
  if (doMC) {
    AnaFrag j2t3JTrkDPhi("j2t3","JTrkDPhi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
    AnaFrag j2t0JTrkDPhi("j2t0","JTrkDPhi",j2t0,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
    AnaFrag j1t0JTrkDPhi("j1t0","JTrkDPhi",j1t0,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
  }

  // -- plot --
  TCanvas * cCompJTrkDPhi = new TCanvas("cCompJTrkDPhi","cCompJTrkDPhi",500,500);
  CPlot cpCompJTrkDPhi("CompJTrkDPhi","CompJTrkDPhi","#Delta#phi(trk,j1)","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{d#Delta#phi}");
  cpCompJTrkDPhi.SetYRange(0,550);
  cpCompJTrkDPhi.AddHist1D(j2t3JTrkDPhi.hRaw,"hiSelectedTrk (p_{T}>1GeV)","E",kBlack,kFullCircle);
  cpCompJTrkDPhi.AddHist1D(j2t3JTrkTight5DPhi.hRaw,"hiSelectedTrk (p_{T}>7GeV)","E",kBlue,kOpenCircle);
  cpCompJTrkDPhi.SetLegend(0.41,0.76,0.71,0.86);
  cpCompJTrkDPhi.SetLegendHeader(header);
  cpCompJTrkDPhi.Draw(cCompJTrkDPhi,true);

  // All done, save and exit
  outf->Write();
}
