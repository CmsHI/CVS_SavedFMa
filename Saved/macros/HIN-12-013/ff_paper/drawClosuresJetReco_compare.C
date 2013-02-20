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
#include "CutAndBinCollection2011.h"
#include "HisMath.C"
#include "HIN-12-013.h"
using namespace std;

void drawClosuresJetReco_compare() {
  TH1::SetDefaultSumw2();

  // Setup
  int ijet=1;
  Plot4x4::title1="Reco Jet";
  Plot4x4::title2="Gen Jet";
  // Inputs
  TString infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root";
  TString reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_mc80to170_refjetsel_hi_pp.root";
  // TString infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17_data_mc80to100_hi_pp.root";
  // TString reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17_mc80to100_refjetsel_hi_pp.root";

  // isolate effect of selection
  // TString infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_mc80to170_pureselbias_hi_pp.root";
  // TString reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_mc80to170_refjetana_hi_pp.root";

  // isolate effect of plotting jet reco pt
  // TString infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_mc80to170_refjetsel_hi_pp.root";
  // TString reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_mc80to170_refjetana_hi_pp.root";

  // compare
  TString infnamecmp  = infname;
  TString reffnamecmp = reffname;

  Plot4x4::outpath="closure/mc_jetreco_bias_closure102_hi_pp";

  int doMC=1;
  TString dataset="mc";
  int ppsm=0;
  Plot4x4::outpath += Form("ppsm%d",ppsm);

  TFile outf(Plot4x4::outpath+".root","recreate"); outf.Close();

  TH1D * h[3][5], * href[3][5];
  TH1D * hcmp[3][5], * hrefcmp[3][5];

  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);
  for ( int icent=1; icent<=4 ; icent++) {
    h[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_hi%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm0_",dataset.Data(),icent));
    href[ijet][icent] = (TH1D*)load(reffname,Form("hjetPt_hi%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm0_",dataset.Data(),icent));
    hcmp[ijet][icent]    = (TH1D*)load(infnamecmp, Form("hjetPt_pp%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm%d_",dataset.Data(),icent,ppsm));
    hrefcmp[ijet][icent] = (TH1D*)load(reffnamecmp,Form("hjetPt_pp%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm%d_",dataset.Data(),icent,ppsm));
  }

  Plot4x4 p1(h,href,1,"_jetpt");
  Plot4x4 p1cmp(hcmp,hrefcmp,1,"_jetptcmp",1);
  p1.Draw(c1,100,299.9);
  p1cmp.Draw(c1,100,299.9);
  p1cmp.DrawLeg(c1,3,0.096,0.179,0.577,0.389,"pp");
  p1.DrawLeg(c1,3);

  int ana[2] = {1,2};
  TString sub[3] = {"raw","mb","sig"};
  for (int ia=0; ia<2; ++ia) {
    for (int isub=0; isub<3; ++isub) {
      TCanvas* c2 =new TCanvas(Form("c2_%d_%d",ana[ia],isub),Form("c2_%d_%d",ana[ia],isub),1000,600);
      makeMultiPanelCanvas(c2,4,2,0.0,0.0,0.25,0.2,0.02);
      for ( int icent=1; icent<=4 ; icent++) {
        h[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_hi%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm0_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        href[ijet][icent] = (TH1D*)load(reffname, Form("hpt_jet_%sTrk_hi%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm0_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        hcmp[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_pp%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm%d_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia],ppsm));
        hrefcmp[ijet][icent] = (TH1D*)load(reffname, Form("hpt_jet_%sTrk_pp%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm%d_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia],ppsm));
      }

      Plot4x4 p2(h,href,0,Form("_ffana%d_sub%d",ana[ia],isub));
      Plot4x4 p2cmp(hcmp,hrefcmp,0,Form("_ffana%d_sub%d_cmp",ana[ia],isub),1);
      if (ana[ia]==1) {
        p2.Draw(c2,1,103,2,2e-5,2e2);
        p2cmp.Draw(c2,1,103,2,2e-5,2e2);
      } else if (ana[ia]==2) {
        p2.Draw(c2,0,5.49,1,2e-2,2e2);
        p2cmp.Draw(c2,0,5.49,1,2e-2,2e2);
      }
      p2cmp.DrawLeg(c2,3,0.096,0.179,0.577,0.389,"pp");
      p2.DrawLeg(c2,3);
    }
  }
}
