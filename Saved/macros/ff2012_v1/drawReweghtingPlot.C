#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>


void drawReweghtingPlot() {
  
  TFile* inf1 = new TFile("ppSmearingHistory.root");
  TH1D* pp[5] ;
  TH1D* pbpb[5] ;
  TH1D* ratio[5] ;
  for ( int icent=1; icent<=4 ; icent++) {
    pp[icent] = (TH1D*)inf1->Get(Form("hjetPt_pp_leadingJet_icent%d_fit1",icent));
    pbpb[icent] = (TH1D*)inf1->Get(Form("hjetPt_hi_leadingJet_icent%d",icent));
    ratio[icent] = (TH1D*)inf1->Get(Form("ratio_leadJet_icent%d",icent));
  }

  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.2,0.15,0.02);

  TLegend* l2  =  new  TLegend(0.03183659,0.5514706,0.9316217,0.8526261,NULL,"brNDC");
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(5-icent);
    
    handsomeTH1(pp[icent],1);
    handsomeTH1(pbpb[icent],1);
    pp[icent]->SetMarkerStyle(24);
    pp[icent]->SetAxisRange(100,300,"X");
    pp[icent]->SetAxisRange(1.1e-5,1,"Y");
    pp[icent]->SetNdivisions(505);
    fixedFontHist(pp[icent],2,2.5);
    pp[icent]->Draw();
    pbpb[icent]->Draw("same");
    gPad->SetLogy();
    if ( icent == 4) {
      drawText("CMS Preliminary",0.25,0.89,1,20);
      TLegend* l1 = new TLegend(0.4189189,0.5708075,1,0.8099379,NULL,"brNDC");
      easyLeg(l1,"|#eta_{jet}| < 2");
      l1->AddEntry(pp[icent],"pp","pl");
      l1->AddEntry(pbpb[icent],"PbPb","pl");
      l1->Draw();
    }
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent]-1;
    if ( icent ==4 ) 
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.3,0.22,1);
    else
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.15,0.22,1);

  }
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(9-icent);
    handsomeTH1(ratio[icent],1);
    ratio[icent]->SetMarkerStyle(20);
    ratio[icent]->SetAxisRange(100,300,"X");
    ratio[icent]->SetAxisRange(0,2.1,"Y");
    ratio[icent]->SetNdivisions(505);
    fixedFontHist(ratio[icent],2,2.5);
    ratio[icent]->Draw();
   if ( icent >1)
    drawPatch(0.90,0.05,1.01,0.147,     0,1001,"NDC");
  if ( icent <4)
    drawPatch(0.0,0.05,0.1,0.147,     0,1001,"NDC");
  }

  c1->SaveAs("plotsOfDijetFF/pp_pbpb_spectrum_comparison_v3.pdf");
}
