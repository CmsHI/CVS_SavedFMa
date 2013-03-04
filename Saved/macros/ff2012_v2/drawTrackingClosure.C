#include "CutAndBinCollection2011.h"
#include "HisMath.C"
#include <TRandom3.h>
#include <time.h>

// const int nPtBin = 25;
// double ptBin[nPtBin+1] = {1, 1.203915, 1.449412, 1.74497, 2.100796, 2.529181, 3.04492, 3.665826, 4.413344, 5.313293, 6.396755, 7.701152, 9.271536, 11.16214, 13.43828, 16.17855, 19.47761, 23.44939, 28.23108, 33.98783, 40.91848, 49.26238, 59.30774, 71.40151, 85.96137, 103.4902}; 
const int nPtBin = 18;
double ptBin[nPtBin+1] = {1,1.2,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};

void drawTrackingClosure()
{
  TCut centCut = "cBin>=0&&cBin<4";
  TCut jetSelCut[2] = {"jetPt>100&&jetPt<300","jetPt>100&&jetPt<300"};
  // TCut jetSelCut[2] = {"jetUnSmPt>100&&jetUnSmPt<300","jetUnSmPt>100&&jetUnSmPt<300"};
  TString jetAlias[2] = {"jetPt","jetPt"};
  // TString jetAlias[2] = {"jetUnSmPt","jetUnSmPt"};

  multiTreeUtil vmt[2];

  // Mar 04
  // hi
  vmt[0].addFile("../ntout/jskim_hydj80_akPu3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[0] && centCut && "pthat>=80&&pthat<100",9.913e-5*1.e9);
  vmt[0].addFile("../ntout/jskim_hydj100_akPu3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[0] && centCut && "pthat>=100&&pthat<170",3.069e-5*1.e9);
  vmt[0].addFile("../ntout/jskim_hydj170_akPu3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[0] && centCut && "pthat>=170&&pthat<9999",1.470e-6*1.e9);
  vmt[1].addFile("../ntout/jskim_hydj80_akPu3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[1] && centCut && "pthat>=80&&pthat<100",9.913e-5*1.e9);
  vmt[1].addFile("../ntout/jskim_hydj100_akPu3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[1] && centCut && "pthat>=100&&pthat<170",3.069e-5*1.e9);
  vmt[1].addFile("../ntout/jskim_hydj170_akPu3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root", "tdj", jetSelCut[1] && centCut && "pthat>=170&&pthat<9999",1.470e-6*1.e9);
  // pp
  // vmt[0].addFile("../ntout/jskim_dj80_ak3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut[0] && "pthat>=80&&pthat<120",9.913e-5*1.e9);
  // vmt[0].addFile("../ntout/jskim_dj120_ak3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut[0] && "pthat>=120&&pthat<170",1.128e-5*1.e9);
  // vmt[0].addFile("../ntout/jskim_dj170_ak3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut[0] && "pthat>=170&&pthat<9999",1.470e-6*1.e9);
  // vmt[1].addFile("../ntout/jskim_dj80_ak3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut[1] && "pthat>=80&&pthat<120",9.913e-5*1.e9);
  // vmt[1].addFile("../ntout/jskim_dj120_ak3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut[1] && "pthat>=120&&pthat<170",1.128e-5*1.e9);
  // vmt[1].addFile("../ntout/jskim_dj170_ak3PF_Mar04job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm0bin0_ak3PF_gj0.root", "tdj", jetSelCut[1] && "pthat>=170&&pthat<9999",1.470e-6*1.e9);

  for (int i=0; i<2; ++i) {
    vmt[i].NormalizeScales(2,"","hEvtCentNoSkim");
  }

  TString annotation="#DeltaR<0.3";
  TString tag="hiMar04job0trkcorr14d_dr3_limcorr";
  // TString tag="ppMar04job0trkcorr14d_dr3_limcorr";

  for (int i=0; i<2; ++i) {
    vmt[i].AddFriend("yTrk=yongsunTrack");
    vmt[i].AddFriend("genPar");
    vmt[i].SetAlias("finalJetPt",jetAlias[i]);
    cout << "(input " << i << ") finalJetPt = " << vmt[i].trees_[0]->GetAlias("finalJetPt") << endl;
  }

  TFile * ouf = new TFile("hists.root","recreate");

  // Draw jet
  // TCut jetCut="abs(jetEta)<2";
  TCut jetCut="abs(jetEta)<2&&abs(jetEta)>0.3";
  TH1D * hJetPt[2];
  for (int i=0; i<2; ++i) {
    hJetPt[i] = new TH1D(Form("hJetPt_%d",i),";Jet p_{T} (GeV/c);",20,0,400);
    vmt[i].Draw(hJetPt[i],"finalJetPt",jetCut,"1");
  }

  // Check Normalization
  TCanvas * c0 = new TCanvas("c0","c0",805,0,800,400);
  c0->Divide(2,1);
  c0->cd(1);
  gPad->SetLogy();
  cleverRange(hJetPt[0],hJetPt[1],5);
  handsomeTH1(hJetPt[0],kBlack);
  handsomeTH1(hJetPt[1],kRed);
  hJetPt[0]->SetYTitle("# of Jets");
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
  float xminpt=1, xmaxpt=119;
  TString trkVar[2]={"yTrk.pt","genPar.pt"};
  TString xiVar[2]={"log(finalJetPt/yTrk.pt)","log(finalJetPt/genPar.pt)"};
  // TCut trkCut[2]={
  //   "yTrk.jetMatch==1 && yTrk.jetDr<0.30 && yTrk.pt>1.00 && yTrk.pt<300.00 && yTrk.trkWeight<10",
  //   "abs(genPar.chg)>0 && genPar.jetMatch==1 && genPar.jetDr<0.30 && genPar.pt>1.00 && genPar.pt<300.00"
  // };
  // Signal Cone
  TCut trkCut[2]={"yTrk.jetMatch==1&&yTrk.jetDr<0.30","abs(genPar.chg)>0 && genPar.jetMatch==1 && genPar.jetDr<0.30"};
  // Bkg Cone
  // TCut trkCut[2]={"yTrk.jetMatch==-1&&yTrk.jetDr<0.30","abs(genPar.chg)>0 && genPar.jetMatch==-1 && genPar.jetDr<0.30"};
  // trkCut[1]=trkCut[1]&&"genPar.sube==0";
  // TString trkWeight[2]={"(yTrk.trkWeight)","1"};
  // TString trkWeight[2]={"((yTrk.trkWeight)*(1-yTrk.jetDr/0.3*0.08*(yTrk.jetDr<0.3)))","1"};
  TString trkWeight[2]={"(yTrk.trkWeight*(yTrk.trkWeight<20)+1.47*(yTrk.trkWeight>=20))","1"};
  TH1D * hTrkPt[2], * hXi[2];
  for (int i=0; i<2; ++i) {
    hTrkPt[i] = new TH1D(Form("hTrkPt_%d",i),";Track p_{T} (GeV/c);",nPtBin,ptBin);
    vmt[i].Draw(hTrkPt[i],trkVar[i],jetCut&&trkCut[i],trkWeight[i]);
    hXi[i] = new TH1D(Form("hXi_%d",i),";#xi = ln(1/z);",16,-1,7);
    vmt[i].Draw(hXi[i],xiVar[i],jetCut&&trkCut[i],trkWeight[i]);
  }

  // Normalize
  for (int i=0; i<2; ++i) {
    hJetPt[i]->Scale(1./nJet[i]);
    hJetPt[i]->SetYTitle("Jet Fraction");
    normHist(hTrkPt[i],0,true,1./nJet[i]);
    normHist(hXi[i],0,true,1./nJet[i]);
  }

  /////////////////////////////////////
  // Draw Jet
  /////////////////////////////////////
  TCanvas * c2 = new TCanvas("c2","c2",805,425,800,400);
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

  /////////////////////////////////////
  // Draw Track Pt
  /////////////////////////////////////
  TCanvas * c3 = new TCanvas("c3","c3",10,0,800,400);
  c3->Divide(2,1);
  c3->cd(1);
  gPad->SetLogy();
  gPad->SetLogx();
  handsomeTH1(hTrkPt[0],kBlack);
  handsomeTH1(hTrkPt[1],kRed);
  hTrkPt[0]->SetTitle(";p_{T}^{ Track} (GeV/c);1/N_{ jet} dN_{ Track} /dp_{T}");
  hTrkPt[0]->Draw("E");
  hTrkPt[1]->Draw("same hist");

  TLegend * l1 = new TLegend(0.35,0.19,0.78,0.40,NULL,"brNDC");
  easyLeg(l1,"",20);
  l1->AddEntry(hTrkPt[0],annotation,"");
  l1->AddEntry(hTrkPt[0],"Rec. Tracks","p");
  l1->AddEntry(hTrkPt[1],"Gen. Partls","l");
  l1->Draw();

  c3->cd(2);
  gPad->SetLogx();
  TH1D * hTrkPtRat = (TH1D*)hTrkPt[0]->Clone(Form("%s_rat",hTrkPt[0]->GetName()));
  hTrkPtRat->Divide(hTrkPt[1]);
  hTrkPtRat->SetAxisRange(xminpt,xmaxpt,"X");
  hTrkPtRat->SetAxisRange(0.5,1.5,"Y");
  hTrkPtRat->SetYTitle("Ratio");
  hTrkPtRat->Draw("E");
  jumSun(xminpt,1,xmaxpt,1,kRed);
  c3->Print(Form("closure/trackingClosure_pt_%s.gif",tag.Data()));
  c3->Print(Form("closure/trackingClosure_pt_%s.pdf",tag.Data()));

  /////////////////////////////////////
  // Draw Xi
  /////////////////////////////////////
  TCanvas * c5 = new TCanvas("c5","c5",10,425,800,400);
  c5->Divide(2,1);
  c5->cd(1);
  gPad->SetLogy();
  handsomeTH1(hXi[0],kBlack);
  handsomeTH1(hXi[1],kRed);
  hXi[0]->SetTitle(";#xi = ln(1/z);1/N_{ jet} dN_{ track} /d#xi");
  hXi[0]->Draw("E");
  hXi[1]->Draw("same hist");
  l1->Draw();
  c5->cd(2);
  TH1D * hXiRat = (TH1D*)hXi[0]->Clone(Form("%s_rat",hXi[0]->GetName()));
  hXiRat->Divide(hXi[1]);
  hXiRat->SetAxisRange(0.5,1.5,"Y");
  hXiRat->SetYTitle("Ratio");
  hXiRat->Draw("E");
  jumSun(0,1,6,1,kRed);
  c5->Print(Form("closure/trackingClosure_xi_%s.gif",tag.Data()));
  c5->Print(Form("closure/trackingClosure_xi_%s.pdf",tag.Data()));
}
