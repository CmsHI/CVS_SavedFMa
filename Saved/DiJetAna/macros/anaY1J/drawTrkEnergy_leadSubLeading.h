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
//#include "Saved/Utilities/macros/histogram/HisMath.C"                                                                            
//#include "Saved/DiJetAna/macros/commonUtility.h"                                                                                
#include "HisMath.C"
#include "commonUtility.h"

using namespace std;
int gab = 0;

void jumSun(double x1=0,double y1=0,double x2=1,double y2=1,int color=1, double width=1)
{
   TLine* t1 = new TLine(x1,y1,x2,y2);
   t1->SetLineWidth(width);
   t1->SetLineStyle(7);
   t1->SetLineColor(color);
   t1->Draw();
}

void getTotalNum(TH1D* h) {
   TH1D* hSim = (TH1D*)h->Clone(Form("%s_oneBin",h->GetName()));
   hSim->Rebin(hSim->GetNbinsX());
   //   cout << " bin numbers = " << hSim->GetNbinsX() << endl;
   cout.precision(4);
   cout << hSim->GetBinContent(1) << " (" << hSim->GetBinError(1) << ")";
   delete hSim;
}

TH1D* combine(TH1D* near, TH1D* away, Int_t normType=0, Float_t norm=1., bool Left=true) {
  // Get Bin Info
  Int_t nbin0=near->GetNbinsX();
  Int_t nbinc=nbin0*2;
  Int_t delta=0;

  // Combine Near and Away
  TH1D* hcombine = new TH1D(Form("hcombine_%s_%s_%d",near->GetName(),away->GetName(),Left),"",nbinc,-TMath::Pi()/2, TMath::Pi()/2) ;

  if (Left) {
     for(int bin=1+delta; bin<=nbin0; bin++) {
	hcombine->SetBinContent(bin-gab,near->GetBinContent(nbin0+1-bin));
	hcombine->SetBinError(bin-gab,near->GetBinError(nbin0+1-bin));
     }
  }
  else {// if you want right  
     for(int bin=nbin0+1; bin<=nbin0*2-delta; bin++) {  
	hcombine->SetBinContent(bin,away->GetBinContent(bin-nbin0)); 
	hcombine->SetBinError(bin,away->GetBinError(bin-nbin0)); 
     }
  }

  // Normalize
  if (normType==1) { //case 1: normalize by near area
    hcombine->Scale(norm);
  }

  hcombine->SetFillColor(near->GetFillColor());
  hcombine->SetStats(0);
  hcombine->SetNdivisions(505,"Y");
  hcombine->SetMinimum(-10);
  if (normType==0) {
    hcombine->SetMaximum(59.9);
    //    hcombine->SetTitle(";;#frac{1}{N_{dijet}} #frac{d#scale[0.9]{#sum}p_{T}^{track} }{ dR } (GeV/c)"); // no 2piR in denominator
    hcombine->SetTitle(";;#Sigmap_{T} per bin (GeV/c)");
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

  // Additional Systematic Error ---------------------------------
  AdditionalSysUncert(hcombine,0.2);

  return hcombine;
}

//------------------------------------------------------
void drawTrkEnergy(TString infile="drawn_jfh_HCPR_J50U_Cent0to10_Aj24to100_SubEtaRefl.root",
		   bool drawLeg=false, bool drawYLab=false, Int_t logScale=0, Int_t normType=0)
{
  TH1::SetDefaultSumw2();
  gStyle->SetMarkerStyle(0);

  // =========================================================================
  // Basica Plot Parameters
  // =========================================================================
  Float_t ymin=-5;
  //Int_t colors[5] = {38,kOrange-8,kBlue-3,kGray,kRed};
  Int_t colors[3] = {kOrange-8,kBlue-3,kRed};

  // =========================================================================
  // Inputs
  // =========================================================================
  cout << endl << "=== " << infile << " ===" << endl;
  TFile *f = new TFile(infile);
  // Pt Bins
  TH1D *hPt = (TH1D*) f->Get("hPt");
  if (0) {
    cout << "Before Combine: " << hPt->GetNbinsX() << " pt bins" << endl;
    for (Int_t i=1; i<=hPt->GetNbinsX(); ++i) {
      cout << "Pt bin " << i << ": " << hPt->GetBinLowEdge(i) << " - " << hPt->GetBinLowEdge(i+1) << endl;
    }
  }

  const Int_t nbin=3;
  // v9:  bin1+2 = 1-4GeV
  //	  bin3 = 4-8 GeV
  //	  bin4+5 = 8+ GeV
  Int_t begbins[nbin] = {1,3,4};
  Int_t endbins[nbin] = {2,3,hPt->GetNbinsX()};

  // Near Jet accumulation histograms
  TH1D *Nr[nbin];
  TH1D *Aw[nbin];
  for (Int_t i=0; i<nbin; ++i) {
    Nr[i] = (TH1D*) f->Get(Form("hPNDR_%d_%d",begbins[0],endbins[i]));
    Nr[i]->SetFillColor(colors[i]);
    Aw[i] = (TH1D*) f->Get(Form("hPADR_%d_%d",begbins[0],endbins[i]));
    Aw[i]->SetFillColor(colors[i]);
  }
 
  // =========================================================================
  // Combine Histgrams
  // =========================================================================
  //TCanvas *c2 = new TCanvas("c2","c2",600,500);
  Float_t norm = 1./(Nr[nbin-1]->Integral()*Nr[nbin-1]->GetBinWidth(1));
  TH1D* hcLeft[nbin];
  TH1D* hcRight[nbin];
  int fillLeft = 3004;
  for (Int_t i=0; i<nbin; ++i) {
    hcLeft[i] = combine(Nr[i],Aw[i],normType,norm);
    hcLeft[i]->SetFillStyle(3004);
    if (i!=0) hcLeft[i]->SetLineColor(colors[i]);
    hcRight[i] = combine(Nr[i],Aw[i],normType,norm,false);
  }

  // =========================================================================
  // Plot Axis
  // =========================================================================
  hcLeft[nbin-1]->GetXaxis()->SetNdivisions(000,true);
  //if(!drawYLab) hcLeft[nbin-1]->GetYaxis()->SetTitle("");
  float shftAxis= hcLeft[nbin-1]->GetBinWidth(1)/2.*gab;
  float drRange = 0.8;
  hcLeft[nbin-1]->SetAxisRange(-0.85,0.85,"X"); //TMath::Pi()/2 - drRange-shftAxis, TMath::Pi()/2 + drRange-shftAxis);
  hcLeft[nbin-1]->SetAxisRange(ymin,70,"Y"); //TMath::Pi()/2 - drRange-shftAxis, TMath::Pi()/2 + drRange-shftAxis); 
  fixedFontHist(hcLeft[nbin-1]);
  
  // =========================================================================
  // Draw Final Plot
  // =========================================================================
  for (Int_t i=nbin-1; i>=0; --i) {
    if (i==nbin-1) {
      hcLeft[i]->Draw("hist"); hcLeft[i]->Draw("esame");
    }
    else {
      hcLeft[i]->Draw("histsame"); hcLeft[i]->Draw("esame");
    }
    hcRight[i]->Draw("histsame"); hcRight[i]->Draw("esame");
  }
  jumSun(0,0,0,hcLeft[nbin-1]->GetMaximum(),1,1);
  TLine * l0 = new TLine(-0.8,0,0.8,0);
  l0->Draw();
  
  // =========================================================================
  // Draw Final Axis
  // =========================================================================
  TF1 *f1=new TF1("f1","-x",0,0.8);
  TGaxis *naxis = new TGaxis(-0.8,ymin,0,ymin,"f1",3,"+");
  TF1 *f2=new TF1("f2","x",0,0.8);
  TGaxis *aaxis = new TGaxis(0,ymin,0.8,ymin,"f2",3,"+");

  naxis->SetTitle("#DeltaR^{Leading jet}"); naxis->CenterTitle(); naxis->SetTitleOffset(1.7);
  fixedFontAxis(naxis);
  aaxis->SetTitle("#DeltaR^{SubLeading jet}"); aaxis->CenterTitle(); aaxis->SetTitleOffset(1.7);
  fixedFontAxis(aaxis);
  naxis->SetTitleSize(22);
  aaxis->SetTitleSize(22);
  naxis->SetTitleOffset(2.3);
  aaxis->SetTitleOffset(2.3);
  naxis->Draw();
  aaxis->Draw();

  if (logScale==1) gPad->SetLogy();

  // =========================================================================
  // Draw Legend
  // =========================================================================
  if (drawLeg) {
     TLegend *leg = new TLegend(0.01425723,0.6506156,0.4549146,0.9576504);
     leg->SetFillStyle(0);
     leg->SetBorderSize(0);
     leg->SetNColumns(1);
     leg->SetTextSize(0.05);
     for (Int_t i=nbin-1;i>=0;--i) {
       if (i==nbin-1) leg->AddEntry(hcRight[i],Form("> %.0f GeV/c",hPt->GetBinLowEdge(begbins[i])),"f");
       else leg->AddEntry(hcRight[i],Form("%.0f-%.0f GeV/c",hPt->GetBinLowEdge(begbins[i]),hPt->GetBinLowEdge(endbins[i]+1)),"f");
     }
    leg->Draw();
  }

  // =========================================================================
  // Get Numbers
  // =========================================================================
  for (Int_t i=0; i<nbin; ++i) {
    cout << Form("integral (%.1f-%.1f GeV) = ",hPt->GetBinLowEdge(begbins[0]),hPt->GetBinLowEdge(endbins[i]+1));
    //Double_t nearsum=Nr[i]->Integral();
    //Double_t awaysum=Aw[i]->Integral();
    //cout << "Nr: " << nearsum << "\t" << "Aw: " << awaysum << "\t" << "Aj:" << (nearsum-awaysum)/(nearsum+awaysum) << endl;
    cout << " Nr: "; getTotalNum(Nr[i]); cout << "\t";
    cout << " Aw: "; getTotalNum(Aw[i]); cout << endl;
  }
}
