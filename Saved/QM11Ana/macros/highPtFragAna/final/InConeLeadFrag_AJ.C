#include <iostream>
#include "TChain.h"
#include "TCut.h"
#include "TH1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TProfile.h"
using namespace std;

void InConeLeadFrag_AJ(
    //TString algo="akpu3pf"
    TString algo="icpu5"
    )
{
  TH1::SetDefaultSumw2();
  TString infnamedata = Form("histntff_tv1data_%s_cv3.root",algo.Data());
  TString infnamemc = Form("histntff_tv1mc80_%s_cv3.root",algo.Data());
  TCut evtSel;
  if (algo=="akpu3pf")
    evtSel = "cent<20&&jtpt[0]>90&&jtpt[1]>40&&abs(jteta[0])<1.6&&abs(jteta[1])<1.6&&abs(jdphi)>2.09";
  if (algo=="icpu5")
    evtSel = "cent<20&&jtpt[0]>120&&jtpt[1]>50&&abs(jteta[0])<1.6&&abs(jteta[1])<1.6&&abs(jdphi)>2.09";

  TChain * tdata = new TChain("tjfrRec");
  tdata->Add(infnamedata);
  TChain * tmc = new TChain("tjfrRec");
  tmc->Add(infnamemc);

  cout << "Cut: " << TString(evtSel) << endl;
  cout << infnamedata << " " << tdata->GetEntries(evtSel) << endl;
  cout << infnamemc << " " << tmc->GetEntries(evtSel) << endl;

  TProfile * hLFJ1Data = new TProfile("hLFJ1Data","",5,0,0.5);
  TProfile * hLFJ1Mc = new TProfile("hLFJ1Mc","",5,0,0.5);
  TProfile * hLFJ2Data = new TProfile("hLFJ2Data","",5,0,0.5);
  TProfile * hLFJ2Mc = new TProfile("hLFJ2Mc","",5,0,0.5);

  cout << "========== LF ana ==========" << endl;
  TCut jet1Sel = evtSel&&"clppt[0]>0";
  TCut jet2Sel = evtSel&&"clppt[1]>0";
  cout << "Cut: " << TString(jet1Sel) << endl;
  cout << infnamedata << " " << tdata->GetEntries(jet1Sel) << endl;
  cout << infnamemc << " " << tmc->GetEntries(jet1Sel) << endl;
  cout << "Cut: " << TString(jet2Sel) << endl;
  cout << infnamedata << " " << tdata->GetEntries(jet2Sel) << endl;
  cout << infnamemc << " " << tmc->GetEntries(jet2Sel) << endl;

  tdata->Project("hLFJ1Data","clppt[0]/jtpt[0]:Aj",jet1Sel*"cltrkwt","prof");
  tmc->Project("hLFJ1Mc","clppt[0]/jtpt[0]:Aj",jet1Sel*"cltrkwt","prof");
  tdata->Project("hLFJ2Data","clppt[1]/jtpt[1]:Aj",jet2Sel*"cltrkwt","prof");
  tmc->Project("hLFJ2Mc","clppt[1]/jtpt[1]:Aj",jet2Sel*"cltrkwt","prof");

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hLFJ1Mc->SetAxisRange(0,0.3,"Y");
  hLFJ1Mc->SetTitle(";A_{J};<z^{lead}>=<p_{T}^{Leading Trk}/p_{T}^{Jet}>;");
  hLFJ1Mc->GetXaxis()->CenterTitle();
  hLFJ1Mc->GetYaxis()->CenterTitle();
  hLFJ1Mc->SetLineColor(kRed);
  hLFJ1Mc->SetLineStyle(2);

  hLFJ2Mc->SetLineColor(kRed);

  hLFJ1Data->SetMarkerStyle(kOpenCircle);

  hLFJ1Mc->Draw("hist");
  hLFJ2Mc->Draw("samehist");
  hLFJ1Data->Draw("sameE");
  hLFJ2Data->Draw("sameE");

  TLegend *leg = new TLegend(0.18,0.18,0.68,0.38);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hLFJ1Data,"0-20%, "+algo,"");
  leg->AddEntry(hLFJ1Data,"Data J1","p");
  leg->AddEntry(hLFJ2Data,"Data J2","p");
  leg->AddEntry(hLFJ1Mc,"PYTHIA+HYDJET J1","l");
  leg->AddEntry(hLFJ2Mc,"PYTHIA+HYDJET J2","l");
  leg->Draw();

  c2->Print(Form("InConeLeadingFragment_vs_AJ_%s.gif",algo.Data()));
}
