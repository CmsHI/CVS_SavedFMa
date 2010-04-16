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

void fit_shapes(TString AnaVersion="V012",
    TString DataSource = "data",
    const char * datafname = "../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root",
    TString MCSource = "pythiaAtlas",
    const char * mcfname = "../pixel_trees/mc/pixelTree_yilmaz-MinBiasATLAS_RECO_0332_v1.root",
    TString FitVersion = "Fit10",
    TString AnaObs = "EaddEpPos", //EvtEta, EsubEp, MinEPz
    int doSel = 1, int anaMode=0, // 0 for D vs ND, 1 for SD vs NSD, 2 for SD, DD, ND
    float EPzMin=0, float EPzMax=200, float EPzBinSize=5,
    TString MCSource2 = "pythiaAtlas",
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

  // done with trees
  dataFile->Close();
  mcFile->Close();

  //
  // ================ Hist Shapes Ana ======================
  //
  // === Define Inputs ===
  TString fitInDir, fitHistTag;
  // target hist
  if (DataSource.Contains("data")) {
    gDataSource="Run 132440 (7TeV)";
    fitInDir=Form("plots/%s/%s/Sel%d",AnaVersion.Data(),"pythiaAtlas",doSel);
    fitHistTag = Form("ana%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
	AnaVersion.Data(),anaMode,
	EPzMin,EPzMax,EPzBinSize,
	doSel,
	"data","pythiaAtlas");
  }
  else {
    gDataSource = DataSource;
    fitInDir=Form("plots/%s/%s/Sel%d",AnaVersion.Data(),DataSource.Data(),doSel);
    fitHistTag = Form("ana%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
	AnaVersion.Data(),anaMode,
	EPzMin,EPzMax,EPzBinSize,
	doSel,
	"data",DataSource.Data());
  }
  // shape hists
  TString shapeInDir, shapeHistTag, shapeInDir2, shapeHistTag2;
  shapeInDir=Form("plots/%s/%s/Sel%d",AnaVersion.Data(),MCSource.Data(),doSel);
  shapeHistTag = Form("ana%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),anaMode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      "data",MCSource.Data());
  gMCSource=MCSource;
  gMCSource2=MCSource2;
  shapeInDir2=Form("plots/%s/%s/Sel%d",AnaVersion.Data(),MCSource2.Data(),doSel);
  shapeHistTag2 = Form("ana%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),anaMode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      "data",MCSource2.Data());
  
  const char * dataHistsName = Form("%s/%s.root",fitInDir.Data(),fitHistTag.Data());
  cout << "Data File: " << dataHistsName << endl;
  const char * shapes0fname = Form("%s/%s.root",shapeInDir.Data(),shapeHistTag.Data());
  cout << "Shapes File: " << shapes0fname << endl;
  const char * shapes1fname = Form("%s/%s.root",shapeInDir2.Data(),shapeHistTag2.Data());
  cout << "Shapes File: " << shapes1fname << endl;
  TFile * dataHistFile = new TFile(dataHistsName);
  TFile * shapes0File = new TFile(shapes0fname);
  TFile * shapes1File = new TFile(shapes1fname);

  // === Get Histograms ===
  vector<TH1D*> inputHists;
  if (DataSource.Contains("data")) {
    inputHists.push_back( (TH1D*)dataHistFile->FindObjectAny( Form("h%s_%s",AnaObs.Data(),dataHistLabel.Data()) ) );
  } else {
    inputHists.push_back( (TH1D*)dataHistFile->FindObjectAny( Form("h%s_%s",AnaObs.Data(),(mcHistLabel+"_All").Data()) ) );
  }
  inputHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_%s_%s",AnaObs.Data(),mcHistLabel.Data(),wanted0.Data())) );
  inputHists.push_back( (TH1D*)shapes1File->FindObjectAny(Form("h%s_%s_%s",AnaObs.Data(),mcHistLabel.Data(),wanted1.Data())) );

  // === Top level info ===
  TString AnaTag = Form("ana%s_%s_Mode%d_EPzMin%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s_%s",
      AnaVersion.Data(),AnaObs.Data(),anaMode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data(),MCSource2.Data());
  cout << "====== Ana: " << AnaTag << endl;

  // === Now define output ===
  TString outdir = "plots/"+AnaVersion+"/"+FitVersion+"/"+DataSource+"/"+MCSource+"_"+MCSource2+"/"+AnaObs;
  gSystem->mkdir(Form("%s",outdir.Data()),kTRUE);
  TFile * fout = new TFile(Form("%s/%s_fits.root",outdir.Data(),AnaTag.Data()),"RECREATE");


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
      printf("       Twiki - %s, %s: | %s | %s_%s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),MCSource.Data(),MCSource2.Data(),bestX*100,(chiEHigh-bestX)*100);
    else
      printf("       Twiki - %s, %s: | %s (%.0fto%.0fGeV) | %s_%s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),EPzMin,EPzMax,MCSource.Data(),MCSource2.Data(),bestX*100,(chiEHigh-bestX)*100);

    // mc truth if using mc as "data"
    if (DataSource.Contains("pythia")||DataSource.Contains("phojet")) {
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
    cChi2->Print(Form("%s/%s_cChi2.gif",outdir.Data(),AnaTag.Data()));

    //
    // ===================== Draw Observables ===============
    //
    // Set some plotting paremeters
    Double_t EPzYMax=0.01+0.035/(EPzMax/180), Chi2YMax=60;
    if (doSel==4) {
      EPzYMax=0.01/(EPzMax/200);
      Chi2YMax=20;
    }
    if (doSel==10) {
      EPzYMax=0.12/(EPzMax/200);
    }

    // === First Look what Default MC looks like compared to data or "data" ===
    /*
    TCanvas * cEaddPzDefault = new TCanvas("cEaddPzDefault","cEaddPzDefault",600,600);
    vector<TH1D*> EaddEpPosHists;
    if (DataSource.Contains("data")) {
      EaddEpPosHists.push_back( (TH1D*)dataHistFile->FindObjectAny( Form("hEaddEpPos_%s",dataHistLabel.Data()) ) );
    } else {
      EaddEpPosHists.push_back( (TH1D*)dataHistFile->FindObjectAny( Form("hEaddEpPos_%s",(mcHistLabel+"_All").Data()) ) );
    }
    EaddEpPosHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("hEaddEpPos_%s_%s",mcHistLabel.Data(),wanted0.Data())) );
    EaddEpPosHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("hEaddEpPos_%s_%s",mcHistLabel.Data(),wanted1.Data())) );
    histDiffrChi2(
	EaddEpPosHists,
	anaMode,
	mcTruthFrac,
	-1,
	2,
	EPzYMax);
    cEaddPzDefault->Print(Form("%s/%s_EaddEpPosDefault.gif",outdir.Data(),AnaTag.Data()));
    cEaddPzDefault->Print(Form("%s/%s_EaddEpPosDefault.eps",outdir.Data(),AnaTag.Data()));
    */

    // === draw distributions with fitted parameters ===
    vector<TString> observs;
    vector<Double_t> obsymax;
    observs.push_back("EaddEpPos"); obsymax.push_back(EPzYMax);
    observs.push_back("EvtEta"); obsymax.push_back(1);
    observs.push_back("MinEPz"); obsymax.push_back(EPzYMax*1.5);
    observs.push_back("EaddEpPos2Bin"); obsymax.push_back(EPzYMax);

    for (UInt_t i=0; i<observs.size(); ++i) {
      // make hist list
      vector<TH1D*> obsHists;
      if (DataSource.Contains("data")) {
	obsHists.push_back( (TH1D*)dataHistFile->FindObjectAny( Form("h%s_%s",observs[i].Data(),dataHistLabel.Data()) ) );
      } else {
	obsHists.push_back( (TH1D*)dataHistFile->FindObjectAny( Form("h%s_%s",observs[i].Data(),(mcHistLabel+"_All").Data()) ) );
      }
      obsHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_%s_%s",observs[i].Data(),mcHistLabel.Data(),wanted0.Data())) );
      obsHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_%s_%s",observs[i].Data(),mcHistLabel.Data(),wanted1.Data())) );
      // draw
      TCanvas * cObs = new TCanvas("cObs","cObs",600,600);
      histDiffrChi2(
	  obsHists,
	  anaMode,
	  bestX,
	  -1,
	  1,
	  obsymax[i]);
      cObs->Print( Form("%s/%s_%s.gif",outdir.Data(),AnaTag.Data(),observs[i].Data()) );
      cObs->Print( Form("%s/%s_%s.eps",outdir.Data(),AnaTag.Data(),observs[i].Data()) );
      delete cObs;
    }
  } // end of anaMode<2
}
