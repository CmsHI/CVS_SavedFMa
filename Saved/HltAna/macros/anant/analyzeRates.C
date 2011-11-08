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
                  TString inname="/net/hisrv0001/home/frankma/scratch01/data/HCRaw/hcraw-rerunl1hlt-masterhil1mctagv2/merge/all.root"
                  )
{
	// Tree variables
	EvtSel evt;
   vector<Trigger> vtrig;
	TFile* inf = new TFile(inname,"read");
	
	TTree* t = (TTree*)inf->Get("hltbitnew/HltTree");
   
   vtrig.push_back(Trigger("HLT_HIJet55_v1",10));
   vtrig.push_back(Trigger("HLT_HIJet65_v1",3));
   vtrig.push_back(Trigger("HLT_HIJet80_v1",1));
   vtrig.push_back(Trigger("HLT_HIJet95_v1",1));
   vtrig.push_back(Trigger("HLT_HIDiJet55_v1",2));
   vtrig.push_back(Trigger("HLT_HIJet65_Jet55_v1",1));
   
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
   cout << "Total events in tree: " << Nevents << endl;
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
   float nMB=Nevents;
   for (unsigned i=0; i<vtrig.size(); ++i) {
      vtrig[i].PrintSummary(nMB);
   }   
   cout << "Cumulative Rates" << endl;
   cout << "noPS: " << culmuNAcc << " frac of MB: " << culmuNAcc/nMB << endl;
   cout << "PS:" << culmuNAccPS << " frac of MB: " << culmuNAccPS/nMB << endl;
}


