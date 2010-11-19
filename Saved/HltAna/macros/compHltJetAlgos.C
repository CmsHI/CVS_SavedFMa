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
  cout << "Sel: " << TString(cut) << ": " << tr->GetEntries(cut) << endl;
  TH1D * hist = new TH1D(name,title,nbins,min,max);
  tr->Draw(var+">>"+name,cut,"goff");
  return hist;
}

TH2D * plot2D(TTree * tr,TCut cut, TString var, TString name, TString title,Int_t xnbins, Float_t xmin, Float_t xmax, Int_t ynbins, Float_t ymin, Float_t ymax)
{
  cout << "Sel: " << TString(cut) << ": " << tr->GetEntries(cut) << endl;
  TH2D * hist = new TH2D(name,title,xnbins,xmin,xmax,ynbins,ymin,ymax);
  tr->Draw(var+">>"+name,cut,"goff");
  return hist;
}

TH1D * plotJEt(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,cut,var,name,";E_{T}^{Jet};#",80,0,120,normType);
}

TH2D * plotJEtCorr(TTree * tr, TCut cut, TString var, TString name, TString title, Int_t normType=0)
{
  TH2D * hist = plot2D(tr,cut,var,name,title,80,0,120,40,0,2);
  TLine *l = new TLine(0,1,120,1);
  l->SetLineStyle(2);
  TCanvas * cJEtCorr0 = new TCanvas("c"+name,"c"+name,500,500);
  cJEtCorr0->SetRightMargin(0.15);
  hist->Draw("colz");
  l->Draw();
  return hist;
}

TChain * compHltJetAlgos(
    //TString infile="../process_aod/dj_HCPR-J50U-151020to151076.root",
    //TString header="HLT_HIJet50U_Core",
    TString mbfile="../process_aod/dj_HCPR-MB-151020to151076_trigana1116.root",
    TString mbheader="HLT_HIMinBiasHfOrBSC_Core"
    )
{
  //TChain * dj = new TChain("hltanalysis/HltTree");
  TChain * mbdj = new TChain("hltanalysis/HltTree");

  //dj->Add(infile);
  //dj->AddFriend("joc = hltanalysisJOC/HltTree",infile);
  //cout << "Trig Total: " << dj->GetEntries() << endl;
  mbdj->Add(mbfile);
  mbdj->AddFriend("joc = hltanalysisJOC/HltTree",mbfile);
  cout << "MB Total: " << mbdj->GetEntries() << endl;

  //TCut evtSel("HLT_HIJet50U_Core && hiBin*2.5<100");
  TCut evtSelMB("HLT_HIMinBiasHfOrBSC_Core && hiBin*2.5<100");

  TH1D * hJEtIc5pu = plotJEt(mbdj,evtSelMB,"recoJetCalPt[0]","hJEtIc5pu",0);
  TH1D * hJEtIc5puJOC = plotJEt(mbdj,evtSelMB,"joc.recoJetCalPt[0]","hJEtIc5puJOC",0);

  TCut matEvtSel(evtSelMB);
  bool doMat = true;
  if (doMat) matEvtSel = evtSelMB && "abs(joc.recoJetCalEta[0]-recoJetCalEta[0])<0.2";
  TH2D * hJEtJOCIc5pu = plotJEtCorr(mbdj,matEvtSel,"joc.recoJetCalPt[0]/recoJetCalPt[0]:recoJetCalPt[0]","hJEtJOCIc5pu",";leading E_{T}^{Ic5pu} [GeV];E_{T}^{Ic5pu Online}/E_{T}^{Ic5pu}");

  TCanvas * cJEt = new TCanvas("cJEt","cJEt",500,500);
  CPlot cpJEt("JEt","JEt","leading E_{T}^{Jet}","#");
  cpJEt.SetLogy();
  cpJEt.AddHist1D(hJEtIc5puJOC,"Ic5pu (Online)","E",kGreen+2,kOpenSquare);
  cpJEt.AddHist1D(hJEtIc5pu,"Ic5pu (Offline)","E",kBlack,kFullCircle);
  cpJEt.SetLegend(0.53,0.78,0.86,0.9);
  cpJEt.SetLegendHeader(mbheader);
  cpJEt.Draw(cJEt,false);

  TCanvas * cDEta = new TCanvas("cDEta","cDEta",500,500);
  cDEta->SetLogy();
  mbdj->Draw("joc.recoJetCalEta[0]-recoJetCalEta[0]>>hDEta",evtSelMB,"E");
  TCanvas * cDPhi = new TCanvas("cDPhi","cDPhi",500,500);
  cDPhi->SetLogy();
  mbdj->Draw("joc.recoJetCalPhi[0]-recoJetCalPhi[0]>>hDPhi",evtSelMB,"E");

  return mbdj;
}
