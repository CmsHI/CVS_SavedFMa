#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "../../commonUtility.h"

void drawTrkCorrPt(Int_t corrLevel=0,
    TString mod="hiGoodTightTrkEffAnalyzer",
		TString modref="hitrkEffAnalyzer_akpu3pf",
    //TString modref="hitrkEffAnalyzer",
		//TString app="_tev9hgtv4_2",
		//TString appref="_tev9hgtv4_2",
		TString app="_ppcorrpthgtv4",
		TString appref="_ppcorrpthgtv4",
		//TString appref="",
    //TString mod="hitrkEffAnalyzer_akpu3pf",
    //TString modref="hitrkEffAnalyzer_akpu3pf",
    //TString mod="hiGoodTightTrkEffAnalyzer_icpu5calo_j2",
    //TString modref="hiGoodTightTrkEffAnalyzer_icpu5calo_j1",
		//TString mod="hiGoodTightTrkEffAnalyzer",
		//TString modref="hiHighPtTrkEffAnalyzer",
    //TString app="_mattgmv1",
    //TString appref="_tev6",
    //TString app="_tev6",
    //TString appref="_matthptv1",
    //TString app="_matthptv1",
    //TString appref="_ppz2gtv1",
    //TString app="_pp413hgtv1",
		//TString appref="_pp413hgtv1",
    //TString app="_tev8hgtv1",
		//TString appref="_tev8hgtv1",
    //TString app="_pp413hgtv1",
    //TString appref="_tev7hpt",
    //TString app="_tev7genjet",
    //TString appref="_tev7genjet",
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
  Int_t centBeg=0, centEnd=4; // HI: 0-30% = 0-2
  Int_t centBegRef=0, centEndRef=4; // pp: all cent 0-4

  Corrector3D trkCorr("trkCorrHisAna_djuq",app,mod);
  //Corrector3D trkCorr("trkhist_may052011_rereco_chi2_v2_hydjetBass_dijet",app,mod);
  trkCorr.ptHatMin_.clear();
  trkCorr.ptHatMin_.push_back(30);
  trkCorr.ptHatMin_.push_back(50);
  trkCorr.ptHatMin_.push_back(80);
  trkCorr.ptHatMin_.push_back(110);
  trkCorr.ptHatMin_.push_back(170);
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.Init();

  Corrector3D trkCorrRef("trkCorrHisAna_djuq",appref,modref);
  //Corrector3D trkCorrRef("trkhist_may052011_rereco_chi2_v2_hydjetBass_dijet",appref,modref);
  trkCorrRef.ptHatMin_.clear();
  trkCorrRef.ptHatMin_.push_back(30);
  trkCorrRef.ptHatMin_.push_back(50);
  trkCorrRef.ptHatMin_.push_back(80);
  trkCorrRef.ptHatMin_.push_back(110);
  trkCorrRef.ptHatMin_.push_back(170);
  trkCorrRef.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrRef.Init();

  cout << endl << "========= plot =========" << endl;
  Float_t jet1PtMin=40.1;
  Float_t jet2PtMin=40.1;
  Int_t jet1BegBin = trkCorrRef.jetBin_->FindBin(jet1PtMin);
  Int_t jet2BegBin = trkCorr.jetBin_->FindBin(jet2PtMin);
  Int_t jetEndBin = 20;
  cout << Form("jet pt %.0f bin: ",jet1PtMin) << jet1BegBin << Form(" %.0f bin: ",jet2PtMin) << jet2BegBin << endl;
  cout << "jet pt end bin: " << jetEndBin << endl;
  cout << "========================" << endl;

  TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
  cEff->SetLogx();
  // eff
  TH1D * hCorrPtRefJet100 = (TH1D*) trkCorrRef.InspectCorr(0,isample,centBegRef,centEndRef,jet1BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  handsomeTH1(hCorrPtRefJet100);
  hCorrPtRefJet100->SetAxisRange(1,119.9,"X");
  hCorrPtRefJet100->SetAxisRange(0,1,"Y");
  hCorrPtRefJet100->SetTitle(";p_{T}^{Trk} (GeV/c); Eff., Fake Rate");
  hCorrPtRefJet100->SetMarkerStyle(kOpenSquare);
  hCorrPtRefJet100->Draw("E");
  TH1D * hCorrPtJet40 = (TH1D*) trkCorr.InspectCorr(0,isample,centBeg,centEnd,jet2BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  hCorrPtJet40->Draw("same E");

  // fake
  TH1D * hCorr1PtRefJet100 = (TH1D*) trkCorrRef.InspectCorr(1,isample,centBegRef,centEndRef,jet1BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  hCorr1PtRefJet100->SetMarkerStyle(kOpenSquare);
  hCorr1PtRefJet100->Draw("sameE");
  TH1D * hCorr1PtJet40 = (TH1D*) trkCorr.InspectCorr(1,isample,centBeg,centEnd,jet2BegBin,jetEndBin,2,7-etaPM,7+etaPM);
  hCorr1PtJet40->Draw("sameE");

  TLegend *leg0 = new TLegend(0.21,0.75,0.51,0.93);
  leg0->SetFillStyle(0);
  leg0->SetBorderSize(0);
  leg0->SetTextSize(0.04);
  leg0->AddEntry(hCorrPtRefJet100,"PYTHIA+HYDJET","");
  leg0->AddEntry(hCorrPtRefJet100,"0-30%","");
  //leg0->AddEntry(hCorrPtRefJet100,Form("#hat{p}_{T} 30,50,80,110,170 GeV/c"),"");
  leg0->AddEntry(hCorrPtRefJet100,Form("Jet p_{T} > %.0f GeV/c, Trk |#eta|<1",jet1PtMin),"");
	leg0->Draw();
  
	TLegend *leg = new TLegend(0.21,0.3,0.51,0.5);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  //leg->AddEntry(hCorrPtRefJet100,"hiGoodTight","");
  leg->AddEntry(hCorrPtRefJet100,Form("Jet Analysis Tracks"),"p");
  leg->AddEntry(hCorrPtJet40,Form("hiGoodTight"),"p");
  leg->Draw();

  cEff->Print("TrkCorr_vs_Pt_Central"+app+".gif");
  cEff->Print("TrkCorr_vs_Pt_Central"+app+".pdf");
}
