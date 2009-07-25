#include <iostream>
#include "AnaCuts.h"
using namespace std;
using namespace DiJetAna;

void anacutDriver ()
{
   AnaCuts cut0;
   printf("cut0: %s\n",cut0.GetCutTag().Data());
   cout << cut0 << endl;

   AnaCuts c1("v1");
   c1.SetNearJetEtMin(90);
   c1.SetNearJetEtMax(110);
   c1.SetAwayJetEtMin(50);
   c1.SetDPhiMin(2.8);
   //c1.SetPPtMin(0.5);
   c1.CreateJetTreeCut();

   cout << c1 << endl;
   
}
