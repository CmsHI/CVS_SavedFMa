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

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/AnaFrag.h"
#include "analysis/root/macros/cplot/CPlot.h"           // helper class for plots
#include "analysis/root/macros/histogram/HisTGroup.h"

#include "Saved/DiJetAna/anaJEx/selectionCut.h"
using namespace std;

void finalDiJetFF(int doMC=0,
    const char * inFile0Name="anaDiJetFF.root",
    TString AnaName = "ZSHd/dp25/a0",
    TString header="July Data (Hard Triggered)",
    //
    TString title1="Data",
    TString title2="MC")
{
  // Define Inputs
  selectionCut mcAna(AnaName,doMC,1,100,200,50,2.5);
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
  HisTGroup<TH1D> hgRecoSigXi("RecoSigXi");
  hgRecoSigXi.Add(inFile0,"hSig_recoNrXi","Nr");
  hgRecoSigXi.Add(inFile0,"hSig_recoAwXi","Aw");
  hgRecoSigXi.Average();

  TH1D * hFrame = (TH1D*)hgRecoSigXi.GetH("Nr")->Clone("hFrame");
  hFrame->Scale(0);

  if (doMC) {
    //  -- t0 --
    HisTGroup<TH1D> hgMcGenTruthSigXi("McGenTruthSigXi");
    hgMcGenTruthSigXi.Add(inFile0,"hSig_mcGenTruthNrXi","Nr");
    hgMcGenTruthSigXi.Add(inFile0,"hSig_mcGenTruthAwXi","Aw");
    hgMcGenTruthSigXi.Average();

    HisTGroup<TH1D> hgMcGenSigXi("McGenSigXi");
    hgMcGenSigXi.Add(inFile0,"hSig_mcGenNrXi","Nr");
    hgMcGenSigXi.Add(inFile0,"hSig_mcGenAwXi","Aw");
    hgMcGenSigXi.Average();

    HisTGroup<TH1D> hgMcj2t0SigXi("Mcj2t0SigXi");
    hgMcj2t0SigXi.Add(inFile0,"hSig_mcj2t0NrXi","Nr");
    hgMcj2t0SigXi.Add(inFile0,"hSig_mcj2t0AwXi","Aw");
    hgMcj2t0SigXi.Average();

    HisTGroup<TH1D> hgMcj2t0SelRefSigXi("Mcj2t0SelRefSigXi");
    hgMcj2t0SelRefSigXi.Add(inFile0,"hSig_mcj2t0SelRefNrXi","Nr");
    hgMcj2t0SelRefSigXi.Add(inFile0,"hSig_mcj2t0SelRefAwXi","Aw");
    hgMcj2t0SelRefSigXi.Average();
  }


  // === FF comparison ===
  HisTGroup<TH1D> hgCompSigXi("CompSigXi");

  // === FF Measurement Plots ===
  TCanvas * cFinalFF = new TCanvas("cFinalFF","cFinalFF",500,500);
  CPlot cpFinalFF("FinalFF","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFinalFF.SetYRange(0,6);
  cpFinalFF.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  cpFinalFF.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<200GeV, 50GeV<p_{T}^{jet2}","",0,0);
  cpFinalFF.AddHist1D(hgRecoSigXi.R("Ave"),"July Hard: Reco DiJet FF","E",kBlack,kFullCircle);
  cpFinalFF.SetLegend(0.194,0.74,0.52,0.94);
  cpFinalFF.Draw(cFinalFF,true);

  // All done, save and exit
  outf->Write();
}
