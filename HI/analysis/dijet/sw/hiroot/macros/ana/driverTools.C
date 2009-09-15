#include "savedfrankTools.C"
#include <iostream>
using namespace std;

void driverTools ()
{
   TH1D * h = createHist("testh","new hist",10,0,10);
   for (int i =0; i<10; ++i) h->Fill(3);
   drawNormHist("testh","histE1","normalized hist","#xi","per jet",0.1,true,kRed,0,3,1,1);
   TH1D * h2 = createHist("testh2","new hist",10,0,10);
   for (int i =0; i<20; ++i) h2->Fill(3);
   drawNormHist("testh2","histE1 same","normalized hist","#xi","per jet",0.1,true,kBlue,0,3,1,1);

   drawDivHist("testh","testh2","hist E1","#xi","per jet",10,0,10);

   //TFile * tf1 = findFile("/net/pstore01/d00/scratch/frankma/hiroot/prodPtHat50/pythia50/iCone5_match35_jetEtCut30_ntJetEt/test_50k.root");
   //TTree * tr1 = findTree(tf1,"NTTruePFF");
   //tr1->Print();
   TString msg("message");
   //terminate(Form("%s now exit\n",msg.Data()));

   printf("will try to integrate: %s\n",h->GetName());
   drawIntHist("testh","testinth","P","step function: integrated delta function","x","value",-1,0,kRed,1,3,3,kFullCircle);
}
