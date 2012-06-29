#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
using namespace std;

class Compare
{
public:
   Compare(TString nm, TCut s, int norm=0) :
   name(nm),
   sel(s),
   normMode(norm) {
   }
   
   TString name;
   TCut sel;
   vector<TCut> vsel;
   vector<int> vnSelEvt;
   vector<float> vnorm;
   vector<TString> vwt;
   int normMode;
   vector<TProfile*> vh;
   vector<TTree*> vt;
   TLegend * leg;
   
   void Init(int nbins, float xmin, float xmax) {
      float * bins = new float[nbins+1];
      float dx = (xmax-xmin)/nbins;
      for (int i=0; i<nbins+1; ++i) {
         bins[i] = xmin+i*dx;
      }
      Init(nbins,bins);
   }
   void Init(int nbins, float * bins) {
      for (int i=0; i<vt.size(); ++i) {
         vnSelEvt.push_back(0);
         vh.push_back(new TProfile(Form("%s_%d",name.Data(),i),"",nbins,bins));
         vnorm.push_back(1.);
      }
   }
   
   void Project(TString var) {
      cout << "draw var: " << var << endl;
      for (int i=0; i<vt.size(); ++i) {
         TCut finalcut = sel&&vsel[i];
         if (vwt.size()>0) finalcut*=vwt[i];
         vt[i]->Draw(Form("%s%s",var.Data(),vh[i]->GetName()),finalcut,"goff");
//          vt[i]->Draw(Form("%s%s",var.Data(),vh[i]->GetName()),finalcut,"goff",10000);
         cout << TString(finalcut) << " " << vh[i]->GetEntries() << endl;
         if (normMode>=20&&normMode<30) {
            vnorm[i] = vt[i]->GetEntries(sel);
         }
         cout << "    # evts: " << vnorm[i] << endl;
      }
   }
   
   void Draw(TString title, float ymin=0, float ymax=0) {
//       int vstyl[10] = {kFullCircle,kOpenCircle,kOpenCircle,kOpenCircle,kOpenCircle};
//       int vstyl[10] = {kFullCircle,kOpenCircle,0,kOpenCircle,kOpenCircle};
      int vstyl[10] = {kFullCircle,0,0,kOpenCircle,kOpenCircle};
      int vcolor[10] = {kBlack,kRed,kBlue,kGreen+2,kOrange+2};
      float foundYMin=1e10,foundYMax=-1;
      for (int i=0; i<vt.size(); ++i) {
         if (normMode==1) {
            vh[i]->Scale(1./vh[i]->Integral());
         } else if (normMode==2) {
            vh[i]->Scale(1./vh[i]->Integral(),"width");
         } else if (normMode==10) {
            vh[i]->Scale(vnorm[i]/vh[i]->Integral());
         } else if (normMode==20) {
            vh[i]->Scale(1./vnorm[i]);
         } else if (normMode==22) {
            vh[i]->Scale(1./vnorm[i],"width");
         }
         vh[i]->SetTitle(title);
         vh[i]->SetLineColor(vcolor[i]);
         vh[i]->SetMarkerColor(vcolor[i]);
         vh[i]->SetMarkerStyle(vstyl[i]);
         vh[i]->GetXaxis()->CenterTitle();
         vh[i]->GetYaxis()->CenterTitle();
         vh[i]->GetXaxis()->SetNdivisions(505);
         if (vh[i]->GetMaximum()>foundYMax) foundYMax = vh[i]->GetMaximum();
         if (vh[i]->GetMinimum()<foundYMin) foundYMin = vh[i]->GetMinimum();
      }

      
      if (ymin>=ymax) {
         ymin=foundYMin*1.2;
         ymax=foundYMax*1.6;
      }
      vh[0]->SetAxisRange(ymin,ymax,"Y");
      vh[0]->Draw("E");      
      for (int i=1; i<vt.size(); ++i) {
         if (vstyl[i]!=0) vh[i]->Draw("sameE");
         else vh[i]->Draw("same hist");
      }
   }
   
   void Legend(float lx1=0.4, float ly1=0.18, float lx2=0.87, float ly2=0.28) {
      leg = new TLegend(lx1,ly1,lx2,ly2);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextFont(63);
      leg->SetTextSize(16);
   }
   
   TH1D * Ratio(TString ytitle="ratio") {
      if (vh.size()<2) {
         cout << "not enough histograms to divide" << endl;
         return 0;
      }
      TH1D * hr = (TH1D*)vh[0]->Clone(Form("%s_div_%s",vh[0]->GetName(),vh[1]->GetName()));
      hr->Divide((TH1D*)vh[1]);
      hr->SetYTitle(ytitle);
      //hr->Fit("pol1","");
      hr->SetAxisRange(0,2,"Y");
      return hr;
   }
};