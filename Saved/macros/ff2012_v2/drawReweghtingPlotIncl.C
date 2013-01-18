#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <time.h>
#include <cassert>
// #include "commonUtility.h"
#include "CutAndBinCollection2011.h"
#include "HisMath.C"
using namespace std;

const int nptbin=11;
double ptbins[nptbin+1] ={100, 110, 120, 130, 140, 150, 160, 170, 180, 200, 240, 300};

// const int nptbin=16;
// double ptbins[nptbin+1] ={90,95,100,105,110,115,120,130,140,150,160,170,180,200,220,260,300};

// const int nptbin=14;
// double ptbins[nptbin+1] ={100,105,110,115,120,130,140,150,160,170,180,200,220,260,300};

// const int nptbin=33;
// double ptbins[nptbin+1] = {30,40,50,60,70,80,90,100,
//           110,120,130,140,150,160,170,180,190,200,
//           210,220,230,240,250,260,270,280,290,300,
//           310,320,330,340,350,400};

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
    // normHist(pbpb[icent],0,true);
    // normHist(pp[icent],0,true);
    ratio[icent]->Divide(pbpb[icent],pp[icent]);
  }

  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);

  TLegend* l2  =  new  TLegend(0.03183659,0.5514706,0.9316217,0.8526261,NULL,"brNDC");
  for ( int icent=1; icent<=4 ; icent++) {
    c1->cd(5-icent);
    
    handsomeTH1(pp[icent],1,1,kOpenCircle);
    handsomeTH1(pbpb[icent],1,1,kFullCircle);
    pp[icent]->SetAxisRange(80,290,"X");
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
      } else if (cmpMode==4) {
        l1->AddEntry(pp[icent],"PbPb","p");
        l1->AddEntry(pbpb[icent],"PbPb jet90 skim","p");      }
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
    ratio[icent]->SetAxisRange(80,290,"X");
    ratio[icent]->SetAxisRange(0.5,1.5,"Y");
    ratio[icent]->SetNdivisions(505);
    fixedFontHist(ratio[icent],1.8,2.5);
    if (cmpMode==1) ratio[icent]->SetYTitle("PbPb/pp");
    else ratio[icent]->SetYTitle("ratio");
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

void drawReweghtingPlotIncl(int smearPP=2, int rewtJet=0, int cmpMode=1) { // mode: 1=pbpb/pp, 2=pp/ppraw
  TH1::SetDefaultSumw2();
  
  assert(rewtJet&&cmpMode==1);

  //////////////////////////////////////////////////
  // Input
  //////////////////////////////////////////////////
  TTree * vthi[5];
  TTree * vtpp[5];
  TH1D * pp[5] ;
  TH1D * pbpb[5] ;
  TH1D * ratio[5] ;
  // cmpModes:
  // 1. PbPb/pp_[sm/unsm]
  // 2. pp_sm/pp_unsm
  // 3. PbPb_sm/PbPb_unsm

  TString numName[5];
  TString denName[5];
  TFile * vinf[5];
  for ( int icent=1; icent<=4 ; icent++) {
    // Specify files
    // Dec 5
    // if (cmpMode==1) {
    //   numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    //   denName[icent] = "../ntout/jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    //   if (smearPP)
    //     denName[icent] = Form("../ntout/jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
    // } else if (cmpMode==2) {
    //   numName[icent] = Form("../ntout/jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
    //   denName[icent] = "../ntout/jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    // } else if (cmpMode==3) {
    //   numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    //   denName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_akPu3PF_gj0.root";
    // }
    // Dec 20
    // if (cmpMode==1) {
    //   numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    //   // numName[icent] = "../ntout/jskim_hltjet80-v21_akPu3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_akPu3PF_gj0.root";
    //   denName[icent] = "../ntout/jskim_pp-full_ak3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    //   if (smearPP)
    //     denName[icent] = Form("../ntout/jskim_pp-full_ak3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0_addedReweight.root",smearPP,icent);
    // } else if (cmpMode==2) {
    //   numName[icent] = Form("../ntout/jskim_pp-full_ak3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0_addedReweight.root",smearPP,icent);
    //   denName[icent] = "../ntout/jskim_pp-full_ak3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    // } else if (cmpMode==3) {
    //   // numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    //   // denName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_akPu3PF_gj0.root";
    //   numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    //   denName[icent] = "../ntout/jskim_hltjet80-v21_akPu3PF_Dec20_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    // }
    // Jan 16
    if (cmpMode==1) {
      // numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
      numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
      // numName[icent] = "../ntout/jskim_hltjet80-v21_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
      denName[icent] = "../ntout/jskim_pp-full_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
      if (smearPP)
        denName[icent] = Form("../ntout/jskim_pp-full_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
    } else if (cmpMode==2) {
      numName[icent] = Form("../ntout/jskim_pp-full_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
      denName[icent] = "../ntout/jskim_pp-full_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    } else if (cmpMode==3) {
      numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
      denName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_akPu3PF_gj0.root";
    } else if (cmpMode==4) {
      numName[icent] = "../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
      denName[icent] = "../ntout/jskim_hltjet80-v21_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    }
    // Jan 16 mc
    // if (cmpMode==1) {
    //   numName[icent] = "../ntout/jskim_hydj50_akPu3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root";
    //   denName[icent] = "../ntout/jskim_dj50_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    //   if (smearPP)
    //     denName[icent] = Form("../ntout/jskim_dj50_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
    // } else if (cmpMode==2) {
    //   numName[icent] = Form("../ntout/jskim_dj50_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm%dbin%d_ak3PF_gj0.root",smearPP,icent);
    //   denName[icent] = "../ntout/jskim_dj50_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root";
    // }
    if (rewtJet==1) denName[icent]=denName[icent].ReplaceAll(".root","_addedReweight.root");
    if (rewtJet==2) numName[icent]=numName[icent].ReplaceAll(".root","_addedReweight.root");
    if (rewtJet==3) {
      numName[icent]=numName[icent].ReplaceAll(".root","_addedReweight.root");
      denName[icent]=denName[icent].ReplaceAll(".root","_addedReweight.root");
    }
    // load trees
    vinf[icent] = new TFile(numName[icent]);
    cout << "Numerator: " << vinf[icent]->GetName() << endl;
    vthi[icent] = (TTree*)vinf[icent]->Get("tdj");
    vinf[icent] = new TFile(denName[icent]);
    cout << "Denominator: " << vinf[icent]->GetName() << endl;
    vtpp[icent] = (TTree*)vinf[icent]->Get("tdj");
  }
  
  //////////////////////////////////////////////////
  // Output
  //////////////////////////////////////////////////
  TString outname=Form("fig/Jan16Report/jet_spectrum_cmp%d_sm%d_rewt%d_pbpbJan16_ppJan16",cmpMode,smearPP,rewtJet);
  TFile * outf = new TFile(outname+".root","recreate");

  for ( int icent=1; icent<=4 ; icent++) {
    pbpb[icent] = new TH1D(Form("hjetPt_hi_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",nptbin,ptbins);
    pp[icent] = new TH1D(Form("hjetPt_pp_inclusiveJet_icent%d",icent),";p_{T} (GeV/c);",nptbin,ptbins);
  }
  
  //////////////////////////////////////////////////
  // Analysis
  //////////////////////////////////////////////////
  TCut jetSel = "jetPt>100";
  for ( int icent=1; icent<=4 ; icent++) {
    TCut hiJetSel=jetSel && Form("cBin>=%.1f&&cBin<%.1f",centBin1[icent-1],centBin1[icent]);
    TCut ppJetSel=jetSel;
    if (rewtJet==1) ppJetSel*="rewtPt";
    if (rewtJet==2) hiJetSel*="rewtPt";
    if (rewtJet==3) {
      ppJetSel*="rewtPt";
      hiJetSel*="rewtPt";
    }
    cout << "hi cut: " << TString(hiJetSel) << endl;
    cout << "pp cut: " << TString(ppJetSel) << endl;

    if (cmpMode==1) {
      cout << "pbpb: " << vthi[icent]->Project(pbpb[icent]->GetName(),"jetPt",hiJetSel) << endl;
      cout << "pp:   " << vtpp[icent]->Project(pp[icent]->GetName(),"jetPt",ppJetSel) << endl;
    } else if (cmpMode==2) {
      cout << "pp smeared: " << vthi[icent]->Project(pbpb[icent]->GetName(),"jetPt",ppJetSel) << endl;      
      cout << "pp raw:     " << vtpp[icent]->Project(pp[icent]->GetName(),"jetPt",jetSel) << endl;
    } else if (cmpMode==3||cmpMode==4) {
      cout << "corr pbpb: " << vthi[icent]->Project(pbpb[icent]->GetName(),"jetPt",hiJetSel) << endl;      
      cout << "raw pbpb:  " << vtpp[icent]->Project(pp[icent]->GetName(),"jetPt",hiJetSel) << endl;
    }
  }

  outf->Write();
  outf->Close();
  
  // Draw
  drawFromHist(outf->GetName(),outname,cmpMode);
}
