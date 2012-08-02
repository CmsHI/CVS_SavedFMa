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

TH1D* smearHorizon(TH1D* h1=0,double theRatio=0, int trial=10000);
void drawDijetResultDiff(int binMode=1, int method=100);
void drawDijetResult() {
  //  drawDijetResultDiff(1, 0);
  //  drawDijetResultDiff(1, 100);
  //  drawDijetResultDiff(2, 0);
  drawDijetResultDiff(2, 100);
}


void drawDijetResultDiff(int binMode, int method) {  // 1 : aj, 2 : cent
  TH1::SetDefaultSumw2();
  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1.root";
  
  TString methodText="";
  TString methodText_="";
  if ( method == 100 ) {
    methodText = "eta refl. method (|#eta|>0.3)";
    methodText_= "etaRef";
  }
  else if ( method == 0 ) {
    methodText = "Event mixing method";
    methodText_="evtMixing";
  }

  // method = "eventMixing";
  TFile* inf = new TFile(Input_.data());
  TH1D* ffhi[3][5]; // (leading, subleading),  (aj bin)
  TH1D* ffpp[3][5];
  TH1D* ffppNoWeight[3][5];
  TH1D* ffratio[3][5];
  
  for ( int ijet=1; ijet<=2; ijet++){ 
    for ( int irj=1;irj<=4;irj++) {
      TString jetName = "lJet";
      

      if ( ijet == 2 )  jetName = "slJet";
      if      ( binMode == 1 ) {
	ffhi[ijet][irj]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_hidata_icent999_irj%d_fragMode2_closure%d",jetName.Data(),irj,method));
	ffpp[ijet][irj]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_ppdata_icent999_irj%d_fragMode2_closure%d",jetName.Data(),irj,method));
	
	ffppNoWeight[ijet][irj]= (TH1D*)inf->Get( Form("hpt_%s_sigTrk_ppdata_icent999_irj%d_fragMode2_closure-1",jetName.Data(),irj));
   
      }
      else if ( binMode == 2 ) {
	ffhi[ijet][irj]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure%d",jetName.Data(),irj,method));
        ffpp[ijet][irj]  = (TH1D*)inf->Get( Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure%d",jetName.Data(),irj,method));
       	ffppNoWeight[ijet][irj]= (TH1D*)inf->Get( Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure-1",jetName.Data(),irj));
	cout << " name = " << Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure%d",jetName.Data(),irj,method) << endl;
	cout << " name = " << Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure0%d",jetName.Data(),irj,method) << endl;
	
      } 
      
      handsomeTH1(ffpp[ijet][irj],1);
      handsomeTH1(ffppNoWeight[ijet][irj],1);
      handsomeTH1(ffhi[ijet][irj],2);
    }
  }
  
  TCanvas* c1[3];
  TLegend* l2[3];

  for ( int ijet=1; ijet<=2 ; ijet++) {
    c1[ijet] = new TCanvas(Form("c1_ijet%d",ijet),"",1500,750);
    c1[ijet]->Divide(4,2); 
    l2[ijet]  = new TLegend(0.2628166,0.1516459,0.9501782,0.4293076,NULL,"brNDC");
    /// inclusive jets
    //    if      ( ijet == 1 )     easyLeg(l2[ijet],"Leading jet");
    if      ( ijet == 1 )     easyLeg(l2[ijet],"Incl. jets, p_{T}>100GeV/c");
    else if ( ijet == 2 )     easyLeg(l2[ijet],"Subleading jet");
    l2[ijet]->AddEntry(ffhi[ijet][1],"PbPb","pl");
    l2[ijet]->AddEntry(ffpp[ijet][1],"pp (Reweight)","l");
    l2[ijet]->AddEntry(ffppNoWeight[ijet][1],"pp (Original)","l");
    
    for ( int irj=1 ; irj<=4 ; irj++) {
      c1[ijet]->cd(irj);
      if ( binMode == 2 )  c1[ijet]->cd(5-irj);
      
      bool doNorm=false;
      if (doNorm) {scaleInt(ffppNoWeight[ijet][irj]);  scaleInt(ffpp[ijet][irj]); scaleInt(ffhi[ijet][irj]);}
      ffppNoWeight[ijet][irj]->SetLineStyle(2);
      ffppNoWeight[ijet][irj]->SetAxisRange(0,5.8,"X");
      ffppNoWeight[ijet][irj]->SetAxisRange(0.005,20,"Y");

      //      ffhi[ijet][irj] = smearHorizon(ffhi[ijet][irj],1,100000);

      if (doNorm)       ffppNoWeight[ijet][irj]->SetAxisRange(0.00025,1,"Y");
      ffppNoWeight[ijet][irj]->Draw("hist");
      ffpp[ijet][irj]->Draw(   "same hist");
      ffhi[ijet][irj]->Draw("same");
      if (irj==1) 	l2[ijet]->Draw();
      if (doNorm)  {if (irj==2)       drawText("Normalized by area",0.25,0.2,1);}
      if ( binMode == 1 ) drawText(Form("%.2f < A_{j} < %.2f", float(rjBin1[irj-1]), float(rjBin1[irj])), 0.3,0.46);
      else if ( binMode == 2 ) drawText(Form("%.0f %% - %.0f %%", float(centBin1[irj-1]*2.5), float(centBin1[irj]*2.5)), 0.3,0.46);
 
      gPad->SetLogy();
      
      c1[ijet]->cd(irj+4);
      if ( binMode == 2)   c1[ijet]->cd(9-irj);
      
      ffratio[ijet][irj]= (TH1D*)ffhi[ijet][irj]->Clone(Form("ffRattio_ijet%d_irj%d",ijet,irj));
      ffratio[ijet][irj]->Divide(ffpp[ijet][irj]);
      ffratio[ijet][irj]->SetAxisRange(0,3,"Y");
      ffratio[ijet][irj]->SetAxisRange(0,5.8,"X");
      ffratio[ijet][irj]->SetYTitle("PbPb/pp");
      ffratio[ijet][irj]->Draw();
      jumSun(0,1,6,1);
      drawText("PbPb/pp",0.25,0.2,1);
    }
    c1[ijet]->SaveAs(Form("plotsOfDijetFF/FF_distribution_%s_ijet%d_binMode%d.gif",methodText_.Data(),ijet,binMode));
  }

  TLegend* l3  = new TLegend(0.2228166,0.766459,0.9501782,0.9393076,NULL,"brNDC");
  TCanvas* c2 = new TCanvas("c2_ijet","",1500,400);
  makeMultiPanelCanvas(c2,4,1,0.0,0.0,0.2,0.15,0.02);
  for ( int irj=1 ; irj<=4 ; irj++) {
    c2->cd(irj);
    if ( binMode == 2)   c2->cd(5-irj);

    ffratio[1][irj]->SetMarkerStyle(20);
    ffratio[1][irj]->Draw();
    //    ffratio[2][irj]->Draw("same");
    if ( binMode == 1 ) {
      if ( irj ==1 )  drawText(Form("%.2f < pt_{2}/pt_{1} < %.2f", float(rjBin1[irj-1]), float(rjBin1[irj])), 0.3,0.6);
      else           drawText(Form("%.2f < pt_{2}/pt_{1}  < %.2f", float(rjBin1[irj-1]), float(rjBin1[irj])), 0.1,0.6);
    }
    else if ( binMode ==2) {
      if ( irj ==4 )  drawText(Form("%d %% - %d %%", int(centBin1[irj-1]*2.5), int(centBin1[irj]*2.5)), 0.3,0.6);
      else            drawText(Form("%d %% - %d %%", int(centBin1[irj-1]*2.5), int(centBin1[irj]*2.5)), 0.1,0.6);
    }
    if ( ( (binMode==2) && (irj ==4) ) || ( (binMode==1) && (irj ==1) ) )   {
      easyLeg(l3,"PbPb/pp of Fragmentation Function");
      //inclusive      l3->AddEntry(ffratio[1][irj],"Leading jet");
      //      l3->AddEntry(ffratio[2][irj],"Subleading jet");
      l3->AddEntry(ffratio[1][irj],"Inclusive jets");
      l3->AddEntry("","p_{T} > 100GeV/c, |#eta|<2","");
      l3->Draw();
    }
    if ( ( (binMode==2) && (irj ==3) ) || ( (binMode==1) && (irj ==2) ) ) {
      drawText(methodText, 0.1,0.9,4,20);
    }
 jumSun(0,1,6,1);
    
  }
  c2->SaveAs(Form("plotsOfDijetFF/FF_ratio_%s_binMode%d.gif",methodText_.Data(),binMode));
  c2->SaveAs(Form("plotsOfDijetFF/FF_ratio_%s_binMode%d.C",methodText_.Data(),binMode));
  
  
  
}

 
TH1D* smearHorizon(TH1D* h1,double theRatio, int trial)   {
  int seconds = time(NULL);
  TRandom3 rand(seconds%10000);
  
  TH1D* ret = (TH1D*)h1->Clone(Form("%s_smearby10p",h1->GetName()));
  ret->Reset();
  
  for ( int i=0;i<=trial;i++) {
    double oVal = h1->GetRandom();
     double shiftVal = rand.Gaus(  oVal, theRatio*oVal ) ;
     ret->Fill( oVal + log(shiftVal) );
  }
  ret->Scale( h1->Integral() / ret->Integral() );
  return ret;
}

