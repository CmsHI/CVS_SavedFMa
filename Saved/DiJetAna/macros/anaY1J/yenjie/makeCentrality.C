#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>

void makeCentrality(char *infname = "dj_data100_cor.root")
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->FindObjectAny("djTree");
   TFile *outf = new TFile("Centrality.root","recreate");
   TCut evtCut = "nljet>120&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2./3*TMath::Pi()";
   TH1D *h = new TH1D("h","",40,0,100);
   t->Draw("cent>>h");
   h->Sumw2();
   h->Scale(1./h->GetEntries());
   h->Write();
   outf->Write();
}
