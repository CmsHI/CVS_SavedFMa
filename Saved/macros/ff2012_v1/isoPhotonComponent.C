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

TString fname1 = "../forest/barrelHiForestPhoton_MCAllphoton30.root"; // 35k
TString fname2 = "../forest/barrelHiForestPhoton_MCemJet80_41007events.root"; // 40k


float isoPhotonComponentDiff(TCut isoCut = "genCalIsoDR04<5", TString name="gen", TString cutName="gen level cut", bool drawScatter=false, bool drawLeg=false);
void getGenIsoDist(TH1D* theHist=0, TCut addCut = "");
void getPtDist(TH1D* theHist=0, TCut addCut="", bool isSig=true);

void isoPhotonComponent(float isoThr=1) {
  //  isoPhotonComponentDiff("genCalIsoDR04<5","gen","Gen Level cut (genIso<5GeV)");
  
  TCanvas* c100 = new TCanvas("c100","",500,500);
  
  
  TH1D* hIsoScan[10];
  TCut isoCutScan[40];
  /*  
  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    TH1D* temph1 = new TH1D("temph1","",20,0,20);
    TH1D* temph2 = new TH1D("temph2","",20,0,20);
    
    hIsoScan[icent] = new TH1D(Form("isoScan_%d",icent),";sumISO cut (GeV); ratio Fragmentation/All",30,-0.5,4.5);
    hIsoScan[icent]->SetAxisRange(0,0.3,"Y");
    hIsoScan[icent]->Sumw2();
    for (int ii=0; ii<=5; ii++){ 
      isoCutScan[ii]  = Form("cc4+cr4+ct4PtCut20 < %f",(float)ii);
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      TCut centCut      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
      get/GenIsoDist(temph1,centCut && isoCutScan[ii]);
      getGenIsoDist(temph2,centCut && isoCutScan[ii] && "abs(genMomId)<22");
      float n1 = temph1->Integral();
      float n2 = temph2->Integral();
      cout << " ratio = " << n2/n1 << endl;
      hIsoScan[icent]->SetBinContent( hIsoScan[icent]->FindBin( ii ), n2/n1);
      hIsoScan[icent]->SetBinError( hIsoScan[icent]->FindBin( ii ), 0.0000001);
    }
    handsomeTH1(hIsoScan[icent],color[icent]);
    
  }
  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    if (icent==1)  hIsoScan[icent]->Draw();
    else hIsoScan[icent]->Draw("same");
  }
  */
  
  TCanvas* c2 =new TCanvas("can_reco","",1132,344);
  TCut isoCut = Form("cc4+cr4+ct4PtCut20 < %f",isoThr);
  if ( isoThr < 10 ) 
    isoCut = isoCut && "sigmaIetaIeta < 0.01";
  makeMultiPanelCanvas(c2,4,1,0.0,0.0,0.2,0.15,0.02);
  
  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    TCut centCut      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
    c2->cd(nCent_std-icent+1);
    bool drawFrag(false);
    //    if ( icent ==1 ) drawFrag = true;
    TString theCutName ;
      if ( isoThr < 10 ) 
	theCutName = Form("PYTHIA (sumISO<%dGeV)",(int)isoThr);
      else 
	theCutName = "PYTHIA";
    isoPhotonComponentDiff(centCut && "hadronicOverEm<0.1" && isoCut,"reco", theCutName ,drawFrag, (icent==4));
    drawText(Form("%.0f%% - %.0f%%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.5,0.5);
  }

  c2->SaveAs(Form("plot_photonComponents_sumIsoCut%d.pdf",(int)isoThr));

  TCanvas* c2Cent =new TCanvas("can_recoCent","",500,500);
  isoPhotonComponentDiff("hadronicOverEm<0.1" && isoCut,"reco", "PYTHIA" ,true,true);
  c2Cent->SaveAs(Form("plot_photonComponents_AllCent_sumIsoCut%d.pdf",(int)isoThr));

  
  
  TCanvas* c3 =new TCanvas("genIsoDist","",1000,350);
  makeMultiPanelCanvas(c3,4,1,0.0,0.0,0.2,0.15,0.02);
  TH1D* hh[10];
  TH1D* hhDir[10];
  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    TCut centCut    ;
    if ( icent > 0 )
      centCut = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
    else 
      centCut = "yEvt.hiBin>=0 && yEvt.hiBin<=40";
    
    hh[icent] = new TH1D(Form("hh_icent%d",icent),";GenIso (GeV); Event fraction",20,0,20);
    hh[icent]->Sumw2();
    hhDir[icent] = (TH1D*)hh[icent]->Clone(Form("hhDir_icent%d",icent));
    getGenIsoDist(hh[icent],centCut && isoCut);
    getGenIsoDist(hhDir[icent],centCut && isoCut && "abs(genMomId)==22");
    handsomeTH1(hh[icent],1);
    handsomeTH1(hhDir[icent],2);
    
    double a = hh[icent]->Integral();
    hhDir[icent]->Scale(1./a);
    hhDir[icent]->SetFillStyle(3001);
    hhDir[icent]->SetFillColor(2);
    scaleInt(hh[icent]);
  }
  
  
  TF1 *myfit[5];  
  TF1 *outF[5];
  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    c3->cd(nCent_std - icent + 1);
    myfit[icent] = new TF1(Form("myfit_icent%d",icent),"[0]*exp(-x/[1])", 0, 10);
    outF[icent] = new TF1(Form("outF_icent%d",icent),"[0]*exp(-x/[1])", 0, 10);
    myfit[icent]->SetParameters(0.1,2);
    TH1D* htt = (TH1D*)hhDir[icent]->Clone(Form("htt_%s",hh[icent]->GetName()));
    htt->Fit(Form("myfit_icent%d",icent),"LL M O Q","",0,20);
    outF[icent]->SetParameter(0, myfit[icent]->GetParameter(0));
    outF[icent]->SetParameter(1, myfit[icent]->GetParameter(1));
    outF[icent]->SetLineStyle(2);
    outF[icent]->SetLineColor(2);
    hhDir[icent]->SetAxisRange(1.e-4,5,"Y");
    hhDir[icent]->Draw("hist");
    hh[icent]->Draw("same pe");
    outF[icent]->Draw("same");
    gPad->SetLogy();
    
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5,0.5);
    
    if ( icent == nCent_std) {
      TLegend *leg1 = new TLegend(0.3430047,0.7361548,0.9947136,0.9257225,NULL,"brNDC");
      if ( isoThr < 10 ) 
	easyLeg(leg1,Form("PYTHIA (SumIso<%dGeV & #sigma_{#eta#eta}",(int)isoThr));    
      else 
	easyLeg(leg1,"PYTHIA");
      leg1->AddEntry(hh[icent],"LO + Frag.","pl");
      leg1->AddEntry(hhDir[icent],"LO only","fl");
      leg1->AddEntry(outF[icent],"LO Fit (exp)","l");
      leg1->Draw();
    }
  }
  
  c3->SaveAs(Form("genIsoDistribution_sumIsoCut%d.pdf",(int)isoThr));
  
  
  TCanvas* c31 =new TCanvas("genIsoDist31","",1000,350);
  makeMultiPanelCanvas(c31,4,1,0.0,0.0,0.2,0.15,0.02);
  TH1D* h2All[10];
  TH1D* h2Dir[10];
  TH1D* h2Frg[10];
  TH1D* h2Bkg[10];
  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    int lowCent = centBin_std[icent-1];
    int highCent = centBin_std[icent]-1;
    TCut centCut    ;
    if ( icent > 0 )
      centCut = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
    else
      centCut = "yEvt.hiBin>=0 && yEvt.hiBin<=40";

    h2Dir[icent] = new TH1D(Form("h2Dir_icent%d",icent),";p_{T} of photon (GeV); Fraction",7,57,119);
    h2Dir[icent]->Sumw2();
    h2Frg[icent] = (TH1D*)h2Dir[icent]->Clone(Form("h2Frg_icent%d",icent));
    h2Bkg[icent] = (TH1D*)h2Dir[icent]->Clone(Form("h2Bkg_icent%d",icent));
    h2All[icent] = (TH1D*)h2Dir[icent]->Clone(Form("h2All_icent%d",icent));
    getPtDist(h2Frg[icent],centCut && isoCut && "abs(genMomId)<22");
    getPtDist(h2Dir[icent],centCut && isoCut && "abs(genMomId)==22");
    getPtDist(h2Bkg[icent],centCut && isoCut,false);
    
    handsomeTH1(h2Dir[icent],1);
    handsomeTH1(h2Frg[icent],2);
    handsomeTH1(h2Bkg[icent],4);
    
    double csDij80 = 9.869e-5;
    float nEvtEmj80     = 41000;
    float effEmj80     = 0.204;
    float weightEmj80 = csDij80*effEmj80/nEvtEmj80;
    
    double csPho30 = 1.59e-6; //effective
    float nEvtPho30     = 35000;
    float weightPho30  = csPho30 / nEvtPho30;
    h2Dir[icent]->Scale(weightPho30);
    h2Frg[icent]->Scale(weightPho30);
    h2Bkg[icent]->Scale(weightEmj80);
    
    h2All[icent]->Add(h2Dir[icent]);
    h2All[icent]->Add(h2Frg[icent]);
    h2All[icent]->Add(h2Bkg[icent]);
    
  }

  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    c31->cd(nCent_std - icent + 1);
    h2Dir[icent]->SetAxisRange(1.e-10,1.e-5,"Y");
    h2Dir[icent]->DrawCopy();
    h2Frg[icent]->DrawCopy("same");
    h2Bkg[icent]->DrawCopy("same");

    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.6,0.6);
    gPad->SetLogy();
    if ( icent == nCent_std){
      //      drawText("After Iso. cut",0.3,0.88,1,17);
      TLegend* leg31 =  new TLegend(0.3506292,0.641055,0.8995147,0.9275199,NULL,"brNDC");
      easyLeg(leg31,"PYTHIA photons");
      leg31->AddEntry(h2Dir[icent],"LO");
      leg31->AddEntry(h2Frg[icent],"Frag.");
      leg31->AddEntry(h2Bkg[icent],"Meson decay bkg.");
      leg31->Draw();
    }
    
    if ( (icent == nCent_std-1) && (isoThr<10) ) {
      drawText(Form("SumIso<%dGeV",(int)isoThr),0.1,0.88,1,17);
      drawText("Shower shape cut (#sigma_{#eta#eta}<0.01)",0.1,0.80,1,17);
    }
  }
  
    c31->SaveAs(Form("allKindPhotonSpectra_sumIsoCut%d.pdf",(int)isoThr));


  TCanvas* c31Cent =new TCanvas("c31Cent","",500,500);
  int icent = 0;
  h2Dir[icent]->SetAxisRange(1.e-10,1.e-5,"Y");
  h2Dir[icent]->Draw();
  h2Frg[icent]->Draw("same");
  h2Bkg[icent]->Draw("same");
  gPad->SetLogy();
  TLegend* leg31 =  new TLegend(0.3891129,0.6631356,0.9395161,0.9300847,NULL,"brNDC");
  easyLeg(leg31,"PYTHIA photons");
  leg31->AddEntry(h2Dir[icent],"(i),(ii) LO");
  leg31->AddEntry(h2Frg[icent],"(iii) Frag.");
  leg31->AddEntry(h2Bkg[icent],"(iv) Meson decay bkg.");
  leg31->Draw();

  c31Cent->SaveAs(Form("allKindPhotonSpectra_allCent_sumIsoCut%d.pdf",(int)isoThr));
  


  for ( int icent = 0 ; icent<=nCent_std ; icent++) {
    divideWOerr(h2Dir[icent],h2All[icent]);
    divideWOerr(h2Frg[icent],h2All[icent]);
    divideWOerr(h2Bkg[icent],h2All[icent]);
  }
  TCanvas* c32 =new TCanvas("genIsoDist32","",1000,350);
  makeMultiPanelCanvas(c32,4,1,0.0,0.0,0.2,0.15,0.02);

  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    c32->cd(nCent_std - icent + 1);
    

    h2Dir[icent]->SetAxisRange(0,2,"Y");
    h2Dir[icent]->DrawCopy();
    h2Frg[icent]->DrawCopy("same");
    h2Bkg[icent]->DrawCopy("same");

    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.3,0.55);
    // gPad->SetLogy();
    if ( icent == nCent_std){
      //      drawText("After Iso. cut",0.3,0.88,1,15);
      TLegend* leg31 =  new TLegend(0.2006292,0.651055,0.8995147,0.9275199,NULL,"brNDC");
      easyLeg(leg31,"Relative Fraction");
      leg31->AddEntry(h2Dir[icent],"(i),(ii) LO");
      leg31->AddEntry(h2Frg[icent],"(iii) Frag.");
      leg31->AddEntry(h2Bkg[icent],"(iv) Meson decay bkg.");
      leg31->Draw();
    }

    if ( (icent == nCent_std-1) && (isoThr<10) ) {
      drawText(Form("SumIso<%dGeV",(int)isoThr),0.1,0.88,1,15);
      drawText("Shower shape cut (#sigma_{#eta#eta}<0.01)",0.1,0.80,1,15);
    }


  }
  
  c32->SaveAs(Form("AllKindPhotonRelativeFraction_sumIsoCut%d.pdf",(int)isoThr));

  TCanvas* c32Cent =new TCanvas("c32Cent","",500,500);
  icent = 0;
  h2Dir[icent]->SetAxisRange(0,2,"Y");
  h2Dir[icent]->Draw();
  h2Frg[icent]->Draw("same");
  h2Bkg[icent]->Draw("same");
  TLegend* leg32 =  new TLegend(0.2506292,0.601055,0.8995147,0.8575199,NULL,"brNDC");
  easyLeg(leg32,"Relative Fraction");
  leg32->AddEntry(h2Dir[icent],"(i),(ii) LO");
  leg32->AddEntry(h2Frg[icent],"(iii) Frag.");
  leg32->AddEntry(h2Bkg[icent],"(iv) Meson decay bkg.");
  leg32->Draw();

  c32Cent->SaveAs(Form("AllKindPhotonRelativeFraction_centAll_sumIsoCut%d.pdf",(int)isoThr));



  
  TCanvas* c4 =new TCanvas("genIsoDist4","",1000,350);
  makeMultiPanelCanvas(c4,4,1,0.0,0.0,0.2,0.15,0.02);
  
  
  for ( int icent = 1 ; icent<=nCent_std ; icent++) {
    c4->cd(nCent_std - icent + 1);
    hh[icent]->SetAxisRange(1.e-4,50,"Y");
    hh[icent]->Draw("pe");
    hhDir[icent]->Draw("same");
    //   outF[icent]->Draw("same");
    gPad->SetLogy();
    
    int lowerCent = centBin_std[icent-1];
    int upperCent = centBin_std[icent]-1;
    drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5,0.5);
    float genIsoFrac = hh[icent]->Integral(1, hh[icent]->FindBin(4.2)) / hh[icent]->Integral(); 
    if ( icent == nCent_std) {
            
      if ( isoThr < 10 )
	drawText(Form("PYTHIA (sumISO<%dGeV)",(int)isoThr),0.3,0.85,1,15);
      else
	drawText("PYTHIA",0.3,0.85,1,15);
      drawText("LO photons",0.3,0.78,2,15);
    }
    float par1 = outF[icent]->GetParameter(1);
    float p99 = 4.6 * par1;
    drawText(Form("99%% of LO in GenIso < %.1fGeV",p99),0.28,0.69,1,13); 
    drawText(Form("%.0f%% of LO+Frag. in GenIso < %.1fGeV",genIsoFrac*100., p99),0.28,0.62,1,13);
    cout << " fraction = " << genIsoFrac << endl;
  }
  
  c4->SaveAs(Form("recoCutAndGenCut_sumIsoCut%d.pdf",(int)isoThr));



TCanvas* c41 =new TCanvas("genIsoDist45GeV","",1000,350);
 makeMultiPanelCanvas(c41,4,1,0.0,0.0,0.2,0.15,0.02);
 
for ( int icent = 1 ; icent<=nCent_std ; icent++) {
  c41->cd(nCent_std - icent + 1);
  hh[icent]->SetAxisRange(1.e-4,50,"Y");
  hh[icent]->Draw("pe");
  hhDir[icent]->Draw("same hist");
  //   outF[icent]->Draw("same");                                                                                                          
  gPad->SetLogy();

  int lowerCent = centBin_std[icent-1];
  int upperCent = centBin_std[icent]-1;
  drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5,0.5);
  float genIsoFrac1 = hh[icent]->Integral(1, hh[icent]->FindBin(4.9999)) / hh[icent]->Integral();
  float genIsoFrac2 = hhDir[icent]->Integral(1, hhDir[icent]->FindBin(4.9999)) / hhDir[icent]->Integral();
  if ( icent == nCent_std) {
    
    if ( isoThr < 10 )
      drawText(Form("PYTHIA (SumIso<%dGeV)",(int)isoThr),0.3,0.85,1,15);
    else
      drawText("PYTHIA",0.3,0.85,1,15);
    drawText("LO photons",0.3,0.78,2,15);
  }
  float par1 = outF[icent]->GetParameter(1);
  float p99 = 4.6 * par1;
  drawText(Form("%.0f%% of LO+Frag. in Iso^{GEN} < 5GeV",genIsoFrac1*100),0.28,0.69,1,13);
  drawText(Form("%.0f%% of LO in Iso^{GEN} < 5GeV",genIsoFrac2*100.),0.28,0.62,1,13);
 }

 c41->SaveAs(Form("recoCutAndGenCut5GeV_SumIsoCut%d.pdf",(int)isoThr));
 

 TCanvas* c41Cent =new TCanvas("genIsoDist45GeVCent","",500,500);
 hh[0]->SetAxisRange(1.e-4,50,"Y");
 hh[0]->Draw("pe");
 hhDir[0]->Draw("same hist");
 gPad->SetLogy();
 if ( isoThr < 10 )
   drawText(Form("PYTHIA (SumIso<%dGeV)",(int)isoThr),0.3,0.85,1,15);
 else
   drawText("PYTHIA",0.3,0.85,1,15);
 drawText("LO photons",0.3,0.78,2,15);
 float genIsoFrac1 = hh[0]->Integral(1, hh[0]->FindBin(4.9999)) / hh[0]->Integral();
 float genIsoFrac2 = hhDir[0]->Integral(1, hhDir[0]->FindBin(4.9999)) / hhDir[0]->Integral();
 drawText(Form("%.0f%% of LO+Frag. in Iso^{GEN} < 5GeV",genIsoFrac1*100),0.28,0.69,1,13);
 drawText(Form("%.0f%% of LO in Iso^{GEN} < 5GeV",genIsoFrac2*100.),0.28,0.62,1,13); 
 c41Cent->SaveAs(Form("recoCutAndGenCut5GeV_AllCent_sumIsoCut%d.pdf",(int)isoThr));
}

			    
			    


void getGenIsoDist(TH1D* theHist, TCut addCut) {
  TFile *f1=new TFile(fname1.Data());
  TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
  photon1->AddFriend("yEvt=yongsunHiEvt"       ,fname1.Data());
  photon1->AddFriend("ySkim=yongsunSkimTree"   ,fname1.Data());
  photon1->AddFriend("yHlt=yongsunHltTree"     ,fname1.Data());
  photon1->AddFriend("tgj");

  TCut generalCut  = etaCut && genMatchCut0 && "hadronicOverEm<0.1" ;
  TCut ptCut       = "genMatchedPt>60 && isGenMatched";
  TCut allcut = generalCut && ptCut && (isFragment||isPrompt) && addCut;
  theHist->Reset();
  TH1D* tempHist = (TH1D*)theHist->Clone("tempHist");
  tempHist->Reset();
  photon1->Draw("genCalIsoDR04>>tempHist", Form("(%s)", allcut.GetTitle()));
  //photon1->Draw("genCalIsoDR04>>tempHist", Form("(%s) * tgj.reweight", allcut.GetTitle()));
  cout << " the cut = " << Form("(%s)", allcut.GetTitle()) << endl;
  theHist->Add(tempHist);
}

void getPtDist(TH1D* theHist, TCut addCut, bool isSig) {
  TString theFname;
  if ( isSig ) theFname = fname1;
  else         theFname = fname2;

  TFile *f1=new TFile(theFname.Data());
  TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
  photon1->AddFriend("yEvt=yongsunHiEvt"       ,theFname.Data());
  photon1->AddFriend("ySkim=yongsunSkimTree"   ,theFname.Data());
  photon1->AddFriend("yHlt=yongsunHltTree"     ,theFname.Data());
  photon1->AddFriend("tgj",                     theFname.Data());

  TCut generalCut  = etaCut && genMatchCut0 && "hadronicOverEm<0.1";
  TCut ptCut       = "genMatchedPt>60 && isGenMatched";
  TCut allcut ;
  if (isSig) allcut = generalCut && ptCut && (isFragment||isPrompt) && addCut;
  else       allcut = etaCut && "hadronicOverEm<0.1" && addCut;
  theHist->Reset();
  TH1D* tempHist = (TH1D*)theHist->Clone("tempHist");
  tempHist->Reset();
  photon1->Draw("pt>>tempHist", Form("(%s)", allcut.GetTitle()));
  theHist->Add(tempHist);
}



float isoPhotonComponentDiff(TCut isoCut,TString name, TString cutName, bool drawScatter, bool drawLeg){

  TFile *f1=new TFile(fname1.Data());
  TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
  photon1->AddFriend("yEvt=yongsunHiEvt"       ,fname1.Data());
  photon1->AddFriend("ySkim=yongsunSkimTree"   ,fname1.Data());
  photon1->AddFriend("yHlt=yongsunHltTree"     ,fname1.Data());
  photon1->AddFriend("tgj");

  TCut generalCut  = etaCut && genMatchCut0 ;
  

  TH1D* hGenPt1 = new TH1D ("hgenPt1",";E_{T} of photon (GeV);Event fraction",40,0,200);
  hGenPt1->Sumw2();
  photon1->Draw("genMatchedPt>>hgenPt1",generalCut && (isFragment||isPrompt));
  TH1D* hGenPt2 = (TH1D*)hGenPt1->Clone("hgenPt2");
  hGenPt2->Sumw2();
  photon1->Draw("genMatchedPt>>hgenPt2",generalCut && isFragment);
  hGenPt2->SetFillColor(48);
  hGenPt2->SetFillStyle(3001);
  hGenPt1->SetAxisRange(0.5,1.e4,"Y");
  handsomeTH1(hGenPt1);
  hGenPt1->Draw("hist" );
  hGenPt2->Draw("same hist");
  gPad->SetLogy();
  jumSun(60,0.5,60,1.e4);
  
  if (drawLeg){
    TLegend* leg1 =  new TLegend(0.2406292,0.751055,0.8995147,0.9175199,NULL,"brNDC");
    int n1 = hGenPt1->Integral( hGenPt1->FindBin(60.001), hGenPt1->GetNbinsX() );
    int n2 = hGenPt2->Integral( hGenPt2->FindBin(60.001), hGenPt1->GetNbinsX() );
    easyLeg(leg1,"LO+Frag. Photons");
    leg1->AddEntry(hGenPt1,Form("LO + Frag. Photons : %d",n1),"l");
    leg1->AddEntry(hGenPt2,Form("Frag. photons : %d",n2),"lf");
    leg1->Draw();
  }
  
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
  hGenPt1iso->SetAxisRange(40,190,"X");
  
  hGenPt1iso->DrawCopy("hist");
  hGenPt2iso->DrawCopy("same hist");
  gPad->SetLogy();
  jumSun(60,1.e-4,60,1);
  
  TLegend* leg2 =   new TLegend(0.2406292,0.701055,0.8995147,0.9175199,NULL,"brNDC");
  float n1i = hGenPt1iso->Integral( hGenPt1iso->FindBin(60.001), hGenPt1iso->GetNbinsX() );
  float n2i = hGenPt2iso->Integral( hGenPt2iso->FindBin(60.001), hGenPt2iso->GetNbinsX() );
  float ratio = (float)n2i/n1i;
    
  easyLeg(leg2,cutName.Data());
  leg2->AddEntry(hGenPt1iso,Form("LO + Frag."),"l");
  leg2->AddEntry(hGenPt2iso,Form("Frag. only (%d %%)",(int)(ratio*100.)),"f");
  leg2->Draw();

  
  if ( drawScatter ) {
    TCanvas* c100 =new TCanvas("can_scatter","",1000,500);
    c100->Divide(2,1);
    c100->Draw();
    c100->cd(1);
    TH2D* h1 = new TH2D("sc1",";GenIso (GeV); SumIso (GeV)",50,0,50,100,-50,50);
    TH2D* h2 = new TH2D("sc2",";GenIso (GeV); SumIso (GeV)",50,0,50,100,-50,50);
    photon1->Draw("(cc4+cr4+ct4PtCut20)/0.9:genCalIsoDR04 >> sc1", generalCut && isPrompt);
    photon1->Draw("(cc4+cr4+ct4PtCut20)/0.9:genCalIsoDR04 >> sc2", generalCut && isFragment);
    h1->DrawCopy("colz");
    drawText("LO Photons",0.3680963,0.229118);
    
    c100->cd(2);
    h2->DrawCopy("colz");
    drawText("Fragmentation Photons",0.3680963,0.229118);
    //  c100->SaveAs("scatteringPlot_iso.pdf");
  }

  return ratio;
}

