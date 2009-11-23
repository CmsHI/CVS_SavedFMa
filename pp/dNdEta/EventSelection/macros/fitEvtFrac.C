// ////////////////////////////////////////////////////////////////////////////
#if !defined( __CINT__) || defined(__MAKECINT__)
#include <TMatrixD.h>
#include "TNtuple.h"
#include "TString.h"
#else
#endif

#include <iostream>
#include <fstream>
#include <vector>

#include "SelectionData.h"
#include "minuit.C"
using namespace std;

void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg){

  // return chi2;
  //result = _folder->convolute(par[0],par[1],par[2]);
  double sd=par[0];
  double nsd=par[1];
  // 900 900
  //double trigA = 0.534*sd+0.92*nsd - 0.834;
  //double trigB = 0.761*sd+0.983*nsd - 0.933;
  // 2.2 900
  //double trigA = 0.551*sd+0.92*nsd - 0.834;
  //double trigB = 0.74*sd+0.98*nsd - 0.933;
  // 7 900
  double trigA = 0.57*sd+0.94*nsd - 0.834;
  double trigB = 0.717*sd+0.98*nsd - 0.933;
  result = trigA*trigA + trigB*trigB;
}

// ===== Main function =====
int fitEvtFrac( )
{
  // delcare selection data
  SelectionData sdata;

  // read in trigger info
  readInputs("../data/trig_eff.txt",sdata);
  minuit();
  return 0;
}
