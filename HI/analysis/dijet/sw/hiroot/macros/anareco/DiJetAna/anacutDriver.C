#include <iostream>
#include "AnaCuts.h"
using namespace std;
using namespace DiJetAna;

void anacutDriver ()
{
   AnaCuts cut0;
   cut0.SetCutTag("vset");
   printf("cut0: %s\n",cut0.GetCutTag().Data());
   cout << cut0 << endl;

   AnaCuts cut1("v1");
   printf("cut1: %s\n",cut1.GetCutTag().Data());
   cout << cut1 << endl;
   
}
