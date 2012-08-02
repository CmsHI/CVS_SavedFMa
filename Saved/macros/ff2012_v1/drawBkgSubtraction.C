#include <TRandom3.h>
#include <time.h>
#include "corrFunctionTrk.h"

void drawBkgSubtraction(int cutOpt=6030, int ipt=1, int icent=1) {
  
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int nPtAssoc = cab.getNPtBinAssoc();
 
  vector<double> ptBinPho = cab.getPtBin();
  int lowPtPho = ptBinPho[ipt-1];
  int highPtPho = ptBinPho[ipt];
  
  int nCent_std = cab.getNCentBin();
  vector<double> centBin_std = cab.getCentBin();
  int lowerCent = centBin_std[icent-1];
  int upperCent = centBin_std[icent]-1;
 
    
  TFile* infHist = new TFile(Form("photonTrackOutput_cutOpt%d.root",cutOpt));
  
  TH1D* singleCorr[4][7];

  TString varName="";
  TH1D* hTemp = new TH1D("hTempFinSumPt",";#Sigmap^{ch}_{T}/p_{T}^{#gamma};Entries per photon",2000,0,100);
  handsomeTH1(hTemp,0);
  
  int opt = 5;
  if ( opt == 1 ) {
    varName = "PtAway";
    hTemp->SetXTitle("Track p_{T} (GeV)");
    hTemp->SetYTitle("Entries per photon");
    hTemp->SetAxisRange(0,30,"X");
    hTemp->SetAxisRange(0.0001,1000,"Y");
  }
  else if ( opt == 5) {
    varName = "xiInc05";
    hTemp->SetXTitle("#xi of inclusive tracks");
    hTemp->SetYTitle("dN/d#xi per photon");
    hTemp->SetAxisRange(0,6,"X");
    hTemp->SetAxisRange(-10,200,"Y");
  }


  TString histName = Form("hidata_icent%d_ipt%d_iptAssoc1_%s", (int)icent, (int)ipt, varName.Data());
  singleCorr[kIsoPho][kRawTrk] = (TH1D*)infHist->Get  (Form("%s_phoCand_rawTrkInAllCone", histName.Data()));
  singleCorr[kIsoPho][kHydTrk] = (TH1D*)infHist->Get(Form("%s_phoCand_xTrkInAllCone", histName.Data()));
  singleCorr[kIsoPho][kHardTrk] = (TH1D*)infHist->Get(Form("%s_phoCand_hardTrackInAllCone", histName.Data()));
  singleCorr[kIsoPho][kMBJetTrk] = (TH1D*)infHist->Get(Form("%s_phoCand_rawTrkInMBCone", histName.Data()));
  singleCorr[kIsoPho][kMBxTrk] = (TH1D*)infHist->Get(Form("%s_phoCand_xTrkInMBCone", histName.Data()));
  singleCorr[kIsoPho][kHardMBTrk] =(TH1D*)infHist->Get(Form("%s_phoCand_hardTrackInMBCone", histName.Data()));
  singleCorr[kIsoPho][kFinalGoodTrk] =(TH1D*)infHist->Get(Form("%s_phoCand_hardTrackInJetCone", histName.Data()));

  singleCorr[kDecay][kRawTrk] = (TH1D*)infHist->Get(Form("%s_decayPho_rawTrkInAllCone", histName.Data()));
  singleCorr[kDecay][kHydTrk] = (TH1D*)infHist->Get(Form("%s_decayPho_xTrkInAllCone", histName.Data()));
  singleCorr[kDecay][kHardTrk] = (TH1D*)infHist->Get(Form("%s_decayPho_hardTrackInAllCone", histName.Data()));
  singleCorr[kDecay][kMBJetTrk] = (TH1D*)infHist->Get(Form("%s_decayPho_rawTrkInMBCone", histName.Data()));
  singleCorr[kDecay][kMBxTrk] = (TH1D*)infHist->Get(Form("%s_decayPho_xTrkInMBCone", histName.Data()));
  singleCorr[kDecay][kHardMBTrk] =(TH1D*)infHist->Get(Form("%s_decayPho_hardTrackInMBCone", histName.Data()));
  singleCorr[kDecay][kFinalGoodTrk] =(TH1D*)infHist->Get(Form("%s_decayPho_hardTrackInJetCone", histName.Data()));
  singleCorr[kPurePho][kFinalGoodTrk] =(TH1D*)infHist->Get(Form("%s_purePho_hardTrackInJetCone", histName.Data()));



  
  
  TCanvas* c1 = new TCanvas(Form("%s_cutOpt%d_ipt%d_icent%d_phoCand",varName.Data(),(int)cutOpt,(int)ipt, (int)icent),"",1000,700);
  makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.24,0.15,0.02);
   
  c1->cd(1);
  hTemp->Draw();
  handsomeTH1(singleCorr[kIsoPho][kRawTrk],1);
  handsomeTH1(singleCorr[kIsoPho][kHydTrk],2);
  singleCorr[kIsoPho][kHydTrk]->DrawCopy("same");
  singleCorr[kIsoPho][kRawTrk]->DrawCopy("same");
  drawText("(a)",0.9,0.9,1,15);
  if ( opt != 5) 
    gPad->SetLogy();
  drawText("Raw tracks in Raw Jet", 0.45,0.55,1,15);
  drawText("Bkg tracks in the cone", 0.45,0.47,2,15);

  c1->cd(3);
  if (highPtPho > 200)
    drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.1,0.9,1,15);
  else
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.1,0.9,1,15);
  drawText("|#eta^{#gamma}| <1.44", 0.4,0.9,1,15);
  drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.1,0.82,1,15);
  drawText("|#eta^{jet}| <1.6", 0.4,0.82,1,15);
  drawText("#Delta#phi_{#gamma-jet} > 7#pi/8",0.1,0.74,1,15);
  drawText(Form("%.0f %% - %.0f %%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.1,0.6,1,15);
  drawText("Triggered by #gamma Candidates", 0.1,0.45,1,19);
  drawText("CMS Preliminary",0.3,0.3,1,15);
  drawText("PbPb #int L dt = ~150 #mub^{-1}",0.3,0.2,1,15);


  c1->cd(4);
  hTemp->Draw();
  singleCorr[kIsoPho][kHardTrk]->Draw("same");
  if ( opt != 5)
    gPad->SetLogy();

  drawText("(b)",0.9,0.9,1,15);
  drawText("After Subtraction in (a)", 0.5,0.55,1,15);
  drawText("= Pure tracks from Raw Jet", 0.5,0.47,1,15);
  c1->cd(2);
  hTemp->Draw();
  handsomeTH1(singleCorr[kIsoPho][kMBJetTrk],1);
  handsomeTH1(singleCorr[kIsoPho][kMBxTrk],2);
  singleCorr[kIsoPho][kMBJetTrk]->DrawCopy("same");
  singleCorr[kIsoPho][kMBxTrk]->DrawCopy("same");
  if ( opt != 5)
    gPad->SetLogy();
  drawText("(c)",0.9,0.9,1,15);
  drawText("Raw tracks in Unassoc'ed Jet", 0.25,0.55,1,15);
  drawText("Bkg tracks in the cone", 0.25,0.47,2,15);

  c1->cd(5);
  hTemp->Draw();
  singleCorr[kIsoPho][kHardMBTrk]->DrawCopy("same");
  if ( opt != 5)
  gPad->SetLogy();
  drawText("(d)",0.9,0.9,1,15);
  drawText("After Subtraction in (c)", 0.3,0.55,1,15);
  drawText("= Pure tracks from Unassoc'ed Jet", 0.15,0.47,1,15);

  c1->cd(6);
  hTemp->Draw();
  singleCorr[kIsoPho][kFinalGoodTrk]->DrawCopy("same");
  if ( opt != 5)
 gPad->SetLogy();
  drawText("(e)",0.9,0.9,1,15);
  drawText("(b) - (d)", 0.2,0.75,1,15);
  drawText("= Pure tracks from pure jet", 0.2,0.65,1,15);

  
  c1->SaveAs(Form("%s_cutOpt%d_ipt%d_icent%d_phoCand.pdf",varName.Data(),(int)cutOpt,(int)ipt, (int)icent));

  TCanvas* c2 = new TCanvas(Form("%s_cutOpt%d_ipt%d_icent%d_decayPho",varName.Data(),(int)cutOpt,(int)ipt, (int)icent),"",1000,700);
  makeMultiPanelCanvas(c2,3,2,0.0,0.0,0.24,0.15,0.02);

  c2->cd(1);
  hTemp->Draw();
  handsomeTH1(singleCorr[kDecay][kRawTrk],1);
  handsomeTH1(singleCorr[kDecay][kHydTrk],2);
  singleCorr[kDecay][kHydTrk]->DrawCopy("same");
  singleCorr[kDecay][kRawTrk]->DrawCopy("same");
  drawText("Raw tracks in Raw Jet", 0.45,0.55,1,15);
  drawText("Bkg tracks in the cone", 0.45,0.47,2,15);
  if ( opt != 5)
  gPad->SetLogy();
  drawText("(a)",0.9,0.9,1,15);
  
  c2->cd(3);  
  if (highPtPho > 200)
    drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.1,0.9,1,15);
  else
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.1,0.9,1,15);
  drawText("|#eta^{#gamma}| <1.44", 0.4,0.9,1,15);
  drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.1,0.82,1,15);
  drawText("|#eta^{jet}| <1.6", 0.4,0.82,1,15);
  drawText("#Delta#phi_{#gamma-jet} > 7#pi/8",0.1,0.74,1,15);
  drawText(Form("%.0f %% - %.0f %%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.1,0.6,1,15);
  drawText("Triggered by Decay Photon", 0.1,0.45,1,19);
  drawText("CMS Preliminary",0.3,0.3,1,15);
  drawText("PbPb #int L dt = ~150 #mub^{-1}",0.3,0.2,1,15);



  c2->cd(4);
  hTemp->Draw();
  singleCorr[kDecay][kHardTrk]->Draw("same");
  if ( opt != 5)
  gPad->SetLogy();

  drawText("(b)",0.9,0.9,1,15);
  drawText("After Subtraction in (a)", 0.5,0.55,1,15);
  drawText("= Pure tracks from Raw Jet", 0.5,0.47,1,15);
  c2->cd(2);
  hTemp->Draw();
  handsomeTH1(singleCorr[kDecay][kMBJetTrk],1);
  handsomeTH1(singleCorr[kDecay][kMBxTrk],2);
  singleCorr[kDecay][kMBJetTrk]->DrawCopy("same");
  singleCorr[kDecay][kMBxTrk]->DrawCopy("same");
  if ( opt != 5)
  gPad->SetLogy();
  drawText("(c)",0.9,0.9,1,15);
  drawText("Raw tracks in Unassoc'ed Jet", 0.25,0.55,1,15);
  drawText("Bkg tracks in the cone", 0.25,0.47,2,15);

  c2->cd(5);
  hTemp->Draw();
  singleCorr[kDecay][kHardMBTrk]->DrawCopy("same");
  if ( opt != 5)
  gPad->SetLogy();
  drawText("(d)",0.9,0.9,1,15);
  drawText("After Subtraction in (c)", 0.3,0.55,1,15);
  drawText("= Pure tracks from Unassoc'ed Jet", 0.15,0.47,1,15);

  c2->cd(6);
  hTemp->Draw();
  singleCorr[kDecay][kFinalGoodTrk]->DrawCopy("same");
  if ( opt != 5)
  gPad->SetLogy();
  drawText("(e)",0.9,0.9,1,15);
  drawText("(b) - (d)", 0.2,0.75,1,15);
  drawText("= Pure tracks from pure jet", 0.2,0.65,1,15);

  c2->SaveAs(Form("%s_cutOpt%d_ipt%d_icent%d_decayPho.pdf",varName.Data(),(int)cutOpt,(int)ipt, (int)icent));

  TCanvas* c3 = new TCanvas(Form("%s_cutOpt%d_ipt%d_icent%d_PuritySubtraction",varName.Data(),(int)cutOpt,(int)ipt, (int)icent),"",700,400);
  makeMultiPanelCanvas(c3,2,1,0.0,0.0,0.24,0.15,0.02);
  c3->cd(1);
  TH1D* hTemp1 = (TH1D*)hTemp->Clone("hTemp1");
  handsomeTH1(hTemp1,0);
  hTemp1->SetAxisRange(0,1,"Y");
  hTemp1->Draw();
  handsomeTH1(singleCorr[kDecay][kFinalGoodTrk],2);
  singleCorr[kDecay][kFinalGoodTrk]->DrawCopy("same");
  singleCorr[kIsoPho][kFinalGoodTrk]->DrawCopy("same");
  drawText("pure tracks in pure jets", 0.5,0.5,1,15);
  drawText("triggered by #gamma Cand's", 0.5,0.42,1,15);
  drawText("triggered by Decay #gamma",  0.5,0.34,2,15);

  c3->cd(2);
  handsomeTH1(singleCorr[kPurePho][kFinalGoodTrk],1);
  hTemp1->Draw();
  singleCorr[kPurePho][kFinalGoodTrk]->DrawCopy("same");

  if (highPtPho > 200)
    drawText(Form("p_{T}^{#gamma} > %dGeV", lowPtPho), 0.1,0.9,1,15);
  else
    drawText(Form("p_{T}^{#gamma} : %d - %dGeV", lowPtPho, highPtPho), 0.1,0.9,1,15);
  drawText("|#eta^{#gamma}| <1.44", 0.4,0.9,1,15);
  drawText(Form("p_{T}^{jet} > %dGeV", (int)cab.getJetPtCut()), 0.1,0.82,1,15);
  drawText("|#eta^{jet}| <1.6", 0.4,0.82,1,15);
  drawText("#Delta#phi_{#gamma-jet} > 7#pi/8",0.1,0.74,1,15);
  drawText(Form("%.0f %% - %.0f %%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.1,0.6,1,15);

  drawText("After decay #gamma Subtraction", 0.2,0.5,1,15);


  drawText("CMS Preliminary",0.45,0.4,1,15);
  drawText("PbPb #int L dt = ~150 #mub^{-1}",0.45,0.3,1,15);

  c3->SaveAs(Form("%s_cutOpt%d_ipt%d_icent%d_PurePho.pdf",varName.Data(),(int)cutOpt,(int)ipt, (int)icent));

  



}
