#include <TH1.h>
#include <TTree.h>
#include <iostream>
#include <TLegend.h>
#include "TLegendEntry.h"
#include "TString.h"

class compareHist
{
   public:
       compareHist(TTree *myTree,const char *var,const char *cut1,const char *cut2, float low=0,float high=0,int nbin=100,int normType=2);
       compareHist(TTree *myTree1,TTree *myTree2,const char *var,const char *cut1,double low=0,double high=0,int nbin=100,int normType=2);
       compareHist(TTree *myTree1,TTree *myTree2,const char *var,const char *name,const char *cut1,const char *cut2,double low=0,double high=0,int nbin=100,int normType=2);

       TLegend * leg1;

       TH1F *h1;
       TH1F *h2;

       string name1;
       string name2;
       
       void init();
       void SetXTitle(const char *t) { h1->SetXTitle(t); h2->SetXTitle(t);}
       void SetYTitle(const char *t) { h1->SetYTitle(t); h2->SetYTitle(t);}
       void SetMinimum(double min)   { h1->SetMinimum(min); h2->SetMinimum(min);}
       void SetHistName1(const char *t) { name1 = t; };
       void SetHistName2(const char *t) { name2 = t; };
       void SetHistName1(string t) { name1 = t; };
       void SetHistName2(string t) { name2 = t; };
       void SetMarkerSize(double t) { h1->SetMarkerSize(t); h2->SetMarkerSize(t);}
       void Draw(TString opt) { h1->Draw("E"); h2->Draw(opt+"same"); if(leg1) leg1->Draw();}
       void Draw2(TString opt) { h2->Draw("E"); h1->Draw(opt+"same"); if(leg1) leg1->Draw();}
       void SetLegend(double x1,double y1, double x2, double y2);
};

compareHist::compareHist(TTree *myTree,const char *var,const char *cut1,const char *cut2, float low,float high,int nbin,int normType)
{
   if (low==high) {
       low = myTree->GetMinimum(var);
       high = myTree->GetMaximum(var);
       
   }
   
   h1 = new TH1F(Form("h%s",var),"",nbin,low,high);
   h2 = new TH1F(Form("h2%s",var),"",nbin,low,high);
   myTree->Draw(Form("%s>>h%s",var,var),cut1);
   myTree->Draw(Form("%s>>h2%s",var,var),cut2);

   h1->SetLineColor(2);
   h2->SetLineColor(4);
   h1->SetMarkerColor(2);
   h2->SetMarkerColor(4);
   h2->SetMarkerStyle(0);
   
   h1->Sumw2();
   h2->Sumw2();
   
   if (normType==1) {
      h1->Scale(1./h1->GetEntries()/h1->GetBinWidth(1));
      h2->Scale(1./h2->GetEntries()/h2->GetBinWidth(1));
   } else if (normType==2) {
      h1->Scale(1./myTree->GetEntries()/h1->GetBinWidth(1));
      h2->Scale(1./myTree->GetEntries()/h2->GetBinWidth(1));
   }
}

compareHist::compareHist(TTree *myTree1,TTree *myTree2,const char *var,const char *cut1,double low,double high,int nbin,int normType)
{
   if (low==high) {
       low = myTree1->GetMinimum(var);
       if (low>myTree2->GetMinimum(var)) low = myTree2->GetMinimum(var);
       high = myTree2->GetMaximum(var);
       if (high<myTree2->GetMinimum(var)) high = myTree2->GetMinimum(var);
       cout <<low<<" "<<high<<endl;
       
   }

   h1 = new TH1F(Form("h%s",var),"",nbin,low,high);
   h2 = new TH1F(Form("h2%s",var),"",nbin,low,high);
   myTree1->Draw(Form("%s>>h%s",var,var),cut1);
   myTree2->Draw(Form("%s>>h2%s",var,var),cut1);

   h1->SetLineColor(2);
   h2->SetLineColor(4);
   h1->SetMarkerColor(2);
   h2->SetMarkerColor(4);
   
   h1->Sumw2();
   h2->Sumw2();
   
   if (normType==1) {
      h1->Scale(1./h1->GetEntries());
      h2->Scale(1./h2->GetEntries());
   } else if (normType==2) {
      h1->Scale(1./myTree1->GetEntries());
      h2->Scale(1./myTree2->GetEntries());
   }
}

compareHist::compareHist(TTree *myTree1,TTree *myTree2,const char *var,const char *name,const char *cut1,const char *cut2,double low,double high,int nbin,int normType)
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
   myTree1->Draw(Form("%s>>h%s",var,name),cut1);
   myTree2->Draw(Form("%s>>h2%s",var,name),cut2);

   h1->SetLineColor(1);
   h2->SetLineColor(2);
   h1->SetMarkerColor(1);
   h2->SetMarkerColor(2);
   h2->SetMarkerStyle(0);
   
   h1->Sumw2();
   h2->Sumw2();
   
   if (normType==1) {
      h1->Scale(1./h1->GetEntries());
      h2->Scale(1./h2->GetEntries());
   } else if (normType==2) {
      h1->Scale(1./myTree1->GetEntries());
      h2->Scale(1./myTree2->GetEntries());
   }
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
   leg1->SetFillStyle(0);  
   leg1->SetTextFont(62);
   leg1->SetFillColor(0); 
   leg1->SetBorderSize(0);
   leg1->SetTextSize(0.02);
   if (name1!="")  {
     leg1->AddEntry(h1,name1.data(),"pl");
   }
   if (name2!="") {
     leg1->AddEntry(h2,name2.data(),"pl");
   }
}
