#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TCut.h>


void handsomeTH1( TH1 *a=0, int col =1, float size=1, int markerstyle=20)
{
  a->SetMarkerColor(col);
  a->SetMarkerSize(size);
  a->SetMarkerStyle(markerstyle);
  a->SetLineColor(col);
  a->GetYaxis()->SetTitleOffset(1.25);
  a->GetXaxis()->CenterTitle();
  a->GetYaxis()->CenterTitle();
}

void getNiceBins( TH1* h=0, int nDiv=2) {
  int nBins = h->GetNbinsX();
  double allInt = h->Integral(1,nBins);

  cout<< " All integral = " << allInt<< endl;

  TH1F* hacc = (TH1F*)h->Clone(Form("%s_accu",h->GetName()));
  hacc->Reset();
  double acc =0;

  for ( int i=1 ; i<= nBins ; i++ ) {
    acc = acc + h->GetBinContent(i);
    hacc->SetBinContent( i , acc ) ;
  }
  TCanvas* c1 = new TCanvas(Form("%s_c1Acc",h->GetName()),"",400,400);
  handsomeTH1(hacc,1);
  handsomeTH1(h,2);
  hacc->DrawCopy();
  h->DrawCopy("same");
  double allInt4 = h->Integral(1,h->FindBin(3.999));
  cout<< " pT sum under 4GeV tracks = " << allInt4<< endl;
}




void getpTBins(
	       TString inFileName="jfhCorrEtaPtBin200RBin20v1_HCPR_J50U_djcalo_Cent0to30_Aj22to100_SubEtaRefl.root") {
  
  TFile *f = new TFile(inFileName);
  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPADR"); // A
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPADRBg");  // A
  TH2D * hPtPNDRBgSub = (TH2D*)hPtPNDR->Clone("hPtPADRBgSub");  //A
  hPtPNDRBgSub->Add(hPtPNDR,hPtPNDRBg,1,-1);  
  TH1D * hPt1 = (TH1D*)hPtPNDRBgSub->ProjectionX("hPt1",1,2);
  cout << " only dr < 0.078" << endl; 
  getNiceBins(hPt1,4);

  TH1D * hPt = (TH1D*)hPtPNDRBgSub->ProjectionX("hPt");
  cout << " all dr" << endl;
  getNiceBins(hPt,4);
  


}



