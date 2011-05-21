#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TMath.h"
#include "MulFFAnaLoop.h"
#include "../../HisMath.C"

void loopMulFF_data(Int_t doMC=0,
    Bool_t doSel=1,
    Bool_t doTrkCorr=true)
{
  // ===================================
  // Inputs
  // ===================================
  TString infrec;
  TString tag;
  TChain * trec = new TChain("tjf");
  Int_t jetType=2;
  Int_t particleType=2;
  Int_t pfCandType=0;
	TString anV="0520rocket";
  cout << "=============== MulFF Ana ======================" << endl;
  cout << " ana: " << anV << endl;
  if (doMC==0) {
    // HI
    infrec=Form("../ntout/tranav12_dataj35pfhgtv1repass2_j4_j2t2_et90.root");
    tag = Form("tv12dataj35hgtrepass2_j4_%s",anV.Data());
    // pp
    //infrec=Form("../ntout/tranav11_ppdatajet_mattpfhgv1_%s_j2t2_pt4.root",algo.Data());
    //tag = Form("tv11ppdatajet_%s_%s",algo.Data(),anV.Data());
  }
  trec->Add(infrec);
  if (trec->GetEntries()>0) cout << infrec << ": " << trec->GetEntries() << endl;
  else { cout << infrec << " has 0 entries" << endl; exit(1); }
  cout << "Output: " << tag << endl;
  TString outFileName(Form("histff_%s.root",tag.Data()));
  cout << "================================================" << endl;

  // ===================================
  // Cuts
  // ===================================
  selectionCut cut;
  cut.doSel = doSel;
  cut.CentMin=0;
  cut.CentMax=12;
  //cut.CentMin=-1;
  //cut.CentMax=40;
  cut.JEtMin[0] = 100;
  cut.JEtMax[0] = 300;
  cut.JEtMin[1] = 40; // 40, 60
  cut.JEtMax[1] = 300; // 300, 90
  cut.JEtaMax[0] = 2.;
  cut.JEtaMax[1] = 2.;
  cut.JDPhiMin = TMath::Pi()*2./3.;
  cut.AjMin = 0.;
  cut.AjMax = 1;
  cut.TrkPtMin = 4.;
  cut.TrkEtaMax = 2.4;
  cut.ConeSize = 0.8;
  if (!cut.doSel) outFileName = Form("histntff_%s.root",tag.Data());

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
  recfana.anaTrkType_=particleType;
  recfana.pfCandType_=pfCandType;
  if (!doTrkCorr) recfana.anaTrkType_=0;
  recfana.t_ = trec;
  recfana.cut_ = &cut;
  //recfana.vtrkCorr_[0] = &trkCorr;
  recfana.vtrkCorr_[0] = &trkCorrJ1;
  recfana.vtrkCorr_[1] = &trkCorrJ2;
  recfana.Init();
  recfana.Loop();

  outf->Write();
}
