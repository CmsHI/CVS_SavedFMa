#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
using namespace std;
#define PI 3.141592653589

void formatHist(TH1* h, int col, double norm){

  //  h->Scale(1/norm);                                                                                                                                       
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

}

void saveCanvas(TCanvas* c, int date){
  c->Write();
  c->Draw();

  c->Print(Form("./figures/%s_d%d.gif",c->GetTitle(),date));
  c->Print(Form("./figures/%s_d%d.eps",c->GetTitle(),date));
  c->Print(Form("./figures/%s_d%d.C",c->GetTitle(),date));

}


TH1D* myDivide(TH1* a,TH1* b)
{
  TH1D *h = new TH1D("h","",a->GetNbinsX(),a->GetXaxis()->GetXmin(),a->GetXaxis()->GetXmax());

  for(int i=0;i<h->GetNbinsX();i++)
    {
      Double_t X = a->GetBinContent(i);
      Double_t Xerr = a->GetBinError(i);
      Double_t Y = b->GetBinContent(i);
      Double_t Yerr = b->GetBinError(i);
      if (Xerr==0) Xerr=X;
      if (Yerr==0) Yerr=Y;

      Double_t R = X/Y;
      Double_t Rerr = R*sqrt((Xerr/X)*(Xerr/X)+(Yerr/Y)*(Yerr/Y));
      if (X==0||Y==0) {
        R=0;
        Rerr=0;
      }
      cout <<i<<" "<<R<<" "<<Rerr<<" "<<X<<" "<<Y<<endl;
      h->SetBinContent(i,R);
      h->SetBinError(i,Rerr);
    }

  h->SetName(Form("%s_%d",a->GetName(),1));
  h->SetTitle(a->GetTitle());
  h->GetXaxis()->SetTitle(a->GetXaxis()->GetTitle());
  h->GetYaxis()->SetTitle(a->GetYaxis()->GetTitle());

  return h;
}

double deltaPhi(double phi1, double phi2){
  double phi = fabs(phi1-phi2);
  if(phi > 2*PI) phi =  phi - 2*PI;
  if(phi > PI ) phi = 2*PI - phi;
  return fabs(phi);

}

double deltaEta(double eta1, double eta2){
  double d = fabs(eta1-eta2);
  return d;
}


double deltaR2(double eta1, double phi1, double eta2, double phi2){
  double e = deltaEta(eta1,eta2);
  double p = deltaPhi(phi1,phi2);
  double d = e*e+p*p;
  return d;
}

double deltaR(double eta1, double phi1, double eta2, double phi2){
  double d = deltaR2(eta1,phi1,eta2,phi2);
  return sqrt(d);
}

double etaVertex(double eta, double vz){
  
  if(fabs(eta) > 1.3) return eta; 

  double rho = 228;  
  double z1 = rho/tan(2*atan(exp(-eta)));
  double z2 = z1 - vz;
  double etav = -log(tan(atan(rho/z2)/2));

}


