#include <iostream>
#include "TFitter.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
using namespace std;

double tsallis(double* var, double* par){

   double pt = var[0];
   double A = par[0]; // Constant term                                  
   double nT = par[1]; // Denominator                                   
   double n = par[2]; // Exponent                                       
   double m = 0.140; // Pion mass                                       
   double et = sqrt(m*m+pt*pt);

   return A*pt*pow((1+et/nT),(-1*n));

}

double poly(double* var, double* par){	

	double pt = var[0];
	double A = par[0]; // Constant term
	double p0 = par[1]; // Denominator
	double n = par[2]; // Exponent

	return A*pt*pow((1+pt/p0),(-1*n));	
}

double chi2(TH1*, TH1*);
void fitSlices(TH2*, TF1*);


class Folder {
public:
  double operator()(int& nDim, double* gout, double& result, double par[], int flg){
    result = convolute(par[0],par[1],par[2]);
  }


	double convolute(double a, double b, double c, bool display = false);

	int nBins;
	int iterations;
	
	TH2D* correlation;

	TH1D* measurement;

   TH1D* truth;	
	
	TH1D* corrMean;
	TH1D* corrSigma;

	TF1* fMean;
	TF1* fSigma;
	
	TF1* spectrum;	
	TF1* smear;
		
	void initialize();

	int counter;
	
private:
	
};

void Folder::initialize(){

	counter = 0;
	
	fMean =  new TF1("fMean","[0]/x",0.02,5);
	fSigma =  new TF1("fSigma","[0]*exp([1]/x)",0.02,5);	
	//	fSigma->SetParameter(0,0.1);
	//	fSigma->SetParameter(1,2);

        smear = new TF1("smear","landau(0)",0,correlation->GetYaxis()->GetXmax()*2);
        smear->SetParameter(0,1);
	smear->SetParLimits(1,0,0.5);

	//	correlation->FitSlicesY(0,0,-1,0,"Q R G1");	
	fitSlices(correlation,smear);

	smear->SetRange(0,correlation->GetYaxis()->GetXmax()*10);

	corrMean = (TH1D*)gDirectory->Get(Form("%s_1",correlation->GetName()));
	corrSigma = (TH1D*)gDirectory->Get(Form("%s_2",correlation->GetName()));
	corrMean->Fit(fMean,"R");
	corrSigma->Fit(fSigma,"R");

	//	fMean->SetRange(0,50);
	//        fSigma->SetRange(0,50);

	spectrum = new TF1("spectrum",tsallis,0.02,correlation->GetXaxis()->GetXmax(),3);
	//	spectrum = new TF1("spectrum","gaus(0)",correlation->GetXaxis()->GetXmin(),correlation->GetXaxis()->GetXmax());

	spectrum->SetParameter(0,400);
	spectrum->SetParameter(1,1.7);
	spectrum->SetParameter(2,10);
		
	iterations = 5;
	nBins = 10;	
	
}

double Folder::convolute(double a, double b, double c, bool display){
	TH1D hTemp("htemp","htemp",measurement->GetNbinsX(),measurement->GetXaxis()->GetXmin(),measurement->GetXaxis()->GetXmax());
	
	spectrum->SetParameter(0,a);
	spectrum->SetParameter(1,b);
	spectrum->SetParameter(2,c);
	
	for(int ifill = 0; ifill < iterations; ++ifill){
	   double x = spectrum->GetRandom();
	   double mean = fMean->Eval(x);
	   //	   if(mean > correlation->GetYaxis()->GetXmax()) smear->SetParameter(0,1);
	   smear->SetParameter(1,mean);
	   smear->SetParameter(2,fSigma->Eval(x));	
	   if(smear->Integral(0,correlation->GetYaxis()->GetXmax()*20) == 0) cout<<"PT = "<<x<<endl;
	   hTemp.Fill(smear->GetRandom());
	   //           if(mean > correlation->GetYaxis()->GetXmax()) smear->SetParameter(0,0);
	}

	hTemp.Scale(measurement->Integral()/hTemp.Integral());
        double c2 = chi2(measurement, &hTemp);

	if(display){

		new TCanvas();
		spectrum->Draw();
		
		new TCanvas();
		measurement->Draw();

		TH1D* hPT0 = dynamic_cast<TH1D*>(spectrum->GetHistogram()->Clone(Form("hPT%d",counter)));
		hPT0->Scale((truth->Integral()*truth->GetBinWidth(5))/(hPT0->Integral()*hPT0->GetBinWidth(5)));

		TH1D* hTemp0 = new TH1D(hTemp);
		hTemp0->SetName(Form("hTemp%d",counter++));		
		hTemp0->SetLineColor(2);		
		hTemp0->Draw("same");		

	}	
        return c2;
	
}





static Folder* _folder;

void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg){
	result = _folder->convolute(par[0],par[1],par[2]);
}


void minuit(){

   TH1::SetDefaultSumw2();

   TFitter* minimizer = new TFitter(3);
   {
      double p1 = -1; minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
   }

	TFile * infile = new TFile("data.root");
	
	TH1D* hPT = dynamic_cast<TH1D*>(infile->Get("hp1"));
	TH1D* hDeltaPhi = dynamic_cast<TH1D*>(infile->Get("hDeltaPhi"));
	TH2D* hDPhiPT = dynamic_cast<TH2D*>(infile->Get("hDPhiPT"));
	TH1D* hDataDeltaPhi = dynamic_cast<TH1D*>(infile->Get("refDeltaPhi"));

	hDataDeltaPhi->Rebin(5);
	
	_folder = new Folder();
	_folder->correlation = hDPhiPT;	
	_folder->measurement = hDataDeltaPhi;
	_folder->truth = hPT;
	_folder->initialize();
	
	hPT->Fit(_folder->spectrum,"M");

	hPT->Draw();
	

	// Minimize convolution by Minuit

	minimizer->SetFCN(minuitFunction);
	minimizer->SetParameter(0,"X",1100.,200,0,0);
	minimizer->SetParameter(1,"Y",1.,0.4,0,0);
	minimizer->SetParameter(2,"Z",20,3.5,0,0);
	//	        minimizer->ExecuteCommand("SIMPLEX",0,0);
	//	        minimizer->ExecuteCommand("MIGRAD",0,0);
	double bestX = minimizer->GetParameter(0);
	double bestY = minimizer->GetParameter(1);
	double bestZ = minimizer->GetParameter(2);

	cout<<"X = "<<bestX<<" Y = "<<bestY<<" Z = "<<bestZ<<endl;


	
// Test Convolution
// Find minimum by hand
	if(0){
	
	cout<<"Results:"<<endl;

	cout<<"Playing with p0"<<endl;

	cout<<_folder->convolute(1288,3.7,10.7,true)<<endl;
        cout<<_folder->convolute(1288,3.1,10.7,true)<<endl;

	cout<<_folder->convolute(1288,2.8,10.7,true)<<endl;
        cout<<_folder->convolute(1288,2.3,10.7,true)<<endl;
	
        cout<<_folder->convolute(1288,1.8,10.7,true)<<endl; // best
        cout<<_folder->convolute(1288,1.6,10.7,true)<<endl;
	cout<<_folder->convolute(1288,1.5,10.7,true)<<endl; // correct
	cout<<_folder->convolute(1288,1.2,10.7,true)<<endl;
	cout<<_folder->convolute(1288,1.1,10.7,true)<<endl;

	cout<<_folder->convolute(1288,0.8,10.7,true)<<endl;
        cout<<_folder->convolute(1288,0.6,10.7,true)<<endl;

	cout<<_folder->convolute(1288,0.3,10.7,true)<<endl;
        cout<<_folder->convolute(1288,0.1,10.7,true)<<endl;

        cout<<"Playing with n"<<endl;

        cout<<_folder->convolute(1288,1.5,15.5,true)<<endl;
        cout<<_folder->convolute(1288,1.5,13.5,true)<<endl;
        cout<<_folder->convolute(1288,1.5,12.9,true)<<endl;
        cout<<_folder->convolute(1288,1.5,12.2,true)<<endl;

        cout<<_folder->convolute(1288,1.5,11.7,true)<<endl;
        cout<<_folder->convolute(1288,1.5,11.2,true)<<endl;
        cout<<_folder->convolute(1288,1.5,10.7,true)<<endl; // correct
        cout<<_folder->convolute(1288,1.5,10.2,true)<<endl;
        cout<<_folder->convolute(1288,1.5,9.8,true)<<endl;

        cout<<_folder->convolute(1288,1.5,9.1,true)<<endl; // best best
        cout<<_folder->convolute(1288,1.5,8.2,true)<<endl;
        cout<<_folder->convolute(1288,1.5,7.1,true)<<endl;
        cout<<_folder->convolute(1288,1.5,5.2,true)<<endl;


	/*
Best fit is:

   1             1.28841e+03   1.50160e+02   1.50160e+00   2.97574e+00
   2             1.51480e+00   3.80057e-01   3.80057e-03   7.10100e+03
   3             1.07135e+01   1.91374e+00   1.91374e-02  -1.21587e+03

	 */

	}

	new TCanvas();

	_folder->spectrum->SetParameter(0,bestX);
	_folder->spectrum->SetParameter(1,bestY);
	_folder->spectrum->SetParameter(2,bestZ);

	TH1* hUnfolded = _folder->spectrum->GetHistogram();
	hUnfolded->Draw();


	TH1D* hDiv = dynamic_cast<TH1D*>(hPT->Clone("hDiv"));
	hDiv->Divide(_folder->spectrum);		

        new TCanvas();
	hDiv->Draw();

}

double chi2(TH1* h1, TH1* h2){

   double c = 0;

   for(int i = 1; i < h1->GetNbinsX(); ++i){
      double d = h1->GetBinContent(i) - h2->GetBinContent(i);
      double e = h1->GetBinError(i);

      if(d != 0 && e != 0){
	 c += (d*d)/(e*e);
      }

   }

   return c;

}

void fitSlices(TH2* hCorr, TF1* func){

   int nBins = hCorr->GetNbinsX();

   TH1D* hMean = new TH1D(Form("%s_1",hCorr->GetName()),"",nBins,hCorr->GetXaxis()->GetXmin(),hCorr->GetXaxis()->GetXmax());
   TH1D* hSigma = new TH1D(Form("%s_2",hCorr->GetName()),"",nBins,hCorr->GetXaxis()->GetXmin(),hCorr->GetXaxis()->GetXmax());

   for(int i = 1; i < nBins; ++i){
      TH1D* h = hCorr->ProjectionY(Form("%s_bin%d",hCorr->GetName(),i),i-1,i);
      h->Fit(func);
      hMean->SetBinContent(i,func->GetParameter(1));
      hMean->SetBinError(i,func->GetParError(1));
      hSigma->SetBinContent(i,func->GetParameter(2));
      hSigma->SetBinError(i,func->GetParError(2));
   }
}





