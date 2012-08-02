#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include "commonUtility.h"
#include <TRandom3.h>
#define PI 3.141592653589

int runByRunMode = 3;
int ptMode   = 1;
void getEff(TH1D* heff=0,TGraphAsymmErrors* geff=0, TCut trigCut="", int theMode=1, TCut addCut="");
TGraphAsymmErrors* drawEffBay(TH1* num=0, TH1* den=0) {
   TGraphAsymmErrors* gEff = new  TGraphAsymmErrors();
   gEff->SetMarkerStyle(20);
   gEff->BayesDivide(num,den);
   return gEff;
}

void triggerEfficiency() {
   
   TCut trigCut;
   trigCut = "yhlt.HLT_HISinglePhoton40_v2";
   //   trigCut = "yhlt.HLT_HISinglePhoton30_v2";
   //trigCut = "yhlt.L1_SingleEG5_BptxAND";
   //   trigCut = "yhlt.HLT_HISinglePhoton30_v2  ";
   //  trigCut = "yhlt.L1_SingleEG5_BptxAND";// || yhlt.HLT_HIJet80_v1";
   // || yhlt.HLT_HIJet80_v1";
   
   TCut goodL1Cut = "yhlt.Run > 181675 && !(yhlt.Run==181912 || yhlt.Run==181910 || yhlt.Run==182089 || yhlt.Run==182098 || yhlt.Run==182099 || yhlt.Run==182124)";
   TCut goodHLTCut = "!(yhlt.Run==182123 || yhlt.Run==182133 || yhlt.Run==181946)  && yhlt.Run<=182134  ";
   TCut goodRunCut = goodL1Cut && goodHLTCut;
   
   const int nPtBin = 10;
   double ptBin[nPtBin+1] = {0,10,15,20,25,30,33,41,50,70,120};
   TH1D* heff         = new TH1D("heff",";Leading Photon E_{T} (GeV);",nPtBin, ptBin);
   TGraphAsymmErrors* geff = new TGraphAsymmErrors();

   TCanvas* c1  = new TCanvas("c2","",610,522);
   c1->Divide(2,1);
   c1->cd(1);
  
   getEff(heff,geff,trigCut,ptMode, goodRunCut);
   handsomeTH1(heff,1);
   handsomeTGraph(geff,1);
   geff->SetMarkerStyle(20);
   heff->SetMarkerSize(0.1);
   for ( int i=0;i<= heff->GetNbinsX() ; i++)
      heff->SetBinError(i,0);
   heff->Draw(0);
   //   heff->DrawCopy("pl");
   geff->Draw("p");
   drawText("HLT_HISinglePhoton40",0.4,0.3,1,18);
   
   c1->cd(2);
   TH1D* heffr         = new TH1D("heffr",";Run number;",182400.5 - 181500.5,181500.5,182400.5);
   TGraphAsymmErrors* geffr = new TGraphAsymmErrors();
   getEff(heffr,geffr,trigCut, runByRunMode, "pt>50" && goodRunCut);
   handsomeTH1(heffr,1);
   heffr->SetNdivisions(505);
   handsomeTGraph(geffr,1);
   geffr->SetMarkerStyle(20);
   heffr->Reset();
   heffr->DrawCopy();
   geffr->Draw("p");
   
   TCanvas* c3  = new TCanvas("c3","",700,500);
   heff->SetYTitle("Efficiency");
   heff->SetFillColor(kGray);
   heff->SetFillStyle(3001);
   heff->SetAxisRange(0,1.2,"Y");
   heff->DrawCopy("hist ");
   geff->Draw("p same");
   drawCMS3(0.5,0.5,84,17);
   drawText("HLT_HISinglePhoton40",0.5,0.3,1,18);
   
      c3->SaveAs("plot_triggerEfficiencyPhoton30.eps");
     c3->SaveAs("plot_triggerEfficiencyPhoton30.gif");

}


void getEff(TH1D* heff, TGraphAsymmErrors* geff, TCut trigCut, int theMode, TCut addCut){
   char* label = "Barrel";
   
   char* histTitle = ";leading photon eT (GeV);Efficiency";
   TString fname1 = "forest/barrelHiForestPhoton10_NoSpikeCut_MinBias.root";//barrelHiForestPhoton35_Skim2011-v2.root";
   //   TCut minbiasCut  ="yhlt.HLT_HIMinBiasHfOrBSC_v1==1";
   TCut collisionCut = "yskim.pcollisionEventSelection==1";
   TCut leadingCut   = "order==0";
   TCut goodSCCut = "abs(seedTime)<4 && swissCrx<0.95 && abs(eta)<1.44";// && energy/rawEnergy<2 && eMax/(eMax+eTop+eBottom+eRight+eLeft)>0.2";
   //   TCut runNumberCut = " yhlt.Run > 182122 && yhlt.Run < 182300";
   TCut centCut      = "yevt.hiBin<1000";  //"yhlt.L1_SingleEG5_BptxAND";//"yhlt.HLT_HISinglePhoton30_v2==1";                                                                                   
   //   TCut r9Cut        = "r9>0.5";

   //TH1D* den = (TH1D*)heff->Clone("den");
   // den->SetName("den11");
   //   const int nPtBin = 6;
   //  double ptBin[nPtBin+1] = {0,5,10,15,20,25,30};

   //   heff->Sumw2(); // num->Sumw2();   den->Sumw2();

      TFile *f1=new TFile(fname1.Data());
   TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
   photon1->AddFriend("yevt=yongsunHiEvt"       ,fname1.Data());
   photon1->AddFriend("yskim=yongsunSkimTree"   ,fname1.Data());
   photon1->AddFriend("yhlt=yongsunHltTree"     ,fname1.Data());
   
   
   TCut generalCut  = collisionCut && leadingCut && goodSCCut && centCut && addCut;
   
   
   // TH1D* den         = new TH1D("den1",";Leading Photon E_{T} (GeV);",3,0,100);
   TH1D* den = (TH1D*)heff->Clone("den321");  
   TH1D* num = (TH1D*)heff->Clone("num321");
   
   if ( theMode == ptMode) { 
      photon1->Draw("lpt>>den321",generalCut);
      photon1->Draw("lpt>>num321",generalCut&&trigCut);                                                                                                                                                     
   }
   if ( theMode == runByRunMode) {
      photon1->Draw("Run>>den321",generalCut);
      photon1->Draw("Run>>num321",generalCut&&trigCut);
   }
   geff->BayesDivide(num,den);
   
   handsomeTH1(num,2);
   den->DrawCopy();
   num->DrawCopy("same");
   num->Divide(den);
   //   den->DrawCopy();
   // photon1->Draw("pt>>num",generalCut&&trigCut);
   //  num->Divide(den);
   
   
   for ( int i = 1 ; i <= heff->GetNbinsX() ; i++) {
      heff->SetBinContent( i  ,  num->GetBinContent(i)   );
      //heff->SetBinError  ( i  ,  num->GetBinError  (i)   );
   }
   
   //   heff->Divide(den);
  
}
