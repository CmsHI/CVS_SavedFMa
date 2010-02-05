#include "compareHist.h"
#include "selectionCut.h"
#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include "TTree.h"
using namespace std;

void compare_data_mc(
    Int_t doSel=1,
    char * inf1="ProcessedTree_ak5calo_MB_V02_8M_2.root",
    char * inf2="ProcessedTree_ak5calo_Summer09_STARTUP3X_V8P_900GeV_Jan29ReReco-v1_V03.root",
    TString title1="Data: 900 GeV Good Runs",
    TString title2="MC: 900 GeV PYTHIA")
{
  TFile * infile1 = new TFile(inf1);
  TTree * tree1 = (TTree*)infile1->FindObjectAny("tr");

  TFile * infile2 = new TFile(inf2);
  TTree * tree2 = (TTree*)infile2->FindObjectAny("tr");

  // setup title
  if (title1==title2) {
    title1 = inf1;
    title2 = inf2;
  }

  int doLog=1;

  selectionCut dataSel(0,doSel);
  selectionCut mcSel(1,doSel);
  TString dataCut = TString(dataSel.Cut);
  TString mcCut = TString(mcSel.Cut);
  cout << "====== selection ===== " << endl;
  cout << " # passed cuts in data: " << tree1->GetEntries(dataCut) << endl;;
  cout << " # passed cuts in mc: " << tree2->GetEntries(mcCut) << endl;;

  TCanvas *cV = new TCanvas("cV","",500,500);
  compareHist compV(tree1,tree2,"eventVar.PVz","PVz",dataCut.Data(),mcCut.Data(),-15,15,15,1);
  compV.SetHistName1(title1);
  compV.SetHistName2(title2);
  compV.SetXTitle("Selected PVz");
  compV.SetYTitle("Arbitrary normalization");
  //compV.SetMarkerSize(1);
  compV.SetLegend(0.6,0.75,0.95,0.99);
  compV.Draw("hist E");
  cV->Print(Form("hSelPVz_Sel%d.gif",doSel));

  TCanvas *c2 = new TCanvas("c2","",500,500);
  c2->SetLogy();
  compareHist comp2(tree1,tree2,"jet1Var.corPt","LeadJetCorPt",dataCut.Data(),mcCut.Data(),15,55,20,1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("Leading Jet Pt^{Corr}");
  comp2.SetYTitle("Arbitrary normalization");
  //comp2.SetMarkerSize(1);
  comp2.SetLegend(0.6,0.75,0.95,0.99);
  comp2.Draw("hist E");
  c2->Print(Form("hLeadJetCorPt_Sel%d.gif",doSel));

  TCanvas *c3 = new TCanvas("c3","",500,500);
  compareHist comp3(tree1,tree2,"jet1Var.eta","LeadJetEta",dataCut.Data(),mcCut.Data(),-4,4,20,1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("Leading Jet #eta");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.6,0.75,0.95,0.95);
  comp3.Draw("hist E");
  c3->Print(Form("hLeadJetEta_Sel%d.gif",doSel));

  TCanvas *c4 = new TCanvas("c4","",500,500);
  compareHist comp4(tree1,tree2,"jet1Var.phi","LeadJetPhi",dataCut.Data(),mcCut.Data(),-3.14,3.14,10,1);
  comp4.SetHistName1(title1);
  comp4.SetHistName2(title2);
  comp4.SetXTitle("Leading Jet #phi");
  comp4.SetYTitle("Arbitrary normalization");
  comp4.SetMinimum(0);
  comp4.SetLegend(0.6,0.75,0.95,0.95);
  comp4.Draw("hist E");
  c4->Print(Form("hLeadJetPhi_Sel%d.gif",doSel));

  TCanvas *c5 = new TCanvas("c5","",500,500);
  compareHist comp5(tree1,tree2,"jet1Var.emf","LeadJetEMF",dataCut.Data(),mcCut.Data(),0,1,20,1);
  comp5.SetHistName1(title1);
  comp5.SetHistName2(title2);
  comp5.SetXTitle("Leading Jet EMF");
  comp5.SetYTitle("Arbitrary normalization");
  comp5.SetLegend(0.6,0.75,0.95,0.95);
  comp5.Draw("hist E");
  c5->Print(Form("hLeadJetEMF_Sel%d.gif",doSel));

  TCanvas *c6 = new TCanvas("c6","",500,500);
  compareHist comp6(tree1,tree2,"jet1Var.n90hits","LeadJetN90",dataCut.Data(),mcCut.Data(),0,30,30,1);
  comp6.SetHistName1(title1);
  comp6.SetHistName2(title2);
  comp6.SetXTitle("Leading Jet n90hits");
  comp6.SetYTitle("Arbitrary normalization");
  comp6.SetLegend(0.6,0.75,0.95,0.95);
  comp6.Draw("hist E");
  c6->Print(Form("hLeadJetN90hits_Sel%d.gif",doSel));
}

