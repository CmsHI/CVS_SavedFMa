#include "CutAndBinCollection2011.h"

void finalPlotsGammaTrack () {
  
  TH1D* hDphiF[5][10];
  TH1D* hXawayF[5][10];
  TH1D* hPtAwayF[5][10];

  TFile* inf = new TFile("photonTrackOutput_photonPt50GeV.root");
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    hXawayF[icent][kHIDATA] = (TH1D*)inf->Get(Form("hidata_icent%d_hTrkXAway_final",icent));
    handsomeTH1(hXawayF[icent][kHIDATA],color[icent]);
    hXawayF[icent][kHIMC] = (TH1D*)inf->Get(Form("himc_icent%d_hTrkXAway_final",icent));
    handsomeTH1(hXawayF[icent][kHIMC],color[icent]);

    TH1ScaleByWidth(hXawayF[icent][kHIDATA]);
    TH1ScaleByWidth(hXawayF[icent][kHIMC]);

  }
  
  TCanvas* c100 = new TCanvas("c100","",1000,500);
  c100->Divide(2,1);
  c100->cd(2);
  TH1D* htemp100 = (TH1D*)hXawayF[1][kHIDATA]->Clone("htemp100");
  htemp100->Reset();
  handsomeTH1(htemp100,1);
  htemp100->SetAxisRange(0.01,1e2,"Y");
  htemp100->SetYTitle("dN_{track}/dx_{T#gamma}   per #gamma");
  htemp100->DrawCopy();
  gPad->SetLogy();
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    hXawayF[nCent_std+1-icent][kHIDATA]->Draw("same");
  }  
  TLegend* l1 = new TLegend(0.4979083,0.4978381,0.9999163,0.7767209,NULL,"brNDC");
  easyLeg(l1,"");
  for ( int icent = nCent_std ; icent>=1 ; icent--) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    l1->AddEntry(hXawayF[icent][kHIDATA], Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)));
  } 

  drawText("CMS Preliminary",0.5,0.83,1);
  drawText("PbPb #int L dt = ~150 #mub^{-1}",0.5,0.75,1);
  
  c100->cd(1);
  htemp100->DrawCopy();
  gPad->SetLogy();
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    hXawayF[nCent_std+1-icent][kHIMC]->Draw("same");
  }
  drawText("MC",0.5,0.83,1);
  drawText("PYTHIA+HYDJET",0.5,0.75,1);
  l1->Draw();

  int percentBin[5] = {0,10,30,50,100};

  TCanvas* c102 = new TCanvas("c102","",1000,350);
  makeMultiPanelCanvas(c102,4,1,0.0,0.0,0.24,0.15,0.02);
  htemp100->SetYTitle("DATA/MC");
  htemp100->SetAxisRange(0,2,"Y");
  htemp100->SetNdivisions(505);
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    c102->cd(nCent_std+1-icent);
    htemp100->DrawCopy();
    jumSun(0,1,1,1);
    TH1D* tempRatio =  (TH1D*)hXawayF[icent][kHIDATA]->Clone(Form("tempRatio_icent%d",icent));
    tempRatio->Divide(hXawayF[icent][kHIMC]);
    tempRatio->Draw("same");
    if ( icent ==4 ) {
      drawText("CMS Preliminary",0.4,0.91,1,16);
      drawText("PbPb #int L dt = ~150 #mub^{-1}",0.4,0.82,1,16);
    }
    drawText(Form("%d%% - %d%%",percentBin[icent-1],percentBin[icent]),0.4,0.73,1,16);
  }
  
  
}
