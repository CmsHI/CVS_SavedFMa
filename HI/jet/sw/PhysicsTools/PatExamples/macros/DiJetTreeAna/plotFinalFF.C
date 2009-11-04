#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TGraph.h>
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

char * drsg = "hist";
char * drdb = "hist same";
char * drsgE = "E1";
char * drdbE = "E1 same";

void plotFinalFF(char * infname = "FFHistos.root",
      char * plotdir = ".",
      float xiRatMax = 10)
{
   TFile * outfile = new TFile(Form("%s/FinalHistos.root",plotdir),"RECREATE");
   TFile * infile = new TFile(infname);
   infile->Print();

   // final styles
   Size_t mksz=1.5;
   Style_t mkst= kFullDotLarge;
   Style_t lnst = 1;
   Color_t lcNear = kBlack;
   Color_t lcAway = kBlue;
   Width_t lwUncut = 1;
   Width_t lw=3;

   // Hi Event
   TH1D * hUncutB = dynamic_cast<TH1D*>(drawNormHist(infile,"hUncutB",drsg,"","","",-1,0,0,0,0,mksz,0,0));
   TH1D * hB = dynamic_cast<TH1D*>(drawNormHist(infile,"hB",drdb,"","","",-1,0,0,0,0,mksz,0,0));
   TLegend *lB = new TLegend(0.2,0.6,0.5,0.80);
   lB->AddEntry(hUncutB,"Before Jet Et cut","L");
   lB->AddEntry(hB,"After Jet Et cut","L");
   lB->SetTextSize(0.03);
   lB->Draw();

   // === single FF ===
   TH1D * hXiNearJet = dynamic_cast<TH1D*>(drawNormHist(infile,"hXiNearJet",drsgE,"","","",-1,0,0,0,0,mksz,0,0));
   TH1D * hXiAwayJet = dynamic_cast<TH1D*>(drawNormHist(infile,"hXiAwayJet",drdbE,"","","",-1,0,0,0,0,mksz,0,0));
   //  - legend -
   TLegend *lXi = new TLegend(0.2,0.6,0.5,0.80);
   lXi->AddEntry(hXiNearJet,"Near Jet","L");
   lXi->AddEntry(hXiAwayJet,"Away Jet","L");
   lXi->SetTextSize(0.03);
   lXi->Draw();

   /*
   TH1F * h = dynamic_cast<TH1F*>(findHist("hNearFFRatio"));
   h->SetTitleSize(0.04,"Y");
   h->GetYaxis()->SetTitleOffset(1.4);
   TH1F * hPnR = dynamic_cast<TH1F*>(drawNormHist(infile,"hNearFFRatio",drsg,"","","",-1,0,kRed,0,0,0,0,4));
   TH1F * hPaR = dynamic_cast<TH1F*>(drawNormHist(infile,"hAwayFFRatio",drdb,"","","",-1,0,kRed,0,0,0,0,4));
   TH1F * hJnR = dynamic_cast<TH1F*>(drawNormHist(infile,"hNearJetFFRatio",drdbE,"","","",-1,0,kBlue,0,0,0,0,4));
   TH1F * hJaR = dynamic_cast<TH1F*>(drawNormHist(infile,"hAwayJetFFRatio",drdbE,"","","",-1,0,kBlue,0,0,0,0,4));

   //
   // Legend
   //
   TLegend *leg = new TLegend(0.2,0.6,0.5,0.80);
   leg->AddEntry(hPnR,"b=0","");
   leg->AddEntry(hPnR,"Leading jet: 90 GeV <  E_{T} < 110GeV","");
   leg->AddEntry(hPnR,"Particle p_{T} > 500MeV","");
   leg->AddEntry(hPnR,"Jet |#eta| < 2.","");
//   leg->AddEntry(hPnR,"","");
   leg->AddEntry(hPnR,"Near FF(parton)","L");
   leg->AddEntry(hPaR,"Away FF(parton)","L");
   leg->AddEntry(hJnR,"Near FF(genjet)","pL");
   leg->AddEntry(hJaR,"Away FF(genjet)","pL");
   leg->SetFillColor(0);
   leg->SetTextSize(0.025);
   leg->SetBorderSize(0);
   leg->Draw();
   */

   // print canvas and save histograms
   printAllCanvases(plotdir);
   outfile->Write();
   outfile->Close();
}
