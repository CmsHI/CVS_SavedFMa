#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TLegend.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/tgraphTools.C"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"
using namespace std;

void calcEffFake(TTree * ntlead, vector<Int_t> vthresh, TH1D* hnum, TH1D* hden, TH1D* heff, TH1D* hfake,Color_t * vc, TString effvar="genpt")
{
   // Ana Parameters
   Double_t etaAcc = 3;
   Double_t dRMax = 0.5;
   TString genjetcut(Form("%s>0",effvar.Data()));
   TString caljetcut(Form("recpt>0 && abs(receta)<%f",etaAcc));
   cout << "gen jets den: " << genjetcut << endl;
   cout << "cal jets den: " << caljetcut << endl;

   // Plot parameters
   Style_t effMarkerStyle;
   if (effvar=="genpt") effMarkerStyle=kFullCircle;
   else if (effvar=="recpt") effMarkerStyle=kFullSquare;

   // Declare needed variables
   TString effcut, fakecut;
   //
   TCanvas * cpt = new TCanvas("c"+effvar,"c"+effvar);
   cpt->SetLogy();
   TCanvas * ceff = new TCanvas("ceff_"+effvar,"ceff_"+effvar,500,500);
   TLegend *legeff = new TLegend(0.55,0.24,0.87,0.37);
   legeff->SetFillColor(0);
   legeff->SetTextSize(0.035);
   //
   TGraphAsymmErrors *gFake = new TGraphAsymmErrors();
   gFake->SetMarkerColor(kRed);
   gFake->SetMarkerStyle(kOpenCircle);
   gFake->SetMarkerSize(1.2);

   // Do calculations for each threshold
   for (UInt_t i=0; i<vthresh.size(); ++i) {
      // Efficiency
      cpt->cd();
      effcut=genjetcut + Form(" && l1j%d==1",vthresh[i]);
      cout << "effcut: " << effcut << endl;
      ntlead->Draw(Form("%s>>hden",effvar.Data()),genjetcut,"hist");
      ntlead->Draw(Form("%s>>hnum",effvar.Data()),effcut,"same hist");
      //
      TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
      gEfficiency->SetMarkerStyle(effMarkerStyle);
      gEfficiency->SetMarkerSize(1.2);
      gEfficiency->SetMarkerColor(vc[i]);
      //
      gEfficiency->BayesDivide(hnum,hden);
      clearXErrorBar(gEfficiency);

      ceff->cd();
      if (i==0)
	 heff->Draw();
      gEfficiency->Draw("pz");

      legeff->AddEntry(gEfficiency,Form("L1_SingleJet%d",(Int_t)vthresh[i]),"p");
   }

   ceff->cd();
   legeff->Draw();
}

void plotL1Eff(char * infile = "MyEffHist_0.root")
{
   // Top variables
   TString EffVar("genpt");
   TString EffObjTitle("gen jet");
   //TString EffVar("recpt");
   //TString EffObjTitle("calo jet uncorr");
   Int_t NBIN=15;
   Int_t MAXPT=60;
   vector<Int_t> vthresh;
   vthresh.push_back(15);
   vthresh.push_back(30);
   vthresh.push_back(50);
   Color_t colors[4] = {kBlack, kAzure+7, kGreen-2, kRed+2};

   // === Get Ntuple ===
   TFile * intf = new TFile(infile);
   TNtuple * ntlead;
   intf->GetObject("ntlead",ntlead);

   // === Book Histograms ===
   TH1D * hnum = new TH1D("hnum","hnum",NBIN,0,MAXPT);
   hnum->Sumw2();
   hnum->SetLineColor(kRed);
   TH1D * hden = new TH1D("hden","hden",NBIN,0,MAXPT);
   hden->Sumw2();
   TH1D * heff = new TH1D("heff",Form(";E_{T}^{%s} [GeV]; Trigger Efficiency",EffObjTitle.Data()),NBIN,0,MAXPT);
   heff->Sumw2();
   heff->SetMarkerStyle(kFullCircle);
   heff->SetLineColor(kGreen);
   TH1D * hnumFake = new TH1D("hnumFake","hnumFake",NBIN,0,MAXPT);
   hnumFake->Sumw2();
   hnumFake->SetLineColor(kRed);
   TH1D * hdenFake = new TH1D("hdenFake","hdenFake",NBIN,0,MAXPT);
   hdenFake->Sumw2();
   TH1D * hfake = new TH1D("hfake",";E_{T}^{calo jet, uncorr} [GeV]; Fake Trigger Rate",NBIN,0,MAXPT);
   hfake->Sumw2();
   hfake->SetMarkerStyle(kOpenCircle);
   hfake->SetLineColor(kBlue);

   // === Do checks ===
   TCanvas * c2 = new TCanvas("cptl1cut_"+EffVar,"cptl1cut_"+EffVar);
   ntlead->Draw(EffVar,Form("%s>0",EffVar.Data()));
   ntlead->Draw(EffVar,Form("%s>0 && l1j15==1",EffVar.Data()),"same");
   ntlead->Draw(EffVar,Form("%s>0 && l1j30==1",EffVar.Data()),"same");
   ntlead->Draw(EffVar,Form("%s>0 && l1j50==1",EffVar.Data()),"same");

   // === Do Calculations ===
   calcEffFake(ntlead,vthresh,hnum,hden,heff,hfake, colors,EffVar);

   // Print Canvases
   printAllCanvases("./results/l1zoomed");
}
