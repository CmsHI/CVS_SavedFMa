#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "../../commonUtility.h"

void drawTrkCorrJetPtvCent(
												TString mod="hitrkEffAnalyzer_akpu3pf",
												TString modref="hitrkEffAnalyzer_akpu3pf",
												//TString modref="hiGoodTightTrkEffAnalyzer",
												//TString mod="hiGoodTightTrkEffAnalyzer",
												//TString app="_mattgmv1",
												TString appref="_ppcorrpthgtv4",
												TString app="_tev9hgtv4_3",
												//TString appref="_ppv1",
												//TString app="_tev6",
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
			vhCorrPt[lv][c] = (TH1D*) trkCorr.InspectCorr(lv,isample,c,c,ptBegBin,ptEndBin,3,7-etaPM,7+etaPM);
			vhCorrPt[lv][c]->SetMarkerColor(colors[c]);
			handsomeTH1(vhCorrPt[lv][c]);
			vhCorrPt[lv][c]->SetAxisRange(40.1,499,"X");
			vhCorrPt[lv][c]->SetAxisRange(0,1,"Y");
			vhCorrPt[lv][c]->SetTitle(";Jet p_{T} (GeV/c);A #times #epsilon");
			vhCorrPt[lv][c]->SetTitleOffset(1.2);
			vhCorrPt[lv][c]->SetTitleSize(0.055);
		}
		vhCorrPtRef[lv][0] = (TH1D*) trkCorrRef.InspectCorr(lv,isample,0,4,ptBegBin,ptEndBin,3,7-etaPM,7+etaPM);
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
  leg0->AddEntry(vhCorrPt[0][0],Form("Track p_{T} > %.0f GeV/c, Track |#eta|<1",ptMin),"");
	leg0->Draw();
	
  TLegend *leg = new TLegend(0.26,0.25,0.56,0.55);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  //leg->AddEntry(vhCorrPt[0][0],"hiGoodTight Tracks","");
	leg->AddEntry(vhCorrPt[0][0],"0-5%","p");
	leg->AddEntry(vhCorrPt[0][1],"5-10%","p");
	leg->AddEntry(vhCorrPt[0][2],"10-20%","p");
	leg->AddEntry(vhCorrPt[0][3],"30-50%","p");
	leg->AddEntry(vhCorrPt[0][4],"50-90%","p");
	leg->AddEntry(vhCorrPtRef[0][0],"pp","p");
  //leg->Draw();
	
	drawText("CMS Simulation",0.64,0.89);
	drawText("Fake Rate",0.6,0.2);
	
  cEff->Print("TrkCorr_vs_JetPt_vsCentrality"+app+".gif");
  cEff->Print("TrkCorr_vs_JetPt_vsCentrality"+app+".pdf");
}
