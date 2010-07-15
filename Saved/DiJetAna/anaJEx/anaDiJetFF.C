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

void anaDiJetFF(int doMC=0,
    /*
       const char * inFile0Name="../process_aod/outputs/dijetAnaTightDPhi_JEx_ZP_MB_proc0_all.root",
       TString AnaName = "ZSMb/dphi25/a0",
       TString header="July Data (MB)",
     */
    const char * inFile0Name="../process_aod/outputs/dijetAnaTightDPhi_JEx_ZP_Hard_proc1_all.root",
    TString AnaName = "ZSHd1/dp25/a3",
    //const char * inFile0Name="../matttrees/dijetAna_anaJet_HardEnriched_ZS_KT4_TightDPhi.root",
    //TString AnaName = "ZSHdfj0/dp25/a2",
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
  selectionCut mcAna(AnaName,doMC,1,100,170,50,2.5);
  mcAna.DJAnd(TCut("(5*cbin)<20"));
  // check
  mcAna.Print();
  // loose
  selectionCut mcAnaLoose(AnaName,doMC,1,20,200,20,2.5);
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
  CPlot::sOutDir = anaoutdir+"/djff";
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJetFF.root",anaoutdir.Data()),"RECREATE");

  // ============= dijet FF ================
  cout << endl << "==================== DiJet Ana ===================" << endl;
  // Reco
  AnaFrag recoNr("reco","NrXi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1./zn)","pndr<0.5","pndrbg<0.5",mcAna.histXiBins,mcAna.histXiMin,mcAna.histXiMax);
  AnaFrag recoAw("reco","AwXi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1./za)","padr<0.5","padrbg<0.5",mcAna.histXiBins,mcAna.histXiMin,mcAna.histXiMax);

  // Systematics
  AnaFrag recoUpperNr("reco","UpperNrXi",j2t3,mcAna.DJ["AnaUpper"],mcAna.Trk["Ana"],"log(nljet*1.14/ppt)","pndr<0.5","pndrbg<0.5",mcAna.histXiBins,mcAna.histXiMin,mcAna.histXiMax);
  AnaFrag recoUpperAw("reco","UpperAwXi",j2t3,mcAna.DJ["AnaUpper"],mcAna.Trk["Ana"],"log(aljet*1.14/ppt)","padr<0.5","padrbg<0.5",mcAna.histXiBins,mcAna.histXiMin,mcAna.histXiMax);

  AnaFrag recoLowerNr("reco","LowerNrXi",j2t3,mcAna.DJ["AnaLower"],mcAna.Trk["Ana"],"log(nljet*0.86/ppt)","pndr<0.5","pndrbg<0.5",mcAna.histXiBins,mcAna.histXiMin,mcAna.histXiMax);
  AnaFrag recoLowerAw("reco","LowerAwXi",j2t3,mcAna.DJ["AnaLower"],mcAna.Trk["Ana"],"log(aljet*0.86/ppt)","padr<0.5","padrbg<0.5",mcAna.histXiBins,mcAna.histXiMin,mcAna.histXiMax);

  if (doMC) {
    // Reco jet + genp
    AnaFrag j2t0Nr("j2t0","NrXi",j2t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1./zn)","pndr<0.5","pndrbg<0.5");
    AnaFrag j2t0Aw("j2t0","AwXi",j2t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1./za)","padr<0.5","padrbg<0.5");
    AnaFrag j2t0SelRefNr("j2t0SelRef","NrXi",j2t0,mcAna.DJ["Ref"],mcAna.Trk["Ana"],"log(1./zn)","pndr<0.5","pndrbg<0.5");
    AnaFrag j2t0SelRefAw("j2t0SelRef","AwXi",j2t0,mcAna.DJ["Ref"],mcAna.Trk["Ana"],"log(1./za)","padr<0.5","padrbg<0.5");

    // Gen
    AnaFrag mcGenNr("mcGen","NrXi",j1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1/zn)","pndr<0.5","pndrbg<0.5");
    AnaFrag mcGenAw("mcGen","AwXi",j1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1/za)","padr<0.5","padrbg<0.5");

    // Gen-Truth
    AnaFrag mcGenTruthNr("mcGenTruth","NrXi",j1t0,mcAna.DJ["Ana"],mcAna.Trk["AnaSig"],"log(1/zn)","pndr<0.5","pndrbg<0.5");
    AnaFrag mcGenTruthAw("mcGenTruth","AwXi",j1t0,mcAna.DJ["Ana"],mcAna.Trk["AnaSig"],"log(1/za)","padr<0.5","padrbg<0.5");
  }

  // All done, save and exit
  outf->Write();
}
