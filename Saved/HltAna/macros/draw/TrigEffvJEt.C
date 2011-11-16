#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TString.h"
#include "TLegend.h"
#include "TLine.h"
#include "efficiency.h"
using namespace std;

void TrigEffvJEt(
               //TString infname="/d100/velicanu/tmp/hiexp-hirun2011-r181611-reco-v1-collisionEvents_lowerSC_autohlt.root"
                 //TString infname="/d100/velicanu/tmp/hiexp-hirun2011-r181685-reco-v1-collisionEvents_lowerSC_autohlt.root"
                 //TString infname="../../../trees/hiexp-hirun2011-b9b9-181611.root"
                 //TString infname="../../../trees/hiexp-hirun2011-b170b170_181683to181695.root"
                 TString infname="../../../trees/hiexp-hirun2011-b358b358.root"
               )
{
   TH1::SetDefaultSumw2();
   TFile * inf = new TFile(infname);
   cout << inf->GetName() << endl;
   
   TTree * t = (TTree*)inf->Get("hltanalysis/HltTree");
   t->AddFriend("skim=skimanalysis/HltTree");
   t->AddFriend("icPu5JetAnalyzer/t");

   TCut sel = "HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection";
   TString jetvar = "Max$(jtpt*(abs(jteta)>3))";
   const int nptbins = 13;
   float ptbins[nptbins+1] = {30,35,40,45,50,55,60,65,70,80,90,100,120,150};
   //TString tag = "Bunch358x358_Hbhe";
   TString tag = "Bunch358x358_Hf";

   // L1
   Efficiency l1eff1(t,"l1eff1",jetvar,sel,"L1_SingleJet36_BptxAND",50,0,150);
   Efficiency l1eff2(t,"l1eff2",jetvar,sel,"L1_SingleJet52_BptxAND",50,0,150);
   Efficiency l1eff3(t,"l1eff3",jetvar,sel,"L1_SingleJet68_BptxAND",50,0,150);
   Efficiency l1eff4(t,"l1eff4",jetvar,sel,"L1_SingleJet92_BptxAND",50,0,150);
   Efficiency l1eff5(t,"l1eff5",jetvar,sel,"L1_SingleJet128_BptxAND",50,0,150);
   
   TCanvas * c2 = new TCanvas("c2","c2",500,500);
   TH1D * hFrame = (TH1D*)l1eff1.hEff->Clone("hFrame");
   hFrame->SetAxisRange(0,1.6,"Y");
   hFrame->SetTitle(";Leading RecoJet p_{T} (GeV/c);Trigger #epsilon");
   hFrame->Draw();

   l1eff1.Draw();
   l1eff2.Draw(kBlue,kOpenCircle);
   l1eff3.Draw(kGreen+2,kOpenSquare);
   l1eff4.Draw(kOrange+2,kOpenTriangleUp);

   TLine * l = new TLine(0,1,150,1);
   l->SetLineStyle(2);
   l->Draw();
	
   TLegend *leg = new TLegend(0.4,0.71,0.88,0.93);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.035);
   leg->AddEntry(l1eff1.hEff,"Bunch 358x358","");
   leg->AddEntry(l1eff1.hEff,"ICPu5, |#eta|>3","");
   leg->AddEntry(l1eff1.hEff,"L1_SingleJet36","p");
   leg->AddEntry(l1eff2.hEff,"L1_SingleJet52","p");
   leg->AddEntry(l1eff3.hEff,"L1_SingleJet68","p");
   leg->AddEntry(l1eff4.hEff,"L1_SingleJet92","p");
   leg->Draw();   
   c2->Print(Form("out/L1JetEffvJEt_%s.gif",tag.Data()));
   
   // HLT
   Efficiency hlteff1(t,"hlteff1",jetvar,sel,"HLT_HIJet55_v1",nptbins,ptbins);
   Efficiency hlteff2(t,"hlteff2",jetvar,"HLT_HIJet55_v1&&skim.pcollisionEventSelection","HLT_HIJet65_v1",nptbins,ptbins);
   Efficiency hlteff3(t,"hlteff3",jetvar,"HLT_HIJet65_v1&&skim.pcollisionEventSelection","HLT_HIJet80_v1",nptbins,ptbins);
   //Efficiency hlteff4(t,"hlteff4",jetvar,sel,"HLT_HIJet95_v1",50,0,200);
   //Efficiency hlteff5(t,"hlteff5",jetvar,sel&&"jtpt[1]>55","HLT_HIDiJet55_v1"&&jetcut,50,0,200);
   TCanvas * c3 = new TCanvas("c3","c3",500,500);
   TH1D * hFrame3 = (TH1D*)hlteff1.hEff->Clone("hFrame3");
   hFrame3->SetAxisRange(0,150,"X");
   hFrame3->SetAxisRange(0,1.6,"Y");
   hFrame3->SetTitle(";Leading RecoJet p_{T} (GeV/c);Trigger #epsilon");
   hFrame3->Draw();
   
   hlteff1.Draw();
   hlteff2.Draw(kBlue,kOpenCircle);
   hlteff3.Draw(kGreen+2,kOpenSquare);
   //hlteff4.Draw(kOrange+2,kOpenTriangleUp);
   
   TLine * l2 = new TLine(30,1,150,1);
   l2->SetLineStyle(2);
   l2->Draw();
	
   TLegend *leg3 = new TLegend(0.4,0.71,0.88,0.93);
   leg3->SetFillStyle(0);
   leg3->SetBorderSize(0);
   leg3->SetTextSize(0.035);
   leg3->AddEntry(hlteff1.hEff,"Bunch 358x358","");
   leg3->AddEntry(hlteff1.hEff,"ICPu5, |#eta|>3","");
   leg3->AddEntry(hlteff1.hEff,"HLT_HIJet55_v1","p");
   leg3->AddEntry(hlteff2.hEff,"HLT_HIJet65_v1","p");
   leg3->AddEntry(hlteff3.hEff,"HLT_HIJet80_v1","p");
   //leg3->AddEntry(hlteff5.hEff,"HLT_HIDiJet55_v1","p");
   leg3->Draw();
   c3->Print(Form("out/HLTJetEffvJEt_%s.gif",tag.Data()));
}