#include "TFile.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

void leadAna (char * infile = "MyEffHist_0.root")
{
   TFile * intf = new TFile(infile);
   TNtuple * ntlead;
   intf->GetObject("ntlead",ntlead);
   Int_t NBIN=30;
   Int_t MAXPT=150;
   //ntlead->Print();

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

   // Efficiency
   TCanvas * cpt = new TCanvas("cpt","cpt");
   cpt->SetLogy();
   ntlead->Draw("genpt>>hden","genpt>0","hist");
   ntlead->Draw("genpt>>hnum","recpt>50 && abs(receta)<3","same hist");

   TCanvas * ceff = new TCanvas("ceff","ceff",500,500);
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(hnum,hden);
   gEfficiency->SetMarkerColor(kGreen-2);
   gEfficiency->SetMarkerStyle(kFullCircle);
   gEfficiency->SetMarkerSize(1.2);
   heff->Draw();
   gEfficiency->Draw("pz");

   // Fakes
   TCanvas * cptfake = new TCanvas("cptfake","cptfake");
   cptfake->SetLogy();
   ntlead->Draw("recpt>>hdenFake","recpt>0 && abs(receta)<3","hist");
   ntlead->Draw("recpt>>hnumFake","recpt>0 && abs(receta)<3 && abs(recdr)>0.5 && abs(recdrgen2)>0.5","same hist");

   TGraphAsymmErrors *gFake = new TGraphAsymmErrors();
   gFake->BayesDivide(hnumFake,hdenFake);
   gFake->SetMarkerColor(kRed);
   gFake->SetMarkerStyle(kOpenCircle);
   gFake->SetMarkerSize(1.2);

   TCanvas * cfake = new TCanvas("cfake","cfake",500,500);
   hfake->Draw();
   gFake->Draw("pz");
}
