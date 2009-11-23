#include <iostream>
#include "TFitter.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
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


void minuit(){

   TH1::SetDefaultSumw2();

   const int NTYPES=2;
   TFitter* minimizer = new TFitter(NTYPES);
   {
      double p1 = -1;
      minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
   }

   //_folder = new Folder();
   //_folder->initialize();

   // Minimize convolution by Minuit

   minimizer->SetFCN(minuitFunction);
   minimizer->SetParameter(0,"SD",0.2,0.2,0,0); //P index, P label, initial value, expected distance, nevermind, nevermind
   minimizer->SetParameter(1,"NSD",0.77,0.2,0,0);

   minimizer->ExecuteCommand("SIMPLEX",0,0);
   minimizer->ExecuteCommand("MIGRAD",0,0);

   double bestSD = minimizer->GetParameter(0);
   double errorSD = minimizer->GetParError(0);
   double bestNSD = minimizer->GetParameter(1);
   double errorNSD = minimizer->GetParError(1);

   cout<<"SD = "<<bestSD<< " error: " << errorSD << endl;
   cout<<"NSD = "<<bestNSD<< " error: " << errorNSD << endl;
}
