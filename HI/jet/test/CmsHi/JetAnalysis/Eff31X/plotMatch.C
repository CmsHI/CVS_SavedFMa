#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"

void compareMatch (TFile * fin, char * jtype="Gen", char * draw="et")
{
  TTree * ntall = dynamic_cast<TTree*>(fin->Get(Form("ntAll%sJets",jtype)));
  TTree * ntmat = dynamic_cast<TTree*>(fin->Get(Form("ntMat%sJets",jtype)));
  TTree * ntunmat = dynamic_cast<TTree*>(fin->Get(Form("ntUnMat%sJets",jtype)));

//  ntall->Print();
//  ntmat->Print();
//  ntunmat->Print();

  TCanvas * c = new TCanvas(Form("c%s_%s",jtype,draw),Form("c%s_%s",jtype,draw));

  ntall->Draw(Form("%s>>h1",draw));
  TH1F * h1 = dynamic_cast<TH1F*>(gDirectory->FindObject("h1"));
  h1->SetLineColor(kBlack);
  h1->SetLineWidth(3);
  h1->SetTitle(Form("%s_%s",jtype,draw));
  ntmat->Draw(Form("%s>>h2",draw),"","same");
  TH1F * h2 = dynamic_cast<TH1F*>(gDirectory->FindObject("h2"));
  h2->SetLineColor(kRed);
  h2->SetLineWidth(3);
  ntunmat->Draw(Form("%s>>h3",draw),"","same"); 
  TH1F * h3 = dynamic_cast<TH1F*>(gDirectory->FindObject("h3"));
  h3->SetLineColor(kBlue);
  h3->SetLineWidth(3);
  h3->Draw("same");

  c->Print(Form("plots/%s_%s.gif",jtype,draw));
  h1->Delete();
  h2->Delete();
  h3->Delete();
}

void plotMatch ()
{
   TFile * fin = new TFile("plots.root");
   compareMatch(fin,"Gen","et");
   compareMatch(fin,"Gen","eta");
   compareMatch(fin,"Gen","phi");

   compareMatch(fin,"Reco","et");
   compareMatch(fin,"Reco","eta");
   compareMatch(fin,"Reco","phi");
}
