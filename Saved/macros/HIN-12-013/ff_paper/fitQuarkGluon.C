#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>
#include "HIN-12-013.h"
using namespace std;

float CalcChi2 (TH1D * h1, TH1D * h2, int begBin=1, int endBin=-1) {
  if (begBin>endBin) { begBin=1; endBin=h1->GetNbinsX(); }
  float total=0;
  for (int i=begBin; i<=endBin; ++i) {
    float diff2 = pow(h1->GetBinContent(i) - h2->GetBinContent(i), 2);
    float err2 = pow(h1->GetBinError(i),2) + pow(h2->GetBinError(i),2);
    total += diff2/err2;
  }
  total /= (endBin-begBin+1);
  return total;
}

class TemplateFit {
public:
  // setup
  float stepSize;
  int nSteps;

  // data members
  TH1D * hTemplate[3];
  TH1D * hData;
  TH1D * hFit[3];
  TH1D * hChi2;
  float minChi2, frac1;
  TF1 * fQuad;

  // Constructor
  TemplateFit(float dx=0.1)
  : stepSize(dx)
  , minChi2(999999999) {
    nSteps = 1./stepSize;
    fQuad = new TF1("fQuad","[0]+[1]*(x-[2])^2");
  }

  // Member Methods
  void Init(TH1D * myData, TH1D * h1, TH1D * h2) {
    hData = myData;
    hTemplate[1] = h1;
    hTemplate[2] = h2;
    hFit[0] = (TH1D*)hData->Clone("hFit");
    hFit[0]->Reset();
    hChi2 = new TH1D("hChi2",";x;#chi2/dof;",nSteps,0,1);
  }

  void Fit(float xmin=0, float xmax=-1) {
    // Get xbin range to fit
    int begBin, endBin;
    if (xmin > xmax) {
      begBin = 1;
      endBin = hData->GetNbinsX();
    } else {
      begBin = hData->FindBin(xmin);
      endBin = hData->FindBin(xmax);
    }
    cout << "begBin: " << begBin << " endBin: " << endBin << endl;

    // Now fill chi2
    for (int i=0; i<nSteps; ++i) {
      float a = i*stepSize;
      hFit[0]->Add(hTemplate[1],hTemplate[2],a,1-a);
      float ichi2 = CalcChi2(hData,hFit[0],begBin,endBin);
      hChi2->SetBinContent(i,ichi2);
      hChi2->SetBinError(i,0.1);
      if (ichi2<minChi2) {
        minChi2 = ichi2;
        frac1 = a;
      }
    }
  }

  void GetMinChi2(float forceFrac=-1) {
    cout << "hChi2 min bin: " << frac1 << endl;
    fQuad->SetParameters(0.5, 10, frac1);
    float xmin = frac1-0.2;
    float xmax = frac1+0.2;
    if (xmin<0) xmin=0;
    if (xmax>1) xmax=1;
    hChi2->Fit("fQuad","0","",xmin,xmax);

    float fitmin = fQuad->GetParameter(2);
    if (fQuad-> GetChisquare()<10) frac1=fitmin;
    if (forceFrac>0) frac1 = forceFrac;
    hFit[1] = (TH1D*)hTemplate[1]->Clone(Form("%s_fit",hTemplate[1]->GetName()));
    hFit[2] = (TH1D*)hTemplate[2]->Clone(Form("%s_fit",hTemplate[2]->GetName()));
    hFit[1]->Scale(frac1);
    hFit[2]->Scale(1-frac1);
    hFit[0]->Add(hFit[1],hFit[2]);
  }

  void Inspect() {
    TCanvas * cInsp1 = new TCanvas("cInsp1","cInsp1",500,500);
    hChi2->Draw();
    fQuad->Draw("same");
    TLine * l0 = new TLine(frac1,0,frac1,hChi2->GetMaximum());
    l0->SetLineColor(kRed);
    l0->Draw();

    TCanvas * cInsp0 = new TCanvas("cInsp0","cInsp0",500,500);
    TH1D * hFrame = (TH1D*)hData->Clone("hFrame");
    hFrame->SetYTitle("PbPb/pp");
    hFrame->SetAxisRange(0,5.25,"X");
    hFrame->SetAxisRange(0,2,"Y");
    hFit[0]->SetLineColor(kRed);
    hFit[1]->SetLineColor(kGreen+2);
    hFit[2]->SetLineColor(kBlue);
    hFrame->Draw();
    hData->Draw("E same");
    hFit[1]->Draw("hist same");
    hFit[2]->Draw("hist same");
    hFit[0]->Draw("hist same");

    TLegend * leg0 = new TLegend(0.34,0.69,0.76,0.92,NULL,"brNDC");
    easyLeg(leg0,"",23);
    leg0->AddEntry(hData,"Data","p");
    leg0->AddEntry(hFit[0],"Fit","l");
    leg0->AddEntry(hFit[1],"Gluon","l");
    leg0->AddEntry(hFit[2],"Quark","l");
    leg0->Draw();
  }
};

void fitQuarkGluon(int fragMode=2)
{ 
  TH1::SetDefaultSumw2();

  //////////////////////////////////////////////////////////////////////
  // Setup
  //////////////////////////////////////////////////////////////////////
  TString outdir="plotsFinalFitQuarkGluon";

  //////////////////////////////////////////////////////////////////////
  // Specify Inputs
  //////////////////////////////////////////////////////////////////////
  TString infdata=Form("plotsFinalFF/FFana2_trkPtProj_binMode2_hi_rewt0_pp_sm2rewt0_bc11_mc1_rat_EtaRef_mc80to170_clos100.root",fragMode);
  // TString infdata=Form("plotsFinalFF/FFana2_trkPtProj_binMode2_hi_rewt0_pp_sm2rewt0_bc11_mc2_rat_EtaRef_mc80to170_clos101.root",fragMode);
  // TString infdata=Form("plotsFinalFF/FFana2_trkPtProj_binMode2_hi_rewt1_pp_sm2rewt1_bc11_mc0_rat_EtaRef.root",fragMode);

  TString inftempl1=Form("plotsFinalFFXChk/FFana2_trkPtProj_binMode2_gluon_hi_rewt0_pp_sm2rewt0_bc11_mc0_rat_EtaRef.root",fragMode);
  TString inftempl2=Form("plotsFinalFFXChk/FFana2_trkPtProj_binMode2_quark_hi_rewt0_pp_sm2rewt0_bc11_mc0_rat_EtaRef.root",fragMode);
  // TString inftempl1=Form("plotsFinalFFXChk/FFana2_trkPtProj_binMode2_gluon_hi_rewt0_pp_sm2rewt0_bc0_mc1_rat_EtaRef.root",fragMode);
  // TString inftempl2=Form("plotsFinalFFXChk/FFana2_trkPtProj_binMode2_quark_hi_rewt0_pp_sm2rewt0_bc0_mc1_rat_EtaRef.root",fragMode);
  // TString tag = TString(infdata.c_str()).ReplaceAll(".root","_Fit");

  /////////////////////////////////////////////////
  // Get Input Histograms
  /////////////////////////////////////////////////
  TH1D* ffratio[3][5];
  TString dataset="mc";
  int anacent=1;
  ffratio[0][1]  = (TH1D*)load(infdata.Data(),Form("ffRattio_ijet1_iaj%d",anacent));
  ffratio[1][1]  = (TH1D*)load(inftempl1.Data(),Form("ffRattio_ijet1_iaj%d",anacent));
  ffratio[2][1]  = (TH1D*)load(inftempl2.Data(),Form("ffRattio_ijet1_iaj%d",anacent));

  /////////////////////////////////////////////////
  // Fit Analysis
  /////////////////////////////////////////////////
  TemplateFit fitrat(0.05);
  fitrat.Init(ffratio[0][1], ffratio[1][1], ffratio[2][1]);

  // fitrat.hData->Draw();
  // fitrat.hTemplate[1]->Draw("same hist");
  // fitrat.hTemplate[2]->Draw("same hist");

  // fitrat.Fit(0,5.5);
  fitrat.Fit(0,3.2);
  fitrat.GetMinChi2();
  fitrat.Inspect();
}

