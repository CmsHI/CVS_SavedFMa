#include "HistMath.C"
#include "TH1D.h"

void histMathDriver()
{
   TH1D * hin = new TH1D("hin","input",20,0,100);
   for (int i=0; i<100; ++i) {
      hin->Fill(i);
   }

   TH1D* hIntegral = integrateHist(hin);
   TH1D* hIntegral2 = integrateHist(hIntegral);

   // with a function
   //form1 = new TFormula("form1","abs(sin(x)/x)");
   //sqroot = new TF1("sqroot","x*gaus(0) + [3]*form1",0,10);
   //sqroot->SetParameters(10,4,1,20);

}

