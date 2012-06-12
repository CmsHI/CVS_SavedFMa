#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "commonUtility.h"

void drawTrkCorrEtavCent()
{
   TString mod="hitrkEffAnalyzer_GeneralCalo";
//    TString mod="hitrkEffAnalyzer_PixTrk";
//    TString mod="hitrkEffAnalyzer_MergeTrk";
   float xmin=-2.4,xmax=2.4;
//    int trkBinBeg = 3;
//    int trkBinBeg = 7;
   int trkBinBeg = 13;
   TString title="Iter. Calo Tracks";
   if (mod.Contains("Pix")) {
      title="Pixel Tracks";
   } else if (mod.Contains("Merge")) {
      TString title="Merged Tracks";
   }
   TrackingCorrections trkCorr("itertrkpixtrk",mod);
   trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj30_v1.root",30);
   trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj50_v1.root",50);
   trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj80_v1.root",80);
   trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj120_v1.root",120);
   trkCorr.AddSample("../trkcorr/itertrkpixtrk/trkcorr_hy18dj200_v1.root",200);
   trkCorr.smoothLevel_ = 0;

   trkCorr.Init();
   
   cout << endl << "========= plot =========" << endl;
   Float_t jet2PtMin=0;
   Int_t jet2BegBin = trkCorr.jetBin_->FindBin(jet2PtMin);
   Int_t jetEndBin = 20;
   cout << Form("jet pt %.0f bin: ",jet2PtMin) << jet2BegBin << endl;
   cout << "jet pt end bin: " << jetEndBin << endl;
   cout << "========================" << endl;
   
   // Get Eff/fake histograms
	TH1D * vhCorrPtRef[2][5], *vhCorrPt[2][5];
	Int_t colors[10] = {kBlack,kRed,kYellow+2,kGreen+2,kBlue};
   Int_t styles[2] = {kFullCircle,kOpenCircle};
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=0; c<5; ++c) {
			vhCorrPt[lv][c] = (TH1D*) trkCorr.InspectCorr(lv,c,c,jet2BegBin,jetEndBin,1,trkBinBeg,24);
			vhCorrPt[lv][c]->SetMarkerColor(colors[c]);
         vhCorrPt[lv][c]->SetMarkerStyle(styles[lv]);
			handsomeTH1(vhCorrPt[lv][c]);
		}
	}
   
	TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
   vhCorrPt[0][0]->SetAxisRange(xmin,xmax,"X");
   vhCorrPt[0][0]->SetAxisRange(0,1,"Y");
   vhCorrPt[0][0]->SetTitle(";Track #eta;A #times #epsilon");
   vhCorrPt[0][0]->SetTitleOffset(1.2);
   vhCorrPt[0][0]->SetTitleSize(0.055);
	vhCorrPt[0][0]->Draw("E");
	vhCorrPt[1][0]->Draw("sameE");
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=4; c>=0; --c) {
			vhCorrPt[lv][c]->Draw("sameE");
		}
	}
   TLegend *leg0 = new TLegend(0.16,0.84,0.46,0.92);
   leg0->SetFillStyle(0);
   leg0->SetBorderSize(0);
   leg0->SetTextSize(0.04);
   leg0->AddEntry(vhCorrPt[0][0],"PYTHIA+HYDJET","");
   //   leg0->AddEntry(vhCorrPt[0][0],Form("Jet p_{T} > %.0f GeV/c, Track |#eta|<1",jet2PtMin),"");
   leg0->AddEntry(vhCorrPt[0][0],Form("Track p_{T} > %.1f",trkCorr.ptBin_->GetBinLowEdge(trkBinBeg)),"");
	leg0->Draw();
//    TLine * l = new TLine(xmin,1,xmax,1);
//    l->SetLineStyle(2);
//    l->Draw();
	
   TLegend *leg = new TLegend(0.39,0.22,0.61,0.52);
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
   
   cEff->Print(mod+Form("_vs_Eta_vsCentrality_ipt%d.gif",trkBinBeg));
   cEff->Print(mod+Form("_vs_Eta_vsCentrality_ipt%d.pdf",trkBinBeg));

	TCanvas * cEff2D = new TCanvas("cEff2D","cEff2D",800,400);
   Double_t jet=0;
   Int_t cBin = 0;
   cEff2D->Divide(2,1);
   cEff2D->cd(1);
   TH2D * hCorr2D = (TH2D*)trkCorr.InspectCorr(0,cBin,cBin,trkCorr.jetBin_->FindBin(jet),trkCorr.jetBin_->FindBin(500));
   gPad->SetRightMargin(0.15);
   hCorr2D->SetAxisRange(0,119.9,"Y");
   hCorr2D->SetAxisRange(0,1,"Z");
   hCorr2D->Draw("colz");
   cEff2D->cd(2);
   TH2D * hCorr2DFak= (TH2D*)trkCorr.InspectCorr(1,cBin,cBin,trkCorr.jetBin_->FindBin(jet),trkCorr.jetBin_->FindBin(500));
   gPad->SetRightMargin(0.15);
   hCorr2DFak->SetAxisRange(0,119.9,"Y");
   hCorr2DFak->SetAxisRange(0,1,"Z");
   hCorr2DFak->Draw("colz");
   
   // test corr
   Double_t pt=10,eta=0;
   cout << "trk weight: " << trkCorr.GetCorr(pt,eta,jet,cBin) << endl;
   Double_t corr[4];
   for (Int_t i=1; i<=trkCorr.ptBin_->GetNbinsX(); ++i) {
      trkCorr.GetCorr(trkCorr.ptBin_->GetBinCenter(i),eta,jet,cBin,corr);
      cout << "trk pt: " << trkCorr.ptBin_->GetBinLowEdge(i) << " trk eff: " << corr[0] << " trk fak: " << corr[1] << endl;
   }
   
}
