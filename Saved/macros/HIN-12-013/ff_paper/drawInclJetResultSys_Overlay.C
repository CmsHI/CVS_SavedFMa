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

void drawInclJetResultSys_Overlay(int fragMode= 2, // 1=trkpt, 2=ff
  int cmpStyle = 1, // 1=ratio, 2=diff
  int jtrewthi = 1,
  int jtrewtpp = 1,
  bool doEtaRef = true ) 
{ 
  TH1::SetDefaultSumw2();

  //////////////////////////////////////////////////////////////////////
  // Setup
  //////////////////////////////////////////////////////////////////////
  int binMode =2; // 1 : aj, 2 : cent
  TString tag = Form("trkPtProj_binMode2_hi_rewt%d_pp_rewt%d",jtrewthi,jtrewtpp);
  if (cmpStyle==1) tag += "_rat";
  else tag += "_diff";
  if (doEtaRef) tag += "_EtaRef";
  TString outdir="plotsFinalFF";

  //////////////////////////////////////////////////////////////////////
  // Specify Inputs
  //////////////////////////////////////////////////////////////////////
  // std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_hi_ppfitrewt.root";
  // std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_pprewt.root";
  // mc
  std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet.root";
  std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet.root";
  // std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mcrefjetsel.root";
  // std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mcrefjetsel.root";
  // std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet_closure101limjeteta.root";
  // std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet_closure101limjeteta.root";
  int doMC=1;

  std::string Input_QM="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pas.root";


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
  if (doMC) dataset="mc";
  for ( int iaj=1;iaj<=4;iaj++) {
    if ( binMode == 2 ) {
      if (!doEtaRef) ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0_jtrewt0_wtmode0_pt1to300",jname[0].Data(),iaj));
      else {
        ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hi%s_icent%d_irj999_fragMode%d_closure100_jtrewt%d_wtmode0_pt1to300",jname[0].Data(),dataset.Data(),iaj,fragMode,jtrewthi));
      }
      ffpp[ijet][iaj]  = (TH1D*)load(Inputpp_.data(),Form("hpt_%s_sigTrk_pp%s_icent%d_irj999_fragMode%d_closure100_jtrewt%d_wtmode0_pt1to300",jname[0].Data(),dataset.Data(),iaj,fragMode,jtrewtpp));
        // ffpp[ijet][iaj]  = (TH1D*)load(Inputpp_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100_jtrewt0_wtmode0_pt1to300",jname[0].Data(),iaj));
        // QM pp reference
        // ffpp[ijet][iaj]  = (TH1D*)load(Input_QM.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jname[ijet].Data(),iaj));
    } 
    handsomeTH1(ffpp[ijet][iaj],1,1.3);
    handsomeTH1(ffhi[ijet][iaj],1,1.3);
  }
  if (doMC) tag += "_mc80and120__closure101limjeteta_ppunsm";

  /////////////////////////////////////////////////
  // Get Comparison Histograms
  /////////////////////////////////////////////////
  TH1D* ffhicmp[3][5];
  TH1D* ffppcmp[3][5];
  TH1D* ffratiocmp[3][5];
  // QM Comparison
  if (fragMode==2) {
    TString cmpFile = Input_QM.data();
    for ( int iaj=1;iaj<=4;iaj++) {
      ffhicmp[ijet][iaj]  = (TH1D*)load(cmpFile.Data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jname[ijet].Data(),iaj));
      ffppcmp[ijet][iaj]  = (TH1D*)load(cmpFile.Data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jname[ijet].Data(),iaj));
      ffratiocmp[ijet][iaj]  = (TH1D*)ffhicmp[ijet][iaj]->Clone(Form("%s_ratcmp",ffhicmp[ijet][iaj]->GetName()));
      if (cmpStyle==1) ffratiocmp[ijet][iaj]->Divide(ffppcmp[ijet][iaj]);
      else if (cmpStyle==2) ffratiocmp[ijet][iaj]->Add(ffppcmp[ijet][iaj],-1);
      handsomeTH1(ffppcmp[ijet][iaj],kRed,0,0);
      ffppcmp[ijet][iaj]->SetLineStyle(2);
      handsomeTH1(ffhicmp[ijet][iaj],kRed,1.3,kOpenCircle);
      handsomeTH1(ffratiocmp[ijet][iaj],kRed,1.3,kOpenCircle);
    }
  } else if (fragMode==1) {
    GetQM12TrkPt(ffppcmp,ffhicmp);
    for ( int iaj=1;iaj<=4;iaj++) {
      ffratiocmp[1][iaj]  = (TH1D*)ffhicmp[ijet][iaj]->Clone(Form("%s_ratcmp",ffhicmp[ijet][iaj]->GetName()));
      if (cmpStyle==1) ffratiocmp[ijet][iaj]->Divide(ffppcmp[ijet][iaj]);
      else if (cmpStyle==2) ffratiocmp[ijet][iaj]->Add(ffppcmp[ijet][iaj],-1);
      handsomeTH1(ffppcmp[ijet][iaj],kRed,0,0);
      ffppcmp[ijet][iaj]->SetLineStyle(2);
      handsomeTH1(ffhicmp[ijet][iaj],kRed,1.3,kOpenCircle);
      handsomeTH1(ffratiocmp[ijet][iaj],kRed,1.3,kOpenCircle);
    }    
  }

  /////////////////////////////////////////////////
  // Monitor
  /////////////////////////////////////////////////
  //  TFile outf("finalSys.root","recreate");
  //  outf.Close();
  
  //////////////////////////////////////////////////////////////////////
  // Define Plotting Frames
  //////////////////////////////////////////////////////////////////////
  float xmin=-0.15, xmax=5.5, rmin=0, rmax=3.4;
  if (fragMode==1) { xmin=1; xmax=51; }
  if (cmpStyle==2) { rmin=-0.8; rmax=1.8; }
  TH2F * hPadFF, *hPadR, * hPadPt;
  hPadPt = new TH2F("hPadPt",";p_{T}^{ jet} (GeV/c);1/N_{ jet} dN_{ jet} /dp_{T}",150,0,320,100,0.000002,300);
  hPadFF = new TH2F("hPadFF",";#xi = ln(1/z);1/N_{ jet} dN_{ track} /d#xi",100,xmin,xmax,100,0.006,20);
  hPadR = new TH2F("hPadR",";#xi = ln(1/z);PbPb/pp",100,xmin,xmax,20000,rmin,rmax);
  if (cmpStyle==2) hPadR->SetYTitle("PbPb - pp");

  handsomeTH1(hPadFF,1,1.2);
  handsomeTH1(hPadR,1,1.2);
  handsomeTH1(hPadPt,1,1.2);

  fixedFontHist(hPadFF,2.2,2.5,25);
  fixedFontHist(hPadR,2.2,2.5,25);
  fixedFontHist(hPadPt,2.2,2.5,25);

  /////////////////////////////////////////////////
  // Define Systematic Uncertainties
  /////////////////////////////////////////////////
  SysErrors sysff(fragMode,binMode,1,xmin,xmax);

  /////////////////////////////////////////////////
  // Draw Result
  /////////////////////////////////////////////////
  TCanvas *c = new TCanvas("c","",1500,860);
  makeMultiPanelCanvasNew(c,4,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
  for ( int iaj=1 ; iaj<=4 ; iaj++) {
    cout << "=============== " << Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)) << " ================" << endl;

      // Draw Distribution
    c->cd(5-iaj);
    bool doNorm=false;
    if (doNorm) {scaleInt(ffpp[ijet][iaj]); scaleInt(ffhi[ijet][iaj]);}
    if (doNorm) ffpp[ijet][iaj]->SetAxisRange(0.00025,1,"Y");

    // Draw Spectrum
    if(ijet == 1) hPadFF->Draw();
    int iajSys = iaj;
    if (binMode==1) iaj=5-iaj;
    if (fragMode==1) gPad->SetLogx();
    gPad->SetLogy();
    // drawSysErr(ffhi[ijet][iaj],sysff.vErrorHi[ijet][iajSys],sysff.NErrorHi,ijet-1,1,2,1,ijet == 1,0,0,(TPad*)c->GetPad(5-iaj),(TPad*)cerr->GetPad(5-iaj));
    sysff.Combine(ffhi[ijet][iaj],sysff.vErrorHi[ijet][iajSys],sysff.NErrorHi);
    sysff.Apply(ffhi[ijet][iaj]);
    sysff.Draw(ffhi[ijet][iaj]);

    ffppcmp[ijet][iaj]->Draw("hist same");
    ffpp[ijet][iaj]->Draw(   "hist same");
    ffhi[ijet][iaj]->Draw("same");
    ffhicmp[ijet][iaj]->Draw("sameE");
    gPad->RedrawAxis();

    // Draw Comparison
    c->cd(9-iaj);
    if (fragMode==1) gPad->SetLogx();
    ffratio[ijet][iaj]= (TH1D*)ffhi[ijet][iaj]->Clone(Form("ffRattio_ijet%d_iaj%d",ijet,iaj));
    if (cmpStyle==1) ffratio[ijet][iaj]->Divide(ffpp[ijet][iaj]);
    else if (cmpStyle==2) ffratio[ijet][iaj]->Add(ffpp[ijet][iaj],-1);

    if(ijet == 1)hPadR->Draw();
    sysff.Combine(ffhi[ijet][iaj],sysff.vErrorRat[ijet][iajSys],sysff.NErrorRatio);
    sysff.ApplyOnRel(ffhi[ijet][iaj],ffpp[ijet][iaj],cmpStyle);
    sysff.Draw(ffratio[ijet][iaj]);
    ffratiocmp[ijet][iaj]->Draw("sameE");
    ffratio[ijet][iaj]->Draw("same");
    if (cmpStyle==1) jumSun(xmin,1,xmax,1);
    else if (cmpStyle==2) jumSun(xmin,0,xmax,0);
    gPad->RedrawAxis();
    PrintHistogram(ffratio[ijet][iaj]);
  }

  /////////////////////////////////////////////////
  // Annotate
  /////////////////////////////////////////////////
  TH1D* hYel = new TH1D("hYel","",0,10,20);
  handsomeTH1(hYel,TColor::GetColor(0xFFEE00));
  hYel->SetFillColor(TColor::GetColor(0xFFEE00));
  hYel->SetFillStyle(1001);
  TLegend* l2[3];
  TLegend* lsys;
  l2[ijet]  = new TLegend(0.04,0.747,0.77,1.02,NULL,"brNDC");
  easyLeg(l2[ijet],"");
  if (!doMC) {
     // l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb AkPu3Calo","p");
     // l2[ijet]->AddEntry(ffratiocmp[ijet][1],"Jet |#eta|<0.8","p");
     // l2[ijet]->AddEntry(ffhi[ijet][1],"PbPb, 4 Cent. Trk. Eff.","p");
     // l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb, QM12 Trk. Eff.","p");
     // l2[ijet]->AddEntry(ffppcmp[ijet][1],"pp, 100<Jet<300 GeV","l");
    l2[ijet]->AddEntry(ffhi[ijet][1],"PbPb (Full)","p");
    l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb QM12","p");
    l2[ijet]->AddEntry(ffpp[ijet][1],"pp reference","l");
    l2[ijet]->AddEntry(ffppcmp[ijet][1],"pp reference QM12","l");
  } else {
    l2[ijet]->AddEntry(ffhi[ijet][1],"PYTHIA+HYDJET","p");
    l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb QM12","p");
    l2[ijet]->AddEntry(ffpp[ijet][1],"PYTHIA","l");
    l2[ijet]->AddEntry(ffppcmp[ijet][1],"pp reference QM12","l");      
  }
  for ( int iaj=1 ; iaj<=4 ; iaj++) {
    c->cd(5-iaj);
    if (iaj==3 && ijet == 1)  l2[ijet]->Draw();      
    if (iaj==4) {
      drawText("CMS Preliminary",0.27,0.9,1,25); 
      // drawText("L_{Int} = 129 #mub^{-1}",0.27,0.80,1,25);
      if (!doMC) drawText("L_{Int} = 150 #mub^{-1}",0.27,0.80,1,25);
    }
    float legdx = 0.2;
    if (binMode==1&&iaj==4) legdx=+0.15;
    if (binMode==2&&iaj==4) legdx=+0.30;
    if ( binMode == 1 ) {
      drawText(Form("%.2f < x_{2,1} < %.2f", float(rjBin1[iaj-1]), float(rjBin1[iaj])), 0.2+legdx,0.18);
    } else if ( binMode == 2 ) drawText(Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)), 2,0.03,kBlack,25,false);

    if  (iaj==2) {
      lsys = new  TLegend(0.03439587,0.8554279,0.7188686,1.001911,NULL,"brNDC");
      easyLeg(lsys,"");
      lsys->AddEntry(hYel,"Systematic uncertainty","f");
      lsys->Draw();
    }

    if (iaj==1) {
      float ptx(0.08),pty1(0.89);
      drawText("Jet p_{T}  > 100GeV/c, |#eta| < 2",ptx,pty1,kBlack,25);
      drawText("Track p_{T}  > 1 GeV/c, r < 0.3",ptx,pty1-0.09,kBlack,25);
    }
  }
  c->SaveAs(outdir+Form("/FFana%d_%s.gif",fragMode,tag.Data()));
  c->SaveAs(outdir+Form("/FFana%d_%s.pdf",fragMode,tag.Data()));
  c->SaveAs(outdir+Form("/FFana%d_%s.C",fragMode,tag.Data()));

  /////////////////////////////////////////////////
  // Inspect Errors
  /////////////////////////////////////////////////
  TCanvas *c2 = new TCanvas("c2","",1500,430);
  //makeMultiPanelCanvasNew(c,4,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
  c2->Divide(4,1);
  for ( int icent=1 ; icent<=4 ; icent++) {
    c2->cd(5-icent);
    if (fragMode==1) gPad->SetLogx();
    sysff.Combine(ffhi[ijet][icent],sysff.vErrorRat[ijet][icent],sysff.NErrorRatio);
    int doLeg=(icent==4);
    sysff.Inspect(sysff.vErrorRat[ijet][icent],sysff.NErrorRatio,doLeg);
    drawText(Form("%.0f%% - %.0f%%", float(centBin1[icent-1]*2.5), float(centBin1[icent]*2.5)), 2,0.03,kBlack,25,false);
  }
}

