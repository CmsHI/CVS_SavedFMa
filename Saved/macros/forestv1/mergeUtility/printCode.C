//example of script showing how to navigate in a ROOT file
//with sub-directories and read the objects in each sub-directory.
//This example uses the file produced by the tutorial importCode.C
//Author: Rene Brun
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TString.h"
#include "TKey.h"
#include "TROOT.h"
#include "TClass.h"
using namespace std;

Int_t ndirs = 0;
vector<TString> vd;
vector<TString> vt;

void readdir(TDirectory *dir) {
   ndirs++;
   TDirectory *dirsav = gDirectory;
   TIter next(dir->GetListOfKeys());
   TKey *key;
   while ((key = (TKey*)next())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      TString clname = cl->GetName();
      if (clname=="TDirectoryFile") {
//          cout << key->GetName() << endl;      
         dir->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         readdir(subdir);
         dirsav->cd();
      } else {
         if (clname=="TTree") {
            TString tname = key->GetName();
            TString dname = gDirectory->GetName();
            if (vt.size()>0&&dname==vd.back()&&tname==vt.back()) continue;
            vd.push_back(dname); vt.push_back(tname);
         }
      }
   }
}
         
      
void printCode() {
   TFile *f = new TFile("~/hadoopfranklocal/forest/promptskim-hihighpt-hltjet80-pt90-v1/reco_100_1_YqR.root");
   if (f->IsZombie()) {
      printf("File code.root does not exist. Run tutorial importCode.C first\n");
      return;
   }
   printf("Reading file ==> code.root\n");
   printf("File size in bytes       = %lld\n",f->GetEND());
   printf("File compression factor  = %g\n",f->GetCompressionFactor());
   
   readdir(f);

   cout << "==========================" << vd.size() << " Dir ============================" << endl;
   for (int i=0; i<vd.size(); ++i) cout << "\"" << vd[i] << "\"," << endl;
   cout << endl;
   
   cout << "==========================" << vt.size() << " Tree ===========================" << endl;
   for (int i=0; i<vt.size(); ++i) cout << "\"" << vt[i] << "\"," << endl;
}
