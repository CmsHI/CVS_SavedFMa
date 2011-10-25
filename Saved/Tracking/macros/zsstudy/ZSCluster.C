#include <iostream>
#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TString.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TSystem.h"
using namespace std;

void RemoveHistZeroPoints(TH1 * h)
{
  for (int i=0; i<=h->GetNbinsX(); ++i) {
    double y = h->GetBinContent(i);
    if (fabs(y)<0.001) h->SetBinContent(i,-10000);
  }
}

void ZSCluster(
               //TString infname="ntout/sstr_RECOZSHEAD10_07_1148am_GTV4_hi2010ped.root",
               //TString infname="ntout/sstr_RECOZSHEAD10_07_1148am_restoreThresh0MeanCM128GTV4_hi2010ped.root",
               TString infname="~/scratch01/work/HLT/hi2011zsyueshi_441/src/Saved/TrackerZS/test/sstr_RECOZSHEADyueshi10195pm_.root",
               int evt=646838,
               int id=436294128,
               int cluster_beg=0,
               int cluster_size=768,
               TString outdir="out/test",
               TString title="PromptRecoZS",
               bool useBLF=true,
               bool newInterface=true
                )
{
  TFile * inf1 = new TFile(infname);
  cout << infname << " " << title << endl;
  
  TTree * tvr1   = (TTree*)inf1->Get("ssnt/vr");
  TTree * tzs1   = (TTree*)inf1->Get("ssnt/zs");
  TTree * tbase1 = (TTree*)inf1->Get("ssnt/pr");
  TTree * tclus  = (TTree*)inf1->Get("ssnt/cl");
  TTree * tbadvr1 = (TTree*)inf1->Get("ssntBad/vr");
  TTree * tblpts1 = 0;
  if (newInterface) tblpts1 = (TTree*)inf1->Get("ssntBadBLPts/zs");
  else tblpts1 = (TTree*)inf1->Get("ssntBadBLPts/pr");
    
  TH1D * hRef = new TH1D("hRef","",768,0,768);
  TH1D * hPR1 = new TH1D("hPR1","",768,0,768);
  TH1D * hBase1 = new TH1D("hBase1","",768,0,768);
  TH1D * hBlPts1 = new TH1D("hBlPts1","",768,0,768);
  TH1D * hZS1 = new TH1D("hZS1","",768,0,768);
  TH1D * hClus = new TH1D("hClus","",768,0,768);
  TH1D * hBadVR1 = new TH1D("hBadVR1","",768,0,768);

  //cout << "booked hists" << endl;
  TString sel=Form("evt==%d&&id==%d",evt,id);
  TString selclus=Form("evt==%d&&id==%d&&strip>=%d&&strip<=%d",evt,id,cluster_beg,cluster_beg+cluster_size);
  cout << sel << endl;
  cout << selclus << endl;
  tvr1->Project("hRef","strip",Form("adc*(%s)",sel.Data()));
  tvr1->Project("hPR1","strip",Form("padc*(%s)",sel.Data()));
  tzs1->Project("hZS1","strip",Form("adc*(%s)",sel.Data()));
  tclus->Project("hClus","strip",Form("50*(%s)",selclus.Data()));
  tbase1->Project("hBase1","strip",Form("adc*(%s)",sel.Data()));
  if (useBLF) {
    if (newInterface) tblpts1->Project("hBlPts1","strip",Form("(adc-128)*(%s)",sel.Data()));
    else tblpts1->Project("hBlPts1","strip",Form("(adc)*(%s)",sel.Data()));
    tbadvr1->Project("hBadVR1","strip",Form("adc*(%s)",sel.Data()));
  }
  
  //cout << "drawn from tree" << endl;
  RemoveHistZeroPoints(hBlPts1);
  //RemoveHistZeroPoints(hZS1);
  RemoveHistZeroPoints(hClus);
  RemoveHistZeroPoints(hBadVR1);

  TCanvas * c2 = new TCanvas("c2","",1000,500);
  TH1D * hFrame = (TH1D*)hRef->Clone("hFrame");
  hFrame->Reset();
  hFrame->SetTitle(";strip # ; ADC");
  //hFrame->SetAxisRange(0,128,"X");
  hFrame->SetAxisRange(-150,1000,"Y");
  hRef->SetLineStyle(2);
  //hRef->SetLineWidth(2);
  hClus->SetLineColor(kGreen-8);
  hClus->SetMarkerColor(kGreen-9);
  hClus->SetLineWidth(2);
  hClus->SetMarkerSize(1.);
  hBase1->SetLineColor(kBlue);
  hBlPts1->SetLineColor(kBlue+1);
  hBlPts1->SetMarkerColor(kBlue+1);
  hBlPts1->SetMarkerSize(0.7);
  hBlPts1->SetMarkerStyle(kOpenCircle);
  hZS1->SetLineColor(kRed);
  //hZS1->SetLineWidth(2);
  hBadVR1->SetLineColor(kYellow);
  hBadVR1->SetMarkerColor(kYellow);
  hBadVR1->SetLineWidth(3);
  hBadVR1->SetMarkerSize(1.);
  hFrame->Draw("hist");
  if (useBLF) hBadVR1->Draw("same p");
  hClus->Draw("same p");
  hRef->Draw("same hist");
  hPR1->Draw("same hist");
  hBase1->Draw("same hist");
  hZS1->Draw("same hist");
  if (useBLF) hBlPts1->Draw("samep");
  
  TLegend *leg = new TLegend(0.61,0.64,0.92,0.94);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hRef,Form("Evt %d",evt),"");
  leg->AddEntry(hRef,Form("DetID %d",id),"");
  //leg->AddEntry(hRef,"HEAD src 2011.10.07 11am","");
  //leg->AddEntry(hRef,"AllPhysics ReReco ZS","");
  leg->AddEntry(hRef,title,"");
  leg->AddEntry(hRef,"Unpacked VR Digi","l");
  leg->AddEntry(hPR1,"Processed Raw","l");
  leg->AddEntry(hBase1,"Baseline","l");
  if (useBLF) leg->AddEntry(hBlPts1,"Flat Regions","p");
  leg->AddEntry(hZS1,"ZS Digi","l");
  leg->AddEntry(hClus,"Hit Cluster","p");
  if (useBLF) leg->AddEntry(hBadVR1,"Bad Module (VR Saved)","p");
  leg->Draw();
  
  gSystem->mkdir(outdir,true);
  c2->Print(Form("%s/Det%d_%s.gif",outdir.Data(),id,title.Data()));
}