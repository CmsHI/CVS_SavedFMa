#include "TChain.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>

#include "Saved/Utilities/macros/cplot/CPlot.h"
#include "Saved/DiJetAna/macros/aliases_dijet.C"

using namespace std;

TH1D * plot1D(TTree * tr,TCut cut,TString var, TString name, TString title,Int_t nbins, Float_t min, Float_t max, Int_t normType=0)
{
  Float_t nSelEvt=tr->GetEntries(cut);
  cout << name << endl;
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

TH1D * plotAPDPhi(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,TCut("("+TString(cut)+")*lp[1].Rho()"),var,name,";d #phi (j1,tower1 away);Et^{Tower}",36,0,3.14,normType);
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
    TString mcfile0="dj_HydQ_DJQ80_F10GSR_djlook1120.root",
    TString mcfile1="dj_HydQ_DJUQ80_F10GSR_djlook1120.root",
    TString header="HLT_HIJet50U_Core"
    )
{
  TH1::SetDefaultSumw2();
  TChain * djdata = new TChain(module+"/djTree");
  djdata->Add(datafile);
  aliases_dijet(djdata);
  TChain * djmc0 = new TChain(module+"/djTree");
  djmc0->Add(mcfile0);
  aliases_dijet(djmc0);
  TChain * djmc1 = new TChain(module+"/djTree");
  djmc1->Add(mcfile1);
  aliases_dijet(djmc1);

  /*
  djdata->AddFriend("hltanalysis/HltTree",datafile);
  djmc0->AddFriend("hltanalysis/HltTree",mcfile0);
  djmc1->AddFriend("hltanalysis/HltTree",mcfile0);
  */

  cout << "Data Total Events: " << djdata->GetEntries() << endl;
  cout << "MC0 Total Events: " << djmc0->GetEntries() << endl;
  cout << "MC1 Total Events: " << djmc1->GetEntries() << endl;

  TCut evtSelMc("cent<10 && nljet>110 && nljet<500 && lppt[0]>10 && abs(nljeta)<2");
  TCut evtSelData = evtSelMc && "hlt[2]";
  TCut trkSel("ppt>0.7&&abs(peta)<3");
  TCut alTrkSel("lp[1].Rho()>0.7&&abs(lp[1].Eta())<3");

  TH1D * hPNDPhiData = plotDPhi(djdata,evtSelData&&trkSel,"abs(pndphi)","hPNDPhiData",3);
  TH1D * hPNDPhiMc0 = plotDPhi(djmc0,evtSelMc&&trkSel,"abs(pndphi)","hPNDPhiMc0",3);
  TH1D * hPNDPhiMc1 = plotDPhi(djmc1,evtSelMc&&trkSel,"abs(pndphi)","hPNDPhiMc1",3);

  TH1D * hLAPNDPhiData = plotAPDPhi(djdata,evtSelData&&alTrkSel,"abs(lpadphi)","hLAPNDPhiData",3);
  TH1D * hLAPNDPhiMc0 = plotAPDPhi(djmc0,evtSelMc&&alTrkSel,"abs(lpadphi)","hLAPNDPhiMc0",3);
  TH1D * hLAPNDPhiMc1 = plotAPDPhi(djmc1,evtSelMc&&alTrkSel,"abs(lpadphi)","hLAPNDPhiMc1",3);

  TCanvas * cPNDPhi = new TCanvas("cPNDPhi","cPNDPhi",500,500);
  CPlot cpPNDPhi("PNDPhi","PNDPhi","d#phi(jet1,tower)","1/N_{jet1} Et^{Tower} [GeV]");
  cpPNDPhi.SetXRange(0,3.1416);
  cpPNDPhi.SetYRange(0,300);
  cpPNDPhi.AddHist1D(hPNDPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpPNDPhi.AddHist1D(hPNDPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpPNDPhi.AddHist1D(hPNDPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpPNDPhi.SetLegendHeader("|#eta|^{Tower}<3, E_{t}^{Tower}>0.7GeV");
  cpPNDPhi.Draw(cPNDPhi,false);

  TCanvas * cLAPNDPhi = new TCanvas("cLAPNDPhi","cLAPNDPhi",500,500);
  CPlot cpLAPNDPhi("LAPNDPhi","LAPNDPhi","d#phi(jet1,tower1 away)","1/N_{jet1} Et^{Tower1} [GeV]");
  cpLAPNDPhi.SetXRange(0,3.1416);
  cpLAPNDPhi.SetYRange(0,30);
  cpLAPNDPhi.AddHist1D(hLAPNDPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpLAPNDPhi.AddHist1D(hLAPNDPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpLAPNDPhi.AddHist1D(hLAPNDPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpLAPNDPhi.SetLegendHeader("|#eta|^{Tower}<3, E_{t}^{Tower}>0.7GeV");
  cpLAPNDPhi.Draw(cLAPNDPhi,false);

  return djdata;
}
