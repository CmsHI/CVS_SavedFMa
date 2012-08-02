#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <iostream>
#include <TMath.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>
#include <TNtuple.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TPolyLine.h>
#include "commonUtility.h"
#include "CutAndBinCollection2011.h"
const int khimc=0;
const int khidata=1;

void doSubtraction(TH1* hbefore=0, TH1* hafter=0 );

valPair fitIt (TH1D* hdphiData=0,bool doLog=false, float fitxmin=3.141592*2./3., bool fixBase=true, bool likel=false);

void fitDphi(float fitxmin = 3.141592*2./3.) {
  
  bool doLog=false;
  bool fixBase = false;
  TH1D* hdphiData[5];   //[data kind] [ centrality] 
  TH1D* hdphiDataAllJet[5];   //[data kind] [ centrality]                                                                                          
  TFile *inf = new TFile("gammaJetOutputs.root");
  for ( int icent=1; icent<=4 ; icent++) { 
    hdphiData[icent] = (TH1D*)inf->Get(Form("data_icent%d_jetDphiPureJPurePho",icent));
    handsomeTH1(hdphiData[icent]);
    hdphiDataAllJet[icent] =(TH1D*)inf->Get(Form("data_icent%d_jetDphiPurePho",icent));
  }
  
  TFile *infPaper = new TFile("/net/hidsk0001/d00/scratch/jazzitup/macros/plotsHIN_11_010/HisOutput_Photonv7_v24_akPu3PF_InclDeltaPhi_gamma60jet30dphiSig700_Isol0_Norm1.root");
  
  TH1D* hdphiPaper[5];
  for ( int icent=1; icent<=4 ; icent++) {
    hdphiPaper[icent] = (TH1D*)infPaper->Get(Form("dataSrc1_reco1_cent%dSubtractedExtrapExtrapNorm",icent-1));
  }
  
  
  TCanvas* c0 = new TCanvas("c0","",1000,350); //compare dphi subtraction
  makeMultiPanelCanvas(c0,4,1,0.0,0.0,0.2,0.15,0.02);
  TH1D* hdphiDataAllJet_SBSubt[5];   //[data kind] [ centrality]                                                                    
  for ( int icent=1; icent<=4 ; icent++) {
    hdphiDataAllJet_SBSubt[icent] = (TH1D*)hdphiDataAllJet[icent]->Clone(Form("data_icent%d_jetDphiPurePho_SBSubt",icent));
    doSubtraction(hdphiDataAllJet[icent],hdphiDataAllJet_SBSubt[icent]);
  }
  for ( int icent=1; icent<=4 ; icent++) {
    c0->cd(5-icent);
    handsomeTH1(hdphiData[icent],2);
    handsomeTH1(hdphiDataAllJet_SBSubt[icent],1);
    hdphiDataAllJet_SBSubt[icent]->SetMarkerStyle(24);
    hdphiData[icent]->SetMarkerStyle(24);
    hdphiData[icent]->SetAxisRange(-0.1,.7,"Y");
    hdphiData[icent]->DrawCopy();
    hdphiDataAllJet_SBSubt[icent]->DrawCopy("same");
    jumSun(0,0,0,3.141592);
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    drawText(Form("%.0f % ~ %.0f %", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.2680963,0.4569118);

  }
  
  TGraphAsymmErrors* dphihidatafrank = new TGraphAsymmErrors(); 
  dphihidatafrank->SetPoint(0,359.2199,0.2821385);
  dphihidatafrank->SetPointError(0,0,0,0.0385427,0.0385427);
  dphihidatafrank->SetPoint(1,235.7692,0.2705194);
  dphihidatafrank->SetPointError(1,0,0,0.02518145,0.02518145);
  dphihidatafrank->SetPoint(2,116.3598,0.2674775);
  dphihidatafrank->SetPointError(2,0,0,0.02726252,0.02726252);
  dphihidatafrank->SetPoint(3,43.66614,0.2470803);
  dphihidatafrank->SetPointError(3,0,0,0.03959091,0.03959091);
  
  
  TGraphAsymmErrors* gdphi = new TGraphAsymmErrors();
  
  double npartbin[5] = {0,359.21, 235.76, 116.35, 43.66};

   
  TCanvas* c1 = new TCanvas("c1","",1000,350);
  makeMultiPanelCanvas(c1,4,1,0.0,0.0,0.2,0.15,0.02);
  
  // TF1 *ff = new TF1("jkjisf","[0]*exp(-(3.1415926-x)/[1])",fitxmin,3.1415926);
  for ( int icent=1; icent<=4 ; icent++) {
    //    fdphi[icent] = (TF1*)ff->Clone(Form("fitFDphi_icent%d",icent));
    c1->cd(5-icent);
    valPair theValPair = fitIt(hdphiData[icent],doLog,fitxmin,fixBase);
    gdphi->SetPoint(icent-1,npartbin[icent], theValPair.val);
    //  gdphi->SetPointError(icent-1,0,0, theValPair.err, theValPair.err);
  }

  TCanvas* c3 = new TCanvas("c3","",1000,350);
  makeMultiPanelCanvas(c3,4,1,0.0,0.0,0.2,0.15,0.02);
  TGraphAsymmErrors* gdphiSB = new TGraphAsymmErrors();
  for ( int icent=1; icent<=4 ; icent++) {
    //    fdphi[icent] = (TF1*)ff->Clone(Form("fitFDphi_icent%d",icent));                                                      
    c3->cd(5-icent);
    valPair theValPair = fitIt(hdphiDataAllJet_SBSubt[icent],doLog,fitxmin,fixBase);
    gdphiSB->SetPoint(icent-1,npartbin[icent], theValPair.val);
    //  gdphi->SetPointError(icent-1,0,0, theValPair.err, theValPair.err);                                                            
  }
  
  TCanvas* c4 = new TCanvas("c4","",1000,350);
  makeMultiPanelCanvas(c4,4,1,0.0,0.0,0.2,0.15,0.02);
  for ( int icent=1; icent<=4 ; icent++) {
    c4->cd(5-icent);
    hdphiPaper[icent]->Draw("");
    //    hdphiDataAllJet_SBSubt[icent]->Draw("same");
    TH1D* hhtemp = (TH1D*)hdphiDataAllJet_SBSubt[icent]->Clone("hhtempte");
    hhtemp->Scale( 1. /   hhtemp->Integral( hhtemp->FindBin(0.7) , hhtemp->GetNbinsX()));
    hhtemp->DrawCopy("same");
    //   hdphiPaper[icent]->Draw("same");
  }

  
  
  TCanvas *cfinal = new TCanvas("cfinal","",500,500);

  TH1D* hTemp4 = new TH1D("htemp2",";N_{part};#sigma(|#Delta#phi_{J#gamma}|)",100,-20,400);
  hTemp4->SetNdivisions(505);
  handsomeTH1(hTemp4,1);
  hTemp4->SetAxisRange(0,.5,"Y");
  hTemp4->Draw();
  handsomeTGraph(gdphi,2);
  handsomeTGraph(gdphiSB,1);
  dphihidatafrank->Draw("p");
  gdphi->Draw("p");
  gdphiSB->Draw("p");
  
}


valPair fitIt (TH1D* hdphiData,bool doLog,float fitxmin, bool fixBase, bool likel){ 
  valPair ret;

  TF1 *ff = new TF1("jkjisf","[0]*exp(-(3.1415926-x)/[1])",fitxmin,3.1415926);
  
  ff->SetParameter(0,1);
  ff->SetParameter(1,0.2);
  
  hdphiData->Fit(ff->GetName(),"LLM0 O","",fitxmin,3.1415926);
  ret.val = ff->GetParameter(1);
  ret.err = ff->GetParError(1);
  ff->SetLineWidth(2);
  ff->SetLineColor(2);
  TH1D* htemp = (TH1D*)hdphiData->Clone("htemptemp222");
  htemp->Reset();
  htemp->Add(hdphiData);
  if ( doLog) htemp->SetAxisRange(0.0001,10,"Y");
  else htemp->SetAxisRange(-0.1,.7,"Y");
  htemp->SetAxisRange(0.2,4,"X");
  htemp->DrawCopy();
  ff->DrawCopy("same");  
  if (doLog) gPad->SetLogy();
  else jumSun(0.2,0,3.141592,0);
  
  cout << " ret.val = " << ret.val << endl;
  return ret;
}

void doSubtraction(TH1* hbefore, TH1* hafter ) {
  
  TH1D* htemp = (TH1D*)hbefore->Clone("htempbefore");
  int bin1 = htemp->FindBin(0.5);
  int bin2 = htemp->FindBin(3.141592/2);
  float theArea = htemp->Integral(bin1,bin2,"width");
  float height = theArea / ( htemp->GetBinWidth(bin1) * (bin2-bin1+1.));
  for ( int ii=1 ; ii<= htemp->GetNbinsX() ; ii++) {
    htemp->SetBinContent(ii, htemp->GetBinContent(ii) - height);
  }
  hafter->Reset();
  hafter->Sumw2();
  hafter->Add(htemp);
}
