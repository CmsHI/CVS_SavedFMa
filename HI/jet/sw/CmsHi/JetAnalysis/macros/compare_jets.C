#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "/home/frankma/UserCode/SavedFMa/HI/analysis/dijet/sw/hiroot/macros/ana/savedfrankTools.C"

using namespace std;
const char * dr1="hist";
const char * dr2="hist same";
Int_t NETBINS = 100;
Float_t ETMAX = 100;

const char * etcut = "";


void compare_one(const vector<TTree*> & vf, char* var="et",Color_t* colors=0,Double_t xmin=0,Double_t xmax=0)
{

   TString hn1(Form("%s1",var));
   TString hn2(Form("%s2",var));

   for (int i=0; i<vf.size(); ++i) {
      //vf[i]->Print();
      TString opt;
      if (i==0)
	 opt=dr1;
      else
	 opt=dr2;

      TString hname(Form("%s%d",var,i));
      TString htitle(Form("%s%d;%s;",var,i,var));

      printf("color %d: %d\n", i, colors[i]);

      drawTree(vf[i],var,etcut,opt.Data(),hname.Data(),htitle.Data(),NETBINS,xmin,xmax,1,colors[i],0,0,0,0);
   }
}

void compare_jets(char* file1, char* file2)
{
   vector<TTree*> vtrees;
   TFile * tf1 = new TFile(file1);
   vtrees.push_back(findTree(tf1,"genevent/hi"));
   TFile * tf2 = new TFile(file2);
   vtrees.push_back(findTree(tf2,"genevent/hi"));

   Color_t colors[4] = {kRed,kBlue,kGreen-2,kBlack};
   compare_one(vtrees,"et",colors,0,ETMAX);
   compare_one(vtrees,"eta",colors,-5,5);
   compare_one(vtrees,"phi",colors,-3.14,3.14);

   printAllCanvases();
}
