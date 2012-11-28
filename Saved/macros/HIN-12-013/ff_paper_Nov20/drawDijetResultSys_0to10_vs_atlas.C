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

void combineSysErrors(TH1 * he1[3][5], TH1 * he2[3][5]) {
}

void drawDijetResultSys_0to10_vs_atlas(int binMode =2, // 1 : aj, 2 : cent
			      bool doEtaRef = true,
			      bool draw2010 = true) 
{  

  TH1::SetDefaultSumw2();
//   std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pfJet.root";
  std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_pas.root";
//   std::string Input_="dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_org.root";
  int refMode = 100; //0 reweight + smear, -1 no rewt, -2 no smear, -3 raw
  TString tag = Form("trkPtProj_binMode%d_ref%d",binMode,refMode);
  if (doEtaRef) tag += "_EtaRef";
  tag+=Form("_approval");
  TString outdir="plotsFinalFF";
  
  float xmin=-0.15, xmax=5.5;

  // Systematics
  const int Nerror = 6;
  const int NerrorHiFF = 8;
  const int NerrorPp = 6;

  TH1* jesError[3][5];
  TH1* jetResError[3][5];
  TH1* bkgSubError[3][5];
  TH1* trkError[3][5];
  TH1* genBkgSubError[3][5];
  TH1* jetAlgoError[3][5];
  TH1* constError[3][5];

  TH1* pyquenJetRecoRefPtError[3][5];
  TH1* pyquenGenJetSelError[3][5];

  TH1* vError[3][5][Nerror];
  TH1* vErrorHiFF[3][5][NerrorHiFF];
  TH1* vErrorPpFF[3][5][NerrorPp];

  // For Ratio
  const char* JESfile = "./systematics/jes_sys.root";
  const char* JetResfile = "./systematics/jetres_sys.root";
  const char* BkgSubfile = "./systematics/bkgsub_sys.root";
  const char* TrkClosurefile = "./systematics/closure_sys.root";
  const char* GenBkgSubfile = "./systematics/closure_bkgsub_sys.root";
  const char* JetAlgofile = "./systematics/systematics_calovspf.root";
  // For FF distribution
  const char* PyquenJetRecoRefPtfile = "./systematics/PYQUEN80_useGenJet0_closureTestRatio_between_100_103.root";
  const char* PyquenGenJetSelfile = "./systematics/closureTestRatio_pyquen80_genJet_recojet_selectionRatio.root";

  TString jname[3] = {"","lJet","slJet"};
  for ( int ijet=1; ijet<=1; ijet++){
    for ( int iaj=1;iaj<=4;iaj++) {
      if (binMode==1) {
        jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0_sys2",jname[ijet].Data(),iaj));
        jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0",jname[ijet].Data(),iaj));
        bkgSubError[ijet][iaj] = loadError(BkgSubfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure100",jname[ijet].Data(),iaj));
        trkError[ijet][iaj] = loadError(TrkClosurefile,Form("hpt_%s_sigTrk_himc_icent999_iaj%d_fragMode2_closure101",jname[ijet].Data(),iaj));
      } else if (binMode==2) {
        jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_%s_sigTrk_hidata_icent2_irj999_fragMode2_closure100_sys7",jname[ijet].Data(),iaj));
        jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
        bkgSubError[ijet][iaj] =  loadError(BkgSubfile,Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0",jname[ijet].Data(),iaj));

        trkError[ijet][iaj] =  loadError(TrkClosurefile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));

        genBkgSubError[ijet][iaj] =  loadError(GenBkgSubfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure102",jname[ijet].Data(),iaj));
        
        // For FF distribution
        pyquenJetRecoRefPtError[ijet][iaj] = loadError(PyquenJetRecoRefPtfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure103",jname[ijet].Data(),iaj));
        pyquenGenJetSelError[ijet][iaj] = loadError(PyquenGenJetSelfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
      }
      
      // additional track error to be flat 10%, add another 6% from pp reweightin sqrt(10*10+6*6)= 11.7
      constError[ijet][iaj] =  (TH1D*)jesError[ijet][iaj]->Clone(Form("%s_constsys",jesError[ijet][iaj]->GetName()));
      for(int i = 1; i <= constError[ijet][iaj]->GetNbinsX(); ++i){
        constError[ijet][iaj]->SetBinContent(i,1.117);
        constError[ijet][iaj]->SetBinError(i,0.1);
        constError[ijet][iaj]->SetBinError(i,0.1);
      }

      // Ratio
      vError[ijet][iaj][0] = jesError[ijet][iaj];
      vError[ijet][iaj][1] = jetResError[ijet][iaj];
      vError[ijet][iaj][2] = bkgSubError[ijet][iaj];
      vError[ijet][iaj][3] = trkError[ijet][iaj];
      vError[ijet][iaj][4] = constError[ijet][iaj];
      vError[ijet][iaj][5] = genBkgSubError[ijet][iaj];

      // HiFF
      vErrorHiFF[ijet][iaj][0] = jesError[ijet][iaj];
      vErrorHiFF[ijet][iaj][1] = jetResError[ijet][iaj];
      vErrorHiFF[ijet][iaj][2] = bkgSubError[ijet][iaj];
      vErrorHiFF[ijet][iaj][3] = trkError[ijet][iaj];
      vErrorHiFF[ijet][iaj][4] = constError[ijet][iaj];
      vErrorHiFF[ijet][iaj][5] = genBkgSubError[ijet][iaj];
      vErrorHiFF[ijet][iaj][6] = pyquenJetRecoRefPtError[ijet][iaj];
      vErrorHiFF[ijet][iaj][7] = pyquenGenJetSelError[ijet][iaj];
    }
  }
  
  TH2F * hPadFF, *hPadR, * hPadPt;;

  hPadFF = new TH2F("hPadFF",";#xi = ln(1/z);1/N_{ jet} dN_{ track} /d#xi",100,xmin,xmax,100,0.006,20);
  hPadPt = new TH2F("hPadPt",";p_{T}^{ jet} (GeV/c);1/N_{ jet} dN_{ jet} /dp_{T}",150,0,320,100,0.000002,300);
//   hPadR = new TH2F("hPadR",";#xi = ln(1/z);PbPb/pp",100,xmin,xmax,20000,0,3.4);
  hPadR = new TH2F("hPadR",";#xi = ln(1/z);Central/Peripheral",100,xmin,xmax,20000,0,3.4);

  handsomeTH1(hPadFF,1,1.2);
  handsomeTH1(hPadR,1,1.2);
  handsomeTH1(hPadPt,1,1.2);

  fixedFontHist(hPadFF,2.2,2.5,25);
  fixedFontHist(hPadR,1.1,1.1,25);
  fixedFontHist(hPadPt,2.2,2.5,25);

//    TCanvas * cerr = new TCanvas("cerr","cerr",4*300,300);
//    cerr->Divide(4,1);

//   TCanvas *c = new TCanvas("c","",1500,760);
//   makeMultiPanelCanvasNew(c,4,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
  TCanvas *c = new TCanvas("c","",500,500);
//   makeMultiPanelCanvasNew(c,4,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);

  /////////////////////////////////////////////////
  // Get Histogram
  /////////////////////////////////////////////////
  TFile* inf = new TFile(Input_.data());
  TH1D* ffhi[3][5]; // (leading, subleading),  (aj bin)
  TH1D* ffpp[3][5];
  TH1D* ffratio[3][5];
  // Analysis Histograms
  for ( int ijet=1; ijet<=1; ijet++){ 
    for ( int iaj=1;iaj<=4;iaj++) {
      if      ( binMode == 1 ) {
        if (!doEtaRef) ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent999_irj%d_fragMode2_closure0",jname[ijet].Data(),iaj));
        else ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent999_irj%d_fragMode2_closure100",jname[ijet].Data(),iaj));
        ffpp[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_ppdata_icent999_irj%d_fragMode2_closure%d",jname[ijet].Data(),iaj,refMode));
      }
      else if ( binMode == 2 ) {
//       	if (!doEtaRef) ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0",jname[ijet].Data(),iaj));
//       	else ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
//         ffpp[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure%d",jname[ijet].Data(),iaj,refMode));
      	if (!doEtaRef) ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0_wtmode0_pt1to500",jname[ijet].Data(),iaj));
      	else ffhi[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jname[ijet].Data(),iaj));
        ffpp[ijet][iaj]  = (TH1D*)load(Input_.data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure%d_wtmode0_pt1to500",jname[ijet].Data(),iaj,refMode));
      } 
  
      handsomeTH1(ffpp[ijet][iaj],1,1.3);
      handsomeTH1(ffhi[ijet][iaj],1,1.3);

    }
  }

  // Compare Histograms
  TH1D* ffhicmp[3][5];
  TH1D* ffppcmp[3][5];
  TH1D* ffratiocmp[3][5];
//   TString cmpFile = "PbPbPP_ratio_dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut2.00_calo.root";
  TString cmpFile = "dijetFF_output_histograms_trkPtProjectOnJetAxis_trackPtCut1_FinaletaCut0.80_pas.root";
  for ( int ijet=1; ijet<=1; ijet++){ 
    for ( int iaj=1;iaj<=4;iaj++) {
//       ffratiocmp[ijet][iaj] = (TH1D*)load(cmpFile.Data(),Form("ffRattio_ijet1_irj%d",iaj));
      ffhicmp[ijet][iaj]  = (TH1D*)load(cmpFile.Data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jname[ijet].Data(),iaj));
      ffppcmp[ijet][iaj]  = (TH1D*)load(cmpFile.Data(),Form("hpt_%s_sigTrk_ppdata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500",jname[ijet].Data(),iaj));
      ffratiocmp[ijet][iaj]  = (TH1D*)ffhicmp[ijet][iaj]->Clone(Form("%s_ratcmp",ffhicmp[ijet][iaj]->GetName()));
      ffratiocmp[ijet][iaj]->Divide(ffppcmp[ijet][iaj]);
      handsomeTH1(ffratiocmp[ijet][iaj],kBlue,1.3,kOpenCircle);
    }
  }
  

  /////////////////////////////////////////////////
  // Draw Result
  /////////////////////////////////////////////////
  int ijet =1; // inclusive jet
  int iaj = 1; // central
  int iajSys = iaj;
  if (binMode==1) iaj=5-iaj;
  cout << "=============== " << Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)) << " ================" << endl;
  // Draw Distribution
//   hPadFF->Draw();
//   gPad->SetLogy();      
//   drawSysErr(ffhi[ijet][iaj],vErrorHiFF[ijet][iajSys],NerrorHiFF,ijet-1,1,2,1,ijet == 1,0,0,(TPad*)c->GetPad(5-iaj),(TPad*)cerr->GetPad(5-iaj));
//   ffpp[ijet][iaj]->Draw(   "hist same");
//   ffhi[ijet][iaj]->Draw("same");
//   gPad->RedrawAxis();

  // Draw Ratio
  for ( int iaj1= 1 ; iaj1<=4 ; iaj1++) {
  ffratio[ijet][iaj1]= (TH1D*)ffhi[ijet][iaj1]->Clone(Form("ffRattio_ijet%d_iaj%d",ijet,iaj1));
//   ffratio[ijet][iaj1]->Divide(ffpp[ijet][iaj1]);
  // rcp
  ffratio[ijet][iaj1]->Divide(ffhi[ijet][4]);
  ffratio[ijet][iaj1]->SetAxisRange(0,3,"Y");
  ffratio[ijet][iaj1]->SetAxisRange(0,5.8,"X");
  }

//       drawSysErr(ffratiocmp[ijet][iaj],vError[ijet][iajSys],Nerror,ijet-1,1,2,1,0,0,0);
  //ffratiocmp[ijet][iaj]->Draw("sameE");

  //   PrintHistogram(ffratio[ijet][iaj]);
  
  /////////////////////////////////////////////////
  // DRAW
  /////////////////////////////////////////////////

  hPadR->Draw();

  drawSysErr(ffratio[ijet][iaj],vError[ijet][iajSys],Nerror,ijet-1,1,2,1,ijet == 1,0,0,0,0,TColor::GetColor(0xFFEE00));
//   if (draw2010)   drawSysErr(ffratio[ijet][2],vError[ijet][2],Nerror,ijet-1,1,2,1,ijet == 1,0,0,0,0,kGreen-3);
//   handsomeTH1(ffratio[ijet][2],1);
//   ffratio[ijet][2]->SetMarkerStyle(25);

  ffratio[ijet][iaj]->Draw("same");
//   if (draw2010)  ffratio[ijet][2]->Draw("same");


  
  gPad->RedrawAxis();
  jumSun(0,1,6,1);
  drawText("CMS Preliminary",0.2,0.88,1,25); 
	drawText("L_{Int} = 140 #mub^{-1}",0.6,0.88,1,25);
// 	drawText(Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)),0.55,2.35,kBlack,25,false);
  float ptx(0.08),pty1(0.89);
  drawText("Jet p_{T}  > 100GeV/c",0.21,0.2,kBlack,25);
  if (!draw2010)   drawText("Track p_{T}  > 1 GeV/c, r < 0.3",0.2,0.69,kBlack,25);

//   TH1D* hYel = new TH1D("hYel","",0,10,20);
//   handsomeTH1(hYel,TColor::GetColor(0xFFEE00));
//   hYel->SetFillColor(TColor::GetColor(0xFFEE00));
//   hYel->SetFillStyle(1001);
//   TLegend* l2[3];
//   TLegend* lsys;
// 	lsys = new  TLegend(0.03439587,0.8554279,0.7188686,1.001911,NULL,"brNDC");
// 	easyLeg(lsys,"");
// 	lsys->AddEntry(hYel,"Systematic uncertainty","f");
// 	lsys->Draw();
	
	// Save Plot
  //////// atlas
  TGraphErrors * g = new TGraphErrors("atlas.txt","%lg %lg %lg");
  g->SetMarkerStyle(kFullCircle);
  g->SetMarkerSize(1.2);
  g->SetMarkerColor(kRed);
  g->SetLineColor(kRed);
  g->Draw("samep");
  ffratio[ijet][iaj]->Draw("same");
  
  TH1D* hdum = new TH1D("hdum","",10,0,10);
  handsomeTH1(hdum,1);
  hdum->SetFillColor(0);
  hdum->SetLineColor(2);
  hdum->SetLineWidth(2);
  hdum->SetMarkerColor(2);
  hdum->SetMarkerStyle(kFullCircle);
  hdum->SetMarkerSize(1.2);
  hdum->SetFillStyle(1000);

  TH1D* hdum2 = new TH1D("hdum2","",10,0,10);
  handsomeTH1(hdum2,1);
  hdum2->SetFillColor(TColor::GetColor(0xFFEE00));
  hdum2->SetLineColor(TColor::GetColor(0xFFEE00));
  hdum2->SetMarkerSize(1.2);
  hdum2->SetLineStyle(1);
  hdum2->SetFillStyle(1001);

  TH1D* hdum3 = new TH1D("hdum3","",10,0,10);
  handsomeTH1(hdum3,1);
  hdum3->SetFillColor(kGreen-3);
  hdum3->SetLineColor(kGreen+4);
  hdum3->SetMarkerStyle(25);
  hdum3->SetLineStyle(1);
  hdum3->SetFillStyle(1001);

  

  TLegend* l2;
  l2 = new TLegend(0.1834677,0.720339,0.75,0.8983051,NULL,"brNDC");
  if (!draw2010) l2 =  new TLegend(0.1814516,0.7966102,0.7479839,0.9237288,NULL,"brNDC");

  
  easyLeg(l2,"");
//   l2->AddEntry(hdum2,"CMS, 0-10%/pp","pf");
  l2->AddEntry(hdum2,"CMS, 0-10%/50-100%","pf");
  l2->AddEntry(hdum, "ATLAS, 0-10%/60-80%","p");
  l2->Draw();
  

  c->SaveAs(outdir+Form("/FF_ratio_ijet%d_%s_0to10and30rcp_withatlas%d.gif",ijet,tag.Data(),draw2010));
  c->SaveAs(outdir+Form("/FF_ratio_ijet%d_%s_0to10and30rcp_withatlas%d.pdf",ijet,tag.Data(),draw2010));
  c->SaveAs(outdir+Form("/FF_ratio_ijet%d_%s_0to10and30rcp_withatlas%d.C",ijet,tag.Data(),draw2010));
}

 
