#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "../../commonUtility.h"

void drawTrkCorrJetPt(Int_t corrLevel=0,
    TString mod="hitrkEffAnalyzer_akpu3pf_j2",
    TString modref="hitrkEffAnalyzer_akpu3pf_j1",
    //TString mod="B2InConeJ2",
    //TString modref="B2InConeJ1",
    //TString app="_mattgmv1",
    TString app="_tev6",
    TString appref="_tev6",
    //TString app="_tev7genjet",
    //TString appref="_tev7genjet",
    //TString app="_ppv1",
    //TString appref="_ppv1",
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
  trkCorr.ptHatMin_.clear();
  trkCorr.ptHatMin_.push_back(30);
  trkCorr.ptHatMin_.push_back(50);
  trkCorr.ptHatMin_.push_back(80);
  trkCorr.ptHatMin_.push_back(110);
  trkCorr.ptHatMin_.push_back(170);
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.Init();

  Corrector3D trkCorrRef("trkCorrHisAna_djuq",appref,modref);
  trkCorrRef.ptHatMin_.clear();
  trkCorrRef.ptHatMin_.push_back(30);
  trkCorrRef.ptHatMin_.push_back(50);
  trkCorrRef.ptHatMin_.push_back(80);
  trkCorrRef.ptHatMin_.push_back(110);
  trkCorrRef.ptHatMin_.push_back(170);
  trkCorrRef.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrRef.Init();

  cout << endl << "========= plot =========" << endl;
  Float_t ptMin=4;
  Int_t ptBegBin = trkCorr.ptBin_->FindBin(ptMin);
  Int_t ptEndBin = trkCorr.ptBin_->FindBin(200);
  cout << "pt beg bin: " << ptBegBin << " end bin: " << ptEndBin << endl;
  cout << "========================" << endl;

  TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
  // eff
  TH1D * hCorrPtRefJet100 = (TH1D*) trkCorrRef.InspectCorr(0,isample,centBeg,centEnd,ptBegBin,ptEndBin,3,7-etaPM,7+etaPM);
  TH1D * hCorrPtJet40 = (TH1D*) trkCorr.InspectCorr(0,isample,centBeg,centEnd,ptBegBin,ptEndBin,3,7-etaPM,7+etaPM);
  handsomeTH1(hCorrPtJet40);
  hCorrPtJet40->SetAxisRange(40.1,499.9,"X");
  hCorrPtJet40->SetAxisRange(0,1,"Y");
  hCorrPtJet40->SetTitle(";Jet p_{T} (GeV/c); Eff., Fake Rate");
  hCorrPtJet40->Draw("E");
  hCorrPtRefJet100->SetMarkerStyle(kOpenCircle);
  hCorrPtRefJet100->SetAxisRange(100.1,499.9,"X");
  hCorrPtRefJet100->Draw("sameE");

  // fake
  TH1D * hCorr1PtRefJet100 = (TH1D*) trkCorrRef.InspectCorr(1,isample,centBeg,centEnd,ptBegBin,ptEndBin,3,7-etaPM,7+etaPM);
  TH1D * hCorr1PtJet40 = (TH1D*) trkCorr.InspectCorr(1,isample,centBeg,centEnd,ptBegBin,ptEndBin,3,7-etaPM,7+etaPM);
  hCorr1PtRefJet100->SetMarkerStyle(kOpenCircle);
  hCorr1PtRefJet100->SetAxisRange(100.1,499.9,"X");
  hCorr1PtJet40->Draw("sameE");
  hCorr1PtRefJet100->Draw("sameE");

  TLegend *leg = new TLegend(0.31,0.25,0.61,0.50);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hCorrPtRefJet100,"PYTHIA+HYDJET 0-30%","");
  leg->AddEntry(hCorrPtRefJet100,"#hat{p}_{T} 30,50,80,110,170 GeV/c","");
  //leg->AddEntry(hCorrPtRefJet100,"#hat{p}_{T} 80 GeV/c","");
  leg->AddEntry(hCorrPtRefJet100,Form("|#eta^{Trk}|<1, p_{T}^{Trk} > %.0f",ptMin),"");
  //leg->AddEntry(hCorrPtRefJet100,"Leading GenJet","p");
  //leg->AddEntry(hCorrPtJet40,"Subleading GenJet","p");
  leg->AddEntry(hCorrPtRefJet100,"Leading RecJet","p");
  leg->AddEntry(hCorrPtJet40,"Subleading RecJet","p");
  leg->Draw();

  cEff->Print("TrkCorr_vs_JetPt_Central"+app+".gif");
  cEff->Print("TrkCorr_vs_JetPt_Central"+app+".pdf");
}
