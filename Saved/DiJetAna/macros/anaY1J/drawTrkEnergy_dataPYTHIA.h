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
int gab = 0;

void jumSun(double x1=0,double y1=0,double x2=1,double y2=1,int color=1, double width=1)
{
   TLine* t1 = new TLine(x1,y1,x2,y2);
   t1->SetLineWidth(width);
   t1->SetLineStyle(7);
   t1->SetLineColor(color);
   t1->Draw();
}

/*
TH1D* foldHalf(TH1D* h, bool isLeft=true) {
   int nBins = h->GetNbinsX();
   int centerBin = h->FindBin(0.0000001);
   cout<< "nBins = " << nBins << "   and half bin = " << centerBin << endl;
   TH1D* hfolded = (TH1D*)h->Clone( Form("hfolded_%s",h->GetName()) );
   hfolded->Reset();
   for ( int i=1 ; int i<=nBins ; i++) {
      hfolded->SetBinContent( i ,   h->GetBinContent(nBins-i+1) );
      hfolded->SetBinError( i , h->GetBinError(nBins-i+1) );
   }
   hfolded->Add(h);
   if ( isLeft) {
      for ( int i=1 ; int i<= ( nBins/2 ) ; i++) {
	 hfolded->SetBinContent(i,0);
	 hfolded->SetBinError(i,0);
      }
   }
   else {
      for ( int i= nBins/2+1 ; int i<= nBins ; i++) {
	 hfolded->SetBinContent(i,0);
	 hfolded->SetBinError(i,0);
      } 
   }
   return hfolded;
   
}

*/

TH1D* combine(TH1D* near, TH1D* away, Int_t normType=0, Float_t norm=1., bool Left=true) {
  // Get Bin Info
  Int_t nbin0=near->GetNbinsX();
  Int_t nbinc=nbin0*2;
  Int_t delta=0;
  if (TString(near->GetName()).Contains("1_7")) cout << "nbin0 " << nbin0 << " nbinc: " << nbinc << endl;

  // Combine Near and Away
  TH1D* hcombine = new TH1D(Form("hcombine_%s_%s",near->GetName(),away->GetName()),"",nbinc,-TMath::Pi()/2, TMath::Pi()/2) ;

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

  //  for(int bin=nbin0+1; bin<=nbin0*2-delta; bin++) {
  //    hcombine->SetBinContent(bin,near->GetBinContent(bin-nbin0));
  //    hcombine->SetBinError(bin,near->GetBinError(bin-nbin0));
  //  }
  //  for(int bin=nbin0*2+delta+1; bin<=nbin0*3; bin++) {
  //   hcombine->SetBinContent(bin,away->GetBinContent(nbin0*3+1-bin));
  //   hcombine->SetBinError(bin,away->GetBinError(nbin0*3+1-bin));
  //  }
  // for(int bin=nbin0*3+1; bin<=nbin0*4-delta; bin++) {
  //   hcombine->SetBinContent(bin,away->GetBinContent(bin-nbin0*3));
  //    hcombine->SetBinError(bin,away->GetBinError(bin-nbin0*3));
  //  }

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

  return hcombine;
}

//------------------------------------------------------
void drawTrkEnergy(TString infile="pythia",
		   TString infile2="data",
		   bool drawLeg=false, bool drawYLab=false, Int_t logScale=0, Int_t normType=0,
		   bool isLeading=true)

{
   TFile *fPythia = new TFile(infile);
  gStyle->SetMarkerStyle(0);

  TH1D *hPt = (TH1D*) fPythia->Get("hPt");

  TH1D *n0Py = (TH1D*) fPythia->Get("hPNDR_1_1");
  TH1D *n1Py = (TH1D*) fPythia->Get("hPNDR_1_2");
  TH1D *n2Py = (TH1D*) fPythia->Get("hPNDR_1_3");
  //  TH1D *n4Py = (TH1D*) fPythia->Get("hPNDR_1_4");
  //  TH1D *n8Py = (TH1D*) fPythia->Get("hPNDR_1_5");
  TH1D *nallPy = (TH1D*) fPythia->Get(Form("hPNDR_1_%d",hPt->GetNbinsX()));

  TH1D *a0Py = (TH1D*) fPythia->Get("hPADR_1_1");
  TH1D *a1Py = (TH1D*) fPythia->Get("hPADR_1_2");
  TH1D *a2Py = (TH1D*) fPythia->Get("hPADR_1_3");
  //  TH1D *a4Py = (TH1D*) fPythia->Get("hPADR_1_4");
  //  TH1D *a8Py = (TH1D*) fPythia->Get("hPADR_1_5");
  TH1D *aallPy = (TH1D*) fPythia->Get(Form("hPADR_1_%d",hPt->GetNbinsX()));

  TFile *fData = new TFile(infile2);
  

  TH1D *n0 = (TH1D*) fData->Get("hPNDR_1_1");
  TH1D *n1 = (TH1D*) fData->Get("hPNDR_1_2");
  TH1D *n2 = (TH1D*) fData->Get("hPNDR_1_3");
  // TH1D *n4 = (TH1D*) fData->Get("hPNDR_1_4");
  // TH1D *n8 = (TH1D*) fData->Get("hPNDR_1_5");
  TH1D *nall = (TH1D*) fData->Get(Form("hPNDR_1_%d",hPt->GetNbinsX()));

  TH1D *a0 = (TH1D*) fData->Get("hPADR_1_1");
  TH1D *a1 = (TH1D*) fData->Get("hPADR_1_2");
  TH1D *a2 = (TH1D*) fData->Get("hPADR_1_3");
  //  TH1D *a4 = (TH1D*) fData->Get("hPADR_1_4");
  //  TH1D *a8 = (TH1D*) fData->Get("hPADR_1_5");
  TH1D *aall = (TH1D*) fData->Get(Form("hPADR_1_%d",hPt->GetNbinsX()));

  
  TH1::SetDefaultSumw2();

  n0->SetFillColor(kGray);
  n1->SetFillColor(kBlue-3);
  n2->SetFillColor(38);
  //  n4->SetFillColor(kOrange-8);
  //  n8->SetFillColor(kRed-6);
  nall->SetFillColor(kRed);
  a0->SetFillColor(kGray);
  a1->SetFillColor(kBlue-3);
  a2->SetFillColor(38);
  // a4->SetFillColor(kOrange-8);
  // a8->SetFillColor(kRed-6);
  aall->SetFillColor(kRed);

  n0Py->SetFillColor(kGray);
  n1Py->SetFillColor(kBlue-3);
  n2Py->SetFillColor(38);
  //  n4Py->SetFillColor(kOrange-8);
  //  n8Py->SetFillColor(kRed-6);
  nallPy->SetFillColor(kRed);
  a0Py->SetFillColor(kGray);
  a1Py->SetFillColor(kBlue-3);
  a2Py->SetFillColor(38);
  //  a4Py->SetFillColor(kOrange-8);
  //  a8Py->SetFillColor(kRed-6);
  aallPy->SetFillColor(kRed);

 
  //TCanvas *c2 = new TCanvas("c2","c2",600,500);
  Float_t norm = 1./(nall->Integral()*nall->GetBinWidth(1));
  TH1D* hcall;
  //  TH1D* hc1248;
  //  TH1D* hc124;
  TH1D* hc12;
  TH1D* hc01;
  TH1D* hc0;

  TH1D* hcallRight;
  //  TH1D* hc1248Right;
  //  TH1D* hc124Right;
  TH1D* hc12Right;
  TH1D* hc01Right;
  TH1D* hc0Right;

  if ( isLeading ) {
     hcall = combine(nallPy,nallPy,normType,norm);
     //    hc1248 = combine(n8Py,n8Py,normType,norm);
     //   hc124 = combine(n4Py,n4Py,normType,norm);
     hc12 = combine(n2Py,n2Py,normType,norm);
     hc01 = combine(n1Py,n1Py,normType,norm);
     hc0 = combine(n0Py,n0Py,normType,norm);

     hcallRight = combine(nall,nall,normType,norm,false);
     //   hc1248Right = combine(n8,n8,normType,norm,false);
     //  hc124Right = combine(n4,n4,normType,norm,false);
     hc12Right = combine(n2,n2,normType,norm,false);
     hc01Right = combine(n1,n1,normType,norm,false);
     hc0Right = combine(n0,n0,normType,norm,false);
  }
  else {
     hcall = combine(aallPy,aallPy,normType,norm);
     //   hc1248 = combine(a8Py,a8Py,normType,norm);
     //    hc124 = combine(a4Py,a4Py,normType,norm);
     hc12 = combine(a2Py,a2Py,normType,norm);
     hc01 = combine(a1Py,a1Py,normType,norm);
     hc0 = combine(a0Py,a0Py,normType,norm);

     hcallRight = combine(aall,aall,normType,norm,false);
     //  hc1248Right = combine(a8,a8,normType,norm,false);
     //   hc124Right = combine(a4,a4,normType,norm,false);
     hc12Right = combine(a2,a2,normType,norm,false);
     hc01Right = combine(a1,a1,normType,norm,false);
     hc0Right = combine(a0,a0,normType,norm,false);

  }
  
  
  int fillLeft = 3004;
  
  hcall->SetFillStyle(3004);
  // hc1248->SetFillStyle(fillLeft);
  //  hc124->SetFillStyle(fillLeft);
  hc12->SetFillStyle(3004);
  hc01->SetFillStyle(3004);
  hc0->SetFillStyle(3004);
  
  hcall->SetLineColor(kRed);                                                                                                                                                      
  // hc1248->SetLineColor(kRed-6);                                                                                                                                                         
  //  hc124->SetLineColor(kOrange-8);                                                                                                                                                       
  hc12->SetLineColor(38);                                                                                                                                                               
  hc01->SetLineColor(kBlue-3);                                                                                                                                                           
  hc0->SetLineColor(1);     
  //  hcall->SetLineWidth(2);
  //  hc1248->SetLineWidth(2);
  //  hc124->SetLineWidth(2);
  //  hc12->SetLineWidth(2);
  //  hc0->SetLineWidth(2);
  // hc0->SetLineWidth(2);

  /*  hcallRight->SetFillColor(kRed-6);
  hc1248Right->SetFillColor(kRed-7);
  hc124Right->SetFillColor(kOrange-9);
  hc12Right->SetFillColor(33);
  hc0Right->SetFillColor(kBlue-4);
  hc0Right->SetFillColor(kGray-1);
  */

  hcall->GetXaxis()->SetNdivisions(000,true);
  //if(!drawYLab) hcall->GetYaxis()->SetTitle("");

  // Original legend = 0 ~ Pi;
  
  float shftAxis= hc12Right->GetBinWidth(1)/2.*gab;
  float drRange = 0.8;
  hcall->SetAxisRange(-0.85,0.85); //TMath::Pi()/2 - drRange-shftAxis, TMath::Pi()/2 + drRange-shftAxis);
  hcall->Draw("hist"); hcall->Draw("esame");
  //  hc1248->Draw("histsame"); hc1248->Draw("esame"); //chist
  //  hc124->Draw("histsame"); hc124->Draw("esame");
  hc12->Draw("histsame"); hc12->Draw("esame");
  hc01->Draw("histsame"); hc01->Draw("esame");
  hc0->Draw("histsame"); hc0->Draw("esame");
  
  hcallRight->Draw("histsame"); hcallRight->Draw("esame");
  //  hc1248Right->Draw("histsame"); hc1248Right->Draw("esame"); //chist
  //  hc124Right->Draw("histsame"); hc124Right->Draw("esame");
  hc12Right->Draw("histsame"); hc12Right->Draw("esame");
  hc01Right->Draw("histsame"); hc01Right->Draw("esame");
  hc0Right->Draw("histsame"); hc0Right->Draw("esame");
  
  jumSun(0,0,0,hcall->GetMaximum(),1,1);

  //hc1248->GetXaxis()->SetAxisColor(0);
  //hc1248->GetXaxis()->SetLabelColor(0);

  //TGaxis *naxis = new TGaxis(0,0,1.4,0,0,1.4,505,"+");
  //  TGaxis *naxis = new TGaxis(-1.4,0,1.4,0,-1.4,1.4,505,"+");
  //  TGaxis *naxis = new TGaxis(0,0,TMath::Pi()/2,0,TMath::Pi(),0,10,"+");
  // TGaxis *aaxis = new TGaxis(TMath::Pi()-1.4,0,TMath::Pi()+1.4,0,-1.4,1.4,505,"+");
  TF1 *f1=new TF1("f1","-x",0,0.8);
  TGaxis *naxis = new TGaxis(-0.8,0,0,0,"f1",3,"+");
  TF1 *f2=new TF1("f2","x",0,0.8);
  TGaxis *aaxis = new TGaxis(0,0,0.8,0,"f2",3,"+");

  naxis->SetTitle("#DeltaR^{track}_{HYDJET+PYTHIA}"); naxis->CenterTitle(); naxis->SetTitleOffset(1.7);
  fixedFontAxis(naxis);
  aaxis->SetTitle("#DeltaR^{track}_{Data}"); aaxis->CenterTitle(); aaxis->SetTitleOffset(1.7);
  fixedFontAxis(aaxis);
  naxis->SetTitleSize(18);
  aaxis->SetTitleSize(18);
  naxis->SetTitleOffset(2.3);
  aaxis->SetTitleOffset(2.3);
  naxis->Draw();
  aaxis->Draw();

  
  //  aaxis->Draw();
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
     TLegend *leg = new TLegend(0.01591379,0.5262957,0.4557399,0.9685675);
     leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetNColumns(1);
    leg->SetTextSize(0.05);
    /*
    leg->AddEntry(hc0,"0.5-1 GeV/c","f");
    leg->AddEntry(hc01,"1-2 GeV/c","f");
    leg->AddEntry(hc12,"2-4 GeV/c","f");
    leg->AddEntry(hc124,"4-8 GeV/c","f");
    leg->AddEntry(hc1248,"16+ GeV/c","f");
    */
    //  leg->AddEntry(hcallRight,"36+ GeV/c","f");
    //  leg->AddEntry(hc1248Right,"18-36 GeV/c","f");
    //  leg->AddEntry(hc124Right,"9-18 GeV/c","f");
    // leg->AddEntry(hc12Right,"6-9 GeV/c","f");
    // leg->AddEntry(hc01Right,"3-6 GeV/c","f");
    //  leg->AddEntry(hc0Right,"1.5-3 GeV/c","f");
    leg->AddEntry(hcallRight,Form("%.0f+ GeV/c",hPt->GetBinLowEdge(hPt->GetNbinsX())),"f");
    leg->AddEntry(hc12Right,Form("%.0f-%.0f GeV/c",hPt->GetBinLowEdge(3),hPt->GetBinLowEdge(4)),"f");
    leg->AddEntry(hc01Right,Form("%.0f-%.0f GeV/c",hPt->GetBinLowEdge(2),hPt->GetBinLowEdge(3)),"f");
    leg->AddEntry(hc0Right,Form("%.1f-%.0f GeV/c",hPt->GetBinLowEdge(1),hPt->GetBinLowEdge(2)),"f");
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