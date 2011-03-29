#include "GeneralJetFragAna.h"
using namespace std;

GeneralJetFragAna::GeneralJetFragAna()
  : anaJetv_(2)
{
  cout << "GeneralJetFragAna" << endl;
}

void GeneralJetFragAna::Init()
{
  // Inputs
  anaEvt_.LoadBranches(evtTree_);
  anaJets_.LoadBranches(jetTree_);
  anaPs_.LoadBranches(pTree_);

  // Outputs
  jfTree_ = new TTree("tjf","jet frag tree");
  jf_.SetBranches(jfTree_);
}

void GeneralJetFragAna::Loop()
{
  Int_t numEvtEnt = evtTree_->GetEntries();
  Int_t numJetEnt = jetTree_->GetEntries();
  Int_t numPEnt	  = pTree_->GetEntries();
  cout << "evtTree: " << numEvtEnt << endl;
  cout << "jetTree: " << numJetEnt << endl;
  cout << "pTree: " << numPEnt << endl;
  if (numEvtEnt!=numJetEnt||numJetEnt!=numPEnt) {
    cout << "Inconsistent trees" << endl;
    return;
  }

  for (Int_t ievt=0; ievt<numEvtEnt; ++ievt) {
    evtTree_->GetEntry(ievt);
    jetTree_->GetEntry(ievt);
    pTree_->GetEntry(ievt);
    if (ievt%1000==0) {
      cout << "Entry " << ievt << endl;
      //cout << "run/lumi/evt: " << anaEvt_.run << "/" << anaEvt_.lumi << "/" << anaEvt_.evt << endl;
      cout << "bin|vz: " << anaEvt_.bin << "|" << anaEvt_.vz << endl;
      cout << "njet: " << anaJets_.njets << " jtpt0: " << anaJets_.jtpt[0] << endl;
      //cout << "np: " << anaPs_.np << " ppt0: " << anaPs_.ppt[0] << endl;
    }

    // Initialize counters for each event
    jf_.clear();
    anaJetv_.clear();
    pv_.clear();

    // Event level vars
    jf_.cent = anaEvt_.bin * 2.5;

    // Jet level vars
    Int_t leadJetInd  = GetLeadingJet(anaJets_,anaJetv_);
    Int_t jet2Ind     = GetJet2(anaJets_,anaJetv_,leadJetInd);
    for (Int_t j=0;j<anaJetv_.size();++j) {
      //cout << "best pt: " << anaJetv_[j].pt() << endl;
      jf_.jtpt[j]=anaJetv_[j].pt();
      jf_.jteta[j]=anaJetv_[j].eta();
      jf_.jtphi[j]=anaJetv_[j].phi();
    }

    // Particle level vars
    for (Int_t ip=0; ip<anaPs_.np; ++ip) {
      if (anaPs_.ppt[ip]<pptMin_) continue;
      jf_.ppt[jf_.np] = anaPs_.ppt[ip];
      jf_.peta[jf_.np] = anaPs_.peta[ip];
      jf_.pphi[jf_.np] = anaPs_.pphi[ip];
      ++jf_.np;
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
    if (fabs(jets.jteta[j])>3) continue; // eta limit for leading jet
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
    if (fabs(jets.jteta[j])>3) continue; // eta limit for away jet
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
