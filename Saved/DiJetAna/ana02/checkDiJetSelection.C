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
  //selectionCut mcAna(AnaName,doMC,1,60,200,60); // loose

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

  //  ===== Check Reco Match Gen =====
  cout << endl << "=== First check Gen Matche Reco ===" << endl;
  AnaFrag mcRecoEtNr("mcReco","EtNr",mcj2t3,mcAna.DJ["Ana"],"","nljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcRecoEtAw("mcReco","EtAw",mcj2t3,mcAna.DJ["Ana"],"","aljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);

  AnaFrag mcRecoMatEtNr("mcRecoMat","EtNr",mcj2t3,mcAna.DJ["AnaMatRef"],"","nljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcRecoMatEtAw("mcRecoMat","EtAw",mcj2t3,mcAna.DJ["AnaMatRef"],"","aljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ1SelREtNr("mcJ1SelR","EtNr",mcj1t0,mcAna.DJ["Ref"],"","nljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ1SelREtAw("mcJ1SelR","EtAw",mcj1t0,mcAna.DJ["Ref"],"","aljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ1SelRLREtNr("mcJ1SelRLR","EtNr",mcj1t0,mcAna.DJ["RefOrderRef"],"","nrlrjet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ1SelRLREtAw("mcJ1SelRLR","EtAw",mcj1t0,mcAna.DJ["RefOrderRef"],"","arlrjet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);

  //  -- histograms, all normalized to # of sel calojets --
  HisTGroup<TH1D> hgMcRecoCompEt("McRecoCompEt");
  hgMcRecoCompEt.Add(mcRecoEtNr.hRaw,"RecoNr");
  hgMcRecoCompEt.Add(mcRecoEtAw.hRaw,"RecoAw");
  hgMcRecoCompEt.Add(mcRecoMatEtNr.hRaw,"RecoMatNr",(Double_t)mcRecoMatEtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcRecoCompEt.Add(mcRecoMatEtAw.hRaw,"RecoMatAw",(Double_t)mcRecoMatEtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcRecoCompEt.Add(mcJ1SelREtNr.hRaw,"J1SelRNr",(Double_t)mcJ1SelREtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcRecoCompEt.Add(mcJ1SelREtAw.hRaw,"J1SelRAw",(Double_t)mcJ1SelREtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcRecoCompEt.Add(mcJ1SelRLREtNr.hRaw,"J1SelRLRNr",(Double_t)mcJ1SelRLREtNr.numDJ/mcRecoEtNr.numDJ);
  hgMcRecoCompEt.Add(mcJ1SelRLREtAw.hRaw,"J1SelRLRAw",(Double_t)mcJ1SelRLREtNr.numDJ/mcRecoEtNr.numDJ);

  // -- final plot --
  TCanvas * cCompRecoMatEt = new TCanvas("cCompRecoMatEt","cCompRecoMatEt",800,800);
  CPlot cpCompRecoMatEt("CompRecoMatEt","CompRecoMatEt","E_{T}^{jet}","pdf");
  cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("RecoNr"),"CaloJet Nr","E",kBlack,kFullCircle);
  cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("RecoAw"),"CaloJet Aw","E",kGray+2,kFullCircle);
  cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("RecoMatNr"),"CaloJet Nr (Matched)","histE",kGreen+3,0);
  cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("RecoMatAw"),"CaloJet Aw (Matched)","histE",kGreen-7,0);
  //cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("J1SelRNr"),"GenJetRef (SelRef) Nr","E",kMagenta+2,kOpenStar);
  //cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("J1SelRAw"),"GenJetRef (SelRef) Aw","E",kMagenta-7,kOpenStar);
  cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("J1SelRLRNr"),"GenJetRef (SelRefOrderRef) Nr","E",kOrange+8,kOpenSquare);
  cpCompRecoMatEt.AddHist1D(hgMcRecoCompEt.H("J1SelRLRAw"),"GenJetRef (SelRefOrderRef) Aw","E",kOrange,kOpenSquare);
  cpCompRecoMatEt.SetLegend(0.16,0.75,0.44,0.94);
  cpCompRecoMatEt.Draw(cCompRecoMatEt,true);


  //  ===== Check Gen Match Reco =====
  cout << endl << "=== Now check Gen Matche Reco ===" << endl;
  AnaFrag mcGenEtNr("mcGen","EtNr",mcj1t0,mcAna.DJ["Ana"],"","nljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcGenEtAw("mcGen","EtAw",mcj1t0,mcAna.DJ["Ana"],"","aljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);

  AnaFrag mcGenMatEtNr("mcGenMat","EtNr",mcj1t0,mcAna.DJ["AnaMatRef"],"","nljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcGenMatEtAw("mcGenMat","EtAw",mcj1t0,mcAna.DJ["AnaMatRef"],"","aljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ2SelREtNr("mcJ2SelR","EtNr",mcj2t3,mcAna.DJ["Ref"],"","nljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ2SelREtAw("mcJ2SelR","EtAw",mcj2t3,mcAna.DJ["Ref"],"","aljet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ2SelRLREtNr("mcJ2SelRLR","EtNr",mcj2t3,mcAna.DJ["RefOrderRef"],"","nrlrjet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);
  AnaFrag mcJ2SelRLREtAw("mcJ2SelRLR","EtAw",mcj2t3,mcAna.DJ["RefOrderRef"],"","arlrjet","","",mcAna.hisJEtBins,mcAna.hisJEtMin,mcAna.hisJEtMax);

  //  -- histograms, all normalized to # of sel calojets --
  HisTGroup<TH1D> hgMcGenCompEt("McGenCompEt");
  hgMcGenCompEt.Add(mcGenEtNr.hRaw,"GenNr");
  hgMcGenCompEt.Add(mcGenEtAw.hRaw,"GenAw");
  hgMcGenCompEt.Add(mcGenMatEtNr.hRaw,"GenMatNr",(Double_t)mcGenMatEtNr.numDJ/mcGenEtNr.numDJ);
  hgMcGenCompEt.Add(mcGenMatEtAw.hRaw,"GenMatAw",(Double_t)mcGenMatEtNr.numDJ/mcGenEtNr.numDJ);
  hgMcGenCompEt.Add(mcJ2SelREtNr.hRaw,"J2SelRNr",(Double_t)mcJ2SelREtNr.numDJ/mcGenEtNr.numDJ);
  hgMcGenCompEt.Add(mcJ2SelREtAw.hRaw,"J2SelRAw",(Double_t)mcJ2SelREtNr.numDJ/mcGenEtNr.numDJ);
  hgMcGenCompEt.Add(mcJ2SelRLREtNr.hRaw,"J2SelRLRNr",(Double_t)mcJ2SelRLREtNr.numDJ/mcGenEtNr.numDJ);
  hgMcGenCompEt.Add(mcJ2SelRLREtAw.hRaw,"J2SelRLRAw",(Double_t)mcJ2SelRLREtNr.numDJ/mcGenEtNr.numDJ);

  // -- final plot --
  TCanvas * cCompGenMatEt = new TCanvas("cCompGenMatEt","cCompGenMatEt",800,800);
  CPlot cpCompGenMatEt("CompGenMatEt","CompGenMatEt","E_{T}^{jet}","pdf");
  cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("GenNr"),"GenJet Nr","E",kBlack,kFullCircle);
  cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("GenAw"),"GenJet Aw","E",kGray+2,kFullCircle);
  cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("GenMatNr"),"GenJet Nr (Matched)","histE",kGreen+3,0);
  cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("GenMatAw"),"GenJet Aw (Matched)","histE",kGreen-7,0);
  //cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("J2SelRNr"),"CaloJetRef (SelRef) Nr","E",kMagenta+2,kOpenStar);
  //cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("J2SelRAw"),"CaloJetRef (SelRef) Aw","E",kMagenta-7,kOpenStar);
  cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("J2SelRLRNr"),"CaloJetRef (SelRefOrderRef) Nr","E",kOrange+8,kOpenSquare);
  cpCompGenMatEt.AddHist1D(hgMcGenCompEt.H("J2SelRLRAw"),"CaloJetRef (SelRefOrderRef) Aw","E",kOrange,kOpenSquare);
  cpCompGenMatEt.SetLegend(0.16,0.75,0.44,0.94);
  cpCompGenMatEt.Draw(cCompGenMatEt,true);


  // Now we have checked that the complete matching is fine
  // Let's compare the eta, phi distributions of the completely matched jets
  cout << endl << "=== Now check completely matched jets' eta, phi ===" << endl;
  AnaFrag mcRecoEtaNr("mcReco","EtaNr",mcj2t3,mcAna.DJ["Ana"],"","nljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);
  AnaFrag mcRecoEtaAw("mcReco","EtaAw",mcj2t3,mcAna.DJ["Ana"],"","aljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);
  AnaFrag mcGenEtaNr("mcGen","EtaNr",mcj1t0,mcAna.DJ["Ana"],"","nljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);
  AnaFrag mcGenEtaAw("mcGen","EtaAw",mcj1t0,mcAna.DJ["Ana"],"","aljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);

  AnaFrag mcRecoPhiNr("mcReco","PhiNr",mcj2t3,mcAna.DJ["Ana"],"","nljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);
  AnaFrag mcRecoPhiAw("mcReco","PhiAw",mcj2t3,mcAna.DJ["Ana"],"","aljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);
  AnaFrag mcGenPhiNr("mcGen","PhiNr",mcj1t0,mcAna.DJ["Ana"],"","nljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);
  AnaFrag mcGenPhiAw("mcGen","PhiAw",mcj1t0,mcAna.DJ["Ana"],"","aljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);

  AnaFrag mcJ1SelRLEtaNr("mcJ1SelRL","EtaNr",mcj1t0,mcAna.DJ["RefOrderRef"],"","nrljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);
  AnaFrag mcJ1SelRLEtaAw("mcJ1SelRL","EtaAw",mcj1t0,mcAna.DJ["RefOrderRef"],"","arljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);
  AnaFrag mcJ2SelRLEtaNr("mcJ2SelRL","EtaNr",mcj2t3,mcAna.DJ["RefOrderRef"],"","nrljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);
  AnaFrag mcJ2SelRLEtaAw("mcJ2SelRL","EtaAw",mcj2t3,mcAna.DJ["RefOrderRef"],"","arljeta","","",mcAna.hisJEtaBins,mcAna.hisJEtaMin,mcAna.hisJEtaMax);

  AnaFrag mcJ1SelRLPhiNr("mcJ1SelRL","PhiNr",mcj1t0,mcAna.DJ["RefOrderRef"],"","nrljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);
  AnaFrag mcJ1SelRLPhiAw("mcJ1SelRL","PhiAw",mcj1t0,mcAna.DJ["RefOrderRef"],"","arljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);
  AnaFrag mcJ2SelRLPhiNr("mcJ2SelRL","PhiNr",mcj2t3,mcAna.DJ["RefOrderRef"],"","nrljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);
  AnaFrag mcJ2SelRLPhiAw("mcJ2SelRL","PhiAw",mcj2t3,mcAna.DJ["RefOrderRef"],"","arljphi","","",mcAna.hisJPhiBins,mcAna.hisJPhiMin,mcAna.hisJPhiMax);

  //  -- histograms, all normalized to # of sel calojets --
  HisTGroup<TH1D> hgMcCompEta("McCompEta");
  hgMcCompEta.Add(mcGenEtaNr.hRaw,"GenNr");
  hgMcCompEta.Add(mcGenEtaAw.hRaw,"GenAw");
  hgMcCompEta.Add(mcRecoEtaNr.hRaw,"RecoNr");
  hgMcCompEta.Add(mcRecoEtaAw.hRaw,"RecoAw");
  hgMcCompEta.Add(mcJ2SelRLEtaNr.hRaw,"J2SelRLNr",(Double_t)mcJ2SelRLEtaNr.numDJ/mcGenEtaNr.numDJ);
  hgMcCompEta.Add(mcJ2SelRLEtaAw.hRaw,"J2SelRLAw",(Double_t)mcJ2SelRLEtaNr.numDJ/mcGenEtaNr.numDJ);
  hgMcCompEta.Add(mcJ1SelRLEtaNr.hRaw,"J1SelRLNr",(Double_t)mcJ1SelRLEtaNr.numDJ/mcGenEtaNr.numDJ);
  hgMcCompEta.Add(mcJ1SelRLEtaAw.hRaw,"J1SelRLAw",(Double_t)mcJ1SelRLEtaNr.numDJ/mcGenEtaNr.numDJ);

  HisTGroup<TH1D> hgMcCompPhi("McCompPhi");
  hgMcCompPhi.Add(mcGenPhiNr.hRaw,"GenNr");
  hgMcCompPhi.Add(mcGenPhiAw.hRaw,"GenAw");
  hgMcCompPhi.Add(mcRecoPhiNr.hRaw,"RecoNr");
  hgMcCompPhi.Add(mcRecoPhiAw.hRaw,"RecoAw");
  hgMcCompPhi.Add(mcJ2SelRLPhiNr.hRaw,"J2SelRLNr",(Double_t)mcJ2SelRLPhiNr.numDJ/mcGenPhiNr.numDJ);
  hgMcCompPhi.Add(mcJ2SelRLPhiAw.hRaw,"J2SelRLAw",(Double_t)mcJ2SelRLPhiNr.numDJ/mcGenPhiNr.numDJ);
  hgMcCompPhi.Add(mcJ1SelRLPhiNr.hRaw,"J1SelRLNr",(Double_t)mcJ1SelRLPhiNr.numDJ/mcGenPhiNr.numDJ);
  hgMcCompPhi.Add(mcJ1SelRLPhiAw.hRaw,"J1SelRLAw",(Double_t)mcJ1SelRLPhiNr.numDJ/mcGenPhiNr.numDJ);

  // -- final plot --
  TCanvas * cCompMatEta = new TCanvas("cCompMatEta","cCompMatEta",800,800);
  CPlot cpCompMatEta("CompMatEta","CompMatEta","#eta^{jet}","pdf");
  cpCompMatEta.AddHist1D(hgMcCompEta.H("GenNr"),"GenJet Nr","E",kBlack,kFullCircle);
  cpCompMatEta.AddHist1D(hgMcCompEta.H("GenAw"),"GenJet Aw","E",kGray+2,kFullCircle);
  cpCompMatEta.AddHist1D(hgMcCompEta.H("J2SelRLNr"),"CaloJet (SelRefOrderRef) Nr","E",kOrange+8,kOpenSquare);
  cpCompMatEta.AddHist1D(hgMcCompEta.H("J2SelRLAw"),"CaloJet (SelRefOrderRef) Aw","E",kOrange,kOpenSquare);
  cpCompMatEta.SetLegend(0.33,0.15,0.61,0.34);
  cpCompMatEta.Draw(cCompMatEta,true);

  TCanvas * cCompMatPhi = new TCanvas("cCompMatPhi","cCompMatPhi",800,800);
  CPlot cpCompMatPhi("CompMatPhi","CompMatPhi","#phi^{jet}","pdf");
  cpCompMatPhi.SetYRange(0,0.3);
  cpCompMatPhi.AddHist1D(hgMcCompPhi.H("GenNr"),"GenJet Nr","E",kBlack,kFullCircle);
  cpCompMatPhi.AddHist1D(hgMcCompPhi.H("GenAw"),"GenJet Aw","E",kGray+2,kFullCircle);
  cpCompMatPhi.AddHist1D(hgMcCompPhi.H("J2SelRLNr"),"CaloJet (SelRefOrderRef) Nr","E",kOrange+8,kOpenSquare);
  cpCompMatPhi.AddHist1D(hgMcCompPhi.H("J2SelRLAw"),"CaloJet (SelRefOrderRef) Aw","E",kOrange,kOpenSquare);
  cpCompMatPhi.SetLegend(0.33,0.15,0.61,0.34);
  cpCompMatPhi.Draw(cCompMatPhi,true);

  // All done, save and exit
  outf->Write();
}
