#include <iostream>
#include <algorithm>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

using namespace std;


void getXS(){

  bool inclusive = false;
  TFile* outf = new TFile("results.root","recreate");

  int j = 0;
    
    TH1::SetDefaultSumw2();
    TFile* inf = new TFile("dijets.root");

    outf->cd();    
    TH1D* h = new TH1D(Form("h%d",j),"",500,0,1000);
    TTree* t = (TTree*)inf->Get("jets/t");
    //    if(j< t->GetEntries()) t->SetEntries(j);
    
    if(inclusive) t->Draw(Form("jtpt>>h%d",j),"abs(jteta) < 5");
    else t->Draw(Form("jtpt[0]>>h%d",j),"abs(jteta[0]) < 5 && nref > 0");

    double Nev = t->GetEntries();
    double ncollProb = 1.;
    double A = 208;
    double AA = A*A*ncollProb;
    double bin = h->GetBinWidth(1); 
    
    double xs = 6.206E-02; // mb
    double intlum = 5000; // invmb
    double s = xs*intlum*AA/Nev/bin;

    double nChosen = 100;
    
    h->Scale(s);

    cout<<"h integral : "<<h->Integral("width")<<endl;

    float jtpt[1000],jteta[1000];
    int nref;
    t->SetBranchAddress("jtpt",jtpt);
    t->SetBranchAddress("jtpt",jteta);
    t->SetBranchAddress("nref",&nref);

    vector<float> vj;

    for(int i = 0; i < t->GetEntries(); ++i){
      t->GetEntry(i);
      if(inclusive){
	for(int it = 0; it < nref; ++it){
	  if(fabs(jteta[it]) < 5) vj.push_back(jtpt[it]);
	}
      }else{
	if(nref > 0 && fabs(jteta[0]) < 5) vj.push_back(jtpt[0]);
	else vj.push_back(0.);
      }
    }

    sort(vj.begin(),vj.end());

    int hund = vj.size() - xs*intlum*AA*nChosen*vj.size();
    if(hund > 0){
      cout<<"Entries = "<<j<<endl;
      cout<<"PT from vector : "<<vj[hund]<<endl;
    }

    double tot = 0;
    for(int i = h->GetNbinsX()+1; i >=0; --i){
      tot+= h->GetBinContent(i)*bin;
      if(tot > nChosen){
	cout<<"Entries = "<<j<<endl;   
	cout<<"PT : "<<h->GetBinLowEdge(i)<<endl;
	break;
      }
    }
    

  outf->Write();
  
}
