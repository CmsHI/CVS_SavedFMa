#include <iostream>
#include <TH1D.h>
#include <TTree.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>
#include <TProfile.h>

void leadFrag(char *infname = "dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root",
    TCut myCut = "cent<30",
    TString module="djcalo_tower")
{
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get(module+"/djTree");

   const int nBin = 5;
   double bins[nBin+1] = {0.5,1.5,4,8,20,1000};  
   double colors[nBin] = {kGray,kBlue-3,38,kOrange-8,kRed-6};

   // Selection cut
   TCut evtCut = "nljet>120&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2./3*TMath::Pi()";
   t->SetAlias("Aj","(nljet-aljet)/(nljet+aljet)");

   // histograms
   TH2D * hFrag_LJEt = new TH2D("hFrag_LJEt",";p_{T}^{J1};p_{T}^{Tower}/p_{T}^{J1}",50,120,300,50,0,1);
   hFrag_LJEt->GetXaxis()->CenterTitle();
   hFrag_LJEt->GetYaxis()->CenterTitle();
   TH2D * hLeadFrag_LJEt = new TH2D("hLeadFrag_LJEt",";p_{T}^{J1};p_{T}^{Highest Tower}/p_{T}^{J1}",50,120,300,50,0,1);
   hLeadFrag_LJEt->GetXaxis()->CenterTitle();
   hLeadFrag_LJEt->GetYaxis()->CenterTitle();

   // project tree
   t->Project("hLeadFrag_LJEt","lppt[0]/nljet:nljet",evtCut);
   t->Project("hFrag_LJEt","ppt/nljet:nljet",evtCut);


   // plot
   TCanvas *c2 = new TCanvas("c2","",600,600);
   c2->SetRightMargin(0.15);
   c2->SetLogz();
   hFrag_LJEt->Draw("colz");

   TCanvas *c3 = new TCanvas("c3","",600,600);
   c3->SetRightMargin(0.15);
   c3->SetLogz();
   hLeadFrag_LJEt->Draw("colz");

   TCanvas *c4 = new TCanvas("c4","",600,600);
   hLeadFrag_LJEt->ProfileX()->Draw("E");
   TH1D * hLeadFrag_LJEt_pfx = (TH1D*)gDirectory->Get("hLeadFrag_LJEt_pfx");
   hLeadFrag_LJEt_pfx->SetAxisRange(0,0.6,"Y");
   hFrag_LJEt->ProfileX()->Draw("same hist");
   TH1D * hFrag_LJEt_pfx = (TH1D*)gDirectory->Get("hFrag_LJEt_pfx");
}
