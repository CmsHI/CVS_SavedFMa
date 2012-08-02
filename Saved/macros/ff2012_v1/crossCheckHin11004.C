#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>


void crossCheckHin11004() { 
  
  TFile* inf1 = new TFile("dijetFF_output_histograms_trackPtCut4.root");
  TH1D* lff1[5] ;
  TH1D* sff1[5] ;
  TH1D* lff1pp[5] ;
  TH1D* sff1pp[5] ; 
  for ( int iaj=1; iaj<=4 ; iaj++) {
    lff1[iaj] = (TH1D*)inf1->Get(Form("hpt_lJet_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0",iaj));
    sff1[iaj] = (TH1D*)inf1->Get(Form("hpt_slJet_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0",iaj));
    lff1pp[iaj] = (TH1D*)inf1->Get(Form("hpt_lJet_sigTrk_ppdata_icent999_iaj%d_fragMode2_closure0",iaj));
    sff1pp[iaj] = (TH1D*)inf1->Get(Form("hpt_slJet_sigTrk_ppdata_icent999_iaj%d_fragMode2_closure0",iaj));
    
    lff1[iaj]->Divide(lff1pp[iaj]);
    sff1[iaj]->Divide(sff1pp[iaj]);
  }


  TFile* inf2 = new TFile("hin-11-004/hin-11-004-hists.root");
  TH1D* lff2_[5] ;
  TH1D* sff2_[5] ;
  TH1D* lff2[5] ;
  TH1D* sff2[5] ;
  for ( int iaj=1; iaj<=4 ; iaj++) {
    lff2_[iaj] = (TH1D*)inf2->Get(Form("xsia%dR_s",iaj-1));
    sff2_[iaj] = (TH1D*)inf2->Get(Form("xsia%dR_l",iaj-1));
    
    lff2[iaj] = (TH1D*)lff1[iaj]->Clone(Form("lff2_%s",lff2_[iaj]->GetName())); 
    lff2[iaj]->Reset();
    sff2[iaj] = (TH1D*)sff1[iaj]->Clone(Form("sff2_%s",sff2_[iaj]->GetName()));
    sff2[iaj]->Reset();    
    for ( int j=1; j<=14 ; j++) {
      lff2[iaj]->SetBinContent(j, lff2_[iaj]->GetBinContent( j+2));
      lff2[iaj]->SetBinError(j, lff2_[iaj]->GetBinError( j+2));
      sff2[iaj]->SetBinContent(j, sff2_[iaj]->GetBinContent( j+2));
      sff2[iaj]->SetBinError(j, sff2_[iaj]->GetBinError( j+2));
    }
  }
  
  /////////////////////////// DRAW!! //////////////////

  double ajBin1[5] = {0,0.13,0.24,0.35,100};
  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.2,0.15,0.02);

  TLegend* l2  =  new  TLegend(0.03183659,0.5514706,0.9316217,0.8526261,NULL,"brNDC");
  for ( int iaj=1; iaj<=4 ; iaj++) {
    c1->cd(iaj);
    //  scaleInt(lff1[iaj]);
    //  scaleInt(lff2[iaj]);
    handsomeTH1(lff1[iaj],1);
    handsomeTH1(lff2[iaj],2);
    lff1[iaj]->SetAxisRange(0.00001,3,"Y");
    lff1[iaj]->SetAxisRange(0,4.5,"X");
    lff1[iaj]->SetYTitle("PbPb/pp");
    fixedFontHist(lff1[iaj],1.3,1.8);
    lff1[iaj]->SetNdivisions(505);
    lff1[iaj]->DrawCopy();
    lff1[iaj]->SetXTitle("#xi = ln(1/z)");
    lff2[iaj]->DrawCopy("same");
    if ( iaj == 1) {
      drawText("Leading jet",0.3,0.65,1,20);
      drawText("CMS Preliminary",0.3,0.85,1,20);
    }
    
    if ( iaj == 1)
      drawText(Form("%.2f < A_{j} < %.2f", float(ajBin1[iaj-1]), float(ajBin1[iaj])), 0.3,0.1,1,22);
    else if ( (iaj == 2) ||(iaj==3)) 
      drawText(Form("%.2f < A_{j} < %.2f", float(ajBin1[iaj-1]), float(ajBin1[iaj])), 0.1,0.1,1,22);
    else 
      drawText(Form("%.2f < A_{j}", float(ajBin1[iaj-1])), 0.1,0.1,1,22);

    jumSun(0,1,6.5,1);
    if ( iaj == 2) {
      easyLeg(l2,"PbPb/pp");
      l2->AddEntry(lff2[iaj],"2010 (HIN-11-004)");
      l2->AddEntry(lff1[iaj],"2011");
      l2->Draw();
    }
  }
  for ( int iaj=1; iaj<=4 ; iaj++) {
    c1->cd(iaj+4);
    // scaleInt(sff1[iaj]);
    //  scaleInt(sff2[iaj]);
    handsomeTH1(sff1[iaj],1);
    handsomeTH1(sff2[iaj],2);
    
    sff1[iaj]->SetAxisRange(0.00001,3,"Y");
    sff1[iaj]->SetXTitle("PbPb/pp");
    sff1[iaj]->SetAxisRange(0,4.5,"X");
    sff1[iaj]->SetNdivisions(505);
    fixedFontHist(sff1[iaj],1.8,1.5);
    sff1[iaj]->DrawCopy();
    sff2[iaj]->DrawCopy("same");
    if ( iaj == 1 ) drawText("Subleading jet",0.3,0.65,1,20);
    jumSun(0,1,6.5,1);
    if ( iaj <4) 
     drawPatch(0.90,0.05,1.01,0.147,     0,1001,"NDC");
    if ( iaj >1) 
     drawPatch(0.0,0.05,0.1,0.147,     0,1001,"NDC");

  }
  /*
  for ( int iaj=1; iaj<=4 ; iaj++) {
    c1->cd(iaj+12);
    sff1[iaj]->Divide(sff2[iaj]);
    sff1[iaj]->SetAxisRange(0.,2,"Y");
    sff1[iaj]->Draw();
    jumSun(0,1,7,1);
  }
  */

  c1->SaveAs("plotsOfDijetFF//jetFF_comparison2011_2010_raw.pdf");
  
  
}
