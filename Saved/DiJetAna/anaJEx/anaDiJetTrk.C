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

void anaDiJetTrk(int doMC=0,
    /*
       const char * inFile0Name="../process_aod/outputs/dijetAnaTightDPhi_JEx_ZP_MB_proc0_all.root",
       TString AnaName = "ZSMb/dphi25/a0",
       TString header="July Data (MB)",
    const char * inFile0Name="../process_aod/outputs/dijetAnaTightDPhi_JEx_ZP_Hard_proc1_all.root",
    TString AnaName = "ZSHd1/dp25/a1",
    TString header="July Data (Hard Triggered)",
     */
    const char * inFile0Name="../matttrees/dijetAna_anaJet_Truth.root",
    TString AnaName = "true/dp25/a3",
    TString header="July Data (Gen)",
    //
    TString title1="Data",
    TString title2="MC")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;
  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();

  // === Define dijet selection ===
  selectionCut mcAna(AnaName,doMC,1,100,170,50,2.5);
  //mcAna.DJAnd(TCut("(5*cbin)<20"));
  mcAna.DJAnd(TCut("npart>216"));
  // check
  mcAna.Print();
  // loose
  selectionCut mcAnaLoose(AnaName,doMC,1,20,200,20,2.5);
  mcAnaLoose.DJAnd(TCut("(5*cbin)<20"));

  // === Get Trees ===
  TTree *j2t3, *j2t0, *j1t0;
  if (!doMC) {
    //inFile0->GetObject("dijetAna_data/djTree",j2t3);
    inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",j2t3);
  }
  else {
    inFile0->GetObject("dijetAna_mc/djTree",j2t3);
    // MC Sysemtics
    inFile0->GetObject("dijetAna_mc_calojet_genp/djTree",j2t0); aliases_dijet(j2t0,1);
    inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",j1t0); aliases_dijet(j1t0,1);
  }
  aliases_dijet(j2t3,doMC);

  TString djSelTag("Ana");
  TString trkSelTag("Ana");
  cout << endl << "====== DJ Selection: " << djSelTag << " ======" << endl;
  cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
  cout << "dijetAna j2t3 # entries: " << j2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << j2t3->GetEntries(mcAna.DJ[djSelTag]) << endl;
  if (doMC) {
    cout << "dijetAna_mc/j2t0 # entries: " << j2t0->GetEntries() << endl;
    cout << "# DJ events passed: " << j2t0->GetEntries(mcAna.DJ[djSelTag]) << endl;
    cout << "dijetAna_mc/j1t0 # entries: " << j1t0->GetEntries() << endl;
    cout << "# DJ events passed: " << j1t0->GetEntries(mcAna.DJ[djSelTag]) << endl;
  }

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir+"/djtrk";
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJetTrk.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  Double_t histTrkPtMax=50;
  Double_t histTrkNHitsMax=30;
  // === Event Level ===
  AnaFrag j2t3TrkPt("j2t3","TrkPt",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"ppt","","",histTrkPtMax,0,histTrkPtMax);
  AnaFrag j2t3JCTrkPt("j2t3","JCTrkPt",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"]&&"pndr<0.5||padr<0.5","ppt","","",histTrkPtMax,0,histTrkPtMax);
  AnaFrag j2t3TrkNHits("j2t3","TrkNHits",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"trkNHits","","",histTrkNHitsMax,0,histTrkNHitsMax);
  // === Jet Level ===
  AnaFrag j2t3NrTrkPt("j2t3","NrTrkPt",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"ppt","pndr<0.5","pndrbg<0.5",histTrkPtMax,0,histTrkPtMax);
  AnaFrag j2t3AwTrkPt("j2t3","AwTrkPt",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"],"ppt","pndr<0.5","pndrbg<0.5",histTrkPtMax,0,histTrkPtMax);

  // === Jet Strip ===
  AnaFrag j2t3JTrkDPhi("j2t3","JTrkDPhi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Ana"]&&"abs(peta-nljeta)<0.5||abs(peta-aljeta)<0.5","pndphi","","",15,0,TMath::Pi());
  AnaFrag j2t3JTrkTight3DPhi("j2t3","JTrkTight3DPhi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Tight3"]&&"abs(peta-nljeta)<0.5||abs(peta-aljeta)<0.5","pndphi","","",15,0,TMath::Pi());

  if (doMC) {
    AnaFrag j2t3JTrkDPhi("j2t3","JTrkDPhi",j2t3,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
    AnaFrag j2t0JTrkDPhi("j2t0","JTrkDPhi",j2t0,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
    AnaFrag j1t0JTrkDPhi("j1t0","JTrkDPhi",j1t0,mcAna.DJ["Ana"],mcAna.Trk["Tight"],"pndphi","","",30,0,TMath::Pi());
  }

  // -- plot --
  // === Event Level ===
  TCanvas * cCompTrkPt = new TCanvas("cCompTrkPt","cCompTrkPt",500,500);
  CPlot cpCompTrkPt("CompTrkPt","CompTrkPt","p_{T}^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dp_{T}}");
  cpCompTrkPt.AddHist1D(j2t3TrkPt.hRaw,"hiSelectedTrk","E",kBlack,kFullCircle);
  cpCompTrkPt.AddHist1D(j2t3JCTrkPt.hRaw,"hiSelectedTrk (in jet)","E",kBlue,kOpenSquare);
  cpCompTrkPt.SetLegend(0.41,0.76,0.71,0.86);
  cpCompTrkPt.SetLegendHeader(header);
  cpCompTrkPt.SetLogy();
  cpCompTrkPt.Draw(cCompTrkPt,true);

  TCanvas * cCompTrkNHits = new TCanvas("cCompTrkNHits","cCompTrkNHits",500,500);
  CPlot cpCompTrkNHits("CompTrkNHits","CompTrkNHits","NHits^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dNHits}");
  cpCompTrkNHits.AddHist1D(j2t3TrkNHits.hRaw,"hiSelectedTrk","E",kBlack,kFullCircle);
  cpCompTrkNHits.SetLegend(0.41,0.76,0.71,0.86);
  cpCompTrkNHits.SetLegendHeader(header);
  cpCompTrkNHits.Draw(cCompTrkNHits,true);

  // === Jet Level ===
  TCanvas * cCompNrTrkPt = new TCanvas("cCompNrTrkPt","cCompNrTrkPt",500,500);
  CPlot cpCompNrTrkPt("CompNrTrkPt","CompNrTrkPt","p_{T}^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dp_{T}}");
  cpCompNrTrkPt.AddHist1D(j2t3NrTrkPt.hRaw,"Raw","E",kGreen+2,kOpenSquare);
  cpCompNrTrkPt.AddHist1D(j2t3NrTrkPt.hBkg,"Bkg","E",kBlue,kOpenCircle);
  cpCompNrTrkPt.AddHist1D(j2t3NrTrkPt.hSig,"Subtracted","E",kBlack,kFullCircle);
  cpCompNrTrkPt.SetLegend(0.23,0.81,0.53,0.90);
  cpCompNrTrkPt.SetLegendHeader(header);
  cpCompNrTrkPt.SetLogx();
  cpCompNrTrkPt.SetLogy();
  cpCompNrTrkPt.Draw(cCompNrTrkPt,true);

  TCanvas * cCompAwTrkPt = new TCanvas("cCompAwTrkPt","cCompAwTrkPt",500,500);
  CPlot cpCompAwTrkPt("CompAwTrkPt","CompAwTrkPt","p_{T}^{Trk}","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{dp_{T}}");
  cpCompAwTrkPt.AddHist1D(j2t3AwTrkPt.hRaw,"Raw","E",kGreen+2,kOpenSquare);
  cpCompAwTrkPt.AddHist1D(j2t3AwTrkPt.hBkg,"Bkg","E",kBlue,kOpenCircle);
  cpCompAwTrkPt.AddHist1D(j2t3AwTrkPt.hSig,"Subtracted","E",kBlack,kFullCircle);
  cpCompAwTrkPt.SetLegend(0.61,0.82,0.91,0.92);
  cpCompAwTrkPt.SetLegendHeader(header);
  cpCompAwTrkPt.SetLogx();
  cpCompAwTrkPt.SetLogy();
  cpCompAwTrkPt.Draw(cCompAwTrkPt,true);

  // === Jet Strip ===
  TCanvas * cCompJTrkDPhi = new TCanvas("cCompJTrkDPhi","cCompJTrkDPhi",500,500);
  CPlot cpCompJTrkDPhi("CompJTrkDPhi","CompJTrkDPhi","#Delta#phi(trk,j1)","#frac{1}{N^{DJ Evt}} #frac{dN^{Trk}}{d#Delta#phi} #cbar_{|#Delta#eta(trk,jet)|<0.5}");
  cpCompJTrkDPhi.SetYRange(0,200);
  cpCompJTrkDPhi.AddHist1D(j2t3JTrkDPhi.hRaw,"hiSelectedTrk (p_{T}>1.2GeV)","E",kBlack,kFullCircle);
  cpCompJTrkDPhi.AddHist1D(j2t3JTrkTight3DPhi.hRaw,"hiSelectedTrk (p_{T}>3GeV)","E",kBlue,kOpenCircle);
  cpCompJTrkDPhi.SetLegend(0.41,0.76,0.71,0.86);
  cpCompJTrkDPhi.SetLegendHeader(header);
  cpCompJTrkDPhi.Draw(cCompJTrkDPhi,true);

  // All done, save and exit
  outf->Write();
}
