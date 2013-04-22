#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
// #include "../forest/hiForest.h"
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

#define PI 3.141592653589
static const int MAXTRK = 30000;

TH1D* smearHorizon(TH1D* h1=0,double theRatio=0, int trial=10000) {
  int seconds = time(NULL);
  TRandom3 rand(seconds%10000);
  
  TH1D* ret = (TH1D*)h1->Clone(Form("%s_sys%.0f",h1->GetName(),100*theRatio));
  ret->Reset();
  
  for ( int i=0;i<=trial;i++) {
    double oVal = h1->GetRandom();
//      double shiftVal = rand.Gaus(  oVal, theRatio*oVal ) ;
     double shiftVal = 1+theRatio;
     ret->Fill( oVal + log(shiftVal) );
  }
  ret->Scale( h1->Integral() / ret->Integral() );
  return ret;
}
void sysJEC() {
  TH1::SetDefaultSumw2();
  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pfJet.root";
  TFile* inf = new TFile(Input_.data());
  TH1D* ff[3][5]; // (leading, subleading),  (aj bin)
  TH1D* ffsys[3][5];
  
  TFile * outf = new TFile("systematics/jes_sys.root","recreate");
  
  for (int binMode=2; binMode<=2; ++binMode) {  // 1 : aj, 2 : cent
    TCanvas * c2 = new TCanvas("c2","c2",1600,800);
    c2->Divide(4,2);
    for ( int ijet=1; ijet<=2; ijet++){ 
      for ( int iaj=1;iaj<=4;iaj++) {
        TString jetName = "lJet";
        
        if ( ijet == 2 )  jetName = "slJet";
        if      ( binMode == 1 ) {
    ff[ijet][iaj]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_hidata_icent999_irj%d_fragMode2_closure100",jetName.Data(),iaj));
        }
        else if ( binMode == 2 ) {
    ff[ijet][iaj]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100",jetName.Data(),iaj));
        } 
    
        if (!ff[ijet][iaj]) cout << "bad pp" << endl;
        handsomeTH1(ff[ijet][iaj],1);
        
        ffsys[ijet][iaj] = smearHorizon(ff[ijet][iaj],0.07,100000);
        ffsys[ijet][iaj]->Divide(ff[ijet][iaj]);
        c2->cd((ijet-1)*4+iaj);
        ffsys[ijet][iaj]->SetAxisRange(0,2,"Y");
        ffsys[ijet][iaj]->Draw();
  //       ff[ijet][iaj]->Draw("hist");
  //       ffsys[ijet][iaj]->Draw("sameE");
      }
    }
  }
  outf->Write();
}

void testShift() {
  TH1::SetDefaultSumw2();
  std::string Input_="dijetFF_output_histograms_trackPtCut2.root";
  
  TFile * inf = new TFile(Input_.data());
  
  TH1D * h = (TH1D*)inf->Get("hpt_slJet_sigTrk_hidata_icent4_iaj999_fragMode2_closure0");
  
  TCanvas * c2 = new TCanvas("c2","",800,400);
  c2->Divide(2,1);
  c2->cd(1);
  h->SetAxisRange(0,4,"Y");
  h->Draw();
  TH1D *hsh = smearHorizon(h,0.02,100000);
  hsh->SetMarkerStyle(kOpenCircle);
  hsh->SetMarkerColor(kRed);
  TH1D *hsl = smearHorizon(h,-0.02,100000);
  hsl->SetMarkerStyle(kOpenCircle);
  hsl->SetMarkerColor(kBlue);
  hsh->Draw("sameE");
  hsl->Draw("sameE");
  c2->cd(2);
  TH1D * hrh = (TH1D*)hsh->Clone("hrh");
  hrh->Divide(h);
  hrh->SetAxisRange(0,2,"Y");
  hrh->Draw("E");
  TH1D * hrl = (TH1D*)hsl->Clone("hrl");
  hrl->Divide(h);
  hrl->Draw("sameE");
}

 
