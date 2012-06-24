#ifndef SignalCorrector_h
#define SignalCorrector_h
#include "TF1.h"
#include "TF2.h"

//---------------------------------------------------------------------
class Region
{
public:
   Region(TString regName) :
   name(regName),
   n(0),nExtrap(0),normBinWidth(false),verbosity(0)
   {}
   
//    void Init(TTree * t, int nbins, float xmin, float xmax) {
//       float * bins = new float[nbins+1];
//       float dx = (xmax-xmin)/nbins;
//       for (int i=0; i<nbins+1; ++i) bins[i] = xmin+i*dx;
//       Init(t,nbins,bins);
//    }
   
   void Init(TTree * t, int nbins, float *bins, int nybins, float *ybins) {
      cut*=weight;
      h = new TH2D(name,"",nbins,bins,nybins,ybins);
      if (var!="") {
         n = t->Project(h->GetName(),var,cut,"");
         if (verbosity>=1) cout << "  " << h->GetName() << "  draw: " << var << " cut: " << TString(cut) << ": " << n << endl;
      }
   }
   
   void Extrapolate() {
      hExtrap = (TH2D*)h->Clone(Form("%sExtrap",h->GetName()));
      hExtrap->Scale(purity);
      nExtrap = n*purity;
   }
   
   void Normalize(float norm) {
      hExtrapNorm = (TH2D*)hExtrap->Clone(Form("%sNorm",hExtrap->GetName()));
      hExtrapNorm->Scale(norm);
      if (normBinWidth) hExtrapNorm->Scale(1./hExtrapNorm->GetBinWidth(1));
   }
   
   TH2D * h;
   TH2D * hExtrap;
   TH2D * hExtrapNorm;
   TString name;
   TString var;
   TCut cut;
   TString weight;
   float purity;
   float n;
   float nExtrap;
   bool normBinWidth;
   int verbosity;
};

//---------------------------------------------------------------------
class SignalCorrector
{
public:
   TString name;
   Region rSigAllLeading;
   Region rSigAll;
   Region rBkgDPhi;
   Region rSubtracted;

   SignalCorrector(TString n) :
   name(n),
   rSigAllLeading(n+"SigAllLeading"),
   rSigAll(n+"SigAll"),
   rBkgDPhi(n+"BkgDPhi"),
   rSubtracted(n+"Subtracted")
   {
   }
   
   void SetVerbosity(int v=1) {
      if (v>=1) {
         rSigAll.verbosity = 1;
         rBkgDPhi.verbosity = 1;
      }
      if (v>=2) {
         rSigAllLeading.verbosity = 1;
      }
   }      
};

double calcMean(TH1* h){
   double tot = 0;
   double integral=  0;
   for(int i = 1; i <= h->GetNbinsX(); ++i){
      double x = h->GetBinCenter(i);
      double y = h->GetBinContent(i);
      tot += y*x;
      integral += y;
   }
   return tot/integral;
}

#endif
