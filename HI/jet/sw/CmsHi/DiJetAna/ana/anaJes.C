#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "CmsHi/DiJetAna/ana/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana/selectionCut.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
using namespace std;

void anaJes(int doMC=1,
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root",
    TString outdir = "plots/mcuq120to170_10k/dj")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  // Define dijet selection
  selectionCut mcMatAna(doMC,11,120,170,100);

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  inFile0->GetObject("dijetAna_mc_periph/djTree",mcj2t3peri);

  cout << "DJ selection: " << TString(mcMatAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcMatAna.DJ) << endl;
  cout << "dijetAna_mc_periph/mcj2t3 # entries: " << mcj2t3peri->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3peri->GetEntries(mcMatAna.DJ) << endl;

  //  - DiJet jes -
  TProfile * hJes = new TProfile("hJes","JES profile",20,mcMatAna.nrJetPtMin,mcMatAna.nrJetPtMax);

  TProfile * hDJesTopCentNr = (TProfile*)hJes->Clone("hDJesTopCentNr");
  mcj2t3->Draw("nljet/nlrjet:nlrjet>>hDJesTopCentNr",mcMatAna.DJ,"goff");

  TProfile * hDJesTopCentAw = (TProfile*)hDJesTopCentNr->Clone("hDJesTopCentAw");
  mcj2t3->Draw("aljet/alrjet:alrjet>>hDJesTopCentAw",mcMatAna.DJ,"goff");

  // === Final Jes Plots ===
  CPlot::sOutDir = outdir+"/"+mcMatAna.AnaTag;
  TCanvas * cDJesTopCent = new TCanvas("cDJesTopCent","cDJesTopCent",500,500);
  CPlot cpDJesTopCent("DJesTopCent","DJ JES","p_{T}^{gen jet}","p_{T}^{reco jet}/p_{T}^{gen jet}");
  cpDJesTopCent.SetYRange(0,1.2);
  cpDJesTopCent.AddProfile(hDJesTopCentNr,"Lead Jet","E",kRed,kOpenCircle);
  cpDJesTopCent.AddProfile(hDJesTopCentAw,"Away Jet","E",kBlue,kOpenCircle);
  cpDJesTopCent.SetLegendHeader("30\% Central");
  cpDJesTopCent.SetLegend(0.57,0.23,0.83,0.34);
  cpDJesTopCent.Draw(cDJesTopCent,true);
}
