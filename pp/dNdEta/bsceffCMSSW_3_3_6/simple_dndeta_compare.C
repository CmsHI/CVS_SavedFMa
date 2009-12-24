#include <TStyle.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TF1.h>
#include <TProfile.h>
using namespace std;

void formatHist(TH1* h, int col = 1, double norm = 1);

void simple_dndeta_compare(){
  int nbins = 10;

  TFile *f1 = new TFile("outputs/pythiaD6T_bscEff100.root");
  TTree* t1 = dynamic_cast<TTree*>(f1->Get("ana/hi"));
  TFile *f2 = new TFile("outputs/pythiaD6T_bscEff95.root");
  TTree* t2 = dynamic_cast<TTree*>(f2->Get("ana/hi"));
  TFile *f3 = new TFile("outputs/pythia_1300.root");
  TTree* t3 = dynamic_cast<TTree*>(f3->Get("ana/hi"));

  TFile * outf = new TFile("genana.root","RECREATE");
  TH1F* h0 = new TH1F("h0",";#eta;dN/d#eta",100,-8,8);
  TH1F* h1300 = new TH1F("h1300",";#eta;dN/d#eta",100,-8,8);
  TH1F* h1 = new TH1F("h1",";#eta;dN/d#eta",nbins,-3,3);
  TH1F* h2 = new TH1F("h2",";#eta;dN/d#eta",nbins,-3,3);

  int n0Pass = t1->GetEntries("1==1");
  int n1Pass = t1->GetEntries("tbit[40]");
  int n2Pass = t2->GetEntries("tbit[40]");
  int n3Pass = t3->GetEntries("1==1");
  cout << "case 0) all: " << n0Pass << endl;
  cout << "case 1300) all: " << n3Pass << endl;
  cout << "case 1) bit40 fired: " << n1Pass << endl;
  cout << "case 2) bit40 fired: " << n2Pass << endl;

  t1->Draw("eta>>h0","chg!=0");
  t3->Draw("eta>>h1300","chg!=0 && st==1");
  t1->Draw("eta>>h1","tbit[40] && chg!=0");
  t2->Draw("eta>>h2","tbit[40] && chg!=0");

  float scale0 = (float)n0Pass*(h0->GetBinWidth(1));
  float scale3 = (float)n3Pass*(h1300->GetBinWidth(1));
  float scale1 = (float)n1Pass/nbins*6.;
  float scale2 = (float)n2Pass/nbins*6.;

  h0->Sumw2();
  h1300->Sumw2();
  h1->Sumw2();
  h2->Sumw2();
  h0->Scale(1./scale0);
  h1300->Scale(1./scale3);
  h1->Scale(1./scale1);
  h2->Scale(1./scale2);

  h1->SetMarkerColor(1);
  h1->SetLineColor(2);
  h0->SetLineColor(2);
  h1300->SetLineColor(kGreen-2);
  h2->SetMarkerColor(4);
  h2->SetLineColor(4);

  h0->SetMaximum(4);
  h1->SetMaximum(4);
  h1->SetMinimum(0);
  h2->SetMinimum(0);

  TCanvas * c3 = new TCanvas("c3","c3",600,600);
  h1->Draw("hist");
  h2->Draw("hist same");

  TLegend* leg2 = new TLegend(0.30,0.4,0.8,0.30,"(Pythia) dN/dEta","brNDC");
  leg2->AddEntry(h1,"trigger on 40, with BSC 100%","l");
  leg2->AddEntry(h2,"trigger on 40, with BSC 95%,","l");
  leg2->SetFillColor(0);
  leg2->SetTextSize(0.04);
  leg2->SetBorderSize(0);
  leg2->Draw();
  c3->SaveAs("compare_bsc.gif");
  c3->SaveAs("compare_bsc.eps");

  TCanvas * cdiff = new TCanvas("cdiff","cdiff",600,600);
  TH1F* h3 = new TH1F("h3",";#eta;dN/d#eta",nbins,-3,3);
  h3->Add(h1,h2,-1,1);
  h3->Divide(h1);
  h3->Scale(100.);
  h3->SetYTitle("\% difference");
  h3->SetLineColor(1);
  h3->SetMinimum(0);
  h3->Draw("hist");
  cdiff->Print("compare_bsc_diff.gif");
  cdiff->Print("compare_bsc_diff.eps");

  TCanvas * c0 = new TCanvas("c0","c0",600,600);
  h0->Draw("hist");
  h1300->Draw("hist same");
  TLegend* leg0 = new TLegend(0.30,0.4,0.8,0.30,"(Pythia) dN/dEta","brNDC");
  leg0->SetFillColor(0);
  leg0->SetTextSize(0.04);
  leg0->SetBorderSize(0);
  leg0->Draw();

  // save and exit
  outf->Write();
  return;
}

void formatHist(TH1* h, int col, double norm){

  h->Scale(1/norm);
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetMarkerSize(0.7);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

}
