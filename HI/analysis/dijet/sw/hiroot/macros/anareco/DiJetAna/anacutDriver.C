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
   //c1.SetPartlPtMin(0.5);
   c1.SetJetPartlDRMax(0.5);
   c1.CreateJetCut();
   c1.CreateJetParticlesCut();

   c1.AddWeight("ppt/nljCone5Et",c1.nearJetPartlsCut_);
   c1.AddWeight("ppt/aljCone5Et",c1.awayJetPartlsCut_);

   cout << c1 << endl;

}
