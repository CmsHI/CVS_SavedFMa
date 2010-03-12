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

  // Make output file
  TFile* outf = new TFile(Form("plots/%s/Sel%d_jEtMax%.1f_jEtaMin%.1f.root",dataAna.AnaTag.Data(),doSel,AnaJetEtMin,AnaJetEtaMax),"recreate");
  TH1::SetDefaultSumw2();

  // ============== pdf comparisons ===============
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
  compareHist comp3(tree1,tree2,"2*(nljet-aljet)/(nljet+aljet)","Balance",djCut.Data(),djCut.Data(),0,1.2,30);
  comp3.Normalize(1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("(E_{T}^{j1}-E_{T}^{j2})/((E_{T}^{j1}+E_{T}^{j2})/2)");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetLegend(0.632,0.828,0.927,0.926);
  comp3.Draw("E");
  ccomp3->Print(Form("plots/%s/%s_Balance.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  // check tracks in jet
  cout << "================= Check tracks in jet ==================" << endl;
  cout << "check: " << dataAna.NConeNP << endl;
  cout << "  and  " << dataAna.AConeNP << endl;
  TCanvas *ccomp4_0 = new TCanvas("ccomp4_0","",500,500);
  compareHist comp4_0(tree1,tree2,dataAna.NConeNP.Data(),"NCone5NP",djCut.Data(),djCut.Data(),0,15,15);
  comp4_0.Normalize(1);
  comp4_0.SetHistName1(title1);
  comp4_0.SetHistName2(title2);
  comp4_0.SetXTitle("# tracks in near jet cone (dR<0.5)");
  comp4_0.SetYTitle("Arbitrary normalization");
  comp4_0.SetLegend(0.632,0.828,0.927,0.926);
  comp4_0.Draw("E");
  ccomp4_0->Print(Form("plots/%s/%s_NCone5NP.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccomp4_1 = new TCanvas("ccomp4_1","",500,500);
  compareHist comp4_1(tree1,tree2,dataAna.AConeNP.Data(),"ACone5NP",djCut.Data(),djCut.Data(),0,15,15);
  comp4_1.Normalize(1);
  comp4_1.SetHistName1(title1);
  comp4_1.SetHistName2(title2);
  comp4_1.SetXTitle("# tracks in away jet cone (dR<0.5)");
  comp4_1.SetYTitle("Arbitrary normalization");
  comp4_1.SetLegend(0.632,0.828,0.927,0.926);
  comp4_1.Draw("E");
  ccomp4_1->Print(Form("plots/%s/%s_ACone5NP.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *ccomp4_2 = new TCanvas("ccomp4_2","",500,500);
  compareHist comp4_2(tree1,tree2,dataAna.NConeNPBg.Data(),"Cone5NPBg",djCut.Data(),djCut.Data(),0,15,15);
  comp4_2.AppendToHist(tree1,tree2,dataAna.AConeNPBg.Data(),"Cone5NPBg",djCut.Data(),djCut.Data());
  comp4_2.Normalize(1);
  comp4_2.SetHistName1(title1);
  comp4_2.SetHistName2(title2);
  comp4_2.SetXTitle("# tracks in jet^{1,2} trans. cone (dR<0.5)");
  comp4_2.SetYTitle("Arbitrary normalization");
  comp4_2.SetLegend(0.632,0.828,0.927,0.926);
  comp4_2.Draw2("E");
  ccomp4_2->Print(Form("plots/%s/%s_Cone5NPBg.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

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
  TCanvas *ctemp = new TCanvas("ctemp","ctemp",500,500);
  // data
  AnaFrag dataNr(0,"Near",tree1,djCut,djTrkCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag dataAw(0,"Away",tree1,djCut,djTrkCut,"log(1/za)","padr<0.5","padrbg<0.5");
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
  AnaFrag mcNr(1,"Near",tree2,djCut,djTrkCut,"log(1/zn)","pndr<0.5","pndrbg<0.5");
  AnaFrag mcAw(1,"Away",tree2,djCut,djTrkCut,"log(1/za)","padr<0.5","padrbg<0.5");
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

  TLegend * leg2 = new TLegend(0.468,0.741,0.873,0.928,NULL,"brNDC");
  leg2->SetFillStyle(0);
  leg2->SetFillColor(0);
  leg2->SetTextSize(0.035);
  leg2->AddEntry("","j1,j2","");
  leg2->AddEntry(dataNr.hXiRaw,"Data 900 GeV Raw","pl");
  leg2->AddEntry(dataNr.hXiBkg,"Data Background","pl");
  leg2->AddEntry(mcNr.hXiRaw,"Pythia 900 GeV Raw","pl");
  leg2->AddEntry(mcNr.hXiBkg,"Pythia Background","pl");

  TLegend * leg3 = new TLegend(0.468,0.8,0.873,0.926,NULL,"brNDC");
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetTextSize(0.035);
  leg3->AddEntry("","j1,j2","");
  leg3->AddEntry(dataNr.hXiSig,"Data 900 GeV, Raw-Bg","pl");
  leg3->AddEntry(mcNr.hXiSig,"Pythia 900 GeV, Raw-Bg","pl");

  TCanvas *cFF1 = new TCanvas("cFF1","cFF1",500,500);
  hDataCombXiRaw->Draw("E");
  hDataCombXiBkg->Draw("Esame");
  leg1->Draw();
  cFF1->Print(Form("plots/%s/%s_XiData.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *cFF2 = new TCanvas("cFF2","cFF2",500,500);
  hMCCombXiRaw->Draw("hist E");
  hMCCombXiBkg->Draw("hist Esame");
  hDataCombXiRaw->Draw("Esame");
  hDataCombXiBkg->Draw("Esame");
  leg2->Draw();
  cFF2->Print(Form("plots/%s/%s_XiMC.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  TCanvas *cFF3 = new TCanvas("cFF3","cFF3",500,500);
  hMCCombXiSig->Draw("hist E");
  hDataCombXiSig->Draw("Esame");
  leg3->Draw();
  cFF3->Print(Form("plots/%s/%s_XiSigRawDataMC.gif",dataAna.AnaTag.Data(),dataAna.SelTag.Data()));

  outf->Write();
}

