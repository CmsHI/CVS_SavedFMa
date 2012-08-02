#include "CutAndBinCollection2011.h"

TH1D* icent2npart (TH1D* h=0);
 
void drawGammaTrackSum(int cutOpt=3,  int ptCutTrack=2, int ipt = 1) {


  


  if ( (ptCutTrack!=0) && (ptCutTrack!=2) && (ptCutTrack!=4) ) {
    cout << " Wrong choice of the pt cut of tracks!!!!!" << endl;
    return;
  }
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int nPtAssoc = cab.getNPtBinAssoc();
 
  vector<double> ptBinPho = cab.getPtBin();
  int lowPtPho = ptBinPho[ipt-1];
  int highPtPho = ptBinPho[ipt];
  
  

  TFile* infHist = new TFile(Form("photonTrackOutput_cutOpt%d.root",cutOpt));
  
  
  TH1D* hfin[5][5];
  TH1D* hfin1bin[5][5] ;
  double maxY = 0;
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    hfin[icent][kHIDATA] = (TH1D*)infHist->Get(Form("hidata_icent%d_ipt%d_iptAssoc1_Xts%d_final",icent,ipt, (int)ptCutTrack ));
    handsomeTH1(hfin[icent][kHIDATA], ycolor[icent]);
    hfin[icent][kHIMC] = (TH1D*)infHist->Get(Form("himc_icent%d_ipt%d_iptAssoc1_Xts%d_final",icent,ipt, (int)ptCutTrack ));
    handsomeTH1(hfin[icent][kHIMC], ycolor[icent]);
    
    hfin[icent][kHIMC]->Rebin(2);
    hfin[icent][kHIDATA]->Rebin(2);    
    
    if ( maxY <  getCleverRange(hfin[icent][kHIDATA]))
      maxY = getCleverRange(hfin[icent][kHIDATA]);
    if ( maxY <  getCleverRange(hfin[icent][kHIMC]))
      maxY = getCleverRange(hfin[icent][kHIMC]);
  }
  maxY = maxY * 1.4;


  
  TCanvas* c1 = new TCanvas(Form("gammaTrackSum4bins_cutOpt%d_ipt%d_ptCutTrack%d",cutOpt,ipt,ptCutTrack),"",1300,400);
  makeMultiPanelCanvas(c1,4,1,0.0,0.0,0.24,0.15,0.02);
   TH1D* hTemp = new TH1D("hTempFinSumPt",";#Sigmap^{ch}_{T}/p_{T}^{#gamma};Entries per photon",1000,0,4);
  handsomeTH1(hTemp,0);
  for ( int icent =1 ; icent<=nCent_std ; icent++) {
    c1->cd(nCent_std+1- icent);
    hTemp->DrawCopy();
    hTemp->SetAxisRange(-.05,maxY,"Y");
    hTemp->SetAxisRange(0,1.99,"X");
    hTemp->SetNdivisions(505);
    hTemp->DrawCopy();
    jumSun(0,0,2,0);
    
    
    
    hfin[icent][kHIMC]->DrawCopy("same hist ");
    hfin[icent][kHIDATA]->DrawCopy("same");
    if ( icent == 1) {
      drawText("CMS Preliminary",0.3,0.83,1);
      drawText("PbPb #int L dt = ~150 #mub^{-1}",0.3,0.75,1);
    }
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;      
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)), 0.67,0.6,1);
    
    if ( icent == nCent_std) {
      if (highPtPho > 200)
	drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.3,0.9,1,15);
      else 
	drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.3,0.9,1,15);
      drawText("|#eta^{#gamma}| <1.44", 0.6,0.9,1,15);
      
      drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.3,0.82,1,15);
      drawText("|#eta^{jet}| <1.6", 0.6,0.82,1,15);
      
      if ( ptCutTrack == 0 )
	drawText("p^{ch}_{T} > 1GeV", 0.3,0.74,1,15);
      else
	drawText(Form("p^{ch}_{T} > %dGeV",ptCutTrack), 0.3,0.74,1,15);
    }
    
    if ( icent == nCent_std -1 ) {
      TLegend* l1 = new TLegend(0.04596402,0.7110489,0.5946804,0.9927337,NULL,"brNDC");
      easyLeg(l1,"");  
      l1->AddEntry(hfin[icent][kHIDATA],"DATA","p");
      l1->AddEntry(hfin[icent][kHIMC],"MC","l");
      l1->Draw();
    }
 }
  
  c1->SaveAs(Form("plotsPhotonTrack/gammaTrackSum_no1_ptPho%d_ptJet%dptCutTrack%d.gif",(int)lowPtPho,(int)cab.getJetPtCut(),ptCutTrack));
  
  TCanvas* c2 = new TCanvas(Form("gammaTrackSum_no2_cutOpt%d_ipt%d_ptCutTrack%d",cutOpt,ipt,ptCutTrack),"",1000,500);
  c2->Divide(2,1);
  c2->cd(1);
  hTemp->SetAxisRange(-.05,maxY,"Y");
  hTemp->SetAxisRange(0,2,"X");
  
  hTemp->DrawCopy();
  jumSun(0,0,2,0);
  TLegend* l11 = new TLegend(0.4596402,0.7110489,0.9246804,0.9927337,NULL,"brNDC");
  easyLeg(l11,"");
  
  for ( int icent =nCent_std ; icent>=1 ; icent--) {
    hfin[icent][kHIMC]->DrawCopy("same");

    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
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

  if ( ptCutTrack == 0 )
    drawText("p^{ch}_{T} > 1GeV", 0.3,0.74,1,15);
  else
    drawText(Form("p^{ch}_{T} > %dGeV",ptCutTrack), 0.3,0.74,1,15);
  
  c2->cd(2);
  hTemp->DrawCopy();
  jumSun(0,0,2,0);
  for ( int icent =nCent_std ; icent>=1 ; icent--) {
    hfin[icent][kHIDATA]->DrawCopy("same");
  }
  drawText("DATA", 0.7,0.65,1,20);
  l11->Draw();

  c2->SaveAs(Form("plotsPhotonTrack/gammaTrackSum_no2_ptPho%d_ptJet%dptCutTrack%d.gif",(int)lowPtPho,(int)cab.getJetPtCut(),ptCutTrack));
	     


  TCanvas* c3 = new TCanvas(Form("Npart_gammaTrackSum_no2_ptPho%d_ptJet%dptCutTrack%d",(int)lowPtPho,(int)cab.getJetPtCut(),ptCutTrack),"", 1200, 400);
  c3->Divide(3,1);
  c3->cd(1);
  
  TH1D* mxt1[5];
  TH1D* mxt[5];
  TH1D* rj1[5];
  TH1D* rj[5];

  mxt1[kHIDATA] = new TH1D(Form("gxt_data_ipt%d_iptAssoc1_Xts%d_purePho_pure",ipt, (int)ptCutTrack),";centrality bin;<#Sigmap^{ch}_{T}/p_{T}^{#gamma}>",4,0.5,4.5);
  mxt1[kHIMC]   = (TH1D*)mxt1[kHIDATA]->Clone(Form("gxt_mc_ipt%d_iptAssoc1_Xts%d_purePho_pure",ipt, (int)ptCutTrack));
  rj1[kHIDATA] = new TH1D(Form("rj_data_ipt%d_iptAssoc1_Xts%d_purePho_pure",ipt, (int)ptCutTrack),";centrality bin;<#Sigmap^{ch}_{T}/p_{T}^{#gamma}>",4,0.5,4.5);
  rj1[kHIMC] = (TH1D*)rj1[kHIDATA]->Clone(Form("rj_mc_ipt%d_iptAssoc1_Xts%d_purePho_pure",ipt, (int)ptCutTrack));
  
for ( int icent =1 ; icent<=nCent_std ; icent++) {
    mxt1[kHIDATA]->SetBinContent(icent, hfin[icent][kHIDATA]->GetMean() );
    mxt1[kHIDATA]->SetBinError  (icent, hfin[icent][kHIDATA]->GetMeanError() );
    mxt1[kHIMC]->SetBinContent(icent, hfin[icent][kHIMC]->GetMean() );
    mxt1[kHIMC]->SetBinError  (icent, hfin[icent][kHIMC]->GetMeanError() );
    
    hfin1bin[icent][kHIDATA] = (TH1D*)hfin[icent][kHIDATA]->Clone(Form("%s_singleBin",hfin[icent][kHIDATA]->GetName()));
    hfin1bin[icent][kHIMC] = (TH1D*)hfin[icent][kHIMC]->Clone(Form("%s_singleBin",hfin[icent][kHIMC]->GetName()));
    hfin1bin[icent][kHIDATA]->Rebin( hfin1bin[icent][kHIDATA]->GetNbinsX());
    hfin1bin[icent][kHIMC]->Rebin( hfin1bin[icent][kHIMC]->GetNbinsX());
    
    rj1[kHIDATA]->SetBinContent(icent, hfin1bin[icent][kHIDATA]->GetBinContent(1));
    rj1[kHIMC]->SetBinContent(  icent, hfin1bin[icent][kHIMC]->GetBinContent(1));
    rj1[kHIDATA]->SetBinError(icent, hfin1bin[icent][kHIDATA]->GetBinError(1));
    rj1[kHIMC]->SetBinError(  icent, hfin1bin[icent][kHIMC]->GetBinError(1));

    
  }
  mxt[kHIDATA] = icent2npart(mxt1[kHIDATA]);
  mxt[kHIMC] = icent2npart(mxt1[kHIMC]);
  handsomeTH1(mxt[kHIDATA],2);
  handsomeTH1(mxt[kHIMC],2);
  mxt[kHIMC]->SetMarkerStyle(24);
  


  TH1D* tempXt = new TH1D("tempXt",";N_{part}; <#Sigmap^{ch}_{T}/p_{T}^{#gamma}>",1000,-10,400);
  handsomeTH1(tempXt,0);
  tempXt->SetAxisRange(0,.7,"Y");
  tempXt->DrawCopy();
  mxt[kHIMC]->Draw("same");
  mxt[kHIDATA]->Draw("same");

  if (highPtPho > 200)
    drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.3,0.9,1,15);
  else
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.3,0.9,1,15);
  drawText("|#eta^{#gamma}| <1.44", 0.6,0.9,1,15);

  drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.3,0.82,1,15);
  drawText("|#eta^{jet}| <1.6", 0.6,0.82,1,15);

  if ( ptCutTrack == 0 )
    drawText("p^{ch}_{T} > 1GeV", 0.3,0.74,1,15);
  else
    drawText(Form("p^{ch}_{T} > %dGeV",ptCutTrack), 0.3,0.74,1,15);

  c3->cd(2);
  TH1D* rmxt1= (TH1D*)mxt1[kHIDATA]->Clone(Form("rmxt_mcData_ipt%d_iptAssoc1_Xts%d_purePho_pure",ipt, (int)ptCutTrack));
  rmxt1->Divide(mxt1[kHIMC]);
  TH1D* rmxt = icent2npart(rmxt1);

  tempXt->SetAxisRange(0.5,1.2,"Y");
  tempXt->SetYTitle("Data / MC of <#Sigmap^{ch}_{T}/p_{T}^{#gamma}>");
  tempXt->DrawCopy();
  rmxt->Draw("same");
  
  
  c3->cd(3);
  rj[kHIDATA] = icent2npart(rj1[kHIDATA]);
    rj[kHIMC] = icent2npart(rj1[kHIMC]);
    handsomeTH1(rj[kHIDATA],2);
    handsomeTH1(rj[kHIMC],2);
    rj[kHIMC]->SetMarkerStyle(24);
    TH1D* temprj = new TH1D("tempRj",";N_{part};R",1000,-10,400);
    handsomeTH1(temprj,0);
    temprj->SetAxisRange(0.4,1.,"Y");
    temprj->DrawCopy();
    rj[kHIMC]->Draw("same");
    rj[kHIDATA]->Draw("same");


      
    c3->SaveAs(Form("plotsPhotonTrack/gammaTrackSum_no3_ptPho%d_ptJet%dptCutTrack%d.gif",(int)lowPtPho,(int)cab.getJetPtCut(),ptCutTrack));

    
    
}

TH1D* icent2npart (TH1D* h) {
  if ( h->GetNbinsX() != 4)  {
    cout << "no 4 bins!!" << endl;
    return 0;
  }
  double a[8] = { 43.6661 -0.1, 43.6661+0.1, 116.36 -0.1,  116.36+ 0.1, 235.769 -0.1, 235.769+0.1, 359.22 -0.1 , 359.22+0.1 } ;
  TH1D* hres = new TH1D( Form("%s_npart",h->GetName()) , "",7,a);
  hres->GetXaxis()->SetTitle("N_{part}");
  hres->GetYaxis()->SetTitle( h->GetYaxis()->GetTitle());
  hres->SetBinContent( 1,  h->GetBinContent(4));
  hres->SetBinContent( 3,  h->GetBinContent(3));
  hres->SetBinContent( 5,  h->GetBinContent(2));
  hres->SetBinContent( 7,  h->GetBinContent(1));
  hres->SetBinError( 1,  h->GetBinError(4));
  hres->SetBinError( 3,  h->GetBinError(3));
  hres->SetBinError( 5,  h->GetBinError(2));
  hres->SetBinError( 7,  h->GetBinError(1));
  return hres;
}
