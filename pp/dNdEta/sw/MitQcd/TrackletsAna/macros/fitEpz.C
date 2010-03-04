//
//  This example has been taken from the ROOT tutorials website
//  and has been modified by P.Castro 14th July, 2003
//
//   Example of a program to fit non-equidistant data points
//   =======================================================
//
//   The fitting function fcn is a simple chisquare function
//   The data consists of 5 data points (arrays x,y,z) + the errors in errorsz
//   More details on the various functions or parameters for these functions
//   can be obtained in an interactive ROOT session with:
//    Root > TMinuit *minuit = new TMinuit(10);
//    Root > minuit->mnhelp("*")  to see the list of possible keywords
//    Root > minuit->mnhelp("SET") explains most parameters
//
#include <iostream.h>

#include <TROOT.h>
#include <TMinuit.h>

const int iNum = 5;
Float_t z[5],x[5],y[5],errorz[5];

TFile *fin;
TH1F *fData;
TH1F *fMC;
TH1F *f1;
TH1F *f2;

// this is the function used for the fit
//   par: vector with the fit parameters
Double_t fit_function(float x,float y,Double_t *par)
{
 double value=( (par[0]*par[0])/(x*x)-1)/ ( par[1]+par[2]*y-par[3]*y*y);
 return value;
}

void calc_chi_square(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  
  TH1D * hData = (TH1D*)fData->Clone("hData");
  TH1D * hMC = (TH1D*) fMC->Clone("hMC");
  TH1D * h1 = (TH1D*) f1->Clone("h1");
  TH1D * h2 = (TH1D*)f2->Clone("h2");
  
  // calc rel frac
  Double_t testNSDFrac = 1-par[0];
  Double_t MCSDFrac = (Double_t)h1->GetEntries()/(Double_t)hMC->GetEntries();
  Double_t MCNSDFrac = (Double_t)h2->GetEntries()/(Double_t)hMC->GetEntries();
  
  Double_t SDRelFrac = par[0];
  Double_t NSDRelFrac = 1.0-par[0];

  h1->Scale(1./h1->GetEntries()/h1->GetBinWidth(1));
  h2->Scale(1./h2->GetEntries()/h1->GetBinWidth(1));
  hMC->Scale(1./hMC->GetEntries()/h1->GetBinWidth(1));
  hData->Scale(1./hData->GetEntries()/h1->GetBinWidth(1));

  // combine different processes in MC with given weights
  TH1D * h3 = (TH1D*)h2->Clone("h3");
  h3->SetLineColor(kRed);
  h3->SetLineStyle(1);
  h3->SetMarkerColor(kRed);
  h3->SetMarkerStyle(kOpenSquare);

  h3->Scale(NSDRelFrac);
  h3->Add(h1,SDRelFrac);

  hData->Draw();
// hMC->Draw("same");
//  h1->Draw("same");
//  h2->Draw("same");
  h3->Draw("same");

  c1->Update();
  getchar();

  // Result
  Double_t sum=0;
  for (Int_t i=1; i<=hData->GetNbinsX(); ++i) {
  //  for (Int_t i=1; i<=40; ++i) {
    if(h3->GetBinContent(i)!=0&&hData->GetBinContent(i)!=0.0){
      Double_t binDiff = h3->GetBinContent(i)-hData->GetBinContent(i);
      Double_t binE1 = hData->GetBinError(i);
      Double_t binE2 = h3->GetBinError(i);
      Double_t binChi2 = binDiff*binDiff/(binE1*binE1+binE2*binE2);
      sum+=binChi2;
    }
  }
  printf("sum: %f\n",sum);
  /*
  //calculate chisquare
  double chisq = 0;
  for (int i=0;i<iNum; i++) {
    // chi square is the quadratic sum of the distance from the point to the function weighted by its error
    double delta  = (z[i]-fit_function(x[i],y[i],par))/errorz[i];
    chisq += delta*delta;
  }
  */
  f = sum;
  return;
}

fitEpz(const char * fname)
{

  fin = new TFile(fname);
  fData = (TH1F *)fin->Get("hEaddEp_data");
  fMC = (TH1F *)fin->Get("hEaddEp_mc_All");
  f1 = (TH1F *)fin->Get("hEaddEp_mc_DF");
  f2 = (TH1F *)fin->Get("hEaddEp_mc_ND");

  fData->Draw();
  fMC->Draw("same");
  f1->Draw("same");
  f2->Draw("same");

  c1->Update();
  getchar();
  // The z values
  z[0]=0.2;
  // The errors on z values
  Float_t error = 0.01;
  errorz[0]=error;
  // the x values
  x[0]=0.2;
  // the y values
  y[0]=0.2;

  TMinuit *ptMinuit = new TMinuit(1);  //initialize TMinuit with a maximum of 5 params
  //
  //  select verbose level:
  //    default :     (58 lines in this test)
  //    -1 : minimum  ( 4 lines in this test)
  //     0 : low      (31 lines)
  //     1 : medium   (61 lines)
  //     2 : high     (89 lines)
  //     3 : maximum (199 lines in this test)
  //
  ptMinuit->SetPrintLevel();
  // set the user function that calculates chi_square (the value to minimize)
  ptMinuit->SetFCN(calc_chi_square);

  Double_t arglist[10];
  Int_t ierflg = 0;

  arglist[0] = 1;
  ptMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

  // Set starting values and step sizes for parameters
  Double_t vstart[4] = {0.1, 1 , 0.1 , 0.01};
  Double_t step[4] = {0.01 , 0.1 , 0.01 , 0.001};
  ptMinuit->mnparm(0, "a1", vstart[0], step[0], 0.0 ,1.0,ierflg);


  // Now ready for minimization step
  arglist[0] = 500;
  arglist[1] = 1.;
  ptMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);

  // Print results
  cout << "\nPrint results from minuit\n";
  double fParamVal;
  double fParamErr;
  ptMinuit->GetParameter(0,fParamVal,fParamErr);
  cout << "a1=" << fParamVal << "err = "<< fParamErr << "\n";
  // if you want to access to these parameters, use:
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  ptMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);

  ptMinuit->mnsimp();
  //  ptMinuit->mnseek();
  //void mnstat(Double_t &fmin, Double_t &fedm, Double_t &errdef, Int_t &npari, Int_t &nparx, Int_t &istat) 
  //*-*-*-*-*Returns concerning the current status of the minimization*-*-*-*-*
  //*-*      =========================================================
  //*-*       User-called
  //*-*          Namely, it returns:
  //*-*        FMIN: the best function value found so far
  //*-*        FEDM: the estimated vertical distance remaining to minimum
  //*-*        ERRDEF: the value of UP defining parameter uncertainties
  //*-*        NPARI: the number of currently variable parameters
  //*-*        NPARX: the highest (external) parameter number defined by user
  //*-*        ISTAT: a status integer indicating how good is the covariance
  //*-*           matrix:  0= not calculated at all
  //*-*                    1= approximation only, not accurate
  //*-*                    2= full matrix, but forced positive-definite
  //*-*                    3= full accurate covariance matrix
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
      cout << "\n";
  cout << " Minimum chi square = " << amin << "\n";
  cout << " Estimated vert. distance to min. = " << edm << "\n";
  cout << " Number of variable parameters = " << nvpar << "\n";
  cout << " Highest number of parameters defined by user = " << nparx << "\n";
  cout << " Status of covariance matrix = " << icstat << "\n";

  cout << "\n";
  ptMinuit->mnprin(3,amin);
  //*-*-*-*Prints the values of the parameters at the time of the call*-*-*-*-*
  //*-*    ===========================================================
  //*-*        also prints other relevant information such as function value,
      //*-*        estimated distance to minimum, parameter errors, step sizes.
  //*-*
  //*-*         According to the value of IKODE, the printout is:
  //*-*    IKODE=INKODE= 0    only info about function value
  //*-*                  1    parameter values, errors, limits
  //*-*                  2    values, errors, step sizes, internal values
  //*-*                  3    values, errors, step sizes, first derivs.
  //*-*                  4    values, parabolic errors, MINOS errors
  //*-*    when INKODE=5, MNPRIN chooses IKODE=1,2, or 3, according to ISW(2)
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

      return 1.0;
}

