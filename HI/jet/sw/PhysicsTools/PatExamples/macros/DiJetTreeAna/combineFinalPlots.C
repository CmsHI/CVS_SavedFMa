#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TROOT.h"
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TGraph.h>
#include "TString.h"
// stl
#include <vector>
//helpers
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

char * drsg = "hist";
char * drdb = "hist same";
char * drsgE = "E1";
char * drdbE = "E1 same";

void combineFinalPlots(char* tag = "sw48")
{
  // set inputs
  vector<TString> jobs;
  jobs.push_back("/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/plots/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV");
  jobs.push_back("/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/plots/CMSSW_3_3_1_fix04/SignalOnly/Hydjet_MinBias_noColl_4TeV");

  vector<TString> gen;
  gen.push_back("pyquen_quendijet_nocoll_pt120to170_d20091024");
  gen.push_back("pyquen_unquendijet_nocoll_pt120to170_d20091025");
  vector<TString> anatype;
  anatype.push_back("j2_1_t1");
  anatype.push_back("j2_0_t1");
  anatype.push_back("j1_1_t0");
  anatype.push_back("j0_1_t0");

  vector<TFile*> infiles;
  for (int j=0; j<jobs.size(); ++j) {
    for (int igen=0; igen<gen.size(); ++igen) {
      for (int itype=0; itype<anatype.size(); ++itype) {
	infiles.push_back(new TFile(Form("%s/%s/%s/jet_125_300_100_jdphi_2.85_%s_%s/jFF/FFHistos.root",
		jobs[j].Data(),gen[igen].Data(),tag,tag,anatype[itype].Data())));
	infiles.back()->Print();
      }
    }
  }

  // outout
  char plotdir[1000];
  sprintf(plotdir,"plots/%s/combinedPlots",tag);

  // ============ start to plot ===================
  // final styles
  Size_t mksz=0.5;
  Style_t mkstNear= kFullDotLarge;
  Style_t mkstAway= kCircle;
  Style_t lnst = 1;
  Color_t lcNear = kBlack;
  Color_t lcAway = kBlue;
  Width_t lwUncut = 1;
  Width_t lw=3;
  int plotmode=1;
  Int_t padxsize=400;
  Int_t padysize=400;
  Int_t ncol = anatype.size();
  Int_t nrow = gen.size()*jobs.size();

  // === dijet properties ===
  // -- dphi --
  TCanvas * chJDPhi = new TCanvas("chJDPhi","chJDPhi",padxsize*ncol,padysize*nrow);
  chJDPhi->Divide(anatype.size(),gen.size()*jobs.size());
  for (int i=0; i<infiles.size(); ++i) {
    chJDPhi->cd(i+1);
    gPad->SetLogy();
    TH2D * hJDPhi = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hJDPhi",drsg,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
  }

  // -- near vs away uncut --
  TCanvas * chUncutNJetEtvsAJetEt = new TCanvas("chUncutNJetEtvsAJetEt","chUncutNJetEtvsAJetEt",padxsize*ncol,padysize*nrow);
  chUncutNJetEtvsAJetEt->Divide(anatype.size(),gen.size()*jobs.size());
  for (int i=0; i<infiles.size(); ++i) {
    chUncutNJetEtvsAJetEt->cd(i+1);
    gPad->SetLogz();
    TH2D * hUncutNJetEtvsAJetEt = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hUncutNJetEtvsAJetEt","colz","","","",-1,0,0,0,0,mksz,0,0,plotmode));
  }

  // -- near vs away --
  TCanvas * chNJetEtvsAJetEt = new TCanvas("chNJetEtvsAJetEt","chNJetEtvsAJetEt",padxsize*ncol,padysize*nrow);
  chNJetEtvsAJetEt->Divide(anatype.size(),gen.size()*jobs.size());
  for (int i=0; i<infiles.size(); ++i) {
    chNJetEtvsAJetEt->cd(i+1);
    gPad->SetLogz();
    TH2D * hNJetEtvsAJetEt = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hNJetEtvsAJetEt","colz","","","",-1,0,0,0,0,mksz,0,0,plotmode));
  }

  // -- near/away et ratio --
  TCanvas * chANRatVsB = new TCanvas("chANRatVsB","chANRatVsB",padxsize*ncol,padysize*nrow);
  chANRatVsB->Divide(anatype.size(),gen.size()*jobs.size());
  for (int i=0; i<infiles.size(); ++i) {
    chANRatVsB->cd(i+1);
    gPad->SetLogz();
    TH2D * hANRatVsB = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hANRatVsB","colz","","","",-1,0,0,0,0,mksz,0,0,plotmode));
  }

  // Hi Event
  TCanvas * chB = new TCanvas("chB","B",padxsize*ncol,padysize*nrow);
  chB->Divide(anatype.size(),gen.size()*jobs.size());
  for (int i=0; i<infiles.size(); ++i) {
    chB->cd(i+1);
    TH1D * hUncutB = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hUncutB",drsg,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
    TH1D * hB = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hB",drdb,"","","",-1,0,0,0,0,mksz,0,0,plotmode));
    TLegend *lB = new TLegend(0.2,0.6,0.5,0.80);
    lB->AddEntry(hUncutB,"Before Jet Et cut","L");
    lB->AddEntry(hB,"After Jet Et cut","L");
    lB->SetTextSize(0.03);
    lB->Draw();
  }

  // print canvas and save histograms

  printAllCanvases(plotdir);
}
