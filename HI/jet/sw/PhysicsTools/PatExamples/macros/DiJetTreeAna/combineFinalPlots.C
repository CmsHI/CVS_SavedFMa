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

void combineFinalPlots()
{
  // set inputs
  char * indir = "/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/plots/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV";
  vector<TString> gen;
  gen.push_back("pyquen_quendijet_nocoll_pt120to170_d20091024");
  gen.push_back("pyquen_unquendijet_nocoll_pt120to170_d20091025");
  vector<TString> anatype;
  anatype.push_back("j2_1_t1");
  anatype.push_back("j2_0_t1");
  anatype.push_back("j1_1_t0");
  anatype.push_back("j0_1_t0");
  char * tag = "sw46_2";

  vector<TFile*> infiles;
  for (int igen=0; igen<gen.size(); ++igen) {
    for (int itype=0; itype<anatype.size(); ++itype) {
      infiles.push_back(new TFile(Form("%s/%s/%s/jet_100_300_80_jdphi_2.85_sw46_2_%s/jFF/FFHistos.root",
	      indir,gen[igen].Data(),tag,anatype[itype].Data())));
      infiles.back()->Print();
    }
  }

  // outout
  char * plotdir = Form("%s/combinedPlots",indir);

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

  // === dijet properties ===
  // -- dphi --
  TCanvas * chJDPhi = new TCanvas("chJDPhi","chJDPhi",1200,600);
  chJDPhi->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chJDPhi->cd(i+1);
    gPad->SetLogy();
    TH2D * hJDPhi = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hJDPhi",drsg,"","","",-1,0,0,0,0,mksz,0,0));
  }

  // -- near vs away uncut --
  TCanvas * chUncutNJetEtvsAJetEt = new TCanvas("chUncutNJetEtvsAJetEt","chUncutNJetEtvsAJetEt",1200,600);
  chUncutNJetEtvsAJetEt->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chUncutNJetEtvsAJetEt->cd(i+1);
    gPad->SetLogz();
    TH2D * hUncutNJetEtvsAJetEt = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hUncutNJetEtvsAJetEt","colz","","","",-1,0,0,0,0,mksz,0,0));
  }

  // -- near vs away --
  TCanvas * chNJetEtvsAJetEt = new TCanvas("chNJetEtvsAJetEt","chNJetEtvsAJetEt",1200,600);
  chNJetEtvsAJetEt->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chNJetEtvsAJetEt->cd(i+1);
    gPad->SetLogz();
    TH2D * hNJetEtvsAJetEt = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hNJetEtvsAJetEt","colz","","","",-1,0,0,0,0,mksz,0,0));
  }

  // -- near/away et ratio --
  TCanvas * chANRatVsB = new TCanvas("chANRatVsB","chANRatVsB",1200,600);
  chANRatVsB->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chANRatVsB->cd(i+1);
    gPad->SetLogz();
    TH2D * hANRatVsB = dynamic_cast<TH2D*>(drawNormHist(infiles[i],"hANRatVsB","colz","","","",-1,0,0,0,0,mksz,0,0));
  }

  // Hi Event
  TCanvas * chB = new TCanvas("chB","B",1200,600);
  chB->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chB->cd(i+1);
    TH1D * hUncutB = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hUncutB",drsg,"","","",-1,0,0,0,0,mksz,0,0));
    TH1D * hB = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hB",drdb,"","","",-1,0,0,0,0,mksz,0,0));
    TLegend *lB = new TLegend(0.2,0.6,0.5,0.80);
    lB->AddEntry(hUncutB,"Before Jet Et cut","L");
    lB->AddEntry(hB,"After Jet Et cut","L");
    lB->SetTextSize(0.03);
    lB->Draw();
  }

  // === FF ===
  TCanvas * chFF = new TCanvas("chFF","FF",1200,600);
  chFF->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chFF->cd(i+1);
    TH1D * hXiNearJet = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hXiNearJet",drsgE,"","","",-1,0,0,0,0,mksz,mkstNear,0));
    TH1D * hXiAwayJet = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hXiAwayJet",drdbE,"","","",-1,0,0,0,0,mksz,mkstAway,0));
    //  - legend -
    TLegend *lXi = new TLegend(0.2,0.6,0.5,0.80);
    lXi->AddEntry(hXiNearJet,"Near Jet","L");
    lXi->AddEntry(hXiAwayJet,"Away Jet","L");
    lXi->SetTextSize(0.03);
    lXi->Draw();
  }

  // === FF ratios ===
  // -- Q/UnQ
  TCanvas * chXiQUnQRat = new TCanvas("chXiQUnQRat","FF",1600,400);
  chXiQUnQRat->Divide(anatype.size());
  Double_t ratmax=2.5;
  for (int i=0; i<anatype.size(); ++i) {
    // - Near -
    TH1D * h1;
    infiles[i]->GetObject("hXiNearJet",h1);
    TH1D * h2;
    infiles[anatype.size()+i]->GetObject("hXiNearJet",h2);
    TH1D * hrat1 = (TH1D*)h1->Clone();
    // set hist properties
    hrat1->Divide(h1,h2);
    hrat1->SetYTitle("#xi(RadQuench)/#xi(UnQuenched)");
    hrat1->SetAxisRange(0,ratmax,"Y");
    hrat1->SetMarkerSize(mksz);
    chXiQUnQRat->cd(i+1);
    hrat1->Draw();

    // - Away -
    TH1D * h3;
    infiles[i]->GetObject("hXiAwayJet",h3);
    TH1D * h4;
    infiles[anatype.size()+i]->GetObject("hXiAwayJet",h4);
    TH1D * hrat2 = (TH1D*)h3->Clone();
    // set hist properties
    hrat2->Divide(h3,h4);
    hrat2->SetYTitle("#xi(RadQuench)/#xi(UnQuenched)");
    hrat2->SetAxisRange(0,ratmax,"Y");
    hrat2->Draw("same");

    //  - legend -
    TLegend *lXiQUnQRat = new TLegend(0.5,0.6,0.8,0.8);
    lXiQUnQRat->AddEntry(hrat1,"Near Jet","p");
    lXiQUnQRat->AddEntry(hrat2,"Away Jet","p");
    lXiQUnQRat->SetTextSize(0.03);
    hrat2->SetMarkerSize(mksz);
    lXiQUnQRat->Draw();
  }
  // print canvas and save histograms
  printAllCanvases(plotdir);
}
