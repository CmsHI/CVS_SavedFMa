#include "ana/savedfrankTools.C"
#include <iostream>
using namespace std;

void driverTools ()
{
   TH1F * h = createHist("testh","new hist",10,0,10);
   for (int i =0; i<10; ++i) h->Fill(3);
   drawNormHist("testh","histE1","normalized hist","#xi","per jet",0.1,true,2,0,3,1,1);

   TFile * tf1 = findFile("/net/pstore01/d00/scratch/frankma/hiroot/prodPtHat50/pythia50/iCone5_match35_jetEtCut30_ntJetEt/test_50k.root");
   //TTree * tr1 = findTree(tf1,"NTTruePFF");
   TTree * tr1 = findTree(tf1,"badname");
   tr1->Print();
   TString msg("message");
   terminate(Form("%s now exit\n",msg.Data()));
}
