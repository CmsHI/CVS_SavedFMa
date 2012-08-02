#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>


void drawDphi2(int cutOpt=101,  int ipt = 1) {
  TFile* infHist = new TFile(Form("photonTrackOutput_cutOpt%d.root",cutOpt));
  
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
   
  vector<double> ptCutAssoc = cab.getPtBinAssoc();
  vector<double> ptBinPho = cab.getPtBin();
  int lowPtPho = ptBinPho[ipt-1];
  int highPtPho = ptBinPho[ipt];
  
  int nCent_std = cab.getNCentBin();
  vector<double> centBin_std = cab.getCentBin();
  
  int  nPtAssoc = cab.getNPtBinAssoc();
  TH1D* hdphi_data[10][10] ; 
  TH1D* hdphi_mc[10][10] ;
  TH1D* hdphiInt_data[10][10] ;
  TH1D* hdphiInt_mc[10][10] ;

  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    for ( int iptAssoc = 1; iptAssoc <= nPtAssoc ; iptAssoc++) {
      
      
      
    float ptCutAssLow  = ptCutAssoc[iptAssoc-1];
    float ptCutAssHigh = ptCutAssoc[iptAssoc];
    hdphi_mc[icent][iptAssoc] = (TH1D*)infHist->Get(Form("himc_icent%d_ipt%d_iptAssoc%d_dphi_purePho_hardTrackInJetCone",
							 icent,ipt,iptAssoc));
    hdphi_data[icent][iptAssoc] = (TH1D*)infHist->Get(Form("hidata_icent%d_ipt%d_iptAssoc%d_dphi_purePho_hardTrackInJetCone",
							   icent,ipt,iptAssoc));  
    
    hdphi_mc[icent][iptAssoc]->Rebin(4);
    hdphi_data[icent][iptAssoc]->Rebin(4);

    hdphiInt_data[icent][iptAssoc] = (TH1D*)hdphi_data[icent][iptAssoc]->Clone(Form("Int_data_%d_%d_%d",ipt,icent,iptAssoc));
    
    if ( iptAssoc > 1 )
      hdphiInt_data[icent][iptAssoc]->Add(hdphiInt_data[icent][iptAssoc-1]);
    hdphiInt_mc[icent][iptAssoc] = (TH1D*)hdphi_mc[icent][iptAssoc]->Clone(Form("Int_mc_%d_%d_%d",ipt,icent,iptAssoc));
    if ( iptAssoc > 1 )
      hdphiInt_mc[icent][iptAssoc]->Add(hdphiInt_mc[icent][iptAssoc-1]);
   
    handsomeTH1(hdphiInt_data[icent][iptAssoc],ycolor[iptAssoc]);
    hdphiInt_data[icent][iptAssoc]->SetFillColor(iptAssoc);
    handsomeTH1(hdphiInt_mc[icent][iptAssoc],ycolor[iptAssoc]);
    hdphiInt_mc[icent][iptAssoc]->SetFillColor(iptAssoc);
    }
  }

  
  TCanvas* c0 = new TCanvas(Form("c0_dphi_cutOpt%d_ipt%d",cutOpt,ipt),"",nCent_std*300+100,nPtAssoc*300+100);
  makeMultiPanelCanvas(c0,nCent_std,nPtAssoc,0.0,0.0,0.24,0.15,0.02);
  
  for ( int iptAssoc = 1; iptAssoc <= nPtAssoc ; iptAssoc++) {
    float ptCutAssLow  = ptCutAssoc[iptAssoc-1];
    float ptCutAssHigh = ptCutAssoc[iptAssoc];
    float maxY = getCleverRange(hdphi_mc[1][iptAssoc]) * 1.5;
    
    for ( int icent =1 ; icent<=nCent_std ; icent++) {
      c0->cd(nCent_std+1 - icent + (iptAssoc-1) * nCent_std);
      hdphi_mc[icent][iptAssoc]->SetAxisRange(maxY * -0.1,maxY,"Y");
      if ( iptAssoc < 4 )
	hdphi_mc[icent][iptAssoc]->SetAxisRange(-1,3,"Y");
      
      hdphi_mc[icent][iptAssoc]->SetXTitle("#Delta#phi_{#photon-h^{#pm}}");
      hdphi_mc[icent][iptAssoc]->SetYTitle("Entries per trigger");
      handsomeTH1(hdphi_mc[icent][iptAssoc],1);
      hdphi_mc[icent][iptAssoc]->Draw("hist");
      hdphi_data[icent][iptAssoc]->Draw("same");
      jumSun(0,0,3.141592,0);
      
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.3,0.8,1,15);
      if ( ptCutAssHigh < 50 ) 
	drawText(Form(" p_{T}^{Assoc.} :%.0f - %.0f GeV", float( ptCutAssLow),  (float)ptCutAssHigh), 0.5,0.6,1,15);
      if ( ptCutAssHigh > 50 )
        drawText(Form("p_{T}^{Assoc.} > %.0f GeV", float( ptCutAssLow)), 0.5,0.6,1,15);
      
	
    }
  }  
  
  
  
  TCanvas* c1 = new TCanvas(Form("dphi_cutOpt%d_ipt%d",cutOpt,ipt),"",nCent_std*300+100,700);
  makeMultiPanelCanvas(c1,nCent_std,2,0.0,0.0,0.24,0.15,0.02);
  TH1D* hTemp = new TH1D("hTempDphi",";#Delta#phi;# of tracks per trigger #gamma",1000,0,PI);
  handsomeTH1(hTemp);
  hTemp->SetNdivisions(505);
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    c1->cd(nCent_std+1- icent);
    hTemp->SetAxisRange(0.001,15,"Y");
    hTemp->DrawCopy();
    
    for ( int iptAssoc = nPtAssoc ; iptAssoc >= 1 ; iptAssoc--) {
      hdphiInt_data[icent][iptAssoc]->Draw("same hist");
      }
    if ( icent == 1) {
      drawText("CMS Preliminary",0.3,0.88,1);
      drawText("PbPb #int L dt = ~150 #mub^{-1}",0.3,0.8,1);
      }
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;      
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.67,0.7,1);
    
    if ( icent == nCent_std) {
      if (highPtPho > 200)
	drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.3,0.9,1,15);
      else 
	drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.3,0.9,1,15);
      drawText("|#eta^{#gamma}| <1.44", 0.6,0.9,1,15);
      
      //	drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.3,0.82,1,15);
      drawText("|#eta^{track}| <2", 0.6,0.82,1,15);
      
    }
    drawText("DATA", 0.6,0.6,1,20);

    
    c1->cd(nCent_std *2 +1- icent);
    hTemp->DrawCopy();
    
    for ( int iptAssoc = nPtAssoc ; iptAssoc >= 1 ; iptAssoc--) {
      hdphiInt_mc[icent][iptAssoc]->Draw("same hist");
    }
    drawText("MC", 0.6,0.6,1,20);
    
    /*    
	  if ( icent == nCent_std -1 ) {
	  TLegend* l1 = new TLegend(0.04596402,0.7110489,0.5946804,0.9927337,NULL,"brNDC");
	  easyLeg(l1,"");  
	  l1->AddEntry(hfin[icent][kHIDATA],"DATA","p");
	  l1->AddEntry(hfin[icent][kHIMC],"MC","l");
	  l1->Draw();
	  }
    */
  }
  
    c1->SaveAs(Form("plotsPhotonTrack/dphi_no1_ptPho%d_ptJet%d.gif",(int)lowPtPho,(int)cab.getJetPtCut()));
    
    
}
