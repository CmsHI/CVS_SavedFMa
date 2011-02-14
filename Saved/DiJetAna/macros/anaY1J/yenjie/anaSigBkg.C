#include <iostream>
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLegend.h"
using namespace std;

TChain * anaSigBkg(TString input="mix30.root",
    TCut mycut="pthat>30")
{
  //
  // Inputs
  //
  TChain * t = new TChain("inclusiveJetAnalyzer/t");
  t->Add(input);
  t->AddFriend("bkg=BkgJetAnalyzer/t",input);
  t->SetAlias("bkgdetajt","abs(jteta-bkg.jteta)");
  t->SetAlias("bkgdphijt","abs(acos(cos(jtphi-bkg.jtphi)))");
  t->SetAlias("bkgdrjt","sqrt(pow(bkgdetajt,2)+pow(bkgdphijt,2))");
  t->SetAlias("bkgdetajt0","abs(jteta[0]-bkg.jteta[0])");
  t->SetAlias("bkgdphijt0","abs(acos(cos(jtphi[0]-bkg.jtphi[0])))");
  t->SetAlias("bkgdrjt0","sqrt(pow(bkgdetajt0,2)+pow(bkgdphijt0,2))");
  t->SetAlias("jdphi0","abs(acos(cos(jtphi[1]-jtphi[0])))");
  t->SetAlias("Aj0","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])");
  TChain * tbkg = new TChain("BkgJetAnalyzer/t");
  tbkg->Add(input);

  //
  // Event Selection
  //
  TCut evtSel("jtpt[0]>100&&abs(jteta[0])<2");
  TCut sigSel("refdrjt[0]<0.15");
  TCut bkgSel("bkgdrjt0<0.1");

  //
  // Event Classes
  Float_t numTot = t->GetEntries(evtSel);
  Float_t numMatSig = t->GetEntries(evtSel&&sigSel&&!bkgSel);
  Float_t numMatBkg = t->GetEntries(evtSel&&bkgSel&&!sigSel);
  Float_t numMatSigOrBkg = t->GetEntries(evtSel&&(sigSel||bkgSel));
  Float_t numMatSigAndBkg = t->GetEntries(evtSel&&(sigSel&&bkgSel));
  Float_t numMatNone = t->GetEntries(evtSel&&!(sigSel||bkgSel));
  cout << "Event Selection: " << TString(evtSel) << endl;
  cout << "  Sig Selection: " << TString(sigSel) << endl;
  cout << "  Bkg Selection: " << TString(bkgSel) << endl;
  cout << "Total Sel events: " << numTot << endl;
  cout << "Fraction matched sig: " << numMatSig/numTot << endl;
  cout << "Fraction matched bkg: " << numMatBkg/numTot << endl;
  cout << "Fraction matched sig&&bkg: " << numMatSigAndBkg/numTot << endl;
  cout << "Fraction matched none: " << numMatNone/numTot << endl;

  evtSel = "jtpt[0]>120&&abs(jteta[0])<2&&jtpt[1]>0&&abs(jteta[1])<2&&acos(cos(jtphi[1]-jtphi[0]))>TMath::Pi()*2./3";
  TH1::SetDefaultSumw2();
  TH1D * hDataMix30= new TH1D("hDataMix30","",20,0,1);
  TH1D * hDataMix50= new TH1D("hDataMix50","",20,0,1);
  TH1D * hDataMix80= new TH1D("hDataMix80","",20,0,1);
  TH1D * hBkg = new TH1D("hBkg","",25,0,1.5);
  //t->Project("hDataMix30","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&!(bkgSel&&!sigSel)&&"pthat>30");
  //t->Project("hDataMix50","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&!(bkgSel&&!sigSel)&&"pthat>50");
  //t->Project("hDataMix80","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&!(bkgSel&&!sigSel)&&"pthat>80");
  t->Project("hDataMix30","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&!(bkgSel)&&"pthat>30");
  t->Project("hDataMix50","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&!(bkgSel)&&"pthat>50");
  t->Project("hDataMix80","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&!(bkgSel)&&"pthat>80");
  //t->Project("hDataMix30","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&"pthat>30");
  //t->Project("hDataMix50","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&"pthat>50");
  //t->Project("hDataMix80","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel&&"pthat>80");
  tbkg->Project("hBkg","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",evtSel);
  hDataMix30->Scale(1./hDataMix30->GetEntries());
  hDataMix50->Scale(1./hDataMix50->GetEntries());
  hDataMix80->Scale(1./hDataMix80->GetEntries());
  hBkg->Scale(1./hBkg->GetEntries());

  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  hDataMix30->SetTitle(";A_{J};Event Fraction;");
  hDataMix30->SetAxisRange(0,1,"X");
  hDataMix30->SetAxisRange(0,0.26,"Y");
  hDataMix50->SetLineColor(2);
  hDataMix80->SetLineColor(4);
  hBkg->SetMarkerStyle(kOpenCircle);
  hDataMix30->Draw("E");
  hDataMix50->Draw("same hist");
  hDataMix80->Draw("same hist");
  hBkg->Draw("sameE");

  TLegend * l = new TLegend(0.54,0.72,0.83,0.88);
  l->SetBorderSize(0);
  l->SetFillStyle(0);
  l->SetTextSize(0.035);
  l->AddEntry(hDataMix30,"Min #hat{p}_{T} > 30GeV","pl");
  l->AddEntry(hDataMix50,"Min #hat{p}_{T} > 50GeV","l");
  l->AddEntry(hDataMix80,"Min #hat{p}_{T} > 80GeV","l");
  l->AddEntry(hBkg,"RECO Bkg","pl");
  l->Draw();

  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  evtSel = "jtpt[0]>100&&abs(jteta[0])<2";
  c3->SetLogz();
  c3->SetRightMargin(0.18);
  TH2D * hBkgPtPt = new TH2D("hBkgPtPt",";Leading Jet p_{T} (GeV);(Leading Bkg Jet p_{T})/(Leading Jet p_{T});",20,100,220,40,0,1.3);
  t->Draw("bkg.jtpt[0]/jtpt[0]:jtpt[0]>>hBkgPtPt",evtSel&&bkgSel,"colz");
  t->Draw("bkg.jtpt[0]/jtpt[0]:jtpt[0]",evtSel&&bkgSel&&sigSel,"box same");

  double sum30=0;
  double sum50=0;
  double sum80=0;
  double err30=0;
  double err50=0;
  double err80=0;

  for (int i=1;i<=3;i++)
  {
      sum30+=hDataMix30->GetBinContent(i);
      sum50+=hDataMix50->GetBinContent(i);
      sum80+=hDataMix80->GetBinContent(i);
      err30+=hDataMix30->GetBinError(i)*hDataMix30->GetBinError(i);
      err50+=hDataMix50->GetBinError(i)*hDataMix50->GetBinError(i);
      err80+=hDataMix80->GetBinError(i)*hDataMix80->GetBinError(i);
  }

  cout <<"pt30: "<<sum30<<" +- "<<sqrt(err30)<<endl;
  cout <<"pt50: "<<sum50<<" +- "<<sqrt(err50)<<endl;
  cout <<"pt80: "<<sum80<<" +- "<<sqrt(err80)<<endl;

  return t;
}
