#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>

void drawFragFunctionOpt(int cutOpt=3, int ipt = 1, int opt=1);

void drawFragFunction(int opt=1, int cutOpt=3, int ipt = 1) {
 drawFragFunctionOpt(cutOpt,ipt,opt);
   
}

void drawFragFunctionOpt(int cutOpt, int ipt , int opt) {
  
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int nPtAssoc = cab.getNPtBinAssoc();
 
  vector<double> ptBinPho = cab.getPtBin();
  int lowPtPho = ptBinPho[ipt-1];
  int highPtPho = ptBinPho[ipt];
  
  int nCentBin = cab.getNCentBin();
  vector<double> centBin = cab.getCentBin();
  

  TFile* infHist = new TFile(Form("photonTrackOutput_cutOpt%d.root",cutOpt));
  
  TH1D* hfin[5][5];
  TH1D* hfin1bin[5][5] ;
  double maxY = 0;
  TString varName="";
  TH1D* hTemp = new TH1D("hTempFinSumPt",";#Sigmap^{ch}_{T}/p_{T}^{#gamma};Normalied by area",2000,0,100);
  handsomeTH1(hTemp,0);
 
  if ( opt == 1 ) {
    varName = "xiAway";
    hTemp->SetXTitle("#xi = ln(p_{T}^{#gamma}/p^{ch}_{T})");
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(0.0001,30,"Y");
  }
  if ( opt == 11 ) {
    varName = "xiAway_lx";
    hTemp->SetXTitle("#xi = ln(p_{T}^{#gamma}/p^{ch}_{T})");
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(0.0001,30,"Y");
  }
  else if ( opt == 12 ) {
    varName = "xiAway_hx";
    hTemp->SetXTitle("#xi = ln(p_{T}^{#gamma}/p^{ch}_{T})");
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(0.0001,30,"Y");
  }


  else if ( opt == 2) {
    varName = "xij";
    hTemp->SetXTitle("#xi = ln(p_{T}^{jet}/p^{ch}_{T})");
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(0.0001,30,"Y");
  }
  else if ( opt == 21) {
    varName = "xij_lx";
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(0.0001,30,"Y");
  }
  else if ( opt == 22) {
    varName = "xij_hx";
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(0.0001,30,"Y");
  }

  else if ( opt == 3) {
    varName = "PtAway";
    hTemp->SetAxisRange(0,100,"X");
  }
  else if ( opt == 4) {
    varName = "dphi";
    hTemp->SetAxisRange(0,3.14159,"X");
  }

  else if ( opt == 5) {
    varName = "xiInc05";
    hTemp->SetAxisRange(0,7,"X");
  }


  for ( int icent =1 ; icent<=nCentBin ; icent++) {
    hfin[icent][kHIDATA] = (TH1D*)infHist->Get(Form("hidata_icent%d_ipt%d_iptAssoc1_%s_purePho_hardTrackInJetCone",icent,ipt, varName.Data()));
    hfin[icent][kHIMC] = (TH1D*)infHist->Get(Form(    "himc_icent%d_ipt%d_iptAssoc1_%s_purePho_hardTrackInJetCone",icent,ipt, varName.Data()));
    // scaleInt(hfin[icent][kHIMC]);
    //  scaleInt(hfin[icent][kHIDATA]);
    handsomeTH1(hfin[icent][kHIDATA], ycolor[icent]);
    handsomeTH1(hfin[icent][kHIMC], ycolor[icent]);
    
    cout << " here " <<endl;
    
    if ( maxY <  getCleverRange(hfin[icent][kHIDATA]))
      maxY = getCleverRange(hfin[icent][kHIDATA]);
    if ( maxY <  getCleverRange(hfin[icent][kHIMC]))
      maxY = getCleverRange(hfin[icent][kHIMC]);
    cout << " name = " << hfin[icent][kHIDATA]->GetName() << endl;
    cout << " nCent = " << nCentBin << endl;
  }
  maxY = maxY * 1.4;
  
  
  cout << " here2 " <<endl;
  
  TCanvas* c1 = new TCanvas(Form("fragFunction_cutOpt%d_ipt%d",cutOpt,ipt),"",880,500);
  makeMultiPanelCanvas(c1,nCentBin  ,1,0.0,0.0,0.24,0.15,0.02);
   
  for ( int icent =1 ; icent<=nCentBin ; icent++) {
    c1->cd(nCentBin+1- icent);
    hTemp->DrawCopy();

    hTemp->SetNdivisions(505);
    hTemp->DrawCopy();
    jumSun(0,0,2,0);
    
    
    hfin[icent][kHIMC]->SetAxisRange(0.0001,30,"Y");
    hfin[icent][kHIDATA]->SetAxisRange(0.0001,30,"Y");

    hfin[icent][kHIMC]->DrawCopy("same hist ");
    hfin[icent][kHIDATA]->DrawCopy("same");
    if ( icent == 1) {
      drawText("CMS Preliminary",0.4,0.88,1);
      drawText("PbPb #int L dt = 150 #mub^{-1}",0.4,0.8,1);
    }
    int lowCent = centBin[icent-1];
    int highCent = centBin[icent]-1;      
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.67,0.7,1);
    
    if ( icent == nCentBin) {
      if (highPtPho > 200)
	drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.3,0.9,1,15);
      else 
	drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.3,0.9,1,15);
      drawText("|#eta^{#gamma}| <1.44", 0.6,0.9,1,15);
      
      drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.3,0.82,1,15);
      drawText("|#eta^{jet}| <1.6", 0.6,0.82,1,15);
      
    }
    
    if ( icent == nCentBin -1 ) {
      TLegend* l1 = new TLegend(0.04596402,0.7110489,0.5946804,0.9927337,NULL,"brNDC");
      easyLeg(l1,"");  
      l1->AddEntry(hfin[icent][kHIDATA],"DATA","p");
      l1->AddEntry(hfin[icent][kHIMC],"MC","l");
      l1->Draw();
    }
    gPad->SetLogy();
  }
 
  c1->SaveAs(Form("plotsPhotonTrack/%s_no1_ptPho%d_ptJet%d.pdf",varName.Data(),(int)lowPtPho,(int)cab.getJetPtCut()));
  
  TCanvas* c2 = new TCanvas(Form("fragFunction_no2_cutOpt%d_ipt%d",cutOpt,ipt),"",1000,500);
  c2->Divide(2,1);
  c2->cd(1);
  hTemp->DrawCopy();
  jumSun(0,0,2,0);
  TLegend* l11 = new TLegend(0.4596402,0.7110489,0.9246804,0.9927337,NULL,"brNDC");
  easyLeg(l11,"");
  
  for ( int icent =nCentBin ; icent>=1 ; icent--) {
    hfin[icent][kHIMC]->DrawCopy("same");

    int lowCent = centBin[icent-1];
    int highCent = centBin[icent]-1;
    l11->AddEntry(hfin[icent][kHIMC],Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),"pl");
  }
  drawText("MC", 0.7,0.65,1,20);

  if (highPtPho > 200)
    drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.3,0.9,1,15);
  else
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.3,0.9,1,15);
  drawText("|#eta^{#gamma}| <1.44", 0.6,0.9,1,15);

  drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.3,0.82,1,15);
  drawText("|#eta^{jet}| <1.6", 0.6,0.82,1,15);
  gPad->SetLogy();

  c2->cd(2);
  hTemp->DrawCopy();
  jumSun(0,0,2,0);
  for ( int icent =nCentBin ; icent>=1 ; icent--) {
    hfin[icent][kHIDATA]->DrawCopy("same");
  }
  drawText("DATA", 0.7,0.65,1,20);
  l11->Draw();
  gPad->SetLogy();
  
  c2->SaveAs(Form("plotsPhotonTrack/%s_no2_ptPho%d_ptJet%d.pdf",varName.Data(),(int)lowPtPho,(int)cab.getJetPtCut()));
  
}
