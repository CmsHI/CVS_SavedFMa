#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "CutAndBinCollection2011.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

#define PI 3.141592653589
static const int MAXTRK = 30000;


void drawDijetEtaReflectionComparison() {
  TH1::SetDefaultSumw2();
  std::string Input_="dijetFF_output_histograms_trackPtCut2.root";
  
  TString methodText="";
  TString methodText_="";
  
  // method = "eventMixing";
  TFile* inf = new TFile(Input_.data());
  TH1D* ffhi[3][5][3]; // (leading, subleading),  (aj bin). (method bin)
  TH1D* ffratio[3][5];

  int binMode = 2 ;
  for ( int ijet=1; ijet<=2; ijet++){ 
    for ( int iaj=1;iaj<=4;iaj++) {
      for ( int im=1; im<=2; im++){
	
	TString jetName = "lJet";
	if ( ijet == 2 )  jetName = "slJet";
	
	int method = 0 ; 
	if ( im == 2 ) method = 100;
	
	if      ( binMode == 1 ) {
	  ffhi[ijet][iaj][im]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure%d",jetName.Data(),iaj,method));
	}
	else if ( binMode == 2 ) {
	  ffhi[ijet][iaj][im]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_hidata_icent%d_iaj999_fragMode2_closure%d",jetName.Data(),iaj,method));
	} 
      }
      ffratio[ijet][iaj] = (TH1D*)ffhi[ijet][iaj][2]->Clone(Form("%s_ratio",ffhi[ijet][iaj][2]->GetName()));
      ffratio[ijet][iaj]->Divide(ffhi[ijet][iaj][1]);
    }
  }
  TCanvas* c1;
  TLegend* l2;
  
  c1 = new TCanvas("c1","",1500,750);
  c1->Divide(4,2); 
  for ( int iaj=1 ; iaj<=4 ; iaj++) {
    c1->cd(iaj);
    ffratio[1][iaj]->Draw();
    c1->cd(iaj+4);
    ffratio[2][iaj]->Draw();
  }
  TFile outf = TFile("dijetFF_systematics_by_two_method_comparison.root","update");
  for ( int ijet=1; ijet<=2; ijet++){
    for ( int iaj=1;iaj<=4;iaj++) {
      ffratio[ijet][iaj]->Write();
    }
  } 
  outf.Close();
  
  
  
}

