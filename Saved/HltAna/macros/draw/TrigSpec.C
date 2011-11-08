#include <iostream>
#include "TStyle.h"
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TProfile.h>
#include "TProfile2D.h"
#include "TLine.h"
#include "TLegend.h"


void TrigSpec(){
  //gStyle->SetLineWidth(2);
  TH1::SetDefaultSumw2();
  TFile * infdata = new TFile("../ntout/L1Tree_mb_mctag_l1hlt_v3.root");
  TTree * tdata = (TTree*)infdata->Get("hltbitnew/HltTree");
  tdata->AddFriend("icPu5JetAnalyzer/t");
  
  // Track Selection
  TCut evtSel = "";
  TCut dataSel = "";
  
  // 1D
  TH1D * hJetPt0 = new TH1D("hJetPt0","",50,0,150);
  TH1D * hJetPt1 = new TH1D("hJetPt1","",50,0,150);
  TH1D * hJetPt2 = new TH1D("hJetPt2","",50,0,150);
  TH1D * hJetPt3 = new TH1D("hJetPt3","",50,0,150);
  TH1D * hJetPt4 = new TH1D("hJetPt4","",50,0,150);
  TH1D * hJetPt5 = new TH1D("hJetPt5","",50,0,150);
  TH1D * hJetPt6 = new TH1D("hJetPt6","",50,0,150);
  tdata->Project("hJetPt0","jtpt[0]",evtSel&&dataSel);
  tdata->Project("hJetPt1","jtpt[0]",evtSel&&dataSel&&"HLT_HIJet55");
  tdata->Project("hJetPt2","jtpt[0]",evtSel&&dataSel&&"HLT_HIJet65");
  tdata->Project("hJetPt3","jtpt[0]",evtSel&&dataSel&&"HLT_HIJet80");
  tdata->Project("hJetPt4","jtpt[0]",evtSel&&dataSel&&"HLT_HIJet95");
  tdata->Project("hJetPt5","jtpt[0]",evtSel&&dataSel&&"HLT_HIDiJet55and55");
  tdata->Project("hJetPt6","jtpt[0]",evtSel&&dataSel&&"HLT_HIDiJet65and55");
  
  // Draw
  TCanvas * c2 = new TCanvas("c2","",500,500);
  gPad->SetLogy();
  hJetPt0->SetAxisRange(0,200,"X");
  hJetPt0->SetAxisRange(0.2,1e5,"Y");
  hJetPt0->GetXaxis()->CenterTitle();
  hJetPt0->GetYaxis()->CenterTitle();
  hJetPt0->SetTitle(";Leading Jet p_{T} (GeV/c); Event Count");
  hJetPt1->SetLineColor(kGray+2);
  hJetPt2->SetLineColor(kBlue);
  hJetPt3->SetLineColor(kGreen+2);
  hJetPt4->SetLineColor(kMagenta);
  hJetPt5->SetLineColor(kYellow+2);
  hJetPt6->SetLineColor(kRed);
  hJetPt0->Draw("hist");
  hJetPt1->Draw("samehist");
  hJetPt2->Draw("samehist");
  hJetPt3->Draw("samehist");
  hJetPt4->Draw("samehist");
  hJetPt5->Draw("samehist");
  hJetPt6->Draw("samehist");
  
  TLegend *leg = new TLegend(0.54,0.60,0.97,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hJetPt0,"HLT_HIMBHfOrBSC","l");
  leg->AddEntry(hJetPt1,"HLT_HIJet55","l");
  leg->AddEntry(hJetPt2,"HLT_HIJet65","l");
  leg->AddEntry(hJetPt3,"HLT_HIJet80","l");
  leg->AddEntry(hJetPt4,"HLT_HIJet95","l");
  leg->AddEntry(hJetPt5,"HLT_HIDiJet55","l");
  leg->AddEntry(hJetPt6,"HLT_HIJet65_Jet55","l");
  leg->Draw();
//  
//  c2->cd(2);
//  TH1D * hRat2 = (TH1D*)hJetPt2->Clone("hTrigTurnOn1");
//  hRat2->Divide(hJetPt0);
//  TH1D * hRat3 = (TH1D*)hJetPt3->Clone("hTrigTurnOn2");
//  hRat3->Divide(hJetPt0);
//  hRat2->SetAxisRange(0,1.2,"Y");
//  hRat2->Draw("p");
//  hRat3->Draw("samep");
//  c2->Print(Form("out/TrigEff_%s.gif",tag.Data()));
//
//  fout->Write();
}
