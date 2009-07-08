#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"

void compareMatch (char * fpath, TFile * fin, char * jtype="Gen", char * draw="et",int nbin=100,float xmin=0, float xmax=200)
{
  TTree * ntall = dynamic_cast<TTree*>(fin->Get(Form("ntAll%sJets",jtype)));
  TTree * ntmat = dynamic_cast<TTree*>(fin->Get(Form("ntMat%sJets",jtype)));
  TTree * ntunmat = dynamic_cast<TTree*>(fin->Get(Form("ntUnMat%sJets",jtype)));

//  ntall->Print();
//  ntmat->Print();
//  ntunmat->Print();

  TCanvas * c = new TCanvas(Form("c%s_%s",jtype,draw),Form("c%s_%s",jtype,draw),400,400);

  ntall->Draw(Form("%s>>h1(%d,%f,%f)",draw,nbin,xmin,xmax));
  TH1F * h1 = dynamic_cast<TH1F*>(gDirectory->FindObject("h1"));
  h1->SetXTitle(draw);
  h1->SetLineColor(kBlack);
  h1->SetLineWidth(3);
  h1->SetTitle(Form("%s_%s",jtype,draw));
  ntmat->Draw(Form("%s>>h2(%d,%f,%f)",draw,nbin,xmin,xmax),"","same");
  TH1F * h2 = dynamic_cast<TH1F*>(gDirectory->FindObject("h2"));
  h2->SetLineColor(kRed);
  h2->SetLineWidth(3);
  ntunmat->Draw(Form("%s>>h3(%d,%f,%f)",draw,nbin,xmin,xmax),"","same"); 
  TH1F * h3 = dynamic_cast<TH1F*>(gDirectory->FindObject("h3"));
  h3->SetLineColor(kBlue);
  h3->SetLineWidth(3);
  h3->Draw("same");

  c->Print(Form("%s/%s_%s.gif",fpath,jtype,draw));
  h1->Delete();
  h2->Delete();
  h3->Delete();
}

void plotMatch (char * fpath=".",char * infname="plots.root")
{
   TFile * fin = new TFile(Form("%s/%s",fpath,infname));
   compareMatch(fpath,fin,"Gen","et",100,0,200);
   compareMatch(fpath,fin,"Gen","eta",100,-5,5);
   compareMatch(fpath,fin,"Gen","phi",100,-3.14,3.14);

   compareMatch(fpath,fin,"Reco","et",100,0,200);
   compareMatch(fpath,fin,"Reco","eta",100,-5,5);
   compareMatch(fpath,fin,"Reco","phi",100,-3.14,3.14);
}
