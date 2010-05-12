#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TCut.h"
#include "TLegend.h"
#include "CmsHi/DiJetAna/ana/aliases_dijet.C"
#include "CmsHi/DiJetAna/ana/selectionCut.h"
using namespace std;

void anaJes(int doMC=1,
    const char * inFile0Name="../process_aod/outputs/dijetAna_anaJet_Mc1_2k.root")
{
  // Define Inputs
  cout << "======= Inputs: ========" << endl;
  cout << inFile0Name << endl;

  TFile * inFile0 = new TFile(inFile0Name);
  inFile0->ls();
  TTree *mcj2t3, *mcj2t3peri, *mcj2t0, *mcj1t0;
  inFile0->GetObject("dijetAna_mc/djTree",mcj2t3);
  inFile0->GetObject("dijetAna_mc_periph/djTree",mcj2t3peri);

  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "dijetAna_mc_periph/mcj2t3 # entries: " << mcj2t3peri->GetEntries() << endl;

  // Define dijet selection
  selectionCut mcAna(1,1);
  cout << "DJ selection: " << TString(mcAna.DJ) << endl;
  cout << "dijetAna_mc/mcj2t3 # entries: " << mcj2t3->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3->GetEntries(mcAna.DJ) << endl;
  cout << "dijetAna_mc_periph/mcj2t3 # entries: " << mcj2t3peri->GetEntries() << endl;
  cout << "# DJ events passed: " << mcj2t3peri->GetEntries(mcAna.DJ) << endl;

  //  - DiJet jes -
  TProfile * hJes = new TProfile("hJes",";p_{T}^{gen jet};p_{T}^{reco jet}/p_{T}^{gen jet}",20,mcAna.nrJetPtMin,mcAna.nrJetPtMax);
  hJes->SetMinimum(0);
  hJes->SetMaximum(1.2);

  TProfile * hDJesNr = (TProfile*)hJes->Clone("hDJesNr");
  hDJesNr->SetMarkerColor(kRed);
  hDJesNr->SetLineColor(kRed);
  hDJesNr->SetMarkerStyle(kOpenCircle);
  TCanvas * cDJesNr = new TCanvas("cDJesNr","cDJesNr",500,500);
  mcj2t3->Draw("nljet/nlrjet:nlrjet>>hDJesNr",mcAna.DJ,"E");

  TProfile * hDJesAw = (TProfile*)hDJesNr->Clone("hDJesAw");
  hDJesAw->SetMarkerColor(kBlue);
  hDJesAw->SetLineColor(kBlue);
  TCanvas * cDJesAw = new TCanvas("cDJesAw","cDJesAw",500,500);
  mcj2t3->Draw("aljet/alrjet:alrjet>>hDJesAw",mcAna.DJ);

  // === Final Jes Plots ===
  TCanvas * cFinalJes = new TCanvas("cFinalJes","cFinalJes",500,500);
  hDJesNr->Draw("E");
  hDJesAw->Draw("same E");
  TLegend *leg2 = new TLegend(0.605,0.216,0.905,0.369,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.03);
  leg2->AddEntry(hDJesNr,"Leading Jet","p");
  leg2->AddEntry(hDJesAw,"Away Jet","p");
  leg2->Draw();
  cFinalJes->Print("plots/cFinalJes.gif");
}
