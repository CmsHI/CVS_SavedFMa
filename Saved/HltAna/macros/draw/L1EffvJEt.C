#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TString.h"
#include "TLegend.h"
#include "TLine.h"
using namespace std;

void L1EffvJEt(TString infname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv1/merge/all.root")
{
  TH1::SetDefaultSumw2();
  TFile * inf = new TFile(infname);
  
  TTree * t = (TTree*)inf->Get("hltbitnew/HltTree");
  t->AddFriend("l1NtupleProducer/L1Tree");
  t->AddFriend("icPu5JetAnalyzer/t");
  t->AddFriend("tjt","../anant/results_alljettype_hil1mctag_eta5000.root");
  t->SetAlias("mul","RankETT*0.5");
  
  TCanvas * cchk = new TCanvas("cchk","cchk",500,500);
  t->Draw("mul","","hist");
  t->Draw("l1ett","","sameE");
  
  TProfile * h36 = new TProfile("h36","",50,0,150);
  TProfile * h52 = new TProfile("h52","",50,0,150);
  TProfile * h68 = new TProfile("h68","",50,0,150);
  TProfile * h92 = new TProfile("h92","",50,0,150);
  TProfile * h128 = new TProfile("h128","",50,0,150);
  TProfile * h160 = new TProfile("h160","",50,0,150);
  TProfile * h184 = new TProfile("h184","",50,0,150);
  
  TCut sel = "";
  TString tag = "MinBias";
  //TCut sel = "mul<400";
  //TString tag = "Periph.";
  //TCut sel = "mul>=1100";
  //TString tag = "Central";
  
  t->Draw("L1_SingleJet36_BptxAND:jtpt[0]>>h36",sel,"profgoff");
  t->Draw("L1_SingleJet52_BptxAND:jtpt[0]>>h52",sel,"profgoff");
  t->Draw("L1_SingleJet68_BptxAND:jtpt[0]>>h68",sel,"profgoff");
  //t->Draw("L1_SingleJet92:jtpt[0]>>h92",sel,"profgoff");
  //t->Draw("L1_SingleJet128:jtpt[0]>>h128",sel,"profgoff");
  //t->Draw("l1pt1>=160:jtpt[0]>>h160",sel,"profgoff");
  //t->Draw("l1pt1>=184:jtpt[0]>>h184",sel,"profgoff");
  
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  TH1D * hFrame = (TH1D*)h36->Clone("hFrame");
  hFrame->SetAxisRange(0,1.6,"Y");
  hFrame->GetXaxis()->SetNdivisions(505);
  hFrame->GetXaxis()->CenterTitle();
  hFrame->GetYaxis()->CenterTitle();
  hFrame->SetTitle(";HLT Jet p_{T} (GeV/c);L1 #epsilon");

  h52->SetMarkerStyle(kOpenCircle);
  h52->SetMarkerColor(kBlue);
  h68->SetMarkerStyle(kOpenSquare);
  h68->SetMarkerColor(kGreen+2);
  h92->SetMarkerStyle(kOpenTriangleUp);
  h92->SetMarkerColor(kOrange+2);
  h128->SetMarkerStyle(kOpenDiamond);
  h160->SetMarkerStyle(kOpenCross);
  h160->SetMarkerColor(kBlue);
  h184->SetMarkerColor(kMagenta);
  h184->SetMarkerStyle(kOpenStar);

  hFrame->Draw();
  h36->Draw("sameE");
  h52->Draw("sameE");
  h68->Draw("sameE");
  //h92->Draw("samep");
  //h128->Draw("sameE");
  //h160->Draw("sameE");
  //h184->Draw("sameE");
  
  TLine * l = new TLine(0,1,150,1);
  l->SetLineStyle(2);
  l->Draw();
	
  TLegend *leg = new TLegend(0.4,0.71,0.88,0.93);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(h36,"HICorePhysics, MB "+tag,"");
  leg->AddEntry(h36,"All L1 jet #eta","");
  leg->AddEntry(h36,"L1_SingleJet36","p");
  leg->AddEntry(h52,"L1_SingleJet52","p");
  leg->AddEntry(h68,"L1_SingleJet68","p");
  //leg->AddEntry(h92,"L1_SingleJet92","p");
  //leg->AddEntry(h128,"L1_SingleJet128","p");
  //leg->AddEntry(h160,"L1_SingleJet160","p");
  //leg->AddEntry(h184,"L1_SingleJet184","p");
  leg->Draw();
  
  c2->Print(Form("out/L1JetEffvJEt_%s.gif",tag.Data()));
}