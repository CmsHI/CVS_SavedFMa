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
#include "analysis/root/macros/histogram/HisGroup.h"
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
  /*
  selectionCut mcMatAna(doMC,11,80,120,70);
  */
  selectionCut mcMatAna(doMC,11,120,170,100);
  selectionCut mcMatAnaLoose(doMC,11,80,200,70);
  selectionCut mcSelRefMat(doMC,111,120,170,100);
  selectionCut mcSelRefLeadRef(doMC,1011,120,170,100);

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  aliases_dijet(mcj2t3);
  inFile0->GetObject("dijetAna_mc_periph/djTree",mcj2t3peri);
  aliases_dijet(mcj2t3peri);

  cout << "DJ selection: " << TString(mcMatAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcMatAna.DJ) << endl;
  cout << "dijetAna_mc_periph/mcj2t3 # entries: " << mcj2t3peri->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3peri->GetEntries(mcMatAna.DJ) << endl;

  TFile * outfile = new TFile("plots/anajes.root","RECREATE");

  //  === Main Ana: DiJet jes ===
  TProfile * hJes = new TProfile("hJes","JES profile",20,mcMatAna.nrJetPtMin,mcMatAna.nrJetPtMax);

  // Centrality 0 to 30%
  TProfile * hDJesTopCentNr = (TProfile*)hJes->Clone("hDJesTopCentNr");
  mcj2t3->Draw("nljet/nlrjet:nlrjet>>hDJesTopCentNr",mcMatAna.DJ,"goff");
  TProfile * hDJesTopCentAw = (TProfile*)hDJesTopCentNr->Clone("hDJesTopCentAw");
  mcj2t3->Draw("aljet/alrjet:alrjet>>hDJesTopCentAw",mcMatAna.DJ,"goff");

  TProfile * hDJesTopCentLooseNr = (TProfile*)hJes->Clone("hDJesTopCentLooseNr");
  mcj2t3->Draw("nljet/nlrjet:nlrjet>>hDJesTopCentLooseNr",mcMatAnaLoose.DJ,"goff");
  TProfile * hDJesTopCentLooseAw = (TProfile*)hDJesTopCentLooseNr->Clone("hDJesTopCentLooseAw");
  mcj2t3->Draw("aljet/alrjet:alrjet>>hDJesTopCentLooseAw",mcMatAnaLoose.DJ,"goff");

  // Centrality 60 to 90%
  TProfile * hDJesPeriphNr = (TProfile*)hJes->Clone("hDJesPeriphNr");
  mcj2t3peri->Draw("nljet/nlrjet:nlrjet>>hDJesPeriphNr",mcMatAna.DJ,"goff");

  TProfile * hDJesPeriphAw = (TProfile*)hDJesPeriphNr->Clone("hDJesPeriphAw");
  mcj2t3peri->Draw("aljet/alrjet:alrjet>>hDJesPeriphAw",mcMatAna.DJ,"goff");

  // === Histograms ===
  HisGroup hgDJesTopCent("hgDJesTopCent");
  hgDJesTopCent.Add(hDJesTopCentNr);
  hgDJesTopCent.Add(hDJesTopCentAw);
  hgDJesTopCent.Sum();
  HisGroup hgDJesTopCentLoose("hgDJesTopCentLoose");
  hgDJesTopCentLoose.Add(hDJesTopCentLooseNr);
  hgDJesTopCentLoose.Add(hDJesTopCentLooseAw);
  hgDJesTopCentLoose.Sum();

  // === Final Jes Plots ===
  CPlot::sOutDir = outdir+"/"+mcMatAna.AnaTag+"/jes";

  TCanvas * cLooseSelDJes = new TCanvas("cLooseSelDJes","cLooseSelDJes",500,500);
  CPlot cpLooseSelDJes("LooseSelDJes","DJ JES","p_{T}^{genjet}","p_{T}^{calojet}/p_{T}^{genjet}");
  cpLooseSelDJes.SetYRange(0,1.2);
  cpLooseSelDJes.AddProfile(hJes,"Centrality: 0-30\%","P",0,0);
  cpLooseSelDJes.AddProfile(hDJesTopCentLooseNr,"Lead calojet","E",kRed,kFullCircle);
  cpLooseSelDJes.AddProfile(hDJesTopCentLooseAw,"Away calojet","E",kBlue,kFullCircle);
  cpLooseSelDJes.AddHist1D(hgDJesTopCentLoose.hSum_,"calojet1,2","E",kBlack,kOpenSquare);
  cpLooseSelDJes.SetLegend(0.57,0.23,0.83,0.44);
  cpLooseSelDJes.SetLegendHeader("80GeV<p_{T}^{calojet1}<200GeV");
  cpLooseSelDJes.Draw(cLooseSelDJes,true);

  TCanvas * cAnaSelDJes = new TCanvas("cAnaSelDJes","cAnaSelDJes",500,500);
  CPlot cpAnaSelDJes("AnaSelDJes","DJ JES","p_{T}^{genjet}","p_{T}^{calojet}/p_{T}^{genjet}");
  cpAnaSelDJes.SetYRange(0,1.2);
  cpAnaSelDJes.AddProfile(hJes,"Centrality: 0-30\%","P",0,0);
  cpAnaSelDJes.AddProfile(hDJesTopCentNr,"Lead calojet","E",kRed,kFullCircle);
  cpAnaSelDJes.AddProfile(hDJesTopCentAw,"Away calojet","E",kBlue,kFullCircle);
  cpAnaSelDJes.AddHist1D(hgDJesTopCent.hSum_,"calojet1,2","E",kBlack,kOpenSquare);
  cpAnaSelDJes.AddProfile(hJes,"Centrality: 60-90\%","P",0,0);
  cpAnaSelDJes.AddProfile(hDJesPeriphNr,"Lead Jet","Ehist",kRed-7,0);
  cpAnaSelDJes.AddProfile(hDJesPeriphAw,"Away Jet","Ehist",kBlue-7,0);
  cpAnaSelDJes.SetLegend(0.57,0.23,0.83,0.44);
  cpAnaSelDJes.SetLegendHeader("120GeV<p_{T}^{calojet1}<170GeV");
  cpAnaSelDJes.Draw(cAnaSelDJes,true);

  TCanvas * cDJesComp = new TCanvas("cDJesComp","cDJesComp",500,500);
  CPlot cpDJesComp("DJesComp","DJ JES","p_{T}^{genjet}","p_{T}^{calojet}/p_{T}^{genjet}");
  cpDJesComp.SetYRange(0,1.2);
  cpDJesComp.AddProfile(hJes,"Centrality: 0-30\%","P",0,0);
  cpDJesComp.AddHist1D(hgDJesTopCent.hSum_,"120GeV<p_{T}^{calojet1}<170GeV","E",kRed,kFullCircle);
  cpDJesComp.AddHist1D(hgDJesTopCentLoose.hSum_,"80GeV<p_{T}^{calojet1}<200GeV","E",kBlue,kOpenSquare);
  cpDJesComp.SetLegend(0.57,0.23,0.83,0.44);
  cpDJesComp.SetLegendHeader("calojet1,2");
  cpDJesComp.Draw(cDJesComp,true);

  TCanvas * cDJesFinal = new TCanvas("cDJesFinal","cDJesFinal",500,500);
  CPlot cpDJesFinal("DJesFinal","DJ JES","p_{T}^{genjet}","p_{T}^{calojet}/p_{T}^{genjet}");
  cpDJesFinal.SetYRange(0,1.2);
  cpDJesFinal.AddProfile(hJes,"Centrality: 0-30\%","P",0,0);
  cpDJesFinal.AddHist1D(hgDJesTopCent.hSum_,"120GeV<p_{T}^{calojet1}<170GeV","E",kRed,kFullCircle);
  cpDJesFinal.SetLegend(0.57,0.23,0.83,0.44);
  cpDJesFinal.SetLegendHeader("calojet1,2");
  cpDJesFinal.Draw(cDJesFinal,true);
}
