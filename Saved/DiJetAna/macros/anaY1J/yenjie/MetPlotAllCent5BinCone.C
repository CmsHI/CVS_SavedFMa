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


   t->SetAlias("metxMergedAll","(metConex0+metConex1+metConex2+metConex3+metConex4+metConex5)");
   t->SetAlias("metxMerged0","(metConex0+metConex1+metConex2+metConex3)");
   t->SetAlias("metxMerged1","(metConex1+metConex2+metConex3)");
   t->SetAlias("metxMerged2","(metConex2+metConex3)");
   t->SetAlias("metxMerged3","(metConex3)");
   t->SetAlias("metxMerged4","(metConex4+metConex5)");

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
   // Find Average Weights
   // ===========================================================
   TH1D *hw[nBinAj];
   float meanWt[nBinAj];
   for ( int iaj = 0 ; iaj< nBinAj ; iaj++) {
      hw[iaj] = new TH1D(Form("hw_aj%d",iaj),"",1000,0,100);
      TCut ajCut = Form("Aj>%f && Aj<%f", ajBins[iaj],ajBins[iaj+1]);
      t->Draw(Form("weight>>hw_aj%d",iaj), evtCut&&myCut&&ajCut);
      meanWt[iaj] = hw[iaj]->GetMean();
      cout << " <Weight>: " << meanWt[iaj] << endl;
   }

   // ===========================================================
   // Draw Weighted Averages
   // ===========================================================
   TH1D *p[nBin];
   TH1D *pe[nBin];
   for (int i=0;i<nBin;i++)
   {
      TH1D *h1 = new TH1D(Form("h1%d",i),"",nBinAj,ajBins);
      TH1D *h2 = new TH1D(Form("h2%d",i),"",nBinAj,ajBins);
      TH1D *he[nBinAj];
      
      // =================================
      // Get Weighted Mean for each Aj bin
      // =================================
      h1->Sumw2();
      h2->Sumw2();
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

      // =================================
      // Caculated Stat Error of the Mean
      // =================================
      cout << "Stat Error for pt bin " << i << ": ";
      for ( int iaj = 0 ; iaj< nBinAj ; iaj++) {
         he[iaj] = new TH1D(Form("he%d_aj%d",i,iaj),"",100,-200,200);
         TCut ajCut = Form("Aj>%f && Aj<%f", ajBins[iaj],ajBins[iaj+1]);
         t->Draw(Form("((metxMerged%d))>>he%d_aj%d",i,i,iaj), "weight" * evtCut&&myCut&&ajCut);
         float theError = he[iaj]->GetRMS()/ (sqrt(he[iaj]->GetEntries()));
	 cout << theError << " ";
	 p[i]->SetBinError(iaj+1, theError);
      }
      cout << endl;
   }
   // correct the error bars..  cummulative -> it's own.
   subtractError(p[0],p[1]);
   subtractError(p[1],p[2]);
   subtractError(p[2],p[3]);
   
   TH1D *pall;
   TH1D *pallE;
   TH1D *h1 = new TH1D(Form("hAll1"),"",nBinAj,ajBins);
   TH1D *h2 = new TH1D(Form("hAll2"),"",nBinAj,ajBins);

   h1->Sumw2();
   h2->Sumw2();
   t->Draw(Form("Aj>>hAll1"), "weight"*(evtCut&&myCut));
   //   t->Draw(Form("Aj>>hAll2"), Form("((-weight*metx))")*(evtCut&&myCut));
   t->Draw(Form("Aj>>hAll2"), Form("((-weight*metxMergedAll))")*(evtCut&&myCut));
   pall=(TH1D*)h2->Clone();
   pall->SetName("pall");
   pall->Divide(h1);

   // replace the sys error from pallE to pall 
   TH1D *he[nBinAj];
   cout << "Stat Error for All pt: ";
   for ( int iaj = 0 ; iaj< nBinAj ; iaj++) {
      he[iaj] = new TH1D(Form("heAll_aj%d",iaj),"",100,-200,200);
      TCut ajCut = Form("Aj>%f && Aj<%f", ajBins[iaj],ajBins[iaj+1]);
      t->Draw(Form("((metxMergedAll))>>heAll_aj%d",iaj), "weight" * evtCut&&myCut&&ajCut);
      float theError = he[iaj]->GetRMS()/ (sqrt(he[iaj]->GetEntries()));
      cout << theError << " ";
      pall->SetBinError(iaj+1,  theError);
   }
   cout << endl;

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
   float addSys = 0;
   if ( drawSys==1)   addSys=0; // No sys error at this moment

   // ====================
   // Finally Draw
   // ====================
   for (int i=0;i<nBin;++i) {
      p[i]->SetLineWidth(1);
      //      p[i]->SetMarkerSize(0.1);
      p[i]->Draw("hist same");
   }
   
   // ====================
   // Draw Statistical Error bars
   // ====================
   for (int i=0;i<nBin;++i) {
      if ( i==0 )       drawErrorShift(p[i],-0.016, addSys);
      if ( i==1 || i==4)       drawErrorShift(p[i],-0.008,addSys);
      if ( i==2 )       drawErrorShift(p[i],0.008,addSys);
      if ( i==3 )       drawErrorShift(p[i],0.016,addSys);
   }
   pall->Draw("E same");
   
   // ====================
   // Draw Systematic Errors
   // ====================
   if (drawSys == 1) {
      for(int i = 0; i < nBinAj; ++i){
        double x = pall->GetBinCenter(i+1);
        double y = pall->GetBinContent(i+1);
        // Quote the difference between GEN and RECO in >8 Bin (20%) before adjusting eff as systematics
        double err = fabs(p[nBin-1]->GetBinContent(i+1)*0.2);
        //DrawTick(y,err,err,x,1,0.02,1);
      }
   }

   // ====================
   // Draw Legend
   // ====================
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


void MetPlotAllCent5BinCone(char *inputFile="data.root")
{
   TCanvas *c1 = new TCanvas("c1","",1000,1000);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent>=30","",false,false);
//   balanceMetVsAj("nt_dj_PYTHIA_Gen.root","cent>=30","",false,false);
//   balanceMetVsAj("nt_dj_mix120_Gen_frank.root","cent>=30","",false,false);
   //balanceMetVsAj("nt_dj_mix100_Gen_yj.root","cent>=30","",false,false);
   drawText("PYTHIA+HYDJET",0.33,0.82);

   drawText("30-100%",0.85,0.9);
   float ptx(0.32),pty(0.25);
   drawText("p_{T,1}  > 120GeV/c",ptx,pty);
   drawText("p_{T,2}  > 50GeV/c",ptx,pty-0.08);
   drawText("#Delta#phi_{12}>  #frac{2}{3}#pi",ptx,pty-0.16);
   
   c1->cd(2);
   balanceMetVsAj("nt_dj_mix100_Gen.root","cent<30","",true,false);
//   balanceMetVsAj("nt_dj_PYTHIA_Gen.root","cent<30","",true,false);
//   balanceMetVsAj("nt_dj_mix120_Gen_frank.root","cent<30","",true,false);
   //balanceMetVsAj("nt_dj_mix100_Gen_yj.root","cent<30","",true,false);
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

