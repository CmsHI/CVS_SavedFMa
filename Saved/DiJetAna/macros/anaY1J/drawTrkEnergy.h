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
using namespace std;

void drawText(const char *text, float xp, float yp){
   TLatex *tex = new TLatex(xp,yp,text);
   tex->SetTextFont(63);
   tex->SetTextSize(16);
   //tex->SetTextSize(0.05);                                                                   
   tex->SetTextColor(kBlack);
   tex->SetLineWidth(1);
   tex->SetNDC();
   tex->Draw();
}


//----------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv,
			  const Int_t columns, 
			  const Int_t rows,
			  const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05) {
   if (canv==0) {
      Error("makeMultiPanelCanvas","Got null canvas.");
      return;
   }
   canv->Clear();
   
   TPad* pad[columns][rows];

   Float_t Xlow[columns];
   Float_t Xup[columns];
   Float_t Ylow[rows];
   Float_t Yup[rows];
   Float_t PadWidth = 
   (1.0-leftOffset)/((1.0/(1.0-leftMargin)) +
   (1.0/(1.0-edge))+(Float_t)columns-2.0);
   Float_t PadHeight =
   (1.0-bottomOffset)/((1.0/(1.0-bottomMargin)) +
   (1.0/(1.0-edge))+(Float_t)rows-2.0);
   Xlow[0] = leftOffset;
   Xup[0] = leftOffset + PadWidth/(1.0-leftMargin);
   Xup[columns-1] = 1;
   Xlow[columns-1] = 1.0-PadWidth/(1.0-edge);

   Yup[0] = 1;
   Ylow[0] = 1.0-PadHeight/(1.0-edge);
   Ylow[rows-1] = bottomOffset;
   Yup[rows-1] = bottomOffset + PadHeight/(1.0-bottomMargin);

   for(Int_t i=1;i<columns-1;i++) {
      Xlow[i] = Xup[0] + (i-1)*PadWidth;
      Xup[i] = Xup[0] + (i)*PadWidth;
   }
   Int_t ct = 0;
   for(Int_t i=rows-2;i>0;i--) {
      Ylow[i] = Yup[rows-1] + ct*PadHeight;
      Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
      ct++;
   }

   TString padName;
   for(Int_t i=0;i<columns;i++) {
      for(Int_t j=0;j<rows;j++) {
         canv->cd();
         padName = Form("p_%d_%d",i,j);
         pad[i][j] = new TPad(padName.Data(),padName.Data(),
            Xlow[i],Ylow[j],Xup[i],Yup[j]);
         if(i==0) pad[i][j]->SetLeftMargin(leftMargin);
         else pad[i][j]->SetLeftMargin(0);

         if(i==(columns-1)) pad[i][j]->SetRightMargin(edge);
         else pad[i][j]->SetRightMargin(0);

         if(j==0) pad[i][j]->SetTopMargin(edge);
         else pad[i][j]->SetTopMargin(0);

         if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
         else pad[i][j]->SetBottomMargin(0);

         pad[i][j]->Draw();
         pad[i][j]->cd();
         pad[i][j]->SetNumber(columns*j+i+1);
      }
   }
}

TH1D* combine(TH1D* near, TH1D* away, Int_t normType=0, Float_t norm=1.) {
  // Get Bin Info
  Int_t nbin0=near->GetNbinsX();
  Int_t nbinc=nbin0*4;
  Int_t delta=0;
  cout << "nbin0 " << nbin0 << " nbinc: " << nbinc << endl;

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

  TH1D *n0 = (TH1D*) f->Get("hPNDR_1_1");
  TH1D *n1 = (TH1D*) f->Get("hPNDR_1_2");
  TH1D *n2 = (TH1D*) f->Get("hPNDR_1_3");
  TH1D *n4 = (TH1D*) f->Get("hPNDR_1_4");
  TH1D *n8 = (TH1D*) f->Get("hPNDR_1_5");
  TH1D *nall = (TH1D*) f->Get("hPNDR_1_7");

  TH1D *a0 = (TH1D*) f->Get("hPADR_1_1");
  TH1D *a1 = (TH1D*) f->Get("hPADR_1_2");
  TH1D *a2 = (TH1D*) f->Get("hPADR_1_3");
  TH1D *a4 = (TH1D*) f->Get("hPADR_1_4");
  TH1D *a8 = (TH1D*) f->Get("hPADR_1_5");
  TH1D *aall = (TH1D*) f->Get("hPADR_1_7");

  TH1::SetDefaultSumw2();

  n0->SetFillColor(kGray);
  n1->SetFillColor(kBlue-3);
  n2->SetFillColor(38);
  n4->SetFillColor(kOrange-8);
  n8->SetFillColor(kRed-6);
  nall->SetFillColor(kRed);

  a0->SetFillColor(kGray);
  a1->SetFillColor(kBlue-3);
  a2->SetFillColor(38);
  a4->SetFillColor(kOrange-8);
  a8->SetFillColor(kRed-6);
  aall->SetFillColor(kRed);

  //TCanvas *c2 = new TCanvas("c2","c2",600,500);
  Float_t norm = 1./(nall->Integral()*nall->GetBinWidth(1));
  TH1D* hcall = combine(nall,aall,normType,norm);
  TH1D* hc1248 = combine(n8,a8,normType,norm);
  TH1D* hc124 = combine(n4,a4,normType,norm);
  TH1D* hc12 = combine(n2,a2,normType,norm);
  TH1D* hc01 = combine(n1,a1,normType,norm);
  TH1D* hc0 = combine(n0,a0,normType,norm);

  hcall->GetXaxis()->SetNdivisions(000,true);
  //if(!drawYLab) hcall->GetYaxis()->SetTitle("");

  hcall->Draw("hist"); hcall->Draw("esame");
  hc1248->Draw("histsame"); hc1248->Draw("esame"); //chist
  hc124->Draw("histsame"); hc124->Draw("esame");
  hc12->Draw("histsame"); hc12->Draw("esame");
  hc01->Draw("histsame"); hc01->Draw("esame");
  hc0->Draw("histsame"); hc0->Draw("esame");

  //hc1248->GetXaxis()->SetAxisColor(0);
  //hc1248->GetXaxis()->SetLabelColor(0);

  TGaxis *naxis = new TGaxis(-1.4,0,1.4,0,-1.4,1.4,505,"+");
  TGaxis *aaxis = new TGaxis(TMath::Pi()-1.4,0,TMath::Pi()+1.4,0,-1.4,1.4,505,"+");
  naxis->SetTitle("#DeltaR^{track}_{leading jet}"); naxis->CenterTitle(); naxis->SetTitleOffset(-1.3);
  naxis->SetLabelFont(63);
  naxis->SetLabelOffset(0.01); naxis->SetLabelSize(22);
  naxis->SetTitleFont(63);
  naxis->SetTitleSize(24);
  naxis->SetTitleOffset(2);
  aaxis->SetTitle("#DeltaR^{track}_{sub-leading jet}"); aaxis->CenterTitle(); aaxis->SetTitleOffset(-1.3);
  aaxis->SetLabelFont(63);
  aaxis->SetLabelOffset(0.01); aaxis->SetLabelSize(22);
  aaxis->SetTitleFont(63);
  aaxis->SetTitleSize(24);
  aaxis->SetTitleOffset(2);
  naxis->Draw();
  aaxis->Draw();
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
    leg->SetTextSize(0.05);
    /*
    leg->AddEntry(hc0,"0.5-1 GeV/c","f");
    leg->AddEntry(hc01,"1-2 GeV/c","f");
    leg->AddEntry(hc12,"2-4 GeV/c","f");
    leg->AddEntry(hc124,"4-8 GeV/c","f");
    leg->AddEntry(hc1248,"16+ GeV/c","f");
    */
    leg->AddEntry(hc0,"1.5-3 GeV/c","f");
    leg->AddEntry(hc01,"3-6 GeV/c","f");
    leg->AddEntry(hc12,"6-9 GeV/c","f");
    leg->AddEntry(hc124,"9-18 GeV/c","f");
    leg->AddEntry(hc1248,"18-36 GeV/c","f");
    leg->AddEntry(hcall,"36+ GeV/c","f");
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
       << awaysum << "(away-side)" << endl;
  cout << "integral of dET/dR (combined hist) = " << cnearsum << "(near-side) \t"
       << cawaysum << "(away-side)" << endl;
}
