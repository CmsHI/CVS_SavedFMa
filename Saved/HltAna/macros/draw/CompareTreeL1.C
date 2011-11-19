#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
#include "compare.h"
#include "../calc/PrintRateJet.C"
using namespace std;

void CompareTreeL1(
                   TString infdataname="/d100/velicanu/tmp/hiexp-hirun2011-r181683-reco-v1-collisionEvents_lowerSC_autohlt.root", //good run 181/611
                   //TString infdataname="/d100/velicanu/tmp/hiexp-hirun2011-r181693-reco-v1-collisionEvents_lowerSC_autohlt.root",
                   TString infrefname="../trees/HIHLT_Validation_Test_GRIF_v10.root",
                   TString tag="",
                   TCut sel="HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection",
                   TCut selref="HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection",
                   TString newtitle="New Data",
                   TString oldtitle="Ref Data"
                   )
{
   TH1::SetDefaultSumw2();
   
   // inputs
   TFile * infdata = new TFile(infdataname);
   TFile * infref = new TFile(infrefname);
   
   TTree * tdata = (TTree*)infdata->Get("hltanalysis/HltTree");
   tdata->AddFriend("skim=skimanalysis/HltTree");
   TTree * tref = (TTree*)infref->Get("hltanalysis/HltTree");
   tref->AddFriend("skim=skimanalysis/HltTree");
   
   // histogram bins
   const int netabins = 22;
   float etabins[netabins+1] = 
   {-5,-4.5,-4,-3.5,-3,-2.172,-1.74,-1.392,-1.044,-0.696,-0.348,0,
      0.348,0.696,1.044,1.392,1.74,2.172,3,3.5,4,4.5,5};
   const int nptbins=70;
   float ptbins[nptbins+1];
   for (int i=0; i<=nptbins; ++i) { ptbins[i]=i*4; }
   
   // histograms
   TCanvas * c2 = new TCanvas(tag+"c2","c2",500,500);
   c2->SetLogy();
   Compare comp2("hEthbhe",tdata,tref,sel,selref,"");
   comp2.Init(35,0,280);
   comp2.Project("L1CenJetEt>>");
   comp2.Project("L1TauEt>>+");
   comp2.Draw(";L1 Jet p_{T} (GeV)/c;a.u.",1e-4,1e2,newtitle,oldtitle);
   c2->Print("out/trigVal/"+comp2.tag+tag+".gif");
   
   TCanvas * c3 = new TCanvas(tag+"c3","c3",500,500);
   c3->SetLogy();
   Compare comp3("hEthf",tdata,tref,sel,selref,"");
   comp3.Init(35,0,280);
   comp3.Project("L1ForJetEt>>");
   comp3.Draw(";L1 Jet p_{T} (GeV)/c;a.u.",1e-4,1e2,newtitle,oldtitle);
   c3->Print("out/trigVal/"+comp3.tag+tag+".gif");
   
   TCanvas * c4 = new TCanvas(tag+"c4","c4",500,500);
   c4->SetLogy();
   Compare comp4("hEtahbhe",tdata,tref,sel,selref,"");
   comp4.Init(netabins,etabins);
   comp4.Project("L1CenJetEta>>");
   comp4.Project("L1TauEta>>+");
   comp4.Draw(";L1 Jet #eta;a.u.",1e-4,1e2,newtitle,oldtitle);
   c4->Print("out/trigVal/"+comp4.tag+tag+".gif");
   
   TCanvas * c5 = new TCanvas(tag+"c5","c5",500,500);
   c5->SetLogy();
   Compare comp5("hEtahf",tdata,tref,sel,selref,"");
   comp5.Init(netabins,etabins);
   comp5.Project("L1ForJetEta>>");
   comp5.Draw(";L1 Jet #eta;a.u.",1e-4,1e2,newtitle,oldtitle);
   c5->Print("out/trigVal/"+comp5.tag+tag+".gif");
   
   PrintRateJet(infdataname);
}
