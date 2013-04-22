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

void drawInclJetResultSys_Overlay_0to30(int fragMode= 2, // 1=trkpt, 2=ff
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
  int doCompare=0;
  bool draw2010 = true;

  TString tag = Form("trkPtProj_binMode2_hi_rewt%d_pp_rewt%d",jtrewthi,jtrewtpp);
  if (cmpStyle==1) tag += "_rat";
  else tag += "_diff";
  if (doEtaRef) tag += "_EtaRef";
  if (doCompare==1) tag+="_vs_qm12";
  TString outdir="plotsFinalFF";

  //////////////////////////////////////////////////////////////////////
  // Specify Inputs
  //////////////////////////////////////////////////////////////////////
  std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_hi_pprewt.root";
  std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17data_hi_pprewt.root";
  // mc
  // std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet.root";
  // std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet.root";
  // std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mcrefjetsel.root";
  // std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mcrefjetsel.root";
  // std::string Input_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet_closure101limjeteta.root";
  // std::string Inputpp_="inclJetFF_output_trackPtCut1_FinalJetPt100to300eta2.00_Jan17mc80and120_hi_ppunsmjet_closure101limjeteta.root";
  int doMC=0;

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
  if (doMC) tag += "_mc80and120";

  /////////////////////////////////////////////////
  // Get Comparison Histograms
  /////////////////////////////////////////////////
  TH1D* ffhicmp[3][5];
  TH1D* ffppcmp[3][5];
  TH1D* ffratiocmp[3][5];
  if (doCompare) {
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
  hPadPt = new TH2F("hPadPt",";p_{T}^{ jet} (GeV/c);1/N_{ jet} dN_{ jet} /dp_{T}",150,xmin,xmax,100,8e-3,20);
  hPadFF = new TH2F("hPadFF",";#xi = ln(1/z);1/N_{ jet} dN_{ track} /d#xi",100,xmin,xmax,100,0.02,20);
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
  TCanvas *c = new TCanvas("c","",500,500);
  // TCanvas *c = new TCanvas("c","",1500,860);
  // makeMultiPanelCanvasNew(c,4,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
  hPadR->Draw();

  // drawSysErr(ffratio[ijet][iaj],vError[ijet][iajSys],Nerror,ijet-1,1,2,1,ijet == 1,0,0,0,0,TColor::GetColor(0xFFEE00));
  // if (draw2010)   drawSysErr(ffratio[ijet][2],vError[ijet][2],Nerror,ijet-1,1,2,1,ijet == 1,0,0,0,0,kGreen-3);
  // handsomeTH1(ffratio[ijet][2],1);
  ffratio[ijet][2]->SetMarkerStyle(25);
  ffratio[ijet][1]->Draw("same");
  if (draw2010)  ffratio[ijet][2]->Draw("same");

  gPad->RedrawAxis();
  jumSun(0,1,6,1);
  drawText("CMS Preliminary",0.2,0.88,1,25); 
  drawText("L_{Int} = 140 #mub^{-1}",0.6,0.88,1,25);
//  drawText(Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)),0.55,2.35,kBlack,25,false);
  float ptx(0.08),pty1(0.89);
  drawText("Jet p_{T}  > 100GeV/c",0.21,0.2,kBlack,25);
  if (!draw2010)   drawText("Track p_{T}  > 1 GeV/c, r < 0.3",0.2,0.69,kBlack,25);

//   TH1D* hYel = new TH1D("hYel","",0,10,20);
//   handsomeTH1(hYel,TColor::GetColor(0xFFEE00));
//   hYel->SetFillColor(TColor::GetColor(0xFFEE00));
//   hYel->SetFillStyle(1001);
//   TLegend* l2[3];
//   TLegend* lsys;
//  lsys = new  TLegend(0.03439587,0.8554279,0.7188686,1.001911,NULL,"brNDC");
//  easyLeg(lsys,"");
//  lsys->AddEntry(hYel,"Systematic uncertainty","f");
//  lsys->Draw();
  
  // Save Plot
  //////// 2010 0-10% leading jet xi
  TH1D *xsin4_1_0_R = new TH1D("xsin4_1_0_R","",20,-1,9);
  xsin4_1_0_R->SetBinContent(3,1.20505);
  xsin4_1_0_R->SetBinContent(4,1.040817);
  xsin4_1_0_R->SetBinContent(5,0.8804085);
  xsin4_1_0_R->SetBinContent(6,0.850761);
  xsin4_1_0_R->SetBinContent(7,0.8131551);
  xsin4_1_0_R->SetBinContent(8,0.7969801);
  xsin4_1_0_R->SetBinContent(9,0.8939498);
  xsin4_1_0_R->SetBinContent(10,1.098302);
  xsin4_1_0_R->SetBinContent(11,0.7099872);
  xsin4_1_0_R->SetBinError(3,0.2486326);
  xsin4_1_0_R->SetBinError(4,0.07325915);
  xsin4_1_0_R->SetBinError(5,0.03967401);
  xsin4_1_0_R->SetBinError(6,0.02982589);
  xsin4_1_0_R->SetBinError(7,0.02485484);
  xsin4_1_0_R->SetBinError(8,0.02288269);
  xsin4_1_0_R->SetBinError(9,0.02875025);
  xsin4_1_0_R->SetBinError(10,0.09731627);
  xsin4_1_0_R->SetBinError(11,0.3331674);
  handsomeTH1(xsin4_1_0_R,kRed,1.3,24);
  xsin4_1_0_R->SetLineWidth(2);
  xsin4_1_0_R->Draw("sameE");
  //////// end of 2010 0-10% leading jet xi
  ffratio[ijet][1]->Draw("same");    
  
  TGraph *graph1 = new TGraph(5);
  graph1->SetName("");
  graph1->SetTitle("");
  graph1->SetFillColor(90);
  graph1->SetMarkerStyle(20);
  graph1->SetMarkerSize(1.3);

  int ient = 0;
  //  graph1->SetPoint(ient,0,0.827905485);  ient++;
  graph1->SetPoint(ient,0,0.7941284219);  ient++;
  graph1->SetPoint(ient,0,1.615971456);  ient++;
  graph1->SetPoint(ient,0.5,1.615971456);  ient++;
  graph1->SetPoint(ient,0.5,1.30570635);  ient++;
  graph1->SetPoint(ient,1,1.30570635);  ient++;
  graph1->SetPoint(ient,1,1.054754209);  ient++;
  graph1->SetPoint(ient,1.5,1.054754209);  ient++;
  graph1->SetPoint(ient,1.5,0.99417728);  ient++;
  graph1->SetPoint(ient,2,0.99417728);  ient++;
  graph1->SetPoint(ient,2,0.9416682292);  ient++;
  graph1->SetPoint(ient,2.5,0.9416682292);  ient++;
  graph1->SetPoint(ient,2.5,0.9237305945);  ient++;
  graph1->SetPoint(ient,3,0.9237305945);  ient++;
  graph1->SetPoint(ient,3,1.0476436);  ient++;
  graph1->SetPoint(ient,3.5,1.0476436);  ient++;
  graph1->SetPoint(ient,3.5,1.322757561);  ient++;
  graph1->SetPoint(ient,4,1.322757561);  ient++;
  graph1->SetPoint(ient,4,0.8974300895);  ient++;
  graph1->SetPoint(ient,4.5,0.8974300895);  ient++;
 


  // half way!                                                                                                                 
  TGraph *graph2 = new TGraph(5);
  graph2->SetName("");
  graph2->SetTitle("");
  graph2->SetFillColor(90);
  graph2->SetMarkerStyle(20);
  graph2->SetMarkerSize(1.3);

  ient = 0;
  graph2->SetPoint(ient,4.5,0.8974300895);  ient++;
  graph2->SetPoint(ient,4.5,0.5225442278); ient++; 
  graph2->SetPoint(ient,4,0.5225442278); ient++;
  graph2->SetPoint(ient,4,0.8738474167); ient++;
  graph2->SetPoint(ient,3.5,0.8738474167); ient++;
  graph2->SetPoint(ient,3.5,0.7402560788); ient++;
  graph2->SetPoint(ient,3,0.7402560788); ient++;
  graph2->SetPoint(ient,3,0.670229673); ient++;
  graph2->SetPoint(ient,2.5,0.670229673); ient++;
  graph2->SetPoint(ient,2.5,0.6846419401); ient++;
  graph2->SetPoint(ient,2,0.6846419401); ient++;
  graph2->SetPoint(ient,2,0.7073446889); ient++;
  graph2->SetPoint(ient,1.5,0.7073446889); ient++;
  graph2->SetPoint(ient,1.5,0.7060627085); ient++;
  graph2->SetPoint(ient,1,0.7060627085); ient++;
  graph2->SetPoint(ient,1,0.7759279165); ient++;
  graph2->SetPoint(ient,0.5,0.7759279165); ient++;
  graph2->SetPoint(ient,0.5,0.7941284219); ient++;
  graph2->SetPoint(ient,0,0.7941284219); ient++;

  graph1->SetLineWidth(2);
  graph2->SetLineWidth(2);
  graph1->SetLineColor(2);
  graph2->SetLineColor(2);
  if (draw2010) {
    graph1->Draw("l");
    graph2->Draw("l");
  }
  
  TH1D* hdum = new TH1D("hdum","",10,0,10);
  handsomeTH1(hdum,1);
  hdum->SetFillColor(0);
  hdum->SetLineColor(2);
  hdum->SetLineWidth(2);
  hdum->SetMarkerColor(2);
  hdum->SetMarkerStyle(24);
  hdum->SetFillStyle(1000);

  TH1D* hdum2 = new TH1D("hdum2","",10,0,10);
  handsomeTH1(hdum2,1);
  hdum2->SetFillColor(TColor::GetColor(0xFFEE00));
  hdum2->SetLineColor(TColor::GetColor(0xFFEE00));
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
  if (draw2010)  { 
    l2->AddEntry(hdum, "2010, 0-30%,  Leading jet","pf");
    l2->AddEntry(hdum2,"2011, 0-10%,  Inclusive jet","pf");
    l2->AddEntry(hdum3,"2011, 10-30%,Inclusive jet","pf");
  }
  else  {
    l2->AddEntry(hdum2,"0-10%","plf");
  }
  l2->Draw();
  

  c->SaveAs(outdir+Form("/FF_ratio_ijet%d_%s_0to10and30_with2010%d.gif",ijet,tag.Data(),draw2010));
  c->SaveAs(outdir+Form("/FF_ratio_ijet%d_%s_0to10and30_with2010%d.pdf",ijet,tag.Data(),draw2010));
  c->SaveAs(outdir+Form("/FF_ratio_ijet%d_%s_0to10and30_with2010%d.C",ijet,tag.Data(),draw2010));

  // /////////////////////////////////////////////////
  // // Annotate
  // /////////////////////////////////////////////////
  // TH1D* hYel = new TH1D("hYel","",0,10,20);
  // handsomeTH1(hYel,TColor::GetColor(0xFFEE00));
  // hYel->SetFillColor(TColor::GetColor(0xFFEE00));
  // hYel->SetFillStyle(1001);
  // TLegend* l2[3];
  // TLegend* lsys;
  // l2[ijet]  = new TLegend(0.04,0.747,0.77,1.02,NULL,"brNDC");
  // easyLeg(l2[ijet],"");
  // if (!doMC) {
  //    // l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb AkPu3Calo","p");
  //    // l2[ijet]->AddEntry(ffratiocmp[ijet][1],"Jet |#eta|<0.8","p");
  //    // l2[ijet]->AddEntry(ffhi[ijet][1],"PbPb, 4 Cent. Trk. Eff.","p");
  //    // l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb, QM12 Trk. Eff.","p");
  //    // l2[ijet]->AddEntry(ffppcmp[ijet][1],"pp, 100<Jet<300 GeV","l");
  //   l2[ijet]->AddEntry(ffhi[ijet][1],"PbPb","p");
  //   if (doCompare) l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb QM12","p");
  //   l2[ijet]->AddEntry(ffpp[ijet][1],"pp reference","l");
  //   if (doCompare) l2[ijet]->AddEntry(ffppcmp[ijet][1],"pp reference QM12","l");
  // } else {
  //   l2[ijet]->AddEntry(ffhi[ijet][1],"PYTHIA+HYDJET","p");
  //   if (doCompare) l2[ijet]->AddEntry(ffratiocmp[ijet][1],"PbPb QM12","p");
  //   l2[ijet]->AddEntry(ffpp[ijet][1],"PYTHIA","l");
  //   if (doCompare) l2[ijet]->AddEntry(ffppcmp[ijet][1],"pp reference QM12","l");      
  // }
  // for ( int iaj=1 ; iaj<=4 ; iaj++) {
  //   c->cd(5-iaj);
  //   if (iaj==3 && ijet == 1)  l2[ijet]->Draw();      
  //   if (iaj==4) {
  //     drawText("CMS Preliminary",0.27,0.9,1,25); 
  //     // drawText("L_{Int} = 129 #mub^{-1}",0.27,0.80,1,25);
  //     if (!doMC) drawText("L_{Int} = 150 #mub^{-1}",0.27,0.80,1,25);
  //   }
  //   float legdx = 0.2;
  //   if (binMode==1&&iaj==4) legdx=+0.15;
  //   if (binMode==2&&iaj==4) legdx=+0.30;
  //   if ( binMode == 1 ) {
  //     drawText(Form("%.2f < x_{2,1} < %.2f", float(rjBin1[iaj-1]), float(rjBin1[iaj])), 0.2+legdx,0.18);
  //   } else if ( binMode == 2 ) drawText(Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)), 2,0.03,kBlack,25,false);

  //   if  (iaj==2) {
  //     lsys = new  TLegend(0.03439587,0.8554279,0.7188686,1.001911,NULL,"brNDC");
  //     easyLeg(lsys,"");
  //     lsys->AddEntry(hYel,"Systematic uncertainty","f");
  //     lsys->Draw();
  //   }

  //   if (iaj==1) {
  //     float ptx(0.08),pty1(0.89);
  //     drawText("Jet p_{T}  > 100GeV/c, |#eta| < 2",ptx,pty1,kBlack,25);
  //     drawText("Track p_{T}  > 1 GeV/c, r < 0.3",ptx,pty1-0.09,kBlack,25);
  //   }
  // }
  // c->SaveAs(outdir+Form("/FFana%d_%s.gif",fragMode,tag.Data()));
  // c->SaveAs(outdir+Form("/FFana%d_%s.pdf",fragMode,tag.Data()));
  // c->SaveAs(outdir+Form("/FFana%d_%s.C",fragMode,tag.Data()));

  // /////////////////////////////////////////////////
  // // Inspect Errors
  // /////////////////////////////////////////////////
  // TCanvas *c2 = new TCanvas("c2","",1500,430);
  // //makeMultiPanelCanvasNew(c,4,2,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
  // c2->Divide(4,1);
  // for ( int icent=1 ; icent<=4 ; icent++) {
  //   c2->cd(5-icent);
  //   if (fragMode==1) gPad->SetLogx();
  //   sysff.Combine(ffhi[ijet][icent],sysff.vErrorRat[ijet][icent],sysff.NErrorRatio);
  //   int doLeg=(icent==4);
  //   sysff.Inspect(sysff.vErrorRat[ijet][icent],sysff.NErrorRatio,doLeg);
  //   drawText(Form("%.0f%% - %.0f%%", float(centBin1[icent-1]*2.5), float(centBin1[icent]*2.5)), 2,0.6,kBlack,25,false);
  // }
  // c2->SaveAs(outdir+Form("/sysana%d_%s.gif",fragMode,tag.Data()));
  // c2->SaveAs(outdir+Form("/sysana%d_%s.pdf",fragMode,tag.Data()));
}

