#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>

void compPtHat()
{
  TCut jetSelCut[2] = {"",""};

  multiTreeUtil vmt[2];
  // measurement in MC
  // smeared pp
  // vmt[0].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
  // vmt[0].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut
  // unsmeared pp
  // vmt[0].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
  // vmt[0].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut
  // pbpb
  TCut centCut = "cBin>=0&&cBin<40";
  vmt[0].addFile("../ntout/jskim_hydj80_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[0] && centCut && "pthat>=80&&pthat<100",(9.913e-5)*1e9);
  vmt[0].addFile("../ntout/jskim_hydj100_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[0] && centCut && "pthat>=100&&pthat<9999",(3.069e-5)*1.e9);
  // vmt[0].addFile("../ntout/jskim_hydj120_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[0] && centCut && "pthat>=120&&pthat<9999",1.128e-5*1.e9);

  // reference
  // unsmeared pp
  // vmt[1].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[1] && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
  // vmt[1].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[1] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut
  // smeared pp
  vmt[1].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root","tdj", jetSelCut[1] && "pthat>=80&&pthat<120", (9.913e-5)*1.e9); // no centrality cut
  vmt[1].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root","tdj", jetSelCut[1] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut

  for (int i=0; i<2; ++i) {
    vmt[i].NormalizeScales(2,"","hEvtCentNoSkim");
  }

  TFile * ouf = new TFile("hists.root","recreate");

  // Draw jet
  TH1D * hJetPt[2];
  for (int i=0; i<2; ++i) {
    hJetPt[i] = new TH1D(Form("hJetPt_%d",i),";#hat{p}_{T} (GeV/c);",80,0,400);
    vmt[i].Draw(hJetPt[i],"pthat","","1");
  }

  // Check Normalization
  TCanvas * c0 = new TCanvas("c0","c0",400,650,600,300);
  c0->Divide(2,1);
  c0->cd(1);
  gPad->SetLogy();
  cleverRange(hJetPt[0],hJetPt[1],5);
  hJetPt[0]->DrawCopy("E");
  hJetPt[1]->DrawCopy("same hist");
  c0->cd(2);
  TH1D * hJetPtUnNormRat = (TH1D*)hJetPt[0]->Clone(Form("%s_unnormrat",hJetPt[0]->GetName()));
  hJetPtUnNormRat->Divide(hJetPt[1]);
  hJetPtUnNormRat->SetAxisRange(0,2,"Y");
  hJetPtUnNormRat->Draw("E");
  jumSun(0,1,400,1);

  // Normalization
  float nJet[2] = {0,0};
  for (int i=0; i<2; ++i) {
    nJet[i] = hJetPt[i]->Integral();
    cout << "(input " << i << ") nJet = " << nJet[i] << endl;
  }

  // Normalize
  for (int i=0; i<2; ++i) {
    hJetPt[i]->Scale(1./nJet[i]);
  }

  // Draw
  TCanvas * c2 = new TCanvas("c2","c2",1000,650,600,300);
  c2->Divide(2,1);
  c2->cd(1);
  gPad->SetLogy();
  hJetPt[0]->Draw("E");
  hJetPt[1]->Draw("same hist");
  c2->cd(2);
  TH1D * hJetPtRat = (TH1D*)hJetPt[0]->Clone(Form("%s_rat",hJetPt[0]->GetName()));
  hJetPtRat->Divide(hJetPt[1]);
  hJetPtRat->SetAxisRange(0,2,"Y");
  hJetPtRat->Draw("E");
  jumSun(0,1,400,1);
}
