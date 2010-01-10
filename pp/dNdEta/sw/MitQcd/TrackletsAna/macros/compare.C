#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
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

// === Main function ===
void compare(int evtType = 0, int doSel = 4,
    const char * datafname="PixelTree-124022-hfcuts.root",
    const char * mcfname="pixelTree_Pythia_MinBias_D6T_900GeV_d20091229_Vertex1229.root",
    const char * mc2fname="pixelTree_Phojet_MinBias_900GeV_d20100108.root")
{
  // get trees
  TFile * dataFile = new TFile(datafname);
  TFile * mcFile = new TFile(mcfname);
  TFile * mc2File = new TFile(mc2fname);
  TTree * treeData; dataFile->GetObject("PixelTree",treeData);
  TTree * treeMC;   mcFile->GetObject("PixelTree",treeMC);
  TTree * treeMC2; mc2File->GetObject("PixelTree",treeMC2);

  // trigger
  selectionCut mcSel(1,doSel,124023,41,96);
  selectionCut dataSel(0,doSel,124023,41,96);

  // configuation
  // sources
  vector<TString> source;
  source.push_back("data");
  source.push_back("pythia");
  source.push_back("phojet");
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
  vector<TCut> etypePhojCut;
  etype.push_back("All"); etypeCut.push_back("1==1");
  etype.push_back("SD"); etypeCut.push_back("evtType==92 || evtType==93");
  etype.push_back("NSD"); etypeCut.push_back("evtType!=92 && evtType!=93");
  etype.push_back("DF"); etypeCut.push_back("evtType==92 || evtType==93 || evtType==94");
  etype.push_back("ND"); etypeCut.push_back("evtType!=92 && evtType!=93 && evtType!=94");

  etypePhojCut.push_back("evtType==1 || evtType==5 || evtType==6 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==5 || evtType==6");
  etypePhojCut.push_back("evtType==1 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==5 || evtType==6 || evtType==7 || evtType==4");
  etypePhojCut.push_back("evtType==1");
  // container for all declared histograms
  vector<TH1*> vh1;

  //
  // declare histograms
  printf("now declare hists\n");
  const Double_t EPzMax=700;
  const Int_t EPzNBINS=EPzMax/5.;
  for (Int_t i=0; i<source.size(); ++i) {
    vh1.push_back(new TH1D(Form("hEvtEta_%s",source[i].Data()),";Event #eta;",100,-5,5));
    vh1.push_back(new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",EPzNBINS,0,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEp_%s",source[i].Data()),";#Sigma E-Pz;",EPzNBINS,0,EPzMax));
    vh1.push_back(new TH2D(Form("hEPz_%s",source[i].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,0,EPzMax,EPzNBINS,0,EPzMax));
    vh1.push_back(new TH1D(Form("hVz_%s",source[i].Data()),";vz;",100,-20,20));
    if (source[i]=="pythia"||source[i]=="phojet") {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";Event #eta;",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",EPzNBINS,0,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",EPzNBINS,0,EPzMax));
	vh1.push_back(new TH2D(Form("hEPz_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,0,EPzMax,EPzNBINS,0,EPzMax));
	vh1.push_back(new TH1D(Form("hVz_%s_%s",source[i].Data(),etype[j].Data()),";vz;",100,-20,20));
      }
    }
  }
  // set histograms
  for (Int_t ih1=0; ih1<vh1.size(); ++ih1) {
    //cout << "hist: " << vh1[ih1]->GetName() << " title: " << vh1[ih1]->GetXaxis()->GetTitle() << endl;
    vh1[ih1]->Sumw2();
    if (TString(vh1[ih1]->GetName()).Contains("pythia")) {
      vh1[ih1]->SetMarkerColor(kRed);
    }
    if (TString(vh1[ih1]->GetName()).Contains("phojet")) {
      vh1[ih1]->SetMarkerColor(kBlue);
    }
  }

  // Fill histos
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSel.Cut) << endl;
  cout << "MC: " << TString(mcSel.Cut&&etypeCut[evtType]) << endl;
  cout << "MC2: " << TString(mcSel.Cut&&etypePhojCut[evtType]) << endl;
  treeMC->Draw("SumEaddEp>>hEaddEp_pythia",mcSel.Cut&&etypeCut[evtType]);
  treeMC2->Draw("SumEaddEp>>hEaddEp_phojet",mcSel.Cut&&TCut(etypePhojCut[evtType]),"same");
  treeData->Draw("SumEaddEp>>hEaddEp_data",dataSel.Cut,"same");

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  TH1D * hEaddEp_pythia = (TH1D*)gDirectory->FindObject("hEaddEp_pythia");
  printf("%s: %f entries\n",hEaddEp_pythia->GetName(),hEaddEp_pythia->GetEntries());
  hEaddEp_pythia->SetMinimum(0);
  hEaddEp_pythia->SetMaximum(0.006);
  hEaddEp_pythia->Draw("E");

  TH1D * hEaddEp_phojet = (TH1D*)gDirectory->FindObject("hEaddEp_phojet");
  printf("%s: %f entries\n",hEaddEp_phojet->GetName(),hEaddEp_phojet->GetEntries());
  hEaddEp_phojet->Draw("Esame");

  TH1D * hEaddEp_data = (TH1D*)gDirectory->FindObject("hEaddEp_data");
  printf("%s: %f entries\n",hEaddEp_data->GetName(),hEaddEp_data->GetEntries());
  if (evtType==0) hEaddEp_data->Draw("Esame");


  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  treeMC->Draw("vz[1]>>hVz_pythia",mcSel.Cut&&etypeCut[evtType],"");
  treeMC2->Draw("vz[1]>>hVz_phojet",mcSel.Cut&&TCut(etypePhojCut[evtType]),"same");
  treeData->Draw("vz[1]>>hVz_data",dataSel.Cut,"same");

  //scale
  for (Int_t ih1=0; ih1<vh1.size(); ++ih1) {
    vh1[ih1]->Scale(1./vh1[ih1]->GetEntries()/vh1[ih1]->GetBinWidth(1));
  }
  c0->Update();
  c2->Update();
  c0->Print("plots/compare_hEaddEp.gif");
}
