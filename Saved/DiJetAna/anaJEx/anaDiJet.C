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
    const char * inFile0Name="../process_aod/outputs/dijetAna_Hard4_Data1_14k.root",
    TString AnaName = "dataHd4/a0",
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

  // Define dijet selection
  selectionCut mcAna(AnaName,doMC,1,80,120,70);
  selectionCut mcAnaLoose(AnaName,doMC,1,40,200,40);

  TTree *dataj2, *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  if (!doMC) {
    inFile0->GetObject("dijetAna_data_allcbin/djTree",dataj2);
    aliases_dijet(dataj2,0);
  } else {
    inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
    aliases_dijet(mcj2t3,1);
    inFile0->GetObject("dijetAna_mc_genjet_genp/djTree",mcj1t0);
    aliases_dijet(mcj1t0,1);
  }

  /*
  TString djSelTag("Ana");
  TString trkSelTag("Ana");
  cout << endl << "====== DJ Selection: " << djSelTag << " ======" << endl;
  if (!doMC) {
    cout << "DJ selection: " << TString(mcAna.DJ[djSelTag]) << endl;
    cout << "dijetAna_data_allcbin/djTree # entries: " << dataj2->GetEntries() << endl;
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
  AnaFrag mcj2JDPhi("mcj2","JDPhi",mcj2t3,mcAna.DJ["Ana"],"","jdphi","","",30,0,TMath::Pi());
  AnaFrag mcj1JDPhi("mcj1","JDPhi",mcj1t0,mcAna.DJ["Ana"],"","jdphi","","",30,0,TMath::Pi());

  AnaFrag mcj2Balance("mcj2","Balance",mcj2t3,mcAna.DJ["Ana"],"","2*(nljet-aljet)/(nljet+aljet)","","",30,0,1.);
  AnaFrag mcj1Balance("mcj1","Balance",mcj1t0,mcAna.DJ["Ana"],"","2*(nljet-aljet)/(nljet+aljet)","","",30,0,1.);

  // -- plot --
  TCanvas * cCompJDPhi = new TCanvas("cCompJDPhi","cCompJDPhi",500,500);
  CPlot cpCompJDPhi("CompJDPhi","CompJDPhi","Leading Di-Jet d #phi","pdf");
  cpCompJDPhi.AddHist1D(mcj1JDPhi.hRaw,"GenJet","Ehist",kRed,0);
  cpCompJDPhi.AddHist1D(mcj2JDPhi.hRaw,"CaloJet","E",kBlack);
  cpCompJDPhi.Draw(cCompJDPhi,true);

  TCanvas * cCompBalance = new TCanvas("cCompBalance","cCompBalance",500,500);
  CPlot cpCompBalance("CompBalance","CompBalance","(p_{T}^{j1}-p_{T}^{j2})/((p_{T}^{j1}+p_{T}^{j2})/2)","pdf");
  cpCompBalance.SetYRange(0,7.5);
  cpCompBalance.AddHist1D(mcj1Balance.hRaw,"GenJet","Ehist",kRed,0);
  cpCompBalance.AddHist1D(mcj2Balance.hRaw,"CaloJet","E",kBlack);
  cpCompBalance.Draw(cCompBalance,true);

  // ============== Dijet Scales ===============
  HisTGroup<TH2F> hgDJEt("DJEt",50,0,200,50,0,200);
  hgDJEt.Add2D("J2");
  hgDJEt.Add2D("J2Mat");
  hgDJEt.Add2D("J2SelRef");
  hgDJEt.Add2D("J1");

  mcj2t3->Draw(Form("aljet:nljet>>%s",hgDJEt.GetH("J2")->GetName()),mcAnaLoose.DJ["Ana"],"goff");
  mcj2t3->Draw(Form("aljet:nljet>>%s",hgDJEt.GetH("J2Mat")->GetName()),mcAnaLoose.DJ["AnaMatRef"],"goff");
  mcj2t3->Draw(Form("aljet:nljet>>%s",hgDJEt.GetH("J2SelRef")->GetName()),mcAnaLoose.DJ["Ref"],"goff");
  mcj1t0->Draw(Form("aljet:nljet>>%s",hgDJEt.GetH("J1")->GetName()),mcAnaLoose.DJ["Ana"],"goff");

  // -- plot --
  TCanvas * cDJ2Et = new TCanvas("cDJ2Et","cDJ2Et",1000,500);
  cDJ2Et->Divide(2,1);
  cDJ2Et->cd(1);
  CPlot cpDJ1Et("DJ1Et","DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpDJ1Et.AddHist2D(hgDJEt.H("J1"),"colz");
  cpDJ1Et.Draw((TPad*)cDJ2Et->GetPad(1),false);
  cDJ2Et->cd(2);
  CPlot cpDJ2Et("DJ2Et","DJ","Leading E_{T}^{jet} [GeV]","Away E_{T}^{jet} [GeV]");
  cpDJ2Et.AddHist2D(hgDJEt.H("J2"),"colz");
  cpDJ2Et.Draw((TPad*)cDJ2Et->GetPad(2),true);

  // All done, save and exit
  outf->Write();
  */
}
