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
#include "compareProf.h"
#include "../calc/PrintRateJet.C"
using namespace std;

void CompareEff(TCut mycut = "etree.hiBin>=20",
                TString infdataname="/d01/velicanu/tmp/reprocessed-openhlt-182382.root",
                TString infrefname="/d01/velicanu/tmp/reprocessed-openhlt-181985.root",
                TString tag="182382v181985",
                TString newtitle="Run 182382",
                TString oldtitle="Run 181985"
                )
{
   TH1::SetDefaultSumw2();
   
   // inputs
   TFile * infdata = TFile::Open(infdataname);
   TFile * infref = TFile::Open(infrefname);
   
   TTree * tdata = (TTree*)infdata->Get("hltanalysis/HltTree");
   tdata->AddFriend("skim=skimanalysis/HltTree");
   tdata->AddFriend("etree=hiEvtAnalyzer/HiTree");
   tdata->AddFriend("icPu5=icPu5JetAnalyzer/t");
   TTree * tref = (TTree*)infref->Get("hltanalysis/HltTree");
   tref->AddFriend("skim=skimanalysis/HltTree");
   tref->AddFriend("etree=hiEvtAnalyzer/HiTree");
   tref->AddFriend("icPu5=icPu5JetAnalyzer/t");
   
   TCut sel="HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection"&&mycut;
   TCut selref="HLT_HIMinBiasHfOrBSC_v1&&skim.pcollisionEventSelection"&&mycut;
   float nSel = tdata->GetEntries(sel);
   float nSelRef = tref->GetEntries(selref);
   cout << "Data: " << TString(sel) << " " << nSel << endl;
   cout << "Ref:  " << TString(selref) << " " << nSelRef << endl;

   TCanvas * c0 = new TCanvas("c0","c0",500,500);
   tdata->Draw("etree.hiBin",sel,"hist");
   tref->Draw("etree.hiBin",selref,"same hist");
   
   // histograms
   TCanvas * c2 = new TCanvas(tag+"c2","c2",500,500);
   Compare comp2("hL1Eff",tdata,tref,sel,selref);
   comp2.Init(40,10,90);
   tdata->Draw("L1_SingleJet36_BptxAND:icPu5.jtpt[0]",sel,"prof");
   comp2.Project("L1_SingleJet36_BptxAND:Max$(icPu5.jtpt*(abs(icPu5.jteta)<3&&icPu5.jtpt>25))>>","prof");
   comp2.Draw(";Reco Jet p_{T} (GeV)/c;#epsilon",0,1.2,newtitle,oldtitle);
   c2->Print("out/trigVal/trigeff"+tag+"_peri.gif");
   
   //PrintRateJet(infdataname);
}
