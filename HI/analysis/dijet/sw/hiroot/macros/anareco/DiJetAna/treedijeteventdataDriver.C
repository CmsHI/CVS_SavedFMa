#include "TreeDiJetEventData.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
using namespace DiJetAna;

void treedijeteventdataDriver()
{
   TFile f("eventdata.root","RECREATE","dijet event data");

   TTree * evtt = new TTree("evtTree","dijet event data tree");

   TreeDiJetEventData td(evtt);
   printf("%f\n",td.nljet_);

   // set branch
   td.SetBranches();

   //  --event level--
   td.run_=77;
   td.event_=77;
   td.mass_=77;
   td.cmeta_=77;

   //  -- parton info --
   td.nlpid_=77;
   td.nlpstat_=77;
   td.nlpet_=77;
   td.nlpetsm_=77;
   td.nlpeta_=77;
   td.nlpphi_=77;

   td.alpid_=77;
   td.alpstat_=77;
   td.alpet_=77;
   td.alpetsm_=77;
   td.alpeta_=77;
   td.alpphi_=77;

   td.pdphi_=77;

   //  -- jet info --
   td.nljet_=77;
   td.nljetsm_=77;
   td.nljeta_=77;
   td.nljphi_=77;
   td.nljnp_=77;

   td.aljet_=77;
   td.aljetsm_=77;
   td.aljeta_=77;
   td.aljphi_=77;
   td.aljnp_=77;

   td.jdphi_=77;

   // -- particle info --
   int evtnp = 0;
   for(int i=0; i<10; ++i) {
      ++evtnp;
      td.ppid_[i] = 3;
      td.ppt_[i] = 3;
      td.peta_[i] = 3;
      td.pphi_[i] = 3;

      td.pndphi_[i] = 3;
      td.pndeta_[i] = 3;
      td.pndr_[i] = 3;
      td.pndrbg_[i] = 3;

      td.padphi_[i] = 3;
      td.padeta_[i] = 3;
      td.padr_[i] = 3;
      td.padrbg_[i] = 3;

      td.zn_[i] = 4;
      td.za_[i] = 4;
   }
   td.evtnp_ = evtnp;


   evtt->Fill();

   // save
   f.Write();
}
