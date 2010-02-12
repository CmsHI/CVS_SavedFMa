#include "compareHist.h"
//#include "selectionCut.h"
#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
using namespace std;

void compare_energy(
    Int_t doSel=1,
    char * inf1="jet_ana_v3_2_et_order_4TeV.root",
    char * inf2="jet_ana_v3_2_et_order_2.8TeV.root",
    TString title1="Hydjet 4TeV",
    TString title2="Hydjet 2.8 TeV")
{
  TFile * infile1 = new TFile(inf1);
  TTree * tree1 = (TTree*)infile1->FindObjectAny("jets");

  TFile * infile2 = new TFile(inf2);
  TTree * tree2 = (TTree*)infile2->FindObjectAny("jets");

  // setup title
  if (title1==title2) {
    title1 = inf1;
    title2 = inf2;
  }

  int doLog=1;

  TString djCut;
  if (doSel==1) djCut="njet>50 && ajet>40 && dphi>2.2 && abs(njeta)<2 && abs(ajeta)<2";
  cout << "====== total input =====" << endl;
  Double_t totn1 = tree1->GetEntries();
  Double_t totn2 = tree2->GetEntries();
  Double_t totnRat = totn2/totn1;
  cout << " tot events in 4.0TeV: " << totn1 << endl;
  cout << " tot events in 2.8TeV: " << totn2 << endl;
  cout << " ratio: " << totnRat << endl;
  cout << "====== selection ===== " << endl;
  cout << "Selection: " << djCut << endl;
  cout << " # passed cuts in 4.0TeV: " << tree1->GetEntries(djCut) << endl;;
  cout << " # passed cuts in 2.8TeV: " << tree2->GetEntries(djCut) << endl;;

  // yield as a function of pt
  TCanvas *cYieldPt = new TCanvas("cYieldPt","",500,500);
  cYieldPt->SetLogy();
  Double_t etBins[] = { 50,55,60,65,70,75,80,90,100,150 };
  TH1D * hYieldPt1 = new TH1D("hYieldPt1",";E_{T}^{leading near,away jets} [GeV]; # / 10 GeV",10,50,150);
  TH1D * hYieldPt2 = new TH1D("hYieldPt2",";E_{T}^{leading near,away jets} [GeV]; # / 10 GeV",10,50,150);
  hYieldPt2->SetMarkerColor(kRed);
  hYieldPt2->SetLineColor(kRed);
  hYieldPt1->Sumw2();
  hYieldPt2->Sumw2();
  tree1->Draw("njet>>hYieldPt1",djCut,"E");
  tree1->Draw("ajet>>+hYieldPt1",djCut,"E");
  tree2->Draw("njet>>hYieldPt2",djCut,"E");
  tree2->Draw("ajet>>+hYieldPt2",djCut,"E");
  // scale 4TeV sample to the same total events as 2.8 TeV Sample
  hYieldPt1->Scale(totnRat);
  hYieldPt1->Draw("E");
  hYieldPt2->Draw("E same");
  TLegend *leg = new TLegend(0.6154,0.714,.9176,0.913,NULL,"brNDC");
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->AddEntry("","1M Events","");
  leg->AddEntry(hYieldPt1,"Hydjet 4.0 TeV","pl");
  leg->AddEntry(hYieldPt2,"Hydjet 2.8 TeV","pl");
  leg->Draw();
  cYieldPt->Print(Form("plots/Sel%d_YieldPt.gif",doSel));

  TCanvas *cYieldPtRat = new TCanvas("cYieldPtRat","",500,500);
  TH1D * hYieldPtRat = (TH1D*)hYieldPt2->Clone("hYieldPtRat");
  hYieldPtRat->Divide(hYieldPt1);
  hYieldPtRat->SetYTitle("2.8 TeV Yield / 4 TeV Yield");
  hYieldPtRat->Draw("E");
  hYieldPtRat->Fit("pol1","RF","",50,150);
  cYieldPtRat->Print(Form("plots/Sel%d_YieldPtRat.gif",doSel));

  // pdf comparisons
  TCanvas *c2 = new TCanvas("c2","",500,500);
  compareHist comp2(tree1,tree2,"dphi","dPhi",djCut.Data(),djCut.Data(),2.,3.14,30,1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("leading dijet d #phi");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetLegend(0.185,0.65,0.53,0.89);
  comp2.Draw("E");
  c2->Print(Form("plots/Sel%d_dPhi.gif",doSel));

  TCanvas *ccomp3 = new TCanvas("ccomp3","",500,500);
  compareHist comp3(tree1,tree2,"njeta","NJEta",djCut.Data(),djCut.Data(),-2.5,2.5,10,1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("leading near jet #eta");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.578,0.165,0.923,0.407);
  comp3.Draw("E");
  ccomp3->Print(Form("plots/Sel%d_NJEta.gif",doSel));

  TCanvas *ccomp4 = new TCanvas("ccomp4","",500,500);
  compareHist comp4(tree1,tree2,"ajeta","AJEta",djCut.Data(),djCut.Data(),-2.5,2.5,10,1);
  comp4.SetHistName1(title1);
  comp4.SetHistName2(title2);
  comp4.SetXTitle("leading away jet #eta");
  comp4.SetYTitle("Arbitrary normalization");
  comp4.SetLegend(0.578,0.165,0.923,0.407);
  comp4.Draw("E");
  ccomp4->Print(Form("plots/Sel%d_AJEta.gif",doSel));

  TCanvas *c5 = new TCanvas("c5","",500,500);
  c5->SetLogz();
  TH2D * hAN1 = new TH2D("hAN1",";E_{T}^{leading near jet};E_{T}^{leading away jet}",15,0,150,15,0,150);
  tree1->Draw("ajet:njet>>hAN1",djCut,"colz");
  c5->Print("plots/hAN1.gif");
  TCanvas *c6 = new TCanvas("c6","",500,500);
  c6->SetLogz();
  TH2D * hAN2 = new TH2D("hAN2",";E_{T}^{leading near jet};E_{T}^{leading away jet}",15,0,150,15,0,150);
  tree2->Draw("ajet:njet>>hAN2",djCut,"colz");
  c6->Print("plots/hAN2.gif");
}

