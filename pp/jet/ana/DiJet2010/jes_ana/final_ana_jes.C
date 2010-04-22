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

void final_ana_jes(int doMC=1,
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
  TFile * inFile1 = new TFile("djtree_JEC_Summer09_7TeV_ReReco332.root");
  TTree * Events; inFile0->GetObject("Events",Events);
  TTree * djtree; inFile1->GetObject("djtree",djtree);
  aliases(Events);

  // === Inclusive Jets ===
  cout << "================ Inclusive Jets ==============" << endl;
  // Some Event Statistics
  TCanvas * cVz0 = new TCanvas("cVz0","cVz0",500,500);
  Events->Draw("vtx.z()");

  Int_t numPreSelEvt0 = Events->GetEntries("!vtx.isFake()");
  cout << "Events:" << endl;
  cout << "- with none-fake vtx: " << numPreSelEvt0 << endl;
  TCut evtSelInc("gjet.pt()>80 && gjet.pt()<120");
  Int_t numSelEvt0 = Events->GetEntries(evtSelInc);
  cout << "- Passing sel: " << numSelEvt0 << endl;

  // checks
  TCanvas * cPt1 = new TCanvas("cPt1","cPt1",500,500);
  TH1D * hPt1 = new TH1D("hPt1","pt",100,0,160);
  TH1D * hGPt1 = new TH1D("hGPt1","pt",100,0,160);
  hGPt1->SetLineColor(kRed);
  hPt1->SetTitle(";jet pt;");
  hGPt1->SetTitle(";jet pt;");
  Events->Draw("gjet.p4().pt()>>hGPt1",evtSelInc);
  Events->Draw("jet.p4().pt()>>hPt1",evtSelInc,"same");
  cPt1->Print("plots/cPt1.gif");

  TProfile * hJes = new TProfile("hJes","jes",25,85,110);
  hJes->SetMinimum(0);
  hJes->SetMaximum(1.2);
  hJes->SetTitle(";p_{T}^{gen jet};p_{T}^{reco jet}/p_{T}^{gen jet}");
  TCanvas * cJes = new TCanvas("cJes","cJes",500,500);
  Events->Draw("jet.p4().pt()/gjet.p4().pt():gjet.p4().pt()>>hJes",evtSelInc);


  // === Dijets ===
  cout << endl << "================ dijets ==============" << endl;
  TCanvas * cVz1 = new TCanvas("cVz1","cVz1",500,500);
  // Some Event Statistics
  djtree->Draw("vz","vz>-99");
  Int_t numPreSelEvtl = djtree->GetEntries("vz>-99");
  cout << "Events:" << endl;
  cout << "- with none-fake vtx: " << numPreSelEvtl << endl;

  //  - selection -
  TCut evtSelDj("nlpet>80 && nlpet<120 && alpet>80");
  TCut evtSelDj2("nlpet>80 && nlpet<120 && alpet>80 && jdphi>3.0");
  Int_t numSelEvt1 = djtree->GetEntries(evtSelDj);
  cout << "- Passing sel: " << numSelEvt1 << endl;
  Int_t numSelEvt2 = djtree->GetEntries(evtSelDj2);
  cout << "- Passing diphi sel: " << numSelEvt2 << endl;

  //  - dphi -
  TCanvas * cDPhi = new TCanvas("cDPhi","cDPhi",500,500);
  TH1D * hDPhi = new TH1D("hDPhi","jdphi",50,0,3.14);
  hDPhi->SetTitle(";reco jet d #phi;");
  djtree->Draw("jdphi>>hDPhi",evtSelDj);
  cDPhi->Print("plots/cDPhi.gif");

  //  - jes -
  TProfile * hDJesNr = (TProfile*)hJes->Clone("hDJesNr");
  hDJesNr->SetMarkerColor(kRed);
  hDJesNr->SetMarkerStyle(kOpenCircle);
  hDJesNr->SetTitle(";p_{T}^{gen jet};p_{T}^{reco jet}/p_{T}^{gen jet}");
  TCanvas * cDJesNr = new TCanvas("cDJesNr","cDJesNr",500,500);
  djtree->Draw("nljet/nlpet:nlpet>>hDJesNr",evtSelDj);

  TProfile * hDJes2Nr = (TProfile*)hDJesNr->Clone("hDJes2Nr");
  hDJes2Nr->SetMarkerColor(kBlue);
  hDJes2Nr->SetMarkerStyle(kOpenSquare);
  TCanvas * cDJes2Nr = new TCanvas("cDJes2Nr","cDJes2Nr",500,500);
  djtree->Draw("nljet/nlpet:nlpet>>hDJes2Nr",evtSelDj2);

  TProfile * hDJesAw = (TProfile*)hDJesNr->Clone("hDJesAw");
  hDJesAw->SetMarkerColor(kBlue);
  TCanvas * cDJesAw = new TCanvas("cDJesAw","cDJesAw",500,500);
  djtree->Draw("aljet/alpet:alpet>>hDJesAw",evtSelDj);

  // === Final Plots ===
  TCanvas * cFinalJes = new TCanvas("cFinalJes","cFinalJes",500,500);
  hJes->Draw("E");
  hDJesNr->Draw("same E");
  //hDJes2Nr->Draw("same E");
  hDJesAw->Draw("same E");
  TLegend *leg2 = new TLegend(0.605,0.216,0.905,0.369,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.03);
  leg2->AddEntry(hJes,"Incl. Jets","p");
  leg2->AddEntry(hDJesNr,"Di-Jets, Near Jet","p");
  //leg2->AddEntry(hDJes2Nr,"Di-Jets d#phi>3, Near Jet","p");
  leg2->AddEntry(hDJesAw,"Di-Jets, Away Jet","p");
  leg2->Draw();
  cFinalJes->Print("plots/cFinalJes.gif");
}
