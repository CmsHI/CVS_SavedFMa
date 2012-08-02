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


void sysStudyGenIsoDiff(TH1D* thehist=0, float genIsoThr=1000, float recoIsoThr=1000, TCut addCut="yEvt.hiBin >= 0 && yEvt.hiBin<=3", TString fname1="../forest/barrelHiforest_qcdAllPhoton30_allCent.root");

void sysStudyGenIso() {
  TString fname1="../forest/barrelHiForestPhoton_MCAllphoton30.root";
  
  TH1D* h[5][5];  // [icent] [ iso condition]
  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    TCut centCut = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);

    h[icent][1]= new TH1D(Form("h_icent%d_geniso",icent),";x_{J,G}",15,0.001,1.999);
    h[icent][2]= new TH1D(Form("h_icent%d_recoiso",icent),";x_{J,G}",15,0.001,1.999);

    h[icent][1]->SetNdivisions(505);
    h[icent][2]->SetNdivisions(505);
    sysStudyGenIsoDiff(h[icent][1],   5,  20,    centCut, fname1);
    sysStudyGenIsoDiff(h[icent][2],   20,  1,   centCut, fname1);
  }
  
  TCanvas* c3 =new TCanvas("genIsoDist","",1200,350);
  makeMultiPanelCanvas(c3,4,1,0.0,0.0,0.2,0.15,0.02);
  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    c3->cd(nCent_std - icent + 1);
    handsomeTH1(h[icent][1],1);
    handsomeTH1(h[icent][2],2);
    h[icent][1]->SetAxisRange(0,0.3,"Y");
    h[icent][1]->Draw();
    h[icent][2]->Draw("same");
    cout << " mean change = " << h[icent][1]->GetMean() - h[icent][2]->GetMean() << endl;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.22,0.8,1,14);
    
    cout << "in 0.4 - 1.6   = " << h[icent][1]->Integral( h[icent][1]->FindBin(0.4), h[icent][1]->FindBin(1.6)) / h[icent][1]->Integral() << endl;
    if ( icent == nCent_std) {
      //      drawText("PYTHIA + HYDJET",0.22,0.87,1,14);
      TLegend *leg1 = new TLegend(0.6028087,0.6634871,0.9827131,0.8846495,NULL,"brNDC");
      easyLeg(leg1,"PYTHIA+HYDJET");
      leg1->AddEntry(h[icent][1],"GenIso Cut","pl");
      leg1->AddEntry(h[icent][2],"SumIso Cut","pl");
      leg1->Draw();
    }
  }

  c3->SaveAs("contributionFromNonGenIso.pdf");
  
  TCanvas* c4 =new TCanvas("ratioCanvas","",1200,350);
  makeMultiPanelCanvas(c4,4,1,0.0,0.0,0.2,0.15,0.02);
  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    c4->cd(nCent_std - icent + 1);
       
    h[icent][3] = (TH1D*)h[icent][1]->Clone(Form("hRatio_icent%d_iso",icent));
    h[icent][3]->SetYTitle("Ratio of (GenIso)/(SumIso)");
    divideWOerr(h[icent][3],h[icent][2]);
    h[icent][3]->SetAxisRange(0,2,"Y");
    h[icent][3]->Draw();

    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.22,0.8,1,14);
    if ( icent == nCent_std) {
      drawText("PYTHIA + HYDJET",0.22,0.87,1,14);
      TLegend *leg1 = new TLegend(0.6028087,0.6634871,0.9827131,0.8846495,NULL,"brNDC");
      easyLeg(leg1,"PYTHIA+HYDJET");
      leg1->AddEntry(h[icent][3],"Ratio","pl");
      leg1->Draw();
    }
  }
  c4->SaveAs("contributionFromNonGenIsoRatio.pdf");

  
  
  TFile outf = TFile("genIsoVSrecoIso.root","recreate");
  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    h[icent][1]->Write();
    h[icent][2]->Write();
    h[icent][3]->Write();

  }
  outf.Close();
}
void sysStudyGenIsoDiff(TH1D* thehist, float genIsoThr, float recoIsoThr, TCut addCut, TString fname1)  { 

  //  TString fname1="../forest/barrelHiForestPhoton_MCAllphoton30.root";
  multiTreeUtil* photon1 = new multiTreeUtil();
  photon1->addFile("../forest/barrelHiforest_qcdAllPhoton30_allCent.root" ,  "yongsunPhotonTree", "" , 1.59e-6);
  photon1->addFile("../forest/barrelHiforest_qcdAllPhoton50_allCent.root" ,  "yongsunPhotonTree", "" , 7.67e-7);
  photon1->addFile("../forest/barrelHiforest_qcdAllPhoton80_allCent.root" ,  "yongsunPhotonTree", "" , 1.72e-7);
  photon1->AddFriend("yEvt=yongsunHiEvt");
  photon1->AddFriend("ySkim=yongsunSkimTree");
  photon1->AddFriend("yHlt=yongsunHltTree");  
  photon1->AddFriend("tgj");
  
  TCut generalEvtCut  = "tgj.offlSel && tgj.photonEt>60 ";
  TCut photonTagCut = " leading==1 && hadronicOverEm<0.1 && sigmaIetaIeta<0.01 && isGenMatched";
  TCut theRecoCut = Form("(cc4+cr4+ct4PtCut20)/0.9<%f",recoIsoThr);
  TCut theGenCut = Form("isGenMatched && genCalIsoDR04< %f",genIsoThr);
  TCut generalCut  = generalEvtCut && photonTagCut && theGenCut && theRecoCut && addCut;
  TCut generalGjCut  = generalCut && "tgj.jetEt>30";
  
  TCut sigDphi = " acos(cos(photonPhi-jetPhi)) > 3.141592 * 7./8.";
  TCut sbDphi = " acos(cos(photonPhi-jetPhi)) > 0.7 &&  acos(cos(photonPhi-jetPhi)) < 3.141592/2. ";
  float subRat = 3.141592 * (1-7./8.) / ( 3.141592/2. - 0.7); 

  thehist->Reset();
  thehist->Sumw2();
  TH1D* hxRaw = (TH1D*)thehist->Clone("hxRaw");  
  TH1D* hxSb = (TH1D*)hxRaw->Clone("hxSb");
  hxRaw->Sumw2();
  hxSb->Sumw2();
  photon1->Draw2(hxRaw, "tgj.jetEt/corrPt", generalGjCut && sigDphi, "reweight");
  photon1->Draw2(hxSb,  "tgj.jetEt/corrPt", generalGjCut && sbDphi,  "reweight");
  
  TH1D* hScale1 = new TH1D("hScale1","",10,-10000,10000);
  TH1D* hScale2 = new TH1D("hScale2","",10,-10000,10000);
  photon1->Draw2(hScale1,  "tgj.jetEt", generalGjCut,  "reweight");
  photon1->Draw2(hScale2,  "tgj.jetEt", generalCut,  "reweight");
  float scale = hScale1->Integral() / hScale2->Integral();
  
  TH1D* hxCorr = (TH1D*)hxRaw->Clone("hxCorr");
  hxCorr->Add(hxSb, -subRat);
  handsomeTH1(hxRaw,1);
  handsomeTH1(hxCorr,2);
  handsomeTH1(hxSb,4);
  //  hxRaw->Draw();
  //  hxSb->Draw("same");
  //  hxCorr->Draw("same");
  
  scaleInt(hxCorr);
  hxCorr->Scale( scale);
  //  hxCorr->Draw();
  thehist->Add(hxCorr);
  
}

