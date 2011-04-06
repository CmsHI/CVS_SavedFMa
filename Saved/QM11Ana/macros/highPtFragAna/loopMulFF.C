#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TProfile.h"
#include "MulFFAnaLoop.h"

void loopMulFF(Int_t inputSample=0,
    Bool_t doSel=true)
{
  // ===================================
  // Inputs
  // ===================================
  Double_t ptHatMin=110;
  TString infrec,infgen;
  TString tag,outName;
  TChain * trec = new TChain("tjf");
  TChain * tgen = new TChain("tjf");
  if (inputSample==0) {
    infrec=Form("ntout/tranav0_data_akpu3pf.root");
    tag = "CorrDatav0cv3";
  }

  if (inputSample==1) {
    infrec=Form("ntout/trana_hydjuq%.0f_mc_akpu3pf_t2_50k.root",ptHatMin);
    infgen=Form("ntout/trana_hydjuq%.0f_mc_akpu3pf_t0_50k.root",ptHatMin);
    tgen->Add(infgen);
    if (tgen->GetEntries()>0) cout << infgen << ": " << tgen->GetEntries() << endl;
    else { cout << infgen << " has 0 entries" << endl; exit(1); }
    tag = Form("CorrUqv0cv3_%.0f",ptHatMin);
  }

  trec->Add(infrec);
  if (trec->GetEntries()>0) cout << infrec << ": " << trec->GetEntries() << endl;
  else { cout << infrec << " has 0 entries" << endl; exit(1); }
  cout << "Output: " << tag << endl;
  TString outFileName(Form("histff_%s.root",tag.Data()));

  // ===================================
  // Cuts
  // ===================================
  selectionCut cut;
  cut.doSel = doSel;
  cut.CentMin=0;
  cut.CentMax=10;
  cut.JEtMin[0] = 110;
  cut.JEtMax[0] = 250;
  cut.JEtMin[1] = 50;
  cut.JEtMax[1] = 250;
  cut.JEtaMax[0] = 1.6;
  cut.JEtaMax[1] = 1.6;
  cut.TrkPtMin = 4.;
  cut.TrkEtaMax = 1;
  cut.ConeSize = 0.5;
  if (!cut.doSel) outFileName = Form("histntff_%s.root",outName.Data());

  // ===================================
  // Correction
  // ===================================
  Corrector3D trkCorr("trkCorrHisAna_djuq","_jtv2_2_cv3","B2");
  trkCorr.ptRebinFactor_ = 1;
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorr.Init(0);
  //TH1D * hPtBinUnRebin = (TH1D*)trkCorr.ptBin_->Clone("hPtBinUnRebin");
  //trkCorr.Init(1,"TrkCorr2DB0.root");
  Corrector3D trkCorrJ1("trkCorrHisAna_djuq","_jtv2_2_cv3","B2InConeJ1");
  trkCorrJ1.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ1.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorrJ1.Init();
  Corrector3D trkCorrJ2("trkCorrHisAna_djuq","_jtv2_2_cv3","B2InConeJ2");
  trkCorrJ2.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorrJ2.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorrJ2.Init();

  // ===================================
  // Setup
  // ===================================
  // bins
  const Int_t numPPtBins=18;
  Float_t pptBins[numPPtBins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};
  vector<Double_t> ptBin(pptBins,pptBins+numPPtBins+1);
  //TH1D * hxbin = new TH1D("hxbin","",numPPtBins,pptBins);
  //TH1D * hxbin = (TH1D*)hPtBinUnRebin->Clone("hxbin");
  /*
  TH1D * hxbin = (TH1D*)trkCorr.ptBin_->Clone("hxbin");
  hxbin->Rebin(2);
  vector<Double_t> ptBin;
  for (Int_t i=1; i<=hxbin->GetNbinsX()+1; ++i) {
    ptBin.push_back(hxbin->GetBinLowEdge(i));
  }
  */
  cout << ptBin.size()-1 << " pt bins: ";
  for (Int_t i=0; i<ptBin.size(); ++i) {cout << ptBin[i] << " ";}
  cout << endl;

  // ===================================
  // Analyze
  // ===================================
  TFile * outf = new TFile(outFileName,"RECREATE");
  // rec trk
  FragAnaLoop recfana("Rec");
  recfana.ptHatMin_ = ptHatMin;
  recfana.anaTrkType_=2;
  recfana.t_ = trec;
  recfana.cut_ = &cut;
  //recfana.vtrkCorr_[0] = &trkCorr;
  recfana.vtrkCorr_[0] = &trkCorrJ1;
  recfana.vtrkCorr_[1] = &trkCorrJ2;
  recfana.ptBin_ = ptBin;
  recfana.Init();
  recfana.Loop();
  // gen partilce
  FragAnaLoop genfana("Gen");
  if (inputSample==1) {
    genfana.ptHatMin_ = ptHatMin;
    genfana.anaTrkType_=0;
    genfana.t_ = tgen;
    genfana.cut_ = &cut;
    //genfana.vtrkCorr_[0] = &trkCorr;
    genfana.vtrkCorr_[0] = &trkCorrJ1;
    genfana.vtrkCorr_[1] = &trkCorrJ2;
    genfana.ptBin_ = ptBin;
    genfana.Init();
    genfana.Loop();
    // get ratio
    for (Int_t j=0; j<2; ++j) {
      for (Int_t lv=0; lv<=4; ++lv) {
	recfana.vhPPtRat_[j][lv]->Divide(recfana.vhPPtCorr_[j][lv],genfana.vhPPtCorr_[j][0]);
      }
    }
  }

  // ===================================
  // Plot
  // ===================================
  /*
  Int_t colors[10] = {kGray+2,kViolet,kBlue+1,kGreen+2,kOrange+2,kMagenta,kRed};
  for (Int_t j=0; j<2; ++j) {
    TString append(Form("_j%d_%s",j,outName.Data()));
    // prepare histogram frame
    genfana.vhPPtCorr_[j][0]->SetAxisRange(4,100,"X");
    genfana.vhPPtCorr_[j][0]->SetAxisRange(1e-3,5e1,"Y");
    genfana.vhPPtCorr_[j][0]->SetLineColor(kRed);
    genfana.vhPPtCorr_[j][0]->SetMarkerColor(kRed);
    genfana.vhPPtCorr_[j][0]->SetMarkerStyle(kOpenCircle);
    for (Int_t lv=0; lv<=3; ++lv) {
      recfana.vhPPtCorr_[j][lv]->SetMarkerStyle(kOpenSquare);
      recfana.vhPPtCorr_[j][lv]->SetMarkerColor(colors[lv]);
      recfana.vhPPtCorr_[j][lv]->SetLineColor(colors[lv]);
      recfana.vhPPtRat_[j][lv]->SetMarkerStyle(kOpenSquare);
      recfana.vhPPtRat_[j][lv]->SetMarkerColor(colors[lv]);
      recfana.vhPPtRat_[j][lv]->SetLineColor(colors[lv]);
    }

    // draw spectrum
    TCanvas *c2 = new TCanvas("c2"+append,"closure"+append,500,900);
    c2->Divide(1,2);
    c2->cd(1);
    c2->GetPad(1)->SetLogy();
    c2->GetPad(1)->SetLogx();
    genfana.vhPPtCorr_[j][0]->Draw("hist");
    recfana.vhPPtCorr_[j][0]->Draw("sameE");
    for (Int_t lv=1; lv<=4; ++lv) {
      recfana.vhPPtCorr_[j][lv]->Draw("sameE");
    }
    // draw ratio
    c2->cd(2);
    c2->GetPad(2)->SetLogx();
    recfana.vhPPtRat_[j][0]->Draw("E");
    recfana.vhPPtRat_[j][0]->SetAxisRange(4,100,"X");
    recfana.vhPPtRat_[j][0]->SetAxisRange(0,1.5,"Y");
    for (Int_t lv=1; lv<=4; ++lv) {
      recfana.vhPPtRat_[j][lv]->Draw("sameE");
    }

    // ====================
    TLine *l = new TLine(4,1,100,1);
    l->SetLineStyle(2);
    l->Draw();

    TString corrLevelName[5] = { "Raw","Eff","Fake","Mul. Rec","Full" };
    c2->cd(1);
    TLegend *leg = new TLegend(0.61,0.57,0.91,0.91);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.035);
    //leg->AddEntry(genfana.vhPPtCorr_[0][0],"#DeltaR(jet,trk)<0.5","");
    leg->AddEntry(genfana.vhPPtCorr_[0][0],"Gen. Particle","pl");
    for (Int_t lv=0; lv<=4; ++lv) {
      leg->AddEntry(recfana.vhPPtCorr_[j][lv],"Trk Corr. "+corrLevelName[lv],"pl");
    }
    leg->Draw();

    c2->Print(Form("ClosureTrkPt%s.gif",append.Data()));

    // ====================
    TCanvas * chk0 = new TCanvas("chk0"+append,"eff_vs_pt"+append,500,500);
    chk0->SetLogz();
    chk0->SetRightMargin(0.15);
    recfana.vhTrkCorrPPt_[j][1]->Draw("colz");
    recfana.vhTrkCorrPPt_[j][1]->ProfileX()->Draw("same");
    //trkCorr.InspectCorr(0,-1,0,-1,2,7-2,7+2,"histsame");
    chk0->Print(Form("AppliedTrkEffPPt%s.gif",append.Data()));

    TCanvas * chk1 = new TCanvas("chk1"+append,"fake_vs_pt"+append,500,500);
    chk1->SetLogz();
    chk1->SetRightMargin(0.15);
    recfana.vhTrkCorrPPt_[j][2]->Draw("colz");
    recfana.vhTrkCorrPPt_[j][2]->ProfileX()->Draw("same");
    //trkCorr.InspectCorr(1,-1,0,-1,2,7-2,7+2,"histsame");
    chk1->Print(Form("AppliedTrkFakPPt%s.gif",append.Data()));

    TCanvas * cCorrJEt = new TCanvas("cCorrJEt"+append,"eff_vs_jet"+append,500,500);
    cCorrJEt->SetLogz();
    cCorrJEt->SetRightMargin(0.15);
    recfana.vhTrkCorrJEt_[j][1]->Draw("colz");
    recfana.vhTrkCorrJEt_[j][1]->ProfileX()->Draw("same");
    //trkCorr.InspectCorr(0,-1,0,-1,2,7-2,7+2,"histsame");
    cCorrJEt->Print(Form("AppliedTrkEffJEt%s.gif",append.Data()));

    TCanvas * cCorrCent = new TCanvas("cCorrCent"+append,"eff_vs_cent"+append,500,500);
    cCorrCent->SetLogz();
    cCorrCent->SetRightMargin(0.15);
    recfana.vhTrkCorrCent_[j][1]->Draw("colz");
    recfana.vhTrkCorrCent_[j][1]->ProfileX()->Draw("same");
    //trkCorr.InspectCorr(0,-1,0,-1,2,7-2,7+2,"histsame");
    cCorrCent->Print(Form("AppliedTrkEffCent%s.gif",append.Data()));
  }
*/

  // ===================================
  // All Done, Write output if choose
  // ===================================
  recfana.tout_->Write();
  if (inputSample==1) {
    genfana.tout_->Write();
  }
  //if (!cut.doSel) recfana.ntCorr_->Write();
  //else outf->Clear();
}
