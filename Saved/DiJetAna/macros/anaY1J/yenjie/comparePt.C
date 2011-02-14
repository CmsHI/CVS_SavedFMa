#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>


void comparePt()
{
   TFile *inf = new TFile ("dj_mix100.root");
   TTree *tReco = (TTree*)inf->FindObjectAny("djTree");
   TFile *inf2 = new TFile ("dj_mix100_Gen.root");
   TTree *tGen = (TTree*)inf2->FindObjectAny("djTree");

   TCanvas *c = new TCanvas("c","",600,600);
   TH1D *hReco = new TH1D("hReco","",100,10,150);
   TH1D *hGen = new TH1D("hGen","",100,10,150);
   TCut evtCut = "nljet>120&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2./3*TMath::Pi()";

   tReco->Draw("ppt>>hReco","abs(peta)<1"&&evtCut,"prof");
   tGen->Draw("ppt>>hGen","abs(peta)<1&&pch!=0"&&evtCut,"prof");

   hReco->SetMarkerStyle(4);
   hReco->SetMarkerColor(2);
   hReco->SetLineColor(2);
   hGen->SetXTitle("p_{T} (GeV/c)");
   hGen->GetXaxis()->CenterTitle();
   hGen->GetYaxis()->CenterTitle();
   hGen->SetYTitle("Entries");
//   hGen->SetAxisRange(-30,30,"Y");
   hGen->Draw("");
   hReco->Draw("same");
   
   
   TLine *l = new TLine(0,0,100,0);
   l->SetLineStyle(2);
   l->Draw();
    
   TLegend *leg = new TLegend(0.25,0.22,0.61,0.51);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);

   leg->AddEntry(hReco,"Reco","pl");
   leg->AddEntry(hGen,"Gen","pl");

   leg->Draw();
}
