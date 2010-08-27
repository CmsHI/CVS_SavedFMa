#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                   // access to gROOT, entry point to ROOT system
#include <TStyle.h>                  // class to handle plot styles in ROOT
#include <TCanvas.h>                 // class for drawing
#include <TH1D.h>                    // 1D histograms
#include <TRandom.h>                 // random number generator
#endif

#include "CPlot.h"           // helper class for plots

void plotExample()
{
  gStyle->SetOptStat(0);  // get rid of stats box
  
  // set up histograms
  TH1D *h1 = new TH1D("h1","",100,-5,5);
  TH1D *h2 = new TH1D("h2","",100,-5,5);
  
  // fill histograms using random generators
  TRandom gen1;
  TRandom gen2;
  for(Int_t i=0; i<5000; i++) {
    h1->Fill(gen1.Gaus());
    h2->Fill(gen2.Uniform(-5,5));
  }
  
  // plot
  TCanvas *c = new TCanvas("c","c",800,600);  
  CPlot plot("plot","An example plot","x","Events");
  plot.AddHist1D(h1,"Gaus (#mu=0,#sigma=1)","",kRed);
  plot.AddHist1D(h2,"Uniform [-5,5]","",kBlue,5);
  plot.AddTextBox("5000 events",0.13,0.85,0.4,0.75,1,kBlack,kGray); 
  plot.TransLegend(-0.05,-0.05);
  plot.Draw(c,false);
}
