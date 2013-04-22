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
  // Plot4x4::title1="Reco. Tracks";
  // Plot4x4::title2="Gen. Particles";
  Plot4x4::title1="Gen. Particles";
  Plot4x4::title2="Sig. Gen. Particles";
  // infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root";
  // reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root";
  infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04v3job0_data_mc80to170_refjetgt100_hi_pp_corrjbias0_v3.root";
  reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04v3job0_data_mc80to170_refjetgt100_hi_pp_corrjbias0_v3.root";
  Plot4x4::outpath="closure/mc_corrjbias0_trk_closure102_Mar04v3_hi_refjetgt100";
  // infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_data_mc80to170_hi_pp_corrjbias0_v3.root";
  // reffname = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_data_mc80to170_hi_pp_corrjbias0_v3.root";
  // Plot4x4::outpath="closure/mc_corrjbias0_trk_closure102_Mar19_hi";
  TString dataset="himc";
  
  TFile outf(Plot4x4::outpath+".root","recreate"); outf.Close();

  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);
  cout << "Study:     " << infname << endl;
  cout << "Reference: " << reffname << endl;
  TH1D * h[3][5], * href[3][5];
  for ( int icent=1; icent<=4 ; icent++) {
    h[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm0__",dataset.Data(),icent));
    href[ijet][icent] = (TH1D*)load(reffname,Form("hjetPt_%s_icent%d_irj999_fragMode2_closure101_rewt0_ppsm0__",dataset.Data(),icent));
  }
  Plot4x4 p1(h,href,1,"jetpt");
  p1.Draw(c1,90,299.9);
  p1.DrawLeg(c1,3);

  int ana[2] = {1,2};
  TString sub[3] = {"raw","mb","sig"};
  for (int ia=0; ia<2; ++ia) {
    for (int isub=0; isub<3; ++isub) {
      TCanvas* c2 =new TCanvas(Form("c2_%d_%d",ana[ia],isub),Form("c2_%d_%d",ana[ia],isub),1000,600);
      makeMultiPanelCanvas(c2,4,2,0.0,0.0,0.25,0.2,0.02);
      for ( int icent=1; icent<=4 ; icent++) {
        h[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm0_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        href[ijet][icent] = (TH1D*)load(reffname, Form("hpt_jet_%sTrk_%s_icent%d_irj999_fragMode%d_closure101_rewt0_ppsm0_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
      }
      Plot4x4 p2(h,href,0,Form("ffana%d_sub%d",ana[ia],isub));
      if (ana[ia]==1) {
        p2.Draw(c2,1,85.9,2,2e-5);
      } else if (ana[ia]==2) {
        p2.Draw(c2,0,5.49,1,2e-3);
      }
      if (isub==1) {
        p2.DrawLeg(c2,3,0.26,0.76,0.74,1,"Bkg. Cone","True. Bkg.");
      }
      else p2.DrawLeg(c2,3);
    }
  }
}
