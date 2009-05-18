#include "savedfrankTools.C"
#include <iostream>
using namespace std;

void driverTools ()
{
   TH1F * h = createHist("testh","new hist",10,0,10);
   for (int i =0; i<10; ++i) h->Fill(3);
   drawNormHist("testh","histE1","normalized hist","#xi","per jet",0.1,true,2,0,3,1,1);
}
