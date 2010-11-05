#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH2.h"
#include "TPaletteAxis.h"
#include "TCanvas.h"
#include "TSystem.h"

using namespace std;

vector <string>* triggers = new vector<string>;
vector <string>* triggerCuts = new vector<string>;
vector< vector <double*> >* results = new vector< vector <double*> >;
TString goutdir;


double calcEffErr(double nEvt,double nAcc)
{
  double eff =  nAcc / nEvt;
  return sqrt(eff*(1-eff)/nEvt);
}

double* calcEff(TTree* HltTree, const char *title, double nEvt,const char *cut,int flag=1)
{
  double nAcc = HltTree->GetEntries(cut);
  double effErr = calcEffErr(nEvt,nAcc);
  double eff = nAcc/nEvt;
  double *result = new double[2];
  result[0]=eff;
  result[1]=effErr;

  // print out or not
  if (flag) {
    triggers->push_back(string(title));
    triggerCuts->push_back(string(cut));
    cout <<" | "<<setw(20)<<title;
    cout <<" | "<<setw(10)<<fixed<<setprecision(3)<<eff*100.<<"% +-";
    cout        <<setw(10)<<fixed<<setprecision(3)<<effErr*100.<<"%";
    cout <<" | "<<endl;
  }

  return result;
}

void printEff(TTree* HltTree,const char *cut,const char *title, char *projectTitle)
{
  cout <<"   * "<<title<<":"<<endl;
  cout <<"      * Efficiencies:"<<endl;
  double nEvt = HltTree->GetEntries(cut);

  cout <<" | "<<setw(20)<<"HLT Path";
  cout <<" | "<<setw(25)<<"Efficiency";
  cout <<" | "<<endl;

  triggers->clear();
  triggerCuts->clear();

  vector <double*> effs;

  // calculate the efficiency //   
  effs.push_back(calcEff(HltTree,"AllPhysics",nEvt,Form("(%s)&&1==1",cut)));
  // -- unprotected --
  effs.push_back(calcEff(HltTree,"L1_BscMinBiasThreshold1",nEvt,Form("(%s)&&L1_BscMinBiasThreshold1",cut)));
  effs.push_back(calcEff(HltTree,"L1_HcalHfCoincidencePm",nEvt,Form("(%s)&&L1_HcalHfCoincidencePm",cut)));
  effs.push_back(calcEff(HltTree,"L1_ETT30",nEvt,Form("(%s)&&L1_ETT30",cut)));
  effs.push_back(calcEff(HltTree,"L1_ZdcCaloPlus_ZdcCaloMinus",nEvt,Form("(%s)&&L1_ZdcCaloPlus_ZdcCaloMinus",cut)));
  // -- protected w/ bptx --
  effs.push_back(calcEff(HltTree,"L1_BscMinBiasOR_BptxAND",nEvt,Form("(%s)&&L1_BscMinBiasOR_BptxAND",cut)));
  effs.push_back(calcEff(HltTree,"L1_BscMinBiasThreshold1_BptxAND",nEvt,Form("(%s)&&L1_BscMinBiasThreshold1_BptxAND",cut)));
  effs.push_back(calcEff(HltTree,"L1_HcalHfCoincidencePm_BptxAND",nEvt,Form("(%s)&&L1_HcalHfCoincidencePm_BptxAND",cut)));
  effs.push_back(calcEff(HltTree,"L1_ZdcCaloPlus_ZdcCaloMinus_BptxAND",nEvt,Form("(%s)&&L1_ZdcCaloPlus_ZdcCaloMinus_BptxAND",cut)));
  effs.push_back(calcEff(HltTree,"L1_ZdcScintLooseVertex_BptxAND",nEvt,Form("(%s)&&L1_ZdcScintLooseVertex_BptxAND",cut)));
  // -- protected w/ bsc2 veto --
  effs.push_back(calcEff(HltTree,"L1_NotBsc2_BscMinBiasOR",nEvt,Form("(%s)&&L1_NotBsc2_BscMinBiasOR",cut)));
  effs.push_back(calcEff(HltTree,"L1_NotBsc2_BscMinBiasThreshold1",nEvt,Form("(%s)&&L1_NotBsc2_BscMinBiasThreshold1",cut)));
  effs.push_back(calcEff(HltTree,"L1_NotBsc2_HcalHfCoincidencePm",nEvt,Form("(%s)&&L1_NotBsc2_HcalHfCoincidencePm",cut)));
  // -- protected w/ bptx and bsc2 veto --
  effs.push_back(calcEff(HltTree,"L1_NotBsc2_BptxAND_BscMinBiasOR",nEvt,Form("(%s)&&L1_NotBsc2_BptxAND_BscMinBiasOR",cut)));
  effs.push_back(calcEff(HltTree,"L1_NotBsc2_BptxAND_BscMinBiasThreshold1",nEvt,Form("(%s)&&L1_NotBsc2_BptxAND_BscMinBiasThreshold1",cut)));
  effs.push_back(calcEff(HltTree,"L1_NotBsc2_BptxAND_HcalHfCoincidencePm",nEvt,Form("(%s)&&L1_NotBsc2_BptxAND_HcalHfCoincidencePm",cut)));
  results->push_back(effs);

  cout <<"      * Correlation Matrix:"<<endl;
  int tsize = (int)triggers->size();
  TH2D *h = new TH2D(Form("h%s",title),"",tsize,0,tsize,tsize,0,tsize);
  TH2D *hct = new TH2D(Form("h%s_ct",title),"",tsize,0,tsize,tsize,0,tsize);

  for (int i=tsize-1;i>=0;i--){
    int nEvtAfterCut = HltTree->GetEntries((*triggerCuts)[i].c_str());
    h->GetXaxis()->SetBinLabel(i+1,(*triggers)[i].c_str());
    h->GetYaxis()->SetBinLabel(i+1,(*triggers)[i].c_str());
    hct->GetXaxis()->SetBinLabel(i+1,(*triggers)[i].c_str());
    hct->GetYaxis()->SetBinLabel(i+1,(*triggers)[i].c_str());
    for (int j=0;j<tsize;j++){
      string cut ="("+(*triggerCuts)[i]+")&&("+(*triggerCuts)[j]+")";
      double* eff = calcEff(HltTree,"",nEvtAfterCut,Form("%s",cut.c_str()),0);
      if (nEvtAfterCut==0) eff[0]=0;
      h->SetBinContent(i+1,j+1,int(eff[0]*1000)/10.);
      hct->SetBinContent(i+1,j+1,HltTree->GetEntries(cut.c_str()));
    }
  }
  h->GetXaxis()->LabelsOption("v");
  h->SetLabelSize(0.03,"XY");
  hct->GetXaxis()->LabelsOption("v");

  TCanvas *c1 = new TCanvas(Form("c%s",title), Form("c_%s",title),1000,800);
  c1->Range(-3.609756,-1.910995,12.7561,10.60209);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(0);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(0.37);
  c1->SetRightMargin(0.1684054);
  c1->SetTopMargin(0.02);
  c1->SetBottomMargin(0.4);
  c1->SetFrameLineColor(0);
  c1->SetFrameBorderMode(0);
  c1->SetFrameLineColor(0);
  c1->SetFrameBorderMode(0);

  h->Draw("col text");

  TPaletteAxis *palette = new TPaletteAxis(tsize*1.02,0,tsize*1.1,tsize,h);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.005);
  palette->SetLabelSize(0.045);
  palette->SetTitleOffset(1);
  palette->SetTitleSize(0.04);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);
  h->GetListOfFunctions()->Add(palette,"br");
  h->Draw("col text z");

  string fname(Form("%s/trigCorr_%s_%s.gif",goutdir.Data(),projectTitle,title));
  c1->SaveAs(fname.c_str());
  c1->SaveAs(Form("%s/trigCorr_%s_%s.C",goutdir.Data(),projectTitle,title));

  cout <<"<img src=\"%ATTACHURLPATH%/"<<fname<<"\" alt=\""<<fname<<"\" width='671'   height='478' />"<<endl;   
}

void trigAnaCorrelation(
    TString inFile0Name="../outputs/oh_wei_391_hil1_1k.root",
    TString outdir = "out/Hydj_HiL1/cerHI_HLT_Test391_V0",
    char *projectTitle = "Hydjet2760GeV",
    string source="mc")
{
  // Load input
  TChain * HltTree = new TChain("hltanalysis/HltTree","HI OpenHLT Tree");
  HltTree->Add(inFile0Name);
  cout << " # entries: " << HltTree->GetEntries() << endl;

  gSystem->mkdir(outdir.Data(),kTRUE);
  goutdir=outdir;
  TFile *outf = new TFile(Form("%s/%s_hist.root",outdir.Data(),projectTitle),"RECREATE");

  // define event types
  vector<string> evtType;
  vector<string> evtTypeCut;
  evtType.push_back("AllPhysics"); evtTypeCut.push_back("1==1");
  if (source=="mc") {
  }
  else if (source=="data") {
  }

  // Print out event type fractions
  cout <<"<pre>"<<endl;
  for (unsigned int i=0; i<evtType.size(); ++i) {
    int nSel = HltTree->GetEntries(evtTypeCut[i].c_str());
    cout <<std::right<<std::setw(20)<<evtType[i] << ": " <<nSel<<endl;
  }
  cout <<"</pre>"<<endl;

  // Calc Efficiencies
  for (unsigned int i=0; i<evtType.size(); ++i) {
    printEff(HltTree,evtTypeCut[i].c_str(),evtType[i].c_str(),projectTitle);
  }

  // Print efficiency results
  cout <<" | "<<setw(20)<<" ";
  for (unsigned int i=0;i<evtType.size();i++) {
    cout <<" | "<<setw(8)<<evtType[i];
  }
  cout <<" | " <<endl;

  for (int i=0;i<(int)triggers->size();i++){
    cout <<" | " <<setw(20)<<(*triggers)[i];
    for (int j=0;j<(int)results->size();j++) {
      double eff=(((*results)[j])[i])[0];
      double effErr=(((*results)[j])[i])[1];
      cout <<" | "<<setw(10)<<fixed<<setprecision(3)<<eff*100.<<"% +-";
      cout        <<setw(10)<<fixed<<setprecision(3)<<effErr*100.<<"% ";
    }
    cout <<" | "<<endl;
  }   

  // save
  outf->Write();
}
