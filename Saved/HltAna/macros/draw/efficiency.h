#ifndef efficiency_h
#define efficiency_h

#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TString.h"
#include "TLegend.h"
#include "TLine.h"

class Efficiency
{
public:
   TString name,var;
   TCut sel,cut;
   TTree * t;
   TProfile * hEff;

   Efficiency(TTree * tree, TString n,TString v,TCut s, TCut c, int nbin,float xmin,float xmax):
   name(n),var(v),sel(s),cut(c){
      t=tree;
      hEff = new TProfile(name,"",nbin,xmin,xmax);
      hEff->GetXaxis()->SetNdivisions(505);
      hEff->GetXaxis()->CenterTitle();
      hEff->GetYaxis()->CenterTitle();      
   }
   
   void Draw(int mkcolor=kBlack, int mkst=kFullCircle, TString opt="sameE") {
      TString draw=TString(cut)+":"+var+">>"+name;
      t->Draw(draw,sel,"prof goff");
      std::cout << draw << " " << TString(sel) << std::endl;
      hEff->SetLineColor(mkcolor);
      hEff->SetMarkerColor(mkcolor);
      hEff->SetMarkerStyle(mkst);
      hEff->Draw(opt);
   }
};

#endif