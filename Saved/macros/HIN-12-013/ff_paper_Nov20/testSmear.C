#include "SmearingFactors.h"

void testSmear(int c=0)
{
   LoadParameters();
   
   TCanvas * c3 = new TCanvas("c3","",500,500);
   fsm[2][c]->Draw();

   for (int i=0; i<20; ++i) {   
      float jpt = i*10;
      float fsmpt = GetSmearedPtData(2,c,jpt,0, "no");
      cout << "jet " << jpt << " smear: " << fsmpt << endl;
   }
}