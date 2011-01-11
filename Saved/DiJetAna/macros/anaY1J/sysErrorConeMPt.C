#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TLine.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include "TSystem.h"
#include "TF1.h"


void sysErrorConeMPt(
    TString inFileName="jfh_HCPR_J50U_Cent30to100_Aj0to100_SubEtaRefl.root",
    Int_t compMode = 0, // Compare mode: 0 reco-genSig, 1 reco-genAll, 3 genAll-genSig, 4 calo_genp-allGen
    Int_t sysMode = 0, // Plot mode: 0 for simple plot, 1 for difference
    TString outdir = ".",
    TString title = "test"
    ) {
  // ===============================================
  // Inputs
  // ===============================================
  TFile *f = new TFile(inFileName);
  TString inFileNameStrip(inFileName); inFileNameStrip.ReplaceAll(".root","");
  TString inFileNameGen(inFileName);
  if (compMode==0) { 
    inFileNameGen.ReplaceAll("djcalo","djcalo_genp");
    inFileNameGen.ReplaceAll("HydjetAll","HydjetSig");
  }
  if (compMode==1) inFileNameGen.ReplaceAll("djcalo","djcalo_genp");
  else if (compMode==3) inFileNameGen.ReplaceAll("HydjetAll","HydjetSig");
  else if (compMode==5) inFileNameGen.ReplaceAll("djcalo_genp","djgen");
  TFile *fgen = new TFile(inFileNameGen);
  TString inFileNameStripGen(inFileNameGen); inFileNameStripGen.ReplaceAll(".root","");
  cout << "==========================================================" << endl;
  cout << "Compare: " << inFileName << endl
       << "         vs " << endl
       << "         " << inFileNameGen << endl;
  cout << "==========================================================" << endl;

  // ===============================================
  // Setup
  // ===============================================
  TString tag=Form("sysErrorConeMPt_%s_%s_%d_%d",inFileNameStrip.Data(),title.Data(),compMode,sysMode);

  // ===============================================
  // Analyze
  // ===============================================
  TH2D * hPtPNDR = (TH2D*) f->Get("hPtPNDR");
  TH2D * hPtPADR = (TH2D*) f->Get("hPtPADR");
  TH2D * hPtPNDRBg = (TH2D*) f->Get("hPtPNDRBg");
  TH2D * hPtPADRBg = (TH2D*) f->Get("hPtPADRBg");
  TH2D * hPtDRMPt = (TH2D*)hPtPNDR->Clone(tag+"hPtDRMPt");
  hPtDRMPt->Add(hPtPNDR,hPtPADR,1,-1);

  TH2D * hPtPNDRGen = (TH2D*) fgen->Get("hPtPNDR");
  TH2D * hPtPADRGen = (TH2D*) fgen->Get("hPtPADR");
  TH2D * hPtPNDRBgGen = (TH2D*) fgen->Get("hPtPNDRBg");
  TH2D * hPtPADRBgGen = (TH2D*) fgen->Get("hPtPADRBg");
  TH2D * hPtDRMPtGen = (TH2D*)hPtPNDRGen->Clone(tag+"hPtDRMPtGen");
  hPtDRMPtGen->Add(hPtPNDRGen,hPtPADRGen,1,-1);

  // ===============================================
  // Draw
  // ===============================================
  // Get Pt info
  Int_t numPtBins=hPtPNDR->GetNbinsX();
  TH1D * hPt = (TH1D*)hPtPNDR->ProjectionX("hPt");
  /*
  cout << "Pt bins: " << numPtBins << endl;
  for (Int_t i=0; i<numPtBins+2; ++i) {
    cout << "Pt Bin " << i << " Low Edge: " << hPt->GetBinLowEdge(i) << endl;
  }
  */

  // What pt bins to draw
  const Int_t numPtBinsDraw=3;

  TCanvas * c6 = new TCanvas("c"+tag,"c"+tag,1400,500);
  c6->Divide(3,1);
  for (Int_t i=0; i<numPtBinsDraw; ++i) {
    Int_t iBeg,iEnd;
    if (i==0) { iBeg=2; iEnd=3;}
    if (i==1) { iBeg=4; iEnd=4;}
    if (i==2) { iBeg=5; iEnd=numPtBins;}
    cout << "Bin: " << iBeg <<  " to " << iEnd << endl;
    TH1D * hNr = (TH1D*)hPtDRMPt->ProjectionY(tag+Form("hMPt_%d_%d",iBeg,iEnd),iBeg,iEnd);
    TH1D * hNrGen = (TH1D*)hPtDRMPtGen->ProjectionY(tag+Form("hMPtGen_%d_%d",iBeg,iEnd),iBeg,iEnd);
    if (sysMode==1) {
      hNr->Add(hNrGen,-1);
    }
    if (sysMode==2) {
      hNr->Divide(hNrGen);
    }
    // Print
    cout << Form("%.1f < P_{T} < %.1f GeV: ",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1))
      << " Cone MPt: " << hNr->Integral() << " Gen: " << hNrGen->Integral() << endl;
    // Styles
    hNr->SetMarkerColor(kRed);
    hNr->SetLineColor(kRed);
    // Axis Range
    if (sysMode==0) {
      hNr->SetYTitle("Cone MPt (GeV/c)");
      hNr->SetAxisRange(-20,20,"Y");
    }
    if (sysMode==2) {
      hNr->SetAxisRange(-2,6,"Y");
    }
    // Axis Label
    if (compMode==0) {
      if (sysMode==1) {
	hNr->SetYTitle("(Reco Trk)-(Sig GenP) (GeV/c)");
	//hNr->SetAxisRange(-20,20,"Y");
      }
      if (sysMode==2) {
	hNr->SetYTitle("(Reco Trk)/(Sig GenP) (GeV/c)");
      }
    }
    if (compMode==3) {
      if (sysMode==1) {
	hNr->SetYTitle("All GenP - Sig GenP (GeV/c)");
	//hNr->SetAxisRange(-3,3,"Y");
      }
      if (sysMode==2) {
	hNr->SetYTitle("(All GenP)/(Sig GenP) (GeV/c)");
	hNr->SetAxisRange(-2,6,"Y");
      }
    }
    hNr->SetXTitle("#Delta R");
    hNr->SetAxisRange(0,0.799,"X");
    hNr->SetTitleOffset(1.5,"X");
    hNr->GetXaxis()->CenterTitle();
    hNr->GetYaxis()->CenterTitle();
    c6->cd(i+1);
    // Fit
    if (sysMode>0) {
      TF1 * f0 = new TF1("f0","pol0");
      f0->SetLineStyle(2);
      f0->SetLineWidth(1);
      hNr->Fit("f0");
    }
    // Draw to Inspect
    hNr->Draw();
    if (sysMode==0) {
      hNrGen->Draw("hist same");
    }
    if (sysMode==0||sysMode==1) {
      TLine *l = new TLine(0,0,0.8,0);
      l->Draw();
    }
    if (sysMode==2) {
      TLine *l = new TLine(0,1,0.8,1);
      l->Draw();
    }
    
    TLegend *leg = new TLegend(0.35,0.7,0.85,0.94);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hNr,Form("%.1f < P_{T} < %.1f GeV",hPt->GetBinLowEdge(iBeg),hPt->GetBinLowEdge(iEnd+1)),"");
    if (sysMode==0) {
      if (compMode==0) {
	leg->AddEntry(hNr,"Reco Trk","pl");
	leg->AddEntry(hNrGen,"Sig. GenP","l");
      }
      if (compMode==1) {
	leg->AddEntry(hNr,"Reco Trk","pl");
	leg->AddEntry(hNrGen,"All GenP","l");
      }
    }
    leg->SetTextSize(0.05);
    leg->Draw();
  }

  // ===============================================
  // Save
  // ===============================================
  c6->Print(Form("%s/%s.gif",outdir.Data(),tag.Data()));
  c6->Print(Form("%s/%s.eps",outdir.Data(),tag.Data()));
  c6->Print(Form("%s/%s.C",outdir.Data(),tag.Data()));
}

void sysErrorAll(
    TString anaV="v18ReWt",
    TString BckSub="SubNone"
    )
{
  TString outdir=anaV+BckSub;
  gSystem->mkdir(outdir.Data(),kTRUE);

  Int_t sysModes[2] = {0,2};
  for (Int_t m=0; m<2;++m) {
    sysErrorConeMPt("jfh"+anaV+"_HydjetAll_djcalo_Cent0to30_Aj0to100_"+BckSub+".root",0,sysModes[m],outdir);
    sysErrorConeMPt("jfh"+anaV+"_HydjetAll_djcalo_Cent0to30_Aj0to100_"+BckSub+".root",1,sysModes[m],outdir);
    sysErrorConeMPt("jfh"+anaV+"_HydjetAll_djcalo_genp_Cent0to30_Aj0to100_"+BckSub+".root",3,sysModes[m],outdir);
  }
}
