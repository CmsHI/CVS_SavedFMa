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

void HLTEffvJEt(
               TString infname="../../../trees/merged_Run181531_HIExpressPhysics_Part.root"
               )
{
   TH1::SetDefaultSumw2();
   TFile * inf = new TFile(infname);
   
   TTree * t = (TTree*)inf->Get("hltanalysis/HltTree");
   t->AddFriend("skim=skimanalysis/HltTree");
   t->AddFriend("icPu5JetAnalyzer/t");
   //t->AddFriend("akPu3PFJetAnalyzer/t");
   
   float ptmax=200;
   TProfile * h55 = new TProfile("h55","",50,0,ptmax);
   TProfile * h65 = new TProfile("h65","",50,0,ptmax);
   TProfile * h80 = new TProfile("h80","",50,0,ptmax);
   TProfile * h95 = new TProfile("h95","",50,0,ptmax);
   TProfile * h55_55 = new TProfile("h55_55","",50,0,ptmax);
   TProfile * h65_55 = new TProfile("h65_55","",50,0,ptmax);
   
   TCut sel = "HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection";
   TString tag = "Run181531";
   
   TCanvas * cchk = new TCanvas("cchk","cchk",500,500);
   t->Draw("L1EtTot",sel,"");
   
   t->Draw("HLT_HIJet55_v1:jtpt[0]>>h55",sel,"profgoff");
   t->Draw("HLT_HIJet65_v1:jtpt[0]>>h65",sel,"profgoff");
   t->Draw("HLT_HIJet80_v1:jtpt[0]>>h80",sel,"profgoff");
   t->Draw("HLT_HIJet95_v1:jtpt[0]>>h95",sel,"profgoff");
   t->Draw("HLT_HIDiJet55_v1:jtpt[0]>>h55_55",sel,"profgoff");
   t->Draw("HLT_HIJet65_Jet55_v1:jtpt[0]>>h65_55",sel,"profgoff");
   
   TCanvas * c2 = new TCanvas("c2","c2",500,500);
   TH1D * hFrame = (TH1D*)h55->Clone("hFrame");
   hFrame->SetAxisRange(0,150,"X");
   hFrame->SetAxisRange(0,1.6,"Y");
   hFrame->GetXaxis()->SetNdivisions(505);
   hFrame->GetXaxis()->CenterTitle();
   hFrame->GetYaxis()->CenterTitle();
   hFrame->SetTitle(";Leading Reco Jet p_{T} (GeV/c);HLT #epsilon");
   
   h65->SetMarkerStyle(kOpenCircle);
   h65->SetMarkerColor(kBlue);
   h80->SetMarkerStyle(kOpenSquare);
   h80->SetMarkerColor(kGreen+2);
   h95->SetMarkerStyle(kOpenTriangleUp);
   h95->SetMarkerColor(kOrange+2);
   h55_55->SetMarkerStyle(kOpenDiamond);
   
   hFrame->Draw();
   h55->Draw("sameE");
   h65->Draw("sameE");
   h80->Draw("sameE");
   h95->Draw("samep");
   h55_55->Draw("sameE");
   
   TLine * l = new TLine(0,1,150,1);
   l->SetLineStyle(2);
   l->Draw();
	
   TLegend *leg = new TLegend(0.4,0.71,0.88,0.93);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.035);
   leg->AddEntry(h55,"MinBias "+tag,"");
   leg->AddEntry(h55,"ICPu5","");
   leg->AddEntry(h55,"HLT_HIJet55_v1","p");
   leg->AddEntry(h65,"HLT_HIJet65_v1","p");
   leg->AddEntry(h80,"HLT_HIJet80_v1","p");
   leg->AddEntry(h95,"HLT_HIJet95_v1","p");
   leg->AddEntry(h55_55,"HLT_HIDiJet55_v1","p");
   leg->Draw();
   
   c2->Print(Form("out/HLTJetEffvJEt_%s.gif",tag.Data()));
}