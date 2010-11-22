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
  Float_t nSelEvt=tr->GetEntries(cut);
  cout << "Sel Evt:  " << TString(cut) << ": " << nSelEvt << endl;
  TH1D * hist = new TH1D(name,title,nbins,min,max);
  Int_t npass = tr->Draw(var+">>"+name,cut,"goff");
  cout << "Sel Draw: " << TString(cut) << ": " << npass << endl;
  if (normType==3)
    hist->Scale(1./nSelEvt);
  return hist;
}

TH2D * plot2D(TTree * tr,TCut cut, TString var, TString name, TString title,Int_t xnbins, Float_t xmin, Float_t xmax, Int_t ynbins, Float_t ymin, Float_t ymax)
{
  cout << "Sel: " << TString(cut) << ": " << tr->GetEntries(cut) << endl;
  TH2D * hist = new TH2D(name,title,xnbins,xmin,xmax,ynbins,ymin,ymax);
  tr->Draw(var+">>"+name,cut,"goff");
  return hist;
}

TH1D * plotDPhi(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,TCut("("+TString(cut)+")*ppt"),var,name,";d #phi (j1,tower);Et^{Tower}",36,0,3.14,normType);
}

TH2D * plotJEtCorr(TTree * tr, TCut cut, TString var, TString name, TString title, Int_t normType=0)
{
  TH2D * hist = plot2D(tr,cut,var,name,title,80,0,250,40,0,2);
  TLine *l = new TLine(0,1,250,1);
  l->SetLineStyle(2);
  TCanvas * cJEtCorr0 = new TCanvas("c"+name,"c"+name,500,500);
  cJEtCorr0->SetRightMargin(0.15);
  hist->Draw("colz");
  l->Draw();
  return hist;
}

TChain * compDataMcJetTrk(
    TString module="djcalo_tower",
    TString datafile="dj_HCPR-J50U150883to151217-djlook1120.root",
    TString mcfile0="",
    TString mcfile1="",
    TString header="HLT_HIJet50U_Core"
    )
{
  TH1::SetDefaultSumw2();
  TChain * djdata = new TChain(module+"/djTree");
  TChain * djmc0 = new TChain(module+"/djTree");
  TChain * djmc1 = new TChain(module+"/djTree");

  djdata->Add(datafile);
  djdata->AddFriend("hltanalysis/HltTree",datafile);
  /*
  djmc0->Add(mcfile0);
  djmc0->AddFriend("hltanalysis/HltTree",mcfile0);
  djmc1->Add(mcfile0);
  djmc1->AddFriend("hltanalysis/HltTree",mcfile0);
  */

  cout << "Data Total Events: " << djdata->GetEntries() << endl;
  //cout << "MC0 Total Events: " << djmc0->GetEntries() << endl;
  //cout << "MC1 Total Events: " << djmc1->GetEntries() << endl;

  TCut evtSelMc("cent<10 && nljet>100 && nljet<500 && lppt[0]>10");
  TCut evtSelData = evtSelMc && "hlt[2]";
  TCut trkSel("ppt>5&&abs(peta)");

  TH1D * hPNDPhiData = plotDPhi(djdata,evtSelData&&trkSel,"abs(pndphi)","hPNDPhiData",3);

  TCanvas * cPNDPhi = new TCanvas("cPNDPhi","cPNDPhi",500,500);
  CPlot cpPNDPhi("PNDPhi","PNDPhi","d #phi (j1,tower)","1/N^{j1} Et^{Tower}");
  cpPNDPhi.SetXRange(0,3.1416);
  cpPNDPhi.SetYRange(0,200);
  cpPNDPhi.AddHist1D(hPNDPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpPNDPhi.SetLegendHeader("|#eta|^{Tower}<3");
  cpPNDPhi.Draw(cPNDPhi,false);

  return djdata;
}
