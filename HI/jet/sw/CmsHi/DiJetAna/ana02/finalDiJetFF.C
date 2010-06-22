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

void finalDiJetFF(int doMC=1,
    const char * inFile0Name="anaDiJetFF.root",
    TString AnaName = "mcuq120V0",
    TString header="Hydjet2.76TeV+DiJet(120-170GeV)",
    //
    TString title1="MC Calojet",
    TString title2="MC Genjet")
{
  // Define Inputs
  selectionCut mcAna(AnaName,doMC,1,120,170,80);
  cout << "======= Inputs: ========" << endl;
  TString indir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  TString inFile0Path = indir+"/"+inFile0Name;
  cout << inFile0Path << endl;
  TFile * inFile0 = new TFile(inFile0Path);
  inFile0->ls();

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = indir;
  cout << "Output dir: " << anaoutdir << endl;
  CPlot::sOutDir = anaoutdir+"/final";
  // Save output
  TFile * outf = new TFile(Form("%s/finalDiJetTrk.root",anaoutdir.Data()),"RECREATE");

  // All done, save and exit
  outf->Write();
}
