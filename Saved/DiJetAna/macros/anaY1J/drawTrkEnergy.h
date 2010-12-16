#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPad.h"
#include "TMath.h"
#include "TLatex.h"
#include "TString.h"
#include "Saved/Utilities/macros/histogram/HisMath.C"
#include "Saved/DiJetAna/macros/commonUtility.h"
using namespace std;

TH1D* combine(TH1D* near, TH1D* away, Int_t normType=0, Float_t norm=1.) {
  // Get Bin Info
  Int_t nbin0=near->GetNbinsX();
  Int_t nbinc=nbin0*4;
  Int_t delta=0;
  if (TString(near->GetName()).Contains("1_7")) cout << "nbin0 " << nbin0 << " nbinc: " << nbinc << endl;

  // Combine Near and Away
  TH1D* hcombine = new TH1D(Form("hcombine_%s_%s",near->GetName(),away->GetName()),"",nbinc,-1.0*TMath::PiOver2(),3.0*TMath::PiOver2());
  for(int bin=1+delta; bin<=nbin0*2; bin++) {
    hcombine->SetBinContent(bin,near->GetBinContent(nbin0+1-bin));
    hcombine->SetBinError(bin,near->GetBinError(nbin0+1-bin));
  }
  for(int bin=nbin0+1; bin<=nbin0*2-delta; bin++) {
    hcombine->SetBinContent(bin,near->GetBinContent(bin-nbin0));
    hcombine->SetBinError(bin,near->GetBinError(bin-nbin0));
  }
  for(int bin=nbin0*2+delta+1; bin<=nbin0*3; bin++) {
    hcombine->SetBinContent(bin,away->GetBinContent(nbin0*3+1-bin));
    hcombine->SetBinError(bin,away->GetBinError(nbin0*3+1-bin));
  }
  for(int bin=nbin0*3+1; bin<=nbin0*4-delta; bin++) {
    hcombine->SetBinContent(bin,away->GetBinContent(bin-nbin0*3));
    hcombine->SetBinError(bin,away->GetBinError(bin-nbin0*3));
  }
  // Normalize
  if (normType==1) { //case 1: normalize by near area
    hcombine->Scale(norm);
  }

  hcombine->SetFillColor(near->GetFillColor());
  hcombine->SetStats(0);
  hcombine->SetNdivisions(505,"Y");
  hcombine->SetMinimum(0.0001);
  if (normType==0) {
    hcombine->SetMaximum(46);
    hcombine->SetTitle(";;#frac{1}{N_{dijet}} #frac{d#scale[0.9]{#sum}p_{T}^{track} }{ dR }"); // no 2piR in denominator
  }
  if (normType==1) {
    hcombine->SetMaximum(7.8);
    hcombine->SetTitle(";;#frac{1}{N_{dijet} #scale[0.9]{#sum_{p_{T}>1.5GeV #in Jet1}}p_{T}^{track}} #frac{d#scale[0.9]{#sum}p_{T}^{track} }{ dR }"); // no 2piR in denominator
  }
  hcombine->GetYaxis()->SetTitleFont(63);
  hcombine->GetYaxis()->SetTitleSize(20);
  hcombine->GetYaxis()->CenterTitle();
  hcombine->GetYaxis()->SetTitleOffset(2.8);
  hcombine->GetYaxis()->SetLabelFont(63);
  hcombine->GetYaxis()->SetLabelSize(22);

  return hcombine;
}

//------------------------------------------------------
void drawTrkEnergy(TString infile="drawn_jfh_HCPR_J50U_Cent0to10_Aj24to100_SubEtaRefl.root",
		   bool drawLeg=false, bool drawYLab=false, Int_t logScale=0, Int_t normType=0)
{
  TFile *f = new TFile(infile);
  gStyle->SetMarkerStyle(0);

  TH1D *hPt = (TH1D*) f->Get("hPt");

  TH1D *n0 = (TH1D*) f->Get("hPNDR_1_1");
  TH1D *n1 = (TH1D*) f->Get("hPNDR_1_2");
  TH1D *n2 = (TH1D*) f->Get("hPNDR_1_3");
  //TH1D *n4 = (TH1D*) f->Get("hPNDR_1_4");
  //TH1D *n8 = (TH1D*) f->Get("hPNDR_1_5");
  TH1D *nall = (TH1D*) f->Get(Form("hPNDR_1_%d",hPt->GetNbinsX()));

  TH1D *a0 = (TH1D*) f->Get("hPADR_1_1");
  TH1D *a1 = (TH1D*) f->Get("hPADR_1_2");
  TH1D *a2 = (TH1D*) f->Get("hPADR_1_3");
  //TH1D *a4 = (TH1D*) f->Get("hPADR_1_4");
  //TH1D *a8 = (TH1D*) f->Get("hPADR_1_5");
  TH1D *aall = (TH1D*) f->Get(Form("hPADR_1_%d",hPt->GetNbinsX()));

  TH1::SetDefaultSumw2();

  n0->SetFillColor(kGray);
  n1->SetFillColor(kBlue-3);
  n2->SetFillColor(38);
  //n4->SetFillColor(kOrange-8);
  //n8->SetFillColor(kRed-6);
  nall->SetFillColor(kRed);

  a0->SetFillColor(kGray);
  a1->SetFillColor(kBlue-3);
  a2->SetFillColor(38);
  //a4->SetFillColor(kOrange-8);
  //a8->SetFillColor(kRed-6);
  aall->SetFillColor(kRed);

  //TCanvas *c2 = new TCanvas("c2","c2",600,500);
  Float_t norm = 1./(nall->Integral()*nall->GetBinWidth(1));
  TH1D* hcall = combine(nall,aall,normType,norm);
  //TH1D* hc1248 = combine(n8,a8,normType,norm);
  //TH1D* hc124 = combine(n4,a4,normType,norm);
  TH1D* hc12 = combine(n2,a2,normType,norm);
  TH1D* hc01 = combine(n1,a1,normType,norm);
  TH1D* hc0 = combine(n0,a0,normType,norm);

  hcall->GetXaxis()->SetNdivisions(000,true);
  //if(!drawYLab) hcall->GetYaxis()->SetTitle("");

  hcall->Draw("hist"); hcall->Draw("esame");
  //hc1248->Draw("histsame"); hc1248->Draw("esame"); //chist
  //hc124->Draw("histsame"); hc124->Draw("esame");
  hc12->Draw("histsame"); hc12->Draw("esame");
  hc01->Draw("histsame"); hc01->Draw("esame");
  hc0->Draw("histsame"); hc0->Draw("esame");

  //hc1248->GetXaxis()->SetAxisColor(0);
  //hc1248->GetXaxis()->SetLabelColor(0);

  //TGaxis *naxis = new TGaxis(0,0,1.4,0,0,1.4,505,"+");
  TGaxis *naxis = new TGaxis(-1.4,0,1.4,0,-1.4,1.4,505,"+");
  TGaxis *aaxis = new TGaxis(TMath::Pi()-1.4,0,TMath::Pi()+1.4,0,-1.4,1.4,505,"+");
  naxis->SetTitle("#DeltaR^{track}_{leading jet}"); naxis->CenterTitle(); naxis->SetTitleOffset(-1.3);
  fixedFontAxis(naxis);
  aaxis->SetTitle("#DeltaR^{track}_{sub-leading jet}"); aaxis->CenterTitle(); aaxis->SetTitleOffset(-1.3);
  fixedFontAxis(aaxis);
  naxis->Draw();
  aaxis->Draw();
  //drawPatch(0.294,0.109,0.341,0.163);
  //TGaxis *nuaxis = new TGaxis(-1.0*TMath::PiOver2(),3000.0,1.4,3000.0,-1.0*TMath::PiOver2(),1.4,510,"U-");
  //TGaxis *auaxis = new TGaxis(TMath::Pi()-1.4,3000.0,3.0*TMath::PiOver2(),3000.0,-1.4,TMath::PiOver2(),510,"U-");
  //nuaxis->Draw();
  //auaxis->Draw();

  if (logScale==1) gPad->SetLogy();
  //gPad->SetRightMargin(0.05);
  //gPad->SetLeftMargin(0.18);
  //gPad->SetBottomMargin(0.18);

  if (drawLeg) {
    TLegend *leg = new TLegend(0.35,0.67,0.95,0.95);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetNColumns(2);
    leg->SetTextFont(63);
    leg->SetTextSize(16);
    leg->AddEntry(hc0,Form("%.1f-%.0f GeV/c",hPt->GetBinLowEdge(1),hPt->GetBinLowEdge(2)),"f");
    leg->AddEntry(hc01,Form("%.0f-%.0f GeV/c",hPt->GetBinLowEdge(2),hPt->GetBinLowEdge(3)),"f");
    leg->AddEntry(hc12,Form("%.0f-%.0f GeV/c",hPt->GetBinLowEdge(3),hPt->GetBinLowEdge(4)),"f");
    leg->AddEntry(hcall,Form("%.0f+ GeV/c",hPt->GetBinLowEdge(hPt->GetNbinsX())),"f");
    leg->Draw();
  }


  Double_t nearsum=nall->Integral();
  Double_t awaysum=aall->Integral();
  Double_t cnearsum=hcall->Integral(1,nall->GetNbinsX()*2);
  Double_t cawaysum=hcall->Integral(1+nall->GetNbinsX()*2,hc0->GetNbinsX());
  if (normType==1) {
    cnearsum*=nall->GetBinWidth(1);
    cawaysum*=aall->GetBinWidth(1);
  }

  cout << "integral of dET/dR (input hist) = " << nearsum << "(near-side) \t"
       << awaysum << "(away-side) \t Aj:" << (nearsum-awaysum)/(nearsum+awaysum) << endl;
  cout << "integral of dET/dR (combined hist) = " << cnearsum << "(near-side) \t"
       << cawaysum << "(away-side)" << endl;
}
