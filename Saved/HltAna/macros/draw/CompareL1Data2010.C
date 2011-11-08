#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
using namespace std;

void CompareL1Data2010(
                       TString infdataname="../trees/openhlt_data_yenjiereco_r180892.root",
                       TString infmcname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv2/merge/all.root"
                       )
{
   TH1::SetDefaultSumw2();
   
   TFile * infdata = new TFile(infdataname);
   TFile * infmc = new TFile(infmcname);
   
   TTree * tdata = (TTree*)infdata->Get("hltana/HltTree");
   
   TTree * tmc = (TTree*)infmc->Get("hltbitnew/HltTree");
   
   const int netabins = 22;
   float etabins[netabins+1] = 
   {-5,-4.5,-4,-3.5,-3,-2.172,-1.74,-1.392,-1.044,-0.696,-0.348,0,
      0.348,0.696,1.044,1.392,1.74,2.172,3,3.5,4,4.5,5};
   const int nptbins=70;
   float ptbins[nptbins+1];
   for (int i=0; i<=nptbins; ++i) { ptbins[i]=i*4; }
   
   
   TH1D * hL1PtData = new TH1D("hL1PtData","",70,0,280);
   TH1D * hL1PtMC = new TH1D("hL1PtMC","",70,0,280);
   TH1D * hL1EtaData = new TH1D("hL1EtaData","",netabins,etabins);
   TH1D * hL1EtaMC = new TH1D("hL1EtaMC","",netabins,etabins);
   TH1D * hL1PhiData = new TH1D("hL1PhiData","",17,-3.14,3.14);
   TH1D * hL1PhiMC = new TH1D("hL1PhiMC","",17,-3.14,3.14);
   
   TH2D * hL1PtEtaData = new TH2D("hL1PtEtaData","",netabins,etabins,nptbins,ptbins);
   
   TString tag="";
   TString src="Run180892";
   TCut sel = "L1_HcalHfCoincPmORBscMinBiasThresh2_BptxAND";
   TCut selmc = "L1_HcalHfCoincPmORBscMinBiasThresh1_BptxAND";
   float nevtdata = tdata->GetEntries();
   float nevtmc = tmc->GetEntries();
   cout << "data frac L1CenJetEt>80: " << (float)tdata->GetEntries("L1CenJetEt>80"&&sel) << " " << nevtdata << endl;
   cout << "data2010   frac L1CenJetEt>80: " << (float)tmc->GetEntries("L1CenJetEt>80"&&sel) << " " << nevtmc << endl;
   
   tdata->Draw("L1CenJetEt>>hL1PtData",sel,"goff");
   tmc->Draw("L1CenJetEt>>hL1PtMC",selmc,"goff");
   
   tdata->Draw("L1CenJetEt>>hL1EtaData",sel,"goff");
   tmc->Draw("L1CenJetEt>>hL1EtaMC",selmc,"goff");
   
   tdata->Draw("L1CenJetEt>>hL1PhiData",sel,"goff");
   tmc->Draw("L1CenJetEt>>hL1PhiMC",selmc,"goff");
   
   hL1PtData->Scale(1./nevtdata);
   hL1PtMC->Scale(1./nevtmc);
   hL1EtaData->Scale(1./nevtdata);
   hL1EtaMC->Scale(1./nevtmc);
   hL1PhiData->Scale(1./nevtdata);
   hL1PhiMC->Scale(1./nevtmc);
   
   TCanvas * c2 = new TCanvas("c2","c2",500,500);
   c2->SetLogy();
   hL1EtaData->SetTitle(";leading L1 Jet #eta;Event Fraction");
   hL1EtaData->SetAxisRange(1e-5,1e2,"Y");
   hL1EtaMC->SetMarkerStyle(kOpenSquare);
   hL1EtaMC->SetMarkerColor(kBlue);
   hL1EtaMC->SetLineColor(kBlue);
   hL1EtaData->Draw("E");
   hL1EtaMC->Draw("same hist");
   
   TLegend *leg = new TLegend(0.4,0.18,0.87,0.28);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.035);
   leg->AddEntry(hL1EtaData,"HICorePhysics, MB "+tag,"");
   leg->AddEntry(hL1EtaData,"Data 2011 Run 180892","p");
   leg->AddEntry(hL1EtaMC,"HICorePhysics 2010","l");
   leg->Draw();
   
   //c2->Print(Form("out/L1EtaData_%s.gif",tag.Data()));
   c2->Print(Form("out/L1Eta%s_%s.gif",src.Data(),tag.Data()));
   
   TCanvas * c3 = new TCanvas("c3","c3",500,500);
   c3->SetLogy();
   hL1PtData->SetTitle(";leading L1 Jet p_{T} (GeV/c);Event Fraction");
   //hL1PtData->SetAxisRange(0,150,"X");
   hL1PtData->SetAxisRange(1e-5,1e2,"Y");
   hL1PtMC->SetMarkerStyle(kOpenSquare);
   hL1PtMC->SetMarkerColor(kBlue);
   hL1PtMC->SetLineColor(kBlue);
   hL1PtData->Draw("E");
   hL1PtMC->Draw("same hist");
   TLegend * leg3 = (TLegend*)leg->Clone();
   leg3->SetY1NDC(0.8); leg3->SetY2NDC(0.9);
   leg3->Draw();
   
   c3->Print(Form("out/L1Pt%s_%s.gif",src.Data(),tag.Data()));
   
   TCanvas * c4 = new TCanvas("c4","c4",500,500);
   c4->SetLogy();
   hL1PhiData->SetTitle(";leading L1 Jet #phi;Event Fraction");
   hL1PhiData->SetAxisRange(1e-5,1e2,"Y");
   hL1PhiMC->SetMarkerStyle(kOpenSquare);
   hL1PhiMC->SetMarkerColor(kBlue);
   hL1PhiMC->SetLineColor(kBlue);
   hL1PhiData->Draw("E");
   hL1PhiMC->Draw("same hist");
   leg->Draw();
   
   c4->Print(Form("out/L1Phi%s_%s.gif",src.Data(),tag.Data()));
}
