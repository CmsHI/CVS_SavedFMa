#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "TSystem.h"
#include "CmsHi/DiJetAna/ana/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana/selectionCut.h"
#include "CmsHi/DiJetAna/ana/AnaFrag.h"
#include "CmsHi/DiJetAna/ana/compareHist.h"
using namespace std;

void anaDiJet(int doMC=1,
    const char * inFile0Name="../process_aod/outputs/McUqDj80to120_DJes002_10k.root",
    /*
    TString outdir = "plots/mcuq80to120_10k",
    TString header="Hydjet2.76TeV+DiJet(80-120GeV)",
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root",
    */
    TString header="Hydjet2.76TeV+DiJet(120-170GeV)",
    TString outdir = "plots/mcuq120to170_10k",
    TString title1="MC Calojet",
    TString title2="MC Genjet")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  // Define dijet selection
  /*
  selectionCut mcAna(doMC,1,80,120,70);
  selectionCut mcMatAna(doMC,11,80,120,70);
  */
  selectionCut mcAna(doMC,1,120,170,100);
  selectionCut mcMatAna(doMC,11,120,170,100);
  TString anaoutdir = Form("%s/%s/dj",outdir.Data(),mcAna.AnaTag.Data());
  gSystem->mkdir(anaoutdir.Data(),kTRUE);

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  aliases_dijet(mcj2t3);
  inFile0->GetObject("dijetAna_mc_calojet_genp/djTree",mcj2t0);
  aliases_dijet(mcj2t0);
  inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);
  aliases_dijet(mcj1t0);

  cout << endl << "====== Ana: " << mcAna.AnaTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t0 # entries: " << mcj2t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t0->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj1t0 # entries: " << mcj1t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ) << endl;

  // Setup root
  TH1::SetDefaultSumw2();

  // Save output
  TFile * outf = new TFile(Form("%s/djana_hists.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  // check dijet
  cout << " --- Check Jet Distributions --- " << endl;
  cout << " Evt Sel: " << mcAna.Evt << endl;
  TCanvas *ccomp2 = new TCanvas("ccomp2","",500,500);
  compareHist comp2(mcj2t3,mcj1t0,"jdphi","dPhi",mcAna.Evt.Data(),mcAna.Evt.Data(),0,3.14,30);
  comp2.Normalize(1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("leading dijet d #phi");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetLegend(0.23,0.81,0.52,0.91);
  comp2.SetLegendHeader(header);
  comp2.SetMaximum(7);
  comp2.Draw2("E");
  ccomp2->Print(Form("%s/%s_dPhi.gif",anaoutdir.Data(),mcAna.Tag.Data()) );

  TCanvas *ccomp3 = new TCanvas("ccomp3","",500,500);
  compareHist comp3(mcj2t3,mcj1t0,"2*(nljet-aljet)/(nljet+aljet)","Balance",mcAna.Evt.Data(),mcAna.Evt.Data(),0,1.2,30);
  comp3.Normalize(1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("(p_{T}^{j1}-p_{T}^{j2})/((p_{T}^{j1}+p_{T}^{j2})/2)");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.45,0.80,0.75,0.9);
  comp3.SetLegendHeader(header);
  comp3.SetMaximum(8);
  comp3.Draw2("E");
  ccomp3->Print(Form("%s/%s_Balance.gif",anaoutdir.Data(),mcAna.Tag.Data()));

  // All done, save and exit
  outf->Write();
}
