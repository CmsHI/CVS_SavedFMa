#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "../macros/aliases.C"
using namespace std;

void flavor_ana(int doMC=1,
    const char * JecType = "abs",
    const char * inFile0Name="../outputs/jetSkimRelValQCD_Pt_80_120_sw354-JEC_Summer09_7TeV_ReReco332.root",
    const char * inFile1Name="djtree_JEC_Summer09_7TeV_ReReco332.root")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  //cout << "Inclusive jets: " << inFile0Name << endl;
  //cout << "Dijets:         " << inFile1Name << endl;
  //TFile * inFile0 = new TFile(inFile0Name);
  //TFile * inFile1 = new TFile(inFile1Name);
  TFile * inFile0 = new TFile("../outputs/jetSkimRelValQCD_Pt_80_120_sw354-JEC_Summer09_7TeV_ReReco332.root");
  TFile * inFile1 = new TFile(Form("djtree_JEC_Summer09_7TeV_ReReco332_%s.root",JecType));
  TTree * Events; inFile0->GetObject("Events",Events);
  TTree * djtree; inFile1->GetObject("djtree",djtree);
  aliases(Events);

  // === Dijets ===
  cout << endl << "================ dijets ==============" << endl;
  Int_t numPreSelEvtl = djtree->GetEntries("vz>-99");
  cout << "Events:" << endl;
  cout << "- with none-fake vtx: " << numPreSelEvtl << endl;
  //  - selection -
  TCut evtSelDj("nlpet>80 && nlpet<120 && alpet>80 && jdphi>2.8");
  TCut evtSelDj2("nlpet>80 && nlpet<120 && alpet>80 && jdphi>3.0");
  Int_t numSelEvt1 = djtree->GetEntries(evtSelDj);
  cout << "- Passing sel: " << numSelEvt1 << endl;
  Int_t numSelEvt2 = djtree->GetEntries(evtSelDj2);
  cout << "- Passing diphi sel: " << numSelEvt2 << endl;

  // === flavor studies ===
  TProfile * hNrJFlav = new TProfile("hNrJFlav","flavor",10,85,110);
  hNrJFlav->SetMarkerColor(kRed);
  hNrJFlav->SetLineColor(kRed);
  hNrJFlav->SetMarkerStyle(kOpenCircle);
  hNrJFlav->SetMinimum(0.);
  hNrJFlav->SetMaximum(0.7);
  hNrJFlav->SetTitle(";p_{T}^{gen jet};q jet fraction");
  TProfile * hAwJFlav = (TProfile*)hNrJFlav->Clone("hAwJFlav");
  hAwJFlav->SetMarkerColor(kBlue);
  hAwJFlav->SetLineColor(kBlue);

  TCanvas * cJFlav = new TCanvas("cJFlav","cJFlav",500,500);
  TCut matParton("nlpstat>-99&&alpstat>-99");
  djtree->Draw("(abs(nlpid)<6):nlpet>>hNrJFlav",evtSelDj&&matParton,"prof");
  djtree->Draw("(abs(alpid)<6):alpet>>hAwJFlav",evtSelDj&&matParton,"prof same");

  TLegend *leg2 = new TLegend(0.605,0.216,0.905,0.369,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.03);
  leg2->AddEntry(hNrJFlav,"Di-Jets, Near Jet","p");
  leg2->AddEntry(hAwJFlav,"Di-Jets, Away Jet","p");
  leg2->Draw();
  cJFlav->Print("plots/cDJFlavor.gif");
}
