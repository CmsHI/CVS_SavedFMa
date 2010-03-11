#include "compareHist.h"
#include "selectionCut.h"
#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TSystem.h"
#include "aliases_dijet.C"
#include "AnaFrag.h"
using namespace std;

void jetFragAna(
    Int_t doSel=3,
    char * inf1="data_v21_all.root",
    char * inf2="mc_v21_1M.root",
    TString title1="Data 900 GeV",
    TString title2="Pythia 900 GeV")
{
  TFile * infile1 = new TFile(inf1);
  TTree * tree1 = (TTree*)infile1->FindObjectAny("dijetTree");
  aliases_dijet(tree1);

  TFile * infile2 = new TFile(inf2);
  TTree * tree2 = (TTree*)infile2->FindObjectAny("dijetTree");
  aliases_dijet(tree2);

  TFile* outf = new TFile("dijetAna.root","recreate");
  TH1::SetDefaultSumw2();
  // setup title
  if (title1==title2) {
    title1 = inf1;
    title2 = inf2;
  }

  int doLog=1;

  // ana config's
  double AnaJetEtMin = 10.;
  double AnaJetEtaMax = 2.;
  selectionCut dataAna(0,doSel,AnaJetEtMin,AnaJetEtaMax,2.14);
  dataAna.AnaTag = "V21/anaFragMar11";
  gSystem->mkdir(Form("plots/%s",dataAna.AnaTag.Data()),kTRUE);
  TString djCut = TString(dataAna.DJCut);
  //TString trkCut = TString(dataAna.vtxCut && dataAna.TrkCut);
  TString djTrkCut = TString(dataAna.DJCut && dataAna.TrkCut);
  dataAna.SelTag = Form("Sel%d_jEtMax%.1f_jEtaMin%.1f",doSel,AnaJetEtMin,AnaJetEtaMax);
  cout << "AnaTag: " << dataAna.SelTag << endl;

  cout << "====== total input =====" << endl;
  Double_t totn1 = tree1->GetEntries();
  Double_t totn2 = tree2->GetEntries();
  cout << " tot events in " << inf1 << ": " << totn1 << endl;
  cout << " tot events in " << inf2 << ": " << totn2 << endl;
  cout << "====== Analysis cuts =====" << endl;
  cout << "djCut:    " << djCut << endl;
  cout << "djTrkCut: " << djTrkCut << endl;
  cout << " # evt passed djCut in " << inf1 << ": " << tree1->GetEntries(djCut) << endl;;
  cout << " # evt passed djCut in " << inf2 << ": " << tree2->GetEntries(djCut) << endl;;

  // pdf comparisons
  // === dijet ana ===
  cout << endl << "==================== DiJet Ana ===================" << endl;
  TCanvas *ctemp = new TCanvas("ctemp","ctemp",500,500);
  // data
  AnaFrag dataNr(0,"dataNr",tree1,djCut,djTrkCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag dataAw(0,"dataAw",tree1,djCut,djTrkCut,"log(1/za)","padr<0.5","padrbg<0.5");
  TH1D * hDataCombXiRaw = (TH1D*)dataNr.hXiRaw->Clone("hDataCombXiRaw");
  TH1D * hDataCombXiBkg = (TH1D*)dataNr.hXiBkg->Clone("hDataCombXiBkg");
  TH1D * hDataCombXiSig = (TH1D*)dataNr.hXiSig->Clone("hDataCombXiSig");
  hDataCombXiRaw->Add(dataNr.hXiRaw,dataAw.hXiRaw,0.5,0.5);
  hDataCombXiRaw->SetMinimum(dataNr.ymin);
  hDataCombXiRaw->SetMaximum(dataNr.ymax);
  hDataCombXiBkg->Add(dataNr.hXiBkg,dataAw.hXiBkg,0.5,0.5);
  hDataCombXiBkg->SetMinimum(dataNr.ymin);
  hDataCombXiBkg->SetMaximum(dataNr.ymax);
  hDataCombXiSig->Add(dataNr.hXiSig,dataAw.hXiSig,0.5,0.5);
  hDataCombXiSig->SetMinimum(dataNr.ymin);
  hDataCombXiSig->SetMaximum(dataNr.ymax);

  // mc
  AnaFrag mcNr(1,"mcNr",tree2,djCut,djTrkCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcAw(1,"mcAw",tree2,djCut,djTrkCut,"log(1/za)","padr<0.5","padrbg<0.5");
  TH1D * hMCCombXiRaw = (TH1D*)mcNr.hXiRaw->Clone("hMCCombXiRaw");
  TH1D * hMCCombXiBkg = (TH1D*)mcNr.hXiBkg->Clone("hMCCombXiBkg");
  TH1D * hMCCombXiSig = (TH1D*)mcNr.hXiSig->Clone("hMCCombXiSig");
  hMCCombXiRaw->Add(mcNr.hXiRaw,mcAw.hXiRaw,0.5,0.5);
  hMCCombXiRaw->SetMinimum(mcNr.ymin);
  hMCCombXiRaw->SetMaximum(mcNr.ymax);
  hMCCombXiBkg->Add(mcNr.hXiBkg,mcAw.hXiBkg,0.5,0.5);
  hMCCombXiBkg->SetMinimum(mcNr.ymin);
  hMCCombXiBkg->SetMaximum(mcNr.ymax);
  hMCCombXiSig->Add(mcNr.hXiSig,mcAw.hXiSig,0.5,0.5);
  hMCCombXiSig->SetMinimum(mcNr.ymin);
  hMCCombXiSig->SetMaximum(mcNr.ymax);

  TLegend * leg1 = new TLegend(0.522,0.828,0.927,0.926,NULL,"brNDC");
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetTextSize(0.035);
  leg1->AddEntry("","j1,j2","");
  leg1->AddEntry(dataNr.hXiRaw,"Data 900 GeV Raw","pl");
  leg1->AddEntry(dataNr.hXiBkg,"Background","pl");

  TLegend * leg2 = new TLegend(0.468,0.778,0.873,0.928,NULL,"brNDC");
  leg2->SetFillStyle(0);
  leg2->SetFillColor(0);
  leg2->SetTextSize(0.035);
  leg2->AddEntry("","j1,j2 FF","");
  leg2->AddEntry(dataNr.hXiRaw,"Data 900 GeV Raw","pl");
  leg2->AddEntry(dataNr.hXiBkg,"Data Background","pl");
  leg2->AddEntry(mcNr.hXiRaw,"Pythia 900 GeV Raw","pl");
  leg2->AddEntry(mcNr.hXiBkg,"Pythia Background","pl");

  TLegend * leg3 = new TLegend(0.468,0.8,0.873,0.926,NULL,"brNDC");
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetTextSize(0.035);
  leg3->AddEntry("","j1,j2 FF","");
  leg3->AddEntry(dataNr.hXiSig,"Data 900 GeV, Raw-Bg","pl");
  leg3->AddEntry(mcNr.hXiSig,"Pythia 900 GeV, Raw-Bg","pl");

  TCanvas *cFF1 = new TCanvas("cFF1","cFF1",500,500);
  hDataCombXiRaw->Draw("E");
  hDataCombXiBkg->Draw("Esame");
  leg1->Draw();
  //cFF1->Print(Form("plots/%s/%s_XiData.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *cFF2 = new TCanvas("cFF2","cFF2",500,500);
  hMCCombXiRaw->Draw("hist E");
  hMCCombXiBkg->Draw("hist Esame");
  hDataCombXiRaw->Draw("Esame");
  hDataCombXiBkg->Draw("Esame");
  leg2->Draw();
  //cFF2->Print(Form("plots/%s/%s_XiMC.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *cFF3 = new TCanvas("cFF3","cFF3",500,500);
  hDataCombXiSig->Draw("E");
  hMCCombXiSig->Draw("hist E same");
  leg3->Draw();
  cFF3->Print(Form("plots/%s/%s_XiSigRawDataMC.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  /*
  TCanvas *cFF1 = new TCanvas("cFF1","cFF1",500,500);
  dataNr.hXiRaw->Draw("E");
  dataNr.hXiBkg->Draw("Esame");
  //leg1->Draw();
  */
}

