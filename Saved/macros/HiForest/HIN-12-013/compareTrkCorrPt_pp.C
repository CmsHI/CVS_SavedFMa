#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "hiForest.h"
#include "TLegend.h"
#include "TSystem.h"
#include "HisMath.C"
#include "commonUtility.h"

void compareTrkCorrPt_pp(
                           TString outdir="fig/Dec20_TrkCorr14"
)
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);
   float xmin=1,xmax=179.9;
   TString title="PYTHIA";
   TString reftitle="PYTHIA+HYDJET";

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
   Int_t etaPM=2.; // 7+2,-3 for |eta|<1.2, 7+5,-6 for full eta
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
   TString centTitle[4]={"0-10%","10-30%","30-50%","50-100%"};

   int icent=0;
   int icentRef=numCentBinRef-1;

	for (Int_t lv=0; lv<2; ++lv) {
		for (Int_t c=0; c<numCentBin; ++c) {
			vhCorrPt[lv][c] = (TH1D*) trkCorr->InspectCorr(lv,c,c,jetBegBin,jetEndBin,2,7-etaPM-1,7+etaPM);
			handsomeTH1(vhCorrPt[lv][c],kBlue,1,kOpenCircle);
         vhCorrPt[lv][c]->SetAxisRange(xmin,xmax,"X");
         vhCorrPt[lv][c]->SetAxisRange(0,1,"Y");
		}
		for (Int_t c=0; c<numCentBinRef; ++c) {
			vhCorrPtRef[lv][c] = (TH1D*) trkCorrRef->InspectCorr(lv,c,c,jetBegBin,jetEndBin,2,7-etaPM-1,7+etaPM);
			handsomeTH1(vhCorrPtRef[lv][c],colors[c]);
         vhCorrPtRef[lv][c]->SetAxisRange(xmin,xmax,"X");
         vhCorrPtRef[lv][c]->SetAxisRange(0,1,"Y");
		}
	}
   
	TCanvas * cEff = new TCanvas("cEff","cEff",500,500);
   cEff->SetLogx();
   vhCorrPt[0][icent]->SetTitle(";Track p_{T} (GeV/c);Tracking Performance");
   vhCorrPt[0][icent]->SetTitleOffset(1.2);
   vhCorrPt[0][icent]->SetTitleSize(0.055);
	vhCorrPt[0][icent]->Draw("E");
	vhCorrPt[1][icent]->Draw("sameE");
   for (Int_t c=numCentBinRef-1; c>=0; --c) {
      vhCorrPtRef[0][c]->Draw("sameE");
      vhCorrPtRef[1][c]->Draw("sameE");
   }   

   TLegend *leg0 = new TLegend(0.16,0.8,0.46,0.92);
   leg0->SetFillStyle(0);
   leg0->SetBorderSize(0);
   leg0->SetTextSize(0.04);
   leg0->AddEntry(vhCorrPt[0][0],"CMS Simulation","");
   if (etaPM==5) leg0->AddEntry(vhCorrPt[0][0],Form("Track |#eta|<2.4"),"");
   if (etaPM==2) leg0->AddEntry(vhCorrPt[0][0],Form("Track |#eta|<1.2"),"");
	leg0->Draw();
   TLine * l = new TLine(xmin,1,xmax,1);
   l->SetLineStyle(2);
   l->Draw();
	
   TLegend *leg = new TLegend(0.26,0.28,0.52,0.56);
   easyLeg(leg);
   for (Int_t c=0; c<numCentBinRef; ++c) leg->AddEntry(vhCorrPtRef[0][c],reftitle+" "+centTitle[c],"p");  
   leg->AddEntry(vhCorrPt[0][icent],title,"p");
   leg->Draw();
   
	// drawText("CMS Simulation",0.64,0.89);
   if (etaPM==5) drawText("Acc. #times Efficiency",0.6,0.74);
   if (etaPM==2) drawText("Acc. #times Efficiency",0.6,0.79);
	drawText("Fake track rate",0.6,0.2);
   
   cEff->Print(outdir+"/"+tag+".gif");
   cEff->Print(outdir+"/"+tag+".pdf");

	TCanvas * cJet = new TCanvas("cJet","cJet",500,500);
	cJet->SetLogy();
   handsomeTH1(trkCorr->vhPtHat[1][icent],kBlue,1,kOpenCircle);
   trkCorr->vhPtHat[1][icent]->SetTitle(";#hat{p}_{T} (GeV/c);a.u.");
	trkCorr->vhPtHat[1][icent]->Draw("E");
	trkCorrRef->vhPtHat[1][icentRef]->Draw("sameE");
   TLegend *leg2 = new TLegend(0.16,0.28,0.46,0.40);
   easyLeg(leg2);
   leg2->AddEntry(trkCorr->vhPtHat[1][icent],title,"p");
   leg2->AddEntry(trkCorrRef->vhPtHat[1][icentRef],reftitle,"p");
   leg2->Draw();
   cJet->Print(outdir+"/"+tag+"_pthat.gif");

	TCanvas * cCent = new TCanvas("cCent","cCent",500,500);
   TH1D * hCent = (TH1D*)trkCorr->sample_[0]->Get("hCent");
   TH1D * hCentRef = (TH1D*)trkCorrRef->sample_[0]->Get("hCent");
   hCent->SetMarkerColor(kBlue);
	hCent->SetMarkerStyle(kOpenCircle);
   handsomeTH1(hCent,kBlue,1,kOpenCircle);
	hCent->Scale(1./hCent->GetEntries());
   hCentRef->Scale(1./hCentRef->GetEntries());
	hCent->Draw("p");
	hCentRef->Draw("samep");
   leg2->Draw();
}
