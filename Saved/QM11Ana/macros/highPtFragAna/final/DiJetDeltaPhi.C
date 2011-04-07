#include <iostream>
#include "TChain.h"
#include "TCut.h"
#include "TH1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
using namespace std;

void DiJetDeltaPhi(
    //TString algo="akpu3pf"
    TString algo="icpu5"
    )
{
  TH1::SetDefaultSumw2();
  TString infnamedata = Form("histntff_tv1data_%s_cv3.root",algo.Data());
  TString infnamemc = Form("histntff_tv1mc80_%s_cv3.root",algo.Data());
  TCut evtSel;
  if (algo=="akpu3pf")
    evtSel = "cent<20&&jtpt[0]>90&&jtpt[1]>40&&abs(jteta[0])<1.6&&abs(jteta[1])<1.6";
  if (algo=="icpu5")
    evtSel = "cent<20&&jtpt[0]>120&&jtpt[1]>50&&abs(jteta[0])<1.6&&abs(jteta[1])<1.6";

  TChain * tdata = new TChain("tjfrRec");
  tdata->Add(infnamedata);
  TChain * tmc = new TChain("tjfrRec");
  tmc->Add(infnamemc);

  cout << "Cut: " << TString(evtSel) << endl;
  cout << infnamedata << " " << tdata->GetEntries(evtSel) << endl;
  cout << infnamemc << " " << tmc->GetEntries(evtSel) << endl;

  TH1D * hDPhiData = new TH1D("hDPhiData","",20,0,3.14);
  TH1D * hDPhiMc = new TH1D("hDPhiMc","",20,0,3.14);

  tdata->Project("hDPhiData","abs(jdphi)",evtSel);
  tmc->Project("hDPhiMc","abs(jdphi)",evtSel);

  hDPhiData->Scale(1./hDPhiData->GetEntries());
  hDPhiMc->Scale(1./hDPhiMc->GetEntries());

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetLogy();
  hDPhiMc->SetLineColor(kRed);
  hDPhiMc->SetTitle(";#Delta#phi_{1,2};a.u.");
  hDPhiMc->GetXaxis()->CenterTitle();
  hDPhiMc->GetYaxis()->CenterTitle();
  hDPhiMc->Draw("hist");
  hDPhiData->Draw("sameE");

  TLegend *leg = new TLegend(0.2,0.78,0.7,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hDPhiData,"0-20%","");
  leg->AddEntry(hDPhiData,"Data, "+algo,"p");
  leg->AddEntry(hDPhiMc,"PYTHIA+HYDJET, "+algo,"l");
  leg->Draw();

  c2->Print(Form("DeltaPhi_%s.gif",algo.Data()));
}
