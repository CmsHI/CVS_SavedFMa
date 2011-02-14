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
#include <TText.h>

void balanceMetVsAj(char *infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root",TCut myCut = "cent<30", char *title = "")
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("djcalo/djTree");

   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.5,4,8,20,1000};  
   double colors[nBin] = {kGray,kBlue-3,38,kOrange-8,kRed-6};

   // Selection cut
   TCut evtCut = "nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2./3*TMath::Pi()";
   t->SetAlias("Aj","(nljet-aljet)/(nljet+aljet)");

   TCanvas *c = new TCanvas("c","",600,600);
   TProfile *p[nBin];

   for (int i=0;i<nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",10,0,0.5);
      p[i]->SetXTitle("A_{J}");
      p[i]->SetYTitle("MET_{#parallel}");
      t->Project(Form("p%d",i),Form("Sum$(ppt*cos(pndphi)*(ppt>=%.1f&&ppt<%.1f&&abs(peta)<2.5)):Aj",bins[i],bins[i+1]), evtCut&&myCut);
      p[i]->SetLineColor(colors[i]);     
      p[i]->SetMarkerColor(colors[i]);     
      //p[i]->SetFillColor(colors[i]);  
      //p[i]->SetFillStyle(3001);
   }

   TProfile *pall = new TProfile("pall","",10,0,0.5);
   pall->SetXTitle("A_{J}");
   pall->SetYTitle("MET_{#parallel}");
   pall->GetXaxis()->CenterTitle();
   pall->GetYaxis()->CenterTitle();
   t->Project("pall",Form("Sum$(ppt*cos(pndphi)*(ppt>=%.1f&&ppt<%.1f&&abs(peta)<2.5)):Aj",bins[0],bins[nBin]), evtCut&&myCut);
   pall->SetAxisRange(-20,40,"Y");
   pall->Draw("E");
   for (int i=0;i<nBin;++i) {
      p[i]->Draw("hist same");
   }

   // Legend
   TLegend *leg = new TLegend(0.20,0.64,0.80,0.92);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   //leg->SetTextFont(63);
   //leg->SetTextSize(16);
   leg->AddEntry(pall,Form("All %.1f+ GeV/c",bins[0]),"pl");
   for (int i=0;i<nBin;++i) {
     leg->AddEntry(p[i],Form("%.1f-%.1f GeV/c",bins[i],bins[i+1]),"l");
   }
   leg->Draw();

   TLine * l0 = new TLine(0,0,0.5,0);
   l0->SetLineStyle(2);
   l0->Draw();

   TText *titleText = new TText(0.3,30,title);
   titleText->Draw();
}
