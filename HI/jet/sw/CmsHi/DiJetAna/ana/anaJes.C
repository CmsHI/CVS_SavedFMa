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
    //const char * inFile0Name="../process_aod/outputs/McUqDj80to120_DJes002_10k.root",
    //TString outdir = "plots/mcuq80to120_10k")
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root",
    TString outdir = "plots/mcuq120to170_10k")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  // Define dijet selection
  //selectionCut mcMatAna(doMC,11,80,120,70);
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

  // Centrality 0 to 30%
  TProfile * hDJesTopCentNr = (TProfile*)hJes->Clone("hDJesTopCentNr");
  mcj2t3->Draw("nljet/nlrjet:nlrjet>>hDJesTopCentNr",mcMatAna.DJ,"goff");

  TProfile * hDJesTopCentAw = (TProfile*)hDJesTopCentNr->Clone("hDJesTopCentAw");
  mcj2t3->Draw("aljet/alrjet:alrjet>>hDJesTopCentAw",mcMatAna.DJ,"goff");

  // Centrality 60 to 90%
  TProfile * hDJesPeriphNr = (TProfile*)hJes->Clone("hDJesPeriphNr");
  mcj2t3peri->Draw("nljet/nlrjet:nlrjet>>hDJesPeriphNr",mcMatAna.DJ,"goff");

  TProfile * hDJesPeriphAw = (TProfile*)hDJesPeriphNr->Clone("hDJesPeriphAw");
  mcj2t3peri->Draw("aljet/alrjet:alrjet>>hDJesPeriphAw",mcMatAna.DJ,"goff");

  // === Final Jes Plots ===
  CPlot::sOutDir = outdir+"/"+mcMatAna.AnaTag+"/dj";
  TCanvas * cDJesComp = new TCanvas("cDJesComp","cDJesComp",500,500);
  CPlot cpDJesComp("DJesComp","DJ JES","p_{T}^{gen jet}","p_{T}^{reco jet}/p_{T}^{gen jet}");
  cpDJesComp.SetYRange(0,1.2);
  cpDJesComp.AddProfile(hJes,"Centrality: 60-90\%","P",0,0);
  cpDJesComp.AddProfile(hDJesPeriphNr,"Lead Jet","Ehist",kRed-7,0);
  cpDJesComp.AddProfile(hDJesPeriphAw,"Away Jet","Ehist",kBlue-7,0);
  cpDJesComp.AddProfile(hJes,"Centrality: 0-30\%","P",0,0);
  cpDJesComp.AddProfile(hDJesTopCentNr,"Lead Jet","E",kRed,kFullCircle);
  cpDJesComp.AddProfile(hDJesTopCentAw,"Away Jet","E",kBlue,kFullCircle);
  cpDJesComp.SetLegend(0.57,0.23,0.83,0.44);
  cpDJesComp.Draw(cDJesComp,true);
}
