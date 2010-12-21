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



TH1D *calcEff(TH1D* h1, TH1D* h2)
{
   h2->Divide(h1);
   return h2;
}

void balanceMetVsAj(TString infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_120_50.root",
                    TCut myCut = "cent<30", char *title = "",bool drawLegend = false,
                    bool drawSys = true
                   )
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("ntjt");
  
   t->SetAlias("metxMerged0","metx0+metx1+metx2+metx3");
   t->SetAlias("metxMerged1","metx1+metx2+metx3");
   t->SetAlias("metxMerged2","metx2+metx3");
   t->SetAlias("metxMerged3","metx3");
   t->SetAlias("metxMerged4","metx4+metx5");
   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.0,1.5,4,8,1000};  
   double colors[nBin] = {kRed-6,38, kOrange-8,kBlue-3,kRed};

   const int nBinAj = 4;
   double ajBins[nBinAj+1] = {0.0001,0.11,0.22,0.33,0.49999};
   // Selection cut
   TCut evtCut = "nljet>120&&abs(nljetacorr)<2&&aljet>50&&abs(aljetacorr)<2&&jdphi>2./3*TMath::Pi()&&!maskEvt";

   cout << "Sel evt: " << t->GetEntries(evtCut&&myCut) << endl;

   TH1D *p[nBin];

   for (int i=0;i<nBin;i++)
   {
      TH1D *h1 = new TH1D(Form("h1%d",i),"",nBinAj,ajBins);
      TH1D *h2 = new TH1D(Form("h2%d",i),"",nBinAj,ajBins);
      h1->Sumw2();
      h2->Sumw2();
  //    t->Project(Form("h%d",i),"Aj", "1"*(evtCut&&myCut));
//      t->Project(Form("h2%d",i),"Aj", Form("((-1*metxMerged%d))",i)*(evtCut&&myCut));
      t->Draw(Form("Aj>>h1%d",i), "weight"*(evtCut&&myCut));
      t->Draw(Form("Aj>>h2%d",i), Form("((-weight*metxMerged%d))",i)*(evtCut&&myCut));
      p[i]=(TH1D*)h2->Clone();
      p[i]->SetName(Form("p%d",i));     
      p[i]->Divide(h1);
      p[i]->SetLineColor(1);     
      p[i]->SetMarkerColor(colors[i]);
      p[i]->SetFillColor(colors[i]);
//      p[i]->SetFillStyle(3004+fabs(i-1));
      p[i]->SetFillStyle(1001);
     
   }

   TH1D *pall;
   TH1D *h1 = new TH1D(Form("hAll1"),"",nBinAj,ajBins);
   TH1D *h2 = new TH1D(Form("hAll2"),"",nBinAj,ajBins);
   h1->Sumw2();
   h2->Sumw2();
   t->Draw(Form("Aj>>hAll1"), "weight"*(evtCut&&myCut));
   t->Draw(Form("Aj>>hAll2"), Form("((-weight*metx))")*(evtCut&&myCut));

   pall=(TH1D*)h2->Clone();
   pall->SetName("pall");
   pall->Divide(h1);
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
   pall->SetAxisRange(-59.9,59.9,"Y");
   pall->SetMarkerSize(1);
   pall->Draw("E");
   for (int i=0;i<nBin;++i) {
      p[i]->SetLineWidth(1);
      p[i]->Draw("hist same");
   }
   pall->Draw("E same");

   if (drawSys == 1) {
      for(int i = 0; i < nBinAj; ++i){
        double x = pall->GetBinCenter(i+1);
        double y = pall->GetBinContent(i+1);
        // Quote the difference between GEN and RECO in >8 Bin (20%) before adjusting eff as systematics
        double err = -p[nBin-1]->GetBinContent(i+1)*0.2;
 
        DrawTick(y,err,err,x,1,0.02,1);
      }
   }

   // Legend
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

   TLine * l0 = new TLine(0,0,0.5,0);
   l0->SetLineStyle(2);
   l0->Draw();


   TLine * l1 = new TLine(0.0001,-10,0.0001,10);
   l1->Draw();

   TText *titleText = new TText(0.3,30,title);
   titleText->Draw();
}


void MetPlotAllCent5Bin(char *inputFile="data.root")
{
   TCanvas *c1 = new TCanvas("c1","",1000,1000);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent>=30","",false,false);
   drawText("HYDJET+PYTHIA",0.43,0.82);
   drawText("30-100%",0.85,0.9);
   c1->cd(2);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent<30","",true,false);
   drawText("0-30%",0.8,0.9);

   c1->cd(3);
   balanceMetVsAj("nt_dj_data100_cor.root","cent>=30","",false);
   drawText("CMS",0.43,0.90);
   drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.43,0.84);
   drawText("#intL dt = 6.7 #mub^{-1}",0.43,0.78);
   drawText("30-100%",0.85,0.9);
   c1->cd(4);
   balanceMetVsAj("nt_dj_data100_cor.root","cent<30","",false);
   drawText("0-30%",0.8,0.9);
}
