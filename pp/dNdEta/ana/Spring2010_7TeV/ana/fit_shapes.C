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
using namespace std;
#include "helpers.h"

void fit_shapes(TString AnaVersion="testV010",
    TString DataSource = "data",
    const char * datafname = "../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root",
    TString MCSource = "pythia_D6T",
    const char * mcfname = "../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root",
    TString AnaObs = "EaddEpPos", //EvtEta, EsubEp, MinEPz
    int doSel = 1, int anaMode=0, // 0 for D vs ND, 1 for SD vs NSD, 2 for SD, DD, ND
    float EPzMin=0, float EPzMax=200, float EPzBinSize=5,
    const char * databgfname = "../pixel_trees/emptybx/pixelTree_emptyBx_132422-veryloosecuts_v2.root")
{
  // set anaMode
  if (anaMode==0) {
    wanted0 ="DF";
    wanted1 ="ND";
  }
  if (anaMode==1) {
    wanted0 ="SD";
    wanted1 ="NSD";
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
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSel.Cut) << endl;
  cout << "MC: " << TString(mcSel.Cut) << endl;
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

  // calc cuts
  // for mc
  printf("\n===== MC Input =====\n");
  Double_t mcTruthFrac=-1;
  if (MCSource.Contains("pythia")) {
    mcTruthFrac = calcFrac(treeMC,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (MCSource.Contains("phojet")) {
    mcTruthFrac = calcFrac(treeMC,mcSel.Cut,etype,etypePhojCut,wanted0);
  }
  // for data or "data"
  Double_t truthFrac=-1;
  if (DataSource.Contains("data")) {
    printf("\n===== Data Input =====\n");
    printf("%d passed cut\n",treeData->GetEntries(dataSel.Cut));
  }
  if (DataSource.Contains("pythia")) {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (DataSource.Contains("phojet")) {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypePhojCut,wanted0);
  }

  // done with trees
  dataFile->Close();
  mcFile->Close();

  // ================ Hist Shapes Ana ======================
  // === Define Inputs ===
  TString indir=Form("plots/%s/%s/Sel%d",AnaVersion.Data(),MCSource.Data(),doSel);
  TString HistsTag = Form("ana%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),anaMode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data());

  if (DataSource == "data") gDataSource="Run 132440 (7TeV)";
  else gDataSource = DataSource;
  gMCSource=MCSource;
  
  const char * dataHistsName = Form("%s/%s.root",indir.Data(),HistsTag.Data());
  cout << "Data File: " << dataHistsName << endl;
  const char * shapes0fname = Form("%s/%s.root",indir.Data(),HistsTag.Data());
  cout << "Shapes File: " << shapes0fname << endl;
  TFile * dataHistFile = new TFile(dataHistsName);
  TFile * shapes0File = new TFile(shapes0fname);
  /*
  const char * shapes0fname = Form("plots/%s/EvtEta_Sel1/ana%s_EvtEta_Mode0_Min%.0f_Max%.0f_Delta%0.f_Sel1_data_use_pythia.root",
      AnaVersion.Data(),AnaVersion.Data(),
      EPzMin,EPzMax,EPzBinSize);
  cout << "Shapes File: " << shapes0fname << endl;
  const char * shapes1fname = Form("plots/%s/EvtEta_Sel10/ana%s_EvtEta_Mode0_Min%.0f_Max%.0f_Delta%0.f_Sel10_data_use_pythia.root",
      AnaVersion.Data(),AnaVersion.Data(),
      EPzMin,EPzMax,EPzBinSize);
  cout << "Shapes File1: " << shapes1fname << endl;
  const char * shapes2fname = Form("plots/%s/EvtEta_Sel4/ana%s_EvtEta_Mode0_Min%.0f_Max%.0f_Delta%0.f_Sel4_data_use_pythia.root",
      AnaVersion.Data(),AnaVersion.Data(),
      EPzMin,EPzMax,EPzBinSize);
  cout << "Shapes File2: " << shapes2fname << endl;
  TFile * shapes0File = new TFile(shapes0fname);
  TFile * shapes1File = new TFile(shapes1fname);
  TFile * shapes2File = new TFile(shapes2fname);
  */

  // === Get Histograms ===
  vector<TH1D*> inputHists;
  inputHists.push_back( (TH1D*)dataHistFile->FindObjectAny(Form("h%s_%s",AnaObs.Data(),dataHistLabel.Data())) );
  inputHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_%s_%s",AnaObs.Data(),mcHistLabel.Data(),wanted0.Data())) );
  inputHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_%s_%s",AnaObs.Data(),mcHistLabel.Data(),wanted1.Data())) );
  //inputHists.push_back( (TH1D*)shapes1File->FindObjectAny(Form("h%s_data",AnaObs.Data())) );
  //inputHists.push_back( (TH1D*)shapes2File->FindObjectAny(Form("h%s_data",AnaObs.Data())) );

  // === Top level info ===
  TString AnaTag = Form("ana%s_%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),AnaObs.Data(),anaMode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data());
  cout << "====== Ana: " << AnaTag << endl;

  // === Now define output ===
  TString outdir = indir+"/"+AnaObs;
  gSystem->mkdir(Form("%s",outdir.Data()),kTRUE);
  TFile * fout = new TFile(Form("%s/%s_fits.root",outdir.Data(),AnaTag.Data()),"RECREATE");


  Double_t EPzYMax=0.035/(EPzMax/200), Chi2YMax=60;
  if (doSel==4) {
    EPzYMax=0.01/(EPzMax/200);
    Chi2YMax=20;
  }
  if (doSel==10) {
    EPzYMax=0.12/(EPzMax/200);
  }
  if (AnaObs=="EvtEta")
    EPzYMax=1;

  // test
  TCanvas * cEaddPzDefault = new TCanvas("cEaddPzDefault","cEaddPzDefault",600,600);
  histDiffrChi2(
      inputHists,
      anaMode,
      truthFrac,
      -1,
      2,
      EPzYMax);

  // === calc chi2 ===
  printf("\n=========== Chi2 clac ================\n");
  Int_t N=100;
  Double_t maxTestFrac=0.5;
  TCanvas * cChi2 = new TCanvas("cChi2","cChi2",600,600);
  TH1D * hChi2 = new TH1D("hChi2",Form(";%s Fraction;#chi^{2}",wanted0.Data()),N,0,maxTestFrac);
  hChi2->SetMinimum(0);
  //if (Chi2YMax>0) hChi2->SetMaximum(Chi2YMax);
  TH2D * h2Chi2 = new TH2D("h2Chi2",Form(";%s Fraction;%s Fraction",wanted0.Data(),wanted1.Data()),N,0,maxTestFrac,N,0,maxTestFrac);
  h2Chi2->SetMinimum(0);

  Double_t bestX=0, bestY=0, bestZ=0;
  Double_t step = maxTestFrac/(Float_t)N;
  // make chi2
  if (anaMode<2) {
    for (Int_t i=1; i<=N; ++i) {
      Double_t trialFrac = i*step;
      Double_t chi2 = histDiffrChi2(
	  inputHists,
	  anaMode,
	  trialFrac);
      hChi2->SetBinContent(i,chi2);
    }
    hChi2->Draw();

    // === fit chi2 ===
    Double_t chi2Min = hChi2->GetBinCenter(hChi2->GetMinimumBin());
    TF1 *myfun = new TF1("myfun","[1]*(x-[0])*(x-[0])+[2]");
    myfun->SetParameters(chi2Min,100,1);
    hChi2->Fit("myfun","emw M","",chi2Min-0.05,chi2Min+0.05);
    // get error
    Double_t a = myfun->GetParameter(0);
    Double_t b = myfun->GetParameter(1);
    Double_t c = myfun->GetParameter(2);
    Double_t equRoot = sqrt(1./b*(1));
    Double_t chiELow = a-equRoot;
    Double_t chiEHigh = a+equRoot;
    Double_t bestX = myfun->GetParameter(0);
    if (bestX<0||bestX>1) bestX = chi2Min;

    printf("\n\n   Best %s fit fraction: %f\n",wanted0.Data(),bestX);
    printf("       Error: (%f,%f)\n",bestX-chiELow,chiEHigh-bestX);
    printf("       Analysis: %s, %s: %f(%f,%f)\n\n",AnaTag.Data(),wanted0.Data(),bestX,bestX-chiELow,chiEHigh-bestX);
    if (AnaObs=="EvtEta")
      printf("       Twiki - %s, %s: | %s | %s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),MCSource.Data(),bestX*100,(chiEHigh-bestX)*100);
    else
      printf("       Twiki - %s, %s: | %s (%.0fto%.0fGeV) | %s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),EPzMin,EPzMax,MCSource.Data(),bestX*100,(chiEHigh-bestX)*100);
    /*

    // mc truth if using mc as "data"
    if (DataSource.Contains("mc")) {
      TLine * l = new TLine(truthFrac,hChi2->GetMinimum(),truthFrac,hChi2->GetMaximum());
      l->SetLineColor(2);
      l->Draw("same");
      TLegend *leg2 = new TLegend(0.1879,0.839,0.39,0.942,NULL,"brNDC");
      leg2->SetFillColor(0);
      leg2->SetBorderSize(0);
      leg2->AddEntry(l,"MC truth:","l");
      leg2->AddEntry("",Form("%.4f",truthFrac),"");
      leg2->Draw();
    }
    */
    //cChi2->Print(Form("%s/%s_cChi2.gif",outdir.Data(),AnaTag.Data()));

    /*
    // draw distributions
    vector<TH1D*> EaddEpPosHists;
    EaddEpPosHists.push_back( (TH1D*)dataFile->FindObjectAny(Form("hEaddEpPos_%s",DataSource.Data())));
    //EaddEpPosHists.push_back( (TH1D*)shapes0File->FindObjectAny("hEaddEpPos_mc_DF"));
    EaddEpPosHists.push_back( (TH1D*)shapes1File->FindObjectAny("hEaddEpPos_data"));
    //EaddEpPosHists.push_back( (TH1D*)shapes0File->FindObjectAny("hEaddEpPos_mc_ND"));
    EaddEpPosHists.push_back( (TH1D*)shapes2File->FindObjectAny("hEaddEpPos_data"));
    vector<TH1D*> EvtEtaHists;
    EvtEtaHists.push_back( (TH1D*)dataFile->FindObjectAny(Form("hEvtEta_%s",DataSource.Data())));
    //EvtEtaHists.push_back( (TH1D*)shapes0File->FindObjectAny("hEvtEta_mc_DF"));
    EvtEtaHists.push_back( (TH1D*)shapes1File->FindObjectAny("hEvtEta_data"));
    //EvtEtaHists.push_back( (TH1D*)shapes0File->FindObjectAny("hEvtEta_mc_ND"));
    EvtEtaHists.push_back( (TH1D*)shapes2File->FindObjectAny("hEvtEta_data"));
    // -- fitted --
    TCanvas * cEaddPz = new TCanvas("cEaddPz","cEaddPz",600,600);
    histDiffrChi2(
	EaddEpPosHists,
	anaMode,
	bestX,
	-1,
	1,
	EPzYMax);
    //cEaddPz->Print(Form("%s/%s_cEaddPz.gif",outdir.Data(),AnaTag.Data()));
    //cEaddPz->Print(Form("%s/%s_cEaddPz.eps",outdir.Data(),AnaTag.Data()));
    TCanvas * cEvtEta = new TCanvas("cEvtEta","cEvtEta",600,600);
    histDiffrChi2(
	EvtEtaHists,
	anaMode,
	bestX,
	-1,
	1,
	1);
    //cEvtEta->Print(Form("%s/%s_cEvtEta.gif",outdir.Data(),AnaTag.Data()));
    //cEvtEta->Print(Form("%s/%s_cEvtEta.eps",outdir.Data(),AnaTag.Data()));
  */
  }
}
