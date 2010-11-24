#include "TChain.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>

#include "Saved/Utilities/macros/cplot/CPlot.h"

using namespace std;

TH1D * plot1D(TTree * tr,TCut cut,TString var, TString name, TString title,Int_t nbins, Float_t min, Float_t max, Int_t normType=0, Float_t normFactor=1.)
{
  cout << "Sel: " << TString(cut) << ": " << tr->GetEntries(cut) << endl;
  TH1D * hist = new TH1D(name,title,nbins,min,max);
  tr->Draw(var+">>"+name,cut,"goff");
  if (normType==3)
    hist->Scale(normFactor);
  return hist;
}

TH2D * plot2D(TTree * tr,TCut cut, TString var, TString name, TString title,Int_t xnbins, Float_t xmin, Float_t xmax, Int_t ynbins, Float_t ymin, Float_t ymax)
{
  cout << "Sel: " << TString(cut) << ": " << tr->GetEntries(cut) << endl;
  TH2D * hist = new TH2D(name,title,xnbins,xmin,xmax,ynbins,ymin,ymax);
  tr->Draw(var+">>"+name,cut,"goff");
  return hist;
}

TH1D * plotJEt(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0,Float_t normFactor=1.)
{
  return plot1D(tr,cut,var,name,";E_{T}^{Jet};#",20,0,250,normType,normFactor);
}

TH2D * plotJEtCorr(TTree * tr, TCut cut, TString var, TString name, TString title, Int_t normType=0)
{
  TH2D * hist = plot2D(tr,cut,var,name,title,100,0,250,40,0,2);
  TLine *l = new TLine(0,1,250,1);
  l->SetLineStyle(2);
  TCanvas * cJEtCorr0 = new TCanvas("c"+name,"c"+name,500,500);
  cJEtCorr0->SetRightMargin(0.15);
  hist->Draw("colz");
  l->Draw();
  return hist;
}

TChain * trigSpectra(
    //TString infile="../process_aod/dj_HCPR-J50U-151020to151076.root",
    TString infile="~/scratch01/ana/HCPR-GoodRuns-151027and151058_jet1123/merge/dj_HCPR-MB-151027and151058_jet1123.root",
    TString header="HLT_HIJet50U_Core",
    //TString mbfile="../process_aod/dj_HCPR-MB-151020to151076_trigana1116.root",
    TString mbfile="~/scratch01/ana/HCPR-GoodRuns-151027and151058_mb1123/merge/dj_HCPR-MB-151027and151058_mb1123.root",
    TString mbheader="HLT_HIMinBiasHfOrBSC_Core"
    )
{
  TH1::SetDefaultSumw2();
  TChain * dj = new TChain("djcalo/djTree");
  TChain * mbdj = new TChain("djcalo/djTree");

  dj->Add(infile);
  //dj->AddFriend("joc = djcaloJOC/djTree",infile);
  //dj->AddFriend("ic5 = djcaloic5/djTree",infile);
  //dj->AddFriend("ak5 = djcaloak5/djTree",infile);
  //dj->AddFriend("kt4 = djcalokt4/djTree",infile);
  dj->AddFriend("hltanalysis/HltTree",infile);
  cout << "Trig Total: " << dj->GetEntries() << endl;
  mbdj->Add(mbfile);
  //mbdj->AddFriend("joc = djcaloJOC/djTree",mbfile);
  //mbdj->AddFriend("ic5 = djcaloic5/djTree",mbfile);
  //mbdj->AddFriend("ak5 = djcaloak5/djTree",mbfile);
  //mbdj->AddFriend("kt4 = djcalokt4/djTree",mbfile);
  mbdj->AddFriend("hltanalysis/HltTree",mbfile);
  cout << "MB Total: " << mbdj->GetEntries() << endl;

  //TCut evtSelMB("hlt[0] && cent<100");
  //TCut evtSel("hlt[2] && cent<100");
  TCut evtSelMB("hlt[0] && cent<10 && abs(nljeta)<2");
  TCut evtSel("hlt[2] && cent<10 && abs(nljeta)<2");

  TH1D * hJEtIc5puMB = plotJEt(mbdj,evtSelMB,"nljet","hJEtIc5puMB",0);
  TH1D * hJEtIc5pu = plotJEt(dj,evtSel,"nljet","hJEtIc5pu",3,1./16.14);
  //TH1D * hJEtIc5pu = plotJEt(mbdj,evtSel,"nljet","hJEtIc5pu",3,1./15.45);
  //TH1D * hJEtIc5pu = plotJEt(dj,evtSel,"nljet","hJEtIc5pu",3,1./20);
  TH1D * hJEtRat = (TH1D*)hJEtIc5pu->Clone("hJEtRat");
  hJEtRat->Divide(hJEtIc5pu,hJEtIc5puMB);

  TCanvas * cJEt = new TCanvas("cJEt","cJEt",500,500);
  CPlot cpJEt("JEt","JEt","leading E_{T}^{Jet, Corr}","#");
  cpJEt.SetLogy();
  cpJEt.AddHist1D(hJEtIc5puMB,"HLT_HIMinBiasHForBSC_Core","E",kBlue,kFullCircle);
  cpJEt.AddHist1D(hJEtIc5pu,"HLT_HIJet50U_Core","E",kBlack,kOpenSquare);
  cpJEt.SetLegend(0.41,0.78,0.74,0.90);
  cpJEt.SetLegendHeader(header+", |#eta|^{Jet}<2");
  cpJEt.Draw(cJEt,false);

  TCanvas * cJEtRat = new TCanvas("cJEtRat","cJEtRat",500,500);
  hJEtRat->Draw("E");
  hJEtRat->Fit("pol0","","",90,150);
  hJEtRat->SetTitle(";leading E_{T}^{Jet, Corr};Trig/MB");

  TH1D * hCentMB = new TH1D("hCentMB",";Centrality;unit norm",20,0,100);
  TH1D * hCentTrig = new TH1D("hCentTrig",";Centrality;unit norm",20,0,100);
  mbdj->Draw("cent>>hCentMB",evtSelMB,"goff");
  dj->Draw("cent>>hCentTrig",evtSel,"goff");
  TCanvas * cCent = new TCanvas("cCent","cCent",500,500);
  hCentMB->SetMinimum(0.1);
  hCentMB->Scale(1./(hCentMB->Integral()*5));
  hCentTrig->Scale(1./(hCentTrig->Integral()*5));
  hCentMB->SetMarkerColor(kBlue);
  hCentMB->SetLineColor(kBlue);
  hCentTrig->SetMarkerStyle(kOpenSquare);
  hCentTrig->Draw("E");
  hCentMB->Draw("Esame");
  //CPlot cpCent("Cent","Cent","Centrality","#");
  //cpCent.AddHist1D(hCentTrig,"HLT_HIJet50U_Core","E",kBlack,kOpenSquare);
  //cpCent.AddHist1D(hCentMB,"HLT_HIMinBiasHForBSC_Core","Esame",kBlue,kFullCircle);
  //cpCent.Draw(cCent,false);

  return dj;
}
