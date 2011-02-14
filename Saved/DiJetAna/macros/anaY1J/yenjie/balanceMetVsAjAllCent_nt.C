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



void balanceMetVsAj(TString infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_120_50.root",TCut myCut = "cent<30", char *title = "",bool drawLegend = false)
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("ntjt");

   const int nBin = 3;
   double bins[nBin+1] = {0.5,4,8,1000};  
   double colors[nBin] = {kOrange-8,kBlue-3,kRed};

   // Selection cut
   TCut evtCut = "nljet>100&&abs(nljetacorr)<2&&aljet>50&&abs(aljetacorr)<2&&jdphi>2./3*TMath::Pi()&&!maskEvt";

   cout << "Sel evt: " << t->GetEntries(evtCut&&myCut) << endl;

   TProfile *p[nBin];

   for (int i=0;i<nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",5,0.0001,0.49999);
      t->Project(Form("p%d",i),Form("-1*metxMerged%d:Aj",i), "1"*(evtCut&&myCut));
      p[i]->SetLineColor(colors[i]);     
      p[i]->SetMarkerColor(colors[i]);     
   }

   TProfile *pall = new TProfile("pall","",5,0.0001,0.49999);
   pall->SetXTitle("A_{J}");
   pall->SetYTitle("<#slash{p}_{T}^{#parallel}> (GeV/c)");
   pall->GetXaxis()->CenterTitle();
   pall->GetYaxis()->CenterTitle();
   pall->SetNdivisions(505);
   t->Project("pall","-1*metx:Aj", "1"*(evtCut&&myCut));
   pall->SetAxisRange(-50,50,"Y");
   pall->SetMarkerSize(1);
   pall->Draw("E");
   for (int i=0;i<nBin;++i) {
      p[i]->SetLineWidth(3);
      p[i]->Draw("e hist same");
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

void balanceMetVsAjAllCent_nt(
    TString infile="dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_djcalo_100_50.root"
    )
{
   TCanvas *c1 = new TCanvas("c1","",1200,600);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,1,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj(infile,"cent>=30","",false);
   drawText("CMS",0.43,0.90);
//   drawText("PbPb  #sqrt{s}_{_{NN}}=2.76 TeV",0.43,0.82);
   drawText("PYTHIA+HYDJET",0.43,0.82);
   drawText("#intL dt = 6.7 #mub^{-1}",0.43,0.7);
   drawText("30-100%",0.85,0.9);
   c1->cd(2);
   balanceMetVsAj(infile,"cent<30","",true);
   drawText("0-30%",0.8,0.9);
}
