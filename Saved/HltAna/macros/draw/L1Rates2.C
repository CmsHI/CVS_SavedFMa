#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TLine.h"
using namespace std;

void L1Rates2(TString infname="../ntout/L1Tree_mb_datatag_l1hlt.root")
{
  TFile * inf = new TFile(infname);
  
  TTree * t = (TTree*)inf->Get("hltbitnew/HltTree");
  t->AddFriend("l1NtupleProducer/L1Tree");
  t->AddFriend("icPu5JetAnalyzer/t");
  
  TH1D * hPt = new TH1D("hPt",";L1 Jet p_{T} (GeV/c);frequency",50,0,400);
  TH1D * hEta = new TH1D("hEta",";L1 Jet #eta;frequency",22,-5,5);
  TH1D * hPhi = new TH1D("hPhi",";L1 Jet #phi;frequency",18,-3.14,3.14);
  TH2D * hPtEta = new TH2D("hPtEta",";L1 Jet #eta;L1 Jet p_{T} (GeV/c)",22,-5,5,50,0,400);
  
  TCut sel;
  //TCut sel = "L1_SingleJet36&&Rankjet>=Max$(Rankjet)";
  //TCut sel = "Rankjet>=Max$(Rankjet)&&Rankjet>=9&&Etajet>4&&Etajet<17";
  //TCut sel = "L1CenJetEt>=Max$(L1CenJetEt)&&L1CenJetEt>=36";
  
  float numTot = t->GetEntries();
  //float numSel = t->Draw("Rankjet*4>>hPt",sel,"goff");
  //t->Draw("Etajet>>hEta",sel,"goff");
  //t->Draw("Phijet>>hPhi",sel,"goff");
  //t->Draw("Rankjet*4:Etajet>>hPtEta",sel,"goff");
  float numSel = t->Draw("L1CenJetEt>>hPt",sel,"goff");
  t->Draw("L1CenJetEta>>hEta",sel,"goff");
  t->Draw("L1CenJetPhi>>hPhi",sel,"goff");
  t->Draw("L1CenJetEt:L1CenJetEta>>hPtEta",sel,"goff");
  
  // Summary
  cout << "total entries in tree: " << numTot << endl;
  cout << "number selected: " << numSel << ", frac: " << numSel/numTot << endl;
  //cout << "L1 cut: " << t->GetEntries("L1_SingleJet36") << endl;
  //cout << "Rank evt cut: " << t->GetEntries("Max$(Rankjet)>=9") << endl;
  
  // Draw
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetLogy();
  hPt->Draw();
  c2->Print("L1JetPt.gif");

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  c3->SetLogy();
  hEta->Draw();
  c3->Print("L1JetEta.gif");
  
  TCanvas * c4 = new TCanvas("c4","c4",500,500);
  hPhi->SetMinimum(0);
  hPhi->Draw();
  c4->Print("L1JetPhi.gif");
  
  TCanvas * c5 = new TCanvas("c5","c5",500,500);
  c5->SetRightMargin(0.15);
  hPtEta->Draw("colz");
  c5->Print("L1JetPtEta.gif");
}