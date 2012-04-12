#ifndef SignalCorrector_h
#define SignalCorrector_h
#include "TF1.h"

//---------------------------------------------------------------------
class Region
{
public:
   Region(TString regName, TString v, TCut c, TString w) :
   name(regName),var(v),cut(c),weight(w),
   n(0),nExtrap(0),normBinWidth(false)
   {}
   
   void Init(TTree * t, int nbins, float xmin, float xmax) {
      float * bins = new float[nbins+1];
      float dx = (xmax-xmin)/nbins;
      for (int i=0; i<nbins+1; ++i) bins[i] = xmin+i*dx;
      Init(t,nbins,bins);
   }
   
   void Init(TTree * t, int nbins, float *bins) {
      cut*=weight;
      h = new TH1D(name,"",nbins,bins);
      if (var!="") {
         n = t->Project(h->GetName(),var,cut);
         if (TString(cut).Contains("Aj>=0.0")&&!TString(h->GetName()).Contains("AllLeading")) cout << "  " << h->GetName() << "  draw: " << var << " cut: " << TString(cut) << ": " << n << endl;
      }
   }
   
   void Extrapolate(float scale) {
      hExtrap = (TH1D*)h->Clone(Form("%sExtrap",h->GetName()));
      hExtrap->Scale(scale);
      nExtrap = n*scale;
   }
   
   void Normalize(float norm) {
      hExtrapNorm = (TH1D*)hExtrap->Clone(Form("%sNorm",hExtrap->GetName()));
      hExtrapNorm->Scale(norm/hExtrapNorm->Integral());
      if (normBinWidth) hExtrapNorm->Scale(1./hExtrapNorm->GetBinWidth(1));
   }
   
   TH1D * h;
   TH1D * hExtrap;
   TH1D * hExtrapNorm;
   TString name;
   TString var;
   TCut cut;
   TString weight;
   float n;
   float nExtrap;
   bool normBinWidth;
};

//---------------------------------------------------------------------
class SignalCorrector
{
public:
   SignalCorrector(TTree * tree, TString n, TString var, TCut s, TString w="(1==1)", int bin=-1, int srcType=1) : 
   name(n),
   observable(var),
   sel(s),
   rSigAllLeading(n+"SignalAllLeading","pt1",s,w),
   rSigAll(n+"SignalAll",var,s,w),
   rBkgDPhi(n+"BkgDPhi",var,s,w),
   rSubtracted(n+"Subtracted",var,s,w),
   weight(w),
   subDPhiSide(true),
   centBin(bin),
   dataSrcType(srcType),
   nSelPhoton(0),fracDPhiBkg(0),fracPhotonBkg(0),fracPhotonDPhiBkg(0) {
      t = tree;
      // photon purity values
      hPhotonPurity = (TH1D*)gDirectory->FindObjectAny("hPhotonPurity");
      if (!hPhotonPurity) hPhotonPurity = new TH1D("hPhotonPurity","",4,0,4);
      
      verbosity=0;
      if (TString(s).Contains("cBin>=0.0")) {
         verbosity=1;
      }
   }
   
   void SetJetWeights(TString jetvar="inclJetPt")
   {
      //
      // Centrality bins: 0=0-4, 1=4-12, 2=12-20, 3=20-40
      //
      TString jetweight;
      if (dataSrcType<=1) {
         if (centBin==0) {
            jetweight = Form("1./(0.992966*0.5*(TMath::Erf(-0.627825+0.054612*%s)+1))",jetvar.Data());
         } else if (centBin==1) {
            jetweight = Form("1./(1.000029*0.5*(TMath::Erf(-0.571441+0.049538*%s)+1))",jetvar.Data());
         } else if (centBin==2) {
            jetweight = Form("1./(0.997227*0.5*(TMath::Erf(-0.695991+0.051474*%s)+1))",jetvar.Data());
         } else if (centBin==3) {
            jetweight = Form("1./(0.998959*0.5*(TMath::Erf(-0.898526+0.059567*%s)+1))",jetvar.Data());
         }
      } else { // for now use peripheral for pp 2.76TeV
         jetweight = Form("1./(0.998959*0.5*(TMath::Erf(-0.898526+0.059567*%s)+1))",jetvar.Data());
      }
      if (verbosity>0) cout << "Jet weight: " << jetweight << endl;
      
      rSigAll.weight += "*"+jetweight;
   }

   void MakeHistograms(TCut sigSel, int nbin, float xmin, float xmax) {
      float * bins = new float[nbin+1];
      float dx = (xmax-xmin)/nbin;
      for (int i=0; i<nbin+1; ++i) {
         bins[i] = xmin+i*dx;
      }
      MakeHistograms(sigSel,nbin,bins);
   }
   
   void MakeHistograms(TCut sigSel, int nbin, float * bins) {
      // setup cuts
      if (verbosity>1) cout << "Base Selection: " << sel << endl;
      rSigAllLeading.cut         = sel&&cutSigAllLeading;
      rSigAll.cut            = sel&&sigSel;
      rBkgDPhi.cut           = sel&&cutBkgDPhi;
      
      rSigAllLeading.Init(t,160,0,800);
      rSigAll.Init(t,nbin,bins);
      rBkgDPhi.Init(t,nbin,bins);
      
      // photon normalization
      nSelPhoton = t->GetEntries(rSigAllLeading.cut);
      // number of events in signal region
      if (verbosity>0) cout << " ** Number of selection photons: " << rSigAllLeading.n << " gamma-jets: " << rSigAll.n << " ** " << endl;
   }
   
   void Extrapolate(float dphisidescale=1) {
      // Scales
      float sssidescale = 0,ssdphisidescale=0;
      if (verbosity>0) {
         cout << " Bkg scale dphi: " << dphisidescale << endl;
      }

      rSigAllLeading.Extrapolate(1-fracPhotonBkg);
      rSigAll.Extrapolate(1.);
      rBkgDPhi.Extrapolate(dphisidescale);

      // Fractions
      if (rSigAll.n>0) {
         fracDPhiBkg = rBkgDPhi.n * dphisidescale/rSigAll.n;
      }
   }

   // special case for dphi
   void ExtrapolateDPhiHist(float dphiSigCut=0.7) {
      float binw = 3.1415926/20;
      float fitmin=dphiSigCut+binw;
      float fitmax=3.1415926/2+binw;
      TF1 *p0 = new TF1("p0","pol0",fitmin,fitmax);
      if (subDPhiSide&&rBkgDPhi.n>=10) {
         p0->SetParameter(0,rBkgDPhi.hExtrap->GetBinContent(rBkgDPhi.hExtrap->FindBin(3.14/2)));
         rBkgDPhi.hExtrap->Fit("p0","","",fitmin,fitmax);
         for (int i=rBkgDPhi.hExtrap->FindBin(dphiSigCut); i<=rBkgDPhi.hExtrap->GetNbinsX(); ++i) {
            rBkgDPhi.hExtrap->SetBinContent(i,p0->GetParameter(0));
            rBkgDPhi.hExtrap->SetBinError(i,p0->GetParError(0));
         }
      }
   }
   
   void SubtractBkg() {
      rSubtracted.hExtrap = (TH1D*)rSigAll.h->Clone(rSubtracted.name+"Extrap");
      rSubtracted.nExtrap = rSigAll.nExtrap;
      if (verbosity>1) cout << "Raw Sig: " << rSigAll.nExtrap;
      if (subDPhiSide) {
         rSubtracted.hExtrap->Add(rBkgDPhi.hExtrap,-1);
         rSubtracted.nExtrap -= rBkgDPhi.nExtrap;
         if (verbosity>1) cout << " - " << rBkgDPhi.nExtrap;
      }
      // store rSubtracted.nExtrap in histogram
      rSubtracted.hExtrap->SetEntries(rSubtracted.nExtrap);
      if (verbosity>1) {
         cout << " =? " << rSubtracted.nExtrap << endl;
         cout << "Check Integ:  " << rSigAll.hExtrap->Integral() << " - " << 
         rBkgDPhi.hExtrap->Integral()
         << " = " << rSubtracted.hExtrap->Integral() << endl;
      }
   }
   
   void Normalize(int normMode=1) { // 0=area is signal region count, 1=unit normalization, 2=per photon normalization
      if (normMode==0) {
         rSigAll.hExtrapNorm = (TH1D*)rSigAll.hExtrap->Clone(Form("%sNorm",rSigAll.hExtrap->GetName()));
         rBkgDPhi.hExtrapNorm = (TH1D*)rBkgDPhi.hExtrap->Clone(Form("%sNorm",rBkgDPhi.hExtrap->GetName()));
         rSubtracted.hExtrapNorm = (TH1D*)rSubtracted.hExtrap->Clone(Form("%sNorm",rSubtracted.hExtrap->GetName()));
      } else {
         float rawarea=1,area=1;
         if (normMode%10==2) {
            rawarea*=(rSigAll.h->Integral())/rSigAllLeading.h->Integral();
            area*=(rSubtracted.hExtrap->Integral()) / (rSigAllLeading.hExtrap->Integral());
         }
         rSigAll.Normalize(rawarea);
         rSubtracted.Normalize(area);
         if (normMode<10) {
            rBkgDPhi.Normalize(rawarea*fracDPhiBkg);
         } else if (normMode>=20) {
            rBkgDPhi.Normalize(rawarea*fracDPhiBkg);
         }
         if (verbosity>0) cout << "Norlamize to: " << area << " chk integ: " << rSubtracted.hExtrapNorm->Integral() << endl;
      }
   }
   
   TTree * t;
   TString name,nameIsol,observable;
   TCut sel,cutIsol;
   Region rSigAllLeading;
   Region rSigAll;
   Region rBkgDPhi;
   Region rSubtracted;
   TString weight;
   bool subDPhiSide;
   bool subSShapeSide;
   bool subSShapeSideDPhiSide;
   TCut cutSigAllLeading;
   TCut cutBkgDPhi;
   float nSelPhoton;
   float fracDPhiBkg;
   float fracPhotonBkg;
   float fracPhotonDPhiBkg;
   int centBin;
   int dataSrcType;
   TH1D * hPhotonPurity;
   int verbosity;
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
