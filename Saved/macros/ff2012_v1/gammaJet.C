#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include "commonUtility.h"
#include <TRandom3.h>
#include "CutAndBinCollection2011.h"
#include "corrFunctions.h"
#define PI 3.141592653589

void gammaJetDiff(corrFunctions* corr=0, int icent=1, bool isMC=false,  TCut addCut="");

void gammaJet(){  
  
  corrFunctions* mcCorr[10];
  corrFunctions* dataCorr[10];
  for (int icent = 1; icent<= nCent_std ; icent++) {
    mcCorr[icent] = new corrFunctions();
    dataCorr[icent] = new corrFunctions();
    gammaJetDiff(mcCorr[icent],  icent,true,  "");
    gammaJetDiff(dataCorr[icent],icent,false,  "");
  }
  
  TCanvas* cx1  = new TCanvas("cx1","",750,700);
  makeMultiPanelCanvas(cx1,2,2,0.0,0.0,0.2,0.15,0.02);
  TH1D* xTemp = (TH1D*)dataCorr[1]->xFinal->Clone("xTemp29");
  xTemp->Reset();  xTemp->SetAxisRange(0,0.28,"Y");
  for (int icent = 1; icent<= nCent_std ; icent++) {
    cx1->cd(nCent_std + 1 - icent);
    handsomeTH1(dataCorr[icent]->xFinal,2);
    mcStyle3(mcCorr[icent]->xFinal);
    xTemp->DrawCopy();
    mcCorr[icent]->xFinal->DrawCopy("hist e same");
    dataCorr[icent]->xFinal->DrawCopy("same");
    //dataCorr[icent]->xPureJetNIpho->Draw("same");
    float xpi0  = dataCorr[icent]->xPureJetNIpho->GetMean();
    cout <<" mean of pi0 =" << xpi0 <<  "    ";
    float xcand = dataCorr[icent]->xPureJetWIpho->GetMean();
    cout <<" mean of isoPho =" << xcand << endl;
    float theP = dataCorr[icent]->purity;
    cout <<" purity = " << theP << endl;
    cout <<" mean of pure pho = " << (xcand - (1.-theP) * xpi0 )/theP << endl;
    float theCentVal = (xcand - (1.-theP) * xpi0 )/theP;
    
    theP = dataCorr[icent]->purity * 0.88;
    cout << " lower bound : " <<  (xcand- (1.-theP) * xpi0 )/theP ;
    theP = dataCorr[icent]->purity * 1.12;
    
    cout << "    upper bound : " << (xcand- (1.-theP) * xpi0 )/theP << endl << endl;
    
    
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5680963,0.8369118);
    
    if ( icent == nCent_std ) {
      TH1D* hTempData =(TH1D*)dataCorr[icent]->xFinal->Clone("hTempData");
      TH1D* hTempMC =(TH1D*)mcCorr[icent]->xFinal->Clone("hTempmc");
      
      TLegend* leg0 =  new TLegend(0.5076446,0.5322479,0.9982492,0.7981618,NULL,"brNDC");
      easyLeg(leg0,"#sqrt{s}_{_{NN}}=2.76 TeV");
      leg0->AddEntry(hTempData,"data","pl");
      leg0->AddEntry(hTempMC,"PYTHIA+HYDJET","fl");
      leg0->Draw();
    }
    if ( icent == nCent_std-1)
      drawCMS2011(0.2,0.9,150);
  }

  cx1->SaveAs("cx1.gif");

  TCanvas* cx2  = new TCanvas("cx2","",1000,500);
  cx2->Divide(2,1);
  cx2->cd(1);
  xTemp->DrawCopy();
  TLegend* leg1 =  new TLegend(0.5,0.65,0.95,0.9,NULL,"brNDC");
  easyLeg(leg1,"data");

  for (int icent = 1; icent<= nCent_std ; icent++) {
    handsomeTH1(dataCorr[icent]->xFinal,color[icent]);
    dataCorr[icent]->xFinal->Draw("same");
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    leg1->AddEntry(dataCorr[icent]->xFinal, Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)), "lp");
  }
  leg1->Draw();
  cx2->cd(2);
  xTemp->DrawCopy();
  for (int icent = 1; icent<= nCent_std ; icent++) {
    mcCorr[icent]->xFinal->SetFillStyle(0);
    handsomeTH1(mcCorr[icent]->xFinal,color[icent]);
    mcCorr[icent]->xFinal->Draw("same");
  }
  TLegend* leg2 = (TLegend*)leg1->Clone();
  easyLeg(leg2,"mc");
  leg2->Draw();

   cx2->SaveAs("cx2.gif");


  ///   Mean X
  TCanvas* cx3  = new TCanvas("cx3","",500,500);
  double nPartVal[10]; 
  TH1D* hnp = new TH1D("hnpart","",nCent_std,centBin_std); 
  for (int icent = 1; icent<= nCent_std ; icent++) {
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    double thenp=0;
    for ( int ii=lowerCent ; ii<=upperCent; ii++) {
      thenp = thenp + getHnaprt(ii);
    }
    nPartVal[icent] = thenp/(upperCent-lowerCent+1.);
  }
  double nPartbins[30];
  nPartbins[0] = 0;
  for (int icent = 1; icent<= nCent_std ; icent++) {
    nPartbins[2*icent-1] = nPartVal[nCent_std+1-icent] - 1;
    nPartbins[2*icent]   = nPartVal[nCent_std+1-icent] + 1;
  }
  TH1D* mcMx = new TH1D("mcMx",";N_{part};<X_{j#gamma}>",nCent_std*2, nPartbins);
  TH1D* dataMx = (TH1D*)mcMx->Clone("dataMx");
  for (int icent = 1; icent<= nCent_std ; icent++) {
    int theBin = (nCent_std - icent + 1 )*2;
    mcMx->SetBinContent(theBin,  mcCorr[icent]->meanx);
    mcMx->SetBinError(theBin,  mcCorr[icent]->meanxErr);
    dataMx->SetBinContent(theBin,  dataCorr[icent]->meanx);
    dataMx->SetBinError(theBin,  dataCorr[icent]->meanxErr);
  }
  handsomeTH1(dataMx,1);
  handsomeTH1(mcMx,1);
  mcMx->SetMarkerStyle(25);
  TH1D* tempMx  = new TH1D("tempMx",";N_{part};<X_{j#gamma}>",50,0,400);
  tempMx->SetAxisRange(0.45,1.2,"Y");
  tempMx->Draw();
  mcMx->Draw("same");
  dataMx->Draw("same");
  TLegend* leg4 =  new TLegend(0.2076446,0.7222479,0.6982492,0.9081618,NULL,"brNDC");
  easyLeg(leg4,"#sqrt{s}_{_{NN}}=2.76 TeV");
  leg4->AddEntry(dataMx,"data","pl");
  leg4->AddEntry(mcMx,"PYTHIA+HYDJET","pl");
  leg4->Draw();

  cx3->SaveAs("cx3.gif");
  

  TCanvas* cx4  = new TCanvas("cx4","",500,500);
  TH1D* mcRx = new TH1D("mcRx","",nCent_std*2, nPartbins);
  TH1D* dataRx = (TH1D*)mcMx->Clone("dataRx");
  for (int icent = 1; icent<= nCent_std ; icent++) {
    int theBin = (nCent_std - icent + 1 )*2;
    mcRx->SetBinContent(theBin,  mcCorr[icent]->rx0);
    mcRx->SetBinError(theBin,  mcCorr[icent]->rxErr0);
    dataRx->SetBinContent(theBin,  dataCorr[icent]->rx0);
    dataRx->SetBinError(theBin,  dataCorr[icent]->rxErr0);
  }
  TH1D* tempRx  = new TH1D("tempRx",";N_{part};R(j#gamma>0)",50,0,400);
  tempRx->SetAxisRange(0,1.5,"Y");
  handsomeTH1(dataRx,1);
  handsomeTH1(mcRx,1);
  mcRx->SetMarkerStyle(25);
  tempRx->Draw();
  mcRx->Draw("same");
  dataRx->Draw("same");
  leg4->Draw();
  
  cx4->SaveAs("cx4.gif");


}









void gammaJetDiff(corrFunctions* corr, int icent, bool isMC,  TCut addCut)   {
  
  int bjType = kEvtMix;
  if ( isMC ) bjType = kDphiJet;
  
  corr->initAll(isMC, icent,bjType);
  
  int lowCent = centBin_std[icent-1];
  int highCent = centBin_std[icent]-1;
  TCut centCut = Form("cBin >= %d && cBin<= %d",lowCent,highCent);
  cout << " cent cut :      " << centCut.GetTitle() << endl;
  
  
  multiTreeUtil* photon1 = new multiTreeUtil();
 
  if ( isMC ) { 
    photon1->addFile("../forest/barrelHiforestv2_qcdAllPhoton30_allCent.root" , 
		     "tgj", "yPhoton.ptHat>30 && yPhoton.ptHat<50" , 1.59e-6 - 7.67e-7);
    photon1->addFile("../forest/barrelHiforestv2_qcdAllPhoton50_allCent.root" , 
		     "tgj", "yPhoton.ptHat>50 && yPhoton.ptHat<80" , 7.67e-7 - 1.72e-7);
    photon1->addFile("../forest/barrelHiforestv2_qcdAllPhoton80_allCent.root" , 
		     "tgj", "yPhoton.ptHat>80" , 1.72e-7);
  }
  else {
    photon1->addFile("yskim_HiForestPhoton-v7-noDuplicate_nMix40_NoEvtPlReq.root","tgj","",1);
    //    photon1->addFile("yskim_HiForestPhoton-v7-noDuplicate_nMix5_noEvtPlnReq.root","tgj","",1);
    //  photon1->addFile("yskim_HiForestPhoton-v7-noDuplicate_nMinBisMixedQuarterPI.root", "tgj", "",1);
  }
  
  photon1->AddFriend("yEvt=yongsunHiEvt");
  photon1->AddFriend("ySkim=yongsunSkimTree");
  photon1->AddFriend("yHlt=yongsunHltTree");  
  photon1->AddFriend("yJet=yongsunJetakPu3PF");
  photon1->AddFriend("yPhoton=yongsunPhotonTree");
  photon1->SetAlias("dphig","acos(cos(photonPhi-yJet.jtphi))");
  if (bjType ==kEvtMix) {
    photon1->AddFriend("ymJet=tmixJet");
    photon1->SetAlias("dphigm","acos(cos(photonPhi-ymJet.phi))");
    
  }


  TCut generalEvtCut  = centCut && addCut && "(ySkim.pcollisionEventSelection==1)"; // && (ySkim.pHBHENoiseFilter==1)";
  if ( !isMC )
    generalEvtCut = generalEvtCut && "ySkim.pHBHENoiseFilter==1";

  // Should we include trigger 30? 
    
  TCut ptPhoCut  = "photonEt>60";
  TCut hoePhoCot = "hovere < 0.1";
  TCut caloIso = "(cc4 + cr4 + ct4PtCut20)/0.9 < 1";
  TCut isoPhoCut = caloIso && "sigmaIetaIeta<0.010";  //todo => should be flexible
  TCut sbPhoCut  = caloIso && "(sigmaIetaIeta>0.011) && (sigmaIetaIeta<0.017)";
  TCut genIsoPhoton = "";
  TCut isoPhotonCut = ptPhoCut && hoePhoCot && isoPhoCut;
  TCut nonIsoPhoCut = ptPhoCut && hoePhoCot && sbPhoCut;
  
  if ( isMC ) 
   isoPhotonCut =  isoPhotonCut && "genIso<5";  //todo : add genMatch Cut with GenIso Cut //  TCut theGenCut = Form("isGenMatched && genCalIsoDR04< %f",genIsoThr)
  
  float cutPtJet = 30;
  float cutEtaJet = 2;
  TCut jetPtCut      = Form("yJet.jtpt>%.1f",(float)cutPtJet);
  TCut jetEtaCut     = Form("abs(yJet.jteta)<%.1f",(float)cutEtaJet);
  TCut jetGeneralCut = jetPtCut && jetEtaCut ;
  TCut awayJetCut    = Form("dphig > %f",  (float)(PI - awayRange));
  TCut sbJetCut      = Form("dphig > 0.7 &&  dphig < %f/2.", (float)PI );
  
  TCut mjetPtCut      = Form("ymJet.pt>%.1f",(float)cutPtJet);
  TCut mjetEtaCut     = Form("abs(ymJet.eta)<%.1f",(float)cutEtaJet);
  TCut mjetGeneralCut = mjetPtCut && mjetEtaCut ;
  TCut mawayJetCut    = Form("dphigm > %f",  (float)(PI - awayRange));
    
  
  TString theWeightBit;
  if ( isMC ) theWeightBit = "reweight";
  else        theWeightBit = "";

  photon1->Draw2(corr->isoPhotonEt, "photonEt", generalEvtCut && isoPhotonCut, theWeightBit);
  photon1->Draw2(corr->nonIsoPhoEt, "photonEt", generalEvtCut && nonIsoPhoCut, theWeightBit);


  photon1->Draw2(corr->jetDphi, "dphig",
		 generalEvtCut && isoPhotonCut && jetGeneralCut, theWeightBit);
  photon1->Draw2(corr->jetDphiNIpho, "dphig",
		 generalEvtCut && nonIsoPhoCut && jetGeneralCut, theWeightBit);

  photon1->Draw2(corr->jetPtAway, "yJet.jtpt", 
		 generalEvtCut && isoPhotonCut && jetEtaCut && awayJetCut, theWeightBit);
  photon1->Draw2(corr->jetPtSBdphi  , "yJet.jtpt", 
		 generalEvtCut && isoPhotonCut && jetEtaCut && sbJetCut  ,theWeightBit);
  photon1->Draw2(corr->xAwayWellIso, "yJet.jtpt/photonEt", 
		 generalEvtCut && isoPhotonCut && awayJetCut && jetGeneralCut, theWeightBit);
  photon1->Draw2(corr->xsbPhiWellIso, "yJet.jtpt/photonEt", 
		 generalEvtCut && isoPhotonCut && sbJetCut && jetGeneralCut, theWeightBit);

  photon1->Draw2(corr->xAwayNonIso, "yJet.jtpt/photonEt",
                 generalEvtCut && nonIsoPhoCut && awayJetCut && jetGeneralCut, theWeightBit);
  photon1->Draw2(corr->xsbPhiNonIso,"yJet.jtpt/photonEt",
                 generalEvtCut && nonIsoPhoCut && sbJetCut   && jetGeneralCut, theWeightBit);
  
  if (bjType ==kEvtMix) {
    photon1->Draw2(corr->jetDphiMix, "dphigm", 
		   generalEvtCut && isoPhotonCut && mjetGeneralCut, theWeightBit);
    photon1->Draw2(corr->jetDphiMixNIpho, "dphigm",
                   generalEvtCut && nonIsoPhoCut && mjetGeneralCut, theWeightBit);

    photon1->Draw2(corr->jetPtMix, "ymJet.pt",
		   generalEvtCut && isoPhotonCut && mjetEtaCut && mawayJetCut, theWeightBit);
    photon1->Draw2(corr->xMJetWellIso, "ymJet.pt/photonEt",
		   generalEvtCut && isoPhotonCut && mawayJetCut && mjetGeneralCut, theWeightBit);
    photon1->Draw2(corr->xMJetNonIso,  "ymJet.pt/photonEt",
		   generalEvtCut && nonIsoPhoCut && mawayJetCut && mjetGeneralCut, theWeightBit);
 

 }
  
  
  
  

  corr->setNorm();
  corr->calCorr();

  
  TString mcLabel = "data";
  if (isMC) mcLabel = "mc";
  TCanvas* c0 = new TCanvas(Form("c0_%s_icent%d",mcLabel.Data(),icent),"",1200,700);
  c0->Divide(4,2);
  c0->cd(1);
  corr->isoPhotonEt->Draw();
  handsomeTH1(corr->nonIsoPhoEt,2);
  corr->nonIsoPhoEt->Draw("same");
  //  gPad->SetLogy();
  drawText(mcLabel.Data(), 0.5,0.8);
  
  c0->cd(2);
  corr->jetDphi->DrawCopy();

  c0->cd(3);
  handsomeTH1(corr->jetPtAway,1);
  corr->jetPtAway->Draw();
  if (bjType == kEvtMix) {
    handsomeTH1(corr->jetPtMix,2);
    corr->jetPtMix->Draw("same hist");
  }
  if (bjType == kDphiJet) {
    handsomeTH1(corr->jetPtSBdphi,2);
    corr->jetPtSBdphi->DrawCopy("same hist");
  }

  c0->cd(4);
  corr->calJetPtPure();
  corr->jetPtPure->Draw();


  // xjg
  c0->cd(5);
  
  handsomeTH1(corr->xAwayWellIso,1);
  handsomeTH1(corr->xsbPhiWellIso,2);
  corr->xAwayWellIso->SetAxisRange(-.02, 0.3,"Y");
  corr->xAwayWellIso->Draw();
  corr->xsbPhiWellIso->Draw("same hist");

  c0->cd(6);
  corr->xPureJetWIpho->SetAxisRange(-.02, 0.3,"Y");
  corr->xPureJetWIpho->Draw();
  
  if ( !isMC) {
    c0->cd(7);
    handsomeTH1(corr->xPureJetWIpho,1);
    handsomeTH1(corr->xPureJetNIpho,2);
    corr->xPureJetWIpho->SetAxisRange(-.02, 0.3,"Y");
    corr->xPureJetWIpho->DrawCopy();
    corr->xPureJetNIpho->DrawCopy(" same hist");
  }
  
  c0->cd(8);
  corr->xFinal->SetAxisRange(-.02, 0.3,"Y");
  
  corr->xFinal->DrawCopy();
  
   c0->SaveAs(Form("detail_%s_icent%d.gif",mcLabel.Data(),icent));


  TCanvas* c1 = new TCanvas(Form("c1_%s_icent%d",mcLabel.Data(),icent),"",1000,700);
  makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.15,0.02);
  //  c1->Divide(3,2);
  c1->cd(1);
  handsomeTH1(corr->jetDphi,1);
  handsomeTH1(corr->jetDphiMix,4);
  corr->jetDphi->SetAxisRange(-0.1,.7,"Y"); 
  corr->jetDphi->Draw();
  corr->jetDphiMix->Draw("same");
  jumSun(0.2,0,3.141592,0);
  
  //  gPad->SetLogy();
  c1->cd(4);
  handsomeTH1(corr->jetDphiPureJ,1);
  corr->jetDphiPureJ->SetAxisRange(-0.1,.7,"Y");
  corr->jetDphiPureJ->DrawCopy();
  jumSun(0.2,0,3.141592,0);
  // gPad->SetLogy();

  c1->cd(2);
  handsomeTH1(corr->jetDphiNIpho,1);
  handsomeTH1(corr->jetDphiMixNIpho,4);
  corr->jetDphiNIpho->SetAxisRange(-0.1,.7,"Y");
  corr->jetDphiNIpho->Draw();
  corr->jetDphiMixNIpho->Draw("same");
  jumSun(0.2,0,3.141592,0);
  //  gPad->SetLogy();
  c1->cd(5);
  handsomeTH1(corr->jetDphiPureJNIpho,color[4]);
  corr->jetDphiPureJNIpho->SetAxisRange(-0.1,.7,"Y");
  corr->jetDphiPureJNIpho->DrawCopy();
  jumSun(0.2,0,3.141592,0);
  //  gPad->SetLogy();

  
  c1->cd(3);
  corr->jetDphiPureJ->SetAxisRange(-0.1,.7,"Y");
  corr->jetDphiPureJ->DrawCopy();
  corr->jetDphiPureJNIpho->DrawCopy("same");
  jumSun(0.2,0,3.141592,0);
  //  gPad->SetLogy();

  c1->cd(6);
  handsomeTH1(corr->jetDphiPureJPurePho,1);
  corr->jetDphiPureJPurePho->SetAxisRange(-0.1,.7,"Y");
  corr->jetDphiPureJPurePho->Draw();
  jumSun(0.2,0,3.141592,0);
  //  gPad->SetLogy();
  
  TFile outf = TFile("gammaJetOutputs.root","update");
  corr->jetDphiPureJPurePho->Write();
  corr->jetDphiPurePho->Write();
  corr->jetDphiPureJ->Write();
  corr->jetDphiPureJNIpho->Write();
  corr->jetDphi->Write();
  corr->jetDphiMix->Write();
  outf.Close();
  
   c1->SaveAs(Form("detailDphi_%s_icent%d.gif",mcLabel.Data(),icent));


  TCanvas* cNull = new TCanvas("cNull","",100,100);
  
}

