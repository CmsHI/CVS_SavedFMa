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

void finalDiJetFF(int doMC=1,
    const char * inFile0Name="anaDiJetFF.root",
    TString AnaName = "mcuq120V0",
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
  CPlot::sOutDir = anaoutdir+"/final";
  // Save output
  TFile * outf = new TFile(Form("%s/finalDiJetFF.root",anaoutdir.Data()),"RECREATE");

  // === Get Histograms ===
  //  -- t0 --
  HisTGroup<TH1D> hgMcGenTruthSigXi("hgMcGenTruthSigXi");
  hgMcGenTruthSigXi.Add(inFile0,"hSig_mcGenTruthNrXi","Nr");
  hgMcGenTruthSigXi.Add(inFile0,"hSig_mcGenTruthAwXi","Aw");
  hgMcGenTruthSigXi.Average();

  HisTGroup<TH1D> hgMcGenSigXi("hgMcGenSigXi");
  hgMcGenSigXi.Add(inFile0,"hSig_mcGenNrXi","Nr");
  hgMcGenSigXi.Add(inFile0,"hSig_mcGenAwXi","Aw");
  hgMcGenSigXi.Average();

  HisTGroup<TH1D> hgMcj2t0SigXi("hgMcj2t0SigXi");
  hgMcj2t0SigXi.Add(inFile0,"hSig_mcj2t0NrXi","Nr");
  hgMcj2t0SigXi.Add(inFile0,"hSig_mcj2t0AwXi","Aw");
  hgMcj2t0SigXi.Average();

  HisTGroup<TH1D> hgMcj2t0SelRefSigXi("hgMcj2t0SelRefSigXi");
  hgMcj2t0SelRefSigXi.Add(inFile0,"hSig_mcj2t0SelRefNrXi","Nr");
  hgMcj2t0SelRefSigXi.Add(inFile0,"hSig_mcj2t0SelRefAwXi","Aw");
  hgMcj2t0SelRefSigXi.Average();

  //  -- t3 --
  HisTGroup<TH1D> hgMcRecoSigXi("hgMcRecoSigXi");
  hgMcRecoSigXi.Add(inFile0,"hSig_mcRecoNrXi","Nr");
  hgMcRecoSigXi.Add(inFile0,"hSig_mcRecoAwXi","Aw");
  hgMcRecoSigXi.Average();

  TH1D * hFrame = (TH1D*)hgMcGenTruthSigXi.GetH("Nr")->Clone("hFrame");
  hFrame->Scale(0);

  // === FF comparison ===
  HisTGroup<TH1D> hgCompSigXi("hgCompSigXi");
  hgCompSigXi.Add(hgMcRecoSigXi.R("Ave"),"j2t3");
  hgCompSigXi.Add(hgMcj2t0SigXi.R("Ave"),"j2t1");
  hgCompSigXi.Add(hgMcRecoSigXi.R("Ave"),"j2t3Corr",1./0.7);
  hgCompSigXi.Divide("j2t3","j2t1");

  TCanvas * cRecoDivGen = new TCanvas("cRecoDivGen","cRecoDivGen",500,500);
  CPlot cpRecoDivGen("RecoDivGen","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","FF_{RecoTrk}/FF_{GenTrk}");
  cpRecoDivGen.SetYRange(0,1.2);
  cpRecoDivGen.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpRecoDivGen.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpRecoDivGen.AddHist1D(hgCompSigXi.R("j2t3Divj2t1"),"HI: CaloJet FF(Raw-Bkg) Ratio","E",kBlue,kFullCircle);
  cpRecoDivGen.SetLegend(0.23,0.18,0.55,0.40);
  cpRecoDivGen.Draw(cRecoDivGen,true);

  // === FF Measurement Plots ===
  TCanvas * cT3FF = new TCanvas("cT3FF","cT3FF",500,500);
  CPlot cpT3FF("T3FF","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpT3FF.SetYRange(0,6);
  cpT3FF.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpT3FF.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpT3FF.AddHist1D(hgMcGenTruthSigXi.R("Ave"),"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpT3FF.AddHist1D(hgMcj2t0SigXi.R("Ave"),"HI: calojet1,2 + gentrk","E",kBlue,kFullCircle);
  cpT3FF.AddHist1D(hgMcRecoSigXi.R("Ave"),"HI: calojet1,2 + recotrk","E",kBlack,kFullCircle);
  cpT3FF.AddHist1D(hgCompSigXi.H("j2t3Corr"),"HI: (calojet1,2 + recotrk)/0.7","E",kBlue+2,kOpenSquare);
  cpT3FF.SetLegend(0.194,0.64,0.52,0.94);
  cpT3FF.Draw(cT3FF,true);

  TCanvas * cT0FF = new TCanvas("cT0FF","cT0FF",500,500);
  CPlot cpT0FF("T0FF","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpT0FF.SetYRange(0,6);
  cpT0FF.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpT0FF.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpT0FF.AddHist1D(hgMcGenTruthSigXi.R("Ave"),"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpT0FF.AddHist1D(hgMcGenSigXi.R("Ave"),"HI: genjet1,2 + gentrk","E",kRed+2,kOpenSquare);
  cpT0FF.AddHist1D(hgMcj2t0SigXi.R("Ave"),"HI: calojet1,2 + gentrk","E",kBlue,kFullCircle);
  cpT0FF.AddHist1D(hgMcj2t0SelRefSigXi.R("Ave"),"HI: calojet1,2 (genJES Sel)+ gentrk","E",kGreen-2,kOpenStar);
  cpT0FF.SetLegend(0.194,0.64,0.52,0.94);
  cpT0FF.Draw(cT0FF,true);

  // All done, save and exit
  outf->Write();
}
