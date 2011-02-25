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
#include "Corrector.h"
#ifdef __MAKECINT__
#pragma link C++ class std::vector < std::vector<float> >+;   
#endif
using namespace std;

#define PI 3.14159265358979

struct JetFrag
{
  Float_t cent;
  Float_t centwt;
  vector<Float_t> * jtpt;
  vector<Float_t> * jteta;
  vector<Float_t> * jtphi;
  Float_t jdphi;
  vector<Float_t> * ppt;
  vector<Float_t> * peta;
  vector<Float_t> * pphi;
  vector<Float_t> * trkeff;
  vector<Float_t> * trkfak;
  vector<Float_t> * trkmul;
  vector<Float_t> * trksec;
  vector<vector<Float_t> > * pdr;
  vector<vector<Float_t> > * pdrbg;
  JetFrag()
  {
    jtpt = 0;
    jteta = 0;
    jtphi = 0;
    ppt = 0;
    peta = 0;
    pphi = 0;
    trkeff = 0;
    trkfak = 0;
    trkmul = 0;
    trksec = 0;
    pdr = 0;
    pdrbg = 0;
  }
};

void SetJetFragTree(TTree * t, JetFrag & jf)
{
  t->SetBranchAddress("cent",&jf.cent);
  t->SetBranchAddress("centwt",&jf.centwt);
  t->SetBranchAddress("jtpt",&jf.jtpt);
  t->SetBranchAddress("jteta",&jf.jteta);
  t->SetBranchAddress("jtphi",&jf.jtphi);
  t->SetBranchAddress("jdphi",&jf.jdphi);
  t->SetBranchAddress("ppt",&jf.ppt);
  t->SetBranchAddress("peta",&jf.peta);
  t->SetBranchAddress("pphi",&jf.pphi);
  t->SetBranchAddress("trkeff",&jf.trkeff);
  t->SetBranchAddress("trkfak",&jf.trkfak);
  t->SetBranchAddress("trkmul",&jf.trkmul);
  t->SetBranchAddress("trksec",&jf.trksec);
  t->SetBranchAddress("pdr",&jf.pdr);
  t->SetBranchAddress("pdrbg",&jf.pdrbg);
}
void setupAlias(TTree* t)
{   
   t->SetAlias("trkwt","(1-trkfak)*(1-trksec)/(trkeff*(1+trkmul))");
}

vector<Float_t> numJetRec;
vector<Float_t> numJetGen;
vector<TH1D*> vhPPtRecCorr;
vector<TH1D*> vhPPtRecRaw;
vector<vector<TH1D*> > vhPPtRecCorrLv;
vector<TH1D*> vhPPtGen;
vector<TH1D*> vhPPtRat;
vector<vector<TH1D*> > vhPPtRatLv;
vector<TH2D*> vhTrkEff;
vector<TH2D*> vhTrkFak;

void loopTrkClosure(
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
  JetFrag jttrk;
  JetFrag jtgenp;
  SetJetFragTree(trec,jttrk);
  SetJetFragTree(tgen,jtgenp);

  Corrector trkCorr;
  trkCorr.ptRebinFactor_ = 1;
  trkCorr.sampleMode_ = 0; // 0 for choosing individual sample, 1 for merge samples
  trkCorr.Init();

  const Int_t numPPtBins=19;
  Float_t pptBins[numPPtBins+1] = {0.0,0.2,1,2,3,4,6,8,10,14,18,22,26,30,40,50,60,70,80,100};

  vector<TCut> jetCut;
  jetCut.push_back("jtpt[0]>100&&jtpt[0]<200&&abs(jteta[0])<0.8");
  jetCut.push_back("jtpt[1]>=50&&jtpt[1]<200&&abs(jteta[1])<0.8");
  vector<TCut> jetTrkCut;
  vector<TCut> jetGenPCut;
  jetGenPCut.push_back("pdr[0]<0.5");
  jetTrkCut.push_back(jetGenPCut[0]&&"trkeff>0&&trkeff<1");
  jetGenPCut.push_back("pdr[1]<0.5");
  jetTrkCut.push_back(jetGenPCut[1]&&"trkeff>0&&trkeff<1");

  TFile * outf = new TFile("closureHists.root","RECREATE");
  for (Int_t i=0; i<jetCut.size(); ++i) {
    numJetRec.push_back(trec->GetEntries(jetCut[i]&&evtCut));
    numJetGen.push_back(tgen->GetEntries(jetCut[i]&&evtCut));
    cout << "numJ" << i << " rec: " << numJetRec[i] << endl;
    cout << "numJ" << i << " gen: " << numJetGen[i] << endl;
    vhPPtRecCorr.push_back(new TH1D(Form("hPPtRecCorr_j%d",i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));
    vhPPtRecCorr[i]->Sumw2();
    vhPPtRecRaw.push_back(new TH1D(Form("hPPtRecRaw_j%d",i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));

    vhPPtRecRaw[i]->Sumw2();
    vhPPtGen.push_back(new TH1D(Form("hPPtGen_j%d",i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));
    vhPPtRat.push_back(new TH1D(Form("hPPtRat_j%d",i),";In-Cone Trk p_{T} (GeV/c); RecTrk/GenParticle",numPPtBins,pptBins));
    vhPPtGen[i]->Sumw2();
    vhTrkEff.push_back(new TH2D(Form("hTrkEff_j%d",i),";In-Cone Trk p_{T} (GeV/c); Eff.",50,0,100,50,-0.2,1.2));
    vhTrkFak.push_back(new TH2D(Form("hTrkFak_j%d",i),";In-Cone Trk p_{T} (GeV/c); Fake Rate",50,0,100,50,-0.2,1.2));

    vhPPtRecCorrLv.resize(5);
    vhPPtRatLv.resize(5);
    for (Int_t lv=0; lv<=4; ++lv) {
      vhPPtRecCorrLv[lv].push_back(new TH1D(Form("hPPtRecCorrLv%d_j%d",lv,i),";In-Cone Trk p_{T} (GeV/c); #frac{1}{N_{Jet}} #frac{dN}{dp_{T}}",numPPtBins,pptBins));
      vhPPtRatLv[lv].push_back(new TH1D(Form("hPPtRatLv%d_j%d",lv,i),";In-Cone Trk p_{T} (GeV/c); ratio",numPPtBins,pptBins));
    }

  }

  // ===================================
  // loop
  // ===================================
  Int_t numEntriesRec = trec->GetEntries();
  vector<Bool_t> passJetRec;
  for (Int_t j=0; j<jetCut.size(); ++j) {
    numJetRec[j]=0;
    passJetRec.push_back(false);
  }
  for (Int_t i=0; i<numEntriesRec; ++i) {
    trec->GetEntry(i);
    //cout << jttrk.jtpt->at(0) << " " << jttrk.jteta->at(0) << "" << jttrk.jtphi->at(0) << endl;
    if (jttrk.cent>=30) continue;
    if ((*jttrk.jtpt)[0]>=100&&(*jttrk.jtpt)[0]<200&&fabs((*jttrk.jteta)[0])<0.8) {
      passJetRec[0]=true;
      ++numJetRec[0];
    }
    if ((*jttrk.jtpt)[1]>=50 &&(*jttrk.jtpt)[1]<200&&fabs((*jttrk.jteta)[1])<0.8) {
      passJetRec[1]=true;
      ++numJetRec[1];
    }
    for (Int_t ip=0; ip<jttrk.ppt->size(); ++ip) {
      Float_t trkEnergy = (*jttrk.ppt)[ip];
      Float_t trkEta = (*jttrk.peta)[ip];
      /*
      Float_t eff = (*jttrk.trkeff)[ip];
      Float_t fak = (*jttrk.trkfak)[ip];
      Float_t mul = (*jttrk.trkmul)[ip];
      Float_t sec = (*jttrk.trksec)[ip];
      */
      Double_t corr[4];
      trkCorr.GetCorr(trkEnergy,trkEta,(*jttrk.jtpt)[0],jttrk.cent,corr);
      Float_t eff = corr[0];
      Float_t fak = corr[1];
      Float_t mul = corr[2];
      Float_t sec = corr[3];
      //if ((eff<1e-5||eff>0.9999)) continue;
      //if ((eff<1e-5||eff>0.9999)&&trkEnergy>40) eff=0.5;
      if (eff<1e-5) { eff=1; }

      Float_t trkwt = (1-fak)*(1-sec)/(eff*(1+mul));
      if (trkwt<0||trkwt>20) {
	cout << trkEnergy << " " << eff << " " << fak << " " << mul << " " << sec << " " << trkwt << endl;
      }
      for (Int_t j=0; j<2; ++j) {
	//cout << (*jttrk.pdr)[j][ip] << endl;
	if (passJetRec[j]&&(*jttrk.pdr)[j][ip]<0.5) {
	  vhTrkEff[j]->Fill(trkEnergy,eff);
	  vhTrkFak[j]->Fill(trkEnergy,fak);
	  vhPPtRecCorr[j]->Fill(trkEnergy,trkwt);
	  vhPPtRecRaw[j]->Fill(trkEnergy);
	  vhPPtRecCorrLv[0][j]->Fill(trkEnergy);
	  vhPPtRecCorrLv[1][j]->Fill(trkEnergy,1./eff);
	  vhPPtRecCorrLv[2][j]->Fill(trkEnergy,(1-fak)/eff);
	  vhPPtRecCorrLv[3][j]->Fill(trkEnergy,(1-fak)/(eff*(1+mul)));
	  vhPPtRecCorrLv[4][j]->Fill(trkEnergy,trkwt);
	}
      }
    }
  }

  Int_t numEntriesGen = tgen->GetEntries();
  vector<Bool_t> passJetGen;
  for (Int_t j=0; j<jetCut.size(); ++j) {
    numJetGen[j]=0;
    passJetGen.push_back(false);
  }
  for (Int_t i=0; i<numEntriesGen; ++i) {
    tgen->GetEntry(i);
    if (jtgenp.cent>=30) continue;
    if ((*jtgenp.jtpt)[0]>=100&&(*jtgenp.jtpt)[0]<200&&fabs((*jtgenp.jteta)[0])<0.8) {
      passJetGen[0]=true;
      ++numJetGen[0];
    }
    if ((*jtgenp.jtpt)[1]>=50 &&(*jtgenp.jtpt)[1]<200&&fabs((*jtgenp.jteta)[1])<0.8) {
      passJetGen[1]=true;
      ++numJetGen[1];
    }
    for (Int_t ip=0; ip<jtgenp.ppt->size(); ++ip) {
      Float_t trkEnergy = (*jtgenp.ppt)[ip];
      for (Int_t j=0; j<2; ++j) {
	if ((*jtgenp.pdr)[j][ip]<0.5) {
	  vhPPtGen[j]->Fill(trkEnergy);
	}
      }
    }
  }

  // ===================================
  // Normalize
  // ===================================
  for (Int_t j=0; j<jetCut.size(); ++j) {
    cout << "jet " << j << " count (rec): " << numJetRec[j] << endl;
    cout << "jet " << j << " count (gen): " << numJetGen[j] << endl;

    normHist(vhPPtRecCorr[j],0,true,1./numJetRec[j]);
    normHist(vhPPtRecRaw[j],0,true,1./numJetRec[j]);
    normHist(vhPPtGen[j],0,true,1./numJetGen[j]);
    for (Int_t lv=0; lv<=4; ++lv) {
      normHist(vhPPtRecCorrLv[lv][j],0,true,1./numJetRec[j]);
    }

    vhPPtRat[j]->Divide(vhPPtRecCorr[j],vhPPtGen[j]);
    for (Int_t lv=0; lv<=4; ++lv) {
      vhPPtRatLv[lv][j]->Divide(vhPPtRecCorrLv[lv][j],vhPPtGen[j]);
    }
  }

  // ===================================
  // Plot
  // ===================================
  Int_t colors[10] = {kBlack,kGray+2,kViolet,kBlue+1,kGreen+2,kOrange+2,kMagenta,kRed};
  for (Int_t j=0; j<jetCut.size(); ++j) {
    vhPPtGen[j]->SetAxisRange(1,100,"X");
    vhPPtGen[j]->SetAxisRange(1e-3,1e1,"Y");
    vhPPtGen[j]->SetLineColor(kRed);
    vhPPtGen[j]->SetMarkerColor(kRed);
    vhPPtGen[j]->SetMarkerStyle(kOpenCircle);
    vhPPtRecRaw[j]->SetMarkerStyle(kOpenCircle);
    for (Int_t lv=0; lv<=4; ++lv) {
      vhPPtRecCorrLv[lv][j]->SetMarkerStyle(kOpenSquare);
      vhPPtRecCorrLv[lv][j]->SetMarkerColor(colors[lv]);
      vhPPtRecCorrLv[lv][j]->SetLineColor(colors[lv]);
      vhPPtRatLv[lv][j]->SetMarkerStyle(kOpenSquare);
      vhPPtRatLv[lv][j]->SetMarkerColor(colors[lv]);
      vhPPtRatLv[lv][j]->SetLineColor(colors[lv]);
    }
  }
  TCanvas *c2 = new TCanvas("c2","closure",500,900);
  c2->Divide(1,2);
  c2->cd(1);
  c2->GetPad(1)->SetLogy();
  vhPPtGen[0]->Draw("hist");
  vhPPtRecRaw[0]->Draw("sameE");
  vhPPtRecCorr[0]->Draw("sameE");
  for (Int_t lv=0; lv<=4; ++lv) {
    vhPPtRecCorrLv[lv][0]->Draw("sameE");
  }
  c2->cd(2);
  vhPPtRat[0]->Draw("E");
  vhPPtRat[0]->SetAxisRange(0,1.5,"Y");
  for (Int_t lv=0; lv<=4; ++lv) {
    vhPPtRatLv[lv][0]->Draw("sameE");
  }

  // ====================
  TLine *l = new TLine(1,1,100,1);
  l->Draw();

  TString corrLevelName[5] = { "Raw","Eff","Fake","Mul. Rec","Sec" };
  c2->cd(1);
  TLegend *leg = new TLegend(0.61,0.57,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(vhPPtGen[0],"#DeltaR(jet,trk)<0.5","");
  leg->AddEntry(vhPPtGen[0],"Gen. Particle","pl");
  for (Int_t lv=0; lv<=4; ++lv) {
    leg->AddEntry(vhPPtRecCorrLv[lv][0],"Trk Corr. "+corrLevelName[lv],"pl");
  }
  leg->AddEntry(vhPPtRecCorr[0],"Trk Corr.","pl");
  leg->Draw();

  // ====================
  TCanvas * chk0 = new TCanvas("chk0","check eff",500,500);
  chk0->SetLogz();
  vhTrkEff[0]->Draw("colz");
  vhTrkEff[0]->ProfileX()->Draw("same");

  TCanvas * chk1 = new TCanvas("chk1","check fake",500,500);
  chk1->SetLogz();
  vhTrkFak[0]->Draw("colz");
  vhTrkFak[0]->ProfileX()->Draw("same");

  return;
}
