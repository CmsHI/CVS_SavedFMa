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


void extractHists(TString AnaVersion="testV010",
    TString DataSource = "data",
    const char * datafname = "../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root",
    TString MCSource = "pythiaAtlas",
    const char * mcfname = "../pixel_trees/mc/pixelTree_yilmaz-MinBiasATLAS_RECO_0332_v1.root",
    int doSel = 1, int mode=0,
    float EPzMin=0, float EPzMax=200, float EPzBinSize=5,
    const char * databgfname = "../pixel_trees/emptybx/pixelTree_emptyBx_132422-veryloosecuts_v2.root")
{
  // top level info
  TString AnaTag = Form("ana%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),mode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data());
  cout << "====== Ana: " << AnaTag << endl;

  // set anaMode
  if (mode==0) {
    wanted0 ="DF";
    wanted1 ="ND";
  }
  if (mode==1) {
    wanted0 ="SD";
    wanted1 ="NSD";
  }
  if (mode==2) {
    wanted0 = "SD";
    wanted1 = "DD";
    wanted2 = "ND";
  }

  // ==== get trees ====
  cout << "Data: " << datafname << endl;
  cout << "MC:   " << mcfname << endl;
  TFile * dataFile = new TFile(datafname);
  TFile * mcFile = new TFile(mcfname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);
  aliases_tree(treeData);
  aliases_tree(treeMC);
  //TFile * databgFile = new TFile(*databgfname);
  //TTree * treeDataBg; databgFile->GetObject("PixelTree",treeDataBg);

  // ===== trigger =====
  bool isMC=true;
  selectionCut mcSel(isMC,doSel);
  if (DataSource=="data") isMC=false;
  selectionCut dataSel(isMC,doSel);
  dataSel.AnaTag = AnaTag;
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSel.Cut) << endl;
  cout << "MC: " << TString(mcSel.Cut) << endl;

  // configuation
  // sources
  vector<TString> source;
  source.push_back(dataHistLabel);
  source.push_back(mcHistLabel);
  // colors
  vector<Color_t> color;
  color.push_back(kRed);
  color.push_back(kBlue);
  color.push_back(kGreen-1);
  // event types
  vector<TString> etype;
  vector<TCut> etypeCut;
  vector<TCut> etypePhojCut;
  //  -pythia-
  etype.push_back("All"); etypeCut.push_back("1==1");
  etype.push_back("SD"); etypeCut.push_back("evtType==92 || evtType==93");
  etype.push_back("NSD"); etypeCut.push_back("evtType!=92 && evtType!=93");
  etype.push_back("DF"); etypeCut.push_back("evtType==92 || evtType==93 || evtType==94");
  etype.push_back("ND"); etypeCut.push_back("evtType!=92 && evtType!=93 && evtType!=94");
  etype.push_back("DD"); etypeCut.push_back("evtType==94");
  //  -phojet-
  etypePhojCut.push_back("evtType==1 || evtType==5 || evtType==6 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==5 || evtType==6");
  etypePhojCut.push_back("evtType==1 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==5 || evtType==6 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==1");
  etypePhojCut.push_back("evtType==7 || evtType==4");

  //
  // === calc cuts ===
  //
  // for mc
  printf("\n===== MC Input =====\n");
  Double_t mcTruthFrac=-1;
  if (MCSource.Contains("pythia")) {
    mcTruthFrac = calcFrac(1,treeMC,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (MCSource.Contains("phojet")) {
    mcTruthFrac = calcFrac(1,treeMC,mcSel.Cut,etype,etypePhojCut,wanted0);
  }
  // for data or "data"
  Double_t truthFrac=-1;
  if (DataSource.Contains("data")) {
    printf("\n===== Data Input =====\n");
    calcFrac(0,treeData,dataSel.Cut,etype,etypeCut,wanted0);
  }
  if (DataSource.Contains("pythia")) {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(1,treeData,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (DataSource.Contains("phojet")) {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(1,treeData,mcSel.Cut,etype,etypePhojCut,wanted0);
  }

  // Checking cuts only, exit
  if (AnaVersion.Contains("Eff")) return;

  // === Define output ===
  TString outdir=Form("plots/%s/%s/Sel%d",AnaVersion.Data(),MCSource.Data(),doSel);
  gSystem->mkdir(Form("%s",outdir.Data()),kTRUE);
  TFile * fout = new TFile(Form("%s/%s.root",outdir.Data(),AnaTag.Data()),"RECREATE");

  //
  // === Declare histograms ===
  //
  printf("now declare hists\n");
  // container for all declared histograms
  vector<TH1*> vh1;
  Double_t EPzYMax=0.035/(EPzMax/200), Chi2YMax=60;
  if (doSel==4) {
    EPzYMax=0.01/(EPzMax/200);
    Chi2YMax=20;
  }
  if (doSel==10) {
    EPzYMax=0.12/(EPzMax/200);
  }
  const Int_t EPzNBINS=EPzMax/EPzBinSize;
  for (Int_t i=0; i<source.size(); ++i) {
    vh1.push_back(new TH1D(Form("hEvtEta_%s",source[i].Data()),";#eta^{Event}_{HF};",100,-5,5));
    vh1.push_back(new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEp_%s",source[i].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEaddEpPos_%s",source[i].Data()),";#Sigma E+Pz (HF+);",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEpNeg_%s",source[i].Data()),";#Sigma E-Pz (HF-);",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hMinEPz_%s",source[i].Data()),";min(#Sigma E+Pz, #Sigma E-Pz);",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH2D(Form("hEPz_%s",source[i].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
    if (source[i]==mcHistLabel) {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";#eta^{Event}_{HF};",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEaddEpPos_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz (HF+);",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEpNeg_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz (HF-);",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hMinEPz_%s_%s",   source[i].Data(),etype[j].Data()),";min(#Sigma E+Pz, #Sigma E-Pz);",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH2D(Form("hEPz_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
      }
    }
  }
  // set histograms
  for (Int_t ih1=0; ih1<vh1.size(); ++ih1) {
    //cout << "hist: " << vh1[ih1]->GetName() << " title: " << vh1[ih1]->GetXaxis()->GetTitle() << endl;
    vh1[ih1]->Sumw2();
  }

  // Fill histos
  vector<TString> evtEtaHists;
  vector<TString> EaddEpHists;
  vector<TString> EsubEpHists;
  vector<TString> EaddEpPosHists;
  vector<TString> EsubEpNegHists;
  vector<TString> MinEPzHists;
  vector<TString> EPzHists;
  if (MCSource.Contains("pythia")) {
    fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,evtEtaHists);
    fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpHists);
    fillHist("SumEsubEp","hEsubEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EsubEpHists);
    fillHist("SumEaddEpPos","hEaddEpPos",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpPosHists);
    fillHist("SumEsubEpNeg","hEsubEpNeg",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EsubEpNegHists);
    fillHist("MinEPz","hMinEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,MinEPzHists);
    fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EPzHists);
  }
  if (MCSource.Contains("phojet")) {
    fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,evtEtaHists);
    fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EaddEpHists);
    fillHist("SumEsubEp","hEsubEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EsubEpHists);
    fillHist("SumEaddEpPos","hEaddEpPos",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EaddEpPosHists);
    fillHist("SumEsubEpNeg","hEsubEpNeg",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EsubEpNegHists);
    fillHist("MinEPz","hMinEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,MinEPzHists);
    fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EPzHists);
  }
  // save
  fout->Write();
  //fout->Close();
}
