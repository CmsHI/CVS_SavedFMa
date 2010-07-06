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
#include "Saved/DiJetAna/ana02/aliases_dijet.C"
#include "Saved/DiJetAna/ana02/selectionCut.h"
#include "Saved/DiJetAna/ana02/AnaFrag.h"
#include "Saved/DiJetAna/ana02/compareHist.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"
using namespace std;

void anaDiJetTrk(int doMC=1,
    /*
    const char * inFile0Name="../process_aod/outputs/McUqDj80to120_DJes002_10k.root",
    TString outdir = "plots/mcuq80to120_10k",
    TString header="Hydjet2.76TeV+DiJet(80-120GeV)",
    */
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root",
    TString AnaName = "mcuq120V0",
    TString header="Hydjet2.76TeV+DiJet(120-170GeV)",
    //
    TString title1="MC Calojet",
    TString title2="MC Genjet")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();

  // Define dijet selection
  selectionCut mcAna(AnaName,doMC,1,110,170,90);
  selectionCut mcAnaLoose(AnaName,doMC,1,50,200,50);

  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  aliases_dijet(mcj2t3);
  inFile0->GetObject("dijetAna_mc_calojet_genp/djTree",mcj2t0);
  aliases_dijet(mcj2t0);
  inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);
  aliases_dijet(mcj1t0);

  TString djSelTag("Ana");
  TString trkSelTag("Ana");
  cout << endl << "====== DJ Selection: " << djSelTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna_mc/mcj2t0 # entries: " << mcj2t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t0->GetEntries(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna_mc/mcj1t0 # entries: " << mcj1t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ[djSelTag]) << endl;

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir+"/djtrk";
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJetTrk.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  AnaFrag mcj2t0JTrkDPhi("mcj2t0","JTrkDPhi",mcj2t0,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
  AnaFrag mcj1t0JTrkDPhi("mcj1t0","JTrkDPhi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());

  // -- plot --
  TCanvas * cCompJTrkDPhi = new TCanvas("cCompJTrkDPhi","cCompJTrkDPhi",500,500);
  CPlot cpCompJTrkDPhi("CompJTrkDPhi","CompJTrkDPhi","d #phi(j1,trk)","pdf per sel evt");
  cpCompJTrkDPhi.SetYRange(0,550);
  cpCompJTrkDPhi.AddHist1D(mcj1t0JTrkDPhi.hRaw,"GenJet, GenTrk (p_{T}>1GeV)","Ehist",kRed,0);
  cpCompJTrkDPhi.AddHist1D(mcj2t0JTrkDPhi.hRaw,"CaloJet, GenTrk (p_{T}>1GeV)","E",kBlack);
  cpCompJTrkDPhi.SetLegend(0.41,0.27,0.71,0.37);
  cpCompJTrkDPhi.Draw(cCompJTrkDPhi,true);

  // All done, save and exit
  outf->Write();
}
