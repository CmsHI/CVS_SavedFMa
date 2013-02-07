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

void drawClosuresDiff() {
  TH1::SetDefaultSumw2();

  int doCompare=1; // 1=qm12, 2=paper data

  // Setup
  int ijet=1;
  TString infname,reffname;
  Plot4x4::title1="Paper pp";
  Plot4x4::title2="QM12 pp";
  // paper vs qm12
  infname  = "inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_and_mc80and100.root";
  reffname  = "dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pas.root";
  Plot4x4::outpath="closure/data_pp_vs_qm12";
  // data

  int doMC=0;
  TString dataset="ppdata";

  // if (!doMC) {
  //   dataset="data";
  //   // Plot4x4::title1="PbPb";
  //   Plot4x4::title1="pp rewt*0.5";
  //   Plot4x4::title2="pp";
  // }

  TFile outf(Plot4x4::outpath+".root","recreate"); outf.Close();

  TH1D * h[3][5], * href[3][5];
  TCanvas* c1 =new TCanvas("c1","",1000,600);
  makeMultiPanelCanvas(c1,4,2,0.0,0.0,0.25,0.2,0.02);
  for ( int icent=1; icent<=4 ; icent++) {
    h[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_%s_icent%d_irj999_fragMode2_closure100_jtrewt1_",dataset.Data(),icent));
        // QM Comparison
    if (doCompare==1) href[ijet][icent] = (TH1D*)load(reffname,Form("hLjetPt_%s_icent%d_irj999_fragMode2_closure100",dataset.Data(),icent));
    else if (doCompare==2) href[ijet][icent]    = (TH1D*)load(infname, Form("hjetPt_%s_icent%d_irj999_fragMode2_closure100_jtrewt1_",dataset.Data(),icent));
  }


  Plot4x4 p1(h,href,1,"_jetpt");
  p1.Draw(c1,100,299.9);
  p1.DrawLeg(c1,3,0.28,0.01,0.76,0.3);

  int ana[2] = {1,2};
  TString sub[3] = {"raw","mb","sig"};
  for (int ia=0; ia<2; ++ia) {
    for (int isub=2; isub<3; ++isub) {
      TCanvas* c2 =new TCanvas(Form("c2_%d_%d",ana[ia],isub),Form("c2_%d_%d",ana[ia],isub),1000,600);
      makeMultiPanelCanvas(c2,4,2,0.0,0.0,0.25,0.2,0.02);
      for ( int icent=1; icent<=4 ; icent++) {
        h[ijet][icent]    = (TH1D*)load(infname,  Form("hpt_jet_%sTrk_%s_icent%d_irj999_fragMode%d_closure100_jtrewt1_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        /////////////////////////////////////////////////
        // Get Comparison Histograms
        /////////////////////////////////////////////////
        if (doCompare==1) {
        // QM Comparison
          if (ana[ia]==2) href[ijet][icent]  = (TH1D*)load(reffname.Data(),Form("hpt_lJet_%sTrk_%s_icent%d_irj999_fragMode%d_closure100_wtmode0_pt1to500",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
          // else if (ana[ia]==1) GetQM12TrkPt(0,href);
          else if (ana[ia]==1) GetQM12TrkPt(href,0);
        } else if (doCompare==2) {
          href[ijet][icent]  = (TH1D*)load(reffname.Data(),Form("hpt_jet_%sTrk_%s_icent%d_irj999_fragMode%d_closure100_jtrewt1_wtmode0_pt1to300",sub[isub].Data(),dataset.Data(),icent,ana[ia]));
        }
      }

      Plot4x4 p2(h,href,0,Form("_ffana%d_sub%d",ana[ia],isub));
      if (ana[ia]==1) {
        p2.Draw(c2,1,59,2,2e-5,20);
      } else if (ana[ia]==2) {
        p2.Draw(c2,0,5.49,1,2e-2);
      }
      p2.DrawLeg(c2,3,0.28,0.01,0.76,0.3);
    }
  }
}
