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
#include "CmsHi/DiJetAna/ana02/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana02/selectionCut.h"
#include "CmsHi/DiJetAna/ana02/AnaFrag.h"
#include "CmsHi/DiJetAna/ana02/compareHist.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"
using namespace std;

void checkDiJetFF(int doMC=1,
    /*
    const char * inFile0Name="../process_aod/outputs/McUqDj80to120_DJes002_10k.root",
    TString outdir = "plots/mcuq80to120_10k",
    TString header="Hydjet2.76TeV+DiJet(80-120GeV)",
    */
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root",
    TString AnaName = "mcuq120V3",
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
  selectionCut mcAna(AnaName,doMC,1,120,170,80);

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
  // Save output
  TFile * outf = new TFile(Form("%s/checkDiJetFF.root",anaoutdir.Data()),"RECREATE");

  // ============= dijet FF ================
  cout << endl << "==================== DiJet Ana ===================" << endl;
  // ====== Beginning (0) ======
  // Gen-Truth
  TCut truthCut = "psube==0";
  AnaFrag mcGenTruthNr("mcGenTruth","NrXi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"]&&truthCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenTruthAw("mcGenTruth","AwXi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"]&&truthCut,"log(1/za)","padr<0.5","padrbg<0.5");

  // === Add HI background ===
  // Gen
  AnaFrag mcGenNr("mcGen","NrXi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenAw("mcGen","AwXi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1/za)","padr<0.5","padrbg<0.5");
  AnaFrag mcGenMatNr("mcGenMat","NrXi",mcj1t0,mcAna.DJ["AnaMatRef"],mcAna.Trk["Ana"],"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcGenMatAw("mcGenMat","AwXi",mcj1t0,mcAna.DJ["AnaMatRef"],mcAna.Trk["Ana"],"log(1/za)","padr<0.5","padrbg<0.5");

  // === Check trees ===
  // - chk0.0
  AnaFrag mcj2t0SelRLRAnaRLRNr("mcj2t0SelRLRAnaRLR","NrXi",mcj2t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(nrlrjet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0SelRLRAnaRLRAw("mcj2t0SelRLRAnaRLR","AwXi",mcj2t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(arlrjet/ppt)","padr<0.5","padrbg<0.5");

  AnaFrag mcj1t0SelRLRAnaRLRNr("mcj1t0SelRLRAnaRLR","NrXi",mcj1t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(nrlrjet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj1t0SelRLRAnaRLRAw("mcj1t0SelRLRAnaRLR","AwXi",mcj1t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(arlrjet/ppt)","padr<0.5","padrbg<0.5");

  //
  // Systematic checks
  //

  // === Check Exlusive Effect of JES in FF calc ===
  // - chk0.1
  AnaFrag mcj1t0AnaRNr("mcj1t0AnaR","NrXi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(nlrjet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj1t0AnaRAw("mcj1t0AnaR","AwXi",mcj1t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(alrjet/ppt)","padr<0.5","padrbg<0.5");
  // - chk0.2
  AnaFrag mcj2t0SelRLRNr("mcj2t0SelRLR","NrXi",mcj2t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(nljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0SelRLRAw("mcj2t0SelRLR","AwXi",mcj2t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(aljet/ppt)","padr<0.5","padrbg<0.5");
  // - chkchk0.2
  AnaFrag mcj2t0SelRLRAnaRLNr("mcj2t0SelRLRAnaRL","NrXi",mcj2t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(nrljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0SelRLRAnaRLAw("mcj2t0SelRLRAnaRL","AwXi",mcj2t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(arljet/ppt)","padr<0.5","padrbg<0.5");

  // === Check Exlusive Effect of JES in DJ Selection===
  // - chk1.0
  AnaFrag mcj1t0SelLRNr("mcj1t0SelLR","NrXi",mcj1t0,mcAna.DJ["AnaOrderRef"],mcAna.Trk["Ana"],"log(nljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj1t0SelLRAw("mcj1t0SelLR","AwXi",mcj1t0,mcAna.DJ["AnaOrderRef"],mcAna.Trk["Ana"],"log(aljet/ppt)","padr<0.5","padrbg<0.5");
  // - chk1.1
  AnaFrag mcj1t0SelRNr("mcj1t0SelR","NrXi",mcj1t0,mcAna.DJ["Ref"],mcAna.Trk["Ana"],"log(nljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj1t0SelRAw("mcj1t0SelR","AwXi",mcj1t0,mcAna.DJ["Ref"],mcAna.Trk["Ana"],"log(aljet/ppt)","padr<0.5","padrbg<0.5");
  // - chk1.2
  AnaFrag mcj1t0SelRLRNr("mcj1t0SelRLR","NrXi",mcj1t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(nljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj1t0SelRLRAw("mcj1t0SelRLR","AwXi",mcj1t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(aljet/ppt)","padr<0.5","padrbg<0.5");
  // - chkchk1.2
  AnaFrag mcj1t0SelRLRAnaRLNr("mcj1t0SelRLRAnaRL","NrXi",mcj1t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(nrljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj1t0SelRLRAnaRLAw("mcj1t0SelRLRAnaRL","AwXi",mcj1t0,mcAna.DJ["RefOrderRef"],mcAna.Trk["Ana"],"log(arljet/ppt)","padr<0.5","padrbg<0.5");

  AnaFrag mcj2t0AnaRNr("mcj2t0AnaR","NrXi",mcj2t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(nlrjet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0AnaRAw("mcj2t0AnaR","AwXi",mcj2t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(alrjet/ppt)","padr<0.5","padrbg<0.5");

  // ====== Beginning (1) = chk0.2: j2t0SelRLRAnaRL ======
  // === Check Selection Effects on top of JES in FF Calc ===
  // -chk2.0
  AnaFrag mcj2t0SelRLAnaNr("mcj2t0SelRLAna","NrXi",mcj2t0,mcAna.DJ["AnaOrderRef"],mcAna.Trk["Ana"],"log(nljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0SelRLAnaAw("mcj2t0SelRLAna","AwXi",mcj2t0,mcAna.DJ["AnaOrderRef"],mcAna.Trk["Ana"],"log(aljet/ppt)","padr<0.5","padrbg<0.5");
  // -chk2.1
  AnaFrag mcj2t0SelRAnaNr("mcj2t0SelRAna","NrXi",mcj2t0,mcAna.DJ["Ref"],mcAna.Trk["Ana"],"log(nljet/ppt)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0SelRAnaAw("mcj2t0SelRAna","AwXi",mcj2t0,mcAna.DJ["Ref"],mcAna.Trk["Ana"],"log(aljet/ppt)","padr<0.5","padrbg<0.5");

  // === Reco jet + genp ===
  AnaFrag mcj2t0Nr("mcj2t0","NrXi",mcj2t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0Aw("mcj2t0","AwXi",mcj2t0,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"log(1./za)","padr<0.5","padrbg<0.5");
  AnaFrag mcj2t0MatNr("mcj2t0Mat","NrXi",mcj2t0,mcAna.DJ["AnaMatRef"],mcAna.Trk["Ana"],"log(1./zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcj2t0MatAw("mcj2t0Mat","AwXi",mcj2t0,mcAna.DJ["AnaMatRef"],mcAna.Trk["Ana"],"log(1./za)","padr<0.5","padrbg<0.5");

  // All done, save and exit
  outf->Write();
}
