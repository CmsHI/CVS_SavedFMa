#include <TH1D.h>

TH1D* IntegrateFromLeft(TH1* h){

   TH1D* hh = (TH1D*)h->Clone(Form("%s_Integrated",h->GetName()));

   double sum = 0;
   double e2 = 0;
   double n = 0;
   for(int i = 1; i < h->GetNbinsX()+1; ++i){
      double e = h->GetBinError(i);
      double a = h->GetBinContent(i);
      sum+=a;
      e2 += e*e;

      hh->SetBinContent(i,sum);
      hh->SetBinError(i,sqrt(e2));
   }

   return hh;
}
