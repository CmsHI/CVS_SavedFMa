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

void drawClosures() {
  TH1::SetDefaultSumw2();

  int ijet=1;
  TString infname,reffname;

  TCanvas* c1 =new TCanvas("c1","",1000,600);
  Plot4x4::title1="Reco. Tracks";
  Plot4x4::title2="Gen. Particles";
  // Plot4x4::title1="All Gen. Particles";
  // Plot4x4::title2="Sig. Gen Particles";
  Plot4x4::outpath="closure/mctrk";
  TFile outf(Plot4x4::outpath+".root","recreate"); outf.Close();

  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);
  // infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet.root";
  // reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet.root";
  infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and100and120_hi.root";
  reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and100and120_hi.root";
  TH1D * h[3][5], * href[3][5];
  for ( int icent=1; icent<=4 ; icent++) {
    h[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_himc_icent%d_irj999_fragMode2_closure100_jtrewt0_",icent));
    href[ijet][icent] = (TH1D*)load(reffname,Form("hjetPt_himc_icent%d_irj999_fragMode2_closure102_jtrewt0_",icent));
  }
  Plot4x4 p1(h,href,1,"jetpt");
  p1.Draw(c1,100,299.9);
  p1.DrawLeg(c1,3);

  int ana[2] = {1,2};
  TString sub[3] = {"raw","mb","sig"};
  for (int ia=0; ia<2; ++ia) {
    for (int isub=0; isub<3; ++isub) {
      TCanvas* c2 =new TCanvas(Form("c2_%d_%d",ana[ia],isub),Form("c2_%d_%d",ana[ia],isub),1000,600);
      makeMultiPanelCanvas(c2,4,2,0.0,0.0,0.25,0.2,0.02);
      for ( int icent=1; icent<=4 ; icent++) {
        h[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_himc_icent%d_irj999_fragMode%d_closure100_jtrewt0_wtmode0_pt1to300",sub[isub].Data(),icent,ana[ia]));
        href[ijet][icent] = (TH1D*)load(reffname, Form("hpt_jet_%sTrk_himc_icent%d_irj999_fragMode%d_closure102_jtrewt0_wtmode0_pt1to300",sub[isub].Data(),icent,ana[ia]));
      }
      Plot4x4 p2(h,href,0,Form("ffana%d_sub%d",ana[ia],isub));
      if (ana[ia]==1) {
        p2.Draw(c2,1,85.9,2,2e-5);
      } else if (ana[ia]==2) {
        p2.Draw(c2,0,5.49,1,2e-3);
      }
      p2.DrawLeg(c2,3);
    }
  }
}
