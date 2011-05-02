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
    TString app="_tev6",
    TString appref="_tev6",
    //TString app="_jtv5_cv6",
    //TString appref="_jtv5_cv6",
    //TString app="_tev7genjet",
    //TString appref="_tev7genjet",
    Int_t isample=0, // -1 for all samples
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  Int_t mode=1; // 0 for write, 1 for read
  Int_t centBeg=0,centEnd=2;

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
  Float_t jet1PtMin=100;
  Float_t jet2PtMin=40;
  Int_t jet1BegBin = trkCorr.jetBin_->FindBin(jet1PtMin);
  Int_t jet2BegBin = trkCorr.jetBin_->FindBin(jet2PtMin);
  Int_t jetEndBin = trkCorr.jetBin_->FindBin(300);
  cout << "jet pt 100 bin: " << jet1BegBin << " 40 bin: " << jet2BegBin << endl;
  cout << "jet pt end bin: " << jetEndBin << endl;
  Float_t ptMin=4;
  Int_t ptBegBin = trkCorr.ptBin_->FindBin(ptMin);
  Int_t ptEndBin = trkCorr.ptBin_->FindBin(200);
  cout << "pt beg bin: " << ptBegBin << " end bin: " << ptEndBin << endl;
  cout << "========================" << endl;

  TCanvas * cEffEta = new TCanvas("cEffEta","cEffEta",500,500);
  // eff
  TH1D * hCorrEtaRef = (TH1D*)trkCorrRef.InspectCorr(0,isample,centBeg,centEnd,jet1BegBin,jetEndBin,1,ptBegBin,ptEndBin);
  TH1D * hCorrEta = (TH1D*)trkCorr.InspectCorr(0,isample,centBeg,centEnd,jet2BegBin,jetEndBin,1,ptBegBin,ptEndBin);
  handsomeTH1(hCorrEtaRef);
  hCorrEtaRef->SetAxisRange(0,1,"Y");
  hCorrEtaRef->SetTitle(";#eta^{Trk}; Eff., Fake Rate");
  hCorrEtaRef->SetMarkerStyle(kOpenCircle);
  hCorrEtaRef->Draw("E");
  hCorrEta->Draw("Esame");

  // fake
  TH1D * hCorr1EtaRef = (TH1D*)trkCorrRef.InspectCorr(1,isample,centBeg,centEnd,jet1BegBin,jetEndBin,1,ptBegBin,ptEndBin);
  TH1D * hCorr1Eta = (TH1D*)trkCorr.InspectCorr(1,isample,centBeg,centEnd,jet2BegBin,jetEndBin,1,ptBegBin,ptEndBin);
  hCorr1EtaRef->SetMarkerStyle(kOpenCircle);
  hCorr1EtaRef->Draw("sameE");
  hCorr1Eta->Draw("Esame");

  TLegend *leg = new TLegend(0.31,0.25,0.61,0.50);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hCorrEta,"PYTHIA+HYDJET 0-30%","");
  leg->AddEntry(hCorrEta,"#hat{p}_{T} 30,50,80,110,170 GeV/c","");
  //leg->AddEntry(hCorrEta,"#hat{p}_{T} 80 GeV/c","");
  leg->AddEntry(hCorrEtaRef,Form("p_{T}^{Trk} > %.0f GeV/c",ptMin),"");
  leg->AddEntry(hCorrEtaRef,Form("Leading RecJet, p_{T} > %.0f GeV/c",jet1PtMin),"p");
  leg->AddEntry(hCorrEta,Form("Subleading RecJet, p_{T} > %.0f GeV/c",jet2PtMin),"p");
  //leg->AddEntry(hCorrEtaRef,Form("Leading GenJet, p_{T} > %.0f GeV/c",jet1PtMin),"p");
  //leg->AddEntry(hCorrEta,Form("Subleading GenJet, p_{T} > %.0f GeV/c",jet2PtMin),"p");
  leg->Draw();

  cEffEta->Print("TrkCorr_vs_Eta_Central"+app+".gif");
  cEffEta->Print("TrkCorr_vs_Eta_Central"+app+".pdf");
}
