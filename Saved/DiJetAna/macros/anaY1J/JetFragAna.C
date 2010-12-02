#define JetFragAna_cxx
#include "Saved/DiJetAna/macros/anaY1J/JetFragAna.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
using namespace std;

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

  Int_t result=-1;
  if (cut.DJCutType=="Ana") {
    if (nljet>=cut.NrJEtMin && nljet<cut.NrJEtMax && fabs(nljeta)<cut.NrJEtaMax &&
	aljet>=cut.AwJEtMin && aljet<cut.AwJEtMax && fabs(aljeta)<cut.AwJEtaMax &&
	jdphi>=cut.DjDPhiMin
       )
      result=1;
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
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      anaJets_[0].SetCoordinates(nljet,nljeta,nljphi,0);
      anaJets_[1].SetCoordinates(aljet,aljeta,aljphi,0);
      if (Cut(ientry)>=0) {
	cout << "Global Entry: " << jentry
	  << " leading et|eta|phi: " << anaJets_[0]
	  << " away et|eta|phi: " << anaJets_[1]
	  << " jdphi: " << jdphi << endl;
	++numDJ_;
      }
      // if (Cut(ientry) < 0) continue;
   }
   cout << "DiJets: " << numDJ_ << endl;
}
