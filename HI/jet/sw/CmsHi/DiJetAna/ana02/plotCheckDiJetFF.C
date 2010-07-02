#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TMath.h"
#include "CmsHi/DiJetAna/ana02/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana02/selectionCut.h"
#include "CmsHi/DiJetAna/ana02/AnaFrag.h"
#include "CmsHi/DiJetAna/ana02/compareHist.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"
using namespace std;

void plotCheckDiJetFF(int doMC=1,
    const char * inFile0Name="checkDiJetFF.root",
    TString AnaName = "mcuq120V3_1",
    TString header="Hydjet2.76TeV+DiJet(120-170GeV)",
    //
    TString title1="MC Calojet",
    TString title2="MC Genjet")
{
  // Define Inputs
  selectionCut mcAna(AnaName,doMC,1,120,170,80);
  cout << "======= Inputs: ========" << endl;
  TString indir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  TString inFile0Path = indir+"/"+inFile0Name;
  cout << inFile0Path << endl;
  TFile * inFile0 = new TFile(inFile0Path);
  inFile0->ls();

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = indir;
  cout << "Output dir: " << anaoutdir << endl;
  CPlot::sOutDir = anaoutdir+"/chkff";
  // Save output
  TFile * outf = new TFile(Form("%s/plotCheckDiJetFF.root",anaoutdir.Data()),"RECREATE");

  // === Get Histograms ===
  //   - mc input -
  HisTGroup<TH1D> hgMcGenTruthSigXi("McGenTruthSigXi");
  hgMcGenTruthSigXi.Add(inFile0,"hSig_mcGenTruthNrXi","Nr");
  hgMcGenTruthSigXi.Add(inFile0,"hSig_mcGenTruthAwXi","Aw");
  hgMcGenTruthSigXi.Average();

  TH1D * hFrame = (TH1D*)hgMcGenTruthSigXi.GetH("Nr")->Clone("hFrame");
  hFrame->Scale(0);
  hFrame->SetTitle("");

  HisTGroup<TH1D> hgMcGenSigXi("McGenSigXi");
  hgMcGenSigXi.Add(inFile0,"hSig_mcGenNrXi","Nr");
  hgMcGenSigXi.Add(inFile0,"hSig_mcGenAwXi","Aw");
  hgMcGenSigXi.Average();

  //   - check trees -
  HisTGroup<TH1D> hgMcGenMatSigXi("McGenMatSigXi");
  hgMcGenMatSigXi.Add(inFile0,"hSig_mcGenMatNrXi","Nr");
  hgMcGenMatSigXi.Add(inFile0,"hSig_mcGenMatAwXi","Aw");
  hgMcGenMatSigXi.Average();

  HisTGroup<TH1D> hgMcj2t0MatSigXi("Mcj2t0MatSigXi");
  hgMcj2t0MatSigXi.Add(inFile0,"hSig_mcj2t0MatNrXi","Nr");
  hgMcj2t0MatSigXi.Add(inFile0,"hSig_mcj2t0MatAwXi","Aw");
  hgMcj2t0MatSigXi.Average();

  HisTGroup<TH1D> hgMcj2t0SelRLRAnaRLRSigXi("Mcj2t0SelRLRAnaRLRSigXi");
  hgMcj2t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAnaRLRNrXi","Nr");
  hgMcj2t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAnaRLRAwXi","Aw");
  hgMcj2t0SelRLRAnaRLRSigXi.Average();

  HisTGroup<TH1D> hgMcj1t0SelRLRAnaRLRSigXi("Mcj1t0SelRLRAnaRLRSigXi");
  hgMcj1t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj1t0SelRLRAnaRLRNrXi","Nr");
  hgMcj1t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj1t0SelRLRAnaRLRAwXi","Aw");
  hgMcj1t0SelRLRAnaRLRSigXi.Average();

  // === FF comparison ===
  HisTGroup<TH1D> hgCompSigXi("CompSigXi");

  // === Check Tree j1t0 ===
  TCanvas * cT0FF = new TCanvas("cT0FF","cT0FF",500,500);
  CPlot cpT0FF("T0FF","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpT0FF.SetYRange(0,6);
  cpT0FF.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpT0FF.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpT0FF.AddHist1D(hgMcGenTruthSigXi.R("Ave"),"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpT0FF.AddHist1D(hgMcGenSigXi.R("Ave"),"HI: getjet1,2 + gentrk","E",kGreen-2,kOpenStar);
  cpT0FF.AddHist1D(hgMcGenMatSigXi.R("Ave"),"HI: genjet1,2 (Mat) + gentrk","E",kRed+2,kOpenSquare);
  cpT0FF.AddHist1D(hgMcj2t0SelRLRAnaRLRSigXi.R("Ave"),"HI: calojet1,2 SelRLRAnaRLR + gentrk","E",kBlue,kFullCircle);
  cpT0FF.SetLegend(0.194,0.64,0.52,0.94);
  cpT0FF.Draw(cT0FF,true);

  // All done, save and exit
  outf->Write();
}
