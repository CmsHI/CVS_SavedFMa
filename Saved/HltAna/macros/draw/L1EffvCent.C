#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TLine.h"
using namespace std;

void L1EffvCent(TString infname="../ntout/L1Tree_mb_mctag_l1hlt.root")
{
  TFile * inf = new TFile(infname);
  
  TTree * t = (TTree*)inf->Get("hltbitnew/HltTree");
  t->AddFriend("l1NtupleProducer/L1Tree");
  t->AddFriend("icPu5JetAnalyzer/t");
  t->AddFriend("hltorg=hltbitorg/HltTree");
  t->SetAlias("mul","RankETT*0.5");
  
  TProfile * h36 = new TProfile("h36","",100,0,2000);
  TProfile * h52 = new TProfile("h52","",100,0,2000);
  TProfile * h68 = new TProfile("h68","",100,0,2000);
  TProfile * h92 = new TProfile("h92","",100,0,2000);
  TProfile * h128 = new TProfile("h128","",100,0,2000);
  
  t->Draw("L1_SingleJet36:mul>>h36","","profgoff");
  t->Draw("L1_SingleJet52:mul>>h52","","profgoff");
  t->Draw("L1_SingleJet68:mul>>h68","","profgoff");
  t->Draw("L1_SingleJet92:mul>>h92","","profgoff");
  t->Draw("L1_SingleJet128:mul>>h128","","profgoff");
  
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  h36->SetAxisRange(0,1.6,"Y");
  h36->GetXaxis()->SetNdivisions(505);
  h36->GetXaxis()->CenterTitle();
  h36->GetYaxis()->CenterTitle();
  h36->SetTitle(";L1 E_{T} Total (GeV/c);Fraction of MinBias");
  h52->SetMarkerStyle(kOpenCircle);
  h52->SetMarkerColor(kBlue);
  h68->SetMarkerStyle(kOpenSquare);
  h68->SetMarkerColor(kGreen+2);
  h92->SetMarkerStyle(kOpenDiamond);
  h92->SetMarkerColor(kOrange+2);
  h128->SetMarkerStyle(kOpenCross);
  h128->SetMarkerColor(kRed);
  h36->Draw("E");
  h52->Draw("sameE");
  h68->Draw("sameE");
  h92->Draw("sameE");
  h128->Draw("sameE*");
  
  TLine * l = new TLine(0,1,150,1);
  l->SetLineStyle(2);
  l->Draw();
	
  TLegend *leg = new TLegend(0.4,0.71,0.88,0.93);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(h36,"HICorePhysics, MinBias","");
  leg->AddEntry(h36,"All L1 jet #eta","");
  leg->AddEntry(h36,"L1_SingleJet36","p");
  leg->AddEntry(h52,"L1_SingleJet52","p");
  leg->AddEntry(h68,"L1_SingleJet68","p");
  leg->AddEntry(h92,"L1_SingleJet92","p");
  leg->AddEntry(h128,"L1_SingleJet128","p");
  leg->Draw();
  
  c2->Print("out/L1JetEffvCent.gif");
}