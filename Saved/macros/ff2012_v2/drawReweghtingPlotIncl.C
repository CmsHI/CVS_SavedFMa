#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <time.h>
// #include "commonUtility.h"
#include "CutAndBinCollection2011.h"
#include "HisMath.C"
using namespace std;

const int nptbin=11;
double ptbins[nptbin+1] ={100, 110, 120, 130, 140, 150, 160, 170, 180, 200, 240, 300};

void drawFromHist(TString infname,TString outname="fig/pp_spectrum_comparison", int cmpMode=1) {
  TH1D * pp[5] ;
  TH1D * pbpb[5] ;
  TH1D * ratio[5] ;
  for ( int icent=1; icent<=4 ; icent++) {
    pbpb[icent]   = (TH1D*)load(infname,Form("hjetPt_hi_inclusiveJet_icent%d",icent));
    pp[icent]     = (TH1D*)load(infname,Form("hjetPt_pp_inclusiveJet_icent%d",icent));
    ratio[icent]  = (TH1D*)pbpb[icent]->Clone(Form("hjetPt_ratio_inclusiveJet_icent%d",icent));
    normHist(pbpb[icent],1,true);
    normHist(pp[icent],1,true);
//     normHist(pbpb[icent],0,true);
//     normHist(pp[icent],0,true);
    if (cmpMode==1) ratio[icent]->Divide(pbpb[icent],pp[icent]);
    else ratio[icent]->Divide(pp[icent],pbpb[icent]);
  }

  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);

  TLegend* l2  =  new  TLegend(0.03183659,0.5514706,0.9316217,0.8526261,NULL,"brNDC");
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(5-icent);
    
    if (cmpMode==1) {
      handsomeTH1(pp[icent],1,1,kOpenCircle);
      handsomeTH1(pbpb[icent],1,1,kFullCircle);
    } else {
      handsomeTH1(pp[icent],1,1,kFullCircle);
      handsomeTH1(pbpb[icent],1,1,kOpenCircle);
    }
    pp[icent]->SetAxisRange(100,290,"X");
    pp[icent]->SetAxisRange(2e-5,0.2,"Y");
    pp[icent]->SetNdivisions(505);
    fixedFontHist(pp[icent],2,2.5);
    pp[icent]->SetYTitle("u.n.");
    pp[icent]->Draw();
    pbpb[icent]->Draw("same");
    gPad->SetLogy();
    if ( icent == 4) {
      drawText("CMS Preliminary",0.3,0.89,1,20);
      TLegend* l1 = new TLegend(0.52,0.51,0.95,0.93,NULL,"brNDC");
      easyLeg(l1);
      l1->AddEntry(pp[icent],"|#eta_{jet}| < 2","");
      if (cmpMode==1) {
        l1->AddEntry(pbpb[icent],"PbPb","p");
        l1->AddEntry(pp[icent],"pp","p");
      } else if (cmpMode==2) {
        l1->AddEntry(pp[icent],"pp","p");
        l1->AddEntry(pbpb[icent],"raw pp","p");
      } else if (cmpMode==3) {
        l1->AddEntry(pp[icent],"PbPb","p");
        l1->AddEntry(pbpb[icent],"raw PbPb","p");
      }
      l1->Draw();
    }
    int lowCent = centBin1[icent-1];
    int highCent = centBin1[icent]-1;
    if ( icent ==4 ) 
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.3,0.09,1);
    else
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.15,0.09,1);

  }
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(9-icent);
    handsomeTH1(ratio[icent],1);
    ratio[icent]->SetMarkerStyle(20);
    ratio[icent]->SetAxisRange(100,290,"X");
    ratio[icent]->SetAxisRange(0,2.1,"Y");
    ratio[icent]->SetNdivisions(505);
    fixedFontHist(ratio[icent],1.8,2.5);
    if (cmpMode==1) ratio[icent]->SetYTitle("PbPb/pp");
    else ratio[icent]->SetYTitle("smeared/raw");
    ratio[icent]->Draw();
    TLine * hline = new TLine(100,1,290,1);
    hline->SetLineStyle(2);
    hline->Draw();
//     if ( icent >1)
//       drawPatch(0.90,0.05,1.01,0.147,     0,1001,"NDC");
//     if ( icent <4)
//       drawPatch(0.0,0.05,0.1,0.147,     0,1001,"NDC");
  }
  
  c1->SaveAs(outname+".gif");
  c1->SaveAs(outname+".pdf");
}

void drawReweghtingPlotIncl(int smearPP=1, int rewtJet=0, int cmpMode=1) { // mode: 1=pbpb/pp, 2=pp/ppraw
  TH1::SetDefaultSumw2();
  
  //////////////////////////////////////////////////
  // Input
  //////////////////////////////////////////////////
  TTree * thi;
  TTree * vtpp[5];
  TH1D * pp[5] ;
  TH1D * pbpb[5] ;
  TH1D * ratio[5] ;

  // pbpb
  TFile* inf1;
//   if (cmpMode==1) inf1 = new TFile("jskim_hltjet80-pt90-v20_akPu3PF_Nov20_jetPt_50_jetEtaCut_2.00_noPbin_sm0_akPu3PF_gj0.root");
//   if (cmpMode==1) inf1 = new TFile("jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_akPu3PF_gj0.root");
  if (cmpMode==1) inf1 = new TFile("jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root");
  else if (cmpMode==2) inf1 = new TFile("jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root");
  else if (cmpMode==3) inf1 = new TFile("jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_akPu3PF_gj0.root");
  cout << "Single: " << inf1->GetName() << endl;
  thi = (TTree*)inf1->Get("tdj");
  // pp
  for ( int icent=1; icent<=4 ; icent++) {
    TString inf1name;
    if (cmpMode<3) {
      if (smearPP==0) inf1name = Form("jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root");
      else inf1name = Form("jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
      if (rewtJet==1) inf1name.ReplaceAll(".root","_addedReweight.root");
    } else if (cmpMode==3) {
      inf1name="jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    }
    inf1 = new TFile(inf1name);
    cout << "Var Cent: " << inf1->GetName() << endl;
    vtpp[icent] = (TTree*)inf1->Get("tdj");
  }  
  
  //////////////////////////////////////////////////
  // Output
  //////////////////////////////////////////////////
  TFile * outf = new TFile(Form("fig/Dec5FinalPbPbCorr/hisCmp%d_Smear%d_Rewt%d.root",cmpMode,smearPP,rewtJet),"recreate");
  TString outname=Form("fig/Dec5FinalPbPbCorr/pp_spectrum_cmp%d_sm%d_rewt%d",cmpMode,smearPP,rewtJet);

  for ( int icent=1; icent<=4 ; icent++) {
    pbpb[icent] = new TH1D(Form("hjetPt_hi_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",nptbin,ptbins);
    pp[icent] = new TH1D(Form("hjetPt_pp_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",nptbin,ptbins);
  }
  
  //////////////////////////////////////////////////
  // Analysis
  //////////////////////////////////////////////////
  TCut jetSel = "jetPt>100";
  for ( int icent=1; icent<=4 ; icent++) {
    TCut centCut = Form("cBin>=%.1f&&cBin<%.1f",centBin1[icent-1],centBin1[icent]);
    cout << TString(centCut) << " ";
    if (cmpMode==1) cout << "pbpb: " << thi->Project(pbpb[icent]->GetName(),"jetPt",centCut&&jetSel) << endl;
    else if (cmpMode==2) cout << "pp raw: " << thi->Project(pbpb[icent]->GetName(),"jetPt",jetSel) << endl;
    else if (cmpMode==3) cout << "pbpb raw: " << thi->Project(pbpb[icent]->GetName(),"jetPt",centCut&&jetSel) << endl;
    TCut ppJetSel=jetSel;
    if (rewtJet==1) ppJetSel*="rewtPt";
    if (cmpMode<3) cout << "pp: " << vtpp[icent]->Project(pp[icent]->GetName(),"jetPt",ppJetSel) << endl;
    else cout << "corr pbpb: " << vtpp[icent]->Project(pp[icent]->GetName(),"jetPt",centCut&&jetSel) << endl;;
  }
  outf->Write();
  outf->Close();
  
  // Draw
  drawFromHist(outf->GetName(),outname,cmpMode);
}
