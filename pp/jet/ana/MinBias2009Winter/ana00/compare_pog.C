#include "compareHist.h"
#include "selectionCut.h"
#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
using namespace std;

void compare_pog(
    Int_t doSel=3,
    char * inf1="data_v21_all.root",
    char * inf2="mc_v21_1M.root",
    TString title1="Data 900 GeV",
    TString title2="Pythia 900 GeV")
{
  TFile * infile1 = new TFile(inf1);
  TTree * tree1 = (TTree*)infile1->FindObjectAny("dijetTree");

  TFile * infile2 = new TFile(inf2);
  TTree * tree2 = (TTree*)infile2->FindObjectAny("dijetTree");

  // setup title
  if (title1==title2) {
    title1 = inf1;
    title2 = inf2;
  }

  int doLog=1;

  double JetEtMin=10;
  double JetEtaMax=2;
  double DJDPhiMin=2.14;
  //selectionCut dataAna(0,doSel,10,3,2.14); // |eta|<3 to compare with jetmet
  selectionCut dataAna(0,doSel,JetEtMin,JetEtaMax,DJDPhiMin); // |eta|<2 for our ana
  dataAna.AnaTag = "V21/anaFragMar11/pog";
  gSystem->mkdir(Form("plots/%s",dataAna.AnaTag.Data()),kTRUE);
  TString djCut = TString(dataAna.DJCut);
  TString trkCut = TString(dataAna.vtxCut && dataAna.TrkCut);
  dataAna.SelTag = Form("Sel%d_jEtMax%.1f_jEtaMin%.1f",doSel,JetEtMin,JetEtaMax);

  cout << "====== total input =====" << endl;
  Double_t totn1 = tree1->GetEntries();
  Double_t totn2 = tree2->GetEntries();
  cout << " tot events in " << inf1 << ": " << totn1 << endl;
  cout << " tot events in " << inf2 << ": " << totn2 << endl;
  cout << "====== selection ===== " << endl;
  cout << "Selection: " << djCut << endl;
  cout << " # passed cuts in " << inf1 << ": " << tree1->GetEntries(djCut) << endl;
  cout << " # passed cuts in " << inf2 << ": " << tree2->GetEntries(djCut) << endl;
  cout << "====== Analysis cuts =====" << endl;
  cout << "djCut:    " << djCut << endl;
  cout << "trkCut: " << trkCut << endl;
  cout << " # evt passed djCut in " << inf1 << ": " << tree1->GetEntries(djCut) << endl;;
  cout << " # evt passed djCut in " << inf2 << ": " << tree2->GetEntries(djCut) << endl;;

  // pdf comparisons
  // check event info
  TCanvas *c0 = new TCanvas("c0","",500,500);
  compareHist comp0(tree1,tree2,"vz","vz","vtxdof>=5","vtxdof>=5",-20,20,40);
  comp0.Normalize(1);
  comp0.SetHistName1(title1);
  comp0.SetHistName2(title2);
  comp0.SetXTitle("vz [cm]");
  comp0.SetYTitle("Arbitrary normalization");
  comp0.SetLegend(0.632,0.828,0.927,0.926);
  comp0.Draw("E");
  c0->Print(Form("plots/%s/vtxdofcut_vz.gif",dataAna.AnaTag.Data()));

  // check dijet
  TCanvas *ccomp2 = new TCanvas("ccomp2","",500,500);
  compareHist comp2(tree1,tree2,"jdphi","dPhi",djCut.Data(),djCut.Data(),0,3.14,30);
  comp2.Normalize(1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("leading dijet d #phi");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetLegend(0.226,0.828,0.520,0.926);
  comp2.SetMaximum(2.8);
  comp2.Draw2("E");
  ccomp2->Print(Form("plots/%s/%s_dPhi.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccomp3 = new TCanvas("ccomp3","",500,500);
  ccomp3->SetLogy(doLog);
  compareHist comp3(tree1,tree2,"mass","djMass",djCut.Data(),djCut.Data(),0,125,30);
  comp3.Normalize(1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("leading dijet mass");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.632,0.828,0.927,0.926);
  comp3.SetMinimum(0.001);
  comp3.Draw2("E");
  ccomp3->Print(Form("plots/%s/%s_djMass.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  // check jet
  TCanvas *ccomp4 = new TCanvas("ccomp4","",500,500);
  ccomp4->SetLogy(doLog);
  compareHist comp4(tree1,tree2,"nljet","djJEt",djCut.Data(),djCut.Data(),10,60,10);
  comp4.AppendToHist(tree1,tree2,"aljet","djJEt",djCut.Data(),djCut.Data());
  comp4.Normalize(1);
  comp4.SetHistName1(title1);
  comp4.SetHistName2(title2);
  comp4.SetXTitle("jet^{1,2} p_{T}");
  comp4.SetYTitle("Arbitrary normalization");
  comp4.SetLegend(0.632,0.828,0.927,0.926);
  comp4.Draw("E");
  ccomp4->Print(Form("plots/%s/%s_djJEt.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccomp5 = new TCanvas("ccomp5","",500,500);
  compareHist comp5(tree1,tree2,"nljeta","djJEta",djCut.Data(),djCut.Data(),-3,3,20);
  comp5.AppendToHist(tree1,tree2,"aljeta","djJEta",djCut.Data(),djCut.Data());
  comp5.Normalize(1);
  comp5.SetHistName1(title1);
  comp5.SetHistName2(title2);
  comp5.SetXTitle("jet^{1,2} #eta");
  comp5.SetYTitle("Arbitrary normalization");
  comp5.SetLegend(0.632,0.828,0.927,0.926);
  comp5.SetMinimum(0);
  comp5.Draw("E");
  ccomp5->Print(Form("plots/%s/%s_djJEta.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccomp6 = new TCanvas("ccomp6","",500,500);
  compareHist comp6(tree1,tree2,"nljphi","djJPhi",djCut.Data(),djCut.Data(),-3.14,3.14,11);
  comp6.AppendToHist(tree1,tree2,"aljphi","djJPhi",djCut.Data(),djCut.Data());
  comp6.Normalize(1);
  comp6.SetHistName1(title1);
  comp6.SetHistName2(title2);
  comp6.SetXTitle("jet^{1,2} #phi");
  comp6.SetYTitle("Arbitrary normalization");
  comp6.SetLegend(0.632,0.828,0.927,0.926);
  comp6.SetMinimum(0);
  comp6.Draw("E");
  ccomp6->Print(Form("plots/%s/%s_djJPhi.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccompemf = new TCanvas("ccompemf","",500,500);
  compareHist compemf(tree1,tree2,"nljemf","djJEmf",djCut.Data(),djCut.Data(),-0.1,1.1,30);
  compemf.AppendToHist(tree1,tree2,"aljemf","djJEmf",djCut.Data(),djCut.Data());
  compemf.Normalize(1);
  compemf.SetHistName1(title1);
  compemf.SetHistName2(title2);
  compemf.SetXTitle("jet^{1,2} EMF");
  compemf.SetYTitle("Arbitrary normalization");
  compemf.SetLegend(0.202,0.828,0.496,0.926);
  compemf.SetMinimum(0);
  compemf.Draw("E");
  ccompemf->Print(Form("plots/%s/%s_djJEmf.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  // check tracks
  cout << " ============== tracks =============" << endl;
  cout << " cut: " << TString(trkCut) << endl;
  TCanvas *ccomp7 = new TCanvas("ccomp7","",500,500);
  ccomp7->SetLogy(1);
  compareHist comp7(tree1,tree2,"ppt","trkPt",trkCut.Data(),trkCut.Data(),0,25,80);
  comp7.Normalize(1);
  comp7.SetHistName1(title1);
  comp7.SetHistName2(title2);
  comp7.SetXTitle("track p_{T}");
  comp7.SetYTitle("Arbitrary normalization");
  comp7.SetLegend(0.632,0.828,0.927,0.926);
  comp7.Draw("E");
  ccomp7->Print(Form("plots/%s/%s_trkPt.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));
}

