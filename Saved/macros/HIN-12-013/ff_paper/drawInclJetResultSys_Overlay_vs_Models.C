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
#include "HIN-12-013.h"
using namespace std;

void drawInclJetResultSys_Overlay_vs_Models(
  int doCompare=99, // 0=no comparison, 99=all comparison
  int doRescale = 1,
  int fragMode= 2, // 1=trkpt, 2=ff
  int cmpStyle = 1 // 1=ratio, 2=diff
  ) 
{ 
  TH1::SetDefaultSumw2();

  //////////////////////////////////////////////////////////////////////
  // Setup
  //////////////////////////////////////////////////////////////////////
  int ppsm=2;
  int biascorr=11;
  int jtrewthi = 1;
  int jtrewtpp = 1;
  int binMode =2; // 1 : aj, 2 : cent
  bool doEtaRef = true;

  TString tag = Form("compare%d_models_rescale%d_hi_rewt%d_pp_sm%drewt%d_bc%d",doCompare,doRescale,jtrewthi,ppsm,jtrewtpp,biascorr);
  if (cmpStyle==1) tag += "_rat";
  else tag += "_diff";
  TString outdir="plotsFinalFFCompare";

  //////////////////////////////////////////////////////////////////////
  // Specify Inputs
  //////////////////////////////////////////////////////////////////////
  std::string Input_=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_data_mc80to170_hi_pp_corrjbias%d_v4.root",biascorr);
  std::string Inputpp_=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar19job0_data_mc80to170_hi_pp_corrjbias%d_v4.root",biascorr);

  float finalEtaCut=2.;

  TString cmpFile_hi, cmpFile_pp;
  if (doCompare) {
    cmpFile_hi=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04v3job0_mc80to170quenwide_genjetana_hi_pp_corrjbias0_v3.root");
    cmpFile_pp=Form("inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_jdr0.3_Mar04v3job0_data_mc80to170_genjetana_hi_pp_corrjbias0_v3.root");
  }

  /////////////////////////////////////////////////
  // Get Result Histograms
  /////////////////////////////////////////////////
  TFile* inf = new TFile(Input_.data());
  TH1D* ffhi[3][5]; // (leading, subleading),  (aj bin)
  TH1D* ffpp[3][5];
  TH1D* ffratio[3][5];
  // Analysis Histograms
  TString jname[3] = {"jet","lJet","slJet"};
  int ijet=1;
  TString dataset="data"; // "data", "mc"
  for ( int iaj=1;iaj<=4;iaj++) {
    if ( binMode == 2 ) {
      if (!doEtaRef) ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0_jtrewt0_wtmode0_pt1to300",jname[0].Data(),iaj));
      else {
        ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hi%s_icent%d_irj999_fragMode%d_closure100_rewt%d_ppsm0_wtmode0_pt1to300",jname[0].Data(),dataset.Data(),iaj,fragMode,jtrewthi));
      }
      ffpp[ijet][iaj]  = (TH1D*)load(Inputpp_.data(),Form("hpt_%s_sigTrk_pp%s_icent%d_irj999_fragMode%d_closure100_rewt%d_ppsm%d_wtmode0_pt1to300",jname[0].Data(),dataset.Data(),iaj,fragMode,jtrewtpp,ppsm));
    }
    handsomeTH1(ffpp[ijet][iaj],1,1.3);
    handsomeTH1(ffhi[ijet][iaj],1,1.3);
  }

  /////////////////////////////////////////////////
  // Get Comparison Histograms
  /////////////////////////////////////////////////
  TH1D* ffhicmp[3][5];
  TH1D* ffppcmp[3][5];
  TH1D* ffratiocmp[3][5];
  if (doCompare) {
    for ( int iaj=1;iaj<=4;iaj++) {
      ffhicmp[ijet][iaj]  = (TH1D*)load(cmpFile_hi.Data(),Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure101_rewt0_ppsm0_wtmode0_pt1to300",iaj,fragMode));
      ffppcmp[ijet][iaj]  = (TH1D*)load(cmpFile_pp.Data(),Form("hpt_jet_sigTrk_ppmc_icent%d_irj999_fragMode%d_closure101_rewt0_ppsm%d_wtmode0_pt1to300",iaj,fragMode,ppsm));
      ffratiocmp[ijet][iaj]  = (TH1D*)ffhicmp[ijet][iaj]->Clone(Form("%s_ratcmp",ffhicmp[ijet][iaj]->GetName()));
      if (cmpStyle==1) ffratiocmp[ijet][iaj]->Divide(ffppcmp[ijet][iaj]);
      else if (cmpStyle==2) ffratiocmp[ijet][iaj]->Add(ffppcmp[ijet][iaj],-1);
      handsomeTH1(ffppcmp[ijet][iaj],kRed,0,0);
      ffppcmp[ijet][iaj]->SetLineStyle(2);
      handsomeTH1(ffhicmp[ijet][iaj],kRed,1.2,kOpenCircle);
      handsomeTH1(ffratiocmp[ijet][iaj],kRed,1.2,kOpenCircle);
    }
  }

  TGraphErrors * gwied = new TGraphErrors("wiedemann_ff_mod_ratio_2005.txt","%lg %lg");
  gwied->SetMarkerStyle(kOpenSquare);
  gwied->SetMarkerSize(1.2);
  gwied->SetMarkerColor(kBlue);
  gwied->SetLineColor(kBlue);
  TGraphErrors * gwiedrescaled = new TGraphErrors("wiedemann_ff_mod_ratio_2005_rescaled.txt","%lg %lg");
  gwiedrescaled->SetMarkerStyle(kOpenCircle);
  gwiedrescaled->SetMarkerSize(1.2);
  gwiedrescaled->SetMarkerColor(kBlue);
  gwiedrescaled->SetLineColor(kBlue);

  // TGraphErrors * gqpy = new TGraphErrors("armesto_salgado_ff_mod_ratio_2008.txt","%lg %lg");
  TGraphErrors * gqpy = new TGraphErrors("qpythia_ff_mod_ratio_qhat1_L5_2009.txt","%lg %lg");
  gqpy->SetMarkerStyle(kOpenStar);
  gqpy->SetMarkerSize(1.2);
  gqpy->SetMarkerColor(kGreen+2);
  gqpy->SetLineColor(kGreen+2);

  TGraphErrors * gmaj_quark = new TGraphErrors("majumder_magenta_circle.txt","%lg %lg");
  gmaj_quark->SetMarkerStyle(kOpenTriangleUp);
  gmaj_quark->SetMarkerSize(1.2);
  gmaj_quark->SetMarkerColor(kMagenta+2);
  gmaj_quark->SetLineColor(kMagenta+2);

  TGraphErrors * gmaj_gluon = new TGraphErrors("majumder_blue_square.txt","%lg %lg");
  gmaj_gluon->SetMarkerStyle(32);
  gmaj_gluon->SetMarkerSize(1.2);
  gmaj_gluon->SetMarkerColor(kOrange+2);
  gmaj_gluon->SetLineColor(kOrange+2);

  //////////////////////////////////////////////////////////////////////
  // Define Plotting Frames
  //////////////////////////////////////////////////////////////////////
  float xmin=-0.15, xmax=5.5, rmin=0.2, rmax=3.;
  if (fragMode==1) { xmin=1; xmax=51; }
  if (cmpStyle==2) { rmin=-0.8; rmax=1.8; }
  TH2F * hPadFF, *hPadR, * hPadPt;
  hPadPt = new TH2F("hPadPt",";p_{T}^{ jet} (GeV/c);1/N_{ jet} dN_{ jet} /dp_{T}",150,xmin,xmax,100,6e-3,20);
  hPadFF = new TH2F("hPadFF",";#xi = ln(1/z);1/N_{ jet} dN_{ track} /d#xi",100,xmin,xmax,100,0.02,20);
  hPadR = new TH2F("hPadR",";#xi = ln(1/z);PbPb/pp",100,xmin,xmax,20000,rmin,rmax);
  if (fragMode==1) hPadR->SetXTitle("p_{T}^{Track} (GeV/c)");
  if (cmpStyle==2) hPadR->SetYTitle("PbPb - pp");

  handsomeTH1(hPadFF,1,1.2);
  handsomeTH1(hPadR,1,1.2);
  handsomeTH1(hPadPt,1,1.2);

  fixedFontHist(hPadFF,2.2,2.5,25);
  fixedFontHist(hPadR,1.,1.,25);
  fixedFontHist(hPadPt,2.2,2.5,25);

  /////////////////////////////////////////////////
  // Define Systematic Uncertainties
  /////////////////////////////////////////////////
  SysErrors sysff(fragMode,binMode,1,xmin,xmax);

  /////////////////////////////////////////////////
  // Draw Result
  /////////////////////////////////////////////////
  TCanvas *c = new TCanvas("c","",800,500);
  int iaj=1;
  int iajSys = iaj;
  cout << "=============== " << Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)) << " ================" << endl;

  // Draw Comparison
  if (fragMode==1) gPad->SetLogx();
  ffratio[ijet][iaj]= (TH1D*)ffhi[ijet][iaj]->Clone(Form("ffRattio_ijet%d_iaj%d",ijet,iaj));
  handsomeTH1(ffratio[ijet][iaj],kBlack,1.3);
  if (cmpStyle==1) ffratio[ijet][iaj]->Divide(ffpp[ijet][iaj]);
  else if (cmpStyle==2) ffratio[ijet][iaj]->Add(ffpp[ijet][iaj],-1);

  if(ijet == 1)hPadR->Draw();
  sysff.Combine(ffhi[ijet][iaj],sysff.vErrorRat[ijet][iajSys],sysff.NErrorRatio);
  sysff.ApplyOnRel(ffhi[ijet][iaj],ffpp[ijet][iaj],cmpStyle);
  sysff.Draw(ffratio[ijet][iaj]);
  if (doCompare==1 || doCompare>=99) ffratiocmp[ijet][4]->Draw("sameE");
  if (doCompare==2 || doCompare>=99) gwied->Draw("p");
  if (doCompare==2 && doRescale) gwiedrescaled->Draw("p");
  if (doCompare==3 || doCompare>=99) gqpy->Draw("p");
  if (doCompare==4 || doCompare>=99) gmaj_quark->Draw("p");
  if (doCompare==5 || doCompare>=99) gmaj_gluon->Draw("p");
  ffratio[ijet][iaj]->Draw("same");


  if (cmpStyle==1) jumSun(xmin,1,xmax,1);
  else if (cmpStyle==2) jumSun(xmin,0,xmax,0);
  gPad->RedrawAxis();
  PrintHistogram(ffratio[ijet][iaj]);

  /////////////////////////////////////////////////
  // Annotate
  /////////////////////////////////////////////////
  TH1D* hYel = new TH1D("hYel","",0,10,20);
  handsomeTH1(hYel,TColor::GetColor(0xFFEE00));
  hYel->SetFillColor(TColor::GetColor(0xFFEE00));
  hYel->SetFillStyle(1001);
  TLegend* l2[3];
  TLegend* lsys;
  if (fragMode==2) l2[ijet]  = new TLegend(0.2,0.59,0.42,0.98,NULL,"brNDC");
  else if (fragMode==1) l2[ijet]  = new TLegend(0.193,0.71,0.93,0.88,NULL,"brNDC");
  easyLeg(l2[ijet],"",18);
  // easyLeg2(l2[ijet],0.04);
  l2[ijet]->AddEntry(ffratio[ijet][1],"PbPb 0-10%, p_{T, Jet}  > 100 GeV/c","p");
  if (doCompare==1 || doCompare>=99) l2[ijet]->AddEntry(ffratiocmp[ijet][4],"Pyquen^{Wide}, p_{T, Jet}  > 100 GeV/c","p");
  if (doCompare>=2 || doCompare>=99) l2[ijet]->AddEntry(ffratio[ijet][1],"E_{Jet} = 100 GeV/c","");
  if (doCompare==2 || doCompare>=99) l2[ijet]->AddEntry(gwied,"Borghini, Wiedemann, f_{med} = 0.8","p");
  if (doCompare==2 && doRescale)     l2[ijet]->AddEntry(gwiedrescaled,"Borghini, Wiedemann E_{Jet} #rightarrow 0.7E_{Jet}","p");
  // if (doCompare>=3) l2[ijet]->AddEntry(gqpy,"Armesto, Cunqueiro, Salgado E_{Jet} = 100 GeV/c","p");
  if (doCompare==3 || doCompare>=99) l2[ijet]->AddEntry(gqpy,"Q-PYTHIA (L=5, #hat{q}=1 GeV^{2}/fm)","p");
  if (doCompare==4 || doCompare>=99) l2[ijet]->AddEntry(gmaj_quark,"Majumder Quark (L=5, #hat{q}=1 GeV^{2}/fm)","p");
  if (doCompare==5 || doCompare>=99) l2[ijet]->AddEntry(gmaj_gluon,"Majumder Gluon (L=5, #hat{q}=1 GeV^{2}/fm)","p");
  l2[ijet]->Draw();
  drawText("CMS Preliminary",0.71,0.88,1,25); 
  drawText("L_{Int} = 150 #mub^{-1}",0.71,0.82,1,22);
  // drawText(Form("0%% - 10%%"), 0.71,0.82,kBlack,27,false);

  // Save
  c->SaveAs(outdir+Form("/FFana%d_%s.gif",fragMode,tag.Data()));
  c->SaveAs(outdir+Form("/FFana%d_%s.pdf",fragMode,tag.Data()));
  c->SaveAs(outdir+Form("/FFana%d_%s.C",fragMode,tag.Data()));
}

