#include <TGraphAsymmErrors.h>
#include <TFile.h>
#include <TTree.h>
#include "TChain.h"
#include <TH1F.h>
#include "TH2.h"
#include "TH3.h"
#include <TCut.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCanvas.h>
#include "TProfile.h"
#include "TProfile2D.h"
#include <iostream>

#include "Saved/DiJetAna/macros/aliases_dijet.C"
#include "Saved/DiJetAna/macros/anaY1J/ptresolution.h"
#include "Saved/Utilities/macros/cplot/CPlot.h"           // helper class for plots
#include "Saved/Utilities/macros/graph/tgraphTools.C"
using namespace std;

//const int nBin=25;
//Float_t bin[nBin+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120,140,160,200,240,300};
const int nBin=21;
Float_t bin[nBin+1]={//1, 5, 6, 8, 10, 12, 15, 
  18, 21, 24, 28, 32, 37, 43, 50, 56, 64, 74, 84,
     97, 114, 133, 153, 174, 196, 220, 245, 272, 300//, 330, 362, 395, 430, 468,
     //507, 548, 592, 638, 686, 737, 790, 846, 905, 967,
     // 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000};
     }; 
const int nBinRat=300;
Float_t binRat[nBinRat+1];
const int nBinEta=4;
Float_t binEta[nBinEta+1]={-2,-1,0,1,2};

TH3F * JES2D(TTree * t,TString var="nljet/nlrjet:nlrjet:nlrjeta",TCut sel="",TCut cut="",TString tag="")
{
  cout << "var: " << var << endl;
  cout << "Trigger: " << TString(sel&&cut) << ": " << t->GetEntries(sel&&cut) << endl;

  //TH3F *h3d = new TH3F("h"+tag,"",nBinEta,binEta,nBin,bin,nBinRat,binRat);
  TH3F *h3d = new TH3F("h"+tag,"#eta^{Jet};p_{T}^{Jet};Reponse",8,-2,2,50,0,300,50,0,2);
  t->Draw(var+">>h"+tag,sel&&cut,"goff");

  return h3d;
}

TH2F * JES(TTree * t,TString var="nljet/nlrjet:nlrjet",TCut sel="",TCut cut="",TString tag="")
{
  cout << "var: " << var << endl;
  cout << "Trigger: " << TString(sel&&cut) << ": " << t->GetEntries(sel&&cut) << endl;

  TH2F *h2d = new TH2F("h"+tag,"",nBin,bin,nBinRat,binRat);
  t->Draw(var+">>h"+tag,sel&&cut,"goff");

  h2d->FitSlicesY(0,0,-1,5);
  return h2d;
}

TChain * scaleResJet(bool doMC=1,
    TString infile0="dj_Data_MinBias_DijetUnquenched50and80_d20101125to27.root",
    //TString infile0="dj_Data_MinBias_DijetUnquenched50_d20101127_MatchedJetGoodTrk1127v2.root",
    //TString infile0="dj_Data_MinBias_DijetUnquenched80_d20101125and1126_MatchedJetGoodTrk1127v2.root",
    //TString infile1="dj_Data_MinBias0to20_DijetUnquenched50_d20101124_StdJetGoodTrk1126.root",
    Float_t centMin=0,
    Float_t centMax=10,
    TString header="McDiJet-DataBackground"
    )
{
  TChain * dj = new TChain("djgen/djTree");
  //TChain * dj1 = new TChain("djcalo/djTree");

  dj->Add(infile0);
  dj->AddFriend("djcalo = djcalo/djTree",infile0);
  aliases_dijet(dj,1.2,doMC,"djcalo");
  cout << "dj0 Total: " << dj->GetEntries() << endl;

  TFile * fout = new TFile(Form("JES_%.0fto%.0f.root",centMin,centMax),"RECREATE");
  //dj1->Add(infile1);
  //dj1->AddFriend("djgen = djgen/djTree",infile1);
  //aliases_dijet(dj1,1.2,doMC,"djgen");
  //cout << "dj1 Total: " << dj1->GetEntries() << endl;

  TCut evtSel(Form("cent>=%.0f && cent<%.0f && nlrjet>20 && abs(nljeta)<2 && alrjet>20 && abs(aljeta)<2 && jdphi>TMath::Pi()*5/6",
	centMin,centMax));
  //TCut evtSel(Form("cent>=%.0f && cent<%.0f && nlrjet>50 && abs(nljeta)<2 && alrjet>0 && abs(aljeta)<2 && jdphi>TMath::Pi()*5/6",
  //	centMin,centMax));
  //evtSel = evtSel && "djgen.nljet>0&&djgen.aljet>0" //for now abs eff --- no selection on mc
  TCut evtSelAw = evtSel;// && "alrjet>40";
  //TCut evtSel("HLT_HIJet50U && cent<10 ");
  
  for (int i=0; i<=nBinRat;++i) binRat[i]=i*3./nBinRat;

  TH1D * hEtNr = new TH1D("hEtNr","",nBin,bin);
  dj->Draw("nljet>>hEtNr",evtSel);

  TCanvas * cEtNr2D = new TCanvas("cEtNr2D","cEtNr2D",500,550);
  TH2D * hEtNr2D = new TH2D("hEtNr2D",";E_{T}^{GenJet};E_{T}^{CaloJet}",nBin,bin,nBinRat,binRat);
  dj->Draw("nlrjet/nljet:nljet>>hEtNr2D",evtSel,"colz");
  //hEtNr2D->ProfileX()->Draw("same");
  TLine *l45 = new TLine(0,0,300,300);
  l45->SetLineStyle(2);
  //l45->Draw();
  cEtNr2D->Print(Form("EtNr2D_%.0fto%.0f.gif",centMin,centMax));
  cEtNr2D->Print(Form("EtNr2D_%.0fto%.0f.pdf",centMin,centMax));

  TCanvas * cEtAw2D = new TCanvas("cEtAw2D","cEtAw2D",500,550);
  TH2D * hEtAw2D = new TH2D("hEtAw2D",";E_{T}^{GenJet};E_{T}^{CaloJet}",nBin,bin,nBinRat,binRat);
  dj->Draw("alrjet/aljet:aljet>>hEtAw2D",evtSelAw,"colz");
  //hEtAw2D->ProfileX()->Draw("same");
  //l45->Draw();
  cEtAw2D->Print(Form("EtAw2D_%.0fto%.0f.gif",centMin,centMax));
  cEtAw2D->Print(Form("EtAw2D_%.0fto%.0f.pdf",centMin,centMax));

  TCanvas * cDr = new TCanvas("cDr","cDr",500,550);
  dj->Draw("nlrjdr",evtSel,"hist");
  dj->Draw("alrjdr",evtSelAw,"sameE");
  
  TCanvas * cCent = new TCanvas("cCent","cCent",500,550);
  dj->Draw("cent",evtSel,"hist");
  dj->Draw("cent",evtSelAw,"sameE");

  // 1D Response
  TH2F * hJESNr2D = JES(dj,"nlrjet/nljet:nljet",evtSel,"nlrjdr<0.3","JESNr2D");
  TH1D * hJESNr2D_1 = (TH1D*)gDirectory->Get("hJESNr2D_1");
  TH1D * hJESNr2D_2 = (TH1D*)gDirectory->Get("hJESNr2D_2");
  TH2F * hJESAw2D = JES(dj,"alrjet/aljet:aljet",evtSelAw,"alrjdr<0.3","JESAw2D");
  TH1D * hJESAw2D_1 = (TH1D*)gDirectory->Get("hJESAw2D_1");
  TH1D * hJESAw2D_2 = (TH1D*)gDirectory->Get("hJESAw2D_2");

  // 2D Reponse
  TH3F * hJESNr3D = JES2D(dj,"nlrjet/nljet:nljet:nljeta",evtSel,"nlrjdr<0.3","JESNr3D");
  TH3F * hJESAw3D = JES2D(dj,"alrjet/aljet:aljet:aljeta",evtSelAw,"alrjdr<0.3","JESAw3D");

  // Draw
  TCanvas * cJES = new TCanvas("cJES","cJES",500,550);
  CPlot cpJES("JES","JES","E_{T}^{GenJet} [GeV]","E_{T}^{CaloJet}/E_{T}^{GenJet}");
  cpJES.SetYRange(0,1.5);
  cpJES.AddHist1D(hJESNr2D_1,"Leading Jet Reponse","E",kBlack,kFullCircle);
  cpJES.AddHist1D(hJESAw2D_1,"Away Jet Reponse","E",kRed,kFullSquare);
  cpJES.SetLegend(0.54,0.8,0.86,0.92);
  cpJES.SetLegendHeader(Form("Centrality %.0f to %.0f",centMin,centMax));
  cpJES.Draw(cJES,false);
  TLine *l = new TLine(0,1,bin[nBin],1);
  l->SetLineStyle(2);
  l->Draw();
  cJES->Print(Form("JES_%.0fto%.0f.gif",centMin,centMax));
  cJES->Print(Form("JES_%.0fto%.0f.pdf",centMin,centMax));
  
  TCanvas * cJReso = new TCanvas("cJReso","cJReso",500,550);
  CPlot cpJReso("JReso","JReso","E_{T}^{Calo} [GeV]","#sigma(E_{T}^{Calo}/E_{T}^{GenJet})");
  cpJReso.SetYRange(0,0.4);
  cpJReso.AddHist1D(hJESNr2D_2,"Leading Jet Resolution","E",kBlack,kOpenCircle);
  cpJReso.AddHist1D(hJESAw2D_2,"Away Jet Resolution","E",kRed,kOpenSquare);
  cpJReso.SetLegend(0.54,0.8,0.86,0.92);
  cpJReso.SetLegendHeader(Form("Centrality %.0f to %.0f",centMin,centMax));
  TF1 * fres = new TF1("fres",res,30,1000,0);
  TF1 * fres2 = new TF1("fres2",res2,30,1000,0);
  //hJESNr2D_2->Fit("fReso","","",30,400);
  cpJReso.Draw(cJReso,false);
  fResoNoBkgVsCalo->SetRange(30,400);
  fResoNoBkgVsCalo->Draw("same");
  //fResoWBkg->SetParameters(fReso->GetParameter(0),fReso->GetParameter(1),fReso->GetParameter(2));
  fResoWBkgVsCalo->SetRange(30,400);
  fResoWBkgVsCalo->SetLineColor(kGreen+2);
  fResoWBkgVsCalo->Draw("same");
  //fres->SetLineStyle(7);
  //fres->Draw("same");
  cJReso->Print(Form("JReso_%.0fto%.0f.gif",centMin,centMax));
  cJReso->Print(Form("JReso_%.0fto%.0f.pdf",centMin,centMax));
  
  TCanvas * cJES2DNr = new TCanvas("cJES2DNr","cJES2DNr",500,550);
  cJES2DNr->SetRightMargin(0.2);
  TProfile2D * hJESNr3D_pyx = (TProfile2D*)hJESNr3D->Project3DProfile("yx");
  hJESNr3D_pyx->SetMinimum(0.8);
  hJESNr3D_pyx->SetMaximum(1.2);
  hJESNr3D_pyx->Draw("colz");
  cJES2DNr->Print(Form("JES2DNr_%.0fto%.0f.gif",centMin,centMax));
  cJES2DNr->Print(Form("JES2DNr_%.0fto%.0f.pdf",centMin,centMax));

  TCanvas * cJES2DAw = new TCanvas("cJES2DAw","cJES2DAw",500,550);
  cJES2DAw->SetRightMargin(0.2);
  TProfile2D * hJESAw3D_pyx = (TProfile2D*)hJESAw3D->Project3DProfile("yx");
  hJESAw3D_pyx->SetMinimum(0.8);
  hJESAw3D_pyx->SetMaximum(1.2);
  hJESAw3D_pyx->Draw("colz");
  cJES2DAw->Print(Form("JES2DAw_%.0fto%.0f.gif",centMin,centMax));
  cJES2DAw->Print(Form("JES2DAw_%.0fto%.0f.pdf",centMin,centMax));


  fout->Write();

  return dj;
}
