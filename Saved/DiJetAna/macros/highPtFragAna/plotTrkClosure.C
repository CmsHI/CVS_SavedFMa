#include <iostream>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>
#include "TLegend.h"
#include "TPad.h"
#include "HisMath.C"
using namespace std;

#define PI 3.14159265358979

void setupAlias(TTree* t)
{   
   t->SetAlias("trkwt","(1-trkfak)*(1-trksec)/(trkeff*(1+trkmul))");
}

void plotTrkClosure(
    TString infrec="nt_djhp_HyUQ110v0_djcalo_100_50_offset0.root",
    TString infgen="nt_djhp_HyUQ110v0_djcalo_genp_100_50_offset0.root",
    TCut evtCut="cent<30")
{
  //TH1::SetDefaultSumw2();
  TChain * trec = new TChain("tjttrk");
  trec->Add(infrec);
  setupAlias(trec);
  TChain * tgen = new TChain("tjttrk");
  tgen->Add(infgen);
  cout << infrec << " cut " << TString(evtCut) << ": " << trec->GetEntries() << endl;
  cout << infgen << " cut " << TString(evtCut) << ": " << tgen->GetEntries() << endl;
   
  const Int_t numPPtBins=19;
  Float_t pptBins[numPPtBins+1] = {0.0,0.2,1,2,3,4,6,8,10,14,18,22,26,30,40,50,60,70,80,100};

  vector<TCut> jetCut;
  jetCut.push_back("jtpt[0]>100&&jtpt[0]<200&&abs(jteta[0])<0.8");
  jetCut.push_back("jtpt[1]>50&&jtpt[1]<200&&abs(jteta[1])<0.8");
  vector<TCut> jetTrkCut;
  vector<TCut> jetGenPCut;
  jetGenPCut.push_back("pdr[0]<0.5");
  jetTrkCut.push_back(jetGenPCut[0]&&"trkeff>0&&trkeff<1");
  jetGenPCut.push_back("pdr[1]<0.5");
  jetTrkCut.push_back(jetGenPCut[1]&&"trkeff>0&&trkeff<1");

  TFile * outf = new TFile("closureHists.root","RECREATE");
  vector<Float_t> numJetRec;
  vector<Float_t> numJetGen;
  vector<TH1D*> vhPPtRecCorr;
  vector<TH1D*> vhPPtRecRaw;
  vector<TH1D*> vhPPtGen;
  vector<TH1D*> vhPPtRat;
  for (Int_t i=0; i<jetCut.size(); ++i) {
    numJetRec.push_back(trec->GetEntries(jetCut[i]&&evtCut));
    numJetGen.push_back(tgen->GetEntries(jetCut[i]&&evtCut));
    cout << "numJ" << i << " rec: " << numJetRec[i] << endl;
    cout << "numJ" << i << " gen: " << numJetGen[i] << endl;
    vhPPtRecCorr.push_back(new TH1D(Form("hPPtRecCorr_j%d",i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));
    vhPPtRecRaw.push_back(new TH1D(Form("hPPtRecRaw_j%d",i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));
    vhPPtGen.push_back(new TH1D(Form("hPPtGen_j%d",i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));
    vhPPtRat.push_back(new TH1D(Form("hPPtRat_j%d",i),";In-Cone Trk p_{T} (GeV/c); RecTrk/GenParticle",numPPtBins,pptBins));
  }

  for (Int_t i=0; i<jetCut.size(); ++i) {
    vhPPtRecCorr[i]->Sumw2();
    trec->Project(vhPPtRecCorr[i]->GetName(),"ppt",(evtCut&&jetCut[i]&&jetTrkCut[i])*"trkwt");
    normHist(vhPPtRecCorr[i],0,true,1./numJetRec[i]);

    vhPPtRecRaw[i]->Sumw2();
    trec->Project(vhPPtRecRaw[i]->GetName(),"ppt",(evtCut&&jetCut[i]&&jetTrkCut[i]));
    normHist(vhPPtRecRaw[i],0,true,1./numJetRec[i]);

    vhPPtGen[i]->Sumw2();
    tgen->Project(vhPPtGen[i]->GetName(),"ppt",(evtCut&&jetCut[i]&&jetGenPCut[i]));
    normHist(vhPPtGen[i],0,true,1./numJetGen[i]);

    vhPPtRat[i]->Divide(vhPPtRecCorr[i],vhPPtGen[i]);
  }

  for (Int_t i=0; i<jetCut.size(); ++i) {
    vhPPtGen[i]->SetAxisRange(1,100,"X");
    vhPPtGen[i]->SetAxisRange(1e-3,1e1,"Y");
    vhPPtGen[i]->SetLineColor(kRed);
    vhPPtGen[i]->SetMarkerColor(kRed);
    vhPPtGen[i]->SetMarkerStyle(kOpenCircle);
  }
  TCanvas *c2 = new TCanvas("c2","closure",500,900);
  c2->Divide(1,2);
  c2->cd(1);
  c2->GetPad(1)->SetLogy();
  vhPPtGen[0]->Draw("hist");
  vhPPtRecCorr[0]->Draw("sameE");
  c2->cd(2);
  vhPPtRat[0]->Draw("E");

  // ====================
  TLine *l = new TLine(1,1,100,1);
  l->Draw();

  c2->cd(1);
  TLegend *leg = new TLegend(0.61,0.78,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(vhPPtGen[0],"#DeltaR(jet,trk)<0.5","");
  leg->AddEntry(vhPPtGen[0],"Gen. Particle","pl");
  leg->AddEntry(vhPPtRecCorr[0],"Trk Corr.","pl");
  leg->Draw();

  // ====================
  TCanvas * chk0 = new TCanvas("chk0","check eff",500,500);
  chk0->SetLogz();
  trec->Draw("trkeff:ppt>>hTrkEffProf(50,0,100)",evtCut&&jetCut[0]&&jetTrkCut[0],"prof");
  trec->Draw("trkeff:ppt>>hTrkEff2D(50,0,100,50,-0.2,1.2)",evtCut&&jetCut[0]&&jetTrkCut[0],"colz");
  ((TH1D*)gDirectory->Get("hTrkEffProf"))->Draw("same");

  TCanvas * chk1 = new TCanvas("chk1","check fake",500,500);
  chk1->SetLogz();
  trec->Draw("trkfak:ppt>>hTrkFakProf(50,0,100)",evtCut&&jetCut[0]&&jetTrkCut[0],"prof");
  trec->Draw("trkfak:ppt>>hTrkFak2D(50,0,100,50,-0.2,1.2)",evtCut&&jetCut[0]&&jetTrkCut[0],"colz");
  ((TH1D*)gDirectory->Get("hTrkFakProf"))->Draw("same");
}
