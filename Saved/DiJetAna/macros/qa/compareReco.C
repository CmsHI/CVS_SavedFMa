#include <iostream>
#include <TH1D.h>
#include "TChain.h"
#include <TTree.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>
#include <TProfile.h>
#include <TText.h>
#include "commonUtility.h"
#include "DrawTick.C"
#include "colorScheme.h"

void compareTree(TTree * t1, TTree * t2, TString var, TCut cut, TString name, TString title, Int_t nbin, Float_t min, Float_t max)
{
  TH1D * h = new TH1D(name,title,nbin,min,max);
  TH1D * h2 = new TH1D(name+"2",title,nbin,min,max);
  t1->Project(name,var,cut);
  t2->Project(name+"2",var,cut);
  h->SetMarkerStyle(kOpenSquare);
  h->SetMarkerColor(kRed);
  h->SetLineColor(kRed);
  h->Draw("E");
  h2->Draw("sameE");

  TLegend *leg = new TLegend(0.49,0.24,0.8,0.33);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(h,"Prompt Reco","p");
  leg->AddEntry(h2,"ReRECO","p");
  leg->Draw();
}

void compareReco(TString infname = "../data.root",
    TString infname2 = "../compare.root",
    TCut myCut = "cent<30"
    )
{
   // ===========================================================
   // Get Input
   // ===========================================================
   TH1::SetDefaultSumw2();
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("djTree");
   TChain * t2 = new TChain("djTree");
   t2->Add(infname2);

   // ===========================================================
   // Analysis Setup
   // ===========================================================
  // Selection cut
   //TCut evtCut = "nljet>120&&abs(nljeta)<1.6&&aljet>50&&abs(aljeta)<1.6&&jdphi>2./3*TMath::Pi()&&!maskEvt";
   TCut evtCut = "";
   Float_t numSel = t->GetEntries(evtCut&&myCut);
   Float_t numSel2 = t2->GetEntries((evtCut&&myCut));
   cout << "Base Sel evt: " << numSel << endl;
   cout << "Comp Sel evt: " << numSel2 << endl;

   TCanvas * c2 = new TCanvas("c2","c2",1200,600);
   compareTree(t,t2,"ppt",evtCut&&myCut,"hPPt",";p_{T} (GeV);u.n;",50,0,200);
   //TLine * l = new TLine(-3.14,1,3.14,1);
   //l->SetLineStyle(2);
   //l->Draw();
}


