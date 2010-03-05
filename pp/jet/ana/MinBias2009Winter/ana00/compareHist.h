#include <TH1.h>
#include <TTree.h>
#include <iostream>
#include <TLegend.h>
#include "TLegendEntry.h"
#include "TString.h"

class compareHist
{
   public:
       compareHist(TTree *myTree1,TTree *myTree2,const char *var,const char *name,const char *cut1,const char *cut2,double low=0,double high=0,int nbin=100);

       TLegend * leg1;

       TH1F *h1;
       TH1F *h2;

       string name1;
       string name2;
       
       void init();
       void SetXTitle(const char *t) { h1->SetXTitle(t); h2->SetXTitle(t);}
       void SetYTitle(const char *t) { h1->SetYTitle(t); h2->SetYTitle(t);}
       void SetMinimum(double min)   { h1->SetMinimum(min); h2->SetMinimum(min);}
       void SetMaximum(double max)   { h1->SetMaximum(max); h2->SetMaximum(max);}
       void SetHistName1(const char *t) { name1 = t; };
       void SetHistName2(const char *t) { name2 = t; };
       void SetHistName1(string t) { name1 = t; };
       void SetHistName2(string t) { name2 = t; };
       void SetMarkerSize(double t) { h1->SetMarkerSize(t); h2->SetMarkerSize(t);}
       void Draw(TString opt) { h1->Draw(opt); h2->Draw("hist E same"); if(leg1) leg1->Draw();}
       void Draw2(TString opt) { h2->Draw("histE"); h1->Draw(opt+"same"); if(leg1) leg1->Draw();}
       void SetLegend(double x1,double y1, double x2, double y2);
       void AppendToHist(TTree *myTree1,TTree *myTree2,const char *var,const char *cut1,const char *cut2);
       void Normalize(int normType=1);
};


compareHist::compareHist(TTree *myTree1,TTree *myTree2,const char *var,const char *name,const char *cut1,const char *cut2,double low,double high,int nbin)
{
   if (low==high) {
       low = myTree1->GetMinimum(var);
       if (low>myTree2->GetMinimum(var)) low = myTree2->GetMinimum(var);
       high = myTree2->GetMaximum(var);
       if (high<myTree2->GetMinimum(var)) high = myTree2->GetMinimum(var);
       cout <<low<<" "<<high<<endl;
   }

   h1 = new TH1F(Form("h%s",name),"",nbin,low,high);
   h2 = new TH1F(Form("h2%s",name),"",nbin,low,high);
   h1->SetLineColor(1);
   h2->SetLineColor(2);
   h2->SetMarkerStyle(0);
   h1->SetMarkerColor(1);
   h2->SetMarkerColor(2);
   h1->Sumw2();
   h2->Sumw2();

   myTree1->Draw(Form("%s>>h%s",var,name),cut1);
   myTree2->Draw(Form("%s>>h2%s",var,name),cut2);
   //cout << "filled hists" << endl;
}



void compareHist::init()
{
   leg1=0;
   name1="";
   name2="";
}


void compareHist::SetLegend(double x1,double y1, double x2, double y2)
{
   leg1 = new TLegend(x1,y1,x2,y2,NULL,"brNDC");
   if (name1!="")  {
     leg1->AddEntry(h1,name1.data(),"pl");
   }
   if (name2!="") {
     leg1->AddEntry(h2,name2.data(),"pl");
   }

   leg1->SetFillStyle(0);
   leg1->SetFillColor(0);
   leg1->SetTextSize(0.035);
}

void compareHist::AppendToHist(TTree *myTree1,TTree *myTree2,const char *var,const char *name,const char *cut1,const char *cut2)
{
  myTree1->Draw(Form("%s>>+h%s",var,name),cut1);
  myTree2->Draw(Form("%s>>+h2%s",var,name),cut2);
}

void compareHist::Normalize(int normType)
{
   if (normType==1) {
      h1->Scale(1./h1->GetEntries(),"width");
      h2->Scale(1./h2->GetEntries(),"width");
   } else if (normType==2) {
      h1->Scale(1./myTree1->GetEntries(),"width");
      h2->Scale(1./myTree2->GetEntries(),"width");
   }
}
