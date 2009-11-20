#include <TMatrixD.h>
#include "TString.h"
#include <vector>

class SelectionData
{
  // trigger info
  // size: N
  std::vector<TString> trigName;
  std::vector<std::vector<Double_t> > trigEff;

  // equations
  // size: N choose s
  std::vector<TMatrixD> coeff;
  std::vector<TMatrixD> rhs;
  std::vector<TMatrixD> sol;
};
