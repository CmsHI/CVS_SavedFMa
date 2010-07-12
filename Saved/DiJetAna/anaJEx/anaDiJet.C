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

void anaDiJet(int doMC=0,
    /*
    const char * inFile0Name="../process_aod/outputs/dijetAnaTightDPhi_JEx_ZP_MB_proc0_all.root",
    TString AnaName = "ZSMb/dp25/a0",
    TString header="July Data (MB)",
    */
    const char * inFile0Name="../process_aod/outputs/dijetAnaTightDPhi_JEx_ZP_Hard_proc0_all.root",
    TString AnaName = "ZSHd/dp25/a0",
    TString header="July Data (Hard Triggered)",
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
  selectionCut mcAna(AnaName,doMC,1,100,200,50,2.5);
  mcAna.DJAnd(TCut("(5*cbin)<20"));
  // check
  mcAna.Print();
  // loose
  selectionCut mcAnaLoose(AnaName,doMC,1,20,200,20,2.5);
  mcAnaLoose.DJAnd(TCut("(5*cbin)<20"));

  // === Get Trees ===
  TTree *dataj2, *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  if (!doMC) {
    inFile0->GetObject("dijetAna_data/djTree",dataj2);
    aliases_dijet(dataj2,0);
  } else {
    inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
    aliases_dijet(mcj2t3,1);
    inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);
    aliases_dijet(mcj1t0,1);
  }

  TString djSelTag("Ana");
  TString trkSelTag("Ana");
  cout << endl << "====== DJ Selection: " << djSelTag << " ======" << endl;
  if (!doMC) {
    cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
    cout << "dijetAna_data/djTree # entries: " << dataj2->GetEntries() << endl;
    cout << "# DJ events passed: " << dataj2->GetEntries(mcAna.DJ[djSelTag]) << endl;
  } else {
    cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
    cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
    cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ[djSelTag]) << endl;
    cout << "dijetAna_mc/mcj1t0 # entries: " << mcj1t0->GetEntries() << endl;
    cout << "# DJ events passed: " << mcj1t0->GetEntries(mcAna.DJ[djSelTag]) << endl;
  }

  // Define Output
  cout << "======= Output Dir: ========" << endl;
  TString anaoutdir = Form("plots/%s/%s",AnaName.Data(),mcAna.AnaTag.Data());
  cout << "Output dir: " << anaoutdir << endl;
  gSystem->mkdir(anaoutdir.Data(),kTRUE);
  CPlot::sOutDir = anaoutdir+"/dj";
  // Save output
  TFile * outf = new TFile(Form("%s/anaDiJet.root",anaoutdir.Data()),"RECREATE");

  // ============== pdf comparisons ===============
  AnaFrag dataj2JDPhi("dataj2","JDPhi",dataj2,mcAna.DJ["Ana"],"","jdphi","","",30,0,TMath::Pi());
  AnaFrag dataj2Balance("dataj2","Balance",dataj2,mcAna.DJ["Ana"],"","(nljet-aljet)/nljet","","",30,0,1.);
  AnaFrag dataj2LooseJDPhi("dataj2Loose","JDPhi",dataj2,mcAnaLoose.DJ["Ana"],"","jdphi","","",30,0,TMath::Pi());
  AnaFrag dataj2LooseBalance("dataj2Loose","Balance",dataj2,mcAnaLoose.DJ["Ana"],"","(nljet-aljet)/nljet","","",30,0,1.);
  if (doMC) {
    AnaFrag mcj2JDPhi("mcj2","JDPhi",mcj2t3,mcAna.DJ["Ana"],"","jdphi","","",30,0,TMath::Pi());
    AnaFrag mcj1JDPhi("mcj1","JDPhi",mcj1t0,mcAna.DJ["Ana"],"","jdphi","","",30,0,TMath::Pi());

    AnaFrag mcj2Balance("mcj2","Balance",mcj2t3,mcAna.DJ["Ana"],"","(nljet-aljet)/(nljet)","","",30,0,1.);
    AnaFrag mcj1Balance("mcj1","Balance",mcj1t0,mcAna.DJ["Ana"],"","(nljet-aljet)/(nljet)","","",30,0,1.);
  }

  // -- plot --
  TCanvas * cCompJDPhi = new TCanvas("cCompJDPhi","cCompJDPhi",500,500);
  CPlot cpCompJDPhi("CompJDPhi","CompJDPhi","Leading Di-Jet d #phi","pdf");
  cpCompJDPhi.SetYRange(0,4);
  if (!doMC) {
    cpCompJDPhi.AddHist1D(dataj2LooseJDPhi.hRaw,"Data DiJet Loose","E",kBlue,kOpenCircle);
    cpCompJDPhi.AddHist1D(dataj2JDPhi.hRaw,"Data DiJet Tight","E",kBlack,kFullCircle);
  }
  cpCompJDPhi.SetLegend(0.21,0.75,0.54,0.87);
  cpCompJDPhi.SetLegendHeader(header);
  cpCompJDPhi.Draw(cCompJDPhi,true);

  TCanvas * cCompBalance = new TCanvas("cCompBalance","cCompBalance",500,500);
  CPlot cpCompBalance("CompBalance","CompBalance","(p_{T}^{j1}-p_{T}^{j2})/(p_{T}^{j1})","pdf");
  cpCompBalance.SetYRange(0,7.5);
  if (!doMC) {
    cpCompBalance.AddHist1D(dataj2LooseBalance.hRaw,"Data DiJet Loose","E",kBlue,kOpenCircle);
    cpCompBalance.AddHist1D(dataj2Balance.hRaw,"Data DiJet Tight","E",kBlack,kFullCircle);
  }
  cpCompBalance.SetLegendHeader(header);
  cpCompBalance.Draw(cCompBalance,true);

  // ============== Dijet Scales ===============
  HisTGroup<TH2F> hgDataDJEt("DataDJEt",50,0,200,50,0,200);
  hgDataDJEt.Add2D("J2");

  HisTGroup<TH2F> hgMcDJEt("McDJEt",50,0,200,50,0,200);
  hgMcDJEt.Add2D("J2");
  hgMcDJEt.Add2D("J2Mat");
  hgMcDJEt.Add2D("J2SelRef");
  hgMcDJEt.Add2D("J1");

  if (!doMC) {
    dataj2->Draw(Form("aljet:nljet>>%s",hgDataDJEt.GetH("J2")->GetName()),mcAnaLoose.DJ["Ana"],"goff");
  } else {
    mcj2t3->Draw(Form("aljet:nljet>>%s",hgMcDJEt.GetH("J2")->GetName()),mcAnaLoose.DJ["Ana"],"goff");
    mcj2t3->Draw(Form("aljet:nljet>>%s",hgMcDJEt.GetH("J2Mat")->GetName()),mcAnaLoose.DJ["AnaMatRef"],"goff");
    mcj2t3->Draw(Form("aljet:nljet>>%s",hgMcDJEt.GetH("J2SelRef")->GetName()),mcAnaLoose.DJ["Ref"],"goff");
    mcj1t0->Draw(Form("aljet:nljet>>%s",hgMcDJEt.GetH("J1")->GetName()),mcAnaLoose.DJ["Ana"],"goff");
  }

  // -- plot --
  if (!doMC) {
    TCanvas * cDataDJ2Et = new TCanvas("cDataDJ2Et","cDataDJ2Et",700,700);
    cDataDJ2Et->SetLogz();
    CPlot cpDataDJ2Et("DataDataDJ2Et","DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
    cpDataDJ2Et.AddHist2D(hgDataDJEt.H("J2"),"colz");
    //cpDataDJ2Et.SetLegendHeader(header);
    cpDataDJ2Et.Draw(cDataDJ2Et,true);
  } else {
    TCanvas * cDJ2Et = new TCanvas("cDJ2Et","cDJ2Et",1000,500);
    cDJ2Et->Divide(2,1);
    cDJ2Et->cd(1);
    CPlot cpDJ1Et("DJ1Et","DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
    cpDJ1Et.AddHist2D(hgMcDJEt.H("J1"),"colz");
    cpDJ1Et.Draw((TPad*)cDJ2Et->GetPad(1),false);
    cDJ2Et->cd(2);
    CPlot cpDJ2Et("DJ2Et","DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
    cpDJ2Et.AddHist2D(hgMcDJEt.H("J2"),"colz");
    cpDJ2Et.Draw((TPad*)cDJ2Et->GetPad(2),true);
  }

  // DJ vs Centrality
  AnaFrag dataj2DJCent("dataj2","DJCent",dataj2,mcAna.DJ["Ana"],"","cbin*5","","",20,0,100);
  TCanvas * cCompDJCent = new TCanvas("cCompDJCent","cCompDJCent",500,500);
  CPlot cpCompDJCent("CompDJCent","CompDJCent","Centrality [%]","frac of (tight) DiJets");
  if (!doMC) {
    cpCompDJCent.AddHist1D(dataj2DJCent.hRaw,"July Data","E",kBlack,kFullCircle);
  }
  cpCompDJCent.SetLegendHeader(header);
  cpCompDJCent.Draw(cCompDJCent,true);

  // All done, save and exit
  outf->Write();
}
