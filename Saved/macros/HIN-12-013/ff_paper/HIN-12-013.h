#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <time.h>
#include <cassert>
#include "CutAndBinCollection2011.h"
#include "HisMath.C"
using namespace std;

class Plot4x4 {
public:
  // Members
  TH1D * h[3][5], * href[3][5], * hrat[3][5];
  TLegend * l1, *l2;
  int ijet;
  static TString title1;
  static TString title2;
  static TString outpath;
  TString outname;

  // Functions
  Plot4x4(TH1D* hnum[3][5], TH1D* hden[3][5], int normMode=0, TString on="pt")
  : ijet(1)
  , outname(on) {
    // Get Histograms
    for ( int icent=1; icent<=4 ; icent++) {
      h[ijet][icent]    = hnum[ijet][icent];
      href[ijet][icent] = hden[ijet][icent];
      assert(h[ijet][icent]);
      assert(href[ijet][icent]);
      normHist(h[ijet][icent],normMode,true);
      normHist(href[ijet][icent],normMode,true);
      handsomeTH1(h[ijet][icent],kRed,1,kFullCircle);
      handsomeTH1(href[ijet][icent],kBlack,1,kOpenCircle);
      h[ijet][icent]->SetNdivisions(505);
      href[ijet][icent]->SetNdivisions(505);
      fixedFontHist(h[ijet][icent],1.8,2.5);
      fixedFontHist(href[ijet][icent],1.6,2.5);
      // Get Ratio
      hrat[ijet][icent]  = (TH1D*)h[ijet][icent]->Clone(Form("%s_ratio",h[ijet][icent]->GetName()));
      hrat[ijet][icent]->Divide(href[ijet][icent]);
    }
  }

  void Draw(TCanvas *c1, float xmin=-1, float xmax=-1, int doLog=1, float ymin=-1, float ymax=-1, int cycle=0) {
    // Set Ranges
    if (xmin==xmax) {
      xmin = h[ijet][1]->GetBinLowEdge(1);
      xmax = h[ijet][1]->GetBinLowEdge(h[ijet][1]->GetNbinsX()+1);
    }
    if (ymin==ymax) ymin=5e-6;
    for ( int icent=1; icent<=4 ; icent++) {
      if (ymax<0) {
        float currmax = getCleverRange(h[ijet][icent])*4;
        if (currmax>ymax) ymax=currmax;
      }
    }
    // Draw
    for ( int icent=1; icent<=4 ; icent++) {
      // Draw Distributions
      c1->cd(5-icent);
      if (doLog>=1) gPad->SetLogy();
      if (doLog>=2) gPad->SetLogx();
      if (cycle==0) {
        h[ijet][icent]->SetAxisRange(xmin,xmax,"X");
        h[ijet][icent]->SetAxisRange(ymin,ymax,"Y");
        h[ijet][icent]->DrawCopy("E");
      } else {
        h[ijet][icent]->DrawCopy("same E");
      }
      href[ijet][icent]->DrawCopy("same hist");
      // Draw Ratios
      c1->cd(9-icent);
      if (doLog>=2) gPad->SetLogx();
      hrat[ijet][icent]->SetAxisRange(xmin,xmax,"X");
      hrat[ijet][icent]->SetAxisRange(0.,2.,"Y");
      fixedFontHist(hrat[ijet][icent],1.8,2.5);
      hrat[ijet][icent]->DrawCopy();
      TLine * hline = new TLine(xmin,1,xmax,1);
      hline->SetLineStyle(2);
      hline->Draw();      
    }
  }

  void DrawLeg(TCanvas *c1, int idraw=3, float x1=0.02,float y1=0.03, float x2=0.50, float y2=0.32) {
    // Book Legend
    l1 = new TLegend(0.23,0.06,0.66,0.27,NULL,"brNDC");
    easyLeg(l1);
    l1->AddEntry(h[ijet][1],"|#eta_{jet}| < 2","");
    l2 = new TLegend(x1,y1,x2,y2,NULL,"brNDC");
    easyLeg(l2,"",20);
    // Annotate
    for ( int icent=1; icent<=4 ; icent++) {
      c1->cd(5-icent);  
      if ( icent == 4) {
        drawText("CMS Preliminary",0.3,0.89,1,20);
        l1->Draw();
      }
      if ( icent == idraw) {
        l2->AddEntry(h[ijet][icent],title1,"p");
        l2->AddEntry(href[ijet][icent],title2,"l");
        l2->Draw();
      }
      c1->cd(9-icent);
      int lowCent = centBin1[icent-1];
      int highCent = centBin1[icent]-1;
      if ( icent ==4 ) 
        drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.35,0.25,kBlack,20);
      else
        drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.15,0.25,kBlack,20);
    }

    c1->SaveAs(outpath+outname+".gif");
    c1->SaveAs(outpath+outname+".pdf");

      // All Done, write output  
    TFile outf = TFile(outpath+".root","update");
    for ( int icent=1; icent<=4 ; icent++) {
      hrat[ijet][icent]->Write();
    }
    outf.Close();
  }
};

TString Plot4x4::title1="PbPb"; // These are necessary for root o_o
TString Plot4x4::title2="pp";
TString Plot4x4::outpath="closure/mc";





#define PI 3.141592653589
static const int MAXTRK = 30000;

// binning
// const int nPtBin = 22;
// double ptBin[nPtBin+1] = { 0,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4,5,6,8,10,12,14,16,20,25,50};
const int nPtBin = 27;
double ptBin[nPtBin+1] = {0, 0.5, 1, 1.203915, 1.449412, 1.74497, 2.100796, 2.529181, 3.04492, 3.665826, 4.413344, 5.313293, 6.396755, 7.701152, 9.271536, 11.16214, 13.43828, 16.17855, 19.47761, 23.44939, 28.23108, 33.98783, 40.91848, 49.26238, 59.30774, 71.40151, 85.96137, 103.4902}; 
// int sysMarkerColor[20] = {kGray+2,kBlue,kAzure+3,kSpring-7,kCyan+3,kGreen+2,kOrange-3,kOrange+2,kRed,kYellow-3,kPink+4};
int sysMarkerColor[20] = {kGray+3,kRed,kBlue,kGreen+2,kOrange-3,kOrange+2,kYellow-3,kPink+4,1,1,1,1};

class SysErrors {
public:
  // setup
  int NErrorRatio;
  int NErrorHi;
  int NErrorPp;
  int ana;
  int binMode;
  int doFit;
  float xmin,xmax;

  // Systematics Uncertainty Inputs
  static const int NErrorTot = 8;
  vector<TString> sysTitle;

  TH1* jesError[3][5];
  TH1* jetResError[3][5];
  TH1* bkgSubError[3][5];
  TH1* trkError[3][5];
  TH1* genBkgSubError[3][5];
  TH1* jetAlgoError[3][5];
  TH1* constError[3][5];

  TH1* pyquenJetRecoRefPtError[3][5];
  TH1* pyquenGenJetSelError[3][5];

  TH1* vErrorRat[3][5][NErrorTot];
  TH1* vErrorHi[3][5][NErrorTot];
  TH1* vErrorPp[3][5][NErrorTot];

  // Systematic Uncertainty Results
  TF1 * fe[NErrorTot];
  TH1D * hErrorTot;
  TH1D * hHigh, * hLow;


  SysErrors(int myana=0, int bm=2, int fit=1, float myxmin=-1, float myxmax=-1) // ana: 0=ff, 1=trkpt
  : ana(myana),binMode(bm),doFit(fit)
  , xmin(myxmin), xmax(myxmax)
  {

    TString jname[3] = {"jet","lJet","slJet"};
    int ijet=1;

      // For Ratio
      // // QM12
      // const char* JESfile = "./systematics/jes_sys.root";
      // const char* JetResfile = "./systematics/jetres_sys.root";
      // const char* BkgSubfile = "./systematics/bkgsub_sys.root";
      // const char* TrkClosurefile = "./systematics/closure_sys.root";
      // const char* GenBkgSubfile = "./systematics/closure_bkgsub_sys.root";
      // const char* JetAlgofile = "./systematics/systematics_calovspf.root";
      // ff paper
    // const char* JESfile = "./systematics/data_sysjes.root";
    // const char* JESfile = "./systematics/data_sysjes1.07.root";
    const char* JESfile = "./systematics/data_sysjes0.935exptra.root";
    const char* JetResfile = "./systematics/data_sysjer.root";
    const char* BkgSubfile = "./systematics/bkgsub_sys.root";
    const char* TrkClosurefile = "./systematics/mctrk.root";
    const char* GenBkgSubfile = "./systematics/mcgenp.root";
    const char* JetAlgofile = "./systematics/systematics_calovspf.root";
      // For FF distribution
    const char* PyquenJetRecoRefPtfile = "./systematics/PYQUEN80_useGenJet0_closureTestRatio_between_100_103.root";
    const char* PyquenGenJetSelfile = "./systematics/closureTestRatio_pyquen80_genJet_recojet_selectionRatio.root";

    for ( int iaj=1;iaj<=4;iaj++) {
      if (binMode==1) {
        jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0_sys2",jname[ijet].Data(),iaj));
        jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0",jname[ijet].Data(),iaj));
        bkgSubError[ijet][iaj] = loadError(BkgSubfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure100",jname[ijet].Data(),iaj)); // from event mixing
        trkError[ijet][iaj] = loadError(TrkClosurefile,Form("hpt_%s_sigTrk_himc_icent999_iaj%d_fragMode2_closure101",jname[ijet].Data(),iaj));
      } else if (binMode==2) {
        // jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_%s_sigTrk_hidata_icent2_irj999_fragMode2_closure100_sys7",jname[ijet].Data(),iaj));
        jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_jet_sigTrk_hidata_icent%d_irj999_fragMode%d_closure100_jtrewt1_wtmode0_pt1to300_ratio",iaj,ana));
        // jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
        jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_jet_sigTrk_hidata_icent%d_irj999_fragMode%d_closure100_jtrewt0_wtmode0_pt1to300_ratio",iaj,ana));
        bkgSubError[ijet][iaj] =  loadError(BkgSubfile,Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0",jname[ijet].Data(),iaj));

        // trkError[ijet][iaj] =  loadError(TrkClosurefile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
        trkError[ijet][iaj] =  loadError(TrkClosurefile,Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure100_jtrewt0_wtmode0_pt1to300_ratio",iaj,ana));

        // genBkgSubError[ijet][iaj] =  loadError(GenBkgSubfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure102",jname[ijet].Data(),iaj));
        genBkgSubError[ijet][iaj] =  loadError(GenBkgSubfile,Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure102_jtrewt0_wtmode0_pt1to300_ratio",iaj,ana));


            // For FF distribution
        pyquenJetRecoRefPtError[ijet][iaj] = loadError(PyquenJetRecoRefPtfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure103",jname[ijet].Data(),iaj));
        pyquenGenJetSelError[ijet][iaj] = loadError(PyquenGenJetSelfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
      }
    }

    // observable specific
    if (ana==2) {
      NErrorRatio=6; NErrorHi=8; NErrorPp=8;
      for ( int iaj=1;iaj<=4;iaj++) {
        // additional track error to be flat 10%
        constError[ijet][iaj] =  (TH1D*)jesError[ijet][iaj]->Clone(Form("%s_constsys",jesError[ijet][iaj]->GetName()));
        for(int i = 1; i <= constError[ijet][iaj]->GetNbinsX(); ++i){
          constError[ijet][iaj]->SetBinContent(i,1.1);
          constError[ijet][iaj]->SetBinError(i,0.01);
        }

        // Ratio
        vErrorRat[ijet][iaj][0] = genBkgSubError[ijet][iaj];
        vErrorRat[ijet][iaj][1] = trkError[ijet][iaj];
        vErrorRat[ijet][iaj][2] = constError[ijet][iaj];
        vErrorRat[ijet][iaj][3] = bkgSubError[ijet][iaj];
        vErrorRat[ijet][iaj][4] = jesError[ijet][iaj];
        vErrorRat[ijet][iaj][5] = jetResError[ijet][iaj];

        // HiFF
        vErrorHi[ijet][iaj][0] = genBkgSubError[ijet][iaj];
        vErrorHi[ijet][iaj][1] = trkError[ijet][iaj];
        vErrorHi[ijet][iaj][2] = bkgSubError[ijet][iaj];
        vErrorHi[ijet][iaj][3] = constError[ijet][iaj];
        vErrorHi[ijet][iaj][4] = jesError[ijet][iaj];
        vErrorHi[ijet][iaj][5] = jetResError[ijet][iaj];
        vErrorHi[ijet][iaj][6] = pyquenJetRecoRefPtError[ijet][iaj];
        vErrorHi[ijet][iaj][7] = pyquenGenJetSelError[ijet][iaj];
      } // iaj
      sysTitle.push_back("Genp. Bkg. Subtr.");
      sysTitle.push_back("Tracking Closure");
      sysTitle.push_back("Data Driven Charged Fraction");
      sysTitle.push_back("Data Driven Bkg. Subt.");
      sysTitle.push_back("JES");
      sysTitle.push_back("JER");
      sysTitle.push_back("Pyquen JES");
      sysTitle.push_back("Pyquen Jet Selection");
    } else if (ana==1) { // trk pt analysis
      NErrorRatio=6; NErrorHi=6; NErrorPp=6;
      for ( int iaj=1;iaj<=4;iaj++) {
        // additional track error to be flat 10%, add another 6% from pp reweightin sqrt(10*10+6*6)= 11.7
        constError[ijet][iaj] =  new TH1D(Form("sysfpt_constsys_%d",iaj),";track p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}",nPtBin,ptBin);
        for(int i = 1; i <= constError[ijet][iaj]->GetNbinsX(); ++i){
          constError[ijet][iaj]->SetBinContent(i,1.1);
          constError[ijet][iaj]->SetBinError(i,0.01);
        }
        // translate event mixing bkgsub error from xi
        TH1* hBkgSubData = (TH1*)constError[ijet][iaj]->Clone(Form("sysfpt_bkgsubdata_%d",iaj));
        hBkgSubData->Reset();
        for(int i = 1; i <= hBkgSubData->GetNbinsX(); ++i){
          float meanJetPt = 121; // mean in data
          float xi = TMath::Log(meanJetPt/hBkgSubData->GetBinCenter(i));
          float xibin = bkgSubError[ijet][iaj]->FindBin(xi);
          // cout << "pt: " << hBkgSubData->GetBinCenter(i) << " xi: " << xi << " xibin: " << xibin << " y: " << bkgSubError[ijet][iaj]->GetBinContent(xibin) << endl;
          if (xibin<1) xibin=1;
          hBkgSubData->SetBinContent(i,bkgSubError[ijet][iaj]->GetBinContent(xibin));
          hBkgSubData->SetBinError(i,bkgSubError[ijet][iaj]->GetBinError(xibin));
        }


        // Ratio
        vErrorRat[ijet][iaj][0] = genBkgSubError[ijet][iaj];
        vErrorRat[ijet][iaj][1] = trkError[ijet][iaj];
        vErrorRat[ijet][iaj][2] = constError[ijet][iaj];
        vErrorRat[ijet][iaj][3] = hBkgSubData;
        vErrorRat[ijet][iaj][4] = jesError[ijet][iaj];
        vErrorRat[ijet][iaj][5] = jetResError[ijet][iaj];

        // HiFF
        vErrorHi[ijet][iaj][0] = genBkgSubError[ijet][iaj];
        vErrorHi[ijet][iaj][1] = trkError[ijet][iaj];
        vErrorHi[ijet][iaj][2] = constError[ijet][iaj];
        vErrorHi[ijet][iaj][3] = hBkgSubData;
        vErrorHi[ijet][iaj][4] = jesError[ijet][iaj];
        vErrorHi[ijet][iaj][5] = jetResError[ijet][iaj];
      } // iaj
      sysTitle.push_back("Genp. Bkg. Subtr.");
      sysTitle.push_back("Tracking Closure");
      sysTitle.push_back("Data Driven Charged Fraction");
      sysTitle.push_back("Data Driven Bkg. Subt.");
      sysTitle.push_back("JES");
      sysTitle.push_back("JER");
    } // end of if ana
    // Setup Boundaries
    if (xmin==xmax) {
      xmin=vErrorHi[ijet][1][0]->GetBinLowEdge(1);
      xmax=vErrorHi[ijet][1][0]->GetBinLowEdge(vErrorHi[ijet][1][0]->GetNbinsX()+1);
    }
  }

  void Combine(TH1* h, TH1** he, int Nerror) {
    // allocate memory for final error
    hErrorTot = (TH1D*)h->Clone(Form("%s_toterr",h->GetName()));
    hErrorTot->Reset();

    // fit errors
    for(int ie = 0; ie< Nerror; ++ie){
      if (ana==1) {
        fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol4",xmin,xmax*2);
        if (ie==1||ie==3) he[ie]->Fit(fe[ie],"QNRll","",xmin,xmax*1.5);
        else he[ie]->Fit(fe[ie],"QNRw","",xmin,xmax*1.5);
      } else if (ana==2) {
        if (ie==0) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",0,6);
        else if (ie==4) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",0,6);
        else if (ie<6) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",0,5.5);
        else fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol4",0,5.5);
        if (ie==0) he[ie]->Fit(fe[ie],"QRNw",0,5.5);
        else if (ie==4) he[ie]->Fit(fe[ie],"QRNll","",0.5,5.5);
        else he[ie]->Fit(fe[ie],"QRN","",0,5.5);
        // if (ie==0) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",xmin,xmax);
        // else if (ie<6) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",xmin,xmax);
        // else fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol4",xmin,xmax);
        // he[ie]->Fit(fe[ie],"QRNw","",0,5);
      }
    }

    // combine errors
    for(int i = 1; i <=hErrorTot->GetNbinsX() ; ++i){
      // conform all error input binning to the final result binning
      double x0 = hErrorTot->GetBinCenter(i);
      // Add Errors in Quadrature
      double tote=0;
      for(int ie = 0; ie< Nerror; ++ie){
        double xbin = he[ie]->FindBin(x0);
        double ec=0,sign=1;
        if (doFit) ec = fe[ie]->Eval(x0) - 1.;
        else       ec = he[ie]->GetBinContent(xbin) - 1.;
        tote+= pow(ec,2);
      }
      tote=sqrt(tote);
      hErrorTot->SetBinContent(i,tote);
      cout << "total error: " << i << ": " << hErrorTot->GetBinContent(i) << endl;
    }
  }

  void Apply(TH1* h) {
    hHigh = (TH1D*)h->Clone(Form("%s_high",h->GetName()));
    hLow = (TH1D*)h->Clone(Form("%s_low",h->GetName()));
    for (int i=1; i<=h->GetNbinsX(); ++i) {
      double y=h->GetBinContent(i);
      double e=hErrorTot->GetBinContent(i);
      hHigh->SetBinContent(i,y*(1+e));
      hLow->SetBinContent(i,y*(1-e));
    }
  }

  void ApplyOnRel(TH1* h, TH1* h2, int cmpStyle=1) {
    hHigh = (TH1D*)h->Clone(Form("%s_highrat",h->GetName()));
    hLow = (TH1D*)h->Clone(Form("%s_lowrat",h->GetName()));
    for (int i=1; i<=h->GetNbinsX(); ++i) {
      double y=h->GetBinContent(i);
      double e=hErrorTot->GetBinContent(i);
      hHigh->SetBinContent(i,y*(1+e));
      hLow->SetBinContent(i,y*(1-e));
    }
    if (cmpStyle==1) {
      hHigh->Divide(h2);
      hLow->Divide(h2);
    } else if (cmpStyle==2) {
      hHigh->Add(h2,-1);
      hLow->Add(h2,-1);
    }
  }

  void Draw(TH1* h, int theColor=TColor::GetColor(0xFFEE00), int theStyle=1001) {
    for ( int i=1 ; i<= h->GetNbinsX(); i++) {
      if (h->GetBinCenter(i)<xmin||h->GetBinCenter(i)>xmax) continue;
      float yhigh = hHigh->GetBinContent(i);
      float ylow = hLow->GetBinContent(i);
      // if (ylow<0.01) ylow=0.01;
      TBox* tt = new TBox(h->GetBinLowEdge(i), ylow , h->GetBinLowEdge(i+1), yhigh);
      tt->SetFillColor(theColor);
      tt->SetFillStyle(theStyle);
      tt->Draw();
    }
  }

  void Inspect(TH1** he=0, int Nerror=0, int doLeg=0) {
    TH1D * hInsp = (TH1D*)hErrorTot->Clone(Form("%s_plot",hErrorTot->GetName()));
    TF1 * fone = new TF1("fone","1",hErrorTot->GetBinLowEdge(1),hErrorTot->GetBinLowEdge(hErrorTot->GetNbinsX()));
    hInsp->Add(fone);
    hInsp->SetAxisRange(xmin,xmax-0.001,"X");
    hInsp->SetAxisRange(0.5,2,"Y");
    hInsp->SetLineWidth(3);
    hInsp->Draw("hist");
    // guide the eye
    TLine * hline = new TLine(xmin,1,xmax,1);
    hline->SetLineStyle(2);
    hline->Draw();

    for(int ie = 0; ie< Nerror; ++ie){
     // cout << "draw errors" << he[ie]->GetName() << endl;
      he[ie]->SetMarkerStyle(kOpenCircle);
      he[ie]->SetMarkerColor(sysMarkerColor[ie]);
      he[ie]->SetLineColor(sysMarkerColor[ie]);
      // he[ie]->DrawClone("psamehist");
      he[ie]->DrawClone("Esame");
      if (fe[ie]){
        fe[ie]->SetLineColor(sysMarkerColor[ie]);
        fe[ie]->SetLineWidth(1);
        fe[ie]->Draw("same");
      }
    }
    if (doLeg) {
      TLegend * lsys = new  TLegend(0.18,(int)(ana==2)*0.05+0.96-(Nerror)*0.062,0.87,0.96,NULL,"brNDC");
      easyLeg(lsys,"",18);
      for (int ie=0; ie<Nerror; ++ie) lsys->AddEntry(he[ie],sysTitle[ie],"l");
      lsys->AddEntry(hInsp,"Total Uncertainty","l");
        lsys->Draw();
    }
  }
};

void GetQM12TrkPt(  TH1D* ffppcmp[3][5], TH1D* ffhicmp[3][5]) {
  // pbpb
  Double_t xAxis1[28] = {0, 0.5, 1, 1.203915, 1.449412, 1.74497, 2.100796, 2.529181, 3.04492, 3.665826, 4.413344, 5.313293, 6.396755, 7.701152, 9.271536, 11.16214, 13.43828, 16.17855, 19.47761, 23.44939, 28.23108, 33.98783, 40.91848, 49.26238, 59.30774, 71.40151, 85.96137, 103.4902}; 
  TH1D *hTrk1 = new TH1D("hTrk1"," zy projection",27, xAxis1);
  hTrk1->SetBinContent(3,2.034728);
  hTrk1->SetBinContent(4,1.661647);
  hTrk1->SetBinContent(5,1.712479);
  hTrk1->SetBinContent(6,1.359556);
  hTrk1->SetBinContent(7,1.170278);
  hTrk1->SetBinContent(8,0.8941326);
  hTrk1->SetBinContent(9,0.7015379);
  hTrk1->SetBinContent(10,0.5450795);
  hTrk1->SetBinContent(11,0.4243944);
  hTrk1->SetBinContent(12,0.3373742);
  hTrk1->SetBinContent(13,0.2705829);
  hTrk1->SetBinContent(14,0.205206);
  hTrk1->SetBinContent(15,0.1696121);
  hTrk1->SetBinContent(16,0.1322215);
  hTrk1->SetBinContent(17,0.1012364);
  hTrk1->SetBinContent(18,0.07595321);
  hTrk1->SetBinContent(19,0.05665375);
  hTrk1->SetBinContent(20,0.04013204);
  hTrk1->SetBinContent(21,0.0284356);
  hTrk1->SetBinContent(22,0.01888555);
  hTrk1->SetBinContent(23,0.01170401);
  hTrk1->SetBinContent(24,0.006629254);
  hTrk1->SetBinContent(25,0.003722117);
  hTrk1->SetBinContent(26,0.001625522);
  hTrk1->SetBinContent(27,0.000553757);
  hTrk1->SetBinContent(28,0.00527477);
  hTrk1->SetBinError(3,0.08866401);
  hTrk1->SetBinError(4,0.0629638);
  hTrk1->SetBinError(5,0.04404573);
  hTrk1->SetBinError(6,0.02984382);
  hTrk1->SetBinError(7,0.01948675);
  hTrk1->SetBinError(8,0.01222556);
  hTrk1->SetBinError(9,0.007836383);
  hTrk1->SetBinError(10,0.005405699);
  hTrk1->SetBinError(11,0.003994027);
  hTrk1->SetBinError(12,0.00312802);
  hTrk1->SetBinError(13,0.002525855);
  hTrk1->SetBinError(14,0.001982447);
  hTrk1->SetBinError(15,0.00164284);
  hTrk1->SetBinError(16,0.001326248);
  hTrk1->SetBinError(17,0.001055913);
  hTrk1->SetBinError(18,0.0008314447);
  hTrk1->SetBinError(19,0.0006522497);
  hTrk1->SetBinError(20,0.0004972429);
  hTrk1->SetBinError(21,0.0003807224);
  hTrk1->SetBinError(22,0.0002821191);
  hTrk1->SetBinError(23,0.0002027759);
  hTrk1->SetBinError(24,0.0001393579);
  hTrk1->SetBinError(25,9.563792e-05);
  hTrk1->SetBinError(26,5.749635e-05);
  hTrk1->SetBinError(27,3.227105e-05);
  hTrk1->SetBinError(28,0.0005091112);
  hTrk1->SetEntries(46211.14);

  TH1D *hTrk2 = new TH1D("hTrk2"," zy projection",27, xAxis1);
  hTrk2->SetBinContent(3,1.850544);
  hTrk2->SetBinContent(4,1.665705);
  hTrk2->SetBinContent(5,1.47361);
  hTrk2->SetBinContent(6,1.288199);
  hTrk2->SetBinContent(7,1.147846);
  hTrk2->SetBinContent(8,0.9186873);
  hTrk2->SetBinContent(9,0.7081558);
  hTrk2->SetBinContent(10,0.5629215);
  hTrk2->SetBinContent(11,0.4524391);
  hTrk2->SetBinContent(12,0.3603946);
  hTrk2->SetBinContent(13,0.2865693);
  hTrk2->SetBinContent(14,0.2242059);
  hTrk2->SetBinContent(15,0.1772424);
  hTrk2->SetBinContent(16,0.1401229);
  hTrk2->SetBinContent(17,0.1077676);
  hTrk2->SetBinContent(18,0.0803899);
  hTrk2->SetBinContent(19,0.06062962);
  hTrk2->SetBinContent(20,0.04320826);
  hTrk2->SetBinContent(21,0.02917251);
  hTrk2->SetBinContent(22,0.01966681);
  hTrk2->SetBinContent(23,0.01217412);
  hTrk2->SetBinContent(24,0.006919396);
  hTrk2->SetBinContent(25,0.003822194);
  hTrk2->SetBinContent(26,0.001554365);
  hTrk2->SetBinContent(27,0.0006866741);
  hTrk2->SetBinContent(28,0.005638056);
  hTrk2->SetBinError(3,0.06499766);
  hTrk2->SetBinError(4,0.04719109);
  hTrk2->SetBinError(5,0.03320498);
  hTrk2->SetBinError(6,0.02284271);
  hTrk2->SetBinError(7,0.0154186);
  hTrk2->SetBinError(8,0.01019269);
  hTrk2->SetBinError(9,0.006854447);
  hTrk2->SetBinError(10,0.004983462);
  hTrk2->SetBinError(11,0.003832465);
  hTrk2->SetBinError(12,0.003045492);
  hTrk2->SetBinError(13,0.002444789);
  hTrk2->SetBinError(14,0.001957406);
  hTrk2->SetBinError(15,0.001586973);
  hTrk2->SetBinError(16,0.001287393);
  hTrk2->SetBinError(17,0.001028897);
  hTrk2->SetBinError(18,0.0008074842);
  hTrk2->SetBinError(19,0.0006370117);
  hTrk2->SetBinError(20,0.000486653);
  hTrk2->SetBinError(21,0.000363486);
  hTrk2->SetBinError(22,0.0002717931);
  hTrk2->SetBinError(23,0.0001950289);
  hTrk2->SetBinError(24,0.0001345276);
  hTrk2->SetBinError(25,9.200154e-05);
  hTrk2->SetBinError(26,5.316408e-05);
  hTrk2->SetBinError(27,3.821754e-05);
  hTrk2->SetBinError(28,0.0004124939);
  hTrk2->SetEntries(77616.52);

  TH1D *hTrk3 = new TH1D("hTrk3"," zy projection",27, xAxis1);
  hTrk3->SetBinContent(3,1.339262);
  hTrk3->SetBinContent(4,1.338548);
  hTrk3->SetBinContent(5,1.295132);
  hTrk3->SetBinContent(6,1.099145);
  hTrk3->SetBinContent(7,0.97305);
  hTrk3->SetBinContent(8,0.8318618);
  hTrk3->SetBinContent(9,0.6638141);
  hTrk3->SetBinContent(10,0.5697279);
  hTrk3->SetBinContent(11,0.4711685);
  hTrk3->SetBinContent(12,0.3623836);
  hTrk3->SetBinContent(13,0.301274);
  hTrk3->SetBinContent(14,0.2476461);
  hTrk3->SetBinContent(15,0.1884686);
  hTrk3->SetBinContent(16,0.1484286);
  hTrk3->SetBinContent(17,0.1182352);
  hTrk3->SetBinContent(18,0.08687204);
  hTrk3->SetBinContent(19,0.06557947);
  hTrk3->SetBinContent(20,0.04417248);
  hTrk3->SetBinContent(21,0.03026352);
  hTrk3->SetBinContent(22,0.02096436);
  hTrk3->SetBinContent(23,0.01281295);
  hTrk3->SetBinContent(24,0.007438461);
  hTrk3->SetBinContent(25,0.003845038);
  hTrk3->SetBinContent(26,0.001613512);
  hTrk3->SetBinContent(27,0.0005992832);
  hTrk3->SetBinContent(28,0.005002143);
  hTrk3->SetBinError(3,0.06755423);
  hTrk3->SetBinError(4,0.04905202);
  hTrk3->SetBinError(5,0.03544622);
  hTrk3->SetBinError(6,0.02488828);
  hTrk3->SetBinError(7,0.01780564);
  hTrk3->SetBinError(8,0.01270214);
  hTrk3->SetBinError(9,0.00932445);
  hTrk3->SetBinError(10,0.007396056);
  hTrk3->SetBinError(11,0.005989601);
  hTrk3->SetBinError(12,0.004733229);
  hTrk3->SetBinError(13,0.003913623);
  hTrk3->SetBinError(14,0.003240035);
  hTrk3->SetBinError(15,0.00256588);
  hTrk3->SetBinError(16,0.002077033);
  hTrk3->SetBinError(17,0.001682418);
  hTrk3->SetBinError(18,0.001317014);
  hTrk3->SetBinError(19,0.001044024);
  hTrk3->SetBinError(20,0.0007790051);
  hTrk3->SetBinError(21,0.0005836767);
  hTrk3->SetBinError(22,0.0004414891);
  hTrk3->SetBinError(23,0.0003148256);
  hTrk3->SetBinError(24,0.000218963);
  hTrk3->SetBinError(25,0.0001458443);
  hTrk3->SetBinError(26,8.53968e-05);
  hTrk3->SetBinError(27,5.573867e-05);
  hTrk3->SetBinError(28,0.0006736282);
  hTrk3->SetEntries(51815.09);

  TH1D *hTrk4 = new TH1D("hTrk4"," zy projection",27, xAxis1);
  hTrk4->SetBinContent(3,1.237576);
  hTrk4->SetBinContent(4,1.172663);
  hTrk4->SetBinContent(5,1.119807);
  hTrk4->SetBinContent(6,1.075697);
  hTrk4->SetBinContent(7,0.9337811);
  hTrk4->SetBinContent(8,0.8624421);
  hTrk4->SetBinContent(9,0.6837416);
  hTrk4->SetBinContent(10,0.577182);
  hTrk4->SetBinContent(11,0.4761935);
  hTrk4->SetBinContent(12,0.400518);
  hTrk4->SetBinContent(13,0.3117255);
  hTrk4->SetBinContent(14,0.2631362);
  hTrk4->SetBinContent(15,0.2169029);
  hTrk4->SetBinContent(16,0.1546248);
  hTrk4->SetBinContent(17,0.1231363);
  hTrk4->SetBinContent(18,0.09036509);
  hTrk4->SetBinContent(19,0.06692674);
  hTrk4->SetBinContent(20,0.04902385);
  hTrk4->SetBinContent(21,0.03291553);
  hTrk4->SetBinContent(22,0.02061061);
  hTrk4->SetBinContent(23,0.01263376);
  hTrk4->SetBinContent(24,0.00731623);
  hTrk4->SetBinContent(25,0.003781511);
  hTrk4->SetBinContent(26,0.00138309);
  hTrk4->SetBinContent(27,0.0004953297);
  hTrk4->SetBinContent(28,0.00535258);
  hTrk4->SetBinError(3,0.07426784);
  hTrk4->SetBinError(4,0.05440216);
  hTrk4->SetBinError(5,0.04140522);
  hTrk4->SetBinError(6,0.03171496);
  hTrk4->SetBinError(7,0.02494865);
  hTrk4->SetBinError(8,0.01978925);
  hTrk4->SetBinError(9,0.01546242);
  hTrk4->SetBinError(10,0.01262797);
  hTrk4->SetBinError(11,0.01037849);
  hTrk4->SetBinError(12,0.008601504);
  hTrk4->SetBinError(13,0.006935747);
  hTrk4->SetBinError(14,0.005819085);
  hTrk4->SetBinError(15,0.004805089);
  hTrk4->SetBinError(16,0.003687556);
  hTrk4->SetBinError(17,0.002998112);
  hTrk4->SetBinError(18,0.002330463);
  hTrk4->SetBinError(19,0.001834495);
  hTrk4->SetBinError(20,0.001434411);
  hTrk4->SetBinError(21,0.001063557);
  hTrk4->SetBinError(22,0.0007655936);
  hTrk4->SetBinError(23,0.0005461723);
  hTrk4->SetBinError(24,0.0003876485);
  hTrk4->SetBinError(25,0.0002513423);
  hTrk4->SetBinError(26,0.0001370012);
  hTrk4->SetBinError(27,7.715681e-05);
  hTrk4->SetBinError(28,0.001373209);
  hTrk4->SetEntries(26431.71);

  ffhicmp[1][1]=hTrk1;
  ffhicmp[1][2]=hTrk2;
  ffhicmp[1][3]=hTrk3;
  ffhicmp[1][4]=hTrk4;

  // pp
  TH1D *hTrkmc1 = new TH1D("hTrkmc1"," zy projection",27, xAxis1);
  hTrkmc1->SetBinContent(3,0.9826916);
  hTrkmc1->SetBinContent(4,1.024127);
  hTrkmc1->SetBinContent(5,1.028526);
  hTrkmc1->SetBinContent(6,0.9560202);
  hTrkmc1->SetBinContent(7,0.9465681);
  hTrkmc1->SetBinContent(8,0.8360189);
  hTrkmc1->SetBinContent(9,0.7555453);
  hTrkmc1->SetBinContent(10,0.6162637);
  hTrkmc1->SetBinContent(11,0.5232184);
  hTrkmc1->SetBinContent(12,0.4408998);
  hTrkmc1->SetBinContent(13,0.3621469);
  hTrkmc1->SetBinContent(14,0.2915999);
  hTrkmc1->SetBinContent(15,0.2389);
  hTrkmc1->SetBinContent(16,0.1747993);
  hTrkmc1->SetBinContent(17,0.1366478);
  hTrkmc1->SetBinContent(18,0.1014618);
  hTrkmc1->SetBinContent(19,0.06891158);
  hTrkmc1->SetBinContent(20,0.04930748);
  hTrkmc1->SetBinContent(21,0.03318468);
  hTrkmc1->SetBinContent(22,0.02261941);
  hTrkmc1->SetBinContent(23,0.0127418);
  hTrkmc1->SetBinContent(24,0.006529172);
  hTrkmc1->SetBinContent(25,0.003104842);
  hTrkmc1->SetBinContent(26,0.001337274);
  hTrkmc1->SetBinContent(27,0.0003934181);
  hTrkmc1->SetBinContent(28,0.0009501012);
  hTrkmc1->SetBinError(3,0.03205463);
  hTrkmc1->SetBinError(4,0.02862094);
  hTrkmc1->SetBinError(5,0.02503955);
  hTrkmc1->SetBinError(6,0.02131369);
  hTrkmc1->SetBinError(7,0.01884037);
  hTrkmc1->SetBinError(8,0.01582361);
  hTrkmc1->SetBinError(9,0.013519);
  hTrkmc1->SetBinError(10,0.01108292);
  hTrkmc1->SetBinError(11,0.009336285);
  hTrkmc1->SetBinError(12,0.00782611);
  hTrkmc1->SetBinError(13,0.006463478);
  hTrkmc1->SetBinError(14,0.005323872);
  hTrkmc1->SetBinError(15,0.004424533);
  hTrkmc1->SetBinError(16,0.003468514);
  hTrkmc1->SetBinError(17,0.002809295);
  hTrkmc1->SetBinError(18,0.002214133);
  hTrkmc1->SetBinError(19,0.001654428);
  hTrkmc1->SetBinError(20,0.001279813);
  hTrkmc1->SetBinError(21,0.0009678812);
  hTrkmc1->SetBinError(22,0.0007324213);
  hTrkmc1->SetBinError(23,0.000501282);
  hTrkmc1->SetBinError(24,0.0003238156);
  hTrkmc1->SetBinError(25,0.000213035);
  hTrkmc1->SetBinError(26,0.0001212611);
  hTrkmc1->SetBinError(27,6.192168e-05);
  hTrkmc1->SetBinError(28,0.0001798565);
  hTrkmc1->SetEntries(46345.03);

  TH1D *hTrkmc2 = new TH1D("hTrkmc2"," zy projection",27, xAxis1);
  hTrkmc2->SetBinContent(3,0.9941635);
  hTrkmc2->SetBinContent(4,1.023858);
  hTrkmc2->SetBinContent(5,1.024354);
  hTrkmc2->SetBinContent(6,0.958434);
  hTrkmc2->SetBinContent(7,0.946172);
  hTrkmc2->SetBinContent(8,0.832207);
  hTrkmc2->SetBinContent(9,0.7543413);
  hTrkmc2->SetBinContent(10,0.6113132);
  hTrkmc2->SetBinContent(11,0.5240611);
  hTrkmc2->SetBinContent(12,0.4412311);
  hTrkmc2->SetBinContent(13,0.3612028);
  hTrkmc2->SetBinContent(14,0.2914949);
  hTrkmc2->SetBinContent(15,0.2380405);
  hTrkmc2->SetBinContent(16,0.1754098);
  hTrkmc2->SetBinContent(17,0.1367526);
  hTrkmc2->SetBinContent(18,0.1016944);
  hTrkmc2->SetBinContent(19,0.06955088);
  hTrkmc2->SetBinContent(20,0.0494318);
  hTrkmc2->SetBinContent(21,0.03326401);
  hTrkmc2->SetBinContent(22,0.02280469);
  hTrkmc2->SetBinContent(23,0.01279481);
  hTrkmc2->SetBinContent(24,0.00674487);
  hTrkmc2->SetBinContent(25,0.00321762);
  hTrkmc2->SetBinContent(26,0.001407672);
  hTrkmc2->SetBinContent(27,0.0004083177);
  hTrkmc2->SetBinContent(28,0.0008457624);
  hTrkmc2->SetBinError(3,0.03285278);
  hTrkmc2->SetBinError(4,0.02911278);
  hTrkmc2->SetBinError(5,0.02544836);
  hTrkmc2->SetBinError(6,0.02176035);
  hTrkmc2->SetBinError(7,0.01915763);
  hTrkmc2->SetBinError(8,0.01603195);
  hTrkmc2->SetBinError(9,0.01374485);
  hTrkmc2->SetBinError(10,0.01122749);
  hTrkmc2->SetBinError(11,0.009489328);
  hTrkmc2->SetBinError(12,0.007960197);
  hTrkmc2->SetBinError(13,0.006572257);
  hTrkmc2->SetBinError(14,0.005405931);
  hTrkmc2->SetBinError(15,0.004485331);
  hTrkmc2->SetBinError(16,0.003530154);
  hTrkmc2->SetBinError(17,0.002855128);
  hTrkmc2->SetBinError(18,0.0022484);
  hTrkmc2->SetBinError(19,0.001684165);
  hTrkmc2->SetBinError(20,0.001294917);
  hTrkmc2->SetBinError(21,0.0009771489);
  hTrkmc2->SetBinError(22,0.0007417128);
  hTrkmc2->SetBinError(23,0.000506765);
  hTrkmc2->SetBinError(24,0.0003332046);
  hTrkmc2->SetBinError(25,0.0002213512);
  hTrkmc2->SetBinError(26,0.0001277063);
  hTrkmc2->SetBinError(27,6.414031e-05);
  hTrkmc2->SetBinError(28,0.0001601049);
  hTrkmc2->SetEntries(44931.55);

  TH1D *hTrkmc3 = new TH1D("hTrkmc3"," zy projection",27, xAxis1);
  hTrkmc3->SetBinContent(3,0.983694);
  hTrkmc3->SetBinContent(4,1.04456);
  hTrkmc3->SetBinContent(5,1.012429);
  hTrkmc3->SetBinContent(6,0.9645292);
  hTrkmc3->SetBinContent(7,0.9443498);
  hTrkmc3->SetBinContent(8,0.8308981);
  hTrkmc3->SetBinContent(9,0.7483722);
  hTrkmc3->SetBinContent(10,0.6118229);
  hTrkmc3->SetBinContent(11,0.5252797);
  hTrkmc3->SetBinContent(12,0.4401947);
  hTrkmc3->SetBinContent(13,0.3591793);
  hTrkmc3->SetBinContent(14,0.2904862);
  hTrkmc3->SetBinContent(15,0.2376497);
  hTrkmc3->SetBinContent(16,0.176066);
  hTrkmc3->SetBinContent(17,0.1362456);
  hTrkmc3->SetBinContent(18,0.1021192);
  hTrkmc3->SetBinContent(19,0.06989341);
  hTrkmc3->SetBinContent(20,0.05045471);
  hTrkmc3->SetBinContent(21,0.03317448);
  hTrkmc3->SetBinContent(22,0.02284722);
  hTrkmc3->SetBinContent(23,0.01297073);
  hTrkmc3->SetBinContent(24,0.006896269);
  hTrkmc3->SetBinContent(25,0.003252371);
  hTrkmc3->SetBinContent(26,0.001457567);
  hTrkmc3->SetBinContent(27,0.0004114206);
  hTrkmc3->SetBinContent(28,0.002111068);
  hTrkmc3->SetBinError(3,0.0332731);
  hTrkmc3->SetBinError(4,0.02983061);
  hTrkmc3->SetBinError(5,0.02571409);
  hTrkmc3->SetBinError(6,0.02223308);
  hTrkmc3->SetBinError(7,0.01950059);
  hTrkmc3->SetBinError(8,0.0163388);
  hTrkmc3->SetBinError(9,0.01392708);
  hTrkmc3->SetBinError(10,0.01144436);
  hTrkmc3->SetBinError(11,0.009663875);
  hTrkmc3->SetBinError(12,0.008087498);
  hTrkmc3->SetBinError(13,0.006652296);
  hTrkmc3->SetBinError(14,0.005482121);
  hTrkmc3->SetBinError(15,0.004556158);
  hTrkmc3->SetBinError(16,0.003594953);
  hTrkmc3->SetBinError(17,0.002890289);
  hTrkmc3->SetBinError(18,0.002284495);
  hTrkmc3->SetBinError(19,0.001714832);
  hTrkmc3->SetBinError(20,0.001327781);
  hTrkmc3->SetBinError(21,0.000984538);
  hTrkmc3->SetBinError(22,0.0007489413);
  hTrkmc3->SetBinError(23,0.0005169398);
  hTrkmc3->SetBinError(24,0.0003419683);
  hTrkmc3->SetBinError(25,0.0002203537);
  hTrkmc3->SetBinError(26,0.0001317421);
  hTrkmc3->SetBinError(27,6.487834e-05);
  hTrkmc3->SetBinError(28,0.0003996303);
  hTrkmc3->SetEntries(43534.83);

  TH1D *hTrkmc4 = new TH1D("hTrkmc4"," zy projection",27, xAxis1);
  hTrkmc4->SetBinContent(3,0.9903552);
  hTrkmc4->SetBinContent(4,1.034722);
  hTrkmc4->SetBinContent(5,0.9968611);
  hTrkmc4->SetBinContent(6,0.9693937);
  hTrkmc4->SetBinContent(7,0.946638);
  hTrkmc4->SetBinContent(8,0.8248127);
  hTrkmc4->SetBinContent(9,0.7457311);
  hTrkmc4->SetBinContent(10,0.6140014);
  hTrkmc4->SetBinContent(11,0.5269764);
  hTrkmc4->SetBinContent(12,0.4370389);
  hTrkmc4->SetBinContent(13,0.35915);
  hTrkmc4->SetBinContent(14,0.2908448);
  hTrkmc4->SetBinContent(15,0.2359818);
  hTrkmc4->SetBinContent(16,0.176936);
  hTrkmc4->SetBinContent(17,0.1385424);
  hTrkmc4->SetBinContent(18,0.1025292);
  hTrkmc4->SetBinContent(19,0.06937703);
  hTrkmc4->SetBinContent(20,0.04940683);
  hTrkmc4->SetBinContent(21,0.03289264);
  hTrkmc4->SetBinContent(22,0.02297276);
  hTrkmc4->SetBinContent(23,0.01289282);
  hTrkmc4->SetBinContent(24,0.007083522);
  hTrkmc4->SetBinContent(25,0.003319527);
  hTrkmc4->SetBinContent(26,0.001578852);
  hTrkmc4->SetBinContent(27,0.0004583495);
  hTrkmc4->SetBinContent(28,0.00626992);
  hTrkmc4->SetBinError(3,0.03402593);
  hTrkmc4->SetBinError(4,0.03026783);
  hTrkmc4->SetBinError(5,0.02615299);
  hTrkmc4->SetBinError(6,0.02257495);
  hTrkmc4->SetBinError(7,0.01988889);
  hTrkmc4->SetBinError(8,0.01654422);
  hTrkmc4->SetBinError(9,0.01411972);
  hTrkmc4->SetBinError(10,0.01165131);
  hTrkmc4->SetBinError(11,0.009840133);
  hTrkmc4->SetBinError(12,0.008189911);
  hTrkmc4->SetBinError(13,0.006744234);
  hTrkmc4->SetBinError(14,0.005581132);
  hTrkmc4->SetBinError(15,0.004605172);
  hTrkmc4->SetBinError(16,0.003648508);
  hTrkmc4->SetBinError(17,0.002956545);
  hTrkmc4->SetBinError(18,0.002318394);
  hTrkmc4->SetBinError(19,0.00173447);
  hTrkmc4->SetBinError(20,0.00132846);
  hTrkmc4->SetBinError(21,0.0009911145);
  hTrkmc4->SetBinError(22,0.000759915);
  hTrkmc4->SetBinError(23,0.0005211535);
  hTrkmc4->SetBinError(24,0.0003533401);
  hTrkmc4->SetBinError(25,0.0002246538);
  hTrkmc4->SetBinError(26,0.0001395109);
  hTrkmc4->SetBinError(27,7.339461e-05);
  hTrkmc4->SetBinError(28,0.001205137);
  hTrkmc4->SetEntries(42172.03);

  ffppcmp[1][1]=hTrkmc1;
  ffppcmp[1][2]=hTrkmc2;
  ffppcmp[1][3]=hTrkmc3;
  ffppcmp[1][4]=hTrkmc4;
}