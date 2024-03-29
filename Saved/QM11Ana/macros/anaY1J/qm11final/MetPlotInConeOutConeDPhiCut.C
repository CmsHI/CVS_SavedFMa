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
#include "../../commonUtility.h"
#include "../../DrawTick.C"
#include "../colorScheme.h"


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
                    TCut myCut = "cent<30", TString metType = "",bool drawLegend = false,
                    bool drawSys = true
		    )
{
   // ===========================================================
   // Get Input
   // ===========================================================
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("ntjt");

   vector<TString> mergedPt,target;
   mergedPt.push_back("metxMergedAll"); target.push_back("(metx0+metx1+metx2+metx3+metx4+metx5)");
   mergedPt.push_back("metxMerged0"); target.push_back("(metx0)");
   mergedPt.push_back("metxMerged1"); target.push_back("(metx1)");
   mergedPt.push_back("metxMerged2"); target.push_back("(metx2)");
   mergedPt.push_back("metxMerged3"); target.push_back("(metx3)");
   mergedPt.push_back("metxMerged4"); target.push_back("(metx4+metx5)");
   for (Int_t i=0; i<mergedPt.size(); ++i) {
     if (metType!="") {
       target[i].ReplaceAll("metx",metType);
     }
     cout << "Alias: " << mergedPt[i] << " target: " << target[i] << endl;
     t->SetAlias(mergedPt[i],target[i]);
   }
   //TString name2(infname);
   //name2.ReplaceAll("ntv2deadforpix_","");
   //name2.ReplaceAll("ntv2_","");
   //t->AddFriend("dj=djTree",name2);

   // ===========================================================
   // Analysis Setup
   // ===========================================================
   Int_t plotLayer=10; // 0 only >0.5, 1 >0.5 and highpt, 10 ~ all
   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.0,2,4,8,1000};  
   
   const int nBinAj = 4;
   double ajBins[nBinAj+1] = {0.0001,0.13,0.24,0.35,0.69999};
   // Selection cut
   TCut evtCut = "nljet>100&&abs(nljetacorr)<1.6&&aljet>40&&abs(aljetacorr)<1.6&&jdphi>5./6*TMath::Pi()&&!maskEvt";

   cout << "Cut: " << TString(evtCut) << endl;
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
   TH1D *ppos[nBin];
   TH1D *pneg[nBin];
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
      
      pe[i]=(TH1D*)h2->Clone();
      pe[i]->SetName(Form("p%d",i));     
      pe[i]->Divide(h1);
      ppos[i] = new TH1D(Form("ppos%d",i),"",nBinAj,ajBins);
      ppos[i]->SetLineColor(1);     
      ppos[i]->SetMarkerColor(colors[i]);
      ppos[i]->SetFillColor(colors[i]);
      ppos[i]->SetFillStyle(1001);
      pneg[i] = new TH1D(Form("pneg%d",i),"",nBinAj,ajBins);
      pneg[i]->SetLineColor(1);     
      pneg[i]->SetMarkerColor(colors[i]);
      pneg[i]->SetFillColor(colors[i]);
      pneg[i]->SetFillStyle(1001);

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
	 pe[i]->SetBinError(iaj+1, theError);
      }
      cout << endl;
   }

   // Stack
   for (int i=nBin-1;i>=0;i--)
   {
      for(int iaj = 0 ; iaj< nBinAj ; iaj++) {
         double posVal=0, negVal=0;
         double posValErr=0, negValErr=0;
         if (i!=nBin-1) {
            posVal = ppos[i+1]->GetBinContent(iaj+1);
            posValErr = ppos[i+1]->GetBinError(iaj+1);
            negVal = pneg[i+1]->GetBinContent(iaj+1);
            negValErr = pneg[i+1]->GetBinError(iaj+1);
         }
         if (pe[i]->GetBinContent(iaj+1)<0) {
            negVal+=pe[i]->GetBinContent(iaj+1);
            negValErr=pe[i]->GetBinError(iaj+1);
            posValErr=0;
         } else if (pe[i]->GetBinContent(iaj+1)>0) {
            posVal+=pe[i]->GetBinContent(iaj+1);
            posValErr=pe[i]->GetBinError(iaj+1);
            negValErr=0;
         } 
         ppos[i]->SetBinContent(iaj+1,posVal);
         ppos[i]->SetBinError(iaj+1,posValErr);
         pneg[i]->SetBinContent(iaj+1,negVal);
         pneg[i]->SetBinError(iaj+1,negValErr);
      }
   }
   
   TH1D *pall;
   TH1D *pallE;
   TH1D *h1 = new TH1D(Form("hAll1"),"",nBinAj,ajBins);
   TH1D *h2 = new TH1D(Form("hAll2"),"",nBinAj,ajBins);

   h1->Sumw2();
   h2->Sumw2();
   t->Draw(Form("Aj>>hAll1"), "weight"*(evtCut&&myCut));
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
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nBin-1) continue;
      ppos[i]->SetLineWidth(1);
      ppos[i]->Draw("hist same");
      pneg[i]->SetLineWidth(1);
      pneg[i]->Draw("hist same");
   }
   
   // ====================
   // Draw Statistical Error bars
   // ====================
   for (int i=0;i<nBin;++i) {
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nBin-1) continue;
      if ( i==0 )       drawErrorShift(ppos[i],-0.016, addSys);
      if ( i==1 || i==4)       drawErrorShift(ppos[i],-0.008,addSys);
      if ( i==2 )       drawErrorShift(ppos[i],0.008,addSys);
      if ( i==3 )       drawErrorShift(ppos[i],0.016,addSys);
      if ( i==0 )       drawErrorShift(pneg[i],-0.016, addSys);
      if ( i==1 || i==4)       drawErrorShift(pneg[i],-0.008,addSys);
      if ( i==2 )       drawErrorShift(pneg[i],0.008,addSys);
      if ( i==3 )       drawErrorShift(pneg[i],0.016,addSys);
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
        double errReco = -pe[nBin-1]->GetBinContent(i+1)*0.2;
	double errBck = 3.0; // compare HYDJET+SIG to SIG
	double err = sqrt(errReco*errReco+errBck*errBck);
        DrawTick(y,err,err,x,1,0.02,1);
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
   leg->AddEntry(pall,Form("> %.1f GeV/c",bins[0]),"p");
   for (int i=0;i<nBin;++i) {
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nBin-1) continue;
      if (i!=nBin-1){
         leg->AddEntry(ppos[i],Form("%.1f - %.1f GeV/c",bins[i],bins[i+1]),"f");
      } else {
         leg->AddEntry(ppos[i],Form("> %.1f GeV/c",bins[i]),"f");
      }
   }

   if (drawLegend) leg->Draw();

   TLine * l0 = new TLine(0,0,0.5,0);
   l0->SetLineStyle(2);
   l0->Draw();


   TLine * l1 = new TLine(0.0001,-10,0.0001,10);
   l1->Draw();
}

void MetPlotInConeOutConeDPhiCut(char *inputFile="data.root")
{
   TCanvas *c1 = new TCanvas("c1","",1000,1000);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
   balanceMetVsAj("../ntout/nt_tranav5_2_hydjuq80v5_akpu3pf_t0_offset0.root","cent<30","metConex",false,false);
   drawText("PYTHIA+HYDJET 0-30%",0.33,0.82);
   drawText("In-Cone",0.85,0.9);
   drawText("#DeltaR<0.8",0.85,0.9-0.06);
   drawText("(a)",0.31,0.91);
   gPad->RedrawAxis();
   
   c1->cd(2);
   balanceMetVsAj("../ntout/nt_tranav5_2_hydjuq80v5_akpu3pf_t0_offset0.root","cent<30","metOutOfConex",true,false);
   drawText("Out-of-Cone",0.7,0.9);
   drawText("#DeltaR#geq0.8",0.7,0.9-0.06);
   drawText("(b)",0.04,0.91);
   float ptx(0.08),pty1(0.22);
   drawText("p_{T,1}  > 100GeV/c",ptx,pty1);
   drawText("p_{T,2}  > 40GeV/c",ptx,pty1-0.07);
   drawText("#Delta#phi_{1,2}>  #frac{5}{6}#pi",ptx,pty1-0.14);
   drawText("|#eta_{1,2}| < 1.6",ptx+0.20,pty1-0.14);
   gPad->RedrawAxis();

   c1->cd(3);
   TCut phiSysCut;
   balanceMetVsAj("../ntout/nt_tranav5_2_datamattpfv2_akpu3pf_hiGood_offset0.root","cent<30","metConex",false);
   //balanceMetVsAj("../ntout/nt_tranav5_2_hydjuq80v5_akpu3pf_t2_offset0.root","cent<30","metConex",false);
   drawText("CMS 0-30%",0.33,0.90);
   drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.33,0.84);
   drawText("#intL dt = 6.7 #mub^{-1}",0.33,0.78);
   drawText("In-Cone",0.85,0.93);
   drawText("#DeltaR<0.8",0.85,0.93-0.06);
   drawText("(c)",0.31,0.95);
   gPad->RedrawAxis();

   c1->cd(4);
   balanceMetVsAj("../ntout/nt_tranav5_2_datamattpfv2_akpu3pf_hiGood_offset0.root","cent<30","metOutOfConex",false);
   //balanceMetVsAj("../ntout/nt_tranav5_2_hydjuq80v5_akpu3pf_t2_offset0.root","cent<30","metOutOfConex",false);
   drawText("Out-of-Cone",0.7,0.93);
   drawText("#DeltaR#geq0.8",0.7,0.93-0.06);
   drawText("(d)",0.04,0.95);
   gPad->RedrawAxis();
   
   c1->SaveAs("missingPtParallel-Corrected-data-akpu3pf-InConeOutConeDPhiCut.eps");
   c1->SaveAs("missingPtParallel-Corrected-data-akpu3pf-InConeOutConeDPhiCut.gif");
}
