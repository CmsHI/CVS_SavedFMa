#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "../../commonUtility.h"

void drawTrkCorrEtavCent(Int_t corrLevel=0,
												//TString mod="hitrkEffAnalyzer",
												//TString modref="hitrkEffAnalyzer",
												//TString app="",
												//TString appref="",
												//TString mod="hitrkEffAnalyzer",
												//TString modref="hitrkEffAnalyzer",
												TString mod="hitrkEffAnalyzer_akpu3pf",
												TString modref="hitrkEffAnalyzer_akpu3pf",
												//TString mod="hiGoodTightTrkEffAnalyzer_icpu5calo_j2",
												//TString modref="hiGoodTightTrkEffAnalyzer_icpu5calo_j1",
												//TString modref="hiGoodTightTrkEffAnalyzer",
												//TString mod="hiGoodTightTrkEffAnalyzer",
												//TString app="_mattgmv1",
												TString appref="_ppcorrpthgtv4",
												TString app="_tev9hgtv4_3",
												//TString app="_tev6",
												//TString appref="_ppv1",
												//TString appref="_matthptv1",
												//TString app="_matthptv1",
												//TString appref="_ppz2gtv1",
												//TString app="_pp413hgtv1",
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
  Float_t jet1PtMin=40;
  Float_t jet2PtMin=40;
  Int_t jet1BegBin = trkCorrRef.jetBin_->FindBin(jet1PtMin);
  Int_t jet2BegBin = trkCorr.jetBin_->FindBin(jet2PtMin);
  Int_t jetEndBin = 20;
  cout << Form("jet pt %.0f bin: ",jet1PtMin) << jet1BegBin << Form(" %.0f bin: ",jet2PtMin) << jet2BegBin << endl;
  cout << "jet pt end bin: " << jetEndBin << endl;
	Float_t ptMin=4;
	Int_t ptBegBin = trkCorrRef.ptBin_->FindBin(ptMin);
	Int_t ptEndBin = trkCorrRef.ptBin_->FindBin(200);
	cout << "pt beg bin: " << ptBegBin << " end bin: " << ptEndBin << endl;
  cout << "========================" << endl;

  // Get Eff/fake histograms
	TH1D * vhCorrPtRef[2][5], *vhCorrPt[2][5];
	Int_t colors[10] = {kBlack,kRed,kYellow+2,kGreen+2,kBlue};
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=0; c<5; ++c) {
			vhCorrPt[lv][c] = (TH1D*) trkCorr.InspectCorr(lv,isample,c,c,jet2BegBin,jetEndBin,1,ptBegBin,ptEndBin);
			vhCorrPt[lv][c]->SetMarkerColor(colors[c]);
			handsomeTH1(vhCorrPt[lv][c]);
			vhCorrPt[lv][c]->SetAxisRange(-2.4,2.4,"X");
			vhCorrPt[lv][c]->SetAxisRange(0,1,"Y");
			vhCorrPt[lv][c]->SetTitle(";Track #eta;A #times #epsilon");
			vhCorrPt[lv][c]->SetTitleOffset(1.2);
			vhCorrPt[lv][c]->SetTitleSize(0.055);
		}
		vhCorrPtRef[lv][0] = (TH1D*) trkCorrRef.InspectCorr(lv,isample,0,4,jet1BegBin,jetEndBin,1,ptBegBin,ptEndBin);
		vhCorrPtRef[lv][0]->SetMarkerStyle(kOpenSquare);
	}
	TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
	vhCorrPt[0][0]->Draw("E");
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=4; c>=0; --c) {
			vhCorrPt[lv][c]->Draw("sameE");
		}
		vhCorrPtRef[lv][0]->Draw("sameE");
	}
  TLegend *leg0 = new TLegend(0.16,0.76,0.46,0.89);
  leg0->SetFillStyle(0);
  leg0->SetBorderSize(0);
  leg0->SetTextSize(0.04);
  leg0->AddEntry(vhCorrPt[0][0],"PYTHIA+HYDJET","");
  //leg0->AddEntry(vhCorrPt[0][0],Form("#hat{p}_{T} 30,50,80,110,170 GeV/c"),"");
  leg0->AddEntry(vhCorrPt[0][0],Form("Jet p_{T} > %.0f GeV/c, Track p_{T} > %.0f GeV/c",jet1PtMin,ptMin),"");
	leg0->Draw();
	
  TLegend *leg = new TLegend(0.51,0.24,0.81,0.54);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  //leg->AddEntry(vhCorrPt[0][0],"Analysis Track Selection","");
	leg->AddEntry(vhCorrPt[0][0],"0-5%","p");
	leg->AddEntry(vhCorrPt[0][1],"5-10%","p");
	leg->AddEntry(vhCorrPt[0][2],"10-20%","p");
	leg->AddEntry(vhCorrPt[0][3],"30-50%","p");
	leg->AddEntry(vhCorrPt[0][4],"50-90%","p");
	leg->AddEntry(vhCorrPtRef[0][0],"pp","p");
  //leg->Draw();

	drawText("CMS Simulation",0.64,0.89);
	drawText("Fake Rate",0.6,0.2);

  cEff->Print("TrkCorr_vs_Eta_vsCentrality"+app+".gif");
  cEff->Print("TrkCorr_vs_Eta_vsCentrality"+app+".pdf");
}
