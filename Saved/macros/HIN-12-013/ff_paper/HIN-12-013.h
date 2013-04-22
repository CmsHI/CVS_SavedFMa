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
  int cycle;

  // Functions
  Plot4x4(TH1D* hnum[3][5], TH1D* hden[3][5], int normMode=0, TString on="pt", int cyc=0)
  : ijet(1)
  , outname(on), cycle(cyc) {
    // Get Histograms
    for ( int icent=1; icent<=4 ; icent++) {
      h[ijet][icent]    = hnum[ijet][icent];
      href[ijet][icent] = hden[ijet][icent];
      assert(h[ijet][icent]);
      assert(href[ijet][icent]);
      normHist(h[ijet][icent],normMode,true);
      normHist(href[ijet][icent],normMode,true);
      if (cycle==0) {
        handsomeTH1(h[ijet][icent],kRed,1,kFullCircle);
        handsomeTH1(href[ijet][icent],kBlack,1,kOpenCircle);
      } else if (cycle==1) {
        handsomeTH1(h[ijet][icent],kBlue,1,kOpenCircle);
        handsomeTH1(href[ijet][icent],kBlue,1,kOpenCircle,7);
      }
      h[ijet][icent]->SetNdivisions(505);
      href[ijet][icent]->SetNdivisions(505);
      fixedFontHist(h[ijet][icent],1.8,2.5);
      fixedFontHist(href[ijet][icent],1.6,2.5);
      // Get Ratio
      hrat[ijet][icent]  = (TH1D*)h[ijet][icent]->Clone(Form("%s_ratio",h[ijet][icent]->GetName()));
      hrat[ijet][icent]->Divide(href[ijet][icent]);
      hrat[ijet][icent]->SetYTitle("Ratio");
    }
  }

  void Draw(TCanvas *c1, float xmin=-1, float xmax=-1, int doLog=1, float ymin=-1, float ymax=-1) {
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
      if (cycle==0) hrat[ijet][icent]->DrawCopy("E");
      else hrat[ijet][icent]->DrawCopy("sameE");
      TLine * hline = new TLine(xmin,1,xmax,1);
      hline->SetLineStyle(2);
      hline->Draw();      
    }
  }

  void DrawLeg(TCanvas *c1, int idraw=3, float x1=0.096,float y1=0.028, float x2=0.577, float y2=0.238, TString pref="", TString newtitle2="") {
    TString mytitle1=title1;
    TString mytitle2=title2;
    if (pref!="") {
      mytitle1=pref+" "+mytitle1;
      mytitle2=pref+" "+mytitle2;
    }
    if (pref!="" && newtitle2!="") {
      mytitle1=pref;
      mytitle2=newtitle2;
    }

    // Book Legend
    if (cycle==0) {
      l1 = new TLegend(0.24,0.07,0.67,0.28,NULL,"brNDC");
      easyLeg(l1);
      l1->AddEntry(h[ijet][1],"|#eta_{jet}| < 2","");
    }
    l2 = new TLegend(x1,y1,x2,y2,NULL,"brNDC");
    easyLeg(l2,"",20);
    // Annotate
    for ( int icent=1; icent<=4 ; icent++) {
      c1->cd(5-icent);  
      if ( icent == idraw) {
        l2->AddEntry(h[ijet][icent],mytitle1,"p");
        l2->AddEntry(href[ijet][icent],mytitle2,"l");
        l2->Draw();
      }
      // meta info
      if (cycle>0) continue;
      if ( icent == 4) {
        drawText("CMS Preliminary",0.3,0.89,1,20);
        l1->Draw();
      }
      c1->cd(9-icent);
      int lowCent = centBin1[icent-1];
      int highCent = centBin1[icent]-1;
      if ( icent ==4 ) 
        drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.35,0.25,kBlack,20);
      else
        drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),  0.15,0.25,kBlack,20);
    }

    if (cycle==0) {
      c1->SaveAs(outpath+outname+".gif");
      c1->SaveAs(outpath+outname+".pdf");
    }

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
    // const char* JESfile = "./systematics/data_sysjes0.935exptra.root";
    const char* JESfile = "./systematics/data_sysjes0.949exptrap.root";
    const char* JetResfile = "./systematics/data_sysjer.root";
    const char* BkgSubfile = "./systematics/bkgsub_sys.root";
    const char* TrkClosurefile = "./systematics/mc_corrjbias0_trk_closure100_Mar04v3_hi.root";
    const char* GenBkgSubfile = "./systematics/mc_corrjbias0_trk_closure102_Mar04v3_hi.root";
    const char* JetAlgofile = "./systematics/systematics_calovspf.root";
      // For FF distribution
    const char* PyquenJetRecoRefPtfile = "./systematics/PYQUEN80_useGenJet0_closureTestRatio_between_100_103.root";
    const char* PyquenGenJetSelfile = "./systematics/closureTestRatio_pyquen80_genJet_recojet_selectionRatio.root";

    for ( int iaj=1;iaj<=4;iaj++) {
      if (binMode==1) {
        // jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0_sys2",jname[ijet].Data(),iaj));
        // jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure0",jname[ijet].Data(),iaj));
        // bkgSubError[ijet][iaj] = loadError(BkgSubfile,Form("hpt_%s_sigTrk_hidata_icent999_iaj%d_fragMode2_closure100",jname[ijet].Data(),iaj)); // from event mixing
        // trkError[ijet][iaj] = loadError(TrkClosurefile,Form("hpt_%s_sigTrk_himc_icent999_iaj%d_fragMode2_closure101",jname[ijet].Data(),iaj));
      } else if (binMode==2) {
        // jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_%s_sigTrk_hidata_icent2_irj999_fragMode2_closure100_sys7",jname[ijet].Data(),iaj));
        jesError[ijet][iaj] =  loadError(JESfile,Form("hpt_jet_sigTrk_hidata_icent%d_irj999_fragMode%d_closure100_jtrewt1_wtmode0_pt1to300_ratio",iaj,ana));
        // jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
        jetResError[ijet][iaj] =  loadError(JetResfile,Form("hpt_jet_sigTrk_hidata_icent%d_irj999_fragMode%d_closure100_jtrewt0_wtmode0_pt1to300_ratio",iaj,ana));
        bkgSubError[ijet][iaj] =  loadError(BkgSubfile,Form("hpt_%s_sigTrk_hidata_icent%d_irj999_fragMode2_closure0",jname[ijet].Data(),iaj));

        // trkError[ijet][iaj] =  loadError(TrkClosurefile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure100",jname[ijet].Data(),iaj));
        // trkError[ijet][iaj] =  loadError(TrkClosurefile,Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure100_jtrewt0_wtmode0_pt1to300_ratio",iaj,ana));
        trkError[ijet][iaj] =  loadError(TrkClosurefile,Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure100_rewt0_ppsm0_wtmode0_pt1to300_ratio",iaj,ana));

        // genBkgSubError[ijet][iaj] =  loadError(GenBkgSubfile,Form("hpt_%s_sigTrk_himc_icent%d_irj999_fragMode2_closure102",jname[ijet].Data(),iaj));
        genBkgSubError[ijet][iaj] =  loadError(GenBkgSubfile,Form("hpt_jet_sigTrk_himc_icent%d_irj999_fragMode%d_closure102_rewt0_ppsm0_wtmode0_pt1to300_ratio",iaj,ana));
        for(int i = 1; i <= genBkgSubError[ijet][iaj]->GetNbinsX(); ++i){
          genBkgSubError[ijet][iaj]->SetBinContent(i,1+(genBkgSubError[ijet][iaj]->GetBinContent(i)-1)*0.5);
          genBkgSubError[ijet][iaj]->SetBinError(i,genBkgSubError[ijet][iaj]->GetBinError(i)*0.5);
        }        


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
        if (ie==0) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol5",xmin,xmax*2);
        else       fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol4",xmin,xmax*2);
        if (ie==0) he[ie]->Fit(fe[ie],"QNRll","",xmin,xmax*1.2);
        else if (ie==1||ie==3) he[ie]->Fit(fe[ie],"QNRll","",xmin,xmax*1.5);
        else he[ie]->Fit(fe[ie],"QNRw","",xmin,xmax*1.5);
      } else if (ana==2) {
        if (ie==0) {
          fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"1+[0]/(1+exp([1]-[2]*x))",0,6);
          fe[ie]->SetParameters(0.2,15,4);
        }
        else if (ie==4) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",0,5.5);
        else if (ie<6) fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol3",0,5.5);
        else fe[ie] = new TF1(Form("%s_fit_%d",h->GetName(),ie),"pol4",0,5.5);
        if (ie==0) {
          he[ie]->Fit(fe[ie],"QNR","",0,5.5);
        }
        else if (ie==4) he[ie]->Fit(fe[ie],"QRN","",0.,5.5);
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

  if (ffhicmp) {
    ffhicmp[1][1]=hTrk1;
    ffhicmp[1][2]=hTrk2;
    ffhicmp[1][3]=hTrk3;
    ffhicmp[1][4]=hTrk4;
  }

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

  if (ffppcmp) {
    ffppcmp[1][1]=hTrkmc1;
    ffppcmp[1][2]=hTrkmc2;
    ffppcmp[1][3]=hTrkmc3;
    ffppcmp[1][4]=hTrkmc4;
  }
}

void drawQM12TrackPtRatioSysBand_0to4() {
 TGraph *    graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,1,0.4898413302);
   graph->SetPoint(1,1,1.614232007);
   graph->SetPoint(2,1.203915408,1.614232007);
   graph->SetPoint(3,1.203915408,0.4898413302);
   graph->SetPoint(4,1,0.4898413302);
   
   TH1F *Graph_Graph176 = new TH1F("Graph_Graph176","",100,0.9796085,1.224307);
   Graph_Graph176->SetMinimum(0.3774023);
   Graph_Graph176->SetMaximum(1.726671);
   Graph_Graph176->SetDirectory(0);
   Graph_Graph176->SetStats(0);
   Graph_Graph176->SetLineStyle(0);
   Graph_Graph176->SetMarkerStyle(20);
   Graph_Graph176->SetMarkerSize(1.3);
   Graph_Graph176->GetXaxis()->SetLabelFont(43);
   Graph_Graph176->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph176->GetXaxis()->SetLabelSize(14);
   Graph_Graph176->GetXaxis()->SetTitleSize(21);
   Graph_Graph176->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph176->GetXaxis()->SetTitleFont(43);
   Graph_Graph176->GetYaxis()->SetLabelFont(43);
   Graph_Graph176->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph176->GetYaxis()->SetLabelSize(14);
   Graph_Graph176->GetYaxis()->SetTitleSize(21);
   Graph_Graph176->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph176->GetYaxis()->SetTitleFont(43);
   Graph_Graph176->GetZaxis()->SetLabelFont(43);
   Graph_Graph176->GetZaxis()->SetLabelSize(14);
   Graph_Graph176->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph176->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph176);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,1.203915408,0.201189696);
   graph->SetPoint(1,1.203915408,1.073848782);
   graph->SetPoint(2,1.449412309,1.073848782);
   graph->SetPoint(3,1.449412309,0.201189696);
   graph->SetPoint(4,1.203915408,0.201189696);
   
   TH1F *Graph_Graph177 = new TH1F("Graph_Graph177","",100,1.179366,1.473962);
   Graph_Graph177->SetMinimum(0.1139238);
   Graph_Graph177->SetMaximum(1.161115);
   Graph_Graph177->SetDirectory(0);
   Graph_Graph177->SetStats(0);
   Graph_Graph177->SetLineStyle(0);
   Graph_Graph177->SetMarkerStyle(20);
   Graph_Graph177->SetMarkerSize(1.3);
   Graph_Graph177->GetXaxis()->SetLabelFont(43);
   Graph_Graph177->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph177->GetXaxis()->SetLabelSize(14);
   Graph_Graph177->GetXaxis()->SetTitleSize(21);
   Graph_Graph177->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph177->GetXaxis()->SetTitleFont(43);
   Graph_Graph177->GetYaxis()->SetLabelFont(43);
   Graph_Graph177->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph177->GetYaxis()->SetLabelSize(14);
   Graph_Graph177->GetYaxis()->SetTitleSize(21);
   Graph_Graph177->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph177->GetYaxis()->SetTitleFont(43);
   Graph_Graph177->GetZaxis()->SetLabelFont(43);
   Graph_Graph177->GetZaxis()->SetLabelSize(14);
   Graph_Graph177->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph177->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph177);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,1.449412309,0.3178243497);
   graph->SetPoint(1,1.449412309,1.05008149);
   graph->SetPoint(2,1.74496981,1.05008149);
   graph->SetPoint(3,1.74496981,0.3178243497);
   graph->SetPoint(4,1.449412309,0.3178243497);
   
   TH1F *Graph_Graph178 = new TH1F("Graph_Graph178","",100,1.419857,1.774526);
   Graph_Graph178->SetMinimum(0.2445986);
   Graph_Graph178->SetMaximum(1.123307);
   Graph_Graph178->SetDirectory(0);
   Graph_Graph178->SetStats(0);
   Graph_Graph178->SetLineStyle(0);
   Graph_Graph178->SetMarkerStyle(20);
   Graph_Graph178->SetMarkerSize(1.3);
   Graph_Graph178->GetXaxis()->SetLabelFont(43);
   Graph_Graph178->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph178->GetXaxis()->SetLabelSize(14);
   Graph_Graph178->GetXaxis()->SetTitleSize(21);
   Graph_Graph178->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph178->GetXaxis()->SetTitleFont(43);
   Graph_Graph178->GetYaxis()->SetLabelFont(43);
   Graph_Graph178->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph178->GetYaxis()->SetLabelSize(14);
   Graph_Graph178->GetYaxis()->SetTitleSize(21);
   Graph_Graph178->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph178->GetYaxis()->SetTitleFont(43);
   Graph_Graph178->GetZaxis()->SetLabelFont(43);
   Graph_Graph178->GetZaxis()->SetLabelSize(14);
   Graph_Graph178->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph178->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph178);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,1.74496981,0.1115966677);
   graph->SetPoint(1,1.74496981,0.695475445);
   graph->SetPoint(2,2.100796041,0.695475445);
   graph->SetPoint(3,2.100796041,0.1115966677);
   graph->SetPoint(4,1.74496981,0.1115966677);
   
   TH1F *Graph_Graph179 = new TH1F("Graph_Graph179","",100,1.709387,2.136379);
   Graph_Graph179->SetMinimum(0.05320879);
   Graph_Graph179->SetMaximum(0.7538633);
   Graph_Graph179->SetDirectory(0);
   Graph_Graph179->SetStats(0);
   Graph_Graph179->SetLineStyle(0);
   Graph_Graph179->SetMarkerStyle(20);
   Graph_Graph179->SetMarkerSize(1.3);
   Graph_Graph179->GetXaxis()->SetLabelFont(43);
   Graph_Graph179->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph179->GetXaxis()->SetLabelSize(14);
   Graph_Graph179->GetXaxis()->SetTitleSize(21);
   Graph_Graph179->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph179->GetXaxis()->SetTitleFont(43);
   Graph_Graph179->GetYaxis()->SetLabelFont(43);
   Graph_Graph179->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph179->GetYaxis()->SetLabelSize(14);
   Graph_Graph179->GetYaxis()->SetTitleSize(21);
   Graph_Graph179->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph179->GetYaxis()->SetTitleFont(43);
   Graph_Graph179->GetZaxis()->SetLabelFont(43);
   Graph_Graph179->GetZaxis()->SetLabelSize(14);
   Graph_Graph179->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph179->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph179);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,2.100796041,-0.01477153739);
   graph->SetPoint(1,2.100796041,0.4621919533);
   graph->SetPoint(2,2.529180722,0.4621919533);
   graph->SetPoint(3,2.529180722,-0.01477153739);
   graph->SetPoint(4,2.100796041,-0.01477153739);
   
   TH1F *Graph_Graph180 = new TH1F("Graph_Graph180","",100,2.057958,2.572019);
   Graph_Graph180->SetMinimum(-0.06246789);
   Graph_Graph180->SetMaximum(0.5098883);
   Graph_Graph180->SetDirectory(0);
   Graph_Graph180->SetStats(0);
   Graph_Graph180->SetLineStyle(0);
   Graph_Graph180->SetMarkerStyle(20);
   Graph_Graph180->SetMarkerSize(1.3);
   Graph_Graph180->GetXaxis()->SetLabelFont(43);
   Graph_Graph180->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph180->GetXaxis()->SetLabelSize(14);
   Graph_Graph180->GetXaxis()->SetTitleSize(21);
   Graph_Graph180->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph180->GetXaxis()->SetTitleFont(43);
   Graph_Graph180->GetYaxis()->SetLabelFont(43);
   Graph_Graph180->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph180->GetYaxis()->SetLabelSize(14);
   Graph_Graph180->GetYaxis()->SetTitleSize(21);
   Graph_Graph180->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph180->GetYaxis()->SetTitleFont(43);
   Graph_Graph180->GetZaxis()->SetLabelFont(43);
   Graph_Graph180->GetZaxis()->SetLabelSize(14);
   Graph_Graph180->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph180->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph180);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,2.529180722,-0.1267855413);
   graph->SetPoint(1,2.529180722,0.2430128694);
   graph->SetPoint(2,3.044919639,0.2430128694);
   graph->SetPoint(3,3.044919639,-0.1267855413);
   graph->SetPoint(4,2.529180722,-0.1267855413);
   
   TH1F *Graph_Graph181 = new TH1F("Graph_Graph181","",100,2.477607,3.096494);
   Graph_Graph181->SetMinimum(-0.1637654);
   Graph_Graph181->SetMaximum(0.2799927);
   Graph_Graph181->SetDirectory(0);
   Graph_Graph181->SetStats(0);
   Graph_Graph181->SetLineStyle(0);
   Graph_Graph181->SetMarkerStyle(20);
   Graph_Graph181->SetMarkerSize(1.3);
   Graph_Graph181->GetXaxis()->SetLabelFont(43);
   Graph_Graph181->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph181->GetXaxis()->SetLabelSize(14);
   Graph_Graph181->GetXaxis()->SetTitleSize(21);
   Graph_Graph181->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph181->GetXaxis()->SetTitleFont(43);
   Graph_Graph181->GetYaxis()->SetLabelFont(43);
   Graph_Graph181->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph181->GetYaxis()->SetLabelSize(14);
   Graph_Graph181->GetYaxis()->SetTitleSize(21);
   Graph_Graph181->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph181->GetYaxis()->SetTitleFont(43);
   Graph_Graph181->GetZaxis()->SetLabelFont(43);
   Graph_Graph181->GetZaxis()->SetLabelSize(14);
   Graph_Graph181->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph181->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph181);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,3.044919639,-0.1950707875);
   graph->SetPoint(1,3.044919639,0.08705594145);
   graph->SetPoint(2,3.665825669,0.08705594145);
   graph->SetPoint(3,3.665825669,-0.1950707875);
   graph->SetPoint(4,3.044919639,-0.1950707875);
   
   TH1F *Graph_Graph182 = new TH1F("Graph_Graph182","",100,2.982829,3.727916);
   Graph_Graph182->SetMinimum(-0.2232835);
   Graph_Graph182->SetMaximum(0.1152686);
   Graph_Graph182->SetDirectory(0);
   Graph_Graph182->SetStats(0);
   Graph_Graph182->SetLineStyle(0);
   Graph_Graph182->SetMarkerStyle(20);
   Graph_Graph182->SetMarkerSize(1.3);
   Graph_Graph182->GetXaxis()->SetLabelFont(43);
   Graph_Graph182->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph182->GetXaxis()->SetLabelSize(14);
   Graph_Graph182->GetXaxis()->SetTitleSize(21);
   Graph_Graph182->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph182->GetXaxis()->SetTitleFont(43);
   Graph_Graph182->GetYaxis()->SetLabelFont(43);
   Graph_Graph182->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph182->GetYaxis()->SetLabelSize(14);
   Graph_Graph182->GetYaxis()->SetTitleSize(21);
   Graph_Graph182->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph182->GetYaxis()->SetTitleFont(43);
   Graph_Graph182->GetZaxis()->SetLabelFont(43);
   Graph_Graph182->GetZaxis()->SetLabelSize(14);
   Graph_Graph182->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph182->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph182);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,3.665825669,-0.1750044687);
   graph->SetPoint(1,3.665825669,0.03263597277);
   graph->SetPoint(2,4.413344004,0.03263597277);
   graph->SetPoint(3,4.413344004,-0.1750044687);
   graph->SetPoint(4,3.665825669,-0.1750044687);
   
   TH1F *Graph_Graph183 = new TH1F("Graph_Graph183","",100,3.591074,4.488096);
   Graph_Graph183->SetMinimum(-0.1957685);
   Graph_Graph183->SetMaximum(0.05340002);
   Graph_Graph183->SetDirectory(0);
   Graph_Graph183->SetStats(0);
   Graph_Graph183->SetLineStyle(0);
   Graph_Graph183->SetMarkerStyle(20);
   Graph_Graph183->SetMarkerSize(1.3);
   Graph_Graph183->GetXaxis()->SetLabelFont(43);
   Graph_Graph183->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph183->GetXaxis()->SetLabelSize(14);
   Graph_Graph183->GetXaxis()->SetTitleSize(21);
   Graph_Graph183->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph183->GetXaxis()->SetTitleFont(43);
   Graph_Graph183->GetYaxis()->SetLabelFont(43);
   Graph_Graph183->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph183->GetYaxis()->SetLabelSize(14);
   Graph_Graph183->GetYaxis()->SetTitleSize(21);
   Graph_Graph183->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph183->GetYaxis()->SetTitleFont(43);
   Graph_Graph183->GetZaxis()->SetLabelFont(43);
   Graph_Graph183->GetZaxis()->SetLabelSize(14);
   Graph_Graph183->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph183->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph183);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,4.413344004,-0.1728590474);
   graph->SetPoint(1,4.413344004,-0.02478892061);
   graph->SetPoint(2,5.313292846,-0.02478892061);
   graph->SetPoint(3,5.313292846,-0.1728590474);
   graph->SetPoint(4,4.413344004,-0.1728590474);
   
   TH1F *Graph_Graph184 = new TH1F("Graph_Graph184","",100,4.323349,5.403288);
   Graph_Graph184->SetMinimum(-0.1876661);
   Graph_Graph184->SetMaximum(-0.009981908);
   Graph_Graph184->SetDirectory(0);
   Graph_Graph184->SetStats(0);
   Graph_Graph184->SetLineStyle(0);
   Graph_Graph184->SetMarkerStyle(20);
   Graph_Graph184->SetMarkerSize(1.3);
   Graph_Graph184->GetXaxis()->SetLabelFont(43);
   Graph_Graph184->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph184->GetXaxis()->SetLabelSize(14);
   Graph_Graph184->GetXaxis()->SetTitleSize(21);
   Graph_Graph184->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph184->GetXaxis()->SetTitleFont(43);
   Graph_Graph184->GetYaxis()->SetLabelFont(43);
   Graph_Graph184->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph184->GetYaxis()->SetLabelSize(14);
   Graph_Graph184->GetYaxis()->SetTitleSize(21);
   Graph_Graph184->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph184->GetYaxis()->SetTitleFont(43);
   Graph_Graph184->GetZaxis()->SetLabelFont(43);
   Graph_Graph184->GetZaxis()->SetLabelSize(14);
   Graph_Graph184->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph184->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph184);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,5.313292846,-0.155830915);
   graph->SetPoint(1,5.313292846,-0.05122015573);
   graph->SetPoint(2,6.396755122,-0.05122015573);
   graph->SetPoint(3,6.396755122,-0.155830915);
   graph->SetPoint(4,5.313292846,-0.155830915);
   
   TH1F *Graph_Graph185 = new TH1F("Graph_Graph185","",100,5.204947,6.505101);
   Graph_Graph185->SetMinimum(-0.166292);
   Graph_Graph185->SetMaximum(-0.04075908);
   Graph_Graph185->SetDirectory(0);
   Graph_Graph185->SetStats(0);
   Graph_Graph185->SetLineStyle(0);
   Graph_Graph185->SetMarkerStyle(20);
   Graph_Graph185->SetMarkerSize(1.3);
   Graph_Graph185->GetXaxis()->SetLabelFont(43);
   Graph_Graph185->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph185->GetXaxis()->SetLabelSize(14);
   Graph_Graph185->GetXaxis()->SetTitleSize(21);
   Graph_Graph185->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph185->GetXaxis()->SetTitleFont(43);
   Graph_Graph185->GetYaxis()->SetLabelFont(43);
   Graph_Graph185->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph185->GetYaxis()->SetLabelSize(14);
   Graph_Graph185->GetYaxis()->SetTitleSize(21);
   Graph_Graph185->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph185->GetYaxis()->SetTitleFont(43);
   Graph_Graph185->GetZaxis()->SetLabelFont(43);
   Graph_Graph185->GetZaxis()->SetLabelSize(14);
   Graph_Graph185->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph185->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph185);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,6.396755122,-0.1290240563);
   graph->SetPoint(1,6.396755122,-0.05410395391);
   graph->SetPoint(2,7.701152051,-0.05410395391);
   graph->SetPoint(3,7.701152051,-0.1290240563);
   graph->SetPoint(4,6.396755122,-0.1290240563);
   
   TH1F *Graph_Graph186 = new TH1F("Graph_Graph186","",100,6.266315,7.831592);
   Graph_Graph186->SetMinimum(-0.1365161);
   Graph_Graph186->SetMaximum(-0.04661194);
   Graph_Graph186->SetDirectory(0);
   Graph_Graph186->SetStats(0);
   Graph_Graph186->SetLineStyle(0);
   Graph_Graph186->SetMarkerStyle(20);
   Graph_Graph186->SetMarkerSize(1.3);
   Graph_Graph186->GetXaxis()->SetLabelFont(43);
   Graph_Graph186->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph186->GetXaxis()->SetLabelSize(14);
   Graph_Graph186->GetXaxis()->SetTitleSize(21);
   Graph_Graph186->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph186->GetXaxis()->SetTitleFont(43);
   Graph_Graph186->GetYaxis()->SetLabelFont(43);
   Graph_Graph186->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph186->GetYaxis()->SetLabelSize(14);
   Graph_Graph186->GetYaxis()->SetTitleSize(21);
   Graph_Graph186->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph186->GetYaxis()->SetTitleFont(43);
   Graph_Graph186->GetZaxis()->SetLabelFont(43);
   Graph_Graph186->GetZaxis()->SetLabelSize(14);
   Graph_Graph186->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph186->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph186);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,7.701152051,-0.1128300016);
   graph->SetPoint(1,7.701152051,-0.05995780033);
   graph->SetPoint(2,9.27153561,-0.05995780033);
   graph->SetPoint(3,9.27153561,-0.1128300016);
   graph->SetPoint(4,7.701152051,-0.1128300016);
   
   TH1F *Graph_Graph187 = new TH1F("Graph_Graph187","",100,7.544114,9.428574);
   Graph_Graph187->SetMinimum(-0.1181172);
   Graph_Graph187->SetMaximum(-0.05467058);
   Graph_Graph187->SetDirectory(0);
   Graph_Graph187->SetStats(0);
   Graph_Graph187->SetLineStyle(0);
   Graph_Graph187->SetMarkerStyle(20);
   Graph_Graph187->SetMarkerSize(1.3);
   Graph_Graph187->GetXaxis()->SetLabelFont(43);
   Graph_Graph187->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph187->GetXaxis()->SetLabelSize(14);
   Graph_Graph187->GetXaxis()->SetTitleSize(21);
   Graph_Graph187->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph187->GetXaxis()->SetTitleFont(43);
   Graph_Graph187->GetYaxis()->SetLabelFont(43);
   Graph_Graph187->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph187->GetYaxis()->SetLabelSize(14);
   Graph_Graph187->GetYaxis()->SetTitleSize(21);
   Graph_Graph187->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph187->GetYaxis()->SetTitleFont(43);
   Graph_Graph187->GetZaxis()->SetLabelFont(43);
   Graph_Graph187->GetZaxis()->SetLabelSize(14);
   Graph_Graph187->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph187->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph187);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,9.27153561,-0.09145869235);
   graph->SetPoint(1,9.27153561,-0.04711718092);
   graph->SetPoint(2,11.16214457,-0.04711718092);
   graph->SetPoint(3,11.16214457,-0.09145869235);
   graph->SetPoint(4,9.27153561,-0.09145869235);
   
   TH1F *Graph_Graph188 = new TH1F("Graph_Graph188","",100,9.082475,11.35121);
   Graph_Graph188->SetMinimum(-0.09589284);
   Graph_Graph188->SetMaximum(-0.04268303);
   Graph_Graph188->SetDirectory(0);
   Graph_Graph188->SetStats(0);
   Graph_Graph188->SetLineStyle(0);
   Graph_Graph188->SetMarkerStyle(20);
   Graph_Graph188->SetMarkerSize(1.3);
   Graph_Graph188->GetXaxis()->SetLabelFont(43);
   Graph_Graph188->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph188->GetXaxis()->SetLabelSize(14);
   Graph_Graph188->GetXaxis()->SetTitleSize(21);
   Graph_Graph188->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph188->GetXaxis()->SetTitleFont(43);
   Graph_Graph188->GetYaxis()->SetLabelFont(43);
   Graph_Graph188->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph188->GetYaxis()->SetLabelSize(14);
   Graph_Graph188->GetYaxis()->SetTitleSize(21);
   Graph_Graph188->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph188->GetYaxis()->SetTitleFont(43);
   Graph_Graph188->GetZaxis()->SetLabelFont(43);
   Graph_Graph188->GetZaxis()->SetLabelSize(14);
   Graph_Graph188->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph188->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph188);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,11.16214457,-0.06110877918);
   graph->SetPoint(1,11.16214457,-0.02404680303);
   graph->SetPoint(2,13.43827783,-0.02404680303);
   graph->SetPoint(3,13.43827783,-0.06110877918);
   graph->SetPoint(4,11.16214457,-0.06110877918);
   
   TH1F *Graph_Graph189 = new TH1F("Graph_Graph189","",100,10.93453,13.66589);
   Graph_Graph189->SetMinimum(-0.06481498);
   Graph_Graph189->SetMaximum(-0.02034061);
   Graph_Graph189->SetDirectory(0);
   Graph_Graph189->SetStats(0);
   Graph_Graph189->SetLineStyle(0);
   Graph_Graph189->SetMarkerStyle(20);
   Graph_Graph189->SetMarkerSize(1.3);
   Graph_Graph189->GetXaxis()->SetLabelFont(43);
   Graph_Graph189->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph189->GetXaxis()->SetLabelSize(14);
   Graph_Graph189->GetXaxis()->SetTitleSize(21);
   Graph_Graph189->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph189->GetXaxis()->SetTitleFont(43);
   Graph_Graph189->GetYaxis()->SetLabelFont(43);
   Graph_Graph189->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph189->GetYaxis()->SetLabelSize(14);
   Graph_Graph189->GetYaxis()->SetTitleSize(21);
   Graph_Graph189->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph189->GetYaxis()->SetTitleFont(43);
   Graph_Graph189->GetZaxis()->SetLabelFont(43);
   Graph_Graph189->GetZaxis()->SetLabelSize(14);
   Graph_Graph189->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph189->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph189);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,13.43827783,-0.05106901834);
   graph->SetPoint(1,13.43827783,-0.019753652);
   graph->SetPoint(2,16.17854974,-0.019753652);
   graph->SetPoint(3,16.17854974,-0.05106901834);
   graph->SetPoint(4,13.43827783,-0.05106901834);
   
   TH1F *Graph_Graph190 = new TH1F("Graph_Graph190","",100,13.16425,16.45258);
   Graph_Graph190->SetMinimum(-0.05420055);
   Graph_Graph190->SetMaximum(-0.01662212);
   Graph_Graph190->SetDirectory(0);
   Graph_Graph190->SetStats(0);
   Graph_Graph190->SetLineStyle(0);
   Graph_Graph190->SetMarkerStyle(20);
   Graph_Graph190->SetMarkerSize(1.3);
   Graph_Graph190->GetXaxis()->SetLabelFont(43);
   Graph_Graph190->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph190->GetXaxis()->SetLabelSize(14);
   Graph_Graph190->GetXaxis()->SetTitleSize(21);
   Graph_Graph190->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph190->GetXaxis()->SetTitleFont(43);
   Graph_Graph190->GetYaxis()->SetLabelFont(43);
   Graph_Graph190->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph190->GetYaxis()->SetLabelSize(14);
   Graph_Graph190->GetYaxis()->SetTitleSize(21);
   Graph_Graph190->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph190->GetYaxis()->SetTitleFont(43);
   Graph_Graph190->GetZaxis()->SetLabelFont(43);
   Graph_Graph190->GetZaxis()->SetLabelSize(14);
   Graph_Graph190->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph190->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph190);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,16.17854974,-0.03901915069);
   graph->SetPoint(1,16.17854974,-0.01199806282);
   graph->SetPoint(2,19.4776053,-0.01199806282);
   graph->SetPoint(3,19.4776053,-0.03901915069);
   graph->SetPoint(4,16.17854974,-0.03901915069);
   
   TH1F *Graph_Graph191 = new TH1F("Graph_Graph191","",100,15.84864,19.80751);
   Graph_Graph191->SetMinimum(-0.04172126);
   Graph_Graph191->SetMaximum(-0.009295954);
   Graph_Graph191->SetDirectory(0);
   Graph_Graph191->SetStats(0);
   Graph_Graph191->SetLineStyle(0);
   Graph_Graph191->SetMarkerStyle(20);
   Graph_Graph191->SetMarkerSize(1.3);
   Graph_Graph191->GetXaxis()->SetLabelFont(43);
   Graph_Graph191->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph191->GetXaxis()->SetLabelSize(14);
   Graph_Graph191->GetXaxis()->SetTitleSize(21);
   Graph_Graph191->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph191->GetXaxis()->SetTitleFont(43);
   Graph_Graph191->GetYaxis()->SetLabelFont(43);
   Graph_Graph191->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph191->GetYaxis()->SetLabelSize(14);
   Graph_Graph191->GetYaxis()->SetTitleSize(21);
   Graph_Graph191->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph191->GetYaxis()->SetTitleFont(43);
   Graph_Graph191->GetZaxis()->SetLabelFont(43);
   Graph_Graph191->GetZaxis()->SetLabelSize(14);
   Graph_Graph191->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph191->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph191);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,19.4776053,-0.02376995359);
   graph->SetPoint(1,19.4776053,-0.0007457027875);
   graph->SetPoint(2,23.44938912,-0.0007457027875);
   graph->SetPoint(3,23.44938912,-0.02376995359);
   graph->SetPoint(4,19.4776053,-0.02376995359);
   
   TH1F *Graph_Graph192 = new TH1F("Graph_Graph192","",100,19.08043,23.84657);
   Graph_Graph192->SetMinimum(-0.02607238);
   Graph_Graph192->SetMaximum(0.001556722);
   Graph_Graph192->SetDirectory(0);
   Graph_Graph192->SetStats(0);
   Graph_Graph192->SetLineStyle(0);
   Graph_Graph192->SetMarkerStyle(20);
   Graph_Graph192->SetMarkerSize(1.3);
   Graph_Graph192->GetXaxis()->SetLabelFont(43);
   Graph_Graph192->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph192->GetXaxis()->SetLabelSize(14);
   Graph_Graph192->GetXaxis()->SetTitleSize(21);
   Graph_Graph192->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph192->GetXaxis()->SetTitleFont(43);
   Graph_Graph192->GetYaxis()->SetLabelFont(43);
   Graph_Graph192->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph192->GetYaxis()->SetLabelSize(14);
   Graph_Graph192->GetYaxis()->SetTitleSize(21);
   Graph_Graph192->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph192->GetYaxis()->SetTitleFont(43);
   Graph_Graph192->GetZaxis()->SetLabelFont(43);
   Graph_Graph192->GetZaxis()->SetLabelSize(14);
   Graph_Graph192->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph192->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph192);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,23.44938912,-0.01806093184);
   graph->SetPoint(1,23.44938912,-0.0002899543446);
   graph->SetPoint(2,28.23108087,-0.0002899543446);
   graph->SetPoint(3,28.23108087,-0.01806093184);
   graph->SetPoint(4,23.44938912,-0.01806093184);
   
   TH1F *Graph_Graph193 = new TH1F("Graph_Graph193","",100,22.97122,28.70925);
   Graph_Graph193->SetMinimum(-0.01983803);
   Graph_Graph193->SetMaximum(0.001487143);
   Graph_Graph193->SetDirectory(0);
   Graph_Graph193->SetStats(0);
   Graph_Graph193->SetLineStyle(0);
   Graph_Graph193->SetMarkerStyle(20);
   Graph_Graph193->SetMarkerSize(1.3);
   Graph_Graph193->GetXaxis()->SetLabelFont(43);
   Graph_Graph193->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph193->GetXaxis()->SetLabelSize(14);
   Graph_Graph193->GetXaxis()->SetTitleSize(21);
   Graph_Graph193->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph193->GetXaxis()->SetTitleFont(43);
   Graph_Graph193->GetYaxis()->SetLabelFont(43);
   Graph_Graph193->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph193->GetYaxis()->SetLabelSize(14);
   Graph_Graph193->GetYaxis()->SetTitleSize(21);
   Graph_Graph193->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph193->GetYaxis()->SetTitleFont(43);
   Graph_Graph193->GetZaxis()->SetLabelFont(43);
   Graph_Graph193->GetZaxis()->SetLabelSize(14);
   Graph_Graph193->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph193->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph193);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,28.23108087,-0.01142501331);
   graph->SetPoint(1,28.23108087,0.001926857563);
   graph->SetPoint(2,33.98783323,0.001926857563);
   graph->SetPoint(3,33.98783323,-0.01142501331);
   graph->SetPoint(4,28.23108087,-0.01142501331);
   
   TH1F *Graph_Graph194 = new TH1F("Graph_Graph194","",100,27.65541,34.56351);
   Graph_Graph194->SetMinimum(-0.0127602);
   Graph_Graph194->SetMaximum(0.003262045);
   Graph_Graph194->SetDirectory(0);
   Graph_Graph194->SetStats(0);
   Graph_Graph194->SetLineStyle(0);
   Graph_Graph194->SetMarkerStyle(20);
   Graph_Graph194->SetMarkerSize(1.3);
   Graph_Graph194->GetXaxis()->SetLabelFont(43);
   Graph_Graph194->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph194->GetXaxis()->SetLabelSize(14);
   Graph_Graph194->GetXaxis()->SetTitleSize(21);
   Graph_Graph194->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph194->GetXaxis()->SetTitleFont(43);
   Graph_Graph194->GetYaxis()->SetLabelFont(43);
   Graph_Graph194->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph194->GetYaxis()->SetLabelSize(14);
   Graph_Graph194->GetYaxis()->SetTitleSize(21);
   Graph_Graph194->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph194->GetYaxis()->SetTitleFont(43);
   Graph_Graph194->GetZaxis()->SetLabelFont(43);
   Graph_Graph194->GetZaxis()->SetLabelSize(14);
   Graph_Graph194->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph194->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph194);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,33.98783323,-0.008487280852);
   graph->SetPoint(1,33.98783323,0.001019547472);
   graph->SetPoint(2,40.9184761,0.001019547472);
   graph->SetPoint(3,40.9184761,-0.008487280852);
   graph->SetPoint(4,33.98783323,-0.008487280852);
   
   TH1F *Graph_Graph195 = new TH1F("Graph_Graph195","",100,33.29477,41.61154);
   Graph_Graph195->SetMinimum(-0.009437964);
   Graph_Graph195->SetMaximum(0.00197023);
   Graph_Graph195->SetDirectory(0);
   Graph_Graph195->SetStats(0);
   Graph_Graph195->SetLineStyle(0);
   Graph_Graph195->SetMarkerStyle(20);
   Graph_Graph195->SetMarkerSize(1.3);
   Graph_Graph195->GetXaxis()->SetLabelFont(43);
   Graph_Graph195->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph195->GetXaxis()->SetLabelSize(14);
   Graph_Graph195->GetXaxis()->SetTitleSize(21);
   Graph_Graph195->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph195->GetXaxis()->SetTitleFont(43);
   Graph_Graph195->GetYaxis()->SetLabelFont(43);
   Graph_Graph195->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph195->GetYaxis()->SetLabelSize(14);
   Graph_Graph195->GetYaxis()->SetTitleSize(21);
   Graph_Graph195->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph195->GetYaxis()->SetTitleFont(43);
   Graph_Graph195->GetZaxis()->SetLabelFont(43);
   Graph_Graph195->GetZaxis()->SetLabelSize(14);
   Graph_Graph195->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph195->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph195);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,40.9184761,-0.004216966475);
   graph->SetPoint(1,40.9184761,0.002141389073);
   graph->SetPoint(2,49.26238383,0.002141389073);
   graph->SetPoint(3,49.26238383,-0.004216966475);
   graph->SetPoint(4,40.9184761,-0.004216966475);
   
   TH1F *Graph_Graph196 = new TH1F("Graph_Graph196","",100,40.08409,50.09677);
   Graph_Graph196->SetMinimum(-0.004852802);
   Graph_Graph196->SetMaximum(0.002777225);
   Graph_Graph196->SetDirectory(0);
   Graph_Graph196->SetStats(0);
   Graph_Graph196->SetLineStyle(0);
   Graph_Graph196->SetMarkerStyle(20);
   Graph_Graph196->SetMarkerSize(1.3);
   Graph_Graph196->GetXaxis()->SetLabelFont(43);
   Graph_Graph196->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph196->GetXaxis()->SetLabelSize(14);
   Graph_Graph196->GetXaxis()->SetTitleSize(21);
   Graph_Graph196->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph196->GetXaxis()->SetTitleFont(43);
   Graph_Graph196->GetYaxis()->SetLabelFont(43);
   Graph_Graph196->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph196->GetYaxis()->SetLabelSize(14);
   Graph_Graph196->GetYaxis()->SetTitleSize(21);
   Graph_Graph196->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph196->GetYaxis()->SetTitleFont(43);
   Graph_Graph196->GetZaxis()->SetLabelFont(43);
   Graph_Graph196->GetZaxis()->SetLabelSize(14);
   Graph_Graph196->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph196->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph196);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,49.26238383,-0.001868768412);
   graph->SetPoint(1,49.26238383,0.002068932513);
   graph->SetPoint(2,59.30774291,0.002068932513);
   graph->SetPoint(3,59.30774291,-0.001868768412);
   graph->SetPoint(4,49.26238383,-0.001868768412);
   
   TH1F *Graph_Graph197 = new TH1F("Graph_Graph197","",100,48.25785,60.31228);
   Graph_Graph197->SetMinimum(-0.002262539);
   Graph_Graph197->SetMaximum(0.002462703);
   Graph_Graph197->SetDirectory(0);
   Graph_Graph197->SetStats(0);
   Graph_Graph197->SetLineStyle(0);
   Graph_Graph197->SetMarkerStyle(20);
   Graph_Graph197->SetMarkerSize(1.3);
   Graph_Graph197->GetXaxis()->SetLabelFont(43);
   Graph_Graph197->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph197->GetXaxis()->SetLabelSize(14);
   Graph_Graph197->GetXaxis()->SetTitleSize(21);
   Graph_Graph197->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph197->GetXaxis()->SetTitleFont(43);
   Graph_Graph197->GetYaxis()->SetLabelFont(43);
   Graph_Graph197->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph197->GetYaxis()->SetLabelSize(14);
   Graph_Graph197->GetYaxis()->SetTitleSize(21);
   Graph_Graph197->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph197->GetYaxis()->SetTitleFont(43);
   Graph_Graph197->GetZaxis()->SetLabelFont(43);
   Graph_Graph197->GetZaxis()->SetLabelSize(14);
   Graph_Graph197->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph197->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph197);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,59.30774291,-0.0005104197361);
   graph->SetPoint(1,59.30774291,0.001744969119);
   graph->SetPoint(2,71.40150548,0.001744969119);
   graph->SetPoint(3,71.40150548,-0.0005104197361);
   graph->SetPoint(4,59.30774291,-0.0005104197361);
   
   TH1F *Graph_Graph198 = new TH1F("Graph_Graph198","",100,58.09837,72.61088);
   Graph_Graph198->SetMinimum(-0.0007359586);
   Graph_Graph198->SetMaximum(0.001970508);
   Graph_Graph198->SetDirectory(0);
   Graph_Graph198->SetStats(0);
   Graph_Graph198->SetLineStyle(0);
   Graph_Graph198->SetMarkerStyle(20);
   Graph_Graph198->SetMarkerSize(1.3);
   Graph_Graph198->GetXaxis()->SetLabelFont(43);
   Graph_Graph198->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph198->GetXaxis()->SetLabelSize(14);
   Graph_Graph198->GetXaxis()->SetTitleSize(21);
   Graph_Graph198->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph198->GetXaxis()->SetTitleFont(43);
   Graph_Graph198->GetYaxis()->SetLabelFont(43);
   Graph_Graph198->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph198->GetYaxis()->SetLabelSize(14);
   Graph_Graph198->GetYaxis()->SetTitleSize(21);
   Graph_Graph198->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph198->GetYaxis()->SetTitleFont(43);
   Graph_Graph198->GetZaxis()->SetLabelFont(43);
   Graph_Graph198->GetZaxis()->SetLabelSize(14);
   Graph_Graph198->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph198->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph198);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,71.40150548,-0.0007305987579);
   graph->SetPoint(1,71.40150548,0.00130709558);
   graph->SetPoint(2,85.96137257,0.00130709558);
   graph->SetPoint(3,85.96137257,-0.0007305987579);
   graph->SetPoint(4,71.40150548,-0.0007305987579);
   
   TH1F *Graph_Graph199 = new TH1F("Graph_Graph199","",100,69.94552,87.41736);
   Graph_Graph199->SetMinimum(-0.0009343682);
   Graph_Graph199->SetMaximum(0.001510865);
   Graph_Graph199->SetDirectory(0);
   Graph_Graph199->SetStats(0);
   Graph_Graph199->SetLineStyle(0);
   Graph_Graph199->SetMarkerStyle(20);
   Graph_Graph199->SetMarkerSize(1.3);
   Graph_Graph199->GetXaxis()->SetLabelFont(43);
   Graph_Graph199->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph199->GetXaxis()->SetLabelSize(14);
   Graph_Graph199->GetXaxis()->SetTitleSize(21);
   Graph_Graph199->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph199->GetXaxis()->SetTitleFont(43);
   Graph_Graph199->GetYaxis()->SetLabelFont(43);
   Graph_Graph199->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph199->GetYaxis()->SetLabelSize(14);
   Graph_Graph199->GetYaxis()->SetTitleSize(21);
   Graph_Graph199->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph199->GetYaxis()->SetTitleFont(43);
   Graph_Graph199->GetZaxis()->SetLabelFont(43);
   Graph_Graph199->GetZaxis()->SetLabelSize(14);
   Graph_Graph199->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph199->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph199);
   
   graph->Draw("f ");
   
   graph = new TGraph(5);
   graph->SetName("");
   graph->SetTitle("");
   graph->SetFillColor(90);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.3);
   graph->SetPoint(0,85.96137257,0);
   graph->SetPoint(1,85.96137257,0.0003206776975);
   graph->SetPoint(2,103.4902209,0.0003206776975);
   graph->SetPoint(3,103.4902209,0);
   graph->SetPoint(4,85.96137257,0);
   
   TH1F *Graph_Graph200 = new TH1F("Graph_Graph200","",100,84.20849,105.2431);
   Graph_Graph200->SetMinimum(0);
   Graph_Graph200->SetMaximum(0.0003527455);
   Graph_Graph200->SetDirectory(0);
   Graph_Graph200->SetStats(0);
   Graph_Graph200->SetLineStyle(0);
   Graph_Graph200->SetMarkerStyle(20);
   Graph_Graph200->SetMarkerSize(1.3);
   Graph_Graph200->GetXaxis()->SetLabelFont(43);
   Graph_Graph200->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph200->GetXaxis()->SetLabelSize(14);
   Graph_Graph200->GetXaxis()->SetTitleSize(21);
   Graph_Graph200->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph200->GetXaxis()->SetTitleFont(43);
   Graph_Graph200->GetYaxis()->SetLabelFont(43);
   Graph_Graph200->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph200->GetYaxis()->SetLabelSize(14);
   Graph_Graph200->GetYaxis()->SetTitleSize(21);
   Graph_Graph200->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph200->GetYaxis()->SetTitleFont(43);
   Graph_Graph200->GetZaxis()->SetLabelFont(43);
   Graph_Graph200->GetZaxis()->SetLabelSize(14);
   Graph_Graph200->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph200->GetZaxis()->SetTitleFont(43);
   graph->SetHistogram(Graph_Graph200);
   
   graph->Draw("f ");
}