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
#include "commonUtility.h"



void balanceMetVsAj(char *infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root",TCut myCut = "cent<30", char *title = "",bool drawLegend = false)
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("djcalo/djTree");

   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.5,4,8,20,1000};  
   double colors[nBin] = {kGray,kBlue-3,38,kOrange-8,kRed-6};

   // Selection cut
   TCut evtCut = "nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2./3*TMath::Pi()";
   t->SetAlias("Aj","(nljet-aljet)/(nljet+aljet)");
   cout << "Sel Evt: " << t->GetEntries(evtCut&&myCut) << endl;

   TProfile *p[nBin];

   for (int i=0;i<nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",5,0.0001,0.49999);
      t->Project(Form("p%d",i),Form("-Sum$(ppt*cos(pndphi)*(ppt>=%.1f&&ppt<%.1f&&abs(peta)<2.5)):Aj",bins[i],bins[i+1]), evtCut&&myCut);
      p[i]->SetLineColor(colors[i]);     
      p[i]->SetMarkerColor(colors[i]);     
      //p[i]->SetFillColor(colors[i]);  
      //p[i]->SetFillStyle(3001);
   }

   TProfile *pall = new TProfile("pall","",5,0.0001,0.49999);
   pall->SetXTitle("A_{J}");
   pall->SetYTitle("<#slash{p}_{T}^{#parallel}> (GeV/c)");
   pall->GetXaxis()->CenterTitle();
   pall->GetYaxis()->CenterTitle();
   pall->SetNdivisions(505);
   t->Project("pall",Form("-Sum$(ppt*cos(pndphi)*(ppt>=%.1f&&ppt<%.1f&&abs(peta)<2.5)):Aj",bins[0],bins[nBin]), evtCut&&myCut);
   pall->SetAxisRange(-30,30,"Y");
   pall->SetMarkerSize(1);
   pall->Draw("E");
   for (int i=0;i<nBin;++i) {
      p[i]->SetLineWidth(3);
      p[i]->Draw("hist same");
   }
   pall->Draw("E same");

   // Legend
   TLegend *leg = new TLegend(0.10,0.68,0.70,0.96);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);
   leg->AddEntry(pall,Form("> %.1f GeV/c",bins[0]),"pl");
   for (int i=0;i<nBin;++i) {
      if (i!=nBin-1){
         leg->AddEntry(p[i],Form("%.1f - %.1f GeV/c",bins[i],bins[i+1]),"l");
      } else {
         leg->AddEntry(p[i],Form("> %.1f GeV/c",bins[i]),"l");
      }
   }

   if (drawLegend) leg->Draw();

   TLine * l0 = new TLine(0,0,0.5,0);
   l0->SetLineStyle(2);
   l0->Draw();

   TText *titleText = new TText(0.3,30,title);
   titleText->Draw();
}

void balanceMetVsAjAllCent()
{
   TCanvas *c1 = new TCanvas("c1","",1200,600);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,1,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj("dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root","cent>=30","",false);
   drawText("CMS",0.43,0.90);
   drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.43,0.82);
   drawText("#intL dt = 6.7 #mub^{-1}",0.43,0.7);
   drawText("30-100%",0.85,0.9);
   c1->cd(2);
   balanceMetVsAj("dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root","cent<30","",true);
   drawText("0-30%",0.8,0.9);
}
