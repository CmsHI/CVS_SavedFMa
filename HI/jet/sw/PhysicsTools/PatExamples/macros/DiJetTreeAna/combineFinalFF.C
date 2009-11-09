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

void combineFinalFF(char * tag = "sw48")
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

  vector<TFile*> infiles;
  for (int igen=0; igen<gen.size(); ++igen) {
    for (int itype=0; itype<anatype.size(); ++itype) {
      infiles.push_back(new TFile(Form("%s/%s/%s/jet_125_300_100_jdphi_2.85_%s_%s/jFF/FFHistos.root",
	      indir,gen[igen].Data(),tag,tag,anatype[itype].Data())));
      infiles.back()->Print();
    }
  }

  // outout
  char plotdir[1000];
  sprintf(plotdir,"%s/%s/combinedPlots",indir,tag);
  gSystem->mkdir(plotdir,kTRUE);
  TFile * outfile = new TFile(Form("%s/combineFinalFFHistos.root",plotdir),"RECREATE");

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
  TH1D * SubXiNears[2][4];
  TH1D * SubXiAways[2][4];

  // histo config
  Double_t ratmax=2.5;

  // === FF raw===
  TCanvas * chRawXi = new TCanvas("chRawXi","FF",1200,600);
  chRawXi->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chRawXi->cd(i+1);
    TH1D * hXiNearJet = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hXiNearJet",drsgE,"","","",-1,0,0,0,0,mksz,mkstNear,0,plotmode));
    TH1D * hXiAwayJet = dynamic_cast<TH1D*>(drawNormHist(infiles[i],"hXiAwayJet",drdbE,"","","",-1,0,0,0,0,mksz,mkstAway,0,plotmode));
    //  - legend -
    TLegend *lXi = new TLegend(0.2,0.6,0.5,0.80);
    lXi->AddEntry(hXiNearJet,"Near Jet","L");
    lXi->AddEntry(hXiAwayJet,"Away Jet","L");
    lXi->SetTextSize(0.03);
    lXi->Draw();
  }

  // === raw FF ratios ===
  // -- Q/UnQ
  TCanvas * chRawXiQUnQRat = new TCanvas("chRawXiQUnQRat","FF",1600,400);
  chRawXiQUnQRat->Divide(anatype.size());
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
    chRawXiQUnQRat->cd(i+1);
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

  // === FF bg sub===
  // near
  TCanvas * chXiNear = new TCanvas("chXiNear","near FF",1200,600);
  chXiNear->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chXiNear->cd(i+1);
    // background subtraction
    TH1D * hXiNearJet; infiles[i]->GetObject("hXiNearJet",hXiNearJet);
    hXiNearJet->SetMarkerSize(mksz);
    TH1D * hBgXiNearJet; infiles[i]->GetObject("hBgXiNearJet",hBgXiNearJet);
    TH1D * hSubXiNearJet = (TH1D*)hXiNearJet->Clone(Form("hSubXiNearJet_%s_%s",gen[i/4].Data(),anatype[i%4].Data()));
    cout << "sub xi: " << Form("hSubXiNearJet_%s_%s",gen[i/4].Data(),anatype[i%4].Data()) << endl;
    hSubXiNearJet->SetMarkerSize(mksz);
    hSubXiNearJet->SetMarkerColor(kRed);
    // now subtract
    hSubXiNearJet->Add(hBgXiNearJet,-1);
    // plot
    hXiNearJet->Draw();
    hBgXiNearJet->Draw("hist same");
    hSubXiNearJet->Draw("same");
    //  - legend -
    TLegend *lXi = new TLegend(0.2,0.6,0.5,0.80);
    lXi->AddEntry(hXiNearJet,"Near Jet, No Sub","P");
    lXi->AddEntry(hBgXiNearJet,"Near Jet, bg","L");
    lXi->AddEntry(hSubXiNearJet,"Near Jet, Bg Subtracted","P");
    lXi->SetTextSize(0.03);
    lXi->Draw();
  }
  // Away
  TCanvas * chXiAway = new TCanvas("chXiAway","near FF",1200,600);
  chXiAway->Divide(anatype.size(),gen.size());
  for (int i=0; i<infiles.size(); ++i) {
    chXiAway->cd(i+1);
    // background subtraction
    TH1D * hXiAwayJet; infiles[i]->GetObject("hXiAwayJet",hXiAwayJet);
    hXiAwayJet->SetMarkerSize(mksz);
    TH1D * hBgXiAwayJet; infiles[i]->GetObject("hBgXiAwayJet",hBgXiAwayJet);
    TH1D * hSubXiAwayJet = (TH1D*)hXiAwayJet->Clone(Form("hSubXiAwayJet_%s_%s",gen[i/4].Data(),anatype[i%4].Data()));
    cout << "sub xi: " << Form("hSubXiAwayJet_%s_%s",gen[i/4].Data(),anatype[i%4].Data()) << endl;
    hSubXiAwayJet->SetMarkerSize(mksz);
    hSubXiAwayJet->SetMarkerColor(kRed);
    // now subtract
    hSubXiAwayJet->Add(hBgXiAwayJet,-1);
    // plot
    hXiAwayJet->Draw();
    hBgXiAwayJet->Draw("hist same");
    hSubXiAwayJet->DrawClone("same");
    //  - legend -
    TLegend *lXi = new TLegend(0.2,0.6,0.5,0.80);
    lXi->AddEntry(hXiAwayJet,"Away Jet, No Sub","P");
    lXi->AddEntry(hBgXiAwayJet,"Away Jet, bg","L");
    lXi->AddEntry(hSubXiAwayJet,"Away Jet, Bg Subtracted","P");
    lXi->SetTextSize(0.03);
    lXi->Draw();
  }

  // === FF ratios ===
  // -- Q/UnQ
  TCanvas * chXiQUnQRat = new TCanvas("chXiQUnQRat","FF",1600,400);
  chXiQUnQRat->Divide(anatype.size());
  for (int i=0; i<anatype.size(); ++i) {
    // - Near -
    //infiles[i]->GetObject("hXiNearJet",h1);
    //infiles[anatype.size()+i]->GetObject("hXiNearJet",h2);
    //cout << "rat: " << Form("hSubXiNearJet_%s_%s",gen[0].Data(),anatype[i].Data()) << endl;
    TH1D * h1 = (TH1D*)gDirectory->Get(Form("hSubXiNearJet_%s_%s",gen[0].Data(),anatype[i].Data()));;
    //cout << h1 << endl;
    //cout << "rat: " << Form("hSubXiNearJet_%s_%s",gen[1].Data(),anatype[i].Data()) << endl;
    TH1D * h2 = (TH1D*)gDirectory->Get(Form("hSubXiNearJet_%s_%s",gen[1].Data(),anatype[i].Data()));;
    //TH1D * hrat1 = (TH1D*)h1->Clone(Form("hSubXiNearJetRat_%s",anatype[i].Data()));
    TH1D * hrat1 = (TH1D*)h1->Clone();
    //TH1D * hrat1 = new TH1D("hrat1","hrat1",25,0,7);
    // set hist properties
    hrat1->Divide(h1,h2);
    hrat1->SetYTitle("#xi(RadQuench)/#xi(UnQuenched)");
    hrat1->SetAxisRange(0,ratmax,"Y");
    hrat1->SetMarkerSize(mksz);
    hrat1->SetMarkerColor(kBlack);
    hrat1->SetLineColor(kBlack);
    chXiQUnQRat->cd(i+1);
    hrat1->Draw();

    // - Away -
//    TH1D * h3;
//    infiles[i]->GetObject("hXiAwayJet",h3);
//    TH1D * h4;
//    infiles[anatype.size()+i]->GetObject("hXiAwayJet",h4);
    cout << "rat: " << Form("hSubXiAwayJet_%s_%s",gen[0].Data(),anatype[i].Data()) << endl;
    cout << "rat: " << Form("hSubXiAwayJet_%s_%s",gen[1].Data(),anatype[i].Data()) << endl;
    TH1D * h3 = (TH1D*)gDirectory->Get(Form("hSubXiAwayJet_%s_%s",gen[0].Data(),anatype[i].Data()));;
    TH1D * h4 = (TH1D*)gDirectory->Get(Form("hSubXiAwayJet_%s_%s",gen[1].Data(),anatype[i].Data()));;
    TH1D * hrat2 = (TH1D*)h3->Clone();
    // set hist properties
    hrat2->Divide(h3,h4);
    hrat2->SetYTitle("#xi(RadQuench)/#xi(UnQuenched)");
    hrat2->SetAxisRange(0,ratmax,"Y");
    hrat2->SetMarkerColor(kBlue);
    hrat2->SetLineColor(kBlue);
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
  outfile->Write();
  outfile->Close();

}
