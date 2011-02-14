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

   TString infnament(infname); infnament.ReplaceAll(".root","_Gen.root");
   cout <<infnament<<endl;
   TFile *inf2 = new TFile(infnament);
   TTree *tGen = (TTree*)inf2->Get("ntjt");
  
   t->SetAlias("metxMerged0","metx0");
   t->SetAlias("metxMerged1","metx1");
   t->SetAlias("metxMerged2","metx2");
   t->SetAlias("metxMerged3","metx3");
   t->SetAlias("metxMerged4","metx4+metx5");

   tGen->SetAlias("metxMerged0","metx0");
   tGen->SetAlias("metxMerged1","metx1");
   tGen->SetAlias("metxMerged2","metx2");
   tGen->SetAlias("metxMerged3","metx3");
   tGen->SetAlias("metxMerged4","metx4+metx5");

   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.0,1.5,4,8,1000};  
   int colors[5] = {kBlue-10,kYellow-7, kOrange-2,kGreen-5,kRed-3};

   // Selection cut
   TCut evtCut = "nljet>120&&abs(nljetacorr)<2&&aljet>50&&abs(aljetacorr)<2&&jdphi>2./3*TMath::Pi()&&!maskEvt";

   cout << "Sel evt: " << t->GetEntries(evtCut&&myCut) << endl;

   TProfile *p[nBin];
   TProfile *pGen[nBin];

   for (int i=0;i<nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",5,0.0001,0.49999);
      t->Project(Form("p%d",i),Form("-1*metxMerged%d:Aj",i), "1"*(evtCut&&myCut));
      p[i]->SetLineColor(colors[i]);     
      p[i]->SetMarkerColor(colors[i]);     
      pGen[i] = new TProfile(Form("pGen%d",i),"",5,0.0001,0.49999);
      tGen->Project(Form("pGen%d",i),Form("-1*metxMerged%d:Aj",i), "1"*(evtCut&&myCut));
      pGen[i]->SetLineColor(colors[i]);     
      pGen[i]->SetMarkerColor(colors[i]);     
   }

   TProfile *pall = new TProfile("pall","",5,0.0001,0.49999);
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
   TProfile *pallGen = new TProfile("pallGen","",5,0.0001,0.49999);
   pallGen->SetXTitle("A_{J}");
   pallGen->SetYTitle("<#slash{p}_{T}^{#parallel}> (GeV/c)");
   pallGen->GetXaxis()->CenterTitle();
   pallGen->GetYaxis()->CenterTitle();
   pallGen->GetXaxis()->SetLabelSize(22);
   pallGen->GetXaxis()->SetLabelFont(43);
   pallGen->GetXaxis()->SetTitleSize(24);
   pallGen->GetXaxis()->SetTitleFont(43);
   pallGen->GetYaxis()->SetLabelSize(22);
   pallGen->GetYaxis()->SetLabelFont(43);
   pallGen->GetYaxis()->SetTitleSize(24);
   pallGen->GetYaxis()->SetTitleFont(43);
   pallGen->GetXaxis()->SetTitleOffset(1.8);
   pallGen->GetYaxis()->SetTitleOffset(2.4);



   pall->SetNdivisions(505);
   t->Project("pall","-1*metx:Aj", "1"*(evtCut&&myCut));
   tGen->Project("pallGen","-1*metx:Aj", "1"*(evtCut&&myCut));
   pall->SetAxisRange(-39.9,39.9,"Y");
   pall->SetMarkerSize(1);
   pall->Draw("E");
   for (int i=0;i<nBin;++i) {
      pGen[i]->SetLineWidth(3);
      pGen[i]->SetLineStyle(2);
      pGen[i]->SetMarkerSize(0);
      pGen[i]->Draw("e hist same");
      p[i]->SetLineWidth(3);
      p[i]->SetMarkerSize(0);
      p[i]->Draw("e hist same");
   }

   pall->Draw("E same");
   pallGen->SetMarkerStyle(4);
   pallGen->Draw("E same");

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


void MetPlotAllCentCompareGen(char *inputFile="data.root")
{
   TCanvas *c1 = new TCanvas("c1","",1000,500);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,1,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj("nt_dj_mix100.root","cent>=30","",false);
   drawText("HYDJET+PYTHIA",0.43,0.82);
   drawText("30-100%",0.85,0.9);
   c1->cd(2);
   balanceMetVsAj("nt_dj_mix100.root","cent<30","",true);
   drawText("0-30%",0.8,0.9);

/*
   c1->cd(3);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent>=30","",false);
   drawText("CMS",0.43,0.90);
   drawText("HYDJET+PYTHIA",0.43,0.82);
//   drawText("#intL dt = 6.7 #mub^{-1}",0.43,0.74);
   drawText("30-100%",0.85,0.9);
   c1->cd(4);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent<30","",false);
   drawText("0-30%",0.8,0.9);
*/
}
