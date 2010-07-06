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
#include "Saved/DiJetAna/ana02/aliases_dijet.C"
#include "Saved/DiJetAna/ana02/selectionCut.h"
#include "Saved/DiJetAna/ana02/AnaFrag.h"
#include "Saved/DiJetAna/ana02/compareHist.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"
using namespace std;

void plotCheckDiJetFF(int doMC=1,
    const char * inFile0Name="checkDiJetFF.root",
    TString AnaName = "mcuq120V3/a3",
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

  //   - Gen -
  HisTGroup<TH1D> hgMcGenSigXi("McGenSigXi");
  hgMcGenSigXi.Add(inFile0,"hSig_mcGenNrXi","Nr");
  hgMcGenSigXi.Add(inFile0,"hSig_mcGenAwXi","Aw");
  hgMcGenSigXi.Average();

  HisTGroup<TH1D> hgMcGenMatSigXi("McGenMatSigXi");
  hgMcGenMatSigXi.Add(inFile0,"hSig_mcGenMatNrXi","Nr");
  hgMcGenMatSigXi.Add(inFile0,"hSig_mcGenMatAwXi","Aw");
  hgMcGenMatSigXi.Average();

  //   - check trees -
  HisTGroup<TH1D> hgMcj2t0SelRLRAnaRLRSigXi("Mcj2t0SelRLRAnaRLRSigXi");
  hgMcj2t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAnaRLRNrXi","Nr");
  hgMcj2t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAnaRLRAwXi","Aw");
  hgMcj2t0SelRLRAnaRLRSigXi.Average();

  HisTGroup<TH1D> hgMcj1t0SelRLRAnaRLRSigXi("Mcj1t0SelRLRAnaRLRSigXi");
  hgMcj1t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj1t0SelRLRAnaRLRNrXi","Nr");
  hgMcj1t0SelRLRAnaRLRSigXi.Add(inFile0,"hSig_mcj1t0SelRLRAnaRLRAwXi","Aw");
  hgMcj1t0SelRLRAnaRLRSigXi.Average();

  // === Chk 0 ===
  HisTGroup<TH1D> hgMcj1t0MatAnaRSigXi("Mcj1t0MatAnaRSigXi");
  hgMcj1t0MatAnaRSigXi.Add(inFile0,"hSig_mcj1t0MatAnaRNrXi","Nr");
  hgMcj1t0MatAnaRSigXi.Add(inFile0,"hSig_mcj1t0MatAnaRAwXi","Aw");
  hgMcj1t0MatAnaRSigXi.Average();

  HisTGroup<TH1D> hgMcj2t0SelRLRSigXi("Mcj2t0SelRLRSigXi");
  hgMcj2t0SelRLRSigXi.Add(inFile0,"hSig_mcj2t0SelRLRNrXi","Nr");
  hgMcj2t0SelRLRSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAwXi","Aw");
  hgMcj2t0SelRLRSigXi.Average();

  HisTGroup<TH1D> hgMcj2t0SelRLRAnaRLSigXi("Mcj2t0SelRLRAnaRLSigXi");
  hgMcj2t0SelRLRAnaRLSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAnaRLNrXi","Nr");
  hgMcj2t0SelRLRAnaRLSigXi.Add(inFile0,"hSig_mcj2t0SelRLRAnaRLAwXi","Aw");
  hgMcj2t0SelRLRAnaRLSigXi.Average();

  // === Reco Jet ====
  HisTGroup<TH1D> hgMcj2t0SigXi("Mcj2t0SigXi");
  hgMcj2t0SigXi.Add(inFile0,"hSig_mcj2t0NrXi","Nr");
  hgMcj2t0SigXi.Add(inFile0,"hSig_mcj2t0AwXi","Aw");
  hgMcj2t0SigXi.Average();

  HisTGroup<TH1D> hgMcj2t0MatSigXi("Mcj2t0MatSigXi");
  hgMcj2t0MatSigXi.Add(inFile0,"hSig_mcj2t0MatNrXi","Nr");
  hgMcj2t0MatSigXi.Add(inFile0,"hSig_mcj2t0MatAwXi","Aw");
  hgMcj2t0MatSigXi.Average();

  // === FF comparison ===
  HisTGroup<TH1D> hgCompSigXi("CompSigXi");

  // === Check Tree j1t0 ===
  TCanvas * cChkTreeJ1T0 = new TCanvas("cChkTreeJ1T0","cChkTreeJ1T0",500,500);
  CPlot cpChkTreeJ1T0("ChkTreeJ1T0","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpChkTreeJ1T0.SetYRange(0,6);
  cpChkTreeJ1T0.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpChkTreeJ1T0.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpChkTreeJ1T0.AddHist1D(hgMcGenTruthSigXi.R("Ave"),"Signal: genjet1,2 + gentrk","histE",kRed,0);
  cpChkTreeJ1T0.AddHist1D(hgMcGenSigXi.R("Ave"),"HI: getjet1,2 + gentrk","E",kGreen-2,kOpenStar);
  cpChkTreeJ1T0.AddHist1D(hgMcGenMatSigXi.R("Ave"),"HI: genjet1,2 (Mat) + gentrk","E",kRed+2,kOpenSquare);
  cpChkTreeJ1T0.AddHist1D(hgMcj2t0SelRLRAnaRLRSigXi.R("Ave"),"HI: calojet1,2 SelRLRAnaRLR + gentrk","E",kBlue,kFullCircle);
  cpChkTreeJ1T0.SetLegend(0.194,0.64,0.52,0.94);
  cpChkTreeJ1T0.Draw(cChkTreeJ1T0,true);

  // === Check Tree j2t0 ===
  TCanvas * cChkTreeJ2T0 = new TCanvas("cChkTreeJ2T0","cChkTreeJ2T0",500,500);
  CPlot cpChkTreeJ2T0("ChkTreeJ2T0","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpChkTreeJ2T0.SetYRange(0,6);
  cpChkTreeJ2T0.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpChkTreeJ2T0.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpChkTreeJ2T0.AddHist1D(hgMcj2t0SigXi.R("Ave"),"HI: calojet1,2 + gentrk","E",kGreen-2,kOpenStar);
  cpChkTreeJ2T0.AddHist1D(hgMcj2t0MatSigXi.R("Ave"),"HI: calojet1,2 Mat + gentrk","E",kRed+2,kOpenSquare);
  cpChkTreeJ2T0.AddHist1D(hgMcj1t0SelRLRAnaRLRSigXi.R("Ave"),"HI: genjet1,2 SelRLRAnaRLR + gentrk","E",kBlue,kFullCircle);
  cpChkTreeJ2T0.SetLegend(0.194,0.64,0.52,0.94);
  cpChkTreeJ2T0.Draw(cChkTreeJ2T0,true);

  // === Exclusive Check of JES on FF Calc ===
  TCanvas * cChk0Jes = new TCanvas("cChk0Jes","cChk0Jes",500,500);
  CPlot cpChk0Jes("Chk0Jes","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpChk0Jes.SetYRange(0,6);
  cpChk0Jes.AddHist1D(hFrame,"Centrality: 0-30\%","",0,0);
  cpChk0Jes.AddHist1D(hFrame,"120GeV<p_{T}^{jet1}<170GeV","",0,0);
  cpChk0Jes.AddHist1D(hgMcGenMatSigXi.R("Ave"),"HI: genjet1,2 (Mat) + gentrk","histE",kRed,0,0,7);
  cpChk0Jes.AddHist1D(hgMcj1t0MatAnaRSigXi.R("Ave"),"HI: genjet1,2 AnaR + gentrk (FF: calo JES)","E",kGreen-2,kOpenStar,1.2);
  cpChk0Jes.AddHist1D(hgMcj2t0SelRLRAnaRLSigXi.R("Ave"),"HI: calojet1,2 SelRLRAnaRL + gentrk","E",kRed+2,kOpenSquare);
  cpChk0Jes.AddHist1D(hgMcj2t0SelRLRSigXi.R("Ave"),"HI: calojet1,2 SelRLR + gentrk","E",kBlue,kFullCircle);
  cpChk0Jes.SetLegend(0.194,0.64,0.52,0.94);
  cpChk0Jes.Draw(cChk0Jes,true);

  // All done, save and exit
  outf->Write();
}
