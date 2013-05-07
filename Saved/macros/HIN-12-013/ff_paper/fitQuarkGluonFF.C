#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>
#include "HIN-12-013.h"
#include "TemplateFit.h"
#include "HIN-12-013.h"
using namespace std;

double centBinAna[5] = {0,4,12,20,40};

void fitQuarkGluonFF(int fragMode=2,
  int doMC = 0,
  int anacent=1
  )
{ 
  TH1::SetDefaultSumw2();

  //////////////////////////////////////////////////////////////////////
  // Setup
  //////////////////////////////////////////////////////////////////////
  TString legtitle =   Form("PbPb Data                %.0f - %.0f%%", float(centBinAna[anacent-1]*2.5), float(centBinAna[anacent]*2.5));
  if (doMC) legtitle = Form("PYTHIA+HYDJET          %.0f - %.0f%%", float(centBinAna[anacent-1]*2.5), float(centBinAna[anacent]*2.5));
  TString tag = Form("templatefits_hi_bc11_mc%d_cent%d",doMC,anacent);
  TString outdir = "plotsFinalFFTemplateFits";

  //////////////////////////////////////////////////////////////////////
  // Specify Inputs
  //////////////////////////////////////////////////////////////////////
  TString infdata=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_data_mc80to170_hi_pp_corrjbias11_v3.root",fragMode);
  // TString infdata=Form("plotsFinalFF/FFana2_trkPtProj_binMode2_hi_rewt0_pp_sm2rewt0_bc11_mc2_rat_EtaRef_mc80to170_clos101.root",fragMode);
  // TString infdata=Form("plotsFinalFF/FFana2_trkPtProj_binMode2_hi_rewt1_pp_sm2rewt1_bc11_mc0_rat_EtaRef.root",fragMode);

  TString inftempl1=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_mc80to170_gluon_hi_pp_corrjbias11_v3.root",fragMode);
  TString inftempl2=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_mc80to170_quark_hi_pp_corrjbias11_v3.root",fragMode);

  /////////////////////////////////////////////////
  // Get Input Histograms
  /////////////////////////////////////////////////
  TH1D* ffratio[3][5];
  TString dataset="hidata";
  if (doMC) dataset="himc";
  int jtrewthi=0;
  int closure=100;
  for (int icent=1; icent<=4; ++icent) {
    ffratio[0][icent]  = (TH1D*)load(infdata.Data(),Form("hpt_jet_sigTrk_%s_icent%d_irj999_fragMode%d_closure%d_rewt%d_ppsm0_wtmode0_pt1to300",dataset.Data(),icent,fragMode,closure,jtrewthi));
    ffratio[1][icent]  = (TH1D*)load(inftempl1.Data(),Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure%d_rewt0_ppsm0_wtmode0_pt1to300",icent,fragMode,closure));
    ffratio[2][icent]  = (TH1D*)load(inftempl2.Data(),Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure%d_rewt0_ppsm0_wtmode0_pt1to300",icent,fragMode,closure));
  }

  /////////////////////////////////////////////////
  // Fit Analysis
  /////////////////////////////////////////////////
  TemplateFit fitrat(0.02);
  fitrat.Init(ffratio[0][anacent], ffratio[1][anacent], ffratio[2][anacent]);

  // fitrat.hData->Draw();
  // fitrat.hTemplate[1]->Draw("same hist");
  // fitrat.hTemplate[2]->Draw("same hist");

  // fitrat.Fit(0,5.5);
  fitrat.Fit(0,2.5);
  fitrat.GetMinChi2();
  
  /////////////////////////////////////////////////
  // Draw
  /////////////////////////////////////////////////
  TCanvas * c2 = new TCanvas("c2","c2",0,0,500,500);
  TH1D * hFrame = (TH1D*)ffratio[0][anacent]->Clone("hFrame");
  TString ytitle = "1/N_{ jet} dN_{ track} /d#xi";
  float xmin=0, xmax=5.25;
  hFrame->Reset();
  hFrame->SetYTitle(ytitle);
  hFrame->SetAxisRange(xmin,xmax,"X");
  hFrame->SetAxisRange(0,4,"Y");
  handsomeTH1(hFrame,1,1);
  fixedFontHist(hFrame,1.,1.,25);
  hFrame->Draw();

  SysErrors sysff(fragMode,2,1,0,5.5);
  sysff.Combine(ffratio[0][anacent],sysff.vErrorRat[1][anacent],sysff.NErrorRatio);
  sysff.Apply(ffratio[0][anacent]);
  sysff.Draw(ffratio[0][anacent]);
  fitrat.Inspect(legtitle,xmin,xmax,"",0,4,c2,outdir+"/"+tag);
}