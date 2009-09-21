#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <vector>
#include <iostream>
#include "/net/hisrv0001/home/frankma/UserCode/SavedFMa/HI/analysis/dijet/sw/hiroot/macros/ana/savedfrankTools.C"

using namespace std;
const char * dr1="hist";
const char * dr2="hist same";
Int_t NETBINS = 200;
Float_t ETMAX = 200;


void compare_one(const vector<TTree*> & vf, const vector<TString> & vars, const vector<TString> & cuts, Color_t* colors=0,Style_t* styles=0, Int_t NBINS = 100, Double_t xmin=0,Double_t xmax=0)
{
   for (int i=0; i<1; ++i) {
      //vf[i]->Print();
      TString opt;
      TString hname;
      TString htitle;

      for (int j=0; j<vars.size(); ++j) {
	 //printf("color %d: %d\n", j, colors[j]);
	 for (int k=0; k<cuts.size(); ++k) {
	    if (k==0)
	       opt=dr1;
	    else
	       opt=dr2;
	    if (k==0) {
	       hname  = Form("%s_uncut",vars[j].Data());
	       htitle = Form("%s_uncut;%s;",vars[j].Data(),vars[j].Data());
	    }
	    else {
	       hname = Form("%s_cut%d",vars[j].Data(),k);
	       htitle = Form("%s_cut%d;%s;",vars[j].Data(),k,vars[j].Data());
	    }
	    cout << opt << "  " << hname << "  " << htitle << "  " << "color/style: " << colors[k] << ", " << styles[j] << endl;
	    drawTree(vf[i],vars[j].Data(),cuts[k].Data(),opt.Data(),hname.Data(),htitle.Data(),NBINS,xmin,xmax,1,colors[k],styles[k],3,0,0);
	 }
      }
   }
}

void compareOHDistr(char* file1)
{
   vector<TTree*> vtrees;
   TFile * tf1 = new TFile(file1);
   vtrees.push_back(findTree(tf1,"HltTree"));

   Color_t colors[4] = {kRed,kBlue,kGreen-2,kBlack};
   Style_t styles[4] = {1,7,3,2};

   vector<TString> jets, cuts;
   jets.push_back("recoJetCalPt");
   jets.push_back("recoJetGenPt");
   cuts.push_back("");
   //cuts.push_back("L1_SingleJet15==1");
   cuts.push_back("HLT_HIJet50U==1");

   compare_one(vtrees,jets,cuts,colors,styles,NETBINS,0,ETMAX);
   makeCanvas("L1_SingleJet15","L1_SingleJet15");
   vtrees[0]->Draw("L1_SingleJet15");

   printAllCanvases();
}
