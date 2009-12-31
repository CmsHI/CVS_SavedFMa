#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TF1.h"
#include "TLegend.h"
#include <iostream>
#include <vector>
#include "../selectionCut.h"
using namespace std;

// === helpers ===
Double_t histChi2(TH1 * h1, TH1 *h2)
{
  Double_t sum=0;
  for (UInt_t i=1; i<=h1->GetNbinsX(); ++i) {
    Double_t binDiff = h2->GetBinContent(i)-h1->GetBinContent(i);
    Double_t binChi2 = binDiff*binDiff;
    sum+=binChi2;
  }
  return sum;
}

Double_t histDiffrChi2(
    Double_t testSDFrac   = 0.11,
    Double_t MCSDFrac     = 0.227,
    char * nameData       = "hEaddEp_data",
    char * nameMC         = "hEaddEp_pythia",
    char * nameSD         = "hEaddEp_pythia_SD",
    char * nameNSD        = "hEaddEp_pythia_NSD",
    Int_t draw            = 0,
    Double_t ymax         = 0.025)
{
  TH1D * hData = (TH1D*)(gDirectory->FindObject(nameData)->Clone("hData"));
  TH1D * hMC = (TH1D*)(gDirectory->FindObject(nameMC)->Clone("hMC"));
  TH1D * h1 = (TH1D*)(gDirectory->FindObject(nameSD)->Clone("h1"));
  TH1D * h2 = (TH1D*)(gDirectory->FindObject(nameNSD)->Clone("h2"));
  // scale
  h1->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  h2->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hMC->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hData->Scale(1./hData->GetEntries()/h1->GetBinWidth(1));

  // combine different processes in MC with given weights
  Double_t SDRelFrac = testSDFrac/MCSDFrac;
  Double_t NSDRelFrac = (1-testSDFrac)/(1-MCSDFrac);
  TH1D * h3 = (TH1D*)h2->Clone("h3");
  h3->SetLineColor(kRed);
  h3->SetLineStyle(1);
  h3->SetMarkerColor(kRed);
  h3->SetMarkerStyle(kOpenSquare);

  h3->Scale(NSDRelFrac);
  h3->Add(h1,SDRelFrac);

  // Result
  Double_t result = histChi2(hData,h3);

  // if draw
  if (draw) {
    cout << "Draw: trial SDFrac: " << testSDFrac << "  Raw hist chi2: " << result << endl;
    //hMC->Draw("h");
    hMC->SetMarkerStyle(0);
    hMC->SetLineWidth(1);
    hMC->SetLineStyle(7);
    hData->SetMaximum(ymax);
    hData->Draw("E");
    h3->Draw("hist same");
    h1->SetLineColor(kBlue);
    h1->SetLineStyle(7);
    h1->SetMarkerStyle(kOpenStar);
    h1->SetMarkerColor(kBlue);
    h1->Draw("E same");
    h2->SetLineStyle(3);
    h2->SetMarkerStyle(kOpenSquare);
    h2->SetMarkerColor(kRed-1);
    h1->SetLineColor(kRed-1);
    h2->Draw("E same");
    //  - add legend -
    TLegend *leg2 = new TLegend(0.53356,0.7657,0.9513,0.9178,NULL,"brNDC");
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->AddEntry(hData,"Data","p");
    leg2->AddEntry(h1,"MC - Best Fit SD","p");
    leg2->AddEntry(h2,"MC - Best Fit NSD","p");
    leg2->AddEntry(h3,"MC - Best Fit All","l");
    leg2->Draw();
  }
  else {
    //cout << "SDRelFrac: " << SDRelFrac << "  Raw hist chi2: " << result << endl;
    hData->Delete();
    hMC->Delete();
    h1->Delete();
    h2->Delete();
    h3->Delete();
  }

  // done
  return result;
}

// === Main function ===
void matchFrac(bool testMC = true,
    const char * datafname="pixelTree_merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210_SDRelFrac0.5.root",
    const char * mcfname="pixelTree_merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210.root")
{
  // get trees
  TFile * dataFile = new TFile(datafname);
  TFile * mcFile = new TFile(mcfname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);

  // trigger
  selectionCut mcSel(1);
  selectionCut dataSel(0);
  TCut mcSelCut = mcSel.Cut;
  TCut dataSelCut = dataSel.Cut;
  TCut SDCut = "evtType==92 || evtType==93";
  TCut NSDCut = "evtType!=92 && evtType!=93";
  TCut mcSelSD = mcSelCut && SDCut;
  TCut mcSelNSD = mcSelCut && NSDCut;

  // get truth info
  Double_t McTotalN = treeMC->GetEntries();
  Double_t McSD = treeMC->GetEntries(SDCut);
  Double_t McSDFrac = McSD/McTotalN;
  Double_t McSelTotalN = treeMC->GetEntries(mcSelCut);
  Double_t McSelSD = treeMC->GetEntries(mcSelSD);
  Double_t McSelSDFrac = McSelSD/McSelTotalN;
  printf("MC input SD frac:%f, after selection MC SD frac: %f.\n",McSDFrac,McSelSDFrac);
  if (testMC) {
    dataSelCut=mcSelCut;
    Double_t DataTotalN = treeData->GetEntries();
    Double_t DataSD = treeData->GetEntries(SDCut);
    Double_t DataSDFrac = DataSD/DataTotalN;
    Double_t DataSelTotalN = treeData->GetEntries(mcSelCut);
    Double_t DataSelSD = treeData->GetEntries(mcSelSD);
    Double_t DataSelSDFrac = DataSelSD/DataSelTotalN;
    printf("\"Data\" input SD frac:%f, after selection \"Data\" SD frac: %f.\n",DataSDFrac,DataSelSDFrac);
  }

  // declare histograms
  vector<TString> source;
  source.push_back("data");
  source.push_back("pythia");
  vector<Color_t> color;
  color.push_back(kBlack);
  color.push_back(kRed);
  for (UInt_t i=0; i<source.size(); ++i) {
    TH1D * hEvtEta = new TH1D(Form("hEvtEta_%s",source[i].Data()),";Event #eta;",100,-5,5);
    hEvtEta->SetLineColor(color[i]);
    hEvtEta->SetMarkerColor(color[i]);
    hEvtEta->Sumw2();
    TH1D * hEaddEp = new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",100,0,500);
    hEaddEp->SetLineColor(color[i]);
    hEaddEp->SetMarkerColor(color[i]);
    hEaddEp->Sumw2();
    if (source[i]=="pythia") {
      TH1D * hEvtEta_SD = new TH1D(Form("hEvtEta_%s_SD",source[i].Data()),";Event #eta;",100,-5,5);
      hEvtEta_SD->Sumw2();
      TH1D * hEvtEta_NSD = new TH1D(Form("hEvtEta_%s_NSD",source[i].Data()),";Event #eta;",100,-5,5);
      hEvtEta_NSD->Sumw2();
      TH1D * hEaddEpSD = new TH1D(Form("hEaddEp_%s_SD",source[i].Data()),";#Sigma E+Pz;",100,0,500);
      hEaddEpSD->SetLineColor(color[i]);
      hEaddEpSD->SetMarkerColor(color[i]);
      hEaddEpSD->SetLineWidth(2);
      hEaddEpSD->SetLineStyle(7);
      hEaddEpSD->Sumw2();
      TH1D * hEaddEpNSD = new TH1D(Form("hEaddEp_%s_NSD",source[i].Data()),";#Sigma E+Pz;",100,0,500);
      hEaddEpNSD->SetLineColor(color[i]);
      hEaddEpNSD->SetMarkerColor(color[i]);
      hEaddEpNSD->SetLineWidth(2);
      hEaddEpNSD->SetLineStyle(9);
      hEaddEpNSD->Sumw2();
    }
  }

  // take a look
  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  treeMC->Draw("evtEta>>hEvtEta_pythia",mcSelCut);
  treeMC->Draw("evtEta>>hEvtEta_pythia_SD",mcSelSD,"same");
  treeMC->Draw("evtEta>>hEvtEta_pythia_NSD",mcSelNSD,"same");
  treeData->Draw("evtEta>>hEvtEta_data",dataSelCut,"same");

  TCanvas * c1 = new TCanvas("c1","c1",500,500);
  treeMC->Draw("SumEaddEp>>hEaddEp_pythia",mcSelCut,"E");
  treeMC->Draw("SumEaddEp>>hEaddEp_pythia_SD",mcSelSD,"same");
  treeMC->Draw("SumEaddEp>>hEaddEp_pythia_NSD",mcSelNSD,"same");
  treeData->Draw("SumEaddEp>>hEaddEp_data",dataSelCut,"same E");

  // calc chi2
  Int_t N=50;
  TH1D * hChi2 = new TH1D("hChi2",";#chi^{2};",N,0,1);
  Float_t step = 1./(Float_t)N;
  for (Int_t i=1; i<=N; ++i) {
    Double_t sdFrac = i*step;
    Double_t chi2 = histDiffrChi2(
	sdFrac,
	McSelSDFrac,
	"hEaddEp_data",
	"hEaddEp_pythia",
	"hEaddEp_pythia_SD",
	"hEaddEp_pythia_NSD"); 
    hChi2->SetBinContent(i,chi2);
  }

  TCanvas * cChi2 = new TCanvas("cChi2","cChi2",600,600);
  hChi2->Draw();
  TF1 *myfun = new TF1("myfun","[1]*(x-[0])*(x-[0])+[2]");
  myfun->SetParameters(0.2,0.01,0);
  hChi2->Fit("myfun","LL");
  cout << "Best SD fraction: " << myfun->GetParameter(0) << endl;

  // draw distributions
  TCanvas * cEaddPz = new TCanvas("cEaddPz","cEaddPz",600,600);
  histDiffrChi2(
      myfun->GetParameter(0),
      McSDFrac,
      "hEaddEp_data",
      "hEaddEp_pythia",
      "hEaddEp_pythia_SD",
      "hEaddEp_pythia_NSD",
      1,
      0.035);
  TCanvas * cEvtEta = new TCanvas("cEvtEta","cEvtEta",600,600);
  histDiffrChi2(
      myfun->GetParameter(0),
      McSDFrac,
      "hEvtEta_data",
      "hEvtEta_pythia",
      "hEvtEta_pythia_SD",
      "hEvtEta_pythia_NSD",
      1,
      1);
}
