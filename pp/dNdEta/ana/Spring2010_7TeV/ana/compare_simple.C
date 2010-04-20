#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TSystem.h"
#include <iostream>
#include <vector>
#include "selectionCut.h"
#include "aliases_tree.C"
#include "helpers.h"
using namespace std;

// === Main function ===
void compare_simple(int doSel = 1, int mode=0,
    const char * datafname="../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root",
    const char * mcfname="../pixel_trees/mc/pixelTree_yilmaz-MinBiasATLAS_RECO_0332_v1.root",
    const char * mc2fname="../pixel_trees/mc/pixelTree_yilmaz-MinBiasPhojet_RECO_0413_v1.root")
{
  // top level info
  TString InspectTag = Form("Sel%d_mode%d",doSel,mode);
  cout << "====== Inspect: " << InspectTag << endl;
  // mkdir dir for output
  TString outdir=Form("plots/inspection");
  gSystem->mkdir(outdir.Data(),kTRUE);

  // get trees
  cout << "data: " << datafname << endl;
  cout << "mc1: " << mcfname << endl;
  cout << "mc2: " << mc2fname << endl;
  TFile * dataFile = new TFile(datafname);
  TFile * mcFile = new TFile(mcfname);
  TFile * mc2File = new TFile(mc2fname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);
  TTree * treeMC2; mc2File->GetObject("PixelTree",treeMC2);
  aliases_tree(treeData);
  aliases_tree(treeMC);
  aliases_tree(treeMC2);

  // Event Selection
  selectionCut dataSel(0,doSel);
  selectionCut mcSel(1,doSel);

  // cuts
  TCut type1Cut("evtType==92 || evtType==93 || evtType==94");
  TCut type2Cut("evtType!=92 && evtType!=93 && evtType!=94");
  TCut type1PhoCut("evtType==5 || evtType==6 || evtType==7 || evtType==4");
  TCut type2PhoCut("evtType==1");


  // draw from tree
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TString VzBins("(100,-20,20)");
  treeData->Draw(Form("vz[1]>>hVz_data%s",VzBins.Data()),dataSel.Cut,"");
  treeMC->Draw(Form("vz[1]-0.4847>>hVz_MC1%s",VzBins.Data()),mcSel.Cut,"same");
  treeMC2->Draw(Form("vz[1]-0.4847>>hVz_MC2%s",VzBins.Data()),mcSel.Cut,"same");
  normHist(hVz_data,11);
  normHist(hVz_MC1,11);
  normHist(hVz_MC2,11);
  hVz_data->SetMarkerStyle(kFullCircle);
  hVz_MC1->SetMarkerStyle(kOpenCircle);
  hVz_MC2->SetMarkerStyle(kOpenSquare);
  hVz_MC1->SetMarkerColor(kRed);
  hVz_MC2->SetMarkerColor(kBlue);
  hVz_MC1->SetLineColor(kRed);
  hVz_MC2->SetLineColor(kBlue);
  c2->Update();
  // declare legend
  TLegend *leg2 = new TLegend(0.595,0.8,0.845,0.927,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.03);
  leg2->AddEntry("","Selection with","");
  leg2->AddEntry("",dataSel.Tag.Data(),"");
  leg2->AddEntry(hVz_data,"Data 7TeV (Run 132440)","p");
  leg2->AddEntry(hVz_MC1,"PYTHIA Atlas","p");
  leg2->AddEntry(hVz_MC2,"PHOJET","p");
  cout << "declared legend" << endl;
  c2->Print(Form("%s/%s_hVz.gif",outdir.Data(),InspectTag.Data()));
  c2->Print(Form("%s/%s_hVz.eps",outdir.Data(),InspectTag.Data()));
}
