#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TGraph.h>
#include "ana/savedfrankTools.C"

char * drsg = "hist";
char * drdb = "hist same";
char * drsgE = "E1";
char * drdbE = "E1 same";

void plotFinal(char * infname1 = "LeadingHistos.root",
      char * infname2 = "FFHistos.root",
      char * plotdir = ".",
      float xiRatMax = 10)
{
   TFile * outfile = new TFile(Form("%s/FinalHistos.root",plotdir),"RECREATE");
   TFile * f1 = new TFile(infname1);
   TFile * f2 = new TFile(infname2);

   TH1D * hXiNearPythia = dynamic_cast<TH1D*>(drawNormHist(f2,"hXiNearPythia",drsg,"","","",-1,0,kRed,0,0,0,0,10));
   TH1D * hXiNearPyquen = dynamic_cast<TH1D*>(drawNormHist(f2,"hXiNearPyquen",drdb,"","","",-1,0,kRed-2,0,0,0,0,10));
   TH1D * hXiNearJetPythia = dynamic_cast<TH1D*>(drawNormHist(f2,"hXiNearJetPythia",drdbE,"","","",-1,0,kBlack,0,0,0,0,10));
   TH1D * hXiNearJetPyquen = dynamic_cast<TH1D*>(drawNormHist(f2,"hXiNearJetPyquen",drdbE,"","","",-1,0,kBlue,0,0,0,0,10));

   TH1F * hPnR = dynamic_cast<TH1F*>(drawNormHist(f2,"hNearFFRatio",drsg));
   TH1F * hPaR = dynamic_cast<TH1F*>(drawNormHist(f2,"hAwayFFRatio",drdb));
   TH1F * hJnR = dynamic_cast<TH1F*>(drawNormHist(f2,"hNearJetFFRatio",drdbE));
   TH1F * hJaR = dynamic_cast<TH1F*>(drawNormHist(f2,"hAwayJetFFRatio",drdbE));

   //
   // Legend
   //
   TLegend *leg = new TLegend(0.2,0.6,0.5,0.80);
   leg->AddEntry(hPnR,"b=0","");
   leg->AddEntry(hPnR,"Leading jet E_{T} > 100GeV","");
   leg->AddEntry(hPnR,"Particle p_{T} > 500MeV","");
//   leg->AddEntry(hPnR,"","");
   leg->AddEntry(hPnR,"Near FF(parton)","L");
   leg->AddEntry(hPaR,"Away FF(parton)","L");
   leg->AddEntry(hJnR,"Near FF(genjet)","pL");
   leg->AddEntry(hJaR,"Away FF(genjet)","pL");
   leg->SetFillColor(0);
   leg->SetTextSize(0.025);
   leg->SetBorderSize(0);
   leg->Draw();

   // print canvas and save histograms
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
