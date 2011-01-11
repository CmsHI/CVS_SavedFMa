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

void subtractError(TH1* h1, TH1* h2)   {
   int nBins = h1->GetNbinsX();
   for ( int i = 1 ; i<=nBins ; i++ ) { 
      float oldErr = h1->GetBinError(i);
      float Err2   = h2->GetBinError(i);
      float newErr = sqrt ( oldErr*oldErr - Err2*Err2) ;
      h1->SetBinError(i,newErr);
   }
}
      
void drawErrorShift(TH1* hOld=0, float dx=0, float addSysErr=0.1) {
   int nBins = hOld->GetNbinsX();
   TLine* tl[100];
   for ( int i =1; i<=nBins ; i ++ ) { 
      float px = hOld->GetBinCenter(i);
      float py = hOld->GetBinContent(i);
      float tErrSta = hOld->GetBinError(i);
      float tErrSys = addSysErr*py;
      float tErr = sqrt( tErrSta*tErrSta + tErrSys*tErrSys);
      tl[i] = new TLine(px+dx,py-tErr, px+dx,py+tErr);
      tl[i]->SetLineWidth(1);
      tl[i]->Draw();
   }
}

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
   t->AddFriend("tcone");

   // ==========================================================
   // Alias for Cone Subtraction
   // ==========================================================
   // For the sake of cross check we don't do bkg subtraction to see
   // if we can get similar results than with bkg subtraction
   bool doBkgSub = false;
   if (doBkgSub) {
     // Case 1 Do Background subtraction
     // Note
     // * for this case we need to exclude jets from certain eta region
     // * For the final sum we start from ptbin1 to compare to Rocket in paper
     t->SetAlias("cpt0bgsubMerge0","(Sum$(cpt[0])-cpt[0][0])-(Sum$(cptbg[0])-cptbg[0][0])");
     t->SetAlias("cpt1bgsubMerge0","(Sum$(cpt[1])-cpt[1][0])-(Sum$(cptbg[1])-cptbg[1][0])");
     t->SetAlias("cpt0bgsubMerge1","(cpt[0][0]+cpt[0][1]+cpt[0][2]+cpt[0][3])-(cptbg[0][0]+cptbg[0][1]+cptbg[0][2]+cptbg[0][3])");
     t->SetAlias("cpt1bgsubMerge1","(cpt[1][0]+cpt[1][1]+cpt[1][2]+cpt[1][3])-(cptbg[1][0]+cptbg[1][1]+cptbg[1][2]+cptbg[1][3])");
     t->SetAlias("cpt0bgsubMerge2","(cpt[0][1]+cpt[0][2]+cpt[0][3])-(cptbg[0][1]+cptbg[0][2]+cptbg[0][3])");
     t->SetAlias("cpt1bgsubMerge2","(cpt[1][1]+cpt[1][2]+cpt[1][3])-(cptbg[1][1]+cptbg[1][2]+cptbg[1][3])");
     t->SetAlias("cpt0bgsubMerge3","(cpt[0][2]+cpt[0][3])-(cptbg[0][2]+cptbg[0][3])");
     t->SetAlias("cpt1bgsubMerge3","(cpt[1][2]+cpt[1][3])-(cptbg[1][2]+cptbg[1][3])");
     t->SetAlias("cpt0bgsubMerge4","(cpt[0][3])-(cptbg[0][3])");
     t->SetAlias("cpt1bgsubMerge4","(cpt[1][3])-(cptbg[1][3])");
     t->SetAlias("cpt0bgsubMerge5","(cpt[0][4]+cpt[0][5])-(cptbg[0][4]+cptbg[0][5])");
     t->SetAlias("cpt1bgsubMerge5","(cpt[1][4]+cpt[1][5])-(cptbg[1][4]+cptbg[1][5])");
     t->SetAlias("metxMerged0","(cpt0bgsubMerge0-cpt1bgsubMerge0)");
     t->SetAlias("metxMerged1","(cpt0bgsubMerge1-cpt1bgsubMerge1)");
     t->SetAlias("metxMerged2","(cpt0bgsubMerge2-cpt1bgsubMerge2)");
     t->SetAlias("metxMerged3","(cpt0bgsubMerge3-cpt1bgsubMerge3)");
     t->SetAlias("metxMerged4","(cpt0bgsubMerge4-cpt1bgsubMerge4)");
     t->SetAlias("metxMerged5","(cpt0bgsubMerge5-cpt1bgsubMerge5)");
   }
   else {
     // Case 2 Direct Cone subtraction --- no Bkg sub applied, this is closer to MPT
     // Note
     // * For the final sum we start from ptbin1 to compare to Rocket in paper
     // * op should be "+" if using cptpara, b/c there cos has been multiplied
     //   and so the sign is already there.
     TString op("-");
     t->SetAlias("metxMerged0","(Sum$(cpt[0])-cpt[0][0])"+op+"(Sum$(cpt[1])-cpt[1][0])");
     t->SetAlias("metxMerged1","((cpt[0][0]+cpt[0][1]+cpt[0][2]+cpt[0][3])"+op+"(cpt[1][0]+cpt[1][1]+cpt[1][2]+cpt[1][3]))");
     t->SetAlias("metxMerged2","((cpt[0][1]+cpt[0][2]+cpt[0][3])"+op+"(cpt[1][1]+cpt[1][2]+cpt[1][3]))");
     t->SetAlias("metxMerged3","((cpt[0][2]+cpt[0][3])"+op+"(cpt[1][2]+cpt[1][3]))");
     t->SetAlias("metxMerged4","((cpt[0][3])"+op+"(cpt[1][3]))");
     t->SetAlias("metxMerged5","((cpt[0][4]+cpt[0][5])"+op+"(cpt[1][4]+cpt[1][5]))");
   }

   // ===========================================================
   // Analysis Setup
   // ===========================================================
   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.0,1.5,4,8,1000};  
   double colors[nBin+1] = {kBlack, kRed-3,38, kOrange-8,kGreen-8,kRed-7};

   const int nBinAj = 4;
   double ajBins[nBinAj+1] = {0.0001,0.11,0.22,0.33,0.49999};
   //
   // Selection cut
   //
   TCut evtCut = "nljet>120&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&jdphi>2./3*TMath::Pi()&&!maskEvt";
   // Require at least one jet to have |eta|>0.8 to better match with paper
   TCut exclCut = "abs(nljetacorr)>0.8||abs(aljetacorr)>0.8";
   // Need to apply both jet exclusion cut if doing bkg subtraction
   //exclCut = "abs(nljetacorr)>0.8&&abs(aljetacorr)>0.8";
   evtCut = evtCut&&exclCut;

   cout << "Sel evt: " << t->GetEntries(evtCut&&myCut) << endl;

   // ===========================================================
   // Draw Profiles
   // ===========================================================
   TProfile *p[nBin+1];
   for (int i=0;i<=nBin;i++)
   {
      p[i] = new TProfile(Form("p%d",i),"",nBinAj,ajBins);
      p[i]->Sumw2();
      t->Project(Form("p%d",i),Form("-1*metxMerged%d:Aj",i), "weight"*(evtCut&&myCut));
      if (i==0) continue;
      p[i]->SetLineColor(1); 
      p[i]->SetLineWidth(1);
      p[i]->SetMarkerStyle(0);
      p[i]->SetMarkerColor(colors[i]);
      p[i]->SetFillColor(colors[i]);
      p[i]->SetFillStyle(1001);
     
   }
   p[0]->SetXTitle("A_{J}");
   p[0]->SetYTitle("<#slash{p}_{T}> (GeV/c)");
   p[0]->GetXaxis()->CenterTitle();
   p[0]->GetYaxis()->CenterTitle();
   p[0]->GetXaxis()->SetLabelSize(22);
   p[0]->GetXaxis()->SetLabelFont(43);
   p[0]->GetXaxis()->SetTitleSize(24);
   p[0]->GetXaxis()->SetTitleFont(43);
   p[0]->GetYaxis()->SetLabelSize(22);
   p[0]->GetYaxis()->SetLabelFont(43);
   p[0]->GetYaxis()->SetTitleSize(24);
   p[0]->GetYaxis()->SetTitleFont(43);
   p[0]->GetXaxis()->SetTitleOffset(1.8);
   p[0]->GetYaxis()->SetTitleOffset(2.4);
   p[0]->SetNdivisions(505);
   p[0]->SetAxisRange(-59.9,59.9,"Y");
   p[0]->SetMarkerSize(1);
   p[0]->Draw("E");
   for (int i=1;i<=nBin;++i) {
      //if (i>1) continue;
      p[i]->Draw("hist same");
     
   }
   p[0]->Draw("E same");

   // correct the error bars..  cummulative -> it's own.
   subtractError(p[1],p[2]);
   subtractError(p[2],p[3]);
   subtractError(p[3],p[4]);

   // ====================
   // Draw Statistical Error bars
   // ====================
   bool addSys = 0;
   for (int i=1;i<=nBin;++i) {
      if ( i==1 || i==4)       drawErrorShift(p[i],-0.008,addSys);
      if ( i==2 )       drawErrorShift(p[i],0.008,addSys);
      if ( i==3 )       drawErrorShift(p[i],0.016,addSys);
      if ( i==5)       drawErrorShift(p[i],-0.016,addSys);
   }
  

   // ===========================================================
   // Draw Systematic Error
   // ===========================================================
   if (drawSys == 1) {
      for(int i = 0; i < nBinAj; ++i){
        double x = p[0]->GetBinCenter(i+1);
        double y = p[0]->GetBinContent(i+1);
        // Quote the difference between GEN and RECO in >8 Bin (20%) before adjusting eff as systematics
        double err = -p[nBin]->GetBinContent(i+1)*0.2;
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
   leg->AddEntry(p[0],Form("> %.1f GeV/c",bins[1]),"pl");
   TH1D *hh = new TH1D("hh","",100,0,1);
   hh->SetMarkerStyle(4);
   leg->AddEntry(hh,Form("> %.1f GeV/c Rocket Plot",bins[1]),"pl");
   for (int i=0;i<nBin;++i) {
      if (i!=nBin-1){
         leg->AddEntry(p[i+1],Form("%.1f - %.1f GeV/c",bins[i],bins[i+1]),"f");
      } else {
         leg->AddEntry(p[i+1],Form("> %.1f GeV/c",bins[i]),"f");
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

void MetConePlotAllCent5Bin_prof(char *inputFile="data.root")
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

   const int nBinAj = 4;
   double ajBins[nBinAj+1] = {0.0001+0.016,0.11+0.016,0.22+0.016,0.33+0.016,0.49999+0.016};
   TH1D *h2 = new TH1D("h2","",nBinAj,ajBins);
   h2->SetBinContent(1 , -1.31 );
   h2->SetBinError(1 , 2.94362 );
   h2->SetBinContent(2 , -8.06 );
   h2->SetBinError(2 , 2.90401 );
   h2->SetBinContent(3 , -23.31 );
   h2->SetBinError(3 , 3.94974 );
   h2->SetBinContent(4 , -22.01 );
   h2->SetBinError(4 , 4.75925 );
   h2->SetMarkerStyle(4);
   h2->Draw("same");



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
   
   TH1D *h = new TH1D("h","",nBinAj,ajBins);
   h->SetBinContent(1 , -9.33 );
   h->SetBinError(1 , 4.51263 );
   h->SetBinContent(2 , 0.01 );
   h->SetBinError(2 , 3.59884 );
   h->SetBinContent(3 , -13.01 );
   h->SetBinError(3 , 3.22952 );
   h->SetBinContent(4 , -31.17 );
   h->SetBinError(4 , 3.01733 );

   h->SetMarkerStyle(4);
   h->Draw("sameE");
   c1->SaveAs("missingPtCone-Corrected-data-allCent.eps");
   c1->SaveAs("missingPtCone-Corrected-data-allCent.gif");
}


