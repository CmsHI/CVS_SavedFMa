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


TString fname1="barrelHiForestPhoton_DiPhotonSkim_v1.root";
TString fname1MC="barrelHiForestPhoton_MC-Zee.root";
TString fPho = "../forest/barrelHiForestPhoton_MCphoton50_37k.root";


void zeeAnaDiff(TH1D* hsee=0, TCut addCut="",TH1D* hseeMc=0);
void getseePhoMC(TH1D* hsee=0, TCut addCut="");
float getMean(TH1* h=0 , int n1=0 , int n2=0);

void zeeAna() {
  
  const int tempNbin = 2;
  int tempBin[tempNbin+1] = {0,12,40};
  
  TH1D* hsee[5];
  TH1D* hseeMC[5];
  TH1D* hseePho[5];
  
  TCanvas* c1 = new TCanvas("c1","",1000,500);
  c1->Divide(2,1);
  c1->Draw();
  for ( int icent = 1 ; icent<= tempNbin ; icent++) {
    c1->cd(icent);
    int lowCent = tempBin[icent-1];
    int highCent = tempBin[icent]-1;
    TCut centCut      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
    TCut firstCut = "pt1>30 && hoe1<0.1 && (cc4+cr4)<5";
    TCut secondCut = "pt2>30 && hoe2<0.2 && see2<0.011 && (cc4+cr4)<5";
    TCut generalCut  = centCut && firstCut && secondCut;
    hsee[icent]   = new TH1D(Form("hsee_icent%d",icent),";#sigma_{#eta#eta};event fraction",25,0,0.025);
    hseeMC[icent] = new TH1D(Form("hseeMC_icent%d",icent),";#sigma_{#eta#eta};event fraction",25,0,0.025);
    hsee[icent]->Sumw2();
    zeeAnaDiff(hsee[icent], generalCut, hseeMC[icent]);
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.6880963,0.8369118);
  }
  
  c1->SaveAs("invMassOfZ.pdf");

  TCanvas* c1a = new TCanvas("c1a","",1000,500);
  for ( int icent = 1 ; icent<= tempNbin ; icent++) {
    int lowCent = tempBin[icent-1];
    int highCent = tempBin[icent]-1;
    TCut centCut      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
    TCut generalCut  = "hadronicOverEm<0.1 && corrPt>40 && abs(eta)<1.44 && isGenMatched";
    hseePho[icent] =  new TH1D(Form("hseePho_icent%d",icent),";#sigma_{#eta#eta};event fraction",25,0,0.025);
    hseePho[icent]->Sumw2();
    getseePhoMC(hseePho[icent],generalCut && centCut);
  }
 
  
  TCanvas* c2 = new TCanvas("c2","",1000,500);
  c2->Divide(2,1);
  c2->Draw();
  for ( int icent = 1 ;icent<=tempNbin ; icent++) {
    int lowCent = tempBin[icent-1];
    int highCent = tempBin[icent]-1;
    c2->cd(icent);
    handsomeTH1(hsee[icent],2);
    handsomeTH1(hseeMC[icent],1);
    handsomeTH1(hseePho[icent],4);
    scaleInt(hsee[icent]);
    scaleInt(hseeMC[icent]);
    scaleInt(hseePho[icent]);
    hsee[icent]->SetAxisRange(0,0.8,"Y");
    hsee[icent]->Draw();
    hseeMC[icent]->Draw("same");
    hseePho[icent]->Draw("same hist");
    TLegend* leg1 =  new TLegend(0.5580645,0.5381356,0.8923387,0.7648305,NULL,"brNDC");
    easyLeg(leg1,"#sigma_{#eta#eta}");
    leg1->AddEntry(hsee[icent],"DATA electron","pl");
    leg1->AddEntry(hseeMC[icent],"MC electron","pl");
    leg1->AddEntry(hseePho[icent],"MC photon","l");
    leg1->Draw();
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.5880963,0.8369118);
    cout << "shift = " << getMean(hsee[icent],5,12) - getMean(hseeMC[icent],5,12) << endl;

  }

  c2->SaveAs("showerShapeComparison.pdf");
  
}


float getMean(TH1* h , int n1, int n2) {
  TH1D* h1 = (TH1D*)h->Clone("htemptemp2");
  h1->Reset();
  for ( int i=n1 ; i<=n2 ; i++) {
    h1->SetBinContent(i, h->GetBinContent(i));
  }
  return h1->GetMean();
}

void getseePhoMC(TH1D* hsee, TCut addCut){

  TFile *f1=new TFile(fPho.Data());
  TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
  photon1->AddFriend("yEvt=yongsunHiEvt"       ,fPho.Data());
  photon1->AddFriend("ySkim=yongsunSkimTree"   ,fPho.Data());
  photon1->AddFriend("yHlt=yongsunHltTree"     ,fPho.Data());
  photon1->AddFriend("tgj",                     fPho.Data());
 
  TH1D* htemp = (TH1D*)hsee->Clone("htemp");  htemp->Reset();
  photon1->Draw("sigmaIetaIeta>>htemp",Form("(%s) *ncoll",addCut.GetTitle()));
  hsee->Reset();
  hsee->Add(htemp);
}

void zeeAnaDiff(TH1D* hsee, TCut addCut,TH1D* hseeMc) {
  
  TFile *f1=new TFile(fname1.Data());
  TTree *zp = (TTree*)f1->Get("tz");
  zp->AddFriend("yEvt=yongsunHiEvt"       ,fname1.Data());
  zp->AddFriend("ySkim=yongsunSkimTree"   ,fname1.Data());
  zp->AddFriend("yHlt=yongsunHltTree"     ,fname1.Data());
  zp->AddFriend("tgj",                     fname1.Data());
  
  TH1D* hmass = new TH1D("hmass",";inv. mass (GeV);event fraction",34,30,200);
  hmass->Sumw2();

  zp->Draw("invm>>hmass",addCut && "isEle1==1 && isEle2==1");
  TH1D* htemp = (TH1D*)hsee->Clone("htemp");   htemp->Reset();
  zp->Draw("see1>>htemp",addCut && "isEle1==1 && isEle2==1" && "invm>80 && invm<100" );
  hsee->Reset();
  hsee->Add(htemp);
  handsomeTH1(hmass,2);
  scaleInt(hmass);
  
  TH1D* htempMC;
  TFile *f1mc;
  TTree *zpmc ;
  f1mc=new TFile(fname1MC.Data());
  zpmc = (TTree*)f1mc->Get("tz");
  zpmc->AddFriend("yEvt=yongsunHiEvt"       ,fname1MC.Data());
  zpmc->AddFriend("ySkim=yongsunSkimTree"   ,fname1MC.Data());
  zpmc->AddFriend("yHlt=yongsunHltTree"     ,fname1MC.Data());
  zpmc->AddFriend("yPho=yongsunPhotonTree"     ,fname1MC.Data());
  zpmc->AddFriend("tgj",                     fname1MC.Data());
  hseeMc->Sumw2();
  htempMC = (TH1D*)hseeMc->Clone("htempMc");   htempMC->Reset();
  TH1D* hmassMC = new TH1D("hmassMC11",";inv. mass (GeV);event fraction",34,30,200);
  hmassMC->Sumw2();
  zpmc->Draw("invm>>hmassMC11",Form("(%s) *yPho.ncoll",addCut.GetTitle()));
  zpmc->Draw("see1>>htempMc",Form("(%s) *yPho.ncoll", (addCut && "invm>80 && invm<100").GetTitle()) );
  hseeMc->Reset();
  hseeMc->Add(htempMC);
  handsomeTH1(hmassMC,1);
  scaleInt(hmassMC);
  
  
  hmassMC->SetAxisRange(0,0.6,"Y");
  hmassMC->DrawCopy();
  hmass->DrawCopy("same");                                                                                             
  TLegend* leg1 =  new TLegend(0.5580645,0.5381356,0.8923387,0.7648305,NULL,"brNDC");
  easyLeg(leg1,"Z->ee mass");
  leg1->AddEntry(hmass,"DATA","pl");
  leg1->AddEntry(hmassMC,"MC","pl");
  leg1->Draw();
  
}

/*
  TLegend* leg1 =  new TLegend(0.2406292,0.751055,0.8995147,0.9175199,NULL,"brNDC");
  int n1 = hGenPt1->Integral( hGenPt1->FindBin(60.001), hGenPt1->GetNbinsX() );
  int n2 = hGenPt2->Integral( hGenPt2->FindBin(60.001), hGenPt1->GetNbinsX() );
  easyLeg(leg1,"All Photons");
  leg1->AddEntry(hGenPt1,Form("All direct Photons : %d",n1),"l");
  leg1->AddEntry(hGenPt2,Form("Fragmentation photons : %d",n2),"lf");
  leg1->Draw();

  TH1D* hGenPt1iso = new TH1D ("hgenPt1iso",";E_{T} of photon (GeV);Entries",40,0,200);
  hGenPt1iso->Sumw2();
  TH1D* hGenPt2iso = (TH1D*)hGenPt1->Clone("hgenPt2iso");
  hGenPt2iso->Sumw2();

  TCut allcut = generalCut && (isFragment||isPrompt) && isoCut;
  TCut frgcut = generalCut && isFragment && isoCut ;
  photon1->Draw("genMatchedPt>>hgenPt1iso",Form("(%s) * tgj.reweight", allcut.GetTitle()));
  photon1->Draw("genMatchedPt>>hgenPt2iso",Form("(%s) * tgj.reweight", frgcut.GetTitle()));

  hGenPt2iso->SetFillColor(48);
  hGenPt2iso->SetFillStyle(3001);
  handsomeTH1(hGenPt1iso);
  float scaleFac = hGenPt1iso->Integral();
  hGenPt1iso->Scale(1./scaleFac);
  hGenPt2iso->Scale(1./scaleFac);
  hGenPt1iso->SetAxisRange(1.e-4,1.e2,"Y");
  hGenPt1iso->DrawCopy("hist");
  hGenPt2iso->DrawCopy("same hist");
  gPad->SetLogy();
  jumSun(60,1.e-4,60,1.e2);
  
  TLegend* leg2 =   new TLegend(0.2406292,0.751055,0.8995147,0.9175199,NULL,"brNDC");
  float n1i = hGenPt1iso->Integral( hGenPt1iso->FindBin(60.001), hGenPt1iso->GetNbinsX() );
  float n2i = hGenPt2iso->Integral( hGenPt2iso->FindBin(60.001), hGenPt2iso->GetNbinsX() );
  float ratio = (float)n2i/n1i;
  easyLeg(leg2,cutName.Data());
  leg2->AddEntry(hGenPt1iso,Form("All direct Photons"),"l");
  leg2->AddEntry(hGenPt2iso,Form("Fragmentation photons (%d %%)",(int)(ratio*100.)),"f");
  leg2->Draw();

  
  if ( drawScatter ) {
    TCanvas* c100 =new TCanvas("can_scatter","",1000,500);
    c100->Divide(2,1);
    c100->Draw();
    c100->cd(1);
    TH2D* h1 = new TH2D("sc1",";Generator level Iso (GeV); SumIso (GeV)",50,0,50,100,-50,50);
    TH2D* h2 = new TH2D("sc2",";Generator level Iso (GeV); SumIso (GeV)",50,0,50,100,-50,50);
    photon1->Draw("(cc4+cr4+ct4PtCut20)/0.9:genCalIsoDR04 >> sc1", generalCut && isPrompt);
    photon1->Draw("(cc4+cr4+ct4PtCut20)/0.9:genCalIsoDR04 >> sc2", generalCut && isFragment);
    h1->DrawCopy("colz");
    drawText("LO Photons",0.3680963,0.229118);
    
    c100->cd(2);
    h2->DrawCopy("colz");
    drawText("Fragmentation Photons",0.3680963,0.229118);
    c100->SaveAs("scatteringPlot_iso.pdf");
  }
}

*/
