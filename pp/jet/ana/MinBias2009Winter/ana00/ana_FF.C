#include "compareHist.h"
#include "selectionCut.h"
#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "aliases_dijet.C"
using namespace std;

void ana_FF(
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
  dataAna.AnaTag = "V21/FF";
  gSystem->mkdir(Form("plots/%s",dataAna.AnaTag.Data()),kTRUE);
  TString djCut = TString(dataAna.DJCut);
  TString trkCut = TString(dataAna.vtxCut && dataAna.TrkCut);
  TString djTrkCut = TString(dataAna.DJCut && dataAna.TrkCut);
  dataAna.SelTag = Form("Sel%d_jEtMax%.1f_jEtaMin%.1f",doSel,AnaJetEtMin,AnaJetEtaMax);
  cout << "AnaTag: " << dataAna.SelTag << endl;

  cout << "====== total input =====" << endl;
  Double_t totn1 = tree1->GetEntries();
  Double_t totn2 = tree2->GetEntries();
  cout << " tot events in " << inf1 << ": " << totn1 << endl;
  cout << " tot events in " << inf2 << ": " << totn2 << endl;
  cout << "====== selection ===== " << endl;
  cout << "Selection: " << djCut << endl;
  cout << " # passed cuts in " << inf1 << ": " << tree1->GetEntries(djCut) << endl;;
  cout << " # passed cuts in " << inf2 << ": " << tree2->GetEntries(djCut) << endl;;

  // pdf comparisons
  // check dijet
  TCanvas *ccomp2 = new TCanvas("ccomp2","",500,500);
  compareHist comp2(tree1,tree2,"jdphi","dPhi",djCut.Data(),djCut.Data(),0,3.14,30);
  comp2.Normalize(1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("leading dijet d #phi");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetLegend(0.202,0.828,0.496,0.926);
  comp2.Draw("E");
  ccomp2->Print(Form("plots/%s/%s_dPhi.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccomp3 = new TCanvas("ccomp3","",500,500);
  compareHist comp3(tree1,tree2,"(nljet-aljet)/(nljet+aljet)","Balance",djCut.Data(),djCut.Data(),0,1,30);
  comp3.Normalize(1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("(E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.632,0.828,0.927,0.926);
  comp3.Draw("E");
  ccomp3->Print(Form("plots/%s/%s_Balance.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  // check tracks in jet
  cout << "================= Check tracks in jet ==================" << endl;
  TString NConeNP = "Sum$("+TString(dataAna.TrkCut)+"&&pndr<0.5)";
  TString AConeNP = "Sum$("+TString(dataAna.TrkCut)+"&&padr<0.5)";
  cout << "count: " << NConeNP << endl;
  TCanvas *ccomp4 = new TCanvas("ccomp4","",500,500);
  compareHist comp4(tree1,tree2,NConeNP.Data(),"Cone5NP",djCut.Data(),djCut.Data(),0,15,15);
  comp4.AppendToHist(tree1,tree2,NConeNP.Data(),"Cone5NP",djCut.Data(),djCut.Data());
  comp4.Normalize(1);
  comp4.SetHistName1(title1);
  comp4.SetHistName2(title2);
  comp4.SetXTitle("# tracks in jet^{1,2} cone (dR<0.5)");
  comp4.SetYTitle("Arbitrary normalization");
  comp4.SetLegend(0.632,0.828,0.927,0.926);
  comp4.Draw("E");
  ccomp4->Print(Form("plots/%s/%s_Cone5NP.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  // check track jet correlations
  TCanvas *ccomp5 = new TCanvas("ccomp5","",500,500);
  compareHist comp5(tree1,tree2,"pndphi","TrkDPhi",djTrkCut.Data(),djTrkCut.Data(),0,3.14,30);
  comp5.Normalize(1);
  comp5.SetHistName1(title1);
  comp5.SetHistName2(title2);
  comp5.SetXTitle("d #phi (j1,trk)");
  comp5.SetYTitle("Arbitrary normalization");
  comp5.SetLegend(0.632,0.828,0.927,0.926);
  comp5.Draw("E");
  ccomp5->Print(Form("plots/%s/%s_TrkDPhi.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  // === dijet ana ===
  cout << endl << "==================== DiJet Ana ===================" << endl;
  TString NearTrkCut = djTrkCut + "&&pndr<0.5";
  TString AwayTrkCut = djTrkCut + "&&padr<0.5";
  TString NearTrkBgCut = djTrkCut + "&&pndrbg<0.5";
  TString AwayTrkBgCut = djTrkCut + "&&padrbg<0.5";
  double XiMax = 5;
  double NXiBin = 20;
  TH1D * djXiData = new TH1D("djXiData",";#xi=ln(E_{T}^{Jet}/p_{T}^{trk});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXiBin,0,XiMax);
  TH1D * djXiDataBg = new TH1D("djXiDataBg","djXiDataBg",NXiBin,0,XiMax);
  djXiDataBg->SetLineColor(kBlue);
  djXiDataBg->SetMarkerColor(kBlue);
  TH1D * djXiDataSig = new TH1D("djXiDataSig",";#xi=ln(E_{T}^{Jet}/p_{T}^{trk});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXiBin,0,XiMax);
  TH1D * djXiMC = new TH1D("djXiMC",";#xi=ln(E_{T}^{Jet}/p_{T}^{trk});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXiBin,0,XiMax);
  djXiMC->SetLineColor(kRed);
  djXiMC->SetMarkerColor(kRed);
  TH1D * djXiMCBg = new TH1D("djXiMCBg","djXiMCBg",NXiBin,0,XiMax);
  djXiMCBg->SetLineColor(kBlue);
  djXiMCBg->SetMarkerColor(kBlue);
  TH1D * djXiMCSig = new TH1D("djXiMCSig",";#xi=ln(E_{T}^{Jet}/p_{T}^{trk});#frac{1}{N_{jet}} #frac{dN}{d#xi}",NXiBin,0,XiMax);

  TCanvas *cFF0 = new TCanvas("cFF0","cFF0",500,500);
  double numDJData = tree1->Draw("nljet",djCut);
  cout << "Data: number of dijets for FF: " << numDJData << endl;
  tree1->Draw("log(1/zn)>>djXiData",NearTrkCut);
  tree1->Draw("log(1/za)>>+djXiData",AwayTrkCut);
  tree1->Draw("log(1/zn)>>djXiDataBg",NearTrkBgCut);
  tree1->Draw("log(1/za)>>+djXiDataBg",AwayTrkBgCut);
  djXiData->Scale(1./(2*numDJData),"width");
  djXiDataBg->Scale(1./(2*numDJData),"width");

  double numDJMC = tree2->Draw("nljet",djCut);
  cout << "MC: number of dijets for FF: " << numDJMC << endl;
  tree2->Draw("log(1/zn)>>djXiMC",NearTrkCut);
  tree2->Draw("log(1/za)>>+djXiMC",AwayTrkCut);
  tree2->Draw("log(1/zn)>>djXiMCBg",NearTrkBgCut);
  tree2->Draw("log(1/za)>>+djXiMCBg",AwayTrkBgCut);
  djXiMC->Scale(1./(2*numDJMC),"width");
  djXiMCBg->Scale(1./(2*numDJMC),"width");

  // subtract bg
  djXiDataSig->Add(djXiData,djXiDataBg,1,-1);
  djXiMCSig->Add(djXiMC,djXiMCBg,1,-1);

  //  -- draw --
  double xiYMax = 1.5;
  djXiData->SetMaximum(xiYMax);
  djXiDataBg->SetMaximum(xiYMax);
  djXiDataSig->SetMaximum(xiYMax);
  djXiMC->SetMaximum(xiYMax);
  djXiMCBg->SetMaximum(xiYMax);
  djXiMCSig->SetMaximum(xiYMax);
  djXiData->SetMinimum(0);
  djXiDataBg->SetMinimum(0);
  djXiDataSig->SetMinimum(0);
  djXiMC->SetMinimum(0);
  djXiMCBg->SetMinimum(0);
  djXiMCSig->SetMinimum(0);

  TLegend * leg1 = new TLegend(0.522,0.828,0.927,0.926,NULL,"brNDC");
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetTextSize(0.035);
  leg1->AddEntry("","FF for j1,j2","");
  leg1->AddEntry(djXiData,"Data 900 GeV Raw","pl");
  leg1->AddEntry(djXiDataBg,"Background","pl");
  TLegend * leg2 = new TLegend(0.522,0.828,0.927,0.926,NULL,"brNDC");
  leg2->SetFillStyle(0);
  leg2->SetFillColor(0);
  leg2->SetTextSize(0.035);
  leg2->AddEntry("","FF for j1,j2","");
  leg2->AddEntry(djXiMC,"Pythia 900 GeV Raw","pl");
  leg2->AddEntry(djXiMCBg,"Background","pl");
  TLegend * leg3 = new TLegend(0.522,0.828,0.927,0.926,NULL,"brNDC");
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetTextSize(0.035);
  leg3->AddEntry(djXiData,"Data 900 GeV, Raw-Bg","pl");
  leg3->AddEntry(djXiMC,"Pythia 900 GeV, Raw-Bg","pl");

  TCanvas *cFF1 = new TCanvas("cFF1","cFF1",500,500);
  djXiData->Draw("E");
  djXiDataBg->Draw("Esame");
  leg1->Draw();
  cFF1->Print(Form("plots/%s/%s_XiData.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *cFF2 = new TCanvas("cFF2","cFF2",500,500);
  djXiMC->Draw("hist E");
  djXiMCBg->Draw("hist E same");
  leg2->Draw();
  cFF2->Print(Form("plots/%s/%s_XiMC.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *cFF3 = new TCanvas("cFF3","cFF3",500,500);
  djXiDataSig->Draw("E");
  djXiMCSig->SetMarkerStyle(0);
  djXiMCSig->SetLineColor(2);
  djXiMCSig->Draw("hist E same");
  leg3->Draw();
  cFF3->Print(Form("plots/%s/%s_XiSigRawDataMC.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));
}

