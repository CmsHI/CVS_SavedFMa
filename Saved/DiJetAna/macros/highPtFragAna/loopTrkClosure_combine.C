#include "TChain.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TProfile.h"
#include "FragAnaLoop.h"

void loopTrkClosure_combine(
    TString infrec="nt_djhp_HyUQ110v0_djcalo_100_50_offset0.root",
    TString infgen="nt_djhp_HyUQ110v0_djcalo_genp_100_50_offset0.root",
    TCut evtCut="cent<30")
{
  TChain * trec = new TChain("tjttrk");
  trec->Add(infrec);
  TChain * tgen = new TChain("tjttrk");
  tgen->Add(infgen);
  cout << infrec << " cut " << TString(evtCut) << ": " << trec->GetEntries() << endl;
  cout << infgen << " cut " << TString(evtCut) << ": " << tgen->GetEntries() << endl;

  // Correction
  Corrector trkCorr;
  TH1D * hPtBinUnRebin = (TH1D*)trkCorr.ptBin_->Clone("hPtBinUnRebin");
  trkCorr.ptRebinFactor_ = 12;
  trkCorr.sampleMode_ = 1; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.smoothLevel_ = 1; // 0: no smooth, 1: smooth jet, 2: smooth jet,eta
  trkCorr.Init(1,"TrkCorr2D.root");

  // bins
  //const Int_t numPPtBins=19;
  //Float_t pptBins[numPPtBins+1] = {0.0,0.2,1,2,3,4,6,8,10,14,18,22,26,30,40,50,60,70,80,100};
  //TH1D * hxbin = new TH1D("hxbin","",numPPtBins,pptBins);
  TH1D * hxbin = (TH1D*)hPtBinUnRebin->Clone("hxbin");
  hxbin->Rebin(4);
  vector<Double_t> ptBin;
  for (Int_t i=1; i<=hxbin->GetNbinsX()+1; ++i) {
    ptBin.push_back(hxbin->GetBinLowEdge(i));
  }
  cout << "pt bins: ";
  for (Int_t i=0; i<ptBin.size(); ++i) {cout << ptBin[i] << " ";}
  cout << endl;

  // Ana
  TFile * outf = new TFile("testLoopHists.root","RECREATE");
  // rec trk
  FragAnaLoop recfana("Rec");
  recfana.anaTrkType_=2;
  recfana.t_ = trec;
  recfana.trkCorr_ = &trkCorr;
  recfana.ptBin_ = ptBin;
  recfana.Init();
  recfana.Loop();
  // gen partilce
  FragAnaLoop genfana("Gen");
  genfana.anaTrkType_=0;
  genfana.t_ = tgen;
  genfana.trkCorr_ = &trkCorr;
  genfana.ptBin_ = ptBin;
  genfana.Init();
  genfana.Loop();

  // get ratio
  for (Int_t j=0; j<2; ++j) {
    for (Int_t lv=0; lv<=4; ++lv) {
      recfana.vhPPtRat_[j][lv]->Divide(recfana.vhPPtCorr_[j][lv],genfana.vhPPtCorr_[j][0]);
    }
  }
  // ===================================
  // Plot
  // ===================================
  Int_t colors[10] = {kBlack,kGray+2,kViolet,kBlue+1,kGreen+2,kOrange+2,kMagenta,kRed};
  for (Int_t j=0; j<2; ++j) {
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
  }
  TCanvas *c2 = new TCanvas("c2","closure",500,900);
  c2->Divide(1,2);
  c2->cd(1);
  c2->GetPad(1)->SetLogy();
  c2->GetPad(1)->SetLogx();
  genfana.vhPPtCorr_[0][0]->Draw("hist");
  recfana.vhPPtCorr_[0][0]->Draw("sameE");
  for (Int_t lv=1; lv<=4; ++lv) {
    recfana.vhPPtCorr_[0][lv]->Draw("sameE");
  }
  c2->cd(2);
  c2->GetPad(2)->SetLogx();
  recfana.vhPPtRat_[0][0]->Draw("E");
  recfana.vhPPtRat_[0][0]->SetAxisRange(4,100,"X");
  recfana.vhPPtRat_[0][0]->SetAxisRange(0,1.5,"Y");
  for (Int_t lv=1; lv<=4; ++lv) {
    recfana.vhPPtRat_[0][lv]->Draw("sameE");
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
    leg->AddEntry(recfana.vhPPtCorr_[0][lv],"Trk Corr. "+corrLevelName[lv],"pl");
  }
  leg->Draw();

  c2->Print("ClosureTrkPt.gif");
  c2->Print("ClosureTrkPt.pdf");

  // ====================
  TCanvas * chk0 = new TCanvas("chk0","check eff",500,500);
  chk0->SetLogz();
  chk0->SetRightMargin(0.15);
  recfana.vhTrkCorrPPt_[0][1]->Draw("colz");
  recfana.vhTrkCorrPPt_[0][1]->ProfileX()->Draw("same");
  //trkCorr.InspectCorr(0,-1,0,-1,2,7-2,7+2,"histsame");
  chk0->Print("ChkTrkEffPPt.gif");

  TCanvas * chk1 = new TCanvas("chk1","check fake",500,500);
  chk1->SetLogz();
  chk1->SetRightMargin(0.15);
  recfana.vhTrkCorrPPt_[0][2]->Draw("colz");
  recfana.vhTrkCorrPPt_[0][2]->ProfileX()->Draw("same");
  //trkCorr.InspectCorr(1,-1,0,-1,2,7-2,7+2,"histsame");
  chk1->Print("ChkTrkFakPPt.gif");
}
