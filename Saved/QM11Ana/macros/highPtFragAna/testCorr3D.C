#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"

void testCorr3D(Int_t corrLevel=0,
    TString mod="B2InConeGluon",
    TString modref="B2InConeQuark",
    TString app="_jtv3_2_cv4qg",
    TString appref="_jtv3_2_cv4qg",
    Int_t isample=0, // -1 for all samples
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  Int_t mode=1; // 0 for write, 1 for read
  Int_t cbin=0;
  Float_t ptHatMin=110;

  Corrector3D trkCorr("trkCorrHisAna_djuq",app,mod);
  trkCorr.ptHatMin_.clear();
  trkCorr.ptHatMin_.push_back(ptHatMin);
  trkCorr.sampleMode_ = 0; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.Init();
  Double_t corr[4];
  cout << trkCorr.GetCorr(10,0,110,0,corr) << endl;

  Corrector3D trkCorrRef("trkCorrHisAna_djuq",appref,modref);
  trkCorrRef.ptHatMin_.clear();
  trkCorrRef.ptHatMin_.push_back(ptHatMin);
  trkCorrRef.sampleMode_ = 0; // 0 for choosing individual sample, 1 for merge samples
  trkCorrRef.Init();

  /*
  // Plot 2D Corr.
  gStyle->SetPadRightMargin(0.15);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH2D *hCorr2D = (TH2D*)trkCorr.InspectCorr(corrLevel,isample,cbin,4,30,0);
  hCorr2D->Draw("colz");
  */

  TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
  TH1D * hCorrPtRef = (TH1D*) trkCorrRef.InspectCorr(corrLevel,isample,cbin,3,15,2,7-etaPM,7+etaPM);
  hCorrPtRef->SetAxisRange(0,120,"X");
  hCorrPtRef->SetMarkerStyle(0);
  hCorrPtRef->SetMarkerColor(kBlack);
  hCorrPtRef->SetLineColor(kBlack);
  hCorrPtRef->Draw("histE");
  TH1D * hCorrPt = (TH1D*) trkCorr.InspectCorr(corrLevel,isample,cbin,3,15,2,7-etaPM,7+etaPM);
  hCorrPt->SetMarkerStyle(0);
  hCorrPt->SetMarkerColor(kRed);
  hCorrPt->SetLineColor(kRed);
  hCorrPt->Draw("hist same E");

  /*
  TCanvas * cEffEta = new TCanvas("cEffEta","cEffEta",500,500);
  TH1D * hCorrEta = (TH1D*)trkCorr.InspectCorr(corrLevel,isample,cbin,4,30,1);
  hCorrEta->Draw("histE");
  */
}
