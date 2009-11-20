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
using namespace std;

void readInputs(const char * infname)
{
  ifstream inFile(infname);
  // check
  if (!inFile) {
    cerr << "Unable to open " << infname << endl;
    exit(1);
  }
  // read
  TString s;
  while (inFile>>s) {
    cout << s << endl;
  }
  inFile.close();
}

// solve function
void solveLin( ) {

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

// ===== Main function =====
int solveEvtFrac( )
{
#ifdef __CINT__
  gSystem->Load("libMatrix");
#endif

  // read in trigger info
  readInputs("../data/trig_eff.txt");
  return 0;
}
