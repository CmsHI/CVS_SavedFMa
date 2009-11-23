#include <iostream>
#include "TFitter.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"

#include "SelectionData.h"
using namespace std;

void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg);

void minuit(){
  TFitter* minimizer = new TFitter(NTYPES);
  {
    double p1 = -1;
    minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
  }

  // Minimize convolution by Minuit

  minimizer->SetFCN(minuitFunction);
  minimizer->SetParameter(0,"SD",0.22,0.2,0,0); //P index, P label, initial value, expected distance, nevermind, nevermind
  minimizer->SetParameter(1,"NSD",0.775,0.2,0,0);

  minimizer->ExecuteCommand("SIMPLEX",0,0);
  minimizer->ExecuteCommand("MIGRAD",0,0);

  double bestSD = minimizer->GetParameter(0);
  double errorSD = minimizer->GetParError(0);
  double bestNSD = minimizer->GetParameter(1);
  double errorNSD = minimizer->GetParError(1);

  cout<<"SD = "<<bestSD<< " error: " << errorSD << endl;
  cout<<"NSD = "<<bestNSD<< " error: " << errorNSD << endl;
}
