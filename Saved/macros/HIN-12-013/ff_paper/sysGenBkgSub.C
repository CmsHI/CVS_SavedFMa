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

void sysGenBkgSub() {
  TH1::SetDefaultSumw2();
//   std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pfJet.root";
//   std::string InputSys_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pfJet.root";
  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00.root";
  std::string InputSys_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00.root";

  TH1D* ff[3][5]; // (leading, subleading),  (aj bin)
  TH1D* ffsys[3][5];
  
  TFile * outf = new TFile("systematics/closure_bkgsub_sys.root","update");
  
  for (int binMode=2; binMode<=2; ++binMode) {  // 1 : aj, 2 : cent
    TCanvas * c2 = new TCanvas(Form("c2_binMode%d",binMode),"c2",1600,800);
    c2->Divide(4,2);
    for ( int ijet=1; ijet<=2; ijet++){ 
      for ( int iaj=1;iaj<=4;iaj++) {
        TString jetName = "lJet";
        
        if ( ijet == 2 )  jetName = "slJet";
        if      ( binMode == 1 ) {
    ff[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure1000",jetName.Data(),iaj));
    ffsys[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0",jetName.Data(),iaj));
        }
        else if ( binMode == 2 ) {
    ff[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure101",jetName.Data(),iaj));
    ffsys[ijet][iaj]  = (TH1D*)load(InputSys_.data(),Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure102",jetName.Data(),iaj));
        } 
    
        handsomeTH1(ff[ijet][iaj],1);
        handsomeTH1(ffsys[ijet][iaj],1);
        
        ffsys[ijet][iaj]->Divide(ff[ijet][iaj]);
        c2->cd((ijet-1)*4+iaj);
        ffsys[ijet][iaj]->SetAxisRange(0,2,"Y");
        ffsys[ijet][iaj]->Draw();
  //       ff[ijet][iaj]->Draw("hist");
  //       ffsys[ijet][iaj]->Draw("sameE");
        outf->cd();
        ffsys[ijet][iaj]->Write();
      }
    }
  }
//   outf->Write();
}