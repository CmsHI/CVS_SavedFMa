#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>

void compInclJetFrag()
{
  // TCut jetSelCut[2] = {"jetPt>100&&jetPt<300","jetPt>100&&jetPt<300"};
  // TCut jetSelCut[2] = {"jetPtGM>100&&jetPtGM<300","jetPtGM>100&&jetPtGM<300"};
  TCut jetSelCut[2] = {"jetPt>100&&jetPt<300&&jetPtGM>300","jetPtGM<100&&jetPtGM<300"};
  // TCut jetSelCut[2] = {"jetPtGM<100","jetPtGM>100&&jetPtGM<300"};

  multiTreeUtil vmt[2];
  // vmt[0].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
  // vmt[0].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin1_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut
  vmt[0].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
  vmt[0].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[0] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut

  vmt[1].addFile("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[1] && "pthat>=80&&pthat<120", 9.913e-5*1.e9); // no centrality cut
  vmt[1].addFile("../ntout/jskim_dj120_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root","tdj", jetSelCut[1] && "pthat>=120&&pthat<9999", 1.128e-5*1.e9); // no centrality cut

  // TString jetAlias[2] = {"jetPt","jetPt"};
  // TString jetAlias[2] = {"jetPtGM","jetPtGM"};
  TString jetAlias[2] = {"jetPt","jetPtGM"};
  for (int i=0; i<2; ++i) {
    vmt[i].AddFriend("yTrk=yongsunTrack");
    vmt[i].AddFriend("genPar");
    vmt[i].SetAlias("finalJetPt",jetAlias[i]);
    cout << "(input " << i << ") finalJetPt = " << vmt[i].trees_[0]->GetAlias("finalJetPt") << endl;
  }

  TFile * ouf = new TFile("hists.root","recreate");

  // Draw jet
  TCut jetCut="abs(jetEta)<2&&abs(jetEta)>0.3";
  TH1D * hJetPt[2];
  for (int i=0; i<2; ++i) {
    hJetPt[i] = new TH1D(Form("hJetPt_%d",i),";Jet p_{T} (GeV/c);",20,0,400);
    vmt[i].Draw(hJetPt[i],"finalJetPt",jetCut,"1");
  }

  // Check Normalization
  TCanvas * c0 = new TCanvas("c0","c0",400,650,600,300);
  c0->Divide(2,1);
  c0->cd(1);
  gPad->SetLogy();
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

  // Draw Trk
  // TCut trkCut="yTrk.jetMatch==1 && yTrk.jetDr<0.30 && yTrk.pt>1.00 && yTrk.pt<300.00";
  // TString trkWeight="(yTrk.trkWeight)";
  // TString trkWeight="(yTrk.trkWeight*(yTrk.trkWeight<3.5)+1.47*(yTrk.trkWeight>=3.5))";
  TCut trkCut="abs(genPar.chg)>0 && (genPar.jetMatch==1) && (genPar.jetDr<0.30) && (genPar.pt>1.00 && genPar.pt<300.00)";
  TString trkWeight="1";
  TH1D * hTrkPt[2], * hXi[2];
  for (int i=0; i<2; ++i) {
    hTrkPt[i] = new TH1D(Form("hTrkPt_%d",i),";Track p_{T} (GeV/c);",20,1,101);
    // vmt[i].Draw(hTrkPt[i],"yTrk.pt",jetCut&&trkCut,trkWeight);
    vmt[i].Draw(hTrkPt[i],"genPar.pt",jetCut&&trkCut,trkWeight);
    hXi[i] = new TH1D(Form("hXi_%d",i),";#xi = ln(1/z);",16,-1,7);
    vmt[i].Draw(hXi[i],"log(finalJetPt/genPar.pt)",jetCut&&trkCut,trkWeight);
  }

  // Draw Jet and Trk
  TH2D * hTrkJetPt2D[2], * hXiJetPt2D[2];
  for (int i=0; i<2; ++i) {
    hTrkJetPt2D[i] = new TH2D(Form("hTrkJetPt2D_%d",i),";Track p_{T} (GeV/c);Jet p_{T} (GeV/c);",20,1,101,20,0,400);
    // vmt[i].Draw2D(hTrkJetPt2D[i],"yTrk.pt:finalJetPt",jetCut&&trkCut,trkWeight);
    vmt[i].Draw2D(hTrkJetPt2D[i],"finalJetPt:genPar.pt",jetCut&&trkCut,trkWeight);
    hXiJetPt2D[i] = new TH2D(Form("hXiJetPt2D_%d",i),";#xi = ln(1/z);Jet p_{T} (GeV/c);",16,-1,7,20,0,400);
    vmt[i].Draw2D(hXiJetPt2D[i],"finalJetPt:log(finalJetPt/genPar.pt)",jetCut&&trkCut,trkWeight);
  }

  // Normalize
  for (int i=0; i<2; ++i) {
    hJetPt[i]->Scale(1./nJet[i]);
    hTrkPt[i]->Scale(1./nJet[i]);
    hXi[i]->Scale(1./nJet[i]);
    hTrkJetPt2D[i]->Scale(1./nJet[i]);
    hXiJetPt2D[i]->Scale(1./nJet[i]);
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

  TCanvas * c3 = new TCanvas("c3","c3",100,0,600,300);
  c3->Divide(2,1);
  c3->cd(1);
  gPad->SetLogy();
  gPad->SetLogx();
  hTrkPt[0]->Draw("E");
  hTrkPt[1]->Draw("same hist");
  c3->cd(2);
  gPad->SetLogx();
  TH1D * hTrkPtRat = (TH1D*)hTrkPt[0]->Clone(Form("%s_rat",hTrkPt[0]->GetName()));
  hTrkPtRat->Divide(hTrkPt[1]);
  hTrkPtRat->SetAxisRange(0,2,"Y");
  hTrkPtRat->Draw("E");
  jumSun(1,1,100,1);

  TCanvas * c4 = new TCanvas("c4","c4",700,0,900,300);
  c4->Divide(3,1);
  c4->cd(1);
  gPad->SetRightMargin(0.15);
  gPad->SetLogx();
  gPad->SetLogz();
  hTrkJetPt2D[0]->SetAxisRange(5e-3,1e1,"Z");
  hTrkJetPt2D[0]->Draw("colz");
  c4->cd(2);
  gPad->SetRightMargin(0.15);
  gPad->SetLogx();
  gPad->SetLogz();
  hTrkJetPt2D[1]->Draw("colz");
  hTrkJetPt2D[1]->SetAxisRange(5e-3,1e1,"Z");
  c4->cd(3);
  gPad->SetRightMargin(0.15);
  gPad->SetLogx();
  TH1D * hTrkJetPt2DRat = (TH1D*)hTrkJetPt2D[0]->Clone(Form("%s_rat",hTrkJetPt2D[0]->GetName()));
  hTrkJetPt2DRat->Divide(hTrkJetPt2D[1]);
  hTrkJetPt2DRat->SetAxisRange(0.4,2,"Z");
  hTrkJetPt2DRat->Draw("colz");


  TCanvas * c5 = new TCanvas("c5","c5",100,325,600,300);
  c5->Divide(2,1);
  c5->cd(1);
  gPad->SetLogy();
  hXi[0]->Draw("E");
  hXi[1]->Draw("same hist");
  c5->cd(2);
  TH1D * hXiRat = (TH1D*)hXi[0]->Clone(Form("%s_rat",hXi[0]->GetName()));
  hXiRat->Divide(hXi[1]);
  hXiRat->SetAxisRange(0,2,"Y");
  hXiRat->Draw("E");
  jumSun(0,1,6,1);

  TCanvas * c6 = new TCanvas("c6","c6",700,325,900,300);
  c6->Divide(3,1);
  c6->cd(1);
  gPad->SetRightMargin(0.15);
  gPad->SetLogz();
  hXiJetPt2D[0]->SetAxisRange(2e-6,5,"Z");
  hXiJetPt2D[0]->Draw("colz");
  c6->cd(2);
  gPad->SetRightMargin(0.15);
  gPad->SetLogz();
  hXiJetPt2D[1]->Draw("colz");
  hXiJetPt2D[1]->SetAxisRange(2e-6,5,"Z");
  c6->cd(3);
  gPad->SetRightMargin(0.15);
  TH1D * hXiJetPt2DRat = (TH1D*)hXiJetPt2D[0]->Clone(Form("%s_rat",hXiJetPt2D[0]->GetName()));
  hXiJetPt2DRat->Divide(hXiJetPt2D[1]);
  hXiJetPt2DRat->SetAxisRange(0.4,2,"Z");
  hXiJetPt2DRat->Draw("colz");

  // Check
  // TCanvas * cc = new TCanvas("cc","cc",2);
  // hTrkPt[0]->Draw("hist");
  // hTrkJetPt2D[0]->ProjectionY(Form("%s_py",hTrkJetPt2D[0]->GetName()))->Draw("sameE");
  // hJetPt[0]->Scale(1./nJet[0]);
  // hJetPt[0]->Draw("hist");
  // hTrkJetPt2D[0]->ProjectionX(Form("%s_px",hTrkJetPt2D[0]->GetName()))->Draw("sameE");
}
