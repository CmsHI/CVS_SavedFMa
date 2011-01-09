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
#include "DrawTick.C"


void balanceMetVsAj(TString infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_120_50.root",
                    TCut myCut = "cent<30", char *title = "",bool drawLegend = false,
                    bool drawSys = true
                   )
{
   // ===========================================================
   // Get Input
   // ===========================================================
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("ntjt");
   t->SetAlias("metxMerged0","(metx0+metx1+metx2+metx3)");
   t->SetAlias("metxMerged1","(metx1+metx2+metx3)");
   t->SetAlias("metxMerged2","(metx2+metx3)");
   t->SetAlias("metxMerged3","(metx3)");
   t->SetAlias("metxMerged4","(metx4+metx5)");

   // ===========================================================
   // Analysis Setup
   // ===========================================================
   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.0,1.5,4,8,1000};  
   double colors[nBin] = {kRed-3,38, kOrange-8,kGreen-8,kRed-7};

   const int nBinAj = 4;
   double ajBins[nBinAj+1] = {0.0001,0.11,0.22,0.33,0.49999};
   // Selection cut
   TCut evtCut = "nljet>120&&abs(nljetacorr)<2&&aljet>50&&abs(aljetacorr)<2&&jdphi>2./3*TMath::Pi()&&!maskEvt";

   cout << "Sel evt: " << t->GetEntries(evtCut&&myCut) << endl;

   // ===========================================================
   // Draw Profiles
   // ===========================================================
   TProfile *p[nBin];

   for (int i=0;i<nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",nBinAj,ajBins);
      p[i]->Sumw2();
      t->Project(Form("p%d",i),Form("-1*metxMerged%d:Aj",i), "weight"*(evtCut&&myCut));
      p[i]->SetLineColor(1);     
      p[i]->SetMarkerColor(colors[i]);
      p[i]->SetFillColor(colors[i]);
      p[i]->SetFillStyle(1001);
     
   }

   TProfile *pall = new TProfile("pall","",nBinAj,ajBins);
   pall->Sumw2();
   pall->SetXTitle("A_{J}");
   pall->SetYTitle("<#slash{p}_{T}^{#parallel}> (GeV/c)");
   pall->GetXaxis()->CenterTitle();
   pall->GetYaxis()->CenterTitle();
   pall->GetXaxis()->SetLabelSize(22);
   pall->GetXaxis()->SetLabelFont(43);
   pall->GetXaxis()->SetTitleSize(24);
   pall->GetXaxis()->SetTitleFont(43);
   pall->GetYaxis()->SetLabelSize(22);
   pall->GetYaxis()->SetLabelFont(43);
   pall->GetYaxis()->SetTitleSize(24);
   pall->GetYaxis()->SetTitleFont(43);
   pall->GetXaxis()->SetTitleOffset(1.8);
   pall->GetYaxis()->SetTitleOffset(2.4);
   pall->SetNdivisions(505);

   t->Project("pall","-1*metx:Aj", "weight"*(evtCut&&myCut));
   pall->SetAxisRange(-59.9,59.9,"Y");
   pall->SetMarkerSize(1);
   pall->Draw("E");
   for (int i=0;i<nBin;++i) {
      p[i]->SetLineWidth(1);
      p[i]->SetMarkerStyle(0);
      p[i]->Draw("hist E same");
   }
   pall->Draw("E same");

   // ===========================================================
   // Draw Systematic Error
   // ===========================================================
   if (drawSys == 1) {
      for(int i = 0; i < nBinAj; ++i){
        double x = pall->GetBinCenter(i+1);
        double y = pall->GetBinContent(i+1);
        // Quote the difference between GEN and RECO in >8 Bin (20%) before adjusting eff as systematics
        double err = -p[nBin-1]->GetBinContent(i+1)*0.2;
        DrawTick(y,err,err,x,1,0.02,1);
      }
   }

   // ===========================================================
   // Draw Legend
   // ===========================================================
   TLegend *leg = new TLegend(0.10,0.68,0.70,0.96);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);
   leg->AddEntry(pall,Form("> %.1f GeV/c",bins[0]),"pl");
   for (int i=0;i<nBin;++i) {
      if (i!=nBin-1){
         leg->AddEntry(p[i],Form("%.1f - %.1f GeV/c",bins[i],bins[i+1]),"f");
      } else {
         leg->AddEntry(p[i],Form("> %.1f GeV/c",bins[i]),"f");
      }
   }
   if (drawLegend) leg->Draw();

   // ===========================================================
   // Annotations
   // ===========================================================
   TLine * l0 = new TLine(0,0,0.5,0);
   l0->SetLineStyle(2);
   l0->Draw();

   TLine * l1 = new TLine(0.0001,-10,0.0001,10);
   l1->Draw();

   TText *titleText = new TText(0.3,30,title);
   titleText->Draw();
}

void MetPlotAllCent5Bin_prof(char *inputFile="data.root")
{
   TCanvas *c1 = new TCanvas("c1","",1000,1000);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent>=30","",false,false);
   drawText("PYTHIA+HYDJET",0.33,0.82);
   drawText("30-100%",0.85,0.9);
   float ptx(0.32),pty(0.25);
   drawText("p_{T,1}  > 120GeV/c",ptx,pty);
   drawText("p_{T,2}  > 50GeV/c",ptx,pty-0.08);
   drawText("#Delta#phi_{12}>  #frac{2}{3}#pi",ptx,pty-0.16);
   
   c1->cd(2);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent<30","",true,false);
   drawText("0-30%",0.8,0.9);

   c1->cd(3);
   balanceMetVsAj("nt_dj_data100_cor.root","cent>=30","",false);
   drawText("CMS",0.33,0.90);
   drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.33,0.84);
   drawText("#intL dt = 6.7 #mub^{-1}",0.33,0.78);
   drawText("30-100%",0.85,0.93);
   float pty1(0.4);
   drawText("p_{T,1}  > 120GeV/c",ptx,pty1);
   drawText("p_{T,2}  > 50GeV/c",ptx,pty1-0.07);
   drawText("#Delta#phi_{1,2}>  #frac{2}{3}#pi",ptx,pty1-0.14);


   c1->cd(4);
   balanceMetVsAj("nt_dj_data100_cor.root","cent<30","",false);
   drawText("0-30%",0.8,0.93);
   
   c1->SaveAs("missingPtParallel-Corrected-data-allCent.eps");

}


