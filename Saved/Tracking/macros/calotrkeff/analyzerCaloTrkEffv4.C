#include "TTree.h"
#include "TString.h"
#include <TFile.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TRandom.h>

#include <TNtuple.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
using namespace std;

#define PI 3.14159

static const int MAXTRK = 10000;

double deltaPhi(double phi1, double phi2) {
  double dePhi = fabs(phi1 - phi2);
  if (dePhi > 3.1415926) dePhi = 2 * 3.1415926 - dePhi;
  return dePhi;
}

double deltaR(double eta1, double phi1, double eta2, double phi2) {
  double deta = eta1-eta2;
  double dphi = deltaPhi(phi1,phi2);
  return sqrt(deta*deta+dphi*dphi);
}

class Particles{
  public:
    void set(int i, float a, float b, float c){
      pt[i] = a;
      eta[i] = b;
      phi[i] = c;
    }
    void swap(int i, int j) {
      float temppt=pt[i], tempeta=eta[i], tempphi=phi[i];
      int tempkey=key[i];
      pt[i]=pt[j]; eta[i]=eta[j]; phi[i]=phi[j]; key[i]=key[j];
      pt[j]=temppt; eta[j]=tempeta; phi[j]=tempphi; key[j]=tempkey;
    }
    void sortPt()
    {
      // first save key before sorting
      for (int k=0; k<n; ++k) { key[k]=k; }
      //cout << "before sort pt." << endl; for (int k=0; k<n; ++k) cout << k << " pt: " << pt[k] << " key: " << key[k] << endl;
      // now sort
      int i, j;
      float temp;
      for (i=0; i< (n -1); ++i) {
	for(j = (i+1); j < n; ++j) {
	  if (pt[i] < pt[j]) swap(i,j);
	}
      }
      //cout << "finished sort pt." << endl; for (int k=0; k<n; ++k) cout << k << " pt: " << pt[k] << " key: " << key[k] << endl;
    }
    int n;
    float pt[MAXTRK];
    float eta[MAXTRK];
    float phi[MAXTRK];
    int key[MAXTRK];
};

class Match {
  public:
    int key[MAXTRK];
    int ncand[MAXTRK];
    float pt[MAXTRK];
    float dr[MAXTRK];
    bool fake[MAXTRK];
};

int FindMatch(Match & mat, int imat, Particles & p1, int i, Particles & p2, float cone, float ptmin, float ptmax)
{
  int ibest=-1;
  float bestpt=0;
  mat.ncand[imat]=0;
  //cout << "find match for imat: " << imat << " cand: " << i << " cand pt|eta|phi: " << p1.pt[i] << "|" << p1.eta[i] << "|" << p1.phi[i] << endl;
  for (int j=0; j<p2.n; ++j) {
    if (p2.pt[j]<ptmin||p2.pt[j]>ptmax) continue;
    // find dr to the partner element
    float dr = deltaR(p1.eta[i],p1.phi[i],p2.eta[j],p2.phi[j]);
    if (dr>cone) continue;
    ++mat.ncand[imat];
    // check if partner already used
    if (imat>0) {
      bool alreadyUsed=false;
      for (int m=0; m<imat; ++m) {
	//cout << "difference to previous pt " << m << " " << fabs(p2.pt[j]-matpt[m]) << endl;
	if (fabs(p2.pt[j]-mat.pt[m])<0.01) { alreadyUsed=true; break; }
      }
      if (alreadyUsed) continue;
    }
    // now find the highest pt partner in cone
    //cout << "partner " << j << " pt: " << p2.pt[j] << endl;
    if (p2.pt[j]>bestpt) {
      bestpt=p2.pt[j];
      ibest=j;
    }
  }
  return ibest;
}

void FindMatchPtDr(Match & mat, int itry, Particles & p1, int i, Particles & p2, float cone, float ptmin, float ptmax)
{
  mat.pt[itry] = 0; mat.dr[itry] = -1;
  //cout << "start match for p1: " << i << " pt: " << p1.pt[i] << endl;
  int ibest = FindMatch(mat,itry,p1,i,p2,cone,ptmin,ptmax);
  //cout << "ibest: " << ibest << " bestpt: " << p2.pt[ibest] << endl;
  mat.key[itry] = ibest;  
  if (ibest>=0) {
    mat.pt[itry] = p2.pt[ibest];
    mat.dr[itry] = deltaR(p1.eta[i],p1.phi[i],p2.eta[ibest],p2.phi[ibest]);
  }
}

class JetTrk{
  public:
    JetTrk() :
      pt1(0),eta1(0),phi1(0),
      pt2(0),eta2(0),phi2(0),
      deta(-99),dphi(-99),
      nTrk(0)
  {}

    float pt1;
    float eta1;
    float phi1;

    float pt2;
    float eta2;
    float phi2;

    float deta;
    float dphi;

    int nTrk;
    float trkPt[MAXTRK];
    float trkEta[MAXTRK];
    float trkPhi[MAXTRK];
    bool trkFake[MAXTRK];
    float pfSumHcal[MAXTRK];
    float pfSumEcal[MAXTRK];

    int nP;
    float pPt[MAXTRK];
    float pEta[MAXTRK];
    float pPhi[MAXTRK];
    int pPId[MAXTRK];
    int pNRec[MAXTRK];

    int nCal;
    float calPt[MAXTRK];
    float calEta[MAXTRK];
    float calPhi[MAXTRK];
};

class EvtSel {
  public:
    int run;
    int evt;
    int cbin;
    int mb;
    int jet35u;
    int jet35uc;
    int jet50u;
    int offlSel;

    bool presel;
};

void analyzerCaloTrkEffv4(TString inname="../trees/hypyuq170-393-trkeff_itertrkcalo-v1_0.root"){

  //double cutDeltaPhi = 2.*PI/3;
  double cutEtaJet = 2;
  double cutEtaTrk = 2.4;

  double cutPtJet = 60;
  double cutTrkPtMin = 10;
  double cutTrkPtMax = 200;
  double cutCalPtMin = 10;
  double cutCalPtMax = 1000;

  double matchCone=0.15;

  // Tree variables
  Particles jets;
  Particles trks;
  Particles ps;
  Particles cals;
  Match pMatCal;
  Match trkMatCal;
  Match calMatP;
  Match calMatTrk;

  bool trkFake[MAXTRK];
  float pfSumHcal[MAXTRK],pfSumEcal[MAXTRK];
  float pNRec[MAXTRK], pPId[MAXTRK];
  EvtSel evt;

  //TString calName = "tower";
  TString calName = "hbhe";
  //TString outname = Form("../ntout/trk%sanav4_dr%.0f.root",calName.Data(),matchCone*100);
  TString outname = Form("../ntout/hgt%sanav4_dr%.0f.root",calName.Data(),matchCone*100);

  TFile* inf = new TFile(inname,"read");

  TTree* t = (TTree*)inf->Get("icPu5JetAnalyzer/t");
  //TTree* ttrk = (TTree*)inf->Get("anaTrack/trackTree");
  TTree* ttrk = (TTree*)inf->Get("anaTrack_hgt/trackTree");
  TTree* tcal = (TTree*)inf->Get("rechitanalyzer/"+calName);

  t->SetBranchAddress("nref",&jets.n);
  t->SetBranchAddress("jtpt",jets.pt);
  t->SetBranchAddress("jteta",jets.eta);
  t->SetBranchAddress("jtphi",jets.phi);

  ttrk->SetBranchAddress("nTrk",&trks.n);
  ttrk->SetBranchAddress("trkPt",trks.pt);
  ttrk->SetBranchAddress("trkEta",trks.eta);
  ttrk->SetBranchAddress("trkPhi",trks.phi);
  ttrk->SetBranchAddress("trkFake",trkFake);
  ttrk->SetBranchAddress("pfSumHcal",pfSumHcal);
  ttrk->SetBranchAddress("pfSumEcal",pfSumEcal);

  ttrk->SetBranchAddress("nParticle",&ps.n);
  ttrk->SetBranchAddress("pPt",ps.pt);
  ttrk->SetBranchAddress("pEta",ps.eta);
  ttrk->SetBranchAddress("pPhi",ps.phi);
  ttrk->SetBranchAddress("pPId",pPId);
  ttrk->SetBranchAddress("pNRec",pNRec);

  tcal->SetBranchAddress("n",&cals.n);
  tcal->SetBranchAddress("et",cals.pt);
  tcal->SetBranchAddress("eta",cals.eta);
  tcal->SetBranchAddress("phi",cals.phi);

  t->SetBranchAddress("run",&evt.run);
  t->SetBranchAddress("evt",&evt.evt);
  t->SetBranchAddress("bin",&evt.cbin);
  t->SetBranchAddress("HLT_HIMinBiasHfOrBSC",&evt.mb);
  t->SetBranchAddress("HLT_HIJet35U",&evt.jet35u);
  t->SetBranchAddress("HLT_HIJet35U_Core",&evt.jet35uc);
  t->SetBranchAddress("HLT_HIJet50U",&evt.jet50u);

  TFile* outf = new TFile(outname,"recreate");

  JetTrk dj;
  TTree * tjt = new TTree("tjt","jet and trk");
  tjt->Branch("evt",&evt.run,"run/I:evt:cbin:mb:jet35u:jet35uc:jet50u:offSel");
  tjt->Branch("jet",&dj.pt1,"pt1/F:eta1:phi1:pt2:eta2:phi2:deta:dphi");

  tjt->Branch("nTrk",&dj.nTrk,"nTrk/I");
  tjt->Branch("trkPt",dj.trkPt,"trkPt[nTrk]/F");
  tjt->Branch("trkEta",dj.trkEta,"trkEta[nTrk]/F");
  tjt->Branch("trkPhi",dj.trkPhi,"trkPhi[nTrk]/F");
  tjt->Branch("trkFake",dj.trkFake,"trkFake[nTrk]/O");
  tjt->Branch("pfSumHcal",dj.pfSumHcal,"pfSumHcal[nTrk]/F");
  tjt->Branch("pfSumEcal",dj.pfSumEcal,"pfSumEcal[nTrk]/F");
  tjt->Branch("trkMatCalPt",trkMatCal.pt,"trkMatCalPt[nTrk]/F");
  tjt->Branch("trkMatCalDr",trkMatCal.dr,"trkMatCalDr[nTrk]/F");

  tjt->Branch("nP",&dj.nP,"nP/I");
  tjt->Branch("pPt",dj.pPt,"pPt[nP]/F");
  tjt->Branch("pEta",dj.pEta,"pEta[nP]/F");
  tjt->Branch("pPhi",dj.pPhi,"pPhi[nP]/F");
  tjt->Branch("pPId",dj.pPId,"pPId[nP]/I");
  tjt->Branch("pNRec",dj.pNRec,"pNRec[nP]/I");
  tjt->Branch("pMatCalPt",pMatCal.pt,"pMatCalPt[nP]/F");
  tjt->Branch("pMatCalDr",pMatCal.dr,"pMatCalDr[nP]/F");

  tjt->Branch("nCal",&dj.nCal,"nCal/I");
  tjt->Branch("calPt",dj.calPt,"calPt[nCal]/F");
  tjt->Branch("calEta",dj.calEta,"calEta[nCal]/F");
  tjt->Branch("calPhi",dj.calPhi,"calPhi[nCal]/F");
  tjt->Branch("calMatNTrk",calMatTrk.ncand,"calMatNTrk[nCal]/I");
  tjt->Branch("calMatTrkPt",calMatTrk.pt,"calMatTrkPt[nCal]/F");
  tjt->Branch("calMatTrkDr",calMatTrk.dr,"calMatTrkDr[nCal]/F");
  tjt->Branch("calMatTrkFake",calMatTrk.fake,"calMatTrkFake[nCal]/O");
  tjt->Branch("calMatNP",calMatP.ncand,"calMatNP[nCal]/I");
  tjt->Branch("calMatPPt",calMatP.pt,"calMatPPt[nCal]/F");
  tjt->Branch("calMatPDr",calMatP.dr,"calMatPDr[nCal]/F");
  //tjt->Branch("calMatPPId",dj.calMatPPId,"calMatPPId[nCal]/I");

  int Nevents = t->GetEntries();
  for(int iev = 0; iev < Nevents; ++iev){
    t->GetEntry(iev);
    ttrk->GetEntry(iev);
    tcal->GetEntry(iev);
    if (iev%2000==0) cout << iev << " (" << (float)iev/Nevents*100. << "%)" << " Run: " << evt.run << " evt: " << evt.evt << " cbin: " << evt.cbin << " sel: " << (evt.mb>0&&evt.offlSel>0) << " njets: " << jets.n << " ntrks: " << trks.n << " ncal: " << cals.n << endl;

    // event presel
    evt.presel = 1;
    if (!evt.presel) continue;

    // sort
    //cout << "sort jet:" << endl;
    jets.sortPt();
    //cout << "sort cal:" << endl;
    cals.sortPt();
    //cout << "sort trk:" << endl;
    trks.sortPt();
    //cout << "sort particles:" << endl;
    ps.sortPt();

    dj.pt1=0; dj.eta1=0; dj.phi1=0;
    for(int i = 0; i < jets.n; ++i){
      if(jets.pt[i] < cutPtJet) continue;
      if(fabs(jets.eta[i]) > cutEtaJet) continue;
      if(jets.pt[i] > dj.pt1){
	dj.pt1 = jets.pt[i];
	dj.eta1 = jets.eta[i];
	dj.phi1 = jets.phi[i];
      }
    }

    dj.nTrk=0;
    for(int i = 0; i < trks.n; ++i){
      if(trks.pt[i] < cutTrkPtMin || trks.pt[i] > cutTrkPtMax) continue;
      if(fabs(trks.eta[i]) > cutEtaTrk) continue;
      dj.trkPt[dj.nTrk]=trks.pt[i];
      dj.trkEta[dj.nTrk]=trks.eta[i];
      dj.trkPhi[dj.nTrk]=trks.phi[i];
      dj.trkFake[dj.nTrk]=trkFake[trks.key[i]];
      //cout << "trk " << i << " pt: " << trks.pt[i] << " key: " << trks.key[i] << " pfhcal: " << pfSumHcal[trks.key[i]] << endl;
      dj.pfSumHcal[dj.nTrk]=pfSumHcal[trks.key[i]];
      dj.pfSumEcal[dj.nTrk]=pfSumEcal[trks.key[i]];
      FindMatchPtDr(trkMatCal,dj.nTrk,trks,i,cals,matchCone,cutCalPtMin,cutCalPtMax);
      ++dj.nTrk;
    }

    dj.nP=0;
    for(int i = 0; i < ps.n; ++i){
      if(ps.pt[i] < cutTrkPtMin || ps.pt[i]>cutTrkPtMax) continue;
      if(fabs(ps.eta[i]) > cutEtaTrk) continue;
      dj.pPt[dj.nP]=ps.pt[i];
      dj.pEta[dj.nP]=ps.eta[i];
      dj.pPhi[dj.nP]=ps.phi[i];
      dj.pPId[dj.nP]=pPId[ps.key[i]];
      dj.pNRec[dj.nP]=pNRec[ps.key[i]];
      FindMatchPtDr(pMatCal,dj.nP,ps,i,cals,matchCone,cutCalPtMin,cutCalPtMax);
      ++dj.nP;
    }

    dj.nCal=0;
    for(int i = 0; i < cals.n; ++i){
      if(cals.pt[i] < cutCalPtMin || cals.pt[i]>cutCalPtMax) continue;
      if(fabs(cals.eta[i]) > cutEtaTrk) continue;
      dj.calPt[dj.nCal]=cals.pt[i];
      dj.calEta[dj.nCal]=cals.eta[i];
      dj.calPhi[dj.nCal]=cals.phi[i];
      FindMatchPtDr(calMatTrk,dj.nCal,cals,i,trks,matchCone,cutTrkPtMin,cutTrkPtMax);
      calMatTrk.fake[dj.nCal] = trkFake[trks.key[calMatTrk.key[dj.nCal]]];
      FindMatchPtDr(calMatP,dj.nCal,cals,i,ps,matchCone,cutTrkPtMin,cutTrkPtMax);
      //cout << "fill cal and p: " << dj.nCal << " " << dj.calMatPPt[dj.nCal] << endl;
      ++dj.nCal;
    }
    tjt->Fill();
  }

  outf->Write();	
}


