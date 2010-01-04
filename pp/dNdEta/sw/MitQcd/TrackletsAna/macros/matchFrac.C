#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include <iostream>
#include <vector>
#include "../selectionCut.h"
using namespace std;

// === helpers ===
Double_t histChi2(TH1 * h1, TH1 *h2)
{
  Double_t sum=0;
  for (Int_t i=1; i<=h1->GetNbinsX(); ++i) {
    Double_t binDiff = h2->GetBinContent(i)-h1->GetBinContent(i);
    Double_t binChi2 = binDiff*binDiff;
    sum+=binChi2;
  }
  return sum;
}

Double_t histDiffrChi2(
    Double_t testSDFrac   = 0.11,
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
  // calc rel frac
  Double_t testNSDFrac = 1-testSDFrac;
  Double_t MCSDFrac = (Double_t)h1->GetEntries()/(Double_t)hMC->GetEntries();
  Double_t MCNSDFrac = (Double_t)h2->GetEntries()/(Double_t)hMC->GetEntries();
  Double_t SDRelFrac = testSDFrac/MCSDFrac;
  Double_t NSDRelFrac = testNSDFrac/MCNSDFrac;
  //printf("histDiffrChi2 - trial MC SD Frac(rel): %f(%f), trial NSD Frac(rel): %f(%f)\n",testSDFrac,SDRelFrac,testNSDFrac,NSDRelFrac);
  // scale
  h1->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  h2->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hMC->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hData->Scale(1./hData->GetEntries()/h1->GetBinWidth(1));

  // combine different processes in MC with given weights
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
    TLegend *leg2 = new TLegend(0.651,0.776,0.953,0.928,NULL,"brNDC");
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

void fillHist(const char* var,const char* hname,
    TTree * treeData, TTree * treeMC, TCut dataSel, TCut mcSel,
    const vector<TString> & etype, const vector<TCut> & etypeCut, vector<TString> & outHists)
{
  TCanvas * c = new TCanvas(Form("c%s",hname),Form("c%s",hname),500,500);

  TString hData = TString(hname)+"_data";
  printf("%s>>%s,%s\n",var,hData.Data(),TString(dataSel).Data());
  treeData->Draw(Form("%s>>%s",var,hData.Data()),dataSel);
  outHists.push_back(hData);

  for (Int_t i=0; i<etype.size(); ++i) {
    TString hMC = TString(hname)+"_pythia_"+etype[i];
    TCut mcCut = mcSel&&etypeCut[i];
    printf("%s>>%s,%s\n",var,hMC.Data(),TString(mcCut).Data());
    treeMC->Draw(Form("%s>>%s",var,hMC.Data()),mcCut,"same");
    outHists.push_back(hMC);
  }
}

// === Main function ===
void matchFrac(bool testMC = true, int doSel = 1,
    const char * datafname="pixelTree_merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210_SDRelFrac0.5.root",
    const char * mcfname="pixelTree_merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210.root")
{
  // get trees
  TFile * dataFile = new TFile(datafname);
  TFile * mcFile = new TFile(mcfname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);

  // trigger
  selectionCut mcSel(1,doSel);
  selectionCut dataSel(0,doSel);
  TCut mcSelCut = mcSel.Cut;
  TCut dataSelCut = dataSel.Cut;
  TCut SDCut = "evtType==92 || evtType==93";
  TCut DDCut = "evtType==94";
  TCut DFCut = SDCut || DDCut;
  TCut NSDCut = !SDCut;
  TCut NDCut = !DFCut;
  TCut mcSelSD = mcSelCut && SDCut;
  TCut mcSelNSD = mcSelCut && NSDCut;
  TCut mcSelDF = mcSelCut && DFCut;
  TCut mcSelND = mcSelCut && NDCut;
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSelCut) << endl;
  cout << "MC: " << TString(mcSelCut) << endl;
  cout << "MC SD: " << TString(mcSelSD) << endl;
  cout << "MC NSD: " << TString(mcSelNSD) << endl;
  cout << "MC DF: " << TString(mcSelDF) << endl;
  cout << "MC ND: " << TString(mcSelND) << endl;

  // get truth info
  Double_t McTotalN = treeMC->GetEntries();
  Double_t McSD = treeMC->GetEntries(SDCut);
  Double_t McSDFrac = McSD/McTotalN;
  Double_t McDF = treeMC->GetEntries(DFCut);
  Double_t McDFFrac = McDF/McTotalN;
  Double_t McSelTotalN = treeMC->GetEntries(mcSelCut);
  Double_t McSelSD = treeMC->GetEntries(mcSelSD);
  Double_t McSelSDFrac = McSelSD/McSelTotalN;
  Double_t McSelDF = treeMC->GetEntries(mcSelDF);
  Double_t McSelDFFrac = McSelDF/McSelTotalN;
  printf("\n===== MC Input =====\n");
  printf("MC input SD frac:%f, after selection MC SD frac: %f.\n",McSDFrac,McSelSDFrac);
  printf("MC input DF frac:%f, after selection MC DF frac: %f.\n",McDFFrac,McSelDFFrac);
  if (testMC) {
    dataSelCut=mcSelCut;
    Double_t DataTotalN = treeData->GetEntries();
    Double_t DataSD = treeData->GetEntries(SDCut);
    Double_t DataSDFrac = DataSD/DataTotalN;
    Double_t DataDF = treeData->GetEntries(DFCut);
    Double_t DataDFFrac = DataDF/DataTotalN;
    Double_t DataSelTotalN = treeData->GetEntries(mcSelCut);
    Double_t DataSelSD = treeData->GetEntries(mcSelSD);
    Double_t DataSelSDFrac = DataSelSD/DataSelTotalN;
    Double_t DataSelDF = treeData->GetEntries(mcSelDF);
    Double_t DataSelDFFrac = DataSelDF/DataSelTotalN;
    printf("\"Data\" input SD frac:%f, after selection \"Data\" SD frac: %f.\n",DataSDFrac,DataSelSDFrac);
    printf("\"Data\" input DF frac:%f, after selection \"Data\" DF frac: %f.\n",DataDFFrac,DataSelDFFrac);
  }

  // configuation
  // sources
  vector<TString> source;
  source.push_back("data");
  source.push_back("pythia");
  // colors
  vector<Color_t> color;
  color.push_back(kRed);
  color.push_back(kBlue);
  color.push_back(kGreen-1);
  // observables
  vector<TString> obs;
  obs.push_back("EvtEta");
  obs.push_back("EaddEp");
  obs.push_back("EsubEp");
  // event types
  vector<TString> etype;
  vector<TCut> etypeCut;
  etype.push_back("All"); etypeCut.push_back("1==1");
  etype.push_back("SD"); etypeCut.push_back("evtType==92 || evtType==93");
  etype.push_back("NSD"); etypeCut.push_back("evtType!=92 && evtType!=93");
  etype.push_back("DF"); etypeCut.push_back("evtType==92 || evtType==93 || evtType==94");
  etype.push_back("ND"); etypeCut.push_back("evtType!=92 && evtType!=93 && evtType!=94");
  // container for all declared histograms
  vector<TH1D*> vh1;

  //
  // declare histograms
  printf("now declare hists\n");
  const Double_t EPzMax=200;
  for (Int_t i=0; i<source.size(); ++i) {
    vh1.push_back(new TH1D(Form("hEvtEta_%s",source[i].Data()),";Event #eta;",100,-5,5));
    vh1.push_back(new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",100,0,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEp_%s",source[i].Data()),";#Sigma E-Pz;",100,0,EPzMax));
    if (source[i]=="pythia") {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";Event #eta;",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",100,0,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",100,0,EPzMax));
      }
    }
  }
  // set histograms
  for (Int_t ih1=0; ih1<vh1.size(); ++ih1) {
    //cout << "hist: " << vh1[ih1]->GetName() << " title: " << vh1[ih1]->GetXaxis()->GetTitle() << endl;
    vh1[ih1]->Sumw2();
  }

  // Fill histos
  vector<TString> evtEtaHists;
  fillHist("evtEta","hEvtEta",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,evtEtaHists);
  vector<TString> EaddEpHists;
  fillHist("SumEaddEp","hEaddEp",treeData,treeMC,dataSel.Cut,mcSel.Cut,etype,etypeCut,EaddEpHists);

  // calc chi2
  printf("\n=========== Chi2 clac ================\n");
  Double_t maxSDFrac=0.5;
  Int_t N=50;
  TH1D * hChi2 = new TH1D("hChi2",";SD Fraction;#chi^{2}",N,0,maxSDFrac);
  Double_t step = maxSDFrac/(Float_t)N;
  for (Int_t i=1; i<=N; ++i) {
    Double_t sdFrac = i*step;
    Double_t chi2 = histDiffrChi2(
	sdFrac,
	"hEaddEp_data",
	"hEaddEp_pythia_All",
	"hEaddEp_pythia_DF",
	"hEaddEp_pythia_ND"); 
    hChi2->SetBinContent(i,chi2);
  }

  TCanvas * cChi2 = new TCanvas("cChi2","cChi2",600,600);
  hChi2->Draw();
  TF1 *myfun = new TF1("myfun","[1]*(x-[0])*(x-[0])+[2]");
  myfun->SetParameters(0.1,0.001,0);
  hChi2->Fit("myfun","LL");
  cout << "Best SD fraction: " << myfun->GetParameter(0) << endl;
  if (testMC) {
    Double_t DataSelTotalN = treeData->GetEntries(mcSelCut);
    Double_t DataSelDF = treeData->GetEntries(mcSelDF);
    Double_t DataSelDFFrac = DataSelDF/DataSelTotalN;
    TLine * l = new TLine(DataSelDFFrac,hChi2->GetMinimum(),DataSelDFFrac,hChi2->GetMaximum());
    l->SetLineColor(2);
    l->Draw("same");
  }
  cChi2->Print(Form("plots/%s_cChi2_Sel%d.gif",datafname,doSel));

  // draw distributions
  TCanvas * cEaddPz = new TCanvas("cEaddPz","cEaddPz",600,600);
  histDiffrChi2(
      myfun->GetParameter(0),
      "hEaddEp_data",
      "hEaddEp_pythia_All",
      "hEaddEp_pythia_DF",
      "hEaddEp_pythia_ND",
      1,
      0.035);
  cEaddPz->Print(Form("plots/%s_cEaddPz_Sel%d.gif",datafname,doSel));
  TCanvas * cEvtEta = new TCanvas("cEvtEta","cEvtEta",600,600);
  histDiffrChi2(
      myfun->GetParameter(0),
      "hEvtEta_data",
      "hEvtEta_pythia_All",
      "hEvtEta_pythia_DF",
      "hEvtEta_pythia_ND",
      0,
      1);
  cEvtEta->Print(Form("plots/%s_cEvtEta_Sel%d.gif",datafname,doSel));
}
