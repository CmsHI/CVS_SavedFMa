#include <iostream>
#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TString.h"
#include "TLine.h"
#include "TCanvas.h"
using namespace std;

void RemoveHistZeroPoints(TH1 * h)
{
  for (int i=0; i<=h->GetNbinsX(); ++i) {
    double y = h->GetBinContent(i);
    if (fabs(y)<0.001) h->SetBinContent(i,-10000);
  }
}

void ZSCluster(
               int id=436294128,
               TString outdir="out/2011.10.11_fixblpts"
                )
{
  //TFile * inf1 = new TFile("../ntout/sstr_RECOZSHEAD10_07_1148am_GTV4_hi2010ped.root");
  //TFile * inf1 = new TFile("../ntout/sstr_RECOZSHEAD10_07_1148am_restoreThresh0MeanCM128GTV4_hi2010ped.root");
  TFile * inf1 = new TFile("../ntout/sstr_RECO1_GTV4_hi2010ped.root");
  
  TTree * tvr1   = (TTree*)inf1->Get("ssnt/vr");
  TTree * tzs1   = (TTree*)inf1->Get("ssnt/zs");
  TTree * tbase1 = (TTree*)inf1->Get("ssnt/pr");
  TTree * tclus  = (TTree*)inf1->Get("ssnt/cl");
  TTree * tbadvr1 = (TTree*)inf1->Get("ssntBad/vr");
  //TTree * tblpts1 = (TTree*)inf1->Get("ssntBadBLPts/zs");
  TTree * tblpts1 = (TTree*)inf1->Get("ssntBadBLPts/pr");
    
  TH1D * hRef = new TH1D("hRef","",768,0,768);
  TH1D * hPR1 = new TH1D("hPR1","",768,0,768);
  TH1D * hBase1 = new TH1D("hBase1","",768,0,768);
  TH1D * hBlPts1 = new TH1D("hBlPts1","",768,0,768);
  TH1D * hZS1 = new TH1D("hZS1","",768,0,768);
  TH1D * hClus = new TH1D("hClus","",768,0,768);
  TH1D * hBadVR1 = new TH1D("hBadVR1","",768,0,768);

  cout << "booked hists" << endl;
  
  tvr1->Project("hRef","strip",Form("adc*(run==151088&&evt==646838&&id==%d)",id));
  tvr1->Project("hPR1","strip",Form("padc*(run==151088&&evt==646838&&id==%d)",id));
  tzs1->Project("hZS1","strip",Form("adc*(run==151088&&evt==646838&&id==%d)",id));
  tbase1->Project("hBase1","strip",Form("adc*(run==151088&&evt==646838&&id==%d)",id));
  //tblpts1->Project("hBlPts1","strip",Form("(adc-128)*(run==151088&&evt==646838&&id==%d)",id));
  tblpts1->Project("hBlPts1","strip",Form("(adc)*(run==151088&&evt==646838&&id==%d)",id));
  tclus->Project("hClus","strip",Form("50*(run==151088&&evt==646838&&id==%d&&adc>0)",id));
  tbadvr1->Project("hBadVR1","strip",Form("adc*(run==151088&&evt==646838&&id==%d)",id));
  
  cout << "drawn from tree" << endl;
  RemoveHistZeroPoints(hBlPts1);
  //RemoveHistZeroPoints(hZS1);
  RemoveHistZeroPoints(hClus);
  RemoveHistZeroPoints(hBadVR1);

  TCanvas * c2 = new TCanvas("c2","",1000,500);
  hRef->SetTitle(";strip # ; ADC");
  //hRef->SetAxisRange(0,128,"X");
  hRef->SetAxisRange(-200,1400,"Y");
  hRef->SetLineStyle(2);
  //hRef->SetLineWidth(2);
  hClus->SetLineColor(kGreen-8);
  hClus->SetMarkerColor(kGreen-9);
  hClus->SetLineWidth(2);
  hBase1->SetLineColor(kBlue);
  hBlPts1->SetLineColor(kBlue+1);
  hBlPts1->SetMarkerColor(kBlue+1);
  hBlPts1->SetMarkerSize(0.6);
  hBlPts1->SetMarkerStyle(kOpenCircle);
  hZS1->SetLineColor(kRed);
  hZS1->SetLineWidth(2);
  hBadVR1->SetLineColor(kYellow);
  hBadVR1->SetMarkerColor(kYellow);
  hBadVR1->SetLineWidth(3);
  hRef->Draw("hist");
  hBadVR1->Draw("same l");
  hClus->Draw("same p-");
  hRef->Draw("same hist");
  hPR1->Draw("same hist");
  hBase1->Draw("same hist");
  hZS1->Draw("same hist");
  hBlPts1->Draw("samep");
  
  TLegend *leg = new TLegend(0.60,0.6,0.91,0.90);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hRef,"Run 151088, Evt 646838","");
  leg->AddEntry(hRef,Form("DetID %d",id),"");
  //leg->AddEntry(hRef,"HEAD src 2011.10.07 11am","");
  leg->AddEntry(hRef,"AllPhysics ReReco ZS","");
  leg->AddEntry(hRef,"Unpacked VR Digi","l");
  leg->AddEntry(hPR1,"Processed Raw","l");
  leg->AddEntry(hBase1,"Baseline","l");
  leg->AddEntry(hBlPts1,"Flat Regions","p");
  leg->AddEntry(hZS1,"ZS Digi","l");
  leg->AddEntry(hClus,"Hit Cluster","p");
  leg->AddEntry(hBadVR1,"Bad Module (VR Saved)","p");
  leg->Draw();
  
  //c2->Print(Form("%s/Det%d.gif",outdir.Data(),id));
  //c2->Print(Form("%s/Det%d_ForceBaseLineFollower.gif",outdir.Data(),id));
  c2->Print(Form("%s/Det%d_RRZS.gif",outdir.Data(),id));
}