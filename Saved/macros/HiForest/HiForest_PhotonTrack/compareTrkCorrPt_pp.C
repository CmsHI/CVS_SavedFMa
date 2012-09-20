#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "hiForest.h"
#include "TLegend.h"
#include "TSystem.h"
#include "HisMath.C"
#include "commonUtility.h"

void compareTrkCorrPt_pp(
                           TString outdir="fig/07.28_TrkCorr14"
)
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);
   float xmin=1,xmax=179.9;
   TString title="pp";
   TString reftitle="PbPb";

   /////////////////////////////////////////////////////////////////////////////////////
   // Load Histograms
   /////////////////////////////////////////////////////////////////////////////////////
   HiForest * c = new HiForest("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat200/mergedFile.root","forest",1);
   c->doTrackCorrections = true;
   c->doTrackingSeparateLeadingSubleading = false;
   c->InitTree();   
   TrackingCorrections * trkCorr = c->trackCorrections[0];

   HiForest * cref = new HiForest("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat200/Dijet200_HydjetDrum_v28_mergedV1.root","forestref",0);
   cref->doTrackCorrections = true;
   cref->doTrackingSeparateLeadingSubleading = false;
   cref->InitTree();
   TrackingCorrections * trkCorrRef = cref->trackCorrections[0];


   cout << endl << "========= plot =========" << endl;
   Int_t etaPM=5.; // 7+2,-3 for |eta|<1.2, 7+5,-6 for full eta
   Float_t jetPtMin=0;
   Int_t jetBegBin = trkCorr->jetBin_->FindBin(jetPtMin);
   Int_t jetEndBin = trkCorr->numJEtBins_;
   cout << Form("jet pt %.0f bin: ",jetPtMin) << jetBegBin << " to " << jetEndBin << endl;
   cout << "========================" << endl;

   TString tag = trkCorr->trkCorrModule_+Form("_vs_Pt_%s_%s_jet%.0f_ieta%d_wts%d",title.Data(),reftitle.Data(),jetPtMin,etaPM,trkCorr->weightSamples_);
   
   // Get Eff/fake histograms
   int numCentBin=trkCorr->numCentBins_;
   int numCentBinRef=trkCorrRef->numCentBins_;
	TH1D * vhCorrPtRef[2][5], *vhCorrPt[2][5];
	Int_t colors[10] = {kBlack,kRed,kYellow+2,kGreen+2,kBlue};
   Int_t styles[2] = {kFullCircle,kOpenCircle};

   int icent=0;
   int icentRef=numCentBinRef-1;

	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=0; c<numCentBin; ++c) {
			vhCorrPt[lv][c] = (TH1D*) trkCorr->InspectCorr(lv,c,c,jetBegBin,jetEndBin,2,7-etaPM-1,7+etaPM);
			handsomeTH1(vhCorrPt[lv][c],kBlue,1,kOpenCircle);
         vhCorrPt[lv][icent]->SetAxisRange(xmin,xmax,"X");
         vhCorrPt[lv][icent]->SetAxisRange(0,1,"Y");
		}
		for (Int_t c=0; c<numCentBinRef; ++c) {
			vhCorrPtRef[lv][c] = (TH1D*) trkCorrRef->InspectCorr(lv,c,c,jetBegBin,jetEndBin,2,7-etaPM-1,7+etaPM);
			handsomeTH1(vhCorrPtRef[lv][c],colors[c]);
         vhCorrPtRef[lv][icent]->SetAxisRange(xmin,xmax,"X");
         vhCorrPtRef[lv][icent]->SetAxisRange(0,1,"Y");
		}
	}
   
	TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
   cEff->SetLogx();
   vhCorrPt[0][icent]->SetTitle(";Track p_{T} (GeV/c);A #times #epsilon");
   vhCorrPt[0][icent]->SetTitleOffset(1.2);
   vhCorrPt[0][icent]->SetTitleSize(0.055);
	vhCorrPt[0][icent]->Draw("E");
	vhCorrPt[1][icent]->Draw("sameE");
	vhCorrPtRef[0][0]->Draw("sameE");
	vhCorrPtRef[1][0]->Draw("sameE");
	vhCorrPtRef[0][icentRef]->Draw("sameE");
	vhCorrPtRef[1][icentRef]->Draw("sameE");

   TLegend *leg0 = new TLegend(0.16,0.84,0.46,0.92);
   leg0->SetFillStyle(0);
   leg0->SetBorderSize(0);
   leg0->SetTextSize(0.04);
   leg0->AddEntry(vhCorrPt[0][0],"PYTHIA+HYDJET","");
   if (etaPM==5)   leg0->AddEntry(vhCorrPt[0][0],Form("Track |#eta|<2.4"),"");
   if (etaPM==2) leg0->AddEntry(vhCorrPt[0][0],Form("Track |#eta|<1.2"),"");
	leg0->Draw();
   TLine * l = new TLine(xmin,1,xmax,1);
   l->SetLineStyle(2);
   l->Draw();
	
   TLegend *leg = new TLegend(0.34,0.32,0.60,0.43);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.035);
   leg->AddEntry(vhCorrPt[0][icent],title,"p");
	leg->AddEntry(vhCorrPtRef[0][icentRef],reftitle+ " 30-100%","p");
	leg->AddEntry(vhCorrPtRef[0][0],reftitle+" 0-30%","p");
   leg->Draw();
   
	drawText("CMS Simulation",0.64,0.89);
	drawText("Fake Rate",0.69,0.26);
   
   cEff->Print(outdir+"/"+tag+".gif");
   cEff->Print(outdir+"/"+tag+".pdf");

	TCanvas * cJet = new TCanvas("cJet","cJet",500,500);
	cJet->SetLogy();
	trkCorrRef->vhPtHat[1][icentRef]->SetMarkerColor(kRed);
	trkCorrRef->vhPtHat[1][icentRef]->SetMarkerStyle(kOpenCircle);
	trkCorrRef->vhPtHat[1][icentRef]->Draw("E");
	trkCorr->vhPtHat[1][icent]->Draw("sameE");

	TCanvas * cCent = new TCanvas("cCent","cCent",500,500);
   TH1D * hCent = (TH1D*)trkCorr->sample_[0]->Get("hCent");
   TH1D * hCentRef = (TH1D*)trkCorrRef->sample_[0]->Get("hCent");
	hCentRef->SetMarkerStyle(kOpenCircle);
	hCentRef->Scale(1./hCentRef->GetEntries());
	hCent->Scale(1./hCent->GetEntries());
	hCentRef->Draw("p");
	hCent->Draw("samep");
}
