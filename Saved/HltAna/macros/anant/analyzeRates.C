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

class EvtSel {
public:
	int run;
	int evt;
};

class Trigger {
public:
   TString name;
   int result;
   bool resultPS;
   int prescale;
   int nacc;
   int naccPS;
   Trigger(TString s, int ps=1) : name(s),prescale(ps),nacc(0),naccPS(0) {}
   void EvalTrig() {
      resultPS=false;
      if (result>0) {
         ++nacc;
         if ( (nacc%prescale)==0 ) {
            resultPS = true;
            ++naccPS;
         }
         //cout << "nacc: " << nacc << " nacc mod pres: " << (nacc%prescale) << " naccPS: " << naccPS << endl;
      }
   }
   void PrintSummary(float norm) {
      cout << name << ". BeforePS: " << nacc << " (" << nacc/norm << ")";
      cout << " AfterPS: " << naccPS << " (" << naccPS/norm << ")" << endl;
   }
};

void LoadTrigBranch(TTree * t, Trigger & trig)
{
   t->SetBranchAddress(trig.name,&trig.result);
}

void analyzeRates(
                  //TString inname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv2/merge/all.root"
                  // /net/hisrv0001/home/dav2105/run/CMSSW_4_4_2/src/HLTrigger/Configuration/python/HLT_HIon_mu_PRESCALE.py:
                  TString inname="/net/hidsk0001/d00/scratch/dav2105/HIHLT_Validation_Test_GRIF_v10.root" // with unprescaled, b/c it's taking 1st prescale column! 
                  )
{
	// Tree variables
	EvtSel evt;
   vector<Trigger> vtrig;
	TFile* inf = new TFile(inname,"read");
	
	TTree* t = (TTree*)inf->Get("hltbitnew/HltTree");
   
   // mb
   vtrig.push_back(Trigger("HLT_HIMinBiasHfOrBSC_v1",191));
   vtrig.push_back(Trigger("HLT_HIMinBiasZDC_Calo_v1",3343));
   // jets
   vtrig.push_back(Trigger("HLT_HIJet55_v1",10));
   vtrig.push_back(Trigger("HLT_HIJet65_v1",3));
   vtrig.push_back(Trigger("HLT_HIJet80_v1",1));
   vtrig.push_back(Trigger("HLT_HIJet95_v1",1));
   vtrig.push_back(Trigger("HLT_HIDiJet55_v1",2));
   vtrig.push_back(Trigger("HLT_HIJet65_Jet55_v1",1));
   // photons
   vtrig.push_back(Trigger("HLT_HISinglePhoton15_v1",6));
   vtrig.push_back(Trigger("HLT_HISinglePhoton20_v1",1));
   vtrig.push_back(Trigger("HLT_HISinglePhoton30_v1",1));
   vtrig.push_back(Trigger("HLT_HISinglePhoton40_v1",1));
   vtrig.push_back(Trigger("HLT_HIDoublePhoton10_v1",30));
   vtrig.push_back(Trigger("HLT_HIDoublePhoton15_v1",1));
   vtrig.push_back(Trigger("HLT_HIDoublePhoton20_v1",1));
   vtrig.push_back(Trigger("HLT_HIPhoton10_Photon15_v1",1));
   vtrig.push_back(Trigger("HLT_HIPhoton15_Photon20_v1",1));
   // single track
   vtrig.push_back(Trigger("HLT_HIFullTrack12_L1Central_v1",8));
   vtrig.push_back(Trigger("HLT_HIFullTrack12_L1Peripheral_v1",8));
   vtrig.push_back(Trigger("HLT_HIFullTrack14_L1Central_v1",2));
   vtrig.push_back(Trigger("HLT_HIFullTrack14_L1Peripheral_v1",2));
   vtrig.push_back(Trigger("HLT_HIFullTrack20_L1Central_v1",1));
   vtrig.push_back(Trigger("HLT_HIFullTrack20_L1Peripheral_v1",1));
   vtrig.push_back(Trigger("HLT_HIFullTrack25_L1Central_v1",1));
   vtrig.push_back(Trigger("HLT_HIFullTrack25_L1Peripheral_v1",1));
   // muons
   vtrig.push_back(Trigger("HLT_HIL1DoubleMu0_HighQ_v1",1));
   vtrig.push_back(Trigger("HLT_HIL2DoubleMu3_v1",1));
   vtrig.push_back(Trigger("HLT_HIL2Mu15_v1",1));
   vtrig.push_back(Trigger("HLT_HIL2Mu3_NHitQ_v1",10));
   vtrig.push_back(Trigger("HLT_HIL2Mu7_v1",5));
   vtrig.push_back(Trigger("HLT_HIL3DoubleMuOpen_Mgt2_OS_NoCowboy_v1",1));
   vtrig.push_back(Trigger("HLT_HIL3DoubleMuOpen_v1",7));
   vtrig.push_back(Trigger("HLT_HIL3Mu3_v1",5));

	// Load Brances
	t->SetBranchAddress("Run",&evt.run);
	t->SetBranchAddress("Event",&evt.evt);
   
   for (unsigned i=0; i<vtrig.size(); ++i) {
      cout << vtrig[i].name << ": prescale = " << vtrig[i].prescale << endl;
      LoadTrigBranch(t,vtrig[i]);
   }
   
   int culmuNAcc=0;
   int culmuNAccPS=0;
	int Nevents = t->GetEntries();
   float nMB=t->GetEntries("HLT_HIMinBiasHfOrBSC_v1");
   cout << "Total events in tree: " << Nevents << endl;
   cout << "Total MB events in tree: " << nMB << endl;
   // Loop Events
	for(int iev = 0; iev < Nevents; ++iev){
		t->GetEntry(iev);
		if (iev%50000==0) { cout << iev << " (" << (float)iev/Nevents*100. << "%)" << " Run: " << evt.run << " evt: " << evt.evt << endl; }

      bool accEvt=false;
      bool accEvtPS=false;
      for (unsigned i=0; i<vtrig.size(); ++i) {
         // 1. evaluate trigger results after trigger rules
         vtrig[i].EvalTrig();
         if (vtrig[i].result>0) accEvt=true;
         if (vtrig[i].resultPS) accEvtPS=true;
      }
      // 2. calc cumulative rate after trigger rules
      if (accEvt) ++culmuNAcc;
      if (accEvtPS) ++culmuNAccPS;
   }
   
   // Finished Looping
   cout << "Final Results" << endl;
   for (unsigned i=0; i<vtrig.size(); ++i) {
      vtrig[i].PrintSummary(nMB);
   }   
   cout << "Cumulative Rates" << endl;
   cout << "noPS: " << culmuNAcc << " frac of MB: " << culmuNAcc/nMB << endl;
   cout << "PS:" << culmuNAccPS << " frac of MB: " << culmuNAccPS/nMB << endl;
}


