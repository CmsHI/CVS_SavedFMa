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

void fit_shapes(TString AnaVersion="V1_3")
{
  Int_t doSel = 1;
  Double_t EPzMin = 0;
  Double_t EPzMax = 200;
  Double_t EPzBinSize=5;
  TString AnaObs("EaddEpPos");
  //TString AnaObs("EvtEta");
  Int_t anaMode = 0; // 0 for D vs ND, 1 for SD vs NSD, 2 for SD, DD, ND

  // set anaMode
  if (anaMode==0) {
    wanted0 ="DF";
    wanted1 ="ND";
  }
  if (anaMode==1) {
    wanted0 ="SD";
    wanted1 ="NSD";
  }

  // === Define Inputs ===
  TString DataSource = "data"; gDataSource="Run 132440 (7TeV)";
  //TString DataSource = "pythia"; gDataSource="PythiaD6T";
  Double_t truthFrac = 0.220092;
  TString MCSource = "pythia";
  
  TString AnaTag = Form("ana%s_%s_Mode%d_Min%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),AnaObs.Data(),anaMode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data());
  cout << "====== Ana: " << AnaTag << endl;
  TString outdir=Form("plots/%s/%s_Sel%d",AnaVersion.Data(),AnaObs.Data(),doSel);

  const char * datafname = Form("%s/%s.root",outdir.Data(),AnaTag.Data());
  cout << "Data File: " << datafname << endl;
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
  TFile * dataFile = new TFile(datafname);
  TFile * shapes0File = new TFile(shapes0fname);
  TFile * shapes1File = new TFile(shapes1fname);
  TFile * shapes2File = new TFile(shapes2fname);


  // === Get Histograms ===
  if (DataSource=="pythia") {
    DataSource = "mc_All";
  }
  vector<TH1D*> inputHists;
  inputHists.push_back( (TH1D*)dataFile->FindObjectAny(Form("h%s_%s",AnaObs.Data(),DataSource.Data())) );
  //inputHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_mc_DF",AnaObs.Data())) );
  inputHists.push_back( (TH1D*)shapes1File->FindObjectAny(Form("h%s_data",AnaObs.Data())) );
  //inputHists.push_back( (TH1D*)shapes0File->FindObjectAny(Form("h%s_mc_ND",AnaObs.Data())) );
  inputHists.push_back( (TH1D*)shapes2File->FindObjectAny(Form("h%s_data",AnaObs.Data())) );

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
  if (Chi2YMax>0) hChi2->SetMaximum(Chi2YMax);
  TH2D * h2Chi2 = new TH2D("h2Chi2",Form(";%s Fraction;%s Fraction",wanted0.Data(),wanted1.Data()),N,0,maxTestFrac,N,0,maxTestFrac);
  h2Chi2->SetMinimum(0);

  Double_t bestX=0, bestY=0, bestZ=0;
  Double_t step = maxTestFrac/(Float_t)N;
  // make chi2
  if (anaMode==0 || anaMode==1) {
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
    if (bestX<0) bestX = chi2Min;
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
  }
}
