#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "TSystem.h"
#include "CmsHi/DiJetAna/ana02/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana02/selectionCut.h"
#include "CmsHi/DiJetAna/ana02/AnaFrag.h"
#include "CmsHi/DiJetAna/ana02/compareHist.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"
using namespace std;

void anaDiJet(int doMC=1,
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
  selectionCut mcAna(AnaName,doMC,1,120,170,80);
  selectionCut mcAnaLoose(AnaName,doMC,1,50,200,50);

  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  aliases_dijet(mcj2t3);
  inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);
  aliases_dijet(mcj1t0);

  cout << endl << "====== Ana Selection: " << mcAna.AnaTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ["Ana"]) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ["Ana"]) << endl;
  cout << "dijetAna_mc/mcj1t0 # entries: " << mcj1t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ["Ana"]) << endl;

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir;
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJet.root",anaoutdir.Data()),"RECREATE");

  /*
  // ============== pdf comparisons ===============
  // check dijet
  cout << " --- Check Jet Distributions --- " << endl;
  cout << " Evt Sel: " << mcAna.Evt << endl;
  cout << " DJ Sel: " << TString(mcAna.DJ) << endl;
  TCanvas *ccomp2 = new TCanvas("ccomp2","",500,500);
  compareHist comp2(mcj2t3,mcj1t0,"jdphi","dPhi",mcAna.DJ["Ana"],mcAna.DJ["Ana"],0,3.14,30);
  comp2.Normalize(1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("leading dijet d #phi");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetLegend(0.23,0.81,0.52,0.91);
  comp2.SetLegendHeader(header);
  comp2.SetMaximum(7);
  comp2.Draw2("E");
  ccomp2->Print(Form("%s/dj/%s_dPhi.gif",anaoutdir.Data(),mcAna.Tag.Data()) );

  TCanvas *ccomp3 = new TCanvas("ccomp3","",500,500);
  compareHist comp3(mcj2t3,mcj1t0,"2*(nljet-aljet)/(nljet+aljet)","Balance",mcAna.DJ["Ana"],mcAna.DJ["Ana"],0,1.2,30);
  comp3.Normalize(1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("(p_{T}^{j1}-p_{T}^{j2})/((p_{T}^{j1}+p_{T}^{j2})/2)");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.45,0.80,0.75,0.9);
  comp3.SetLegendHeader(header);
  comp3.SetMaximum(8);
  comp3.Draw2("E");
  ccomp3->Print(Form("%s/dj/%s_Balance.gif",anaoutdir.Data(),mcAna.Tag.Data()));
  */

  // Dijet Scales
  /*
  TH2F * hJEtNrAwCalo = new TH2F("hJEtNrAwCalo","",50,0,200,50,0,200);
  TH2F * hJEtNrAwGenMatCalo = (TH2F*)hJEtNrAwCalo->Clone("hJEtNrAwGenMatCalo");
  TH2F * hJEtNrAwGen = (TH2F*)hJEtNrAwCalo->Clone("hJEtNrAwGen");

  mcj2t3->Draw("aljet:nljet>>hJEtNrAwCalo",mcAnaLoose.DJ["Ana"],"goff");
  mcj1t0->Draw("aljet:nljet>>hJEtNrAwGen",mcAnaLoose.DJ["Ana"],"goff");

  // === Final Plots ===
  TCanvas * cJEtNrAwCalo = new TCanvas("cJEtNrAwCalo","cJEtNrAwCalo",500,500);
  CPlot cpJEtNrAwCalo("JEtNrAwCalo","FF","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpJEtNrAwCalo.SetXRange(0,200);
  cpJEtNrAwCalo.SetYRange(0,200);
  cpJEtNrAwCalo.AddHist2D(hJEtNrAwCalo,"colz");
  cpJEtNrAwCalo.Draw(cJEtNrAwCalo,true);

  TCanvas * cJEtNrAwGenMatCalo = new TCanvas("cJEtNrAwGenMatCalo","cJEtNrAwGenMatCalo",500,500);
  mcj2t3->Draw("alrjet:nlrjet",mcMatAnaLoose.DJ,"goff");
  */
  /*
  CPlot cpJEtNrAwGenMatCalo("JEtNrAwGenMatCalo","FF","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpJEtNrAwGenMatCalo.SetXRange(0,200);
  cpJEtNrAwGenMatCalo.SetYRange(0,200);
  cpJEtNrAwGenMatCalo.AddHist2D(hJEtNrAwGenMatCalo,"colz");
  cpJEtNrAwGenMatCalo.Draw(cJEtNrAwGenMatCalo,true);
  */

  /*
  TCanvas * cJEtNrAwGen = new TCanvas("cJEtNrAwGen","cJEtNrAwGen",500,500);
  CPlot cpJEtNrAwGen("JEtNrAwGen","FF","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpJEtNrAwGen.SetXRange(0,200);
  cpJEtNrAwGen.SetYRange(0,200);
  cpJEtNrAwGen.AddHist2D(hJEtNrAwGen,"colz");
  cpJEtNrAwGen.Draw(cJEtNrAwGen,true);
  */

  // All done, save and exit
  outf->Write();
}
