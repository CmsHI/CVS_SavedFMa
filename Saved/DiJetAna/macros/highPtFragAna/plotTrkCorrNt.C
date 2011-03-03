#include <iostream>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include "TProfile.h"
#include <TCanvas.h>
#include <TLine.h>
#include "TLegend.h"
#include "TPad.h"
#include "HisMath.C"
using namespace std;

#define PI 3.14159265358979

void plotTrkCorrNt(
    TString infrec="histsntFragLoop_All5.root"
    )
{
  //TH1::SetDefaultSumw2();
  TChain * trec = new TChain("ntCorr");
  trec->Add(infrec);
  cout << infrec << ": " << trec->GetEntries() << endl;
   
  //const Int_t numPPtBins=19;
  //Float_t pptBins[numPPtBins+1] = {0.0,0.2,1,2,3,4,6,8,10,14,18,22,26,30,40,50,60,70,80,100};
  const Int_t numPtHatBins=5;
  Double_t ptHatBins[numPtHatBins+1] = {30,50,80,110,170,200};
  const Int_t numEffBins = 40;
  Double_t effBins[numEffBins+1];
  for (int i=0;i<numEffBins+1;i++)   { effBins[i] = 1/((Double_t)numEffBins)*i; }

  TCut evtCut="cent<30";
  vector<TCut> jetCut;
  jetCut.push_back("jtpt1>30&&jtpt1<250&&abs(jteta1)<0.8");
  jetCut.push_back("jtpt2>30&&jtpt2<250&&abs(jteta2)<0.8");
  vector<TCut> jetTrkCut;
  //jetTrkCut.push_back("pj1dr<0.5");
  //jetTrkCut.push_back("pj2dr<0.5");
  jetTrkCut.push_back("ppt>16&&abs(peta)<1");
  jetTrkCut.push_back("ppt>16&&abs(peta)<1");

  TString centLeg("0-30%");
  TString trkPtLeg("Trk p_{T}>16GeV");
  TString trkEtaLeg("Trk |#eta|<1");
  for (Int_t j=0; j<jetCut.size(); ++j) {
    cout << "Jet" << j << ": " << TString(evtCut&&jetCut[j]&&jetTrkCut[j]) << endl;
  }

  vector<vector<TH2D*> > vhTrkCorrPtHat(4);
  TString levelVar[4] = { "eff","fak" };
  TString levelName[4] = { "Eff","Fak","Mul","Sec" };
  for (Int_t lv=0; lv<2; ++lv) {
    for (Int_t j=0; j<jetCut.size(); ++j) {
      vhTrkCorrPtHat[lv].push_back(new TH2D(Form("h%sPtHat_j%d",levelName[lv].Data(),j),Form(";#hat{p}_{T} (GeV/c); Trk %s Corr.",levelName[lv].Data()),numPtHatBins,ptHatBins,numEffBins,effBins));
    }
  }

  for (Int_t lv=0; lv<2; ++lv) {
    for (Int_t j=0; j<jetCut.size(); ++j) {
      vhTrkCorrPtHat[lv][j]->Sumw2();
      trec->Project(vhTrkCorrPtHat[lv][j]->GetName(),levelVar[lv]+":pthatmin",(evtCut&&jetCut[j]&&jetTrkCut[j]));
    }
  }

  TH1D * hFrame = (TH1D*)vhTrkCorrPtHat[0][0]->ProjectionX("hFrame");
  hFrame->Reset();
  TCanvas *c2 = new TCanvas("c2","trkcorr",500,500);
  hFrame->SetAxisRange(0,200,"X");
  hFrame->SetAxisRange(0.4,0.8,"Y");
  hFrame->SetXTitle("#hat{p}_{T} min of sample (GeV/c)");
  hFrame->SetYTitle("<Eff>");
  hFrame->DrawCopy();
  vhTrkCorrPtHat[0][0]->ProfileX()->Draw("sameE");

  TLegend *leg = new TLegend(0.61,0.75,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hFrame,centLeg,"");
  leg->AddEntry(hFrame,trkPtLeg,"");
  leg->AddEntry(hFrame,trkEtaLeg,"");
  leg->Draw();
  c2->Print("EffPtHatMin.gif");

  TCanvas *c3 = new TCanvas("c3","trkcorr",500,500);
  hFrame->SetAxisRange(0,0.1,"Y");
  hFrame->SetYTitle("<Fake Rate>");
  hFrame->DrawCopy();
  vhTrkCorrPtHat[1][0]->ProfileX()->Draw("sameE");
  leg->Draw();
  c3->Print("FakPtHatMin.gif");

  // ====================
  //TLine *l = new TLine(1,1,100,1);
  //l->Draw();

}
