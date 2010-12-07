#define JetFragAna_cxx
#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
using namespace std;

JetFragAna::JetFragAna(TTree *tree,TString tag,Int_t doMC) :
  cut(tag,doMC),
  anaJets_(2),
  refJets_(2)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root");
      if (!f) {
         f = new TFile("dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root");
         f->cd("dj_HCPR-GoodTrkAndPixel_CleanEvt1130.root:/djcalo");
      }
      tree = (TTree*)gDirectory->Get("djTree");

   }
   Init(tree);

   // Histograms
   // jet
   hJDPhi = new TH1D("hJDPhi","",50,0,PI);
   hJEtNr = new TH1D("hJEtNr","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hJEtAw = new TH1D("hJEtAw","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hAj = new TH1D("hAj","",20,0,1);
   hJEtaNr = new TH1D("hJEtaNr","",50,-3,3);
   hJEtaAw = new TH1D("hJEtaAw","",50,-3,3);
   hJDEta = new TH1D("hJDEta","",50,-6,6);
   hRefJEtNr = new TH1D("hRefJEtNr","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   hRefJEtAw = new TH1D("hRefJEtAw","",cut.numJEtBins,cut.hisJEtMin,cut.hisJEtMax);
   // cone
   hCNPNr = new TH1D("hCNPNr","",cut.numC5NPBins,cut.hisC5NPMin,cut.hisC5NPMax);
   hCNPBgNr = new TH1D("hCNPBgNr","",cut.numC5NPBins,cut.hisC5NPMin,cut.hisC5NPMax);
   hCNPSubNr = new TH1D("hCNPSubNr","",cut.numC5NPSubBins,cut.hisC5NPSubMin,cut.hisC5NPSubMax);
   hCNPAw = new TH1D("hCNPAw","",cut.numC5NPBins,cut.hisC5NPMin,cut.hisC5NPMax);
   hCNPBgAw = new TH1D("hCNPBgAw","",cut.numC5NPBins,cut.hisC5NPMin,cut.hisC5NPMax);
   hCNPSubAw = new TH1D("hCNPSubAw","",cut.numC5NPSubBins,cut.hisC5NPSubMin,cut.hisC5NPSubMax);
   // trk
   hPNDR = new TH1D("hPNDR","",numDRBins,dRBins);
   hPADR = new TH1D("hPADR","",numDRBins,dRBins);
   hPNDRBg = new TH1D("hPNDRBg","",numDRBins,dRBins);
   hPADRBg = new TH1D("hPADRBg","",numDRBins,dRBins);
   hPtPNDR = new TH2D("hPtPNDR","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPADR = new TH2D("hPtPADR","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPNDRBg = new TH2D("hPtPNDRBg","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPADRBg = new TH2D("hPtPADRBg","",numPtBins,ptBins,numDRBins,dRBins);

   hPNDRDens = new TH1D("hPNDRDens","",numDRBins,dRBins);
   hPADRDens = new TH1D("hPADRDens","",numDRBins,dRBins);
   hPNDRDensBg = new TH1D("hPNDRDensBg","",numDRBins,dRBins);
   hPADRDensBg = new TH1D("hPADRDensBg","",numDRBins,dRBins);
   hPtPNDRDens = new TH2D("hPtPNDRDens","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPADRDens = new TH2D("hPtPADRDens","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPNDRDensBg = new TH2D("hPtPNDRDensBg","",numPtBins,ptBins,numDRBins,dRBins);
   hPtPADRDensBg = new TH2D("hPtPADRDensBg","",numPtBins,ptBins,numDRBins,dRBins);

   hPNDPhi = new TH1D("hPNDPhi","",numDPhiBins,dPhiBins);
   hPADPhi = new TH1D("hPADPhi","",numDPhiBins,dPhiBins);
   hPNDPhiBg = new TH1D("hPNDPhiBg","",numDPhiBins,dPhiBins);
   hPADPhiBg = new TH1D("hPADPhiBg","",numDPhiBins,dPhiBins);
   hPtPNDPhi = new TH2D("hPtPNDPhi","",numPtBins,ptBins,numDPhiBins,dPhiBins);
   hPtPADPhi = new TH2D("hPtPADPhi","",numPtBins,ptBins,numDPhiBins,dPhiBins);
   hPtPNDPhiBg = new TH2D("hPtPNDPhiBg","",numPtBins,ptBins,numDPhiBins,dPhiBins);
   hPtPADPhiBg = new TH2D("hPtPADPhiBg","",numPtBins,ptBins,numDPhiBins,dPhiBins);
}

Int_t JetFragAna::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.

  // preselection
  if (cut.BaseCutType=="S0")
    if (vz<=cut.VzMin || vz>= cut.VzMax)
      return -1;

  if (cut.BaseCutType=="S1")
    if (vz<=cut.VzMin || vz>= cut.VzMax ||
       cent<cut.CentMin || cent>=cut.CentMax)
      return -1;

  if (cut.BaseCutType=="S2")
    if (vz<=cut.VzMin || vz>= cut.VzMax ||
	cent<cut.CentMin || cent>=cut.CentMax ||
	!hlt->at(2))
      return -1;

  Float_t Aj = (anaJets_[0].pt()-anaJets_[1].pt())/(anaJets_[0].pt()+anaJets_[1].pt());
  if (Aj<cut.AjMin||Aj>=cut.AjMax) return -1;

  Int_t result=-1;
  if (cut.DJCutType=="Ana") {
    if (anaJets_[0].pt()>=cut.NrJEtMin && anaJets_[0].pt()<cut.NrJEtMax && fabs(anaJets_[0].eta())<cut.NrJEtaMax &&
	anaJets_[1].pt()>=cut.AwJEtMin && anaJets_[1].pt()<cut.AwJEtMax && fabs(anaJets_[1].eta())<cut.AwJEtaMax &&
	jdphi>=cut.DjDPhiMin
       )
      result=1;
  }

  return result;
}

Int_t JetFragAna::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  Int_t result = fChain->GetEntry(entry);
  if (result<0) return result;
  else {
    anaJets_[0].SetCoordinates(nljet,nljeta,nljphi,0);
    anaJets_[1].SetCoordinates(aljet,aljeta,aljphi,0);
    particles_.clear();
    for (Int_t i=0; i<evtnp; ++i) {
      particles_.push_back(math::PtEtaPhiMLorentzVector(ppt[i],peta[i],pphi[i],0.13957));
    }
  }
  return result;
}
void JetFragAna::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L JetFragAna.C
//      Root > JetFragAna t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   numDJ_=0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = GetEntry(jentry);   nbytes += nb;

      if (Cut(ientry)>=0) {
	if (cut.BkgSubType=="EtaRefl") {
	  if (fabs(nljeta)<0.8||fabs(aljeta)<0.8) continue;
	}
	if (cut.BkgSubType=="PhiRot") {
	  if (fabs(nljeta-aljeta)<1.6) continue;
	}
	//cout << "Global Entry: " << jentry << " leading et|eta|phi: " << anaJets_[0] << " away et|eta|phi: " << anaJets_[1] << " jdphi: " << jdphi << endl;
	hJDPhi->Fill(jdphi);
	hJEtNr->Fill(anaJets_[0].pt());
	hJEtAw->Fill(anaJets_[1].pt());
	hAj->Fill((anaJets_[0].pt()-anaJets_[1].pt())/(anaJets_[0].pt()+anaJets_[1].pt()));
	hJEtaNr->Fill(anaJets_[0].eta());
	hJEtaAw->Fill(anaJets_[1].eta());
	hJDEta->Fill(anaJets_[1].eta()-anaJets_[0].eta());
	for (Int_t i=0; i<evtnp;++i) {
	  // Trk Cut
	  if (ppt[i]<cut.TrkPtMin) continue;
	  //if (particles_[i].pt()>30) cout << "particle " << i << ": " << particles_[i] << endl;
	  // Trk histograms
	  Double_t PNdRBkg=-999,PAdRBkg=-999;
	  if (cut.BkgSubType=="EtaRefl") {
	    PNdRBkg = reco::deltaR(peta[i],pphi[i],-nljeta,nljphi);
	    PAdRBkg = reco::deltaR(peta[i],pphi[i],-aljeta,aljphi);
	  }
	  if (cut.BkgSubType=="PhiRot") {
	    PNdRBkg = reco::deltaR(peta[i],pphi[i],nljeta,nljphi+PI);
	    PAdRBkg = reco::deltaR(peta[i],pphi[i],aljeta,aljphi+PI);
	  }

	  hPNDR->Fill(pndr[i],ppt[i]);
	  hPADR->Fill(padr[i],ppt[i]);
	  hPNDRDens->Fill(pndr[i],ppt[i]/(2*PI*pndr[i]));
	  hPADRDens->Fill(padr[i],ppt[i]/(2*PI*padr[i]));

	  hPNDRBg->Fill(PNdRBkg,ppt[i]);
	  hPADRBg->Fill(PAdRBkg,ppt[i]);
	  hPNDRDensBg->Fill(PNdRBkg,ppt[i]/(2*PI*PNdRBkg));
	  hPADRDensBg->Fill(PAdRBkg,ppt[i]/(2*PI*PAdRBkg));

	  hPtPNDR->Fill(ppt[i],pndr[i],ppt[i]);
	  hPtPADR->Fill(ppt[i],padr[i],ppt[i]);
	  hPtPNDRDens->Fill(ppt[i],pndr[i],ppt[i]/(2*PI*pndr[i]));
	  hPtPADRDens->Fill(ppt[i],padr[i],ppt[i]/(2*PI*padr[i]));

	  hPtPNDRBg->Fill(ppt[i],PNdRBkg,ppt[i]);
	  hPtPADRBg->Fill(ppt[i],PAdRBkg,ppt[i]);
	  hPtPNDRDensBg->Fill(ppt[i],PNdRBkg,ppt[i]/(2*PI*PNdRBkg));
	  hPtPADRDensBg->Fill(ppt[i],PAdRBkg,ppt[i]/(2*PI*PAdRBkg));
	}
	++numDJ_;
      }
      // if (Cut(ientry) < 0) continue;
   }
   cout << "DiJets: " << numDJ_ << endl;
   hJDPhi->Scale(1./(numDJ_));
   hJEtNr->Scale(1./(numDJ_));
   hJEtAw->Scale(1./(numDJ_));
   hAj->Scale(1./(numDJ_));
   hJEtaNr->Scale(1./(numDJ_));
   hJEtaAw->Scale(1./(numDJ_));
   hJDEta->Scale(1./(numDJ_));

   hPNDR->Scale(1./(numDJ_));
   hPADR->Scale(1./(numDJ_));
   hPNDRDens->Scale(1./(numDJ_));
   hPADRDens->Scale(1./(numDJ_));

   hPNDRBg->Scale(1./(numDJ_));
   hPADRBg->Scale(1./(numDJ_));
   hPNDRDensBg->Scale(1./(numDJ_));
   hPADRDensBg->Scale(1./(numDJ_));

   hPtPNDR->Scale(1./(numDJ_));
   hPtPADR->Scale(1./(numDJ_));
   hPtPNDRDens->Scale(1./(numDJ_));
   hPtPADRDens->Scale(1./(numDJ_));

   hPtPNDRBg->Scale(1./(numDJ_));
   hPtPADRBg->Scale(1./(numDJ_));
   hPtPNDRDensBg->Scale(1./(numDJ_));
   hPtPADRDensBg->Scale(1./(numDJ_));
}
