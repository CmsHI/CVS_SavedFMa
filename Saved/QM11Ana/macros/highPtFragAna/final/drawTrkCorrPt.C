#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "../../commonUtility.h"

void drawTrkCorrPt(Int_t corrLevel=0,
    //TString mod="hitrkEffAnalyzer",
    //TString modref="hitrkEffAnalyzer",
    TString mod="hitrkEffAnalyzer_akpu3pf_j2",
    TString modref="hitrkEffAnalyzer_akpu3pf_j1",
    //TString mod="hiHighPtTrkEffAnalyzer_akpu3pf_j2",
    //TString modref="hiHighPtTrkEffAnalyzer_akpu3pf_j1",
    //TString mod="B2InConeJ2",
    //TString modref="B2InConeJ1",
    //TString app="_mattgmv1",
    //TString app="_tev7hpt",
    //TString appref="_tev7hpt",
    TString app="_tev7genjet",
    TString appref="_tev7genjet",
    //TString app="_ppv1",
    //TString appref="_ppv1",
    //TString app="_pphptv1",
    //TString appref="_pphptv1",
    //TString app = "_LargeSet1_1k_rereco",
    //TString appref = "_LargeSet1_1k",
    //TString app="_jtv5_cv6",
    //TString appref="_jtv5_cv6",
    Int_t isample=0, // -1 for all samples
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  Int_t mode=1; // 0 for write, 1 for read
  Int_t centBeg=0, centEnd=2; // HI: 0-30%
  //Int_t centBeg=0, centEnd=4; // pp: all cent

  Corrector3D trkCorr("trkCorrHisAna_djuq",app,mod);
  //Corrector3D trkCorr("trkhist_apr262011_hydjetBass_dijet",app,mod);
  trkCorr.ptHatMin_.clear();
  //trkCorr.ptHatMin_.push_back(50);
  trkCorr.ptHatMin_.push_back(80);
  //trkCorr.ptHatMin_.push_back(110);
  //trkCorr.ptHatMin_.push_back(170);
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.Init();

  Corrector3D trkCorrRef("trkCorrHisAna_djuq",appref,modref);
  //Corrector3D trkCorrRef("trkhist_apr262011_hydjetBass_dijet",appref,modref);
  trkCorrRef.ptHatMin_.clear();
  //trkCorrRef.ptHatMin_.push_back(50);
  trkCorrRef.ptHatMin_.push_back(80);
  //trkCorrRef.ptHatMin_.push_back(110);
  //trkCorrRef.ptHatMin_.push_back(170);
  trkCorrRef.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrRef.Init();

  cout << "========= plot =========" << endl;
  Float_t jet1PtMin=100;
  Float_t jet2PtMin=40;
  Int_t jet1BegBin = trkCorr.jetBin_->FindBin(jet1PtMin);
  Int_t jet2BegBin = trkCorr.jetBin_->FindBin(jet2PtMin);
  Int_t jetEndBin = trkCorr.jetBin_->FindBin(300);
  cout << "jet pt 100 bin: " << jet1BegBin << " 40 bin: " << jet2BegBin << endl;
  cout << "jet pt end bin: " << jetEndBin << endl;
  TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
  // eff
  TH1D * hCorrPtRefJet100 = (TH1D*) trkCorrRef.InspectCorr(0,isample,centBeg,centEnd,jet1BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  handsomeTH1(hCorrPtRefJet100);
  hCorrPtRefJet100->SetAxisRange(0,59.9,"X");
  hCorrPtRefJet100->SetAxisRange(0,1,"Y");
  hCorrPtRefJet100->SetTitle(";p_{T}^{Trk} (GeV/c); Eff., Fake Rate");
  hCorrPtRefJet100->SetMarkerStyle(kOpenCircle);
  hCorrPtRefJet100->Draw("E");
  TH1D * hCorrPtJet40 = (TH1D*) trkCorr.InspectCorr(0,isample,centBeg,centEnd,jet2BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  hCorrPtJet40->Draw("same E");

  // fake
  TH1D * hCorr1PtRefJet100 = (TH1D*) trkCorrRef.InspectCorr(1,isample,centBeg,centEnd,jet1BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  hCorr1PtRefJet100->SetMarkerStyle(kOpenCircle);
  hCorr1PtRefJet100->Draw("sameE");
  TH1D * hCorr1PtJet40 = (TH1D*) trkCorr.InspectCorr(1,isample,centBeg,centEnd,jet2BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  hCorr1PtJet40->Draw("sameE");

  TLegend *leg = new TLegend(0.3,0.77,0.60,0.93);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hCorrPtRefJet100,"0-30%, |#eta^{Trk}|<1","");
  //leg->AddEntry(hCorrPtRefJet100,"pp, |#eta^{Trk}|<1","");
  leg->AddEntry(hCorrPtRefJet100,Form("Leading Jet, p_{T} > %.0f GeV/c",jet1PtMin),"p");
  leg->AddEntry(hCorrPtJet40,Form("Subleading Jet, p_{T} > %.0f GeV/c",jet2PtMin),"p");
  leg->Draw();

  cEff->Print("TrkCorr_vs_Pt_Central.gif");
  cEff->Print("TrkCorr_vs_Pt_Central.pdf");
}
