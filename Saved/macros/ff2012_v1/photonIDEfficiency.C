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
#include "CutAndBinCollection2011.h"
#define PI 3.141592653589

int runByRunMode = 3;
int ptMode   = 1;
void getEff(TString var="genMatchedPt", TH1D* heff=0,TGraphAsymmErrors* geff=0, TCut moreGeneralCut="", TCut addCut="",bool fromTGJ=false);

TGraphAsymmErrors* drawEffBay(TH1* num=0, TH1* den=0) {
   TGraphAsymmErrors* gEff = new  TGraphAsymmErrors();
   gEff->SetMarkerStyle(20);
   gEff->BayesDivide(num,den);
   return gEff;
}

void photonIDEfficiency(){ 
   
   const int nPtBin = 5;
   double ptBin[nPtBin+1] = {60,70,90,110,140,200};
   TH1D* heff[7][5];
   TGraphAsymmErrors* geff[7][5];   
   for (int icent = 1; icent <=nCent_std; icent++) {
      for ( int iid=1 ; iid<=5; iid++) {
	 heff[icent][iid] = new TH1D(Form("heff_icent%d_id%d",icent,iid),";Leading Photon E_{T} (GeV);",nPtBin, ptBin);
	 geff[icent][iid] = new TGraphAsymmErrors();
	 geff[icent][iid]->SetName(Form("geff_%s",heff[icent][iid]->GetName()));
      }
   }
   
   int nId=0;
   TCanvas* c1  = new TCanvas("c2","",1200,400);
   c1->Divide(3,1);
   c1->cd(1);
   c1->Draw();
   for (int icent = 1; icent <=nCent_std; icent++) {
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      TCut centCut      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
      
      nId=1;
      getEff("genMatchedPt",heff[icent][nId],geff[icent][nId],centCut, "swissCrx<0.90 && seedTime<4");
      nId++;
      getEff("genMatchedPt",heff[icent][nId],geff[icent][nId],centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1");
      nId++;
      getEff("genMatchedPt",heff[icent][nId],geff[icent][nId],centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1" && iso3dCut);
      nId++;
      getEff("genMatchedPt",heff[icent][nId],geff[icent][nId],centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1 && sigmaIetaIeta<0.010" && iso3dCut);
      
      
   }

   for (int icent = 1; icent <=nCent_std; icent++) {
      for ( int iid=1 ; iid<=nId ; iid++) {
	 handsomeTH1(heff[icent][iid],ycolor[iid]);
	 handsomeTGraph(geff[icent][iid],ycolor[iid]);
      }
   }
   
   for (int icent = 1; icent <=nCent_std; icent++) {
      c1->cd(icent);
      int lowerCent = centBin_std[icent-1];
      int upperCent = centBin_std[icent]-1;
      heff[1][1]->SetAxisRange(0.,1.3,"Y");
      heff[1][1]->Draw();
      for ( int iid=1 ; iid<=nId ; iid++) {
	 heff[icent][iid]->Draw("p same");
         geff[icent][iid]->Draw("p");
	 
      }
      drawText(Form("%.0f %% - %.0f %%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5980963,0.8569118);
      if ( icent ==1 )
	 {
	    TLegend* leg1 =  new TLegend(0.25,0.20,0.95,0.55,NULL,"brNDC");
	    easyLeg(leg1,"Photon ID efficiency");
	    leg1->AddEntry(heff[icent][1],"spike rejection","lp");
	    leg1->AddEntry(heff[icent][2],"+ H/E < 0.1","lp");
	    leg1->AddEntry(heff[icent][3],"+ Fisher Disc","lp");
	    leg1->AddEntry(heff[icent][4],"+ #sigma_{#eta#eta} <0.010","lp");
	    leg1->Draw();
	 }
   }
   
   c1->SaveAs("photonID_efficiency.gif");
   c1->SaveAs("photonID_efficiency.eps");

   
}


void getEff(TString var, TH1D* heff, TGraphAsymmErrors* geff, TCut moreGeneralCut, TCut addCut, bool fromTGJ){
   
  //   TString fname1 = "barrelHiForestPhoton_MCphoton_all.root";
  TString fname1 = "skimmed/yskim_qcdAllPhotonAllPt_allCent_nMix10_v4.root";
   TFile *f1=new TFile(fname1.Data());
   TTree *photon1;
   if ( fromTGJ==true)
     photon1 = (TTree*)f1->Get("tgj");
   else 
     photon1 = (TTree*)f1->Get("yongsunPhotonTree");
   
   photon1->AddFriend("yEvt=yongsunHiEvt"       ,fname1.Data());
   photon1->AddFriend("ySkim=yongsunSkimTree"   ,fname1.Data());
   photon1->AddFriend("yJet=yongsunJetakPu3PF",fname1.Data());
   photon1->AddFriend("yHlt=yongsunHltTree"     ,fname1.Data());
    TCut generalCut  = etaCut && genMatchCut1 ; 
    if ( fromTGJ==true)
     generalCut ="";
   
   TH1D* den = (TH1D*)heff->Clone("den321");  
   TH1D* num = (TH1D*)heff->Clone("num321");
   den->Reset();
   num->Reset();
   den->Sumw2();
   num->Sumw2();
   photon1->Draw(Form("%s>>den321",var.Data()),generalCut && moreGeneralCut   );
   photon1->Draw(Form("%s>>num321",var.Data()),generalCut && moreGeneralCut && addCut);
   
   cout << "cu1 = " << (generalCut && moreGeneralCut).GetTitle() << endl;
   cout << "cu2 = " << (generalCut && moreGeneralCut && addCut).GetTitle() << endl;

   
   
   geff->BayesDivide(num,den);
   num->Divide(den);
   handsomeTH1(num,2);
   for ( int i = 1 ; i <= heff->GetNbinsX() ; i++) {
      heff->SetBinContent( i  ,  num->GetBinContent(i)   );
      heff->SetBinError  ( i  ,  num->GetBinError(i)   );
   }
   
}
