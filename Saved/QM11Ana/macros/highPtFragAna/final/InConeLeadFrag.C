#include <iostream>
#include "TChain.h"
#include "TCut.h"
#include "TH1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TProfile.h"
using namespace std;

void InConeLeadFrag(
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

  TH1D * hLFJ1Data = new TH1D("hLFJ1Data","",10,0,6);
  TH1D * hLFJ1Mc = new TH1D("hLFJ1Mc","",10,0,6);
  TH1D * hLFJ2Data = new TH1D("hLFJ2Data","",10,0,6);
  TH1D * hLFJ2Mc = new TH1D("hLFJ2Mc","",10,0,6);

  cout << "========== LF ana ==========" << endl;
  TCut jet1Sel = evtSel&&"clppt[0]>0";
  TCut jet2Sel = evtSel&&"clppt[1]>0";
  cout << "Cut: " << TString(jet1Sel) << endl;
  cout << infnamedata << " " << tdata->GetEntries(jet1Sel) << endl;
  cout << infnamemc << " " << tmc->GetEntries(jet1Sel) << endl;
  cout << "Cut: " << TString(jet2Sel) << endl;
  cout << infnamedata << " " << tdata->GetEntries(jet2Sel) << endl;
  cout << infnamemc << " " << tmc->GetEntries(jet2Sel) << endl;

  tdata->Project("hLFJ1Data","-log(clppt[0]/jtpt[0])",jet1Sel*"cltrkwt");
  tmc->Project("hLFJ1Mc","-log(clppt[0]/jtpt[0])",jet1Sel*"cltrkwt");
  tdata->Project("hLFJ2Data","-log(clppt[1]/jtpt[1])",jet2Sel*"cltrkwt");
  tmc->Project("hLFJ2Mc","-log(clppt[1]/jtpt[1])",jet2Sel*"cltrkwt");

  hLFJ1Mc->Scale(1./hLFJ1Mc->GetEntries());
  hLFJ2Mc->Scale(1./hLFJ2Mc->GetEntries());
  hLFJ1Data->Scale(1./hLFJ1Data->GetEntries());
  hLFJ2Data->Scale(1./hLFJ2Data->GetEntries());

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hLFJ1Mc->SetAxisRange(0,0.9,"Y");
  hLFJ1Mc->SetTitle(";#xi^{lead}=log(p_{T}^{Jet}/p_{T}^{Leading Trk});a.u.");
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

  TLegend *leg = new TLegend(0.38,0.7,0.88,0.92);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hLFJ1Data,"0-20%, "+algo,"");
  leg->AddEntry(hLFJ1Data,"Data J1","p");
  leg->AddEntry(hLFJ2Data,"Data J2","p");
  leg->AddEntry(hLFJ1Mc,"PYTHIA+HYDJET J1","l");
  leg->AddEntry(hLFJ2Mc,"PYTHIA+HYDJET J2","l");
  leg->Draw();

  c2->Print(Form("InConeLeadingFragment_%s.gif",algo.Data()));
}
