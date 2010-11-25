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

TH1D * plotTrkPt(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,cut,var,name,";Trk Pt;#",50,0,150,normType);
}

TH1D * plotJDPhi(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,cut,var,name,";d #phi (j1,j2);#/j1",50,0,3.14,normType);
}

TH1D * plotNDPhi(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,TCut("("+TString(cut)+")*ppt"),var,name,";d #phi (j1,tower);Et^{Trk}",36,0,3.14,normType);
}

TH1D * plotAPNDPhi(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,TCut("("+TString(cut)+")*lp[1].Rho()"),var,name,";d #phi (j1,tower1 away);Et^{Trk}",36,0,3.14,normType);
}

TH1D * plotNPNDPhi(TTree * tr, TCut cut, TString var, TString name, Int_t normType=0)
{
  return plot1D(tr,TCut("("+TString(cut)+")*lp[0].Rho()"),var,name,";d #phi (j1,tower1 near);Et^{Trk}",36,0,3.14,normType);
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
    TString module="djcalo",
    TString datafile="dj_HCPR-GoodTrk1123_All0.root",
    TString mcfile0="dj_HydjetQ_DJQ80_F10GSR_GoodTrk1123.root",
    TString mcfile1="dj_HydjetQ_DJUQ80_F10GSR_GoodTrk1123.root"//,
    //TString header="HLT_HIJet50U_Core"
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

  TCut evtSelMc("cent<10 && nljet>120 && nljet<500 && abs(nljeta)<2");
  TCut awayCut("aljet>50 && abs(aljeta)<2 && jdphi>TMath::Pi()*5/6");
  evtSelMc = evtSelMc && awayCut;
  TCut evtSelData = evtSelMc && "hlt[2]";
  TCut trkSel("ppt>1.2&&abs(peta)<3");
  TString header("p_{T}^{Trk}>1.2GeV");
  TCut nlTrkSel("lp[0].Rho()>0.7&&abs(lp[0].Eta())<3");
  TCut alTrkSel("lp[1].Rho()>0.7&&abs(lp[1].Eta())<3");

  TH1D * hJDPhiData = plotJDPhi(djdata,evtSelData,"jdphi","hJDPhiData",3);
  TH1D * hJDPhiMc0 = plotJDPhi(djmc0,evtSelMc,"jdphi","hJDPhiMc0",3);
  TH1D * hJDPhiMc1 = plotJDPhi(djmc1,evtSelMc,"jdphi","hJDPhiMc1",3);

  TH1D * hPPtData = plotTrkPt(djdata,evtSelData&&trkSel,"ppt","hPPtData",3);
  TH1D * hJetPPtData = plotTrkPt(djdata,evtSelData&&trkSel&&"pndr<0.5||padr<0.5","ppt","hJetPPtData",3);
  TH1D * hJet1PPtData = plotTrkPt(djdata,evtSelData&&trkSel&&"pndr<0.5","ppt","hJet1PPtData",3);
  TH1D * hJet2PPtData = plotTrkPt(djdata,evtSelData&&trkSel&&"padr<0.5","ppt","hJet2PPtData",3);
  TH1D * hNotJetPPtData = plotTrkPt(djdata,evtSelData&&trkSel&&"pndr>0.5&&padr>0.5","ppt","hNotJetPPtData",3);
  TH1D * hBkgPPtData = plotTrkPt(djdata,evtSelData&&trkSel&&"pndrbg<0.5||padrbg<0.5","ppt","hBkgPPtData",3);

  TH1D * hPNDPhiData = plotNDPhi(djdata,evtSelData&&trkSel,"abs(pndphi)","hPNDPhiData",3);
  TH1D * hPNDPhiMc0 = plotNDPhi(djmc0,evtSelMc&&trkSel,"abs(pndphi)","hPNDPhiMc0",3);
  TH1D * hPNDPhiMc1 = plotNDPhi(djmc1,evtSelMc&&trkSel,"abs(pndphi)","hPNDPhiMc1",3);

  TH1D * hPADPhiData = plotNDPhi(djdata,evtSelData&&trkSel,"abs(padphi)","hPADPhiData",3);
  TH1D * hPADPhiMc0 = plotNDPhi(djmc0,evtSelMc&&trkSel,"abs(padphi)","hPADPhiMc0",3);
  TH1D * hPADPhiMc1 = plotNDPhi(djmc1,evtSelMc&&trkSel,"abs(padphi)","hPADPhiMc1",3);

  TH1D * hLNPNDPhiData = plotNPNDPhi(djdata,evtSelData&&nlTrkSel,"abs(lpndphi)","hLNPNDPhiData",3);
  TH1D * hLNPNDPhiMc0 = plotNPNDPhi(djmc0,evtSelMc&&nlTrkSel,"abs(lpndphi)","hLNPNDPhiMc0",3);
  TH1D * hLNPNDPhiMc1 = plotNPNDPhi(djmc1,evtSelMc&&nlTrkSel,"abs(lpndphi)","hLNPNDPhiMc1",3);

  TH1D * hLAPNDPhiData = plotAPNDPhi(djdata,evtSelData&&alTrkSel,"abs(lpadphi)","hLAPNDPhiData",3);
  TH1D * hLAPNDPhiMc0 = plotAPNDPhi(djmc0,evtSelMc&&alTrkSel,"abs(lpadphi)","hLAPNDPhiMc0",3);
  TH1D * hLAPNDPhiMc1 = plotAPNDPhi(djmc1,evtSelMc&&alTrkSel,"abs(lpadphi)","hLAPNDPhiMc1",3);

  TCanvas * cJDPhi = new TCanvas("cJDPhi","cJDPhi",500,500);
  CPlot cpJDPhi("JDPhi","JDPhi","d#phi(j1,j2)","# /DJ");
  cpJDPhi.AddHist1D(hJDPhiData,"Data","E",kBlack,kFullCircle);
  cpJDPhi.AddHist1D(hJDPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpJDPhi.AddHist1D(hJDPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpJDPhi.Draw(cJDPhi,false);

  TCanvas * cPPt = new TCanvas("cPPt","cPPt",500,500);
  CPlot.SetLogy();
  CPlot cpPPt("PPt","PPt","Trk Pt [GeV]","# /N_{DJ}");
  cpPPt.AddHist1D(hPPtData,"Data: All Tracks","E",kBlack,kFullCircle);
  cpPPt.AddHist1D(hJetPPtData,"Data: Trks in Jet1,Jet2","E",kRed,kOpenSquare);
  cpPPt.AddHist1D(hJet1PPtData,"Data: Trks in Jet1","E",kBlue,kOpenCircle);
  cpPPt.AddHist1D(hJet2PPtData,"Data: Trks in Jet2","E",kMagenta,kOpenCircle);
  cpPPt.AddHist1D(hNotJetPPtData,"Data: Trks Outside Jet1,Jet2","E",kGray+2,kOpenStar);
  cpPPt.AddHist1D(hBkgPPtData,"Data: Trks in Rotated Cone","E",kGreen+2,kOpenCircle);
  cpPPt.SetLegendHeader(header);
  cpPPt.Draw(cPPt,false);

  TCanvas * cPNDPhi = new TCanvas("cPNDPhi","cPNDPhi",500,500);
  CPlot cpPNDPhi("PNDPhi","PNDPhi","d#phi(jet1,tower)","1/N_{DJ} Pt^{Trk} [GeV]");
  cpPNDPhi.SetXRange(0,3.1416);
  cpPNDPhi.SetYRange(0.01,80);
  cpPNDPhi.AddHist1D(hPNDPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpPNDPhi.AddHist1D(hPNDPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpPNDPhi.AddHist1D(hPNDPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpPNDPhi.SetLegendHeader(header);
  cpPNDPhi.Draw(cPNDPhi,false);

  TCanvas * cPADPhi = new TCanvas("cPADPhi","cPADPhi",500,500);
  CPlot cpPADPhi("PADPhi","PADPhi","d#phi(jet1,tower)","1/N_{DJ} Pt^{Trk} [GeV]");
  cpPADPhi.SetXRange(0,3.1416);
  cpPADPhi.SetYRange(0.01,80);
  cpPADPhi.AddHist1D(hPADPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpPADPhi.AddHist1D(hPADPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpPADPhi.AddHist1D(hPADPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpPADPhi.SetLegendHeader(header);
  cpPADPhi.Draw(cPADPhi,false);

  TCanvas * cLNPNDPhi = new TCanvas("cLNPNDPhi","cLNPNDPhi",500,500);
  CPlot cpLNPNDPhi("LNPNDPhi","LNPNDPhi","d#phi(jet1,tower1 near)","1/N_{DJ} Pt^{Trk} [GeV]");
  cpLNPNDPhi.SetXRange(0,3.1416);
  cpLNPNDPhi.SetYRange(0.01,100);
  cpLNPNDPhi.SetLogy();
  cpLNPNDPhi.AddHist1D(hLNPNDPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpLNPNDPhi.AddHist1D(hLNPNDPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpLNPNDPhi.AddHist1D(hLNPNDPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpLNPNDPhi.SetLegendHeader(header);
  cpLNPNDPhi.Draw(cLNPNDPhi,false);

  TCanvas * cLAPNDPhi = new TCanvas("cLAPNDPhi","cLAPNDPhi",500,500);
  CPlot cpLAPNDPhi("LAPNDPhi","LAPNDPhi","d#phi(jet1,tower1 away)","1/N_{DJ} Pt^{Trk} [GeV]");
  cpLAPNDPhi.SetXRange(0,3.1416);
  cpLAPNDPhi.SetYRange(0.01,100);
  cpLAPNDPhi.SetLogy();
  cpLAPNDPhi.AddHist1D(hLAPNDPhiData,"Data (HLT_HIJet50U)","E",kBlack,kFullCircle);
  cpLAPNDPhi.AddHist1D(hLAPNDPhiMc0,"Hydjet+DJQuen80","E",kRed,kOpenCircle);
  cpLAPNDPhi.AddHist1D(hLAPNDPhiMc1,"Hydjet+DJUnQuen80","E",kBlue,kOpenSquare);
  cpLAPNDPhi.SetLegendHeader(header);
  cpLAPNDPhi.Draw(cLAPNDPhi,false);

  return djdata;
}
