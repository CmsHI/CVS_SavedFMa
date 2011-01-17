#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
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

TH1D * projectDR(TH2D * h2d,Int_t iBeg,Int_t iEnd)
{
  TH1D * hDr = (TH1D*)h2d->ProjectionY(Form("%s_%d_%d",h2d->GetName(),iBeg,iEnd),iBeg,iEnd);
  //hDr->Sumw2();
  //cout << "Project " << h2d->GetName() << ": " << iBeg << " - " << iEnd << endl;
  return hDr;
}

void subtractError(TH1* h1, TH1* h2) {
   int nBins = h1->GetNbinsX();
   for ( int i = 1 ; i<=nBins ; i++ ) {
      float oldErr = h1->GetBinError(i);
      float Err2   = h2->GetBinError(i);
      float newErr = sqrt ( oldErr*oldErr - Err2*Err2) ;
      h1->SetBinError(i,newErr);
   }
}


TH2D * bkgSub(TH2D * hSig, TH2D * hBkg, TString name)
{
  //hSig->Sumw2();
  //hBkg->Sumw2();
  TH2D * hSub = (TH2D*)hSig->Clone(name+"Sub");
  hSub->Add(hSig,hBkg,1,-1);
  return hSub;
}

void getDRHists(TFile * f,
    Int_t nbin,
    Int_t * begbins,
    Int_t * endbins,
    TH1D ** Nr,
    TH1D ** Aw,
    TH1D * hPt
    )
{
  // Input
  TString inFileNameStrip(f->GetName());
  inFileNameStrip.ReplaceAll(".root","");

  // 2D histograms
  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");
  // Subtract Bkg
  TH2D * hPtPNDRSub = bkgSub(hPtPNDR,hPtPNDRBg,inFileNameStrip+"Nr");
  TH2D * hPtPADRSub = bkgSub(hPtPADR,hPtPADRBg,inFileNameStrip+"Aw");

  TH1D* NrE[100] ;
  TH1D* AwE[100] ;

  for (Int_t i=0; i<nbin; ++i) {
    Nr[i] = projectDR(hPtPNDRSub,begbins[0],endbins[i]);
    Aw[i] = projectDR(hPtPADRSub,begbins[0],endbins[i]);
    if ( i>0) {
       NrE[i]= projectDR(hPtPNDRSub,begbins[i],endbins[i]);
       AwE[i]= projectDR(hPtPADRSub,begbins[i],endbins[i]);
       
       int jBins = Nr[i]->GetNbinsX();
       for ( int j = 1 ; j<= jBins ; j++ ) { 
	  Nr[i]->SetBinError(j,   NrE[i]->GetBinError(j));
	  Aw[i]->SetBinError(j,   AwE[i]->GetBinError(j));
       }
    }
    // Print
    //cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(begbins[0]),hPt->GetBinLowEdge(endbins[i]+1))
    //  << " SigSubBkg Integral - Nr: " << Nr[i]->Integral() << " Aw: " << Aw[i]->Integral() << endl;
  }

  // ==================================================
  // xcheck w/ evt by evt histograms
  // ==================================================
  TTree * ntjt = (TTree*)f->Get("ntjt");
  ntjt->AddFriend("tcone");
  // Get Evt-by-Evt histograms
  //TH1D * hNrCPtBgSub = (TH1D*)f->Get("hNrCPtBgSub");
  //TH1D * hAwCPtBgSub = (TH1D*)f->Get("hAwCPtBgSub");
  TH1D * hNrCPtBgSub = new TH1D(inFileNameStrip+"hCPt0BgSubMerge","",10000,-1000,1000);
  TH1D * hAwCPtBgSub = new TH1D(inFileNameStrip+"hCPt1BgSubMerge","",10000,-1000,1000);
  ntjt->SetAlias("cpt0bgsubMerge","(Sum$(cpt[0])-cpt[0][0])-(Sum$(cptbg[0])-cptbg[0][0])");
  ntjt->SetAlias("cpt1bgsubMerge","(Sum$(cpt[1])-cpt[1][0])-(Sum$(cptbg[1])-cptbg[1][0])");
  ntjt->Project(inFileNameStrip+"hCPt0BgSubMerge","(cpt0bgsubMerge)","weight*(abs(nljetacorr)>0.8)");
  ntjt->Project(inFileNameStrip+"hCPt1BgSubMerge","(cpt1bgsubMerge)","weight*(abs(aljetacorr)>0.8)");
  Float_t meanNr = hNrCPtBgSub->GetMean();
  Float_t meanAw = hAwCPtBgSub->GetMean();

  // Get Mean Centrality Weight
  TH1D * hWtNr = new TH1D("hWtNr","hWt",10000,0,100);
  TH1D * hWtAw = new TH1D("hWtAw","hWt",10000,0,100);
  ntjt->Project("hWtNr","weight","abs(nljetacorr)>0.8");
  ntjt->Project("hWtAw","weight","abs(aljetacorr)>0.8");
  Float_t wtNr=hWtNr->GetMean();
  Float_t wtAw=hWtAw->GetMean();
  delete hWtNr; delete hWtAw;
  cout << "Evt by evt SigSubConePt - <Weight>: " << wtNr << "," << wtAw << " Nr: " << meanNr << " Aw: " << meanAw << endl;
}

void jumSun(double x1=0,double y1=0,double x2=1,double y2=1,int color=1, double width=1)
{
   TLine* t1 = new TLine(x1,y1,x2,y2);
   t1->SetLineWidth(width);
   t1->SetLineStyle(7);
   t1->SetLineColor(color);
   t1->Draw();
}

void getTotalNum(TH1D* h, Float_t sysErrorFrac=0.2) {
  Float_t maxYValue = h->GetBinContent(h->GetMaximumBin());
  Float_t sysError = maxYValue*sysErrorFrac;
  TH1D* hSim = (TH1D*)h->Clone(Form("%s_oneBin",h->GetName()));
  hSim->Rebin(hSim->GetNbinsX());
  //   cout << " bin numbers = " << hSim->GetNbinsX() << endl;
  cout.precision(4);
  cout << hSim->GetBinContent(1) << " (" << hSim->GetBinError(1) << ", " << sysError << ")";
  delete hSim;
}
TH1D* combine(TH1D* near, TH1D* away, Int_t normType=0, Float_t norm=1., bool Left=true,
	      Int_t sysErrorType=0) {
  // Get Bin Info
  Int_t nbin0=near->GetNbinsX();
  Int_t nbinc=nbin0*2;
  Int_t delta=0;

  // Combine Near and Away
  Float_t xmax = near->GetBinLowEdge(nbin0+1);
  TH1D* hcombine = new TH1D(Form("hcombine_%s_%s_%d",near->GetName(),away->GetName(),Left),"",nbinc,-xmax, xmax) ;

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
  //hcombine->SetMinimum(-10);
  if (normType==0) {
    //hcombine->SetMaximum(59.9);
    //    hcombine->SetTitle(";;#frac{1}{N_{dijet}} #frac{d#scale[0.9]{#sum}p_{T}^{track} }{ dR } (GeV/c)"); // no 2piR in denominator
    hcombine->SetTitle(";;#Sigmap_{T} per bin (GeV/c)");
  }
  if (normType==1) {
    //hcombine->SetMaximum(7.8);
    hcombine->SetTitle(";;#frac{1}{N_{dijet} #scale[0.9]{#sum_{p_{T}>1.5GeV #in Jet1}}p_{T}^{track}} #frac{d#scale[0.9]{#sum}p_{T}^{track} }{ dR }"); // no 2piR in denominator
  }
  hcombine->GetYaxis()->SetTitleFont(63);
  hcombine->GetYaxis()->SetTitleSize(20);
  hcombine->GetYaxis()->CenterTitle();
  hcombine->GetYaxis()->SetTitleOffset(2.8);
  hcombine->GetYaxis()->SetLabelFont(63);
  hcombine->GetYaxis()->SetLabelSize(22);

  // Additional Systematic Error ---------------------------------
  if (sysErrorType==1) AdditionalSysUncert(hcombine,0.2);

  return hcombine;
}

//------------------------------------------------------
void drawTrkEnergy(TString infile="drawn_jfh_HCPR_J50U_Cent0to10_Aj24to100_SubEtaRefl.root",

		   bool drawLeg=false, bool drawYLab=false, Int_t logScale=0, Int_t normType=0,
		   Int_t sysErrorType=0)
{
   gStyle->SetMarkerStyle(0);

  // =========================================================================
  // Basic Plot Parameters
  // =========================================================================
  Float_t ymin=-5,ymax=69.9;
  if (logScale==1) ymin=0.3;
  //  double colors[nBin] = {kBlue+1,kGreen-8, kYellow-7,kOrange-4,kRed-3};
  Int_t colors[3] = {kGreen-8,kOrange-4,kRed-3};
  
  // =========================================================================
  // Inputs
  // =========================================================================
  cout << endl << "=== " << infile << " ===" << endl;
  TFile *f = new TFile(infile);
  // Get Pt info
  TH2D *hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  if (0) {
    cout << "Before Combine: " << hPt->GetNbinsX() << " pt bins" << endl;
    for (Int_t i=1; i<=hPt->GetNbinsX(); ++i) {
      cout << "Pt bin " << i << ": " << hPt->GetBinLowEdge(i) << " - " << hPt->GetBinLowEdge(i+1) << endl;
    }
  }

  const Int_t nbin=3;
  // v10+:  bin2+3  = 1-4GeV
  //	    bin4    = 4-8 GeV
  //	    bin5+6  = 8+ GeV
  Int_t begbins[nbin] = {2,4,5};
  Int_t endbins[nbin] = {3,4,hPt->GetNbinsX()};

  // =========================================================================
  // Project Accumulation Histograms
  // =========================================================================
  TH1D *Nr[nbin];
  TH1D *Aw[nbin];
  getDRHists(f,nbin,begbins,endbins,Nr,Aw,hPt);
  for (Int_t i=0; i<nbin; ++i) {
    Nr[i]->SetFillColor(colors[i]);
    Aw[i]->SetFillColor(colors[i]);
  }
 
  // =========================================================================
  // Combine Histgrams
  // =========================================================================
  //TCanvas *c2 = new TCanvas("c2","c2",600,500);
  Float_t norm = 1./(Nr[nbin-1]->Integral()*Nr[nbin-1]->GetBinWidth(1));
  TH1D* hcLeft[nbin];
  TH1D* hcRight[nbin];
  int fillLeft = 3013;
  for (Int_t i=0; i<nbin; ++i) {
    hcLeft[i] = combine(Nr[i],Aw[i],normType,norm,true,sysErrorType);
    hcLeft[i]->SetFillStyle(fillLeft);
    //if (i!=0) hcLeft[i]->SetLineColor(colors[i]);
    hcRight[i] = combine(Nr[i],Aw[i],normType,norm,false,sysErrorType);
  }

  // =========================================================================
  // Plot Axis
  // =========================================================================
  hcLeft[nbin-1]->GetXaxis()->SetNdivisions(000,true);
  //if(!drawYLab) hcLeft[nbin-1]->GetYaxis()->SetTitle("");
  float shftAxis= hcLeft[nbin-1]->GetBinWidth(1)/2.*gab;
  float drRange = 0.8;
  hcLeft[nbin-1]->SetAxisRange(-0.85,0.85,"X"); //TMath::Pi()/2 - drRange-shftAxis, TMath::Pi()/2 + drRange-shftAxis);
  hcLeft[nbin-1]->SetAxisRange(ymin,ymax,"Y"); //TMath::Pi()/2 - drRange-shftAxis, TMath::Pi()/2 + drRange-shftAxis); 
  fixedFontHist(hcLeft[nbin-1]);
  
  //correct stat error
  //  subtractError(hcLeft[2],hcLeft[1]);
  //  subtractError(hcLeft[1],hcLeft[0]);
  //  subtractError(hcRight[2],hcRight[1]);
  //  subtractError(hcRight[1],hcRight[0]);

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
     TLegend *leg = new TLegend(0.1201672,0.6548998,0.5002153,0.9644227,NULL,"brNDC");
     leg->SetFillStyle(0);
     leg->SetBorderSize(0);
     leg->SetNColumns(1);
     leg->SetTextSize(0.05);
     TLegend *legLeft = new TLegend(0.04637139,0.6548998,0.5075949,0.9644227,NULL,"brNDC");
     legLeft->SetFillStyle(0);
     legLeft->SetBorderSize(0);
     legLeft->SetNColumns(1);
     legLeft->SetTextSize(0.05);
     for (Int_t i=nbin-1;i>=0;--i) {
       if (i==nbin-1) leg->AddEntry(hcRight[i],Form("> %.0f GeV/c",hPt->GetBinLowEdge(begbins[i])),"f");
       else leg->AddEntry(hcRight[i],Form("%.0f-%.0f GeV/c",hPt->GetBinLowEdge(begbins[i]),hPt->GetBinLowEdge(endbins[i]+1)),"f");
       if (i==nbin-1) legLeft->AddEntry(hcLeft[i],"","f");
       else legLeft->AddEntry(hcLeft[i],"","f");
     }
     legLeft->Draw();
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
