#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGraphErrors.h"
#include "commonUtility.h"
#include "compare.h"
using namespace std;
const float PI = 3.1415926535857932;

void Compare_DataMC_pPb_Approval()
{
  TH1::SetDefaultSumw2();

  // inputs
  TString outdir = "fig/2012.10.13";
  TString infname = outdir+"/comparisons.root";
  TFile * inf = new TFile(infname);
  
  // histograms
  TH1D * vhMul[4]; // pPb, ampt, hijing, pp

  vhMul[0] = (TH1D*)load(infname.Data(),Form("cmp_1_0_0"));
  vhMul[1] = (TH1D*)load(infname.Data(),Form("cmp_1_0_1"));
  vhMul[2] = (TH1D*)load(infname.Data(),Form("cmp_1_0_2"));
//   TGraphErrors * gMulPp = new TGraphErrors("pp_mul_mb.txt","%lg %lg %lg");
  vhMul[3] = (TH1D*)load("pp_mul_mb.root","hmulpp");


  // 1. Multiplicity of pPb  
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetLogy();
  handsomeTH1(vhMul[0],kBlack,1.2,kFullCircle);
  vhMul[0]->SetAxisRange(2e1,9e5,"Y");
  vhMul[0]->SetTitle(";           N_{Trk}^{offline} (p_{T}>0.4GeV/c, |#eta|<2.4);Events;");
  vhMul[0]->DrawClone();
  drawText("CMS Preliminary",0.62,0.88,kBlack,20);
  drawText("#sqrt{s_{NN}}=5.02 TeV ",0.64,0.82,kBlack,20);
	TLegend * leg1 = new TLegend(0.25,0.37,0.63,0.50,NULL,"brNDC");
	easyLeg(leg1,"",25);
  leg1->AddEntry(vhMul[0],"pPb Data","p");
  leg1->Draw();
  c2->Print("fig/PPbMul.gif");
  c2->Print("fig/PPbMul.pdf");

  // 2. Compare normalized Multiplicity distributions
  // Normalize
  for (int i=0; i<4; ++i) {
    vhMul[i]->Scale(1./vhMul[i]->Integral()/vhMul[i]->GetBinWidth(1));
    vhMul[i]->SetAxisRange(1e-7,3e-1,"Y");
    vhMul[i]->SetTitle(";           N_{Trk}^{offline} (p_{T}>0.4GeV/c, |#eta|<2.4);Unity Normalization;");
  }

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  c3->SetLogy();
  handsomeTH1(vhMul[1],kRed,1.2,kOpenCircle);
  handsomeTH1(vhMul[2],kBlue,1.2,kOpenCircle);
  handsomeTH1(vhMul[3],kBlack,1.2,kOpenCircle);
  vhMul[0]->Draw("E");
  vhMul[3]->Draw("sameE");

	TLegend * leg4 = new TLegend(0.17,0.18,0.55,0.48,NULL,"brNDC");
	easyLeg(leg4,"",25);
  leg4->AddEntry(vhMul[0],"pPb Data","p");
  leg4->AddEntry(vhMul[3],"pp Data (7 TeV)","p");
  leg4->Draw();

  drawText("CMS Preliminary",0.62,0.88,kBlack,20);
  drawText("#sqrt{s_{NN}}=5.02 TeV ",0.64,0.82,kBlack,20);

  c3->Print("fig/CompareMulData.gif");
  c3->Print("fig/CompareMulData.pdf");

  leg4->AddEntry(vhMul[1],"AMPT","p");
  leg4->AddEntry(vhMul[2],"HIJING","p");
  vhMul[1]->Draw("sameE");
  vhMul[2]->Draw("sameE");
  leg4->Draw();


  c3->Print("fig/CompareMul.gif");
  c3->Print("fig/CompareMul.pdf");
  
  // 2. Compare normalized Track Distributions
  int color[4] = {kBlack,kRed,kBlue,kBlack};
  int style[4] = {kFullCircle,kOpenCircle,kOpenCircle,kOpenCircle};
  TH1D * vhTrkQual[10][4][3]; // var, mul bin, sample
  for (int i=0; i<3; ++i) {
    for (int j=0; j<4; ++j) {
      for (int k=0; k<2; ++k) {
        vhTrkQual[i][j][k] = (TH1D*)load("fig/2012.10.13/comparisons_trk100k.root",Form("cmp_%d_%d_%d",i+5,j,k));
        vhTrkQual[i][j][k]->Scale(1./vhTrkQual[i][j][k]->Integral());
        handsomeTH1(vhTrkQual[i][j][k],color[k],1.2,style[k]);
        vhTrkQual[i][j][k]->SetAxisRange(5e-4,8e-1,"Y");
        fixedFontHist(vhTrkQual[i][j][k],2.2,2.5,28);
        if (i==0) {
          vhTrkQual[i][j][k]->SetTitle(";d_{z}/#sigma(d_{z});Track Fraction");
          vhTrkQual[i][j][k]->SetAxisRange(-2.99,2.99,"X");
        }
        if (i==1) {
          vhTrkQual[i][j][k]->SetTitle(";d_{xy}/#sigma(d_{xy});Track Fraction");
          vhTrkQual[i][j][k]->SetAxisRange(-2.99,2.99,"X");
        }
        if (i==2) {
          vhTrkQual[i][j][k]->SetTitle(";#sigma(p_{T})/p_{T};Track Fraction");
          vhTrkQual[i][j][k]->SetAxisRange(0,0.0899,"X");
          vhTrkQual[i][j][k]->GetXaxis()->SetNdivisions(505);
        }
      }
    }
  }

  TCanvas *c = new TCanvas("c","",1125,860);
  makeMultiPanelCanvasNew(c,3,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
	
  // dz
  c->cd(1);
  gPad->SetLogy();
  vhTrkQual[0][0][0]->Draw("E");
  vhTrkQual[0][0][1]->Draw("sameE");
  c->cd(4);
  gPad->SetLogy();
  vhTrkQual[0][3][0]->Draw("E");
  vhTrkQual[0][3][1]->Draw("sameE");

  // dxy
  c->cd(2);
  gPad->SetLogy();
  vhTrkQual[1][0][0]->Draw("E");
  vhTrkQual[1][0][1]->Draw("sameE");
  c->cd(5);
  gPad->SetLogy();
  vhTrkQual[1][3][0]->Draw("E");
  vhTrkQual[1][3][1]->Draw("sameE");

  // pterror
  c->cd(3);
  gPad->SetLogy();
  vhTrkQual[2][0][0]->Draw("E");
  vhTrkQual[2][0][1]->Draw("sameE");
  c->cd(6);
  gPad->SetLogy();
  vhTrkQual[2][3][0]->Draw("E");
  vhTrkQual[2][3][1]->Draw("sameE");
  
  // more annotation
	TLegend * leg2 = new TLegend(0.43,0.59,0.86,0.79,NULL,"brNDC");
	easyLeg(leg2,"",28);
	leg2->AddEntry(vhTrkQual[0][0][0],"pPb Data","p");
	leg2->AddEntry(vhTrkQual[0][0][1],"AMPT MC","p");
  c->cd(1);
  drawText("CMS Preliminary",0.28,0.91,kBlack,20);
  drawText("#sqrt{s_{NN}}=5.02 TeV ",0.3,0.84,kBlack,20);
  c->cd(6);
	leg2->Draw();
  for (int i=0; i<3; ++i) {
    float tx=0.58,ty=0.85;
    if (i==0) tx+=0.075;
    c->cd(i+1);
    drawText("N_{Trk}^{offline} < 35",tx,ty,kBlack,28);
    c->cd(i+4);
    tx-=0.05;
    ty+=0.03;
    drawText("N_{Trk}^{offline} #geq 110",tx,ty,kBlack,28);
  }
  c->Print("fig/CompareTrkQuals.gif");
  c->Print("fig/CompareTrkQuals.pdf");
}