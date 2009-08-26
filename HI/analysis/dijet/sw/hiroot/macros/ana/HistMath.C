#ifndef HISTMATH_H
#define HISTMATH_H
#include "TH1D.h"

void HistMath() {;}

TH1D * integrateHist(TH1 * hin)
{
   TH1D * hout = dynamic_cast<TH1D*>(hin->Clone(Form("%s_Integ",hin->GetName())));
   hout->SetTitle(Form("%s Integrated",hin->GetTitle()));

   Float_t sum = 0.;
   for (Int_t i=1; i<=hin->GetNbinsX(); ++i) {
      sum += hin->GetBinContent(i);
      hout->SetBinContent(i,sum);
   }
   return hout;
}

#endif // HISTMATH_H
