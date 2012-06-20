#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "TSystem.h"
#include "commonUtility.h"

void drawTrkCorrPtvCent(
                           TString outdir="fig/06.19_TrkMt"
)
{
   gSystem->mkdir(outdir,kTRUE);
//    TString mod="hitrkEffAnalyzer_MergedSelected";
   TString mod="hitrkEffAnalyzer_MergedGeneral";
//    TString mod="hitrkEffAnalyzer_Selected";
//    TString mod="hitrkEffAnalyzer_General";
//    TString mod="hitrkEffAnalyzer_GeneralCalo";
//    TString mod="hitrkEffAnalyzer_PixTrk";
//    TString mod="hitrkEffAnalyzer_MergeTrk";
   float xmin=0.5,xmax=179.9;
   TString title="Iterative Tracking";
   if (mod.Contains("Pix")) {
      title="Pixel Tracks";
      xmin=0.5,
      xmax=9.9;
   } else if (mod.Contains("Calo")) {
      title="Iter. Calo Tracks";
   }
   TrackingCorrections trkCorr("itertrkpixtrk",mod);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj30_v1.root",30);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj50_v1.root",50);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj80_v1.root",80);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj120_v1.root",120);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj200_v1.root",200);

//    trkCorr.AddSample("../trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj50_iterTrkPixTrk_v1_full.root",50);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj120_iterTrkPixTrk_v1_full.root",120);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj170_iterTrkPixTrk_v1_full.root",170);
//    trkCorr.AddSample("../trkcorr/itertrkpixtrk_hgt/trkcorr_hy18dj200_iterTrkPixTrk_v1_full.root",200);

//    trkCorr.AddSample("../trkcorr/HighPt_v1/trkcorr_hy18dj50_HighPt_v1.root",50);
//    trkCorr.AddSample("../trkcorr/HighPt_v1/trkcorr_hy18dj120_HighPt_v1.root",120);

   trkCorr.AddSample("../trkcorr/Forest2_v19/trkcorr_hy18dj30_Forest2_v19.root",30);
   trkCorr.AddSample("../trkcorr/Forest2_v19/trkcorr_hy18dj50_Forest2_v19.root",50);
   trkCorr.AddSample("../trkcorr/Forest2_v19/trkcorr_hy18dj80_Forest2_v19.root",80);
   trkCorr.AddSample("../trkcorr/Forest2_v19/trkcorr_hy18dj120_Forest2_v19.root",120);
   trkCorr.AddSample("../trkcorr/Forest2_v19/trkcorr_hy18dj170_Forest2_v19.root",170);
   trkCorr.smoothLevel_ = 1;
   trkCorr.Init();
   
   cout << endl << "========= plot =========" << endl;
   Int_t etaPM=2; // 7 +2,-3 for |eta|<1.2, 7 =5,-6 for full eta
   Float_t jetPtMin=0;
   Int_t jetBegBin = trkCorr.jetBin_->FindBin(jetPtMin);
   Int_t jetEndBin = trkCorr.numJEtBins_;
   cout << Form("jet pt %.0f bin: ",jetPtMin) << jetBegBin << " to " << jetEndBin << endl;
   cout << "========================" << endl;
   
   // Get Eff/fake histograms
	TH1D * vhCorrPtRef[2][5], *vhCorrPt[2][5];
	Int_t colors[10] = {kBlack,kRed,kYellow+2,kGreen+2,kBlue};
   Int_t styles[2] = {kFullCircle,kOpenCircle};
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=0; c<5; ++c) {
			vhCorrPt[lv][c] = (TH1D*) trkCorr.InspectCorr(lv,c,c,jetBegBin,jetEndBin,2,7-etaPM-1,7+etaPM);
			vhCorrPt[lv][c]->SetMarkerColor(colors[c]);
         vhCorrPt[lv][c]->SetMarkerStyle(styles[lv]);
			handsomeTH1(vhCorrPt[lv][c]);
		}
	}
   
	TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
   cEff->SetLogx();
   vhCorrPt[0][0]->SetAxisRange(xmin,xmax,"X");
   vhCorrPt[0][0]->SetAxisRange(0,1,"Y");
   vhCorrPt[0][0]->SetTitle(";Track p_{T} (GeV/c);A #times #epsilon");
   vhCorrPt[0][0]->SetTitleOffset(1.2);
   vhCorrPt[0][0]->SetTitleSize(0.055);
	vhCorrPt[0][0]->Draw("E");
	vhCorrPt[1][0]->Draw("sameE");
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=4; c>=0; --c) {
			vhCorrPt[lv][c]->Draw("sameE");
		}
	}
   TLegend *leg0 = new TLegend(0.16,0.786,0.46,0.92);
   leg0->SetFillStyle(0);
   leg0->SetBorderSize(0);
   leg0->SetTextSize(0.04);
   leg0->AddEntry(vhCorrPt[0][0],"PYTHIA+HYDJET","");
   if (jetPtMin >= 40) leg0->AddEntry(vhCorrPt[0][0],Form("Jet p_{T} #geq %.0f GeV/c",jetPtMin),"");
   leg0->AddEntry(vhCorrPt[0][0],Form("Track %.1f < #eta < %.1f",trkCorr.etaBin_->GetBinLowEdge(7-etaPM-1), trkCorr.etaBin_->GetBinLowEdge(7+etaPM+1)),"");
	leg0->Draw();
//    TLine * l = new TLine(xmin,1,xmax,1);
//    l->SetLineStyle(2);
//    l->Draw();
	
   TLegend *leg = new TLegend(0.34,0.25,0.56,0.55);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.035);
   leg->AddEntry(vhCorrPt[0][0],title,"");
	leg->AddEntry(vhCorrPt[0][0],"0-5%","p");
	leg->AddEntry(vhCorrPt[0][1],"5-10%","p");
	leg->AddEntry(vhCorrPt[0][2],"10-20%","p");
	leg->AddEntry(vhCorrPt[0][3],"30-50%","p");
	leg->AddEntry(vhCorrPt[0][4],"50-90%","p");
   leg->Draw();
   
	drawText("CMS Simulation",0.64,0.89);
	drawText("Fake Rate",0.69,0.26);
   
   cEff->Print(outdir+"/"+mod+Form("_vs_Pt_vsCentrality_jet%.0f_ieta%d.gif",jetPtMin,etaPM));
   cEff->Print(outdir+"/"+mod+Form("_vs_Pt_vsCentrality_jet%.0f_ieta%d.pdf",jetPtMin,etaPM));

	TCanvas * cEff2D = new TCanvas("cEff2D","cEff2D",800,400);
	gPad->SetLogy();
   Double_t pt=10,eta=0,jet=120;
   Int_t cBin = 0;
   cEff2D->Divide(2,1);
   cEff2D->cd(1);
	gPad->SetLogy();
   TH2D * hCorr2D = (TH2D*)trkCorr.InspectCorr(0,cBin,cBin,trkCorr.jetBin_->FindBin(jet),trkCorr.jetBin_->FindBin(jet));
   gPad->SetRightMargin(0.15);
   hCorr2D->SetAxisRange(0,119.9,"Y");
   hCorr2D->SetAxisRange(0,1,"Z");
   hCorr2D->Draw("colz");
   cEff2D->cd(2);
	gPad->SetLogy();
   TH2D * hCorr2DFak= (TH2D*)trkCorr.InspectCorr(1,cBin,cBin,trkCorr.jetBin_->FindBin(jet),trkCorr.jetBin_->FindBin(jet));
   gPad->SetRightMargin(0.15);
   hCorr2DFak->SetAxisRange(0,119.9,"Y");
   hCorr2DFak->SetAxisRange(0,1,"Z");
   hCorr2DFak->Draw("colz");
   
   // test corr
   cout << "trk weight: " << trkCorr.GetCorr(pt,eta,jet,cBin) << endl;
   Double_t corr[4];
   for (Int_t i=1; i<=trkCorr.ptBin_->GetNbinsX(); ++i) {
      trkCorr.GetCorr(trkCorr.ptBin_->GetBinCenter(i),eta,jet,cBin,corr);
      cout << "trk pt: " << trkCorr.ptBin_->GetBinLowEdge(i) << " trk eff: " << corr[0] << " trk fak: " << corr[1] << endl;
   }
   
}
