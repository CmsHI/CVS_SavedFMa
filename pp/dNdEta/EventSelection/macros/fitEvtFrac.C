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

// delcare selection data
SelectionData sdata;

double trig_chi(double effsd, double fsd, double effnsd, double fnsd, double effall)
{
  double A = effsd*fsd + effnsd*fnsd - effall;
  return A*A;
}

double full_chi(double fsd, double fnsd)
{
  // 900 900
  // MinBiasPixel1Track, MinBiasBscOr
//  double A = trig_chi(0.53,fsd,0.92,fnsd,0.83);
//  double B = trig_chi(0.76,fsd,0.98,fnsd,0.93);

  double sum=0;
  //for (UInt_t i=2; i<sdata.eff_.size()-2; ++i) {
  for (UInt_t i=3; i<7; ++i) {
    sum+= trig_chi(sdata.eff_[i][1],fsd,sdata.eff_[i][3],fnsd,sdata.eff_[i][0]);
  }
  return sum;
}

void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg)
{
  result = full_chi(par[0],par[1]);
}

// ===== Main function =====
int fitEvtFrac(const char* inData="../data/trig_eff_900_900.txt")
{
  // read in trigger info
  readInputs(inData,sdata);

  cout << "total # of trigs: " << sdata.eff_.size() << endl;;
  //cout << sdata << endl;
  cout << "using for now: " << endl;
  for (UInt_t i=2; i<sdata.eff_.size()-2; ++i) {
    cout << sdata.trig_[i] << " effsd: " << sdata.eff_[i][1] << " effnsd: " << sdata.eff_[i][3] << " effall: " << sdata.eff_[i][0] << endl;
  }

  // run minuit macro
  minuit();
  return 0;
}
