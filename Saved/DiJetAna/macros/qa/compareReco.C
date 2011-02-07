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
#include "../commonUtility.h"

void compareTree(TTree * t1, TTree * t2, TString var, TCut sel, TCut cut, TString name, TString title, Int_t nbin, Float_t min, Float_t max, Float_t ymin=0, Float_t ymax=0)
{
  TH1D * h = new TH1D(name,title,nbin,min,max);
  TH1D * h2 = new TH1D(name+"2",title,nbin,min,max);
  t1->Project(name,var,sel&&cut);
  t2->Project(name+"2",var,sel&&cut);
  h->SetMarkerStyle(kOpenSquare);
  h->SetMarkerColor(kRed);
  h->SetLineColor(kRed);
  h2->SetMarkerStyle(kFullCircle);

  // normalize
  Int_t numSel = t1->GetEntries(sel);
  Int_t numSel2 = t2->GetEntries(sel);
  cout << "1. Event sel: " << TString(sel) << " " << numSel << endl;
  cout << "2. Event sel: " << TString(sel) << " " << numSel2 << endl;
  h->Scale(1./numSel);
  h2->Scale(1./numSel2);

  // draw
  if (ymax>ymin) {
    cout << "ymin " << ymin << " ymax " << ymax << endl;
    h->SetAxisRange(ymin,ymax,"Y");
  }
  h->Draw("hist");
  h2->Draw("sameE");

  TLegend *leg = new TLegend(0.54,0.77,0.85,0.86);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(h,"MinBias HF or BSC","");
  leg->AddEntry(h,"Prompt Reco","l");
  leg->AddEntry(h2,"ReRECO","p");
  leg->Draw();
}

void compareReco(TString infname = "data.root",
    TString infname2 = "data2.root",
    TCut myCut = "cent<30"
    )
{
   // ===========================================================
   // Get Input
   // ===========================================================
   TH1::SetDefaultSumw2();
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("djTree");
   TChain * t2 = new TChain("djcalo_tower/djTree");
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

   TCanvas * c100 = new TCanvas("c100","c100",600,600);
   compareTree(t,t2,"cent",evtCut&&myCut,"","hCent",";Centrality (%);count/event;",20,0,100,0,0.2);

   TCanvas * c2 = new TCanvas("c2","c2",600,600);
   c2->SetLogy();
   compareTree(t,t2,"ppt",evtCut&&myCut,"abs(peta)<3","hPPt",";tower p_{T} (GeV);count/event;",50,0,50);

   TCanvas * c3 = new TCanvas("c3","c3",600,600);
   compareTree(t,t2,"peta",evtCut&&myCut,"abs(peta)<3","hPEta",";tower #eta;count/event;",100,-3,3,0,25);

   TCanvas * c4 = new TCanvas("c4","c4",600,600);
   compareTree(t,t2,"pphi",evtCut&&myCut,"abs(peta)<3","hPPhi",";tower #phi;count/event;",36,-3.14,3.14,0,25); // 72
}


