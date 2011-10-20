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

static const int MAX = 1000;
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
	float pt[MAX];
	float eta[MAX];
	float phi[MAX];
	int n;
};


class JetTrk{
public:
	JetTrk() :
	pt1(0),eta1(0),phi1(0),
	pt2(0),eta2(0),phi2(0),
	deta(-99),dphi(-99),
	l1pt1(0),l1eta1(0),l1phi1(0),isTau(false),isFwd(false)
	{}
	
	float pt1;
	float eta1;
	float phi1;
  
	float pt2;
	float eta2;
	float phi2;
	
	float deta;
	float dphi;
  
  float l1pt1;
  float l1eta1;
  float l1phi1;
  bool isTau;
  bool isFwd;
};

class EvtSel {
public:
	int run;
	int evt;
  int cbin;
	int mb;
	int jetl1data;
	int jetl1mc;
  int njets;
  float l1ett;
};

void analyzeTrig(
                 TString inname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv1/merge/all.root"
                 )
{
	double cutEtaJet = 5;
	double cutPtJet = 0;
	
	// Tree variables
	EvtSel evt;
	Particles jets;
	Particles l1cenjets;
  Particles l1taujets;
  Particles l1forjets;

	TString outname = Form("results_alljettype_hil1mctag_eta%.0f.root",cutEtaJet*1000);
	
	TFile* inf = new TFile(inname,"read");
	
	TTree* t = (TTree*)inf->Get("hltbitnew/HltTree");
	TTree* tjet = (TTree*)inf->Get("icPu5JetAnalyzer/t");
	
	t->SetBranchAddress("Run",&evt.run);
	t->SetBranchAddress("Event",&evt.evt);
	t->SetBranchAddress("HLT_HIMinBiasHfOrBSC",&evt.mb);
	t->SetBranchAddress("L1_SingleJet30_BptxAND",&evt.jetl1data);
	t->SetBranchAddress("L1_SingleJet36_BptxAND",&evt.jetl1mc);
  t->SetBranchAddress("L1EtTot",&evt.l1ett);
	
  t->SetBranchAddress("L1CenJetEt",l1cenjets.pt);
  t->SetBranchAddress("L1CenJetEta",l1cenjets.eta);
  t->SetBranchAddress("L1CenJetPhi",l1cenjets.phi);
  t->SetBranchAddress("NL1CenJet",&l1cenjets.n);
  
  t->SetBranchAddress("L1TauEt",l1taujets.pt);
  t->SetBranchAddress("L1TauEta",l1taujets.eta);
  t->SetBranchAddress("L1TauPhi",l1taujets.phi);
  t->SetBranchAddress("NL1Tau",&l1taujets.n);
  
  t->SetBranchAddress("L1ForJetEt",l1forjets.pt);
  t->SetBranchAddress("L1ForJetEta",l1forjets.eta);
  t->SetBranchAddress("L1ForJetPhi",l1forjets.phi);
  t->SetBranchAddress("NL1ForJet",&l1forjets.n);
  
	t->SetBranchAddress("bin",&evt.cbin);
	tjet->SetBranchAddress("jtpt",jets.pt);
	tjet->SetBranchAddress("jteta",jets.eta);
	tjet->SetBranchAddress("jtphi",jets.phi);
	tjet->SetBranchAddress("nref",&jets.n);
  
  
	TFile* outf = new TFile(outname,"recreate");
	
  JetTrk dj;
  TTree * tjt = new TTree("tjt","jet and trk");
  tjt->Branch("evt",&evt.run,"run/I:evt:cbin:mb:jetl1data:jetl1mc:njets:l1ett/F");
  tjt->Branch("jet",&dj.pt1,"pt1/F:eta1:phi1:pt2:eta2:phi2:deta:dphi:l1pt1:l1eta1:l1phi1:isTau/O:isFwd");
  
	int Nevents = t->GetEntries();
  cout << "Total events in tree: " << Nevents << endl;
	for(int iev = 0; iev < Nevents; ++iev){
		t->GetEntry(iev);
		tjet->GetEntry(iev);
    evt.njets = jets.n;
		if (iev%20000==0) cout << iev << " (" << (float)iev/Nevents*100. << "%)" << " Run: " << evt.run << " evt: " << evt.evt << " njets: " << evt.njets << " l1ett: " << evt.l1ett << endl;
		
    // rec jets
    dj.pt1=0; dj.eta1=-99; dj.phi1=-99;
		for(int i = 0; i < jets.n; ++i){
			if(jets.pt[i] < cutPtJet) continue;
			if(fabs(jets.eta[i]) > cutEtaJet) continue;
			if(jets.pt[i] > dj.pt1){
				dj.pt1 = jets.pt[i];
				dj.eta1 = jets.eta[i];
				dj.phi1 = jets.phi[i];
			}
    }
    
    // l1 jets
    dj.isTau=false; dj.isFwd=false;
    dj.l1pt1=0; dj.l1eta1=-99; dj.l1phi1=-99;
    for(int i = 0; i < l1cenjets.n; ++i){
      if(l1cenjets.pt[i] < cutPtJet) continue;
      if(fabs(l1cenjets.eta[i]) > cutEtaJet) continue;
      if(l1cenjets.pt[i] > dj.l1pt1){
        dj.l1pt1 = l1cenjets.pt[i];
        dj.l1eta1 = l1cenjets.eta[i];
        dj.l1phi1 = l1cenjets.phi[i];
      }
    }
    
    for(int i = 0; i < l1taujets.n; ++i){
      if(l1taujets.pt[i] < cutPtJet) continue;
      if(fabs(l1taujets.eta[i]) > cutEtaJet) continue;
      if(l1taujets.pt[i] > dj.l1pt1){
        dj.l1pt1 = l1taujets.pt[i];
        dj.l1eta1 = l1taujets.eta[i];
        dj.l1phi1 = l1taujets.phi[i];
        dj.isTau = true;
      }
    }
    
    for(int i = 0; i < l1forjets.n; ++i){
      if(l1forjets.pt[i] < cutPtJet) continue;
      if(fabs(l1forjets.eta[i]) > cutEtaJet) continue;
      if(l1forjets.pt[i] > dj.l1pt1){
        dj.l1pt1 = l1forjets.pt[i];
        dj.l1eta1 = l1forjets.eta[i];
        dj.l1phi1 = l1forjets.phi[i];
        dj.isFwd = true;
      }
    }

    tjt->Fill();
  }
  outf->Write();	
}


