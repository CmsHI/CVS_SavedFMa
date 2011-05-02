#include "GeneralJetFragAna.h"
using namespace std;

GeneralJetFragAna::GeneralJetFragAna(TString name) :
  name_(name),
  doMC_(false),
  doJetOnly_(false),
  anaJetv_(2),
  treeFormat_(0),
  jetType_(2),
  pType_(2)
{
}

void GeneralJetFragAna::Init(Int_t jType, Int_t pType)
{
  jetType_=jType;
  pType_=pType;
  cout << "============================" << endl;
  cout << "GeneralJetFragAna: " << name_ << " doMC: " << doMC_ << " doJEC: " << doJEC_ << endl;
  cout << "leadJetPtMin: " << leadJetPtMin_ << " pptMin: " << pptMin_ << endl;
  cout << "treeFormat: " << treeFormat_ << " jetType: " << jType << " particleType: " << pType << endl;
  cout << "============================" << endl;
  // Inputs
  anaEvt_.LoadBranches(evtTree_,treeFormat_);
  anaJets_.LoadBranches(jetTree_,treeFormat_,name_,jType,doMC_);
  if (!doJetOnly_) anaPs_.LoadBranches(pTree_,treeFormat_,pType);

  // Outputs
  jfTree_ = new TTree("tjf","jet frag tree");
  jf_.SetBranches(jfTree_);
  jfTree_->SetAlias("Aj","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])");
  jfTree_->SetAlias("jdphi","acos(cos(jtphi[0]-jtphi[1]))");
}

void GeneralJetFragAna::Loop()
{
  Int_t numEvtEnt = evtTree_->GetEntries();
  Int_t numJetEnt = jetTree_->GetEntries();
  Int_t numPEnt = 0;
  if (!doJetOnly_) numPEnt = pTree_->GetEntries();
  cout << "evtTree: " << numEvtEnt << endl;
  cout << "jetTree: " << numJetEnt << endl;
  if (!doJetOnly_) {
    cout << "pTree: " << numPEnt << endl;
    if (numEvtEnt!=numJetEnt||numJetEnt!=numPEnt) {
      cout << "Inconsistent trees" << endl;
      return;
    }
  }

  //numEvtEnt=100;
  for (Int_t ievt=0; ievt<numEvtEnt; ++ievt) {
    evtTree_->GetEntry(ievt);
    jetTree_->GetEntry(ievt);
    if (!doJetOnly_) pTree_->GetEntry(ievt);
    if (ievt%5000==0) {
      cout << "Entry " << ievt << " (" << (Float_t)ievt/numEvtEnt*100 << "%)" << endl;
      //cout << "run/lumi/evt: " << anaEvt_.run << "/" << anaEvt_.lumi << "/" << anaEvt_.evt << endl;
      cout << "bin|vz: " << anaEvt_.cbin << "|" << anaEvt_.vz
	<< " njet: " << anaJets_.njets << " jtpt0: " << anaJets_.jtpt[0] << " refjet: " << anaJets_.refpt[0]
	<< " trk pid|pt|eta|phi|nhits: " << anaPs_.ppid[0] << "|" << anaPs_.ppt[0] << "|" << anaPs_.peta[0] << "|" << anaPs_.pphi[0] << "|" << anaPs_.trkNHits[0] << endl;
    }

    // JEC
    if (doJEC_==1) {
      for (Int_t j=0; j<anaJets_.njets; ++j) {
	anaJets_.jtpt[j]=anaJets_.rawpt[j]*anajec_->GetJEC(anaJets_.rawpt[j],anaJets_.jteta[j]);
      }
    }

    // Initialize counters for each event
    jf_.clear();
    anaJetv_.clear();
    pv_.clear();

    // Event level vars
    jf_.cent = anaEvt_.cbin * 2.5;
    jf_.cbin = anaEvt_.cbin;
    jf_.vz = anaEvt_.vz;

    // Jet level vars
    Int_t leadJetInd  = GetLeadingJet(anaJets_,anaJetv_);
    if (leadJetPtMin_>0 && anaJets_.jtpt[leadJetInd]<leadJetPtMin_) continue; // leading jet selection
    Int_t jet2Ind     = GetJet2(anaJets_,anaJetv_,leadJetInd);
    Int_t anaJInd[2] = { leadJetInd, jet2Ind };
    for (Int_t j=0;j<anaJetv_.size();++j) {
      if (anaJInd[j]<0) continue;
      jf_.jtpt[j]=anaJets_.jtpt[anaJInd[j]];
      jf_.refpt[j]=anaJets_.refpt[anaJInd[j]];
      jf_.rawpt[j]=anaJets_.rawpt[anaJInd[j]];
      jf_.jteta[j]=anaJets_.jteta[anaJInd[j]];
      jf_.jtphi[j]=anaJets_.jtphi[anaJInd[j]];
    }
    //cout << "best pt: " << anaJets_.jtpt[leadJetInd] << " ref: " << anaJets_.refpt[leadJetInd] << endl;

    // Particle level vars
    if (!doJetOnly_) {
      for (Int_t ip=0; ip<anaPs_.np; ++ip) {
	if (anaPs_.ppt[ip]<pptMin_) continue;
	if (fabs(anaPs_.peta[ip])>2.4) continue; // tracker acceptance
	if (anaPs_.trkNHits[ip]<5) continue; // full tracks only for the moment
	if (treeFormat_==1 && pType_==0 &&
	    !(abs(anaPs_.ppid[ip])==11 // e- 0.46%
	      || abs(anaPs_.ppid[ip])==13 // mu- 0.12%
	      || abs(anaPs_.ppid[ip])==211 // pi+ 77.46%
	      || abs(anaPs_.ppid[ip])==321 // k+ 12.75%
	      || abs(anaPs_.ppid[ip])==2112 // p+ 7.81%
	      || abs(anaPs_.ppid[ip])==3112 // Sigma- 0.59%
	      || abs(anaPs_.ppid[ip])==3222 // Sigma+ 0.59%
	      || abs(anaPs_.ppid[ip])==3312 // Xi- 0.21%
	      || abs(anaPs_.ppid[ip])==3334 // Omega- 0.005%
	      )
	   ) continue;
	jf_.ppt[jf_.np] = anaPs_.ppt[ip];
	jf_.peta[jf_.np] = anaPs_.peta[ip];
	jf_.pphi[jf_.np] = anaPs_.pphi[ip];
	jf_.pfid[jf_.np] = anaPs_.pfid[ip];
	++jf_.np;
      }
    }

    // All vars calculated
    jfTree_->Fill();
  }
}

Int_t GeneralJetFragAna::GetLeadingJet(AnaJets & jets, std::vector<PtEtaPhiMLorentzVectorD> & jv)
{
  Float_t maxpt=-99;
  Int_t bestInd=-99;
  for (Int_t j=0; j<jets.njets; ++j) {
    if (fabs(jets.jteta[j])>2) continue; // eta limit for leading jet
    if (jets.jtpt[j]>maxpt) {
      maxpt=jets.jtpt[j];
      bestInd=j;
    }
  }
  if (bestInd>=0) {
    jv.push_back(
	PtEtaPhiMLorentzVectorD(
	  jets.jtpt[bestInd],
	  jets.jteta[bestInd],
	  jets.jtphi[bestInd],
	  0)
	);
  }
  return bestInd;
}

Int_t GeneralJetFragAna::GetJet2(AnaJets & jets, std::vector<PtEtaPhiMLorentzVectorD> & jv, Int_t leadJetInd)
{
  Float_t maxpt=-99;
  Int_t bestInd=-99;
  for (Int_t j=0; j<jets.njets; ++j) {
    if (j==leadJetInd) continue; // not leading jet
    if (fabs(jets.jteta[j])>2) continue; // eta limit for away jet
    if (jets.jtpt[j]>maxpt) {
      maxpt=jets.jtpt[j];
      bestInd=j;
    }
  }
  if (bestInd>=0) {
    jv.push_back(
	PtEtaPhiMLorentzVectorD(
	  jets.jtpt[bestInd],
	  jets.jteta[bestInd],
	  jets.jtphi[bestInd],
	  0)
	);
  }
  return bestInd;
}
