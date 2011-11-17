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
   Compare(TString name, TTree * t1, TTree * t2, TCut s1, TCut s2, TString c) :
   name1(name+"1"),name2(name+"2"),tag(name),
   sel1(s1), sel2(s2), cut(c),
   nSelEvt1(0), nSelEvt2(0), normMode(1) {
      tr1 = t1;
      tr2 = t2;
   }
   
   TString name1, name2, tag;
   TCut sel1, sel2, cut;
   int nSelEvt1, nSelEvt2;
   int normMode;
   TH1D * h1;
   TH1D * h2;
   TTree * tr1;
   TTree * tr2;
   TLegend * leg;
   
   void Init(int nbins, float xmin, float xmax) {
      h1 = new TH1D(name1,"",nbins,xmin,xmax);
      h2 = new TH1D(name2,"",nbins,xmin,xmax);      
   }
   void Init(int nbins, float * bins) {
      h1 = new TH1D(name1,"",nbins,bins);
      h2 = new TH1D(name2,"",nbins,bins);
   }
   
   void Project(TString var) {
      cout << var+name1 << " " << TString(sel1&&cut) << endl;
      cout << var+name2 << " " << TString(sel2&&cut) << endl;
      tr1->Draw(var+name1,sel1&&cut,"goff");
      tr2->Draw(var+name2,sel2&&cut,"goff");
   }
   
   void Draw(TString title, float ymin=0, float ymax=0, TString lname1="data", TString lname2="ref", bool drawLeg = true, float lx1=0.4, float ly1=0.18, float lx2=0.87, float ly2=0.28) {
      nSelEvt1 = tr1->GetEntries(sel1);
      nSelEvt2 = tr2->GetEntries(sel2);
      
      if (normMode==1) {
         h1->Scale(1./nSelEvt1);
         h2->Scale(1./nSelEvt2);
      }      

      h1->SetTitle(title);
      h2->SetTitle(title);
      h2->SetLineColor(kRed);
      h2->SetMarkerColor(kRed);
      h2->SetMarkerStyle(kOpenCircle);
      
      if (ymin<ymax) {
         h1->SetAxisRange(ymin,ymax,"Y");
         h2->SetAxisRange(ymin,ymax,"Y");
      }
      
      if (h1->GetMaximum()>h2->GetMaximum()) {
         h1->Draw("E");
         h2->Draw("same hist");
      } else {
         h2->Draw("hist");
         h1->Draw("same E");
      }

      leg = new TLegend(lx1,ly1,lx2,ly2);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.035);
      leg->AddEntry(h1,lname1,"p");
      leg->AddEntry(h2,lname2,"l");
      if (drawLeg) leg->Draw();
   }
   
   TH1D * Ratio(TString ytitle="ratio") {
      TH1D * hr = (TH1D*)h1->Clone(name1+"_div_"+name2);
      hr->Divide(h2);
      hr->SetYTitle(ytitle);
      return hr;
   }
};