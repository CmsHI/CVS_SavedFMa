#include "CutAndBinCollection2011.h"
#include <TRandom3.h>
#include <time.h>
class AnaTH2{
public:
  // memember data
  TString name;
  int normMode;
  TH2D * h2;
  int nbinx;
  TH1D * vh1[100];
  TH1D * hymean;
  TH1D * hyrms;

  AnaTH2(TString nm="")
  : name(nm)
  , normMode(1)
  , nbinx(0)
  {}

  void Init(TH2D * h) {
    // cout << h << " " << h->GetName() << endl;
    h2 = h;
    if (name=="") name = h2->GetName();
    nbinx = h2->GetNbinsX();
    cout << name << " nbinx: " << nbinx << endl;
    for (int i=1; i<=nbinx; ++i) {
      TString hname = Form("%s_xbin%d",name.Data(),i);
      vh1[i] = h2->ProjectionY(hname,i,i,"e");
      if (normMode==1) vh1[i]->Scale(1./vh1[i]->Integral());
    }
  }

  void GetSummary() {
    hymean = h2->ProjectionX(Form("%s_ymean",name.Data()));
    hymean->Reset();
    hyrms = (TH1D*)hymean->Clone( Form("%s_yrms",name.Data()) );
    for (int i=1; i<=hymean->GetNbinsX(); ++i) {
      hymean->SetBinContent(i,vh1[i]->GetMean());
      hymean->SetBinError(i,vh1[i]->GetMeanError());
      hyrms->SetBinContent(i,vh1[i]->GetRMS());
      hyrms->SetBinError(i,vh1[i]->GetRMSError());
    }
  }
};

void compConeFluc(
  int anamode=10 // 0=signal cone, 10=bkgcone
)
{
  TString AnaTitle = "Bkg";
  if (anamode==0) AnaTitle = "Jet";

  TCut jetSelCut[2] = {"jetPt>100&&jetPt<300","jetPt>100&&jetPt<300"};
  TString jetAlias[2] = {"jetPt","jetPt"};

  multiTreeUtil vmt[2];
  TCut centCut = "cBin>=0&&cBin<40";
  vmt[0].addFile("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0_addedReweight.root","tdj", jetSelCut[0] && centCut,1);

  vmt[1].addFile("../ntout/jskim_hydj80_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0_addedReweight.root", "tdj", jetSelCut[1] && centCut && "pthat>=80&&pthat<100",9.913e-5*1.e9);
  vmt[1].addFile("../ntout/jskim_hydj100_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0_addedReweight.root", "tdj", jetSelCut[1] && centCut && "pthat>=100&&pthat<170",3.069e-5*1.e9);
  vmt[1].addFile("../ntout/jskim_hydj170_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0_addedReweight.root", "tdj", jetSelCut[1] && centCut && "pthat>=170&&pthat<9999",1.470e-6*1.e9);
  vmt[1].NormalizeScales(2,"","hEvtCentNoSkim");

  for (int i=0; i<2; ++i) {
    vmt[i].AddFriend("yTrk=yongsunTrack");
    if (i>0) vmt[i].AddFriend("genPar");
    vmt[i].SetAlias("finalJetPt",jetAlias[i]);
    cout << "(input " << i << ") finalJetPt = " << vmt[i].trees_[0]->GetAlias("finalJetPt") << endl;
  }

  TFile * ouf = new TFile("hists.root","recreate");
  TString outdir = "fig/Mar26";

  // Draw jet
  TCut jetCut="abs(jetEta)<2&&abs(jetEta)>0.3";
  TH1D * hJetPt[2];
  for (int i=0; i<2; ++i) {
    hJetPt[i] = new TH1D(Form("hJetPt_%d",i),";Jet p_{T} (GeV/c);",20,0,400);
    vmt[i].Draw(hJetPt[i],"finalJetPt",jetCut,"1");
  }

  // Normalization
  float nJet[2] = {0,0};
  for (int i=0; i<2; ++i) {
    nJet[i] = hJetPt[i]->Integral();
    cout << "(input " << i << ") nJet = " << nJet[i] << endl;
    hJetPt[i]->Scale(1./nJet[i]);
  }

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

  // Draw Trk
  TCut trkCut;
  if (anamode==10) trkCut = "yTrk.jetMatch==-1&&yTrk.jetDr<0.3&&yTrk.pt>1 && yTrk.pt<300";
  if (anamode==0)  trkCut = "yTrk.jetMatch==1 &&yTrk.jetDr<0.3&&yTrk.pt>1 && yTrk.pt<300";
  // TString trkWeight="( yTrk.trkWeight*(yTrk.trkWeight<10) + 1.47*(yTrk.trkWeight>=10) )";
  TString trkWeight="( yTrk.trkWeight*(yTrk.trkWeight<10) + 1.47*(yTrk.trkWeight>=10) ) * yTrk.pt";
  // Draw Jet and Trk
  TH2D * hMulvCent[2];
  float xmin=0, xmax=120;
  if (anamode==0) {xmax=300;}
  for (int i=0; i<2; ++i) {
    // hMulvCent[i] = new TH2D(Form("hMulvCent_%d",i),Form(";Centrality Bin;%s Cone Track Mult.;",AnaTitle.Data()),10,0,40,80,0,80);
    hMulvCent[i] = new TH2D(Form("hMulvCent_%d",i),Form(";Centrality Bin;Cone #Sigma p_{T} (GeV/c);"),10,0,40,120,xmin,xmax);
    TString drawvar = Form("Sum$( ( %s ) * ( %s) ):cBin",trkCut.GetTitle(),trkWeight.Data());
    cout << drawvar << endl;
    vmt[i].Draw2D(hMulvCent[i],drawvar,jetCut,"1");
  }

  // Normalize
  for (int i=0; i<2; ++i) {
    hMulvCent[i]->Scale(1./nJet[i]);
  }

  // Draw
  TCanvas * c4 = new TCanvas("c4","c4",700,0,800,400);
  c4->Divide(2,1);
  c4->cd(1);
  // gPad->SetRightMargin(0.15);
  hMulvCent[0]->Draw("box");
  c4->cd(2);
  gPad->SetRightMargin(0.15);
  hMulvCent[1]->Draw("box");

  // Project
  AnaTH2 vana2d[2];
  for (int i=0; i<2; ++i) {
    vana2d[i].Init(hMulvCent[i]);
    vana2d[i].GetSummary();
  }

  // TCanvas * c5 = new TCanvas("c5","c5",1200,600);
  // c5->Divide(4,2);
  // int maxnpad=8;
  TCanvas * c5 = new TCanvas("c5","c5",1200,900);
  c5->Divide(4,3);
  int maxnpad=12;
  int nplot = TMath::Min(vana2d[0].nbinx,maxnpad);
  for (int ip=1; ip<=nplot; ++ip) {
    c5->cd(ip);
    gPad->SetLogy();
    fixedFontHist(vana2d[0].vh1[ip],2.6,2.6,20);
    handsomeTH1(vana2d[0].vh1[ip],kBlack,1,kFullCircle);
    handsomeTH1(vana2d[1].vh1[ip],kRed,1,kOpenCircle);
    vana2d[0].vh1[ip]->SetYTitle("unity norm.");
    vana2d[0].vh1[ip]->Draw();
    vana2d[1].vh1[ip]->Draw("sameE");
    drawText(Form("%.0f%% - %.0f%%", float((ip-1)*100./vana2d[0].nbinx), float(ip*100./vana2d[0].nbinx)),0.48,0.85,kBlack,25);
  }
  // Legend
  TLegend * l2 = new TLegend(0.43,0.6,0.94,0.89,NULL,"brNDC");
  easyLeg(l2,"",22);
  l2->AddEntry(vana2d[0].vh1[1],AnaTitle+" Cone","");
  l2->AddEntry(vana2d[0].vh1[1],"PbPb Data","p");
  l2->AddEntry(vana2d[1].vh1[1],"PbPb MC","p");
  c5->cd(4);
  l2->Draw();
  c5->Print(outdir+Form("/conesum_cent_bins_ana%d.gif",anamode));
  c5->Print(outdir+Form("/conesum_cent_bins_ana%d.pdf",anamode));

  TCanvas * c6 = new TCanvas("c6","c6",800,400);
  c6->Divide(2,1);
  handsomeTH1(vana2d[0].hymean,kBlack,1,kFullCircle);
  fixedFontHist(vana2d[0].hymean,1.,1.,20);
  handsomeTH1(vana2d[1].hymean,kRed,1,kOpenCircle);
  // vana2d[0].hymean->SetYTitle(Form("<%s Cone Mult.>",AnaTitle.Data()));
  vana2d[0].hymean->SetYTitle(Form("<Cone #Sigma p_{T}> (GeV/c)"));
  vana2d[0].hymean->SetAxisRange(0,25,"Y");
  if (anamode==0) vana2d[0].hymean->SetAxisRange(0,120,"Y");
  handsomeTH1(vana2d[0].hyrms,kBlack,1,kFullCircle);
  handsomeTH1(vana2d[1].hyrms,kRed,1,kOpenCircle);
  fixedFontHist(vana2d[0].hyrms,1.,1.,20);
  vana2d[0].hyrms->SetYTitle(Form("#sigma(Cone #Sigma p_{T}) (GeV/c)"));
  vana2d[0].hyrms->SetAxisRange(0,12,"Y");
  if (anamode==0)  vana2d[0].hyrms->SetAxisRange(0,60,"Y");
  c6->cd(1);
  vana2d[0].hymean->Draw("E");
  vana2d[1].hymean->Draw("Esame");
  gPad->RedrawAxis();
  c6->cd(2);
  vana2d[0].hyrms->Draw("E");
  vana2d[1].hyrms->Draw("Esame");
  gPad->RedrawAxis();

  // TLegend * l2 = new TLegend(0.51,0.65,0.94,0.86,NULL,"brNDC");
  // easyLeg(l2,"",20);
  // l2->AddEntry(vana2d[0].hymean,"PbPb Data","p");
  // l2->AddEntry(vana2d[1].hymean,"PbPb MC","p");
  l2->Draw();
  c6->Print(outdir+Form("/conesumfluc_vs_cent_ana%d.gif",anamode));
  c6->Print(outdir+Form("/conesumfluc_vs_cent_ana%d.pdf",anamode));
}
