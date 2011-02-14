#include <iostream>
#include <TH1D.h>
#include <TTree.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>
#include <TProfile.h>

void plotMETVsAJ(char *infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root",TCut myCut = "cent<30")
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("djcalo/djTree");

   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.5,4,8,20,1000};  
   double colors[nBin] = {kGray,kBlue-3,38,kOrange-8,kRed-6};

   // Selection cut
   TCut evtCut = "nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>3.14159265358979/3*2";


   TCanvas *c = new TCanvas("c","",600,600);
   TProfile *p[nBin];

   for (int i=0;i<nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",10,0,0.5);
      p[i]->SetXTitle("A_{J}");
      p[i]->SetYTitle("MET_{#parallel}");
      t->Project(Form("p%d",i),Form("Sum$(-ppt*sin(pndphi)*(ppt>%.1f)):(nljet-aljet)/(nljet+aljet)",bins[i]), evtCut&&myCut);
      p[i]->SetLineColor(colors[i]);     
      p[i]->SetMarkerColor(colors[i]);     
      p[i]->SetFillColor(colors[i]);  
   }

   p[nBin-1]->SetAxisRange(-60,60,"Y");
   p[nBin-1]->Draw("hist");
   for (int i=nBin-2;i>=0;i--) {
      p[i]->Draw("hist same");
   }


}
