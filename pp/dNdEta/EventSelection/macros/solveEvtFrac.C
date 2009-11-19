// ////////////////////////////////////////////////////////////////////////////
#if !defined( __CINT__) || defined(__MAKECINT__)
#include <TMatrixD.h>
#include "TNtuple.h"
#include "TString.h"
#else
#endif

#include<vector>
using namespace std

void readEff(const char * infile)
{
}

void solveEvtFrac( ) {

#ifdef __CINT__
  gSystem->Load("libMatrix");
#endif

  // Matrix of coeffs.
  TMatrixD coeff(2,2);
  coeff(0,0) = 0.534;
  coeff(0,1) = 0.921;
  coeff(1,0) = 0.761;
  coeff(1,1) = 0.983;

  coeff.Print();

  // Matrix of Right Hand side
  TMatrixD rhs(2,1);
  rhs(0,0) = 0.834;
  rhs(1,0) = 0.933;

  rhs.Print();

  // Invert the matrix
  TMatrixD coeffIN(2,2);
  coeffIN = coeff.Invert();

  coeffIN.Print();

  // Solution of the equations
  TMatrixD sol(2,1);
  sol.Mult(coeffIN, rhs);
  sol.Print();
}
