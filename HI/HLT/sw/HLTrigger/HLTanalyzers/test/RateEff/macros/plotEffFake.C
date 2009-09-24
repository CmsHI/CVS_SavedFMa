#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TLegend.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/tgraphTools.C"
using namespace std;

void calcEffFake(TTree * ntlead, vector<Double_t> vthresh, TH1D* hnum, TH1D* hden, TH1D* heff, TH1D* hfake,Color_t * vc)
{
   // Ana Parameters
   Double_t etaAcc = 3;
   Double_t dRMax = 0.5;
   TString genjetcut("genpt>0");
   TString caljetcut(Form("recpt>0 && abs(receta)<%f",etaAcc));
   cout << "gen jets den: " << genjetcut << endl;
   cout << "cal jets den: " << caljetcut << endl;

   // Declare needed variables
   TString effcut, fakecut;
   //
   TCanvas * cpt = new TCanvas("cpt","cpt");
   cpt->SetLogy();
   TCanvas * ceff = new TCanvas("ceff","ceff",500,500);
   TCanvas * cfake = new TCanvas("cfake","cfake",500,500);
   TLegend *legeff = new TLegend(0.19,0.79,0.5,0.92);
   legeff->SetFillColor(0);
   //
   TGraphAsymmErrors *gFake = new TGraphAsymmErrors();
   gFake->SetMarkerColor(kRed);
   gFake->SetMarkerStyle(kOpenCircle);
   gFake->SetMarkerSize(1.2);

   // Do calculations for each threshold
   for (UInt_t i=0; i<vthresh.size(); ++i) {
      // Efficiency
      cpt->cd();
      effcut=genjetcut + Form(" && abs(receta)<%f && recpt>%f",etaAcc,vthresh[i]);
      cout << "effcut: " << effcut << endl;
      ntlead->Draw("genpt>>hden",genjetcut,"hist");
      ntlead->Draw("genpt>>hnum",effcut,"same hist");
      //
      TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
      gEfficiency->SetMarkerStyle(kFullCircle);
      gEfficiency->SetMarkerSize(1.2);
      gEfficiency->SetMarkerColor(vc[i]);
      //
      gEfficiency->BayesDivide(hnum,hden);
      clearXErrorBar(gEfficiency);

      ceff->cd();
      if (i==0)
	 heff->Draw();
      gEfficiency->Draw("pz");

      legeff->AddEntry(gEfficiency,Form("HLT_HIJet%dU",(Int_t)vthresh[i]),"p");

      // Fakes
      cpt->cd();
      ntlead->Draw("recpt>>hden",caljetcut,"hist");
      fakecut=caljetcut + Form(" && abs(recdr)>%f && abs(recdrgen2)>%f",dRMax,dRMax);
      cout << "fakecut: " << fakecut << endl;
      ntlead->Draw("recpt>>hnum",fakecut,"same hist");
      gFake->BayesDivide(hnum,hden);
      clearXErrorBar(gFake);

      cfake->cd();
      if (i==0)
	 hfake->Draw();
      gFake->Draw("pz");
   }

   ceff->cd();
   legeff->Draw();
}

void plotEffFake(char * infile = "MyEffHist_0.root")
{
   // Top variables
   Int_t NBIN=30;
   Int_t MAXPT=150;
   vector<Double_t> vthresh;
   vthresh.push_back(30);
   vthresh.push_back(35);
   vthresh.push_back(50);
   Color_t colors[4] = {kGreen-2, kBlue, kMagenta-4, kRed+2};

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
   TH1D * heff = new TH1D("heff",";E_{T}^{gen jet} [GeV]; Trigger Efficiency",NBIN,0,MAXPT);
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
   TCanvas * c0 = new TCanvas("c0","c0");
   // here we see that quite often the leading cal jet corresponds to the
   // away side of the gen jet
   // However it does not add up to the unmatched distribution, so we are
   // still missing some.
   ntlead->Draw("abs(recdphi)","recpt>0");
   ntlead->Draw("abs(recdphigen2)","recpt>0","same");
   TCanvas * c1 = new TCanvas("c1","c1");
   ntlead->Draw("abs(recdr)","recpt>0");
   ntlead->Draw("abs(recdrgen2)","recpt>0","same");
   TCanvas * c2 = new TCanvas("c2","c2");
   // Though at high pt most leading cal jets match
   ntlead->Draw("genpt","recpt>0 && genpt>40");
   ntlead->Draw("genpt","recpt>0 && abs(recdr<0.5) && genpt>40","same");
   ntlead->Draw("genpt","recpt>0 && abs(recdrgen2<0.5) && genpt>40","same");

   // === Do Calculations ===
   calcEffFake(ntlead,vthresh,hnum,hden,heff,hfake, colors);
}
