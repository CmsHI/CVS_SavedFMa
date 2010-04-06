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


// === Main function ===
void extractHists(TString AnaVersion="V0",
    TString DataSource = "data", TString MCSource = "pythia",
    TString AnaObs = "EaddEp", //EvtEta, EsubEp
    int doSel = 1, int mode=0,
    float EPzMin=0, float EPzMax=200, float EPzBinSize=5)
{
  // top level info
  gDataSource="Run 132440 (7TeV)";
  gMCSource=MCSource;
  TString AnaTag = Form("ana%s_%s_Mode%d_Min%.0f_Max%.0f_Delta%.0f_Sel%d_%s_use_%s",
      AnaVersion.Data(),AnaObs.Data(),mode,
      EPzMin,EPzMax,EPzBinSize,
      doSel,
      DataSource.Data(),MCSource.Data());
  cout << "====== Ana: " << AnaTag << endl;
  // mkdir dir for output
  TString outdir=Form("plots/%s/%s_Sel%d",AnaVersion.Data(),AnaObs.Data(),doSel);
  gSystem->mkdir(Form("%s",outdir.Data()),kTRUE);

  // set anaMode
  anaMode=mode;
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
  TString * datafname;
  TString * mcfname;
  TString * databgfname;
  // data
  if (DataSource=="data")
    datafname = new TString("../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root");
  if (DataSource=="pythia")
    datafname = new TString("../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root");
  if (DataSource=="phojet") {
    datafname= new TString("pixelTree_Phojet_MinBias_7TeV.root");
  }
  // mc
  if (MCSource=="pythia")
    mcfname= new TString("../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root");
  if (MCSource=="phojet") {
    mcfname= new TString("pixelTree_Phojet_MinBias_7TeV.root");
  }
  databgfname= new TString("../pixel_trees/emptybx/pixelTree_emptyBx_132422-veryloosecuts_v2.root");
  cout << "Data: " << datafname->Data() << endl;
  cout << "MC:   " << mcfname->Data() << endl;

  TFile * dataFile = new TFile(*datafname);
  TFile * mcFile = new TFile(*mcfname);
  //TFile * databgFile = new TFile(*databgfname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);
  //TTree * treeDataBg; databgFile->GetObject("PixelTree",treeDataBg);
  aliases_tree(treeData);
  aliases_tree(treeMC);

  // Now define output
  TFile * fout = new TFile(Form("%s/%s.root",outdir.Data(),AnaTag.Data()),"RECREATE");

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
  // container for all declared histograms
  vector<TH1*> vh1;

  //
  // declare histograms
  printf("now declare hists\n");
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
    vh1.push_back(new TH2D(Form("hEPz_%s",source[i].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
    if (source[i]==mcHistLabel) {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";#eta^{Event}_{HF};",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEaddEpPos_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz (HF+);",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEpNeg_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz (HF-);",EPzNBINS,EPzMin,EPzMax));
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
  if (MCSource=="pythia") {
    fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,evtEtaHists);
    fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpHists);
    fillHist("SumEsubEp","hEsubEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EsubEpHists);
    fillHist("SumEaddEpPos","hEaddEpPos",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpPosHists);
    fillHist("SumEsubEpNeg","hEsubEpNeg",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EsubEpNegHists);
    fillHist("MinEPz","hMinEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,MinEPzHists);
    fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EPzHists);
  }
  if (MCSource=="phojet") {
    fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,evtEtaHists);
    fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EaddEpHists);
    fillHist("SumEsubEp","hEsubEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EsubEpHists);
    fillHist("SumEaddEpPos","hEaddEpPos",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EaddEpPosHists);
    fillHist("SumEsubEpNeg","hEsubEpNeg",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EsubEpNegHists);
    fillHist("SumEsubEp:SumEaddEp","hEPz",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypePhojCut,EPzHists);
  }
  // calc cuts
  // for mc
  printf("\n===== MC Input =====\n");
  Double_t mcTruthFrac=-1;
  if (MCSource=="pythia") {
    mcTruthFrac = calcFrac(treeMC,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (MCSource=="phojet") {
    mcTruthFrac = calcFrac(treeMC,mcSel.Cut,etype,etypePhojCut,wanted0);
  }
  // for data or "data"
  Double_t truthFrac=-1;
  if (DataSource=="data") {
    printf("\n===== Data Input =====\n");
    printf("%d passed cut\n",treeData->GetEntries(dataSel.Cut));
  }
  if (DataSource=="pythia") {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypeCut,wanted0);
  }
  if (DataSource=="phojet") {
    printf("\n===== \"Data\" Input =====\n");
    truthFrac = calcFrac(treeData,mcSel.Cut,etype,etypePhojCut,wanted0);
  }


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
  vector<TString> fitObsHists;
  // determine what var to fit
  if (AnaObs=="EvtEta") fitObsHists = evtEtaHists;
  if (AnaObs=="EaddEp") fitObsHists = EaddEpHists;
  if (AnaObs=="EsubEp") fitObsHists = EsubEpHists;
  if (AnaObs=="EaddEpPos") fitObsHists = EaddEpPosHists;
  if (AnaObs=="EsubEpNeg") fitObsHists = EsubEpNegHists;
  if (AnaObs=="MinEPz") fitObsHists = EsubEpNegHists;
  // make chi2
  if (anaMode==0 || anaMode==1) {
    for (Int_t i=1; i<=N; ++i) {
      Double_t trialFrac = i*step;
      Double_t chi2 = histDiffrChi2(
	  fitObsHists,
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
    printf("\n\n   Best %s fit fraction: %f\n",wanted0.Data(),bestX);
    printf("       Error: (%f,%f)\n",bestX-chiELow,chiEHigh-bestX);
    printf("       Analysis: %s, %s: %f(%f,%f)\n\n",AnaTag.Data(),wanted0.Data(),bestX,bestX-chiELow,chiEHigh-bestX);
    if (AnaObs=="EvtEta")
      printf("       Twiki - %s, %s: | %s | %s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),MCSource.Data(),bestX*100,(chiEHigh-bestX)*100);
    else
      printf("       Twiki - %s, %s: | %s (%.0fto%.0fGeV) | %s | %.1f+-%.1f\% |\n\n",AnaTag.Data(),wanted0.Data(),AnaObs.Data(),EPzMin,EPzMax,MCSource.Data(),bestX*100,(chiEHigh-bestX)*100);
    TLine * lELow = new TLine(chiELow,hChi2->GetMinimum(),chiELow,hChi2->GetMaximum());
    //lELow->Draw("same");
    TLine * lEHigh = new TLine(chiEHigh,hChi2->GetMinimum(),chiEHigh,hChi2->GetMaximum());
    //lEHigh->Draw("same");
    // mc truth if using mc as "data"
    if (DataSource=="pythia"||DataSource=="phojet") {
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

    // draw distributions
    // -- default --
    TCanvas * cEaddPzDefault = new TCanvas("cEaddPzDefault","cEaddPzDefault",600,600);
    histDiffrChi2(
	EaddEpPosHists,
	anaMode,
	//0,
	mcTruthFrac,
	-1,
	2,
	EPzYMax);
    cEaddPzDefault->Print(Form("%s/%s_cEaddPzDefault.gif",outdir.Data(),AnaTag.Data()));
    cEaddPzDefault->Print(Form("%s/%s_cEaddPzDefault.eps",outdir.Data(),AnaTag.Data()));

    // -- fitted --
    TCanvas * cEaddPz = new TCanvas("cEaddPz","cEaddPz",600,600);
    histDiffrChi2(
	EaddEpPosHists,
	anaMode,
	bestX,
	-1,
	1,
	EPzYMax);
    cEaddPz->Print(Form("%s/%s_cEaddPz.gif",outdir.Data(),AnaTag.Data()));
    cEaddPz->Print(Form("%s/%s_cEaddPz.eps",outdir.Data(),AnaTag.Data()));
    TCanvas * cEvtEta = new TCanvas("cEvtEta","cEvtEta",600,600);
    histDiffrChi2(
	evtEtaHists,
	anaMode,
	bestX,
	-1,
	1,
	1);
    cEvtEta->Print(Form("%s/%s_cEvtEta.gif",outdir.Data(),AnaTag.Data()));
    cEvtEta->Print(Form("%s/%s_cEvtEta.eps",outdir.Data(),AnaTag.Data()));
  } // end of anaMode<2


  if (anaMode==2) {
    for (Int_t i=1; i<=N; ++i) {
      Double_t trialFrac = i*step;
      for (Int_t j=1; j<=N;++j) {
	Double_t trialFrac2 = j*step;
	Double_t chi2 = histDiffrChi2(
	    evtEtaHists,
	    anaMode,
	    trialFrac,
	    trialFrac2);
	h2Chi2->SetBinContent(i,j,chi2);
      }
    }
    h2Chi2->Draw("Cont1");
    Int_t bestXBin,bestYBin,bestZBin;
    h2Chi2->GetMinimumBin(bestXBin,bestYBin,bestZBin);
    bestX=h2Chi2->GetXaxis()->GetBinCenter(bestXBin);
    bestY=h2Chi2->GetYaxis()->GetBinCenter(bestYBin);
    printf("\n\n   Best %s,%s fit fraction: %f,%f\n",wanted0.Data(),wanted1.Data(),bestX,bestY);
    cChi2->Print(Form("%s/%s_c2DChi2.gif",outdir.Data(),AnaTag.Data()));
    TCanvas * cEvtEta = new TCanvas("c2DEvtEta","c2DEvtEta",600,600);
    histDiffrChi2(
	evtEtaHists,
	anaMode,
	0.137,
	0.087,
	1,
	1);
  }


  // save
  fout->Write();
  //fout->Close();
}
