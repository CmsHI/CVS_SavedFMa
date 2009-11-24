#include <iostream>
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"

#include "SelectionData.h"
using namespace std;

double trig_chi(double effsd, double fsd, double effnsd, double fnsd, double effall);
double full_chi(double fsd, double fnsd);
void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg);

void minuit(FitData & ft)
{
  // make it quiet
  TFitter* minimizer_ = new TFitter(NTYPES);
  cout << "allocated minimizer" << endl;
  double p1 = -1;
  minimizer_->ExecuteCommand("SET PRINTOUT",&p1,1);

  // Minimize chi2 by Minuit
  minimizer_->SetFCN(minuitFunction);
  minimizer_->SetParameter(0,"SD",ft.bestX_,ft.searchDist_,0,0); //P index, P label, initial value, expected distance, nevermind, nevermind
  minimizer_->SetParameter(1,"NSD",ft.bestY_,ft.searchDist_,0,0);

  minimizer_->ExecuteCommand("SIMPLEX",0,0);
  minimizer_->ExecuteCommand("MIGRAD",0,0);

  ft.bestX_ = minimizer_->GetParameter(0);
  ft.errorX_ = minimizer_->GetParError(0);
  ft.bestY_ = minimizer_->GetParameter(1);
  ft.errorY_ = minimizer_->GetParError(1);

  cout<<"SD = "<<ft.bestX_<< " error: " << ft.errorX_ << endl;
  cout<<"NSD = "<<ft.bestY_<< " error: " << ft.errorY_ << endl;
}

void scanUncert(FitData & ft)
{
  // make it quiet
  TFitter* minimizer_ = new TFitter(NTYPES);
  cout << "allocated minimizer" << endl;
  double p1 = -1;
  minimizer_->ExecuteCommand("SET PRINTOUT",&p1,1);

  // Scan the X parameter to find its uncertainty
  double errX;
  double dChi2Max=0.1;
  for (errX=0; ft.bestX_+errX<1; errX+=0.01) {
    minimizer_->SetParameter(0,"X",ft.bestX_+errX,0,0,0);
    minimizer_->SetParameter(1,"Y",ft.bestY_,ft.searchDist_,0,0);
    minimizer_->ExecuteCommand("MIGRAD",0,0);
    double t = full_chi(minimizer_->GetParameter(0),
			minimizer_->GetParameter(1));
    //cout << "t: " << t << endl;
    if (t-full_chi(ft.bestX_,ft.bestY_) > dChi2Max) {
      cout << "reached deltaChi2 > "<< dChi2Max << endl;
      break;
    }
  }
  ft.errorX_ = errX;
  cout << " errorX: " << errX << endl;;
}
