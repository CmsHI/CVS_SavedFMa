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

void checkDiJetSelection(int doMC=1,
    /*
    const char * inFile0Name="../process_aod/outputs/McUqDj80to120_DJes002_10k.root",
    TString outdir = "plots/mcuq80to120_10k",
    TString header="Hydjet2.76TeV+DiJet(80-120GeV)",
    */
    const char * inFile0Name="../process_aod/outputs/McUqDj120to170_DJes002_10k.root",
    TString AnaName = "mcuq120V2",
    TString header="Hydjet2.76TeV+DiJet(120-170GeV)",
    //
    TString title1="MC Calojet",
    TString title2="MC Genjet")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();

  // Define dijet selection
  selectionCut mcAna(AnaName,doMC,1,120,170,80);
  selectionCut mcAnaLoose(AnaName,doMC,1,50,200,50);

  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  aliases_dijet(mcj2t3);
  inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);
  aliases_dijet(mcj1t0);

  TString djSelTag("Ana");
  TString trkSelTag("Ana");
  cout << endl << "====== DJ Selection: " << djSelTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna_mc/mcj1t0 # entries: " << mcj1t0->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ[djSelTag]) << endl;

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir+"/chkdjsel";
  // Save output
  TFile * outf = new TFile(Form("%s/checkDiJetSelection.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  AnaFrag mcRecoEtNr("mcReco","EtNr",mcj2t3,mcAna.DJ["Ana"],"","nljet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  AnaFrag mcRecoEtAw("mcReco","EtAw",mcj2t3,mcAna.DJ["Ana"],"","aljet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);

  //  ------- Check Reco Match Gen -------
  AnaFrag mcRecoMatEtNr("mcRecoMat","EtNr",mcj2t3,mcAna.DJ["AnaMatRef"],"","nljet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  AnaFrag mcRecoMatEtAw("mcRecoMat","EtAw",mcj2t3,mcAna.DJ["AnaMatRef"],"","aljet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  AnaFrag mcJ1SelREtNr("mcJ1SelR","EtNr",mcj1t0,mcAna.DJ["Ref"],"","nljet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  AnaFrag mcJ1SelREtAw("mcJ1SelR","EtAw",mcj1t0,mcAna.DJ["Ref"],"","aljet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  AnaFrag mcJ1SelRLREtNr("mcJ1SelRLR","EtNr",mcj1t0,mcAna.DJ["RefOrderRef"],"","nrlrjet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);
  AnaFrag mcJ1SelRLREtAw("mcJ1SelRLR","EtAw",mcj1t0,mcAna.DJ["RefOrderRef"],"","arlrjet","","",mcAna.histJetPtBins,mcAna.histJetPtMin,mcAna.histJetPtMax);

  //  -- histograms, all normalized to # of sel calojets --
  HisTGroup<TH1D> hgMcCompEt("McCompEt");
  hgMcCompEt.Add(mcRecoEtNr.hRaw,"RecoNr");
  hgMcCompEt.Add(mcRecoEtAw.hRaw,"RecoAw");
  hgMcCompEt.Add(mcRecoMatEtNr.hRaw,"RecoMatNr",(Double_t)mcRecoMatEtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcCompEt.Add(mcRecoMatEtAw.hRaw,"RecoMatAw",(Double_t)mcRecoMatEtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcCompEt.Add(mcJ1SelREtNr.hRaw,"J1SelRNr",(Double_t)mcJ1SelREtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcCompEt.Add(mcJ1SelREtAw.hRaw,"J1SelRAw",(Double_t)mcJ1SelREtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcCompEt.Add(mcJ1SelRLREtNr.hRaw,"J1SelRLRNr",(Double_t)mcJ1SelRLREtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcCompEt.Add(mcJ1SelRLREtAw.hRaw,"J1SelRLRAw",(Double_t)mcJ1SelRLREtNr.numDJ/mcRecoEtNr.numDJ);

  // -- final plot --
  TCanvas * cCompRecoMat = new TCanvas("cCompRecoMat","cCompRecoMat",800,800);
  CPlot cpCompRecoMat("CompRecoMat","CompRecoMat","E_{T}^{jet}","pdf");
  cpCompRecoMat.AddHist1D(hgMcCompEt.H("RecoNr"),"CaloJet Nr","E",kBlack,kFullCircle);
  cpCompRecoMat.AddHist1D(hgMcCompEt.H("RecoAw"),"CaloJet Aw","E",kGray+2,kFullCircle);
  cpCompRecoMat.AddHist1D(hgMcCompEt.H("RecoMatNr"),"CaloJet Nr (Matched)","histE",kGreen+3,0);
  cpCompRecoMat.AddHist1D(hgMcCompEt.H("RecoMatAw"),"CaloJet Aw (Matched)","histE",kGreen-7,0);
  //cpCompRecoMat.AddHist1D(hgMcCompEt.H("J1SelRNr"),"GenJetRef (SelRef) Nr","E",kMagenta+2,kOpenStar);
  //cpCompRecoMat.AddHist1D(hgMcCompEt.H("J1SelRAw"),"GenJetRef (SelRef) Aw","E",kMagenta-7,kOpenStar);
  cpCompRecoMat.AddHist1D(hgMcCompEt.H("J1SelRLRNr"),"GenJetRef (SelRefOrderRef) Nr","E",kOrange+1,kOpenSquare);
  cpCompRecoMat.AddHist1D(hgMcCompEt.H("J1SelRLRAw"),"GenJetRef (SelRefOrderRef) Aw","E",kOrange-9,kOpenSquare);
  cpCompRecoMat.SetLegend(0.20,0.66,0.53,0.90);
  cpCompRecoMat.Draw(cCompRecoMat,true);

  // All done, save and exit
  outf->Write();
}
