#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <cmath>
#include <string>
#include <vector>
#include <TH2.h>
#include <TPaletteAxis.h>
#include <TCanvas.h>
#include "compareHist.h"

using namespace std;

void compareBsc(TTree* BscTree, TTree* BscTree2,const char *cut,const char *title, char *projectTitle)
{
  string title1=string(title)+", bsc 100%";
  string title2=string(title)+", bsc 95%";

  TCanvas *c2 = new TCanvas(Form("cnhit_%s",title),"",400,400);
  compareHist comp2(BscTree,BscTree2,"nhit",cut,0,32,32,1);
  comp2.SetHistName1(title1);
  comp2.SetHistName2(title2);
  comp2.SetXTitle("N_{hits} (any segment)");
  comp2.SetYTitle("Arbitrary normalization");
  comp2.SetMarkerSize(0);
  comp2.SetLegend(0.5,0.7,0.9,0.9);
  comp2.Draw2("hist");
  c2->Print(Form("cnhit_%s.gif",title));

  TCanvas *c3 = new TCanvas(Form("cnzp_%s",title),"",400,400);
  compareHist comp3(BscTree,BscTree2,"nzp",cut,0,16,16,1);
  comp3.SetHistName1(title1);
  comp3.SetHistName2(title2);
  comp3.SetXTitle("N_{hits} (z>0)");
  comp3.SetYTitle("Arbitrary normalization");
  comp3.SetMarkerSize(0);
  comp3.SetLegend(0.5,0.7,0.9,0.9);
  comp3.Draw2("hist");
  c3->Print(Form("cnzp_%s.gif",title));

  TCanvas *cpm = new TCanvas(Form("cplus_minus_%s",title), Form("cplus_minus_%s",title),600,600);
  TH2D *hpm = new TH2D(Form("hnplus_minus_%s",title),"",16,0,16,16,0,16);
  BscTree->Draw(Form("nzm:nzp>>hnplus_minus_%s",title),cut);
  hpm->SetXTitle("BSC N_{hits} (z>0)");
  hpm->SetYTitle("BSC N_{hits} (z<0)");
  hpm->Draw("colz text");
  cpm->Print(Form("cnpm_%s.gif",title));

  TCanvas *c95pm = new TCanvas(Form("c95plus_minus_%s",title), Form("c95plus_minus_%s",title),600,600);
  TH2D *h95pm = new TH2D(Form("h95nplus_minus_%s",title),"",16,0,16,16,0,16);
  BscTree2->Draw(Form("nzm:nzp>>h95nplus_minus_%s",title),cut);
  h95pm->SetXTitle("BSC N_{hits} (z>0)");
  h95pm->SetYTitle("BSC N_{hits} (z<0)");
  h95pm->Draw("colz text");
  c95pm->Print(Form("c95npm_%s.gif",title));

   /*
   c1->Range(-3.609756,-1.910995,12.7561,10.60209);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(0);
   c1->SetTickx();
   c1->SetTicky();
   c1->SetLeftMargin(0.25);
   c1->SetRightMargin(0.1684054);
   c1->SetTopMargin(0.02);
   c1->SetBottomMargin(0.3);
   c1->SetFrameLineColor(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameLineColor(0);
   c1->SetFrameBorderMode(0);
   

   TPaletteAxis *palette = new TPaletteAxis(tsize*1.02,0,tsize*1.1,tsize,h);
   palette->SetLabelColor(1);
   palette->SetLabelFont(42);
   palette->SetLabelOffset(0.005);
   palette->SetLabelSize(0.045);
   palette->SetTitleOffset(1);
   palette->SetTitleSize(0.04);
   palette->SetFillColor(100);
   palette->SetFillStyle(1001);
   hpm->GetListOfFunctions()->Add(palette,"br");
   hpm->Draw("col text z");

   string fname(Form("plot_%s_%s.gif",projectTitle,title));
   c1->SaveAs(fname.c_str());
   c1->SaveAs(Form("plot_%s_%s.C",projectTitle,title));

   cout <<"<img src=\"%ATTACHURLPATH%/"<<fname<<"\" alt=\""<<fname<<"\" width='671'   height='478' />"<<endl;   
   */
}

void bscHitAna(char *infile="output/treeBscEff100.root",char* infile2="output/treeBscEff95.root", char *projectTitle = "pythis900GeV",string source="mc")
{
   // Load input
   TFile *inf = new TFile(infile);
   TTree *BscTree =  (TTree*) inf->Get("simBscDigis/ntBsc");
   BscTree->Print();
   TFile *inf2 = new TFile(infile2);
   TTree *BscTree2 =  (TTree*) inf2->Get("simBscDigis/ntBsc");

   // define event types
   vector<string> evtType;
   vector<string> evtTypeCut;
   evtType.push_back("inelastic"); evtTypeCut.push_back("1==1");
   if (source=="mc") {
     evtType.push_back("Single_Diffr"); evtTypeCut.push_back("(evtType==92||evtType==93)");
     //evtType.push_back("Double_Diffr"); evtTypeCut.push_back("(evtType==94)");
     evtType.push_back("Non_Single_Diffr"); evtTypeCut.push_back("(evtType!=92 && evtType!=93)");
     //evtType.push_back("Non_Diffr"); evtTypeCut.push_back("(evtType!=92 && evtType!=93 && evtType!=94)");
   }

   // Print out event type fractions
   cout <<"<pre>"<<endl;
   for (unsigned int i=0; i<evtType.size(); ++i) {
     int nSel = BscTree->GetEntries(evtTypeCut[i].c_str());
     cout <<std::right<<std::setw(20)<<evtType[i] << ": " <<nSel<<endl;
   }
   cout <<"</pre>"<<endl;

   // plot bsc segment info
   for (unsigned int i=0; i<evtType.size(); ++i) {
     compareBsc(BscTree,BscTree2,evtTypeCut[i].c_str(),evtType[i].c_str(),projectTitle);
   }
}
