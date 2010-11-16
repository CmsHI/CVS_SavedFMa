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

TH1D * plot1D(TTree * tr,TCut cut,TString var, TString name, TString title,Int_t nbins, Float_t min, Float_t max, Int_t normType=0)
{
  TH1D * hist = new TH1D(name,title,nbins,min,max);
  tr->Draw(var+">>"+name,cut,"goff");
  return hist;
}

TH2D * plot2D(TTree * tr,TCut cut, TString var, TString name, TString title,Int_t xnbins, Float_t xmin, Float_t xmax, Int_t ynbins, Float_t ymin, Float_t ymax)
{
  TH2D * hist = new TH2D(name,title,xnbins,xmin,xmax,ynbins,ymin,ymax);
  tr->Draw(var+">>"+name,cut,"goff");
  return hist;
}

TH1D * plotJEt(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,cut,var,name,";E_{T}^{Jet};#",80,0,200,normType);
}

TH2D * plotJEtCorr(TTree * tr, TCut cut, TString var, TString name, TString title, Int_t normType=0)
{
  TH2D * hist = plot2D(tr,cut,var,name,title,80,0,200,40,0,2);
  TLine *l = new TLine(0,1,200,1);
  l->SetLineStyle(2);
  TCanvas * cJEtCorr0 = new TCanvas("c"+name,"c"+name,500,500);
  cJEtCorr0->SetRightMargin(0.15);
  hist->Draw("colz");
  l->Draw();
  return hist;
}

TChain * compJetAlgos(
    TString infile="../process_aod/dj_icPu5patJets_try10.root",
    TString header="HLT_HIJet50U"
    )
{
  TChain * dj = new TChain("djcalo/djTree");

  dj->Add(infile);
  dj->AddFriend("joc = djcaloJOC/djTree",infile);
  dj->AddFriend("ic5 = djcaloic5/djTree",infile);
  dj->AddFriend("ak5 = djcaloak5/djTree",infile);
  dj->AddFriend("kt4 = djcalokt4/djTree",infile);
  cout << "Total: " << dj->GetEntries() << endl;

  TCut evtSel("cent<100");
  cout << "Evt Sel: " << TString(evtSel) << ": " << dj->GetEntries(evtSel) << endl;

  TH1D * hJEtIc5pu = plotJEt(dj,evtSel,"nljet","hJEtIc5pu",0);
  TH1D * hJEtIc5puJOC = plotJEt(dj,evtSel,"joc.nljet","hJEtIc5puJOC",0);
  TH1D * hJEtIc5 = plotJEt(dj,evtSel,"ic5.nljet","hJEtIc5",0);
  TH1D * hJEtAk5 = plotJEt(dj,evtSel,"ak5.nljet","hJEtAk5",0);
  TH1D * hJEtKt4 = plotJEt(dj,evtSel,"kt4.nljet","hJEtKt4",0);

  TCut matEvtSel(evtSel);
  bool doMat = true;
  if (doMat) matEvtSel = evtSel && "abs(joc.nljeta-nljeta)<0.2";
  TH2D * hJEtJOCIc5pu = plotJEtCorr(dj,matEvtSel,"joc.nljet/nljet:nljet","hJEtJOCIc5pu",";leading E_{T}^{Ic5pu} [GeV];E_{T}^{Ic5pu Online}/E_{T}^{Ic5pu}");
  if (doMat) matEvtSel = evtSel && "abs(kt4.nljeta-nljeta)<0.2";
  TH2D * hJEtKt4Ic5pu = plotJEtCorr(dj,matEvtSel,"kt4.nljet/nljet:nljet","hJEtKt4Ic5pu",";leading E_{T}^{Ic5pu} [GeV];E_{T}^{Kt4}/E_{T}^{Ic5pu}");
  if (doMat) matEvtSel = evtSel && "abs(joc.nljeta-ak5.nljeta)<0.2";
  TH2D * hJEtJOCAk5 = plotJEtCorr(dj,matEvtSel,"joc.nljet/ak5.nljet:ak5.nljet","hJEtJOCAk5",";leading E_{T}^{Ak5} [GeV];E_{T}^{Ic5pu Online}/E_{T}^{Ak5}");
  if (doMat) matEvtSel = evtSel && "abs(nljeta-ak5.nljeta)<0.2";
  TH2D * hJEtIc5puAk5 = plotJEtCorr(dj,matEvtSel,"nljet/ak5.nljet:ak5.nljet","hJEtIc5puAk5",";leading E_{T}^{Ak5} [GeV];E_{T}^{Ic5pu}/E_{T}^{Ak5}");
  if (doMat) matEvtSel = evtSel && "abs(kt4.nljeta-ak5.nljeta)<0.2";
  TH2D * hJEtKt4Ak5 = plotJEtCorr(dj,matEvtSel,"kt4.nljet/nljet:ak5.nljet","hJEtKt4Ak5",";leading E_{T}^{Ak5} [GeV];E_{T}^{Kt4}/E_{T}^{Ak5}");

  TCanvas * cJEt = new TCanvas("cJEt","cJEt",500,500);
  CPlot cpJEt("JEt","JEt","leading E_{T}^{Jet}","#");
  cpJEt.SetLogy();
  cpJEt.AddHist1D(hJEtIc5puJOC,"Ic5pu (Online)","E",kGreen+2,kOpenSquare);
  cpJEt.AddHist1D(hJEtIc5pu,"Ic5pu (Offline)","E",kBlack,kFullCircle);
  cpJEt.AddHist1D(hJEtAk5,"Ak5","E",kRed,kOpenCircle);
  //cpJEt.AddHist1D(hJEtKt4,"Kt4","E",kBlue,kOpenSquare);
  cpJEt.SetLegendHeader(header+", |#eta|^{Jet}<3");
  cpJEt.Draw(cJEt,false);

  return dj;
}
