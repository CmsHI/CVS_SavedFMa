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

double trig_chi(double effsd, double fsd, double effnsd, double fnsd, double effall)
{
  return effsd*fsd + effnsd*fnsd - effall;
}

double full_chi(double fsd, double fnsd)
{
  // 900 900
  // MinBiasPixel1Track, MinBiasBscOr
  double A = trig_chi(0.53,fsd,0.92,fnsd,0.83);
  double B = trig_chi(0.76,fsd,0.98,fnsd,0.93);
  return A*A+B*B;
}

void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg)
{
  result = full_chi(par[0],par[1]);
}

// ===== Main function =====
int fitEvtFrac( )
{
  // delcare selection data
  SelectionData sdata;

  // read in trigger info
  readInputs("../data/trig_eff.txt",sdata);

  // run minuit macro
  minuit();
  return 0;
}
