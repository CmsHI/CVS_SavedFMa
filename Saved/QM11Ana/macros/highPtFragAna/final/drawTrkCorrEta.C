#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "../../commonUtility.h"

void drawTrkCorrEta(Int_t corrLevel=0,
    TString mod="hitrkEffAnalyzer_akpu3pf_j2",
    TString modref="hitrkEffAnalyzer_akpu3pf_j1",
    //TString mod="B2InConeJ2",
    //TString modref="B2InConeJ1",
    //TString app="_mattgmv1",
    TString app="_tev5",
    TString appref="_tev5",
    //TString app="_jtv5_cv6",
    //TString appref="_jtv5_cv6",
    Int_t isample=0, // -1 for all samples
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  Int_t mode=1; // 0 for write, 1 for read
  Int_t centBeg=0,centEnd=2;
  Float_t ptHatMin=-1;

  Corrector3D trkCorr("trkCorrHisAna_djuq",app,mod);
  //trkCorr.ptHatMin_.clear();
  //trkCorr.ptHatMin_.push_back(ptHatMin);
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.Init();

  Corrector3D trkCorrRef("trkCorrHisAna_djuq",appref,modref);
  //trkCorrRef.ptHatMin_.clear();
  //trkCorrRef.ptHatMin_.push_back(ptHatMin);
  trkCorrRef.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrRef.Init();

  TCanvas * cEffEta = new TCanvas("cEffEta","cEffEta",500,500);
  // eff
  TH1D * hCorrEtaRef = (TH1D*)trkCorrRef.InspectCorr(0,isample,centBeg,centEnd,6,15,1,7,19);
  TH1D * hCorrEta = (TH1D*)trkCorr.InspectCorr(0,isample,centBeg,centEnd,3,15,1,7,19);
  handsomeTH1(hCorrEtaRef);
  hCorrEtaRef->SetAxisRange(0,1,"Y");
  hCorrEtaRef->SetTitle(";#eta^{Trk}; Eff., Fake Rate");
  hCorrEtaRef->SetMarkerStyle(kOpenCircle);
  hCorrEtaRef->Draw("E");
  hCorrEta->Draw("Esame");

  // fake
  TH1D * hCorr1EtaRef = (TH1D*)trkCorrRef.InspectCorr(1,isample,centBeg,centEnd,6,15,1,7,19);
  TH1D * hCorr1Eta = (TH1D*)trkCorr.InspectCorr(1,isample,centBeg,centEnd,3,15,1,7,19);
  hCorr1EtaRef->SetMarkerStyle(kOpenCircle);
  hCorr1EtaRef->Draw("sameE");
  hCorr1Eta->Draw("Esame");

  TLegend *leg = new TLegend(0.3,0.77,0.60,0.93);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hCorrEtaRef,"0-30%, p_{T}^{Trk} > 4 GeV/c","");
  leg->AddEntry(hCorrEtaRef,"Leading Jet, p_{T} > 100 GeV/c","p");
  leg->AddEntry(hCorrEta,"Subleading Jet, p_{T} > 40 GeV/c","p");
  leg->Draw();

  cEffEta->Print("TrkCorr_vs_Eta_Central.gif");
  cEffEta->Print("TrkCorr_vs_Eta_Central.pdf");
}
