#ifndef HISTMATH_H
#define HISTMATH_H
#include "TH1D.h"

void HistMath() {;}

TH1D * integrateHist(TH1 * hin)
{
   printf("Want to integrate: %s\n",hin->GetName());
   TH1D* hout = (TH1D*)hin->Clone(Form("%s_Integ",hin->GetName()));
   //TH1D * hout = dynamic_cast<TH1D*>(hin->Clone(Form("%s_Integ",hin->GetName())));
   hout->SetTitle(Form("%s Integrated",hin->GetTitle()));

   Float_t sum = 0.;
   for (Int_t i=1; i<=hin->GetNbinsX(); ++i) {
      // calc the integral up to this bin
      sum += (hin->GetBinContent(i) * hin->GetBinWidth(i));
      // set the integral up to this bin
      hout->SetBinContent(i,sum);
      // set the error
      hout->SetBinError(i,0);
   }
   return hout;
}

void normHist(TH1 * hin, Int_t type)
{
  // call sumw2 if hasn't been called
  if (hin->GetSumw2N()!=(hin->GetNbinsX()+2)) hin->Sumw2();
  // Normalize by area
  if (type<10) {
    hin->Scale(1./hin->Integral());
  } else if (type<20) {
    //cout << "Entries " << hin->GetEntries() << endl;
    hin->Scale(1./hin->GetEntries());
  }

  // normalize bin width
  if (type%10==0) {
    hin->Scale(1./hin->GetBinWidth(1));
  }
  if (type%10==1) {
    for (Int_t i=1; i<=hin->GetNbinsX(); ++i) {
      Double_t w = hin->GetBinWidth(i);
      Double_t h = hin->GetBinContent(i);
      Double_t e = hin->GetBinError(i);
      //cout << "bin " << i << ": " << "width: " << w << " height: " << h << " error: " << e << endl;
      //cout << "    new height: " << h/w << " new error: " << e/w << endl;
      hin->SetBinContent(i,h/w);
      hin->SetBinError(i,e/w);
    }
  }
}
#endif // HISTMATH_H
