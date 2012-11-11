#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <time.h>
#include "commonUtility.h"
#include "../forestv3/CutAndBinCollection2011.h"
using namespace std;

void drawReweghtingPlotTree() {
  TH1::SetDefaultSumw2();
  
  TTree * thi;
  TTree * vtpp[5];
  TH1D * pp[5] ;
  TH1D * pbpb[5] ;
  TH1D * ratio[5] ;

//   TFile* inf1 = new TFile("../forestv3/output_data_pp_Nov07_jet_ak3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm0.root");
  TFile* inf1 = new TFile("../forestv3/output_data_hi_Nov07_report_akPu3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm0.root");
  thi = (TTree*)inf1->Get("tgj");
  for ( int icent=1; icent<=4 ; icent++) {
    TFile* inf1;
//     if (icent!=1) inf1 = new TFile(Form("../forestv3/output_data_pp_Nov07_report_ak3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm%d.root",icent));
//     inf1 = new TFile(Form("../forestv3/output_data_pp_Nov07_smnoshift_ak3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm%d.root",icent));
    inf1 = new TFile(Form("../forestv3/output_data_pp_Nov07_smfix_ak3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm%d.root",icent));
//     TFile* inf1 = new TFile(Form("../forestv3/output_data_pp_Nov07_jet_ak3PF_0_-1_-1000_saveTrk1_jmin10_tmin1_genj0_sm0.root",icent));
    vtpp[icent] = (TTree*)inf1->Get("tgj");
  }  
  
  // Make Histograms
  TFile * outf = new TFile("histograms.root","recreate");
  for ( int icent=1; icent<=4 ; icent++) {
    pbpb[icent] = new TH1D(Form("hjetPt_hi_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",50,0,500);
    pp[icent] = new TH1D(Form("hjetPt_pp_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",50,0,500);
    ratio[icent] = new TH1D(Form("hjetPt_ratio_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",50,0,500);
  }
//   TCut jetSel = "inclJetPt>100";
  TCut jetSel = "inclJetPt>100";
  for ( int icent=1; icent<=4 ; icent++) {
    TCut centCut = Form("cBin>=%.1f&&cBin<%.1f",centBin[icent-1],centBin[icent]);
    cout << TString(centCut) << " ";
//     cout << "pbpb: " << thi->Project(pbpb[icent]->GetName(),"inclJetPt",jetSel) << endl;
    cout << "pbpb: " << thi->Project(pbpb[icent]->GetName(),"inclJetPt",centCut&&jetSel) << endl;
    cout << "pp: " << vtpp[icent]->Project(pp[icent]->GetName(),"inclJetPt",jetSel) << endl;
    pbpb[icent]->Scale(1./pbpb[icent]->Integral());
    pp[icent]->Scale(1./pp[icent]->Integral());
//     ratio[icent]->Divide(pp[icent],pbpb[icent]);
    ratio[icent]->Divide(pbpb[icent],pp[icent]);
  }
  
  // Draw
  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.2,0.15,0.02);

  TLegend* l2  =  new  TLegend(0.03183659,0.5514706,0.9316217,0.8526261,NULL,"brNDC");
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(5-icent);
    
    handsomeTH1(pp[icent],1,1,kFullCircle);
    handsomeTH1(pbpb[icent],1,1,kOpenCircle);
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
      l1->AddEntry(pp[icent],"pp","p");
      l1->AddEntry(pbpb[icent],"PbPb","p");
//       l1->AddEntry(pp[icent],"pp after smearing","p");
//       l1->AddEntry(pbpb[icent],"pp before smearing","p");
      l1->Draw();
    }
    int lowCent = centBin[icent-1];
    int highCent = centBin[icent]-1;
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
    handsomeTH1(ratio[icent],2);
    ratio[icent]->SetYTitle("");
    ratio[icent]->Draw();
    if ( icent >1)
      drawPatch(0.90,0.05,1.01,0.147,     0,1001,"NDC");
    if ( icent <4)
      drawPatch(0.0,0.05,0.1,0.147,     0,1001,"NDC");
  }
  
//   c1->SaveAs("fig/pp_smearing_spectrum_comparison.gif");
//   c1->SaveAs("fig/pp_smearing_spectrum_comparison.pdf");
  c1->SaveAs("fig/ppsmcent1noshift_pbpb_spectrum_comparison.gif");
  c1->SaveAs("fig/ppsmcent1noshift_pbpb_spectrum_comparison.pdf");
}
