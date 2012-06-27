#include <iostream>
#include <string>
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
#include "TSystem.h"
#include "commonUtility.h"
#include "DrawTick.C"
#include "HisMpt.h"
#include "loopMpt.h"


bool doResCorr=false;

void balanceMetVsAj(TString infname,
                    TString insrc, TString metType = "",bool drawLegend = false,
                    bool drawSys = true
                    )
{
   // ===========================================================
   // Get Input
   // ===========================================================
   TFile *inf = new TFile(infname);
   
   // ===========================================================
   // Analysis Setup
   // ===========================================================
   Int_t plotLayer=10; // 0 only >0.5, 1 >0.5 and highpt, 10 ~ all
   
   cout << infname << " " << insrc << endl;
   
   TH1D *ppos[nptrange+1];
   TH1D *pneg[nptrange+1];
   TH1D *pe[nptrange+1];

   // =================================
   // Get Weighted Mean for each Aj bin
   // =================================
   // Book histograms
   for (int i=0;i<nptrange+1;i++) {
      pe[i]=new TH1D(Form("p%d",i),"",nAjBin,AjBins);
   }
   
   TTree * tm = (TTree*)inf->Get("t"+insrc);
   // Find cone boundary
   int coneidr=0;
   for (int idr=0; idr<ndrbin; ++idr) {
      if (drbins[idr]>=0.8)  {
         coneidr=idr;
         break;
      }
   }
   
   // Get Values
   for (int a=0; a<nAjBin; ++a) {
      float sum=0, sumerr=0;
      for (int i=0;i<nptrange+1;i++) {
//          hname = Form("%s_merge%d_%s",insrc.Data(),i,metType.Data());
         TString hname = "hMpt"+insrc;
         if (i<nptrange) hname+=Form("_pt%d",i);
         else hname+=Form("_ptall");
//          TH2D * hMptAj = (TH2D*)inf->Get(hname);
//          cout << hname << " " << hMptAj << endl;
//          TH1D * hMpt = hMptAj->ProjectionY(hname+Form("_a%d",a),a+1,a+1);
         hname+=Form("_a%d",a);
         TString cut = Form("Aj>=%.3f&&Aj<%.3f",AjBins[a],AjBins[a+1]);
         TH1D * hMpt = new TH1D(hname,cut,1600,-800,800);
         // Define Observable: limited dR
         TString var;
         int idrbeg,idrend;
         if (metType=="InCone") {
            idrbeg=0; idrend=coneidr;
         } else {
            idrbeg=coneidr; idrend=ndrbin;
         }
         if (i<nptrange) {
            var  = Form("xptdr[%d][%d]",i,idrbeg);
            for (int idr=idrbeg+1; idr<idrend; ++idr) {
               var+= Form("+xptdr[%d][%d]",i,idr);
            }
         } else {
            var = Form("Sum$(xptdr[][%d])",idrbeg);
            for (int idr=idrbeg+1; idr<idrend; ++idr) {
               var+= Form("+Sum$(xptdr[][%d])",idr);
            }
         }
         // Now make histograms
         if (a==0) cout << metType << ": " << var << endl;
         tm->Project(hname,var,cut);
         float mpt = hMpt->GetMean();
         float mpterr = hMpt->GetRMS()/sqrt(hMpt->GetEntries());
         if (doResCorr) {
            if (i==nptrange) {
               mpt-=(pe[nptrange-4]->GetBinContent(a+1)*0.2);
               mpt-=(pe[nptrange-3]->GetBinContent(a+1)*0.2);
               mpt-=(pe[nptrange-2]->GetBinContent(a+1)*0.2);
               mpt-=(pe[nptrange-1]->GetBinContent(a+1)*0.2);
            } else if (i>=nptrange-4) {
               mpt*=0.8;
            }
         }
         pe[i]->SetBinContent(a+1,mpt);
         pe[i]->SetBinError(a+1,mpterr);
//          cout << hMpt->GetName() << ": " << hMpt->GetEntries() << " mean: " << pe[i]->GetBinContent(a+1) << " err: " << pe[i]->GetBinError(a+1) << endl;
         if (i<nptrange) {
            sum+=mpt;
            sumerr+=pow(mpterr,2);
         }
      }      
      cout << "Aj " << a << " pt sum: " << sum << endl;
   }
   
   StackHistograms(nptrange,pe,ppos,pneg,nAjBin);
   
   TH1D *pall=pe[nptrange];
   
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
   pall->SetMarkerColor(kBlack);
   pall->SetMarkerStyle(kFullCircle);
   pall->Draw("E");
   float addSys = 0;
   if ( drawSys==1)   addSys=0; // No sys error at this moment
   
   // ====================
   // Finally Draw
   // ====================
   for (int i=0;i<nptrange;++i) {
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nptrange-1) continue;
      ppos[i]->SetLineWidth(1);
      ppos[i]->SetFillStyle(1001);
      ppos[i]->Draw("hist same");
      pneg[i]->SetLineWidth(1);
      pneg[i]->SetFillStyle(1001);
      pneg[i]->Draw("hist same");
//      PrintHistogram(ppos[i]);
//      PrintHistogram(pneg[i]);
   }
   
   // ====================
   // Draw Statistical Error bars
   // ====================
   for (int i=0;i<nptrange;++i) {
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nptrange-1) continue;
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
//   PrintHistogram(pall);
   
   // ====================
   // Draw Systematic Errors
   // ====================
   if (drawSys == 1) {
      for(int i = 0; i < nAjBin; ++i){
         double x = pall->GetBinCenter(i+1);
         double y = pall->GetBinContent(i+1);
         // Quote the difference between GEN and RECO in >8 Bin (20%) before adjusting eff as systematics
         double errReco = -pe[nptrange-1]->GetBinContent(i+1)*0.2;
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
   leg->AddEntry(pall,Form("> %.1f GeV/c",ptranges[0]),"p");
   for (int i=0;i<nptrange;++i) {
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nptrange-1) continue;
      if (i!=nptrange-1){
         leg->AddEntry(ppos[i],Form("%.1f - %.1f GeV/c",ptranges[i],ptranges[i+1]),"f");
      } else {
         leg->AddEntry(ppos[i],Form("> %.1f GeV/c",ptranges[i]),"f");
      }
   }
   
   if (drawLegend) leg->Draw();
   
   TLine * l0 = new TLine(0,0,0.5,0);
   l0->SetLineStyle(2);
   l0->Draw();
   
   
   TLine * l1 = new TLine(0.0001,-10,0.0001,10);
   l1->Draw();
}

void MptAjPlotInConeOutCone(
)
{
   TH1::SetDefaultSumw2();

//    TString inputFile_mc="fig/06.21HIN_loop/HisMc_icPu5_trkHPCorr_120_50_2094_eta24.root";
//    TString inputFile_data="fig/06.21HIN_loop/HisData_icPu5_trkHPCorr_120_50_2094_eta24.root";
//    TString inputFile_mc="fig/06.25_simtrkloop_olderforest/HisMc_icPu5_trkHPCorr_120_50_2094_eta24_prec1.root";
//    TString inputFile_data="fig/06.25_simtrkloop_olderforest/HisData_icPu5_trkHPCorr_120_50_2094_eta24_prec1.root";
//    TString inputFile_mc="fig/06.25HIN_loop/HisMc_icPu5_trkHPCorr_120_50_2094_eta24.root";
//    TString inputFile_data="fig/06.25HIN_loop/HisData_icPu5_trkHPCorr_120_50_2094_eta24.root";
//    TString inputFile_mc="fig/06.26_genploop/HisMc_icPu5_trkHPCorr_120_50_2094_eta24_prec0.root";
//    TString inputFile_data="fig/06.26_genploop/HisData_icPu5_trkHPCorr_120_50_2094_eta24_prec4.root";
//    TString inputFile_mc="fig/06.26_genploop/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec0.root";
//    TString inputFile_data="fig/06.26_genploop/HisData_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";
   TString inputFile_mc="fig/06.26_tree/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";
   TString inputFile_data="fig/06.26_tree/HisData_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";

   string path=inputFile_data.Data();
   TString outdir = path.substr(0, path.find_last_of('/'));
   cout << "Output: " << outdir << endl;
   gSystem->mkdir(outdir,kTRUE);

   float coneSize = 0.8;
   TString tag = Form("CorrRes%d-data-mcRec",doResCorr);

   Float_t leftMargin=0.28,bottomMargin=0.18;
   TCanvas *c1 = new TCanvas("c1","",1000,1000);
   makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);
//    TCanvas *c1 = new TCanvas("c1","",1000,500);
//    makeMultiPanelCanvas(c1,2,1,0.0,0.0,leftMargin,bottomMargin,0.02);

   c1->cd(1);
   balanceMetVsAj(inputFile_mc,"0to12","InCone",false,false);
   drawText("PYTHIA+HYDJET 0-30%",0.33,0.82);
   drawText("In-Cone",0.85,0.9);
   drawText(Form("#DeltaR<%.1f",coneSize),0.85,0.9-0.06);
//    drawText("(a)",0.31,0.91);
   gPad->RedrawAxis();
   
   c1->cd(2);
   balanceMetVsAj(inputFile_mc,"0to12","OutCone",true,false);
   drawText("Out-of-Cone",0.7,0.9);
   drawText(Form("#DeltaR#geq%.1f",coneSize),0.7,0.9-0.06);
   drawText("(b)",0.04,0.91);
   float ptx(0.08),pty1(0.22);
   drawText("p_{T,1}  > 120GeV/c",ptx,pty1);
   drawText("p_{T,2}  > 50GeV/c",ptx,pty1-0.07);
   drawText("#Delta#phi_{1,2}>  #frac{7}{8}#pi",ptx,pty1-0.14);
//    drawText("#Delta#phi_{1,2}>  #frac{5}{6}#pi",ptx,pty1-0.14);
   drawText("|#eta_{1,2}| < 1.6",ptx+0.20,pty1-0.14);
   gPad->RedrawAxis();
   
   c1->cd(3);
   balanceMetVsAj(inputFile_data,"0to12","InCone",false,false);
//    float ptx(0.33),pty1(0.35);
//    drawText("p_{T,1}  > 120GeV/c",ptx,pty1);
//    drawText("p_{T,2}  > 50GeV/c",ptx,pty1-0.07);
//    drawText("#Delta#phi_{1,2}>  #frac{2}{3}#pi",ptx,pty1-0.14);
   drawText("CMS 0-30%",0.33,0.90);
   drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.33,0.84);
   drawText("#intL dt = 150 #mub^{-1}",0.33,0.78);
   drawText("In-Cone",0.85,0.93);
   drawText(Form("#DeltaR<%.1f",coneSize),0.85,0.93-0.06);
//    drawText("(c)",0.31,0.95);
   gPad->RedrawAxis();
   
   c1->cd(4);
   balanceMetVsAj(inputFile_data,"0to12","OutCone",false,false);
   drawText("Out-of-Cone",0.7,0.93);
   drawText(Form("#DeltaR#geq%.1f",coneSize),0.7,0.93-0.06);
//    drawText("(d)",0.04,0.95);
   gPad->RedrawAxis();
   
   c1->SaveAs(Form("%s/Mpt_InConeOutCone%.0f_%s.pdf",outdir.Data(),coneSize*10,tag.Data()));
   c1->SaveAs(Form("%s/Mpt_InConeOutCone%.0f_%s.gif",outdir.Data(),coneSize*10,tag.Data()));
}
