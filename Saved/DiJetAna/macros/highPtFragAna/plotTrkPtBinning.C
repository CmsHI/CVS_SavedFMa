#include <iostream>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>
#include "TLegend.h"
#include "TPad.h"
#include "HisMath.C"
using namespace std;

#define PI 3.14159265358979

TH1D * drawPt(TChain * t, TString name, TString var, TCut sel, vector<Float_t> bins) {
  TH1D * h = new TH1D(name,"",bins.size()-1,&bins[0]);
  t->Project(name,var,sel);
  return h;
}

void plotTrkPtBinning(TString infile="trknt_hydjetBass_v2_dijet110.root",
    TCut evtCut="cbin<2&&jetr>110&&abs(etas)<1")
{
  TChain * t = new TChain("hitrkEffAnalyzer/simTrackTree");
  t->Add(infile);

  // ============================================
  // Histogram Bins
  // ============================================
  Float_t pt, small=1e-3;
  Int_t nbin=120;

  vector<Float_t> ptBinsA0;
  for(pt =   0  ; pt <   1.2-small; pt +=  0.05) ptBinsA0.push_back(pt); // 24 bins
  for(pt =   1.2; pt <   2.4-small; pt +=  0.1 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =   2.4; pt <   7.2-small; pt +=  0.2 ) ptBinsA0.push_back(pt); // 24 bins
  for(pt =   7.2; pt <  13.2-small; pt +=  0.5 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =  13.2; pt <  25.2-small; pt +=  1.0 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =  25.2; pt <  61.2-small; pt +=  3.0 ) ptBinsA0.push_back(pt); // 12 bins
  for(pt =  61.2; pt < 181.2-small; pt +=  5.0 ) ptBinsA0.push_back(pt); // 24 bins
  ptBinsA0.push_back(181.2);

  vector<Float_t> ptBinsA1;
  for(pt =   0  ; pt <   1.2-small; pt +=  0.05) ptBinsA1.push_back(pt); // 24 bins 
  for(pt =   1.2; pt <   2.4-small; pt +=  0.1 ) ptBinsA1.push_back(pt); // 12 bins
  for(pt =   2.4; pt <   7.2-small; pt +=  0.4 ) ptBinsA1.push_back(pt); // 12 bins
  for(pt =   7.2; pt <  16.8-small; pt +=  1.6 ) ptBinsA1.push_back(pt); // 6 bins 
  for(pt =  16.8; pt <  55.2-small; pt +=  6.4 ) ptBinsA1.push_back(pt); // 6 bins
  for(pt =  55.2; pt < 170.4-small; pt +=  9.6 ) ptBinsA1.push_back(pt); // 12 bins
  ptBinsA1.push_back(170.4);

  vector<Float_t> ptBinsB0;
  cout << "B0: ";
  Float_t a = 1;
  for (Int_t i=nbin; i>0; --i) {
    Float_t y = i/Float_t(nbin);
    Float_t pt = pow(y,-1./a);
    printf("%.3f ",pt);
    ptBinsB0.push_back(pt);
  }
  if (ptBinsB0.back()<200) ptBinsB0.push_back(200);
  cout << ptBinsB0.back() << endl;

  vector<Float_t> ptBinsB1;
  cout << "B1: ";
  a = 0.8;
  for (Int_t i=nbin; i>0; --i) {
    Float_t y = i/Float_t(nbin);
    Float_t pt = pow(y,-1./a);
    printf("%.3f ",pt);
    ptBinsB1.push_back(pt);
  }
  if (ptBinsB1.back()<200) ptBinsB1.push_back(200);
  cout << ptBinsB1.back() << endl;

  // ============================================
  // Analyze
  // ============================================
  TH1D * hPPtA0 = drawPt(t,"hPPtA0","pts",evtCut,ptBinsA0);
  //hPPtA0->Scale(1./hPPtA0->GetBinContent(hPPtA0->FindBin(4)));

  TH1D * hPPtA1 = drawPt(t,"hPPtA1","pts",evtCut,ptBinsA1);
  //hPPtA1->Scale(1./hPPtA1->GetBinContent(hPPtA1->FindBin(4)));

  TH1D * hPPtB0 = drawPt(t,"hPPtB0","pts",evtCut,ptBinsB0);
  //hPPtB0->Scale(1./hPPtB0->GetBinContent(hPPtB0->FindBin(4)));

  TH1D * hPPtB1 = drawPt(t,"hPPtB1","pts",evtCut,ptBinsB1);

  // ============================================
  // Plot
  // ============================================
  TH1D * hFrame = new TH1D("hFrame",";Trk p_{T} (GeV/c); count",400,0.1,400.1);
  hFrame->SetAxisRange(2,400,"X");
  hFrame->SetAxisRange(1,1e5,"Y");
  Int_t colors[10] = {kGray+2,kViolet,kBlue+1,kGreen+2,kOrange+2,kMagenta,kRed};
  hPPtA0->SetLineColor(colors[0]);
  hPPtA1->SetLineColor(colors[1]);
  hPPtB0->SetLineColor(colors[2]);
  hPPtB1->SetLineColor(colors[3]);

  TCanvas *c2 = new TCanvas("c2","Trk",500,500);
  c2->SetLogx();
  c2->SetLogy();
  hFrame->Draw();
  hPPtA0->Draw("same");
  hPPtA1->Draw("same");
  hPPtB0->Draw("same");
  hPPtB1->Draw("same");

  TCanvas *c3 = new TCanvas("c3","Trk",500,500);
  c3->SetLogx();
  c3->SetLogy();
  hFrame->Draw();
  hPPtA1->Draw("same");

  TCanvas *c4 = new TCanvas("c4","Trk",500,500);
  c4->SetLogx();
  c4->SetLogy();
  hFrame->Draw();
  hPPtB0->Draw("same");

  TCanvas *c5 = new TCanvas("c5","Trk",500,500);
  c5->SetLogx();
  c5->SetLogy();
  hFrame->Draw();
  hPPtB1->Draw("same");

  // ====================
  TLegend *leg = new TLegend(0.61,0.78,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  //leg->AddEntry(vhPPtGen[0],"#DeltaR(jet,trk)<0.5","");
  //leg->Draw();
}
