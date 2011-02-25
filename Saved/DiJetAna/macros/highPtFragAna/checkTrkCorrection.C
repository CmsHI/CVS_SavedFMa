#include <iostream>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TCanvas.h>
#include <TLine.h>
#include "TLegend.h"
#include "TPad.h"
#include "HisMath.C"
using namespace std;

#define PI 3.14159265358979

TH1D * projectPt(TH3F * h3,Int_t xbin, Int_t zbin)
{
  h3->GetXaxis()->SetRange(xbin,xbin);
  h3->GetZaxis()->SetRange(zbin,zbin);
  TH1D * hx = (TH1D*)h3->Project3D(Form("y_%d_%d_py",xbin,zbin));
  hx->SetAxisRange(0,100,"X");
  hx->SetAxisRange(-0.2,1.6,"Y");
  hx->Draw();
  return hx;
}

void checkTrkCorrection(
    Int_t corrLevel=0,
    Int_t sampleIndex=3,
    TString infrec="nt_djhp_HyUQ110v0_djcalo.root",
    TString infgen="nt_djhp_HyUQ110v0_djcalo_genp.root",
    TCut evtCut="cent<30")
{
  //TH1::SetDefaultSumw2();
  TChain * trec = new TChain("tjttrk");
  trec->Add(infrec);
  TChain * tgen = new TChain("tjttrk");
  tgen->Add(infgen);
  cout << infrec << " cut " << TString(evtCut) << ": " << trec->GetEntries() << endl;
  cout << infgen << " cut " << TString(evtCut) << ": " << tgen->GetEntries() << endl;
   
  // Correction Histograms
  TFile * fTrkCorr = new TFile(infrec);
  Int_t nfile=5, ncbin=5, nlv=4;
  TString lvName[4] = {"Eff","Fak","Mul","Sec"};
  vector<vector<vector<TH3F*> > > vhCorr(nfile, vector<vector<TH3F*> >(ncbin,vector<TH3F*>(nlv)));
  for (Int_t i=0; i<nlv; ++i) {
    for (Int_t j=0; j<nfile; ++j) {
      for (Int_t k=0; k<ncbin; ++k) {
	vhCorr[i][j][k] = (TH3F*)fTrkCorr->Get(Form("hTrk%s_f%d_c%d",lvName[i].Data(),j,k));
	//cout << vhCorr[i][j][k]->GetName() << " " << vhCorr[i][j][k]->GetEntries() << endl;
      }
    }
  }

  TH1D * trackingEtaBin = (TH1D*)vhCorr[0][3][0]->Project3D("x");
  TH1D * trackingPtBin = (TH1D*)vhCorr[0][3][0]->Project3D("y");
  TH1D * trackingJEtBin = (TH1D*)vhCorr[0][3][0]->Project3D("z");
  for (Int_t icbin=0; icbin<ncbin; ++icbin) {
    TString name=Form("Corr_cbin%d",icbin);
    TCanvas *cCorr = new TCanvas("c"+name,name,1200,1000);
    Int_t nEtaBin = trackingEtaBin->GetNbinsX();
    Int_t nPtBin = trackingPtBin->GetNbinsX();
    Int_t nJEtBin = trackingJEtBin->GetNbinsX();
    cout << "nbins x,y,z: " << nEtaBin << " " << nPtBin << " " << nJEtBin << endl;
    Int_t nEtaBinPlot=6,nJEtBinPlot=6,startEtaBin=2,startJEtBin=4;
    cCorr->Divide(nEtaBinPlot,nJEtBinPlot);
    for (Int_t j=startJEtBin;j<startJEtBin+nJEtBinPlot; ++j) {
      for (Int_t i=startEtaBin; i<startEtaBin+nEtaBinPlot; ++i) {
	Int_t ipad = 1+(j-startJEtBin)*nEtaBinPlot+(i-startEtaBin);
	cCorr->cd(ipad);
	cout << "pad " << ipad << " - ";
	Float_t eta0=trackingEtaBin->GetBinLowEdge(i),eta1=trackingEtaBin->GetBinLowEdge(i+1);
	Float_t JEt0=trackingJEtBin->GetBinLowEdge(j),JEt1=trackingJEtBin->GetBinLowEdge(j+1);
	cout << " eta bin: " << eta0 << "to" << eta1;
	cout << " JEt bin: " << JEt0 << "to" << JEt1 << endl;
	TH1D * hEff = projectPt(vhCorr[corrLevel][sampleIndex][icbin],i,j);
	if (j==startJEtBin) {
	  TLegend *leg = new TLegend(0.2,0.78,0.9,0.98);
	  leg->SetBorderSize(0);
	  leg->SetFillStyle(0);
	  leg->SetTextSize(0.1);
	  leg->AddEntry(hEff,Form("%.1f < #eta < %.1f",eta0,eta1),"");
	  leg->Draw();
	}
	if (i==startEtaBin) {
	  TLegend *leg2 = new TLegend(0.02,0.68,0.9,0.88);
	  leg2->SetBorderSize(0);
	  leg2->SetFillStyle(0);
	  leg2->SetTextSize(0.1);
	  leg2->AddEntry(hEff,Form("%.0f<Jet p_{T}<%.0fGeV",JEt0,JEt1),"");
	  leg2->Draw();
	}
      }
    }
    cCorr->Print(Form("out/hTrk%s_f%d_c%d.gif",lvName[corrLevel].Data(),sampleIndex,icbin));
  }

  // ====================
  TLegend *leg = new TLegend(0.61,0.78,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
}
