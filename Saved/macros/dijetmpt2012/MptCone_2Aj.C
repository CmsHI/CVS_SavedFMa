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
#include "HisMpt.h"

void balanceMetVsAj(TString infname,
                    TString insrc, TString metType,TString anaBin, bool drawLegend = false,
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
   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.0,2,4,8,1000};  
   
   const int nBinAj = 4;
   double ajBins[nBinAj+1] = {0,0.4,0.8,1.2,1.6};
   
   cout << infname << endl;
   
   TH1D *ppos[nBin+1];
   TH1D *pneg[nBin+1];
   TH1D *pe[nBin+1];
   // =================================
   // Get Weighted Mean for each Aj bin
   // =================================
   for (int i=0;i<nBin+1;i++)
   {
      pe[i]=new TH1D(Form("p%d",i),"",nBinAj,ajBins);
      for (int a=0; a<nBinAj; ++a) {
         TString hname = Form("%s%s%.0f_merge%d_%s",insrc.Data(),metType.Data(),(a+1)*4.,i,anaBin.Data());
         if (a==3) hname = Form("%sAllAcc_merge%d_%s",insrc.Data(),i,anaBin.Data());
         TH1D * hMpt = (TH1D*)inf->Get(hname);
         cout << hname << " Aj bin: " << a+1 << " mean: " << hMpt->GetMean() << endl;
         pe[i]->SetBinContent(a+1,hMpt->GetMean());
         pe[i]->SetBinError(a+1,hMpt->GetRMS()/sqrt(hMpt->GetEntries()));
         if (a==3&&metType.Contains("Out")) {
            pe[i]->SetBinContent(a+1,0);
            pe[i]->SetBinError(a+1,0);
         }
      }      
   }
   
   StackHistograms(nBin,pe,ppos,pneg,nBinAj);
   
   TH1D *pall=pe[nBin];
   
   pall->SetXTitle("R");
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
   for (int i=0;i<nBin;++i) {
      if (plotLayer==0) continue;
      if (plotLayer==1&&i!=nBin-1) continue;
      ppos[i]->SetLineWidth(1);
      ppos[i]->Draw("hist same");
      pneg[i]->SetLineWidth(1);
      pneg[i]->Draw("hist same");
//      PrintHistogram(ppos[i]);
//      PrintHistogram(pneg[i]);
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
//   PrintHistogram(pall);
   
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
   //   leg->AddEntry(pall,Form("> %.1f GeV/c",bins[0]),"p");
   leg->AddEntry(pall,Form("> %.1f GeV/c",bins[1]),"p");
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

void MptCone_2Aj()
{
   TString inputFile_central="fig/04.18_data/HisOutput_DiJetv7_v4_icPu5_Mpt_120_50_2749_Norm0_c0_12.root";
   TString inputFile_peripheral="fig/04.18_data/HisOutput_DiJetv7_v4_icPu5_Mpt_120_50_2749_Norm0_c12_40.root";
//   TString inputFile_Py="fig/04.18_pythia_reco_gen/HisOutput_DiJetv7_v4_icPu5_Mpt_120_50_2618_Norm0_c0_40.root";
   TString inputFile_Py="fig/04.18_pythia_reco_gen/HisOutput_DiJetv7_v4_icPu5_Mpt_120_50_2749_Norm0_c0_40.root";
   
   TCanvas *c1 = new TCanvas("c1","",1000,1000);
   Float_t leftMargin=0.28,bottomMargin=0.18;
   makeMultiPanelCanvas(c1,2,2,0.0,0.0,leftMargin,bottomMargin,0.02);
   c1->cd(1);
//   balanceMetVsAj(inputFile_Py,"hypho_mptxgenpSig","InCone","0SigAll",false,false);
   balanceMetVsAj(inputFile_Py,"hypho_mptxtrkCorr","InCone","0SigAll",false,false);
//   drawText("PYTHIA+HYDJET",0.33,0.82);
   drawText("Aj<0.11",0.85,0.9);
   drawText("PYTHIA Gen. Signal",0.33,0.82);
   drawText("(a)",0.31,0.91);
   float ptx(0.08+0.3),pty1(0.22);
   drawText("p_{T,1}  > 120GeV/c",ptx,pty1);
   drawText("p_{T,2}  > 50GeV/c",ptx,pty1-0.07);
   drawText("#Delta#phi_{1,2}>  #frac{7}{8}#pi",ptx,pty1-0.14);
   drawText("|#eta_{1,2}| < 1.6",ptx+0.20,pty1-0.14);
   gPad->RedrawAxis();
   
   c1->cd(2);
//   balanceMetVsAj(inputFile_Py,"hypho_mptxgenpSig","InCone","3SigAll",false,false);
   balanceMetVsAj(inputFile_Py,"hypho_mptxtrkCorr","InCone","3SigAll",false,false);
   drawText("Aj>0.33",0.7,0.9);
   drawText("(b)",0.04,0.91);
   gPad->RedrawAxis();
   
   c1->cd(3);
   TCut phiSysCut;
   balanceMetVsAj(inputFile_central,"hi_mptxtrkCorr","InCone","0SigAll",false);
   drawText("CMS",0.33,0.90);
   drawText("Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV",0.33,0.84);
   drawText("#intL dt = 6.7 #mub^{-1}",0.33,0.78);
   drawText(" Aj<0.11",0.85,0.93);
   drawText("(c)",0.31,0.95);
   gPad->RedrawAxis();
   
   c1->cd(4);
   balanceMetVsAj(inputFile_central,"hi_mptxtrkCorr","InCone","3SigAll",false);
   drawText("Aj>0.33",0.7,0.93);
   drawText("(d)",0.04,0.95);
   gPad->RedrawAxis();
   
   c1->SaveAs(Form("missingPtParallel-Corrected-data-mcrec-InConeSize.pdf"));
   c1->SaveAs(Form("missingPtParallel-Corrected-data-mcrec-InConeSize.gif"));
}
