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

void readInputs(const char * infname, SelectionData & sdata)
{
  ifstream inFile(infname);
  // check
  if (!inFile) {
    cerr << "Unable to open " << infname << endl;
    exit(1);
  }
  // read
  TString trigName;
  Double_t effAll;
  Double_t effSD;
  Double_t effDD;
  Double_t effNSD;
  Double_t effND;
  while (inFile>>trigName &&
	 inFile>>effAll &&
	 inFile>>effSD &&
	 inFile>>effDD &&
	 inFile>>effNSD &&
	 inFile>>effND
	 ) {
    cout << trigName << ": " << effAll << " " << effSD << " " << effDD << " " << effNSD << " " << effND << endl;
    sdata.trig_.push_back(trigName);
    vector<Double_t> veff;
    veff.push_back(effAll);
    veff.push_back(effSD);
    veff.push_back(effDD);
    veff.push_back(effNSD);
    veff.push_back(effND);
    sdata.eff_.push_back(veff);
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

  // delcare selection data
  SelectionData sdata;

  // read in trigger info
  readInputs("../data/trig_eff.txt",sdata);
  return 0;
}
