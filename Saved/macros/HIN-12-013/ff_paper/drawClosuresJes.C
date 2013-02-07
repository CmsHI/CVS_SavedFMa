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

void drawClosuresJes() {
  TH1::SetDefaultSumw2();

  // Setup
  int ijet=1;
  TString infname,reffname;
  Plot4x4::title1="JES*{0.97 to 0.949)";
  Plot4x4::title2="JES*1";
  // Inputs
  // data
  // infname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17dataJes0.935extrap_hi_pprewt.root";
  // reffname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_hi_pprewt.root";
  infname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_and_mc80and100_Jes0.949extrap.root";
  reffname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_and_mc80and100.root";
  Plot4x4::outpath="closure/data_sysjes0.949exptrap";

  int doMC=0;

  TString dataset="data";
  TFile outf(Plot4x4::outpath+".root","recreate"); outf.Close();

  TH1D * h[3][5], * href[3][5];
  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);
  for ( int icent=1; icent<=4 ; icent++) {
    h[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_hi%s_icent%d_irj999_fragMode2_closure100_jtrewt1_",dataset.Data(),icent));
    href[ijet][icent] = (TH1D*)load(reffname,Form("hjetPt_hi%s_icent%d_irj999_fragMode2_closure100_jtrewt1_",dataset.Data(),icent));
  }
  Plot4x4 p1(h,href,1,"_jetpt");
  p1.Draw(c1,100,299.9);
  p1.DrawLeg(c1,3);

  int ana[2] = {1,2};
  TString sub[3] = {"raw","mb","sig"};
  for (int ia=0; ia<2; ++ia) {
    for (int isub=2; isub<3; ++isub) {
      TCanvas* c2 =new TCanvas(Form("c2_%d_%d",ana[ia],isub),Form("c2_%d_%d",ana[ia],isub),1000,600);
      makeMultiPanelCanvas(c2,4,2,0.0,0.0,0.25,0.2,0.02);
      for ( int icent=1; icent<=4 ; icent++) {
        h[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_hi%s_icent%d_irj999_fragMode%d_closure100_jtrewt1_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        href[ijet][icent] = (TH1D*)load(reffname, Form("hpt_jet_%sTrk_hi%s_icent%d_irj999_fragMode%d_closure100_jtrewt1_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
      }
      Plot4x4 p2(h,href,0,Form("_ffana%d_sub%d_clos100",ana[ia],isub));
      if (ana[ia]==1) {
        p2.Draw(c2,1,59,2,2e-5,20);
      } else if (ana[ia]==2) {
        p2.Draw(c2,0,5.49,1,2e-2);
      }
      p2.DrawLeg(c2,3);
    }
  }
}
