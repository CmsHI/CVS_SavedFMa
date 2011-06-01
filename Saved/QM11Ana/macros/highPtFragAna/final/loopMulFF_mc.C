#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TMath.h"
#include "MulFFAnaLoop.h"
#include "../../HisMath.C"

void loopMulFF_mc(
    Bool_t doSel=1,
    Bool_t doTrkCorr=true)
{
  // ===================================
  // Inputs
  // ===================================
  Int_t jetType=2;
  Int_t particleType=2;
  Int_t pfCandType=0;
	TString anV="0601Closure";
  cout << "=============== MulFF Ana ======================" << endl;
  cout << " ana: " << anV << endl;
	// HI
	TString infgen=Form("../ntout/tranav12_hydjuq80pfhgtv1repass_j4_j2t0_et90.root");
	TString infrec=Form("../ntout/tranav12_hydjuq80pfhgtv1repass_j4_j2t2_et90.root");
	TString tag = Form("tv12hydjuq80repass_j4_j2t2_et90_%s",anV.Data());
  TChain * tgen = new TChain("tjf");  tgen->Add(infgen);
	TChain * trec = new TChain("tjf");  trec->Add(infrec);
	if (tgen->GetEntries()>0) cout << infgen << ": " << tgen->GetEntries() << endl;
	else { cout << infgen << " has 0 entries" << endl; exit(1); }
  if (trec->GetEntries()>0) cout << trec->GetName() << ": " << trec->GetEntries() << endl;
  else { cout << infrec << " has 0 entries" << endl; exit(1); }
	// for reweighting
//  TChain * trecdata = new TChain("tjf");
//	trecdata->Add("../ntout/tranav12_dataj35pfhgtv1repass2_j4_j2t2_et90.root");
//  if (trecdata->GetEntries()>0) cout << trecdata->GetName() << ": " << trecdata->GetEntries() << endl;
//  else { cout << trecdata->GetName() << " has 0 entries" << endl; exit(1); }

  // ===================================
  // Cuts
  // ===================================
  selectionCut cut;
  cut.doSel = doSel;
  cut.CentMin=0;
  cut.CentMax=12;
  cut.JEtMin[0] = 100;
  cut.JEtMax[0] = 300;
  cut.JEtMin[1] = 40; // 40, 60
  cut.JEtMax[1] = 300; // 300, 90
  cut.JEtaMax[0] = 2; // 1.6 for rocket
  cut.JEtaMax[1] = 2; // 1.6 for rocket
  cut.JDPhiMin = TMath::Pi()*2./3.;
  cut.AjMin = 0;
  cut.AjMax = 1;
  cut.TrkPtMin = 4;
  cut.TrkEtaMax = 2.4;
  cut.ConeSize = 0.3;
	
	//tag+=Form("_excleta0_c%.0fto%.0f_a%.0fto%.0f",cut.CentMin,cut.CentMax,cut.AjMin*100,cut.AjMax*100);
	tag+=Form("_c%.0fto%.0f_a%.0fto%.0f",cut.CentMin,cut.CentMax,cut.AjMin*100,cut.AjMax*100);
  cout << "Output: " << tag << endl;
  TString outFileName(Form("histff_%s.root",tag.Data()));
  cout << "================================================" << endl;

	// pt bins
	const Int_t numPPtBins=18;
  Float_t pptBins[numPPtBins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};
  TH1D * hxbin = new TH1D("hxbin","",numPPtBins,pptBins);
	//hxbin->Rebin(2);
	vector<Double_t> ptBin;
	for (Int_t i=1; i<=hxbin->GetNbinsX()+1; ++i) {
		ptBin.push_back(hxbin->GetBinLowEdge(i));
	}
	
  // ===================================
  // Correction
  // ===================================
  Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_tev9hgtv4_3","hitrkEffAnalyzer_akpu3pf");
  trkCorrJ1.isLeadingJet_ = true;
  trkCorrJ1.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ1.smoothLevel_ = 4; // 0: no smooth, 1: smooth jet, 2: smooth jet,pt 3: smooth jet,pt,cbin
  trkCorrJ1.Init();
  Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_tev9hgtv4_3","hitrkEffAnalyzer_akpu3pf");
  trkCorrJ2.isLeadingJet_ = false;
  trkCorrJ2.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ2.smoothLevel_ = 4; // 0: no smooth, 1: smooth jet, 2: smooth jet,pt 3: smooth jet,pt,cbin
  trkCorrJ2.Init();

  // ===================================
  // Analyze
  // ===================================
  TFile * outf = new TFile(outFileName,"RECREATE");
  // rec trk
  FragAnaLoop recfana("Rec");
	//recfana.isMC_ = 1;
  recfana.anaTrkType_=particleType;
  recfana.pfCandType_=pfCandType;
  if (!doTrkCorr) recfana.anaTrkType_=0;
  recfana.t_ = trec;
	//recfana.tdata_ = trecdata;
  recfana.cut_ = &cut;
  recfana.vtrkCorr_[0] = &trkCorrJ1;
  recfana.vtrkCorr_[1] = &trkCorrJ2;
	recfana.ptBin_ = ptBin;
  recfana.Init();
  recfana.Loop();
	
	// gen partilce
  FragAnaLoop genfana("Gen");
	//recfana.isMC_ = 1;
	genfana.anaTrkType_=0;
	genfana.t_ = tgen;
	genfana.cut_ = &cut;
	genfana.vtrkCorr_[0] = &trkCorrJ1;
	genfana.vtrkCorr_[1] = &trkCorrJ2;
	genfana.ptBin_ = ptBin;
	genfana.Init();
	genfana.Loop();
	
	outf->Write();
}
