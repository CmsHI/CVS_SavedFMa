#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TSystem.h"
#include <iostream>
#include <vector>
#include "../selectionCut.h"
using namespace std;

// === Helpers ===
void calcTrigEff(TTree * tree, TString Source, TCut baseSel,
    const vector<TString> & etype, const vector<TCut> & etypeCut,
    Int_t wantType=3)
{
  cout << "  --- Trig Eff for " << Source << " ---" << endl;
  Double_t nTotal, nTrig, nTrigSel, nTypeTotal, nTypeTrig, nTypeTrigSel;

  // base cuts
  TCut All=etypeCut[0];
  if (Source=="data") All="1==1"; 

  TCut trigCut=baseSel && All;
  TCut selCut="nHFn>0&&nHFp>0";
  TCut trigSelCut=trigCut && selCut;

  cout << "Base Selection: " << TString(trigCut) << endl;
  cout << "Base+HF_Coinc: " << TString(trigSelCut) << endl;

  nTotal=tree->GetEntries(All);
  nTrig=tree->GetEntries(trigCut);
  nTrigSel=tree->GetEntries(trigSelCut);
  cout << "Base Selection Eff: " << nTrig/nTotal << "   Base+HF_Coinc/Base Eff: " << nTrigSel/nTrig << endl << endl;
  if (Source=="data") return;

  // MC only
  TCut trigTypeCut=trigCut&&etypeCut[wantType];
  TCut trigTypeSelCut=trigTypeCut && selCut; 
  cout << etype[wantType] << " Base Selection: " << TString(trigTypeCut) << endl;
  cout << etype[wantType] << " Base+HF_Coinc: " << TString(trigTypeSelCut) << endl;

  nTypeTotal=tree->GetEntries(etypeCut[wantType]);
  nTypeTrig=tree->GetEntries(trigTypeCut);
  nTypeTrigSel=tree->GetEntries(trigTypeSelCut);

  cout << "Type: " << etype[wantType] << "  " << etypeCut[wantType] << endl;
  cout << "Truth Frac: " << nTypeTotal/nTotal << endl;
  cout << "  After base sel: " << nTypeTrig/nTrig << "  After base+HF coinc: " << nTypeTrigSel/nTrigSel << endl;
  cout << etype[wantType] << " Sel Eff: " << nTypeTrig/nTypeTotal << "   DF - Base+HF_Coinc/Base Eff: " << nTypeTrigSel/nTypeTrig << endl;
  return;
}

// === Main function ===
void compare(int evtType = 0, int doSel = 1,
    const char * datafname="../input/pixelTree_124022a3a4-vtxcomp_MB.root",
    const char * mcfname="pixelTree_Pythia_MinBias_D6T_900GeV_d20091229_Vertex1229.root",
    const char * mc2fname="pixelTree_Phojet_MinBias_900GeV_d20100108.root")
{
  // top level info
  TString InspectTag = Form("Sel%d_type%d",doSel,evtType);
  cout << "====== Inspect: " << InspectTag << endl;
  // mkdir dir for output
  TString outdir=Form("plots/inspection");
  gSystem->mkdir(outdir.Data(),kTRUE);

  // get trees
  cout << "data: " << datafname << endl;
  cout << "mc1: " << mcfname << endl;
  cout << "mc2: " << mc2fname << endl;
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
  const Double_t EPzMin=0;
  const Double_t EPzMax=600;
  const Int_t EPzNBINS=EPzMax/5.;
  for (Int_t i=0; i<source.size(); ++i) {
    vh1.push_back(new TH1D(Form("hEvtEta_%s",source[i].Data()),";Event #eta;",100,-5,5));
    vh1.push_back(new TH1D(Form("hEaddEp_%s",source[i].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEp_%s",source[i].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEaddEpPos_%s",source[i].Data()),";#Sigma E+Pz (HF+);",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hEsubEpNeg_%s",source[i].Data()),";#Sigma E-Pz (HF-);",EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH2D(Form("hEPz_%s",source[i].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
    vh1.push_back(new TH1D(Form("hVz_%s",source[i].Data()),";vz;",100,-20,20));
    if (source[i]=="pythia"||source[i]=="phojet") {
      for (Int_t j=0; j<etype.size(); ++j) {
	vh1.push_back(new TH1D(Form("hEvtEta_%s_%s",source[i].Data(),etype[j].Data()),";Event #eta;",100,-5,5));
	vh1.push_back(new TH1D(Form("hEaddEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEp_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz;",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEaddEpPos_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz (HF+);",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH1D(Form("hEsubEpNeg_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E-Pz (HF-);",EPzNBINS,EPzMin,EPzMax));
	vh1.push_back(new TH2D(Form("hEPz_%s_%s",source[i].Data(),etype[j].Data()),";#Sigma E+Pz;E-Pz",EPzNBINS,EPzMin,EPzMax,EPzNBINS,EPzMin,EPzMax));
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
      vh1[ih1]->SetLineColor(kRed);
      vh1[ih1]->SetMarkerStyle(kOpenSquare);
    }
    if (TString(vh1[ih1]->GetName()).Contains("phojet")) {
      vh1[ih1]->SetMarkerColor(kBlue);
      vh1[ih1]->SetLineColor(kBlue);
      vh1[ih1]->SetMarkerStyle(kOpenStar);
    }
    if (TString(vh1[ih1]->GetName()).Contains("DF")) {
      vh1[ih1]->SetLineStyle(2);
    }
    if (TString(vh1[ih1]->GetName()).Contains("ND")) {
      vh1[ih1]->SetLineStyle(7);
    }
  }

  // Fill histos
  printf("\n===== Triggering =====\n");
  cout << "Data: " << TString(dataSel.Cut) << endl;
  cout << "MC: " << TString(mcSel.Cut&&etypeCut[evtType]) << endl;
  cout << "MC2: " << TString(mcSel.Cut&&etypePhojCut[evtType]) << endl;
  // EPz
  cout << "draw: hEaddEp_pythia: " << treeMC->Draw("SumEaddEp>>hEaddEp_pythia",mcSel.Cut&&etypeCut[evtType],"E") << endl;;
  cout << "draw: hEaddEp_pythia_DF: " << treeMC->Draw("SumEaddEp>>hEaddEp_pythia_DF",mcSel.Cut&&etypeCut[3],"Esame") << endl;;
  cout << "draw: hEaddEp_pythia_ND: " << treeMC->Draw("SumEaddEp>>hEaddEp_pythia_ND",mcSel.Cut&&etypeCut[4],"Esame") << endl;;
  cout << "draw: hEaddEp_phojet: " << treeMC2->Draw("SumEaddEp>>hEaddEp_phojet",mcSel.Cut&&TCut(etypePhojCut[evtType]),"Esame") << endl;;
  cout << "draw: hEaddEp_phojet_DF: " << treeMC2->Draw("SumEaddEp>>hEaddEp_phojet_DF",mcSel.Cut&&TCut(etypePhojCut[3]),"Esame") << endl;;
  cout << "draw: hEaddEp_phojet_ND: " << treeMC2->Draw("SumEaddEp>>hEaddEp_phojet_ND",mcSel.Cut&&TCut(etypePhojCut[4]),"Esame") << endl;;
  cout << "draw: hEaddEp_data: " << treeData->Draw("SumEaddEp>>hEaddEp_data",dataSel.Cut,"Esame") << endl;;
  // EPz one sided
  cout << "draw: hEaddEpPos_pythia: " << treeMC->Draw("SumEaddEpPos>>hEaddEpPos_pythia",mcSel.Cut&&etypeCut[evtType],"Esame") << endl;;
  cout << "draw: hEaddEpPos_phojet: " << treeMC2->Draw("SumEaddEpPos>>hEaddEpPos_phojet",mcSel.Cut&&TCut(etypePhojCut[evtType]),"Esame") << endl;;
  cout << "draw: hEaddEpPos_data: " << treeData->Draw("SumEaddEpPos>>hEaddEpPos_data",dataSel.Cut,"Esame") << endl;;
  // calc trigger eff
  printf("\n===== Trig Eff =====\n");
  Int_t calcEvtType=3; // All=0, SD=1, NSD=2, DF=3, ND=4
  calcTrigEff(treeData,"data",dataSel.Cut,etype,etypeCut,calcEvtType);
  calcTrigEff(treeMC,"pythia",mcSel.Cut,etype,etypeCut,calcEvtType);
  calcTrigEff(treeMC2,"phojet",mcSel.Cut,etype,etypePhojCut,calcEvtType);
  printf("======================================\n\n");

  // get histograms
  TH1D * hEaddEp_data = (TH1D*)gDirectory->FindObject("hEaddEp_data");
  printf("%s: %f entries\n",hEaddEp_data->GetName(),hEaddEp_data->GetEntries());
  TH1D * hEaddEp_pythia = (TH1D*)gDirectory->FindObject("hEaddEp_pythia");
  printf("%s: %f entries\n",hEaddEp_pythia->GetName(),hEaddEp_pythia->GetEntries());
  TH1D * hEaddEp_pythia_DF = (TH1D*)gDirectory->FindObject("hEaddEp_pythia_DF");
  printf("%s: %f entries\n",hEaddEp_pythia_DF->GetName(),hEaddEp_pythia_DF->GetEntries());
  TH1D * hEaddEp_pythia_ND = (TH1D*)gDirectory->FindObject("hEaddEp_pythia_ND");
  printf("%s: %f entries\n",hEaddEp_pythia_ND->GetName(),hEaddEp_pythia_ND->GetEntries());
  TH1D * hEaddEp_phojet = (TH1D*)gDirectory->FindObject("hEaddEp_phojet");
  printf("%s: %f entries\n",hEaddEp_phojet->GetName(),hEaddEp_phojet->GetEntries());
  TH1D * hEaddEp_phojet_DF = (TH1D*)gDirectory->FindObject("hEaddEp_phojet_DF");
  printf("%s: %f entries\n",hEaddEp_phojet_DF->GetName(),hEaddEp_phojet_DF->GetEntries());
  TH1D * hEaddEp_phojet_ND = (TH1D*)gDirectory->FindObject("hEaddEp_phojet_ND");
  printf("%s: %f entries\n",hEaddEp_phojet_ND->GetName(),hEaddEp_phojet_ND->GetEntries());

  TH1D * hEaddEpPos_data = (TH1D*)gDirectory->FindObject("hEaddEpPos_data");
  printf("%s: %f entries\n",hEaddEpPos_data->GetName(),hEaddEpPos_data->GetEntries());
  TH1D * hEaddEpPos_pythia = (TH1D*)gDirectory->FindObject("hEaddEpPos_pythia");
  printf("%s: %f entries\n",hEaddEpPos_pythia->GetName(),hEaddEpPos_pythia->GetEntries());
  TH1D * hEaddEpPos_phojet = (TH1D*)gDirectory->FindObject("hEaddEpPos_phojet");
  printf("%s: %f entries\n",hEaddEpPos_phojet->GetName(),hEaddEpPos_phojet->GetEntries());

  // declare legend
  TLegend *leg2 = new TLegend(0.679,0.802,0.929,0.93,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->AddEntry(hEaddEp_data,"data","p");
  leg2->AddEntry(hEaddEp_pythia,Form("default pythia %s",etype[evtType].Data()),"p");
  leg2->AddEntry(hEaddEp_phojet,Form("default phojet %s",etype[evtType].Data()),"p");

  // draw from tree
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  treeData->Draw("vz[1]>>hVz_data",dataSel.Cut,"");
  treeMC->Draw("vz[1]>>hVz_pythia",mcSel.Cut&&etypeCut[evtType],"same");
  treeMC2->Draw("vz[1]>>hVz_phojet",mcSel.Cut&&TCut(etypePhojCut[evtType]),"same");
  leg2->Draw();

  //scale
  for (Int_t ih1=0; ih1<vh1.size(); ++ih1) {
    vh1[ih1]->Scale(1./vh1[ih1]->GetEntries()/vh1[ih1]->GetBinWidth(1));
  }
  c2->Update();
  c2->Print(Form("%s/%s_hVz.gif",outdir.Data(),InspectTag.Data()));

  // draw from hists
  TCanvas * cEPz = new TCanvas("cEPz","cEPz",500,500);
  cEPz->SetLogy();
  Double_t EPzYMax = 0.018;
  if (doSel==4) EPzYMax = 0.007;
  if (doSel==10) EPzYMax = 0.12;
  hEaddEpPos_pythia->SetMinimum(0.0001);
  hEaddEpPos_pythia->SetMaximum(EPzYMax);
  hEaddEpPos_pythia->Draw("E");
  hEaddEpPos_phojet->Draw("Esame");
  hEaddEpPos_data->Draw("Esame");
  leg2->Draw();

  cEPz->Print(Form("%s/%s_hEaddEpPos.gif",outdir.Data(),InspectTag.Data()));
}
