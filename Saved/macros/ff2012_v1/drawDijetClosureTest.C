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

const int kRAW = 1;
const int kBKG =2;
const int kSIG =3;


void drawDijetClosureTestSingle(int mode=1);
void drawDijetClosureTest() {
  //  drawDijetClosureTestSingle(1);
  drawDijetClosureTestSingle(1);

}
void drawDijetClosureTestSingle(int mode) { // 1 = centrality bins,  2 = irj bins
  
  bool usingPara = false;
  TString attPara = ( (usingPara == true) ? "Para" : "" ) ;
  int nClos1 = 101;  // denomination    1 == The most gen level one
  int nClos2 = 100;  // numerator
  int fragMode=2;
  TH1::SetDefaultSumw2();
  
  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1.root";
  //  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut2_genJetUsed.root";
  // std::string Input_="dijetFF_output_histograms_trackPtCut2.root";
  
  TFile* inf = new TFile(Input_.data());
  TH1D* genAj[3][4][5];
  TH1D* recAj[3][4][5];
  for ( int irj=1;irj<=4;irj++) {
    if ( mode == 1) {
    genAj[1][kRAW][irj] = (TH1D*)inf->Get( Form("hpt_lJet_rawTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
    cout <<Form("hpt_lJet_rawTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()) << endl;
      recAj[1][kRAW][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_rawTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[1][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_mbTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[1][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_mbTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[1][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_sigTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[1][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_sigTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[2][kRAW][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_rawTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[2][kRAW][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_rawTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[2][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_mbTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[2][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_mbTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[2][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_sigTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[2][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_sigTrk_himc_icent%d_irj999_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
    }
    else if ( mode == 2) {
      genAj[1][kRAW][irj] = (TH1D*)inf->Get( Form("hpt_lJet_rawTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[1][kRAW][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_rawTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[1][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_mbTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[1][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_mbTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[1][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_sigTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[1][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_lJet_sigTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[2][kRAW][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_rawTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[2][kRAW][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_rawTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[2][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_mbTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[2][kBKG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_mbTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
      genAj[2][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_sigTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos1,attPara.Data()));
      recAj[2][kSIG][irj] =  (TH1D*)inf->Get(Form("hpt_slJet_sigTrk_himc_icent999_irj%d_fragMode%d_closure%d%s",irj, fragMode, nClos2, attPara.Data()));
    }
  }
  
  for ( int irj=1;irj<=4;irj++) {
    for ( int j=1;  j<=2;  j++) {
      for ( int k=1;  k<=3;  k++) {
	handsomeTH1(genAj[j][k][irj],1);
	handsomeTH1(recAj[j][k][irj],2);
      }
    }
  }
  TCanvas* c1[3][4];
  TLegend* l2[3][3];
  
  for ( int ijet=1; ijet<=2 ; ijet++) {
    for ( int ii=1; ii<=3; ii++) {
      c1[ijet][ii] = new TCanvas(Form("c1_%d_%d",ijet,ii),"",1200,600);
      makeMultiPanelCanvas(c1[ijet][ii],4,2,0.0,0.0,0.2,0.15,0.02);
      
      for ( int irj=1 ; irj<=4 ; irj++) {
	c1[ijet][ii]->cd(irj);
	genAj[ijet][ii][irj]->SetAxisRange(5e-4,50,"Y");
	genAj[ijet][ii][irj]->SetAxisRange(0.1,5.4,"X");
	genAj[ijet][ii][irj]->Draw("hist");
	recAj[ijet][ii][irj]->DrawCopy("same");
	gPad->SetLogy();
	
	if (irj==1) {
	  l2[ijet][ii] = new TLegend(0.3928166,0.1916459,0.7801782,0.3593076,NULL,"brNDC");
	  /// inclusive
	  //	  if      ( ijet == 1 )     drawText("Lead. jet", 0.2,0.86,1);
	  if      ( ijet == 1 )     drawText("p_{T}^{jet} > 100GeV/c", 0.3,0.86,1);
	  else if ( ijet == 2 )     drawText("Sublead. jet", 0.3,0.86,1);
	  
	  if      ( ii==1) easyLeg(l2[ijet][ii],"raw tracks");
	  else if ( ii==2) easyLeg(l2[ijet][ii],"background tracks");
	  else if ( ii==3) easyLeg(l2[ijet][ii],"signal tracks");
	  //	  l2[ijet][ii]->AddEntry(genAj[ijet][ii][irj],"Gen Particle","pl");
	  //	  l2[ijet][ii]->AddEntry(recAj[ijet][ii][irj],"Reco. tracks (corr)","l");
	  l2[ijet][ii]->Draw();
	}
	int lowCent = centBin1[irj-1];
	int highCent = centBin1[irj]-1;
	if (mode == 1 ) 
	  drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.40,0.46);
	else if ( mode == 2 )
	  drawText(Form("%.2f < A_{j} < %.2f", float(rjBin1[irj-1]), float(rjBin1[irj])), 0.25,0.46);

	c1[ijet][ii]->cd(4+irj);
	recAj[ijet][ii][irj]->Divide(genAj[ijet][ii][irj]);
	recAj[ijet][ii][irj]->SetAxisRange(0,2,"Y");
	recAj[ijet][ii][irj]->SetAxisRange(0.1,5.4,"X");
	recAj[ijet][ii][irj]->SetYTitle("Ratio Reco/Gen");
	recAj[ijet][ii][irj]->Draw();
	jumSun(0,1,7,1);
      }
      
      if      ( ii==1) c1[ijet][ii]->SaveAs(Form("closureTest_rawTrack_leading%d.gif",ijet));
      else if ( ii==2) c1[ijet][ii]->SaveAs(Form("closureTest_bkgTrack_leading%d.gif",ijet));
      else if ( ii==3) c1[ijet][ii]->SaveAs(Form("closureTest_sigTrack_leading%d.gif",ijet));
      
    }
  }

  TFile outf = TFile(Form("closureTestRatio_between_%d_%d.root",nClos1,nClos2),"update");
  for ( int ijet=1; ijet<=2 ; ijet++) {
    for ( int irj=1; irj<=4 ; irj++) {
      recAj[ijet][kSIG][irj]->Write();
    }
  }
  outf.Close();
  
  
}
