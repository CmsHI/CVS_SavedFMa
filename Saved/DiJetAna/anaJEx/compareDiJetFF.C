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

void compareDiJetFF(int doMC=0,
    const char * inFile0Name="anaDiJetFF.root",
    TString AnaName = "ZSHd1/dp25/a3",
    TString Ana2Name = "hyjNZSDJ/dp25/a3",
    TString header="July Data (Hard Triggered)",
    //
    TString title1="Data",
    TString title2="MC")
{
  // Define Inputs
  selectionCut mcAna(AnaName,doMC,1,100,170,50,2.5);
  cout << "======= Inputs: ========" << endl;
  TString indir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  TString inFile0Path = indir+"/"+inFile0Name;
  cout << inFile0Path << endl;
  TFile * inFile0 = new TFile(inFile0Path);
  inFile0->ls();

  TFile * inFile2;
  bool doCompare = true;
  if (doCompare) {
    TString indir2 = Form("plots/%s/%s",Ana2Name.Data(),mcAna.AnaTag.Data());
    TString inFile2Path = indir2+"/"+inFile0Name;
    cout << inFile2Path << endl;
    inFile2 = new TFile(inFile2Path);
    inFile2->ls();
  }

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = indir;
  cout << "Output dir: " << anaoutdir << endl;
  CPlot::sOutDir = anaoutdir+"/cmpff";
  // Save output
  TFile * outf = new TFile(Form("%s/compareDiJetFF.root",anaoutdir.Data()),"RECREATE");

  // === Get Histograms ===
  // -- Ana --
  HisTGroup<TH1D> hgRecoRawXi("RecoRawXi");
  hgRecoRawXi.Add(inFile0,"hRaw_recoNrXi","Nr");
  hgRecoRawXi.Add(inFile0,"hRaw_recoAwXi","Aw");
  hgRecoRawXi.Average();

  HisTGroup<TH1D> hgRecoBkgXi("RecoBkgXi");
  hgRecoBkgXi.Add(inFile0,"hBkg_recoNrXi","Nr");
  hgRecoBkgXi.Add(inFile0,"hBkg_recoAwXi","Aw");
  hgRecoBkgXi.Average();

  HisTGroup<TH1D> hgRecoSigXi("RecoSigXi");
  hgRecoSigXi.Add(inFile0,"hSig_recoNrXi","Nr");
  hgRecoSigXi.Add(inFile0,"hSig_recoAwXi","Aw");
  hgRecoSigXi.Average();

  TH1D * hFrame = (TH1D*)hgRecoSigXi.GetH("Nr")->Clone("hFrame");
  hFrame->Scale(0);

  // -- Systematics --
  HisTGroup<TH1D> hgRecoUpperSigXi("RecoUpperSigXi");
  hgRecoUpperSigXi.Add(inFile0,"hSig_recoUpperNrXi","Nr");
  hgRecoUpperSigXi.Add(inFile0,"hSig_recoUpperAwXi","Aw");
  hgRecoUpperSigXi.Average();

  HisTGroup<TH1D> hgRecoLowerSigXi("RecoLowerSigXi");
  hgRecoLowerSigXi.Add(inFile0,"hSig_recoLowerNrXi","Nr");
  hgRecoLowerSigXi.Add(inFile0,"hSig_recoLowerAwXi","Aw");
  hgRecoLowerSigXi.Average();

  // -- Compare --
  HisTGroup<TH1D> hgReco2RawXi("Reco2RawXi");
  HisTGroup<TH1D> hgReco2BkgXi("Reco2BkgXi");
  HisTGroup<TH1D> hgReco2SigXi("Reco2SigXi");
  if (doCompare) {
    hgReco2RawXi.Add(inFile2,"hRaw_recoNrXi","Nr");
    hgReco2RawXi.Add(inFile2,"hRaw_recoAwXi","Aw");
    hgReco2RawXi.Average();

    hgReco2BkgXi.Add(inFile2,"hBkg_recoNrXi","Nr");
    hgReco2BkgXi.Add(inFile2,"hBkg_recoAwXi","Aw");
    hgReco2BkgXi.Average();

    hgReco2SigXi.Add(inFile2,"hSig_recoNrXi","Nr");
    hgReco2SigXi.Add(inFile2,"hSig_recoAwXi","Aw");
    hgReco2SigXi.Average();
  }

  HisTGroup<TH1D> hgMcGenTruthSigXi("McGenTruthSigXi");
  HisTGroup<TH1D> hgMcGenSigXi("McGenSigXi");
  HisTGroup<TH1D> hgMcj2t0SigXi("Mcj2t0SigXi");
  HisTGroup<TH1D> hgMcj2t0SelRefSigXi("Mcj2t0SelRefSigXi");
  if (doMC) {
    //  -- t0 --
    hgMcGenTruthSigXi.Add(inFile2,"hSig_mcGenTruthNrXi","Nr");
    hgMcGenTruthSigXi.Add(inFile2,"hSig_mcGenTruthAwXi","Aw");
    hgMcGenTruthSigXi.Average();

    hgMcGenSigXi.Add(inFile2,"hSig_mcGenNrXi","Nr");
    hgMcGenSigXi.Add(inFile2,"hSig_mcGenAwXi","Aw");
    hgMcGenSigXi.Average();

    hgMcj2t0SigXi.Add(inFile2,"hSig_j2t0NrXi","Nr");
    hgMcj2t0SigXi.Add(inFile2,"hSig_j2t0AwXi","Aw");
    hgMcj2t0SigXi.Average();

    hgMcj2t0SelRefSigXi.Add(inFile2,"hSig_j2t0SelRefNrXi","Nr");
    hgMcj2t0SelRefSigXi.Add(inFile2,"hSig_j2t0SelRefAwXi","Aw");
    hgMcj2t0SelRefSigXi.Average();
  }


  // === FF comparison ===
  HisTGroup<TH1D> hgCompXi("CompXi");

  // === FF Corrections ===
  HisTGroup<TH1D> hgCorrXi("CorrXi");
  hgCorrXi.Add(hgRecoSigXi.R("Ave"),"ICPu5",1./0.65);
  hgCorrXi.Add(hgRecoUpperSigXi.R("Ave"),"UpperICPu5",1./0.65);
  hgCorrXi.Add(hgRecoLowerSigXi.R("Ave"),"LowerICPu5",1./0.65);
  hgCorrXi.Add(hgRecoSigXi.R("Ave"),"ICPu5TrkUpper",1./(0.65*1.05));
  hgCorrXi.Add(hgRecoSigXi.R("Ave"),"ICPu5TrkLower",1./(0.65*0.95));
  if (doCompare) hgCorrXi.Add(hgReco2SigXi.R("Ave"),"Kt4",1./0.65);

  // === FF QA Plots ===

  // === FF Measurement Plots ===
  // -- Raw --
  TCanvas * cFinalRawFF = new TCanvas("cFinalRawFF","cFinalRawFF",500,500);
  CPlot cpFinalRawFF("FinalRawFF","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFinalRawFF.SetXRange(0,6);
  cpFinalRawFF.SetYRange(0,9);
  cpFinalRawFF.AddHist1D(hFrame,"July Hard: Reco DiJetFF","",0,0);
  cpFinalRawFF.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  cpFinalRawFF.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<170, 50GeV<p_{T}^{jet2}","",0,0);
  cpFinalRawFF.AddHist1D(hgRecoLowerSigXi.R("Ave"),"JES*0.86","hist",kYellow+1,0);
  cpFinalRawFF.AddHist1D(hgRecoUpperSigXi.R("Ave"),"JES*1.14","hist",kGreen+1,0);
  cpFinalRawFF.AddHist1D(hgRecoSigXi.R("Ave"),"Near+Away (iConePu5)","E",kBlack,kFullCircle);
  if (doCompare) cpFinalRawFF.AddHist1D(hgReco2SigXi.R("Ave"),"Near+Away (FastJet Kt4 PuSub)","E",kBlue,kOpenSquare);
  cpFinalRawFF.SetLegend(0.194,0.71,0.52,0.94);
  cpFinalRawFF.Draw(cFinalRawFF,true);

  // -- Corrected --
  TCanvas * cFinalCorrFF = new TCanvas("cFinalCorrFF","cFinalCorrFF",500,500);
  CPlot cpFinalCorrFF("FinalCorrFF","FF","#xi=ln(E_{T}^{Jet}/p_{T}^{trk})","#frac{1}{N_{jet}} #frac{dN}{d#xi} (Raw-Bkg)");
  cpFinalCorrFF.SetXRange(0,6);
  cpFinalCorrFF.SetYRange(0,9);
  cpFinalCorrFF.AddHist1D(hFrame,"July Hard: Corrected Reco DiJetFF","",0,0);
  cpFinalCorrFF.AddHist1D(hFrame,"Centrality: 0-20\%","",0,0);
  cpFinalCorrFF.AddHist1D(hFrame,"100GeV<p_{T}^{jet1}<170, 50GeV<p_{T}^{jet2}","",0,0);
  cpFinalCorrFF.AddHist1D(hgCorrXi.H("LowerICPu5"),"JES*0.86","hist",kYellow+1,0);
  cpFinalCorrFF.AddHist1D(hgCorrXi.H("UpperICPu5"),"JES*1.14","hist",kGreen+1,0);
  cpFinalCorrFF.AddHist1D(hgCorrXi.H("ICPu5"),"Near+Away (iConePu5)","E",kBlack,kFullCircle);
  cpFinalCorrFF.AddHist1D(hgCorrXi.H("ICPu5TrkUpper"),"Trk Eff * 1.05","hist",kBlue,0);
  cpFinalCorrFF.AddHist1D(hgCorrXi.H("ICPu5TrkLower"),"Trk Eff * 0.95","hist",kMagenta,0);
  if (doCompare) {
    cpFinalCorrFF.AddHist1D(hgCorrXi.H("Kt4"),"Near+Away (FastJet Kt4 PuSub)","E",kBlue,kOpenSquare);
    cpFinalCorrFF.AddHist1D(hgMcGenTruthSigXi.R("Ave"),"Near+Away (FastJet Kt4 PuSub)","histE",kRed,0);
  }
  cpFinalCorrFF.SetLegend(0.194,0.71,0.52,0.94);
  cpFinalCorrFF.Draw(cFinalCorrFF,true);

  // All done, save and exit
  outf->Write();
}
