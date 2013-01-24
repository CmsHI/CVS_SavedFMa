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

void drawFinalSys() 
{  
  TH1::SetDefaultSumw2();
    
  const int Nerror = 6;
  TH1* vError[3][5][Nerror];
  TH1* vErrorTot[3][5];
  TF1* vFit[3][5][Nerror];
  
  TString tag="_approval";
  TString outdir="plotsFinalFF";

//   TString infname("finalSys.root");
  TString infname("finalSysFFRatio.root");
  TFile * inf = new TFile(infname);
  TString jname[3] = {"","lJet","slJet"};
  for ( int ijet=1; ijet<=1; ijet++){
    for ( int iaj=1;iaj<=4;iaj++) {
      vError[ijet][iaj][0] = loadError(infname.Data(),Form("hpt_%s_sigTrk_hidata_icent2_irj999_fragMode2_closure100_sys7",jname[ijet].Data(),iaj));
      vError[ijet][iaj][1] = loadError(infname.Data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
      vError[ijet][iaj][2] = loadError(infname.Data(),Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0",jname[ijet].Data(),iaj));
      vError[ijet][iaj][3] = loadError(infname.Data(),Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
      vError[ijet][iaj][4] = loadError(infname.Data(),Form("hpt_%s_sigTrk_hidata_icent2_irj999_fragMode2_closure100_sys7_constsys",jname[ijet].Data(),iaj));
      vError[ijet][iaj][5] = loadError(infname.Data(),Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure102",jname[ijet].Data(),iaj));
      if (Nerror>6) {
        vError[ijet][iaj][6] = loadError(infname.Data(),Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure103",jname[ijet].Data(),iaj));
        vError[ijet][iaj][7] = loadError(infname.Data(),Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
      }
      vErrorTot[ijet][iaj] = loadError(infname.Data(),Form("hpt_lJet_sigTrk_hidata_icent%d_irj999_fragMode2_closure100_tot",iaj));
      for (int ie = 0; ie<Nerror; ++ie) {
//         TString fitname = Form("hpt_lJet_sigTrk_hidata_icent%d_irj999_fragMode2_closure100_wtmode0_pt1to500_fit1_%d",iaj,ie);
        TString fitname = Form("ffRattio_ijet1_iaj%d_fit1_%d",iaj,ie);
        vFit[ijet][iaj][ie] = (TF1*)inf->Get(fitname);
        if (!vFit[ijet][iaj][ie]) {
          cout << "Bad input TF1 " << fitname << endl;
          exit(1);
        }
      }
    }
  }
  
  float xmin=-0.05, xmax=5.5;
  TH2F * hPadFF;
  hPadFF = new TH2F("hPadFF",";#xi = ln(1/z);1 + Systematic Uncertainty",100,xmin,xmax,100,0.4,1.9);
  handsomeTH1(hPadFF,1,1.2);
  fixedFontHist(hPadFF,1.5,1.5,25);
  TLine * l1 = new TLine(xmin,1,xmax,1);
  l1->SetLineStyle(2);
  TCanvas *c = new TCanvas("c","",1500,500);
  makeMultiPanelCanvasNew(c,4,1,0.0,0.0,0.22,0.22,0.01,1.0,0.95);
  
  /////////////////////////////////////////////////
  // Draw Result
  /////////////////////////////////////////////////
  for ( int ijet=1; ijet>=1 ; ijet--) {
    for ( int iaj=1 ; iaj<=4 ; iaj++) {
      cout << "=============== " << Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)) << " ================" << endl;
      
      // Draw Distribution
      c->cd(5-iaj);
      hPadFF->Draw();
      for (int ie = 0; ie<Nerror; ++ie) {
        l1->Draw("same");
        vFit[ijet][iaj][ie]->SetLineStyle(1);
        vFit[ijet][iaj][ie]->Draw("same");
        vErrorTot[ijet][iaj]->Draw("same hist");
      }
    }
  }

  /////////////////////////////////////////////////
  // Annotate
  /////////////////////////////////////////////////
  TLegend* l2[3], *l3[3];
  for ( int ijet=1; ijet>=1 ; ijet--) {
    l2[ijet]  = new TLegend(0.014,0.7,0.77,1,NULL,"brNDC");
    easyLeg(l2[ijet],"",18);
    l2[ijet]->AddEntry(vFit[1][1][0],"Jet Energy Scale 7%","l");
    l2[ijet]->AddEntry(vFit[1][1][1],"Jet Energy Resolution 10-15%","l");
    l2[ijet]->AddEntry(vFit[1][1][2],"Bkg Sub Methods","l");
    l2[ijet]->AddEntry(vFit[1][1][3],"Tracking Closure","l");

    l3[ijet]  = new TLegend(0.014,0.75,0.77,1,NULL,"brNDC");
    easyLeg(l3[ijet],"",18);
    l3[ijet]->AddEntry(vFit[1][1][4],"Tracking Eff. Uncert. in Data","l");
    l3[ijet]->AddEntry(vFit[1][1][5],"Gen. Level Bkg Sub","l");
    l3[ijet]->AddEntry(vErrorTot[1][1],"Total","l");
    
    for ( int iaj=1 ; iaj<=4 ; iaj++) {
      c->cd(5-iaj);
      if (iaj==3)  l2[ijet]->Draw();      
      if (iaj==2)  l3[ijet]->Draw();      
      float legdx = 0.4;
      if (iaj==4) legdx=+0.30;
      drawText(Form("%.0f%% - %.0f%%", float(centBin1[iaj-1]*2.5), float(centBin1[iaj]*2.5)), 0.2+legdx,0.26,kBlack,24);
    }
  }
  c->SaveAs(outdir+Form("/FF_distribution_sys%s.gif",tag.Data()));
  c->SaveAs(outdir+Form("/FF_distribution_sys%s.pdf",tag.Data()));
}

 
