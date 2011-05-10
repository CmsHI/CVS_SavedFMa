#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TMath.h"
#include "MulFFAnaLoop.h"
#include "../../HisMath.C"

void loopMulFF(Int_t doMC=1,
    Bool_t doSel=1,
    Bool_t doTrkCorr=true)
{
  // ===================================
  // Inputs
  // ===================================
  Double_t ptHatMin=80;
  TString infrec,infgen;
  TString tag;
  TChain * trec = new TChain("tjf");
  TChain * tgen = new TChain("tjf");
  //TString algo="j4";
  TString algo="akpu3pf";
  TString anV="an0510ff";
  Int_t jetType=2;
  Int_t particleType=2;
  Int_t pfCandType=0;
  cout << "=============== MulFF Ana ======================" << endl;
  cout << "  jet: " << algo << " ana: " << anV << endl;
  if (doMC==0) {
    // HI
    //infrec=Form("../ntout/tranav11_dataj35mattpfhgv2repass_%s_j2t2_pt4.root",algo.Data());
    //tag = Form("tv11data_%s_%s",algo.Data(),anV.Data());
    infrec=Form("../ntout/tranav12_dataj35mattpfhgmv2repass_%s_j2t2_pt4.root",algo.Data());
    tag = Form("tv12datahgm_%s_%s",algo.Data(),anV.Data());
    // pp
    //infrec=Form("../ntout/tranav11_ppdatajet_mattpfhgv1_%s_j2t2_pt4.root",algo.Data());
    //tag = Form("tv11ppdatajet_%s_%s",algo.Data(),anV.Data());
  }

  if (doMC==1) {
    // HI
    infgen=Form("../ntout/tranav11_hydjuq%.0fmattpfgmv2repass_%s_j%dt0_pt4.root",ptHatMin,algo.Data(),jetType);
    infrec=Form("../ntout/tranav11_hydjuq%.0fmattpfgmv2repass_%s_j%dt%d_pt4.root",ptHatMin,algo.Data(),jetType,particleType);
    tag = Form("tv11_hydj%.0fmattpfgmv2_%s_j%dt%d_%s",ptHatMin,algo.Data(),jetType,particleType,anV.Data());
    // pp
    //infgen=Form("../ntout/tranav11_dj%.0fmatthptv1_%s_j2t0.root",ptHatMin,algo.Data());
    //infrec=Form("../ntout/tranav11_dj%.0fmatthptv1_%s_j2t2.root",ptHatMin,algo.Data());
    //tag = Form("tv11_dj%.0fmatthptv1_%s_%s",ptHatMin,algo.Data(),anV.Data());
    //infrec=Form("../ntout/tranav11_dj%.0fmattpfgmv1_%s_j2t2.root",ptHatMin,algo.Data());
    //infgen=Form("../ntout/tranav11_dj%.0fmattpfgmv1_%s_j2t0.root",ptHatMin,algo.Data());
    //tag = Form("tv11_dj%.0fmattpfgmv1_%s_j2t2_%s",ptHatMin,algo.Data(),anV.Data());
    tgen->Add(infgen);
    if (tgen->GetEntries()>0) cout << infgen << ": " << tgen->GetEntries() << endl;
    else { cout << infgen << " has 0 entries" << endl; exit(1); }
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
  //cut.TrkEtaMax = 1;
  cut.ConeSize = 0.3;
  if (!cut.doSel) outFileName = Form("histntff_%s.root",tag.Data());

  // ===================================
  // Correction
  // ===================================
  Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_tev6","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_tev6","hitrkEffAnalyzer_akpu3pf_j1");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_tev7genjet","hitrkEffAnalyzer_akpu3pf_j1");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_matthptv1","hitrkEffAnalyzer_akpu3pf_j1");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_tev7genjet","hiHighPtTrkEffAnalyzer_akpu3pf_j1");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_ppv1","hitrkEffAnalyzer_akpu3pf_j1");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_ppv1","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_pphptv1","hitrkEffAnalyzer_akpu3pf_j1");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_pphptv1","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_tev5","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ1("trkhist_mar292011_jet_v1_hydjetBass_dijet","_nq_jettrkonly_slead","hitrkEffAnalyzer");
  //trkCorrJ1.ptHatMin_.clear();
  //trkCorrJ1.ptHatMin_.push_back(80);
  trkCorrJ1.isLeadingJet_ = true;
  trkCorrJ1.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ1.smoothLevel_ = 4; // 0: no smooth, 1: smooth jet, 2: smooth jet,pt 3: smooth jet,pt,cbin
  trkCorrJ1.Init();
  //Corrector3D trkCorrJ2("trkhist_mar292011_jet_v1_hydjetBass_dijet","_nq_jettrkonly_lead","hitrkEffAnalyzer");
  Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_tev6","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_tev7genjet","hitrkEffAnalyzer_akpu3pf_j2");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_tev6","hitrkEffAnalyzer_akpu3pf_j2");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_matthptv1","hitrkEffAnalyzer_akpu3pf_j2");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_tev7genjet","hiHighPtTrkEffAnalyzer_akpu3pf_j2");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_ppv1","hitrkEffAnalyzer_akpu3pf_j2");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_ppv1","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_pphptv1","hitrkEffAnalyzer_akpu3pf_j2");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_pphptv1","hitrkEffAnalyzer_akpu3pf");
  //Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_tev5","hitrkEffAnalyzer_akpu3pf");
  //trkCorrJ2.ptHatMin_.clear();
  //trkCorrJ2.ptHatMin_.push_back(80);
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
  recfana.ptHatMin_ = ptHatMin;
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
  // gen partilce
  FragAnaLoop genfana("Gen");
  if (doMC==1) {
    genfana.ptHatMin_ = ptHatMin;
    genfana.anaTrkType_=0;
    genfana.t_ = tgen;
    genfana.cut_ = &cut;
    //genfana.vtrkCorr_[0] = &trkCorr;
    genfana.vtrkCorr_[0] = &trkCorrJ1;
    genfana.vtrkCorr_[1] = &trkCorrJ2;
    genfana.Init();
    genfana.Loop();
    // normalize and get ratio
    for (Int_t j=0; j<2; ++j) {
      for (Int_t lv=0; lv<=2; ++lv) {
	if ((!doTrkCorr||particleType!=2) && lv>0) continue;
	recfana.vhXiRat_[j][lv]->Divide(recfana.vhXiCorr_[j][lv],genfana.vhXiCorr_[j][0]);
      }
    }
  }

  // ===================================
  // Plot
  // ===================================
  cout << "==============" << endl;
  cout << "Now plot" << endl;
  cout << "==============" << endl;
  Int_t colors[10] = {kGray+2,kViolet,kBlue+1,kGreen+2,kOrange+2,kMagenta,kRed};
  if (doMC) {
    for (Int_t j=0; j<2; ++j) {
      TString append(Form("_j%d_%s",j,tag.Data()));
      // prepare histogram frame
      genfana.vhXiCorr_[j][0]->SetAxisRange(-1,5,"X");
      genfana.vhXiCorr_[j][0]->SetAxisRange(1e-3,5e2,"Y");
      genfana.vhXiCorr_[j][0]->SetLineColor(kRed);
      genfana.vhXiCorr_[j][0]->SetMarkerColor(kRed);
      genfana.vhXiCorr_[j][0]->SetMarkerStyle(kOpenCircle);
      for (Int_t lv=0; lv<=1; ++lv) {
	if ((!doTrkCorr||particleType!=2) && lv>0) continue;
	recfana.vhXiCorr_[j][lv]->SetMarkerStyle(kOpenSquare);
	recfana.vhXiCorr_[j][lv]->SetMarkerColor(colors[lv]);
	recfana.vhXiCorr_[j][lv]->SetLineColor(colors[lv]);
	recfana.vhXiRat_[j][lv]->SetMarkerStyle(kOpenSquare);
	recfana.vhXiRat_[j][lv]->SetMarkerColor(colors[lv]);
	recfana.vhXiRat_[j][lv]->SetLineColor(colors[lv]);
      }

      // draw spectrum
      TCanvas *c2 = new TCanvas("c2"+append,"closure"+append,500,900);
      c2->Divide(1,2);
      c2->cd(1);
      c2->GetPad(1)->SetLogy();
      genfana.vhXiCorr_[j][0]->Draw("hist");
      recfana.vhXiCorr_[j][0]->Draw("sameE");
      for (Int_t lv=1; lv<=2; ++lv) {
	if ((!doTrkCorr||particleType!=2) && lv>0) continue;
	if (lv<2) continue;
	recfana.vhXiCorr_[j][lv]->Draw("sameE");
      }
      // draw ratio
      c2->cd(2);
      recfana.vhXiRat_[j][0]->Draw("E");
      recfana.vhXiRat_[j][0]->SetAxisRange(-1,5,"X");
      recfana.vhXiRat_[j][0]->SetAxisRange(0,1.5,"Y");
      for (Int_t lv=1; lv<=2; ++lv) {
	if ((!doTrkCorr||particleType!=2) && lv>0) continue;
	if (lv<2) continue;
	recfana.vhXiRat_[j][lv]->Draw("sameE");
      }

      // ====================
      TLine *l = new TLine(0,1,5,1);
      l->SetLineStyle(2);
      l->Draw();

      //TString corrLevelName[5] = { "Raw","Eff","Fake","Mul. Rec","Full" };
      TString corrLevelName[3] = { "Raw","Eff","Full" };
      c2->cd(1);
      TLegend *leg = new TLegend(0.61,0.75,0.91,0.91);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.035);
      leg->AddEntry(genfana.vhXiCorr_[0][0],Form("0-30%, |#eta^{Jet}|<%.1f",cut.JEtaMax[0]),"");
      leg->AddEntry(genfana.vhXiCorr_[0][0],"Gen. Particle","pl");
      for (Int_t lv=0; lv<=2; ++lv) {
	if ((!doTrkCorr||particleType!=2) && lv>0) continue;
	if (lv<2) continue;
	leg->AddEntry(recfana.vhXiCorr_[j][lv],"Trk Corr. "+corrLevelName[lv],"pl");
      }
      leg->Draw();

      c2->Print(Form("ClosureXi%s.gif",append.Data()));
      c2->Print(Form("ClosureXi%s.pdf",append.Data()));

      TCanvas * chkaj = new TCanvas("chkaj"+append,"aj"+append,500,500);
      genfana.vhJetAj_[j]->Draw("hist");
      recfana.vhJetAj_[j]->Draw("sameE");
    }
  }

  // ===================================
  // All Done, Write output if choose
  // ===================================
  /*
  if (cut.doSel) {
    outf->Delete(recfana.tout_->GetName());
    if (doMC==1) {
      outf->Delete(genfana.tout_->GetName());
    }
  }
  */
  outf->Write();
}
