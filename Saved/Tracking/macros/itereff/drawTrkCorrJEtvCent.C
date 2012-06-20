#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "Corrector3D.h"
#include "TLegend.h"
#include "TSystem.h"
#include "commonUtility.h"

void drawTrkCorrJEtvCent(
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
   float xmin=0.5,xmax=499.9;
   TString title="Iterative Tracking";
   if (mod.Contains("Pix")) {
      title="Pixel Tracks";
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
   Float_t trkPtMin=1;
   Int_t trkBegBin = trkCorr.ptBin_->FindBin(trkPtMin);
   Int_t trkEndBin = trkCorr.numPtBins_;
   cout << Form("trk pt %.0f bin: ",trkPtMin) << trkBegBin << " to " << trkEndBin << endl;
   cout << "========================" << endl;
   
   // Get Eff/fake histograms
	TH1D * vhCorrPtRef[2][5], *vhCorrPt[2][5];
	Int_t colors[10] = {kBlack,kRed,kYellow+2,kGreen+2,kBlue};
   Int_t styles[2] = {kFullCircle,kOpenCircle};
	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=0; c<5; ++c) {
			vhCorrPt[lv][c] = (TH1D*) trkCorr.InspectCorr(lv,c,c,trkBegBin,trkEndBin,3,7-etaPM-1,7+etaPM);
			vhCorrPt[lv][c]->SetMarkerColor(colors[c]);
         vhCorrPt[lv][c]->SetMarkerStyle(styles[lv]);
			handsomeTH1(vhCorrPt[lv][c]);
		}
	}
   
	TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
   cEff->SetLogx();
   vhCorrPt[0][0]->SetAxisRange(xmin,xmax,"X");
   vhCorrPt[0][0]->SetAxisRange(0,1,"Y");
   vhCorrPt[0][0]->SetTitle(";Jet p_{T} (GeV/c);A #times #epsilon");
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
   leg0->AddEntry(vhCorrPt[0][0],Form("Track p_{T} > %.0f GeV/c",trkPtMin),"");
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
   
   cEff->Print(outdir+"/"+mod+Form("_vs_JEt_vsCentrality_pt%.0f_ieta%d.gif",trkPtMin,etaPM));
   cEff->Print(outdir+"/"+mod+Form("_vs_JEt_vsCentrality_pt%.0f_ieta%d.pdf",trkPtMin,etaPM));
}
