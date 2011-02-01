#include <iostream>
#include <vector>
#include <TH1D.h>
#include "TChain.h"
#include <TTree.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>
#include <TProfile.h>
#include <TText.h>
#include "commonUtility.h"
#include "DrawTick.C"
#include "colorScheme.h"
using namespace std;

void RandomConePull(TString infname = "datambv3mix1_djcalo_c0to30",
                    TCut myCut = "cent<30", char *title = "",bool drawLegend = false,
                    bool drawSys = true
		    )
{
  // ===========================================================
  // Get Input
  // ===========================================================
  vector<TChain*> t;
  vector<TH1D*> vhMetx;
  vector<TH1D*> vhJPhi;
  for (Int_t i=0; i<=20; ++i) {
    t.push_back(new TChain("ntjt"));
    TString infile(Form("../ntv5_%s_offset%d.root",infname.Data(),i*10));
    t[i]->Add(infile);
    cout << infile << ": " << t[i]->GetEntries() << endl;
    vhMetx.push_back(new TH1D(Form("hMetx_%d",i),";#slash{p}_{T}^{#parallel} (GeV/c);",100,-300,300));
    vhJPhi.push_back(new TH1D(Form("hJPhi_%d",i),";#phi_{1};",20,-3.14,3.14));
  }


  // ===========================================================
  // Analysis Setup
  // ===========================================================
  const int nBin = 5;
  double bins[nBin+1] = {0.5,1.0,2,4,8,1000};  

  const int nBinAj = 4;
  double ajBins[nBinAj+1] = {0.0001,0.11,0.22,0.33,0.49999};
  // Selection cut
  TCut evtCut = "nljet>120&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&jdphi>2./3*TMath::Pi()&&!maskEvt";

  TH1D * hMean = new TH1D("hMean",";<#slash{p}_{T}^{#parallel}>;sub experiment",40,-10,10);
  for (Int_t i=0; i<t.size(); ++i) {
    cout << "Sel evt: " << t[i]->GetEntries(evtCut&&myCut) << endl;
    t[i]->Project(vhMetx[i]->GetName(),"-metx",evtCut*"weight");
    t[i]->Project(vhJPhi[i]->GetName(),"nljphi",evtCut*"weight");
    //vhMetx[i]->Fit("gaus");
    Float_t mean = vhMetx[i]->GetMean();
    cout << "metx mean: " << mean << endl;
    hMean->Fill(mean);
  }

  TCanvas * c2 = new TCanvas("c2","c2",600,600);
  vhMetx[0]->SetAxisRange(0,500,"Y");
  vhMetx[0]->Draw("");
  for (Int_t i=1; i<t.size(); ++i) {
    vhMetx[i]->Draw("same");
  }

  TCanvas * c3 = new TCanvas("c3","c3",600,600);
  vhJPhi[0]->Draw("");
  vhJPhi[0]->SetAxisRange(0,500,"Y");
  for (Int_t i=1; i<t.size(); ++i) {
    vhJPhi[i]->Draw("same");
  }

  TCanvas * c13 = new TCanvas("c13","c13",600,600);
  hMean->Fit("gaus");
  hMean->Draw();
}

