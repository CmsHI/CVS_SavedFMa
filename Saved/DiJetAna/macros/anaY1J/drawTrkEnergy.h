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
using namespace std;


TH1D* combine(TH1D* near, TH1D* away) {

  Int_t nbin0=near->GetNbinsX();
  Int_t nbinc=nbin0*4;
  Int_t delta=nbin0*0.4;
  cout << "nbin0 " << nbin0 << " nbinc: " << nbinc << endl;

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

  hcombine->SetFillColor(near->GetFillColor());
  hcombine->SetStats(0);
  hcombine->SetMinimum(0.1);
  hcombine->SetMaximum(40);
  hcombine->SetTitle(";;#frac{1}{N_{dijet}}  #frac{d#sump_{T}^{track} }{ dR } [GeV]"); // no 2piR in denominator
  hcombine->GetYaxis()->CenterTitle();
  hcombine->GetYaxis()->SetTitleOffset(1.7);

  return hcombine;
}
