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

void drawClosuresSmear() {
  TH1::SetDefaultSumw2();

  // Setup
  int ijet=1;
  Plot4x4::title1="PYTHIA+HYDJET";
  Plot4x4::title2="PYTHIA";

  // hi vs pp
  TString infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Feb14v2_data_mc80to170_hi_pp.root";
  TString reffname = infname;
  Plot4x4::outpath="closure/mc_ana_closure102";
  // TString infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17_data_mc80to100_hi_pp.root";
  // TString reffname = infname;
  // Plot4x4::outpath="closure/mc_ana_closure102_Jan17";

  int doMC=1;
  TString dataset="mc";
  int ppsm=2;
  Plot4x4::outpath += Form("ppsm%d",ppsm);

  TFile outf(Plot4x4::outpath+".root","recreate"); outf.Close();

  TH1D * h[3][5], * href[3][5];
  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);
  for ( int icent=1; icent<=4 ; icent++) {
    h[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_hi%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm0_",dataset.Data(),icent));
    href[ijet][icent] = (TH1D*)load(reffname,Form("hjetPt_pp%s_icent%d_irj999_fragMode2_closure102_rewt0_ppsm%d_",dataset.Data(),icent,ppsm));
  }
  Plot4x4 p1(h,href,1,"_jetpt");
  p1.Draw(c1,100,299.9);
  p1.DrawLeg(c1,3);

  int ana[2] = {1,2};
  TString sub[3] = {"raw","mb","sig"};
  for (int ia=0; ia<2; ++ia) {
    for (int isub=0; isub<3; ++isub) {
      TCanvas* c2 =new TCanvas(Form("c2_%d_%d",ana[ia],isub),Form("c2_%d_%d",ana[ia],isub),1000,600);
      makeMultiPanelCanvas(c2,4,2,0.0,0.0,0.25,0.2,0.02);
      for ( int icent=1; icent<=4 ; icent++) {
        h[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_hi%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm0_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        href[ijet][icent] = (TH1D*)load(reffname, Form("hpt_jet_%sTrk_pp%s_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm%d_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia],ppsm));
      }
      Plot4x4 p2(h,href,0,Form("_ffana%d_sub%d",ana[ia],isub));
      if (ana[ia]==1) {
        p2.Draw(c2,1,103,2,2e-5,2e2);
      } else if (ana[ia]==2) {
        p2.Draw(c2,0,5.49,1,2e-2,2e2);
      }
      p2.DrawLeg(c2,3);
    }
  }
}
