#include "TChain.h"
#include <iostream>
using namespace std;

void printLumi(TString infile)
{
  TChain * t = new TChain("djcalo/djTree");
  t->Add(infile);

  Int_t run,lumi;

  t->SetBranchAddress("run",&run);
  t->SetBranchAddress("lumi",&lumi);
  //t->Scan("run:lumi");

  Int_t numEvt = t->GetEntries();
  cout << "NEntries: " << numEvt << endl;
  for (Int_t i=0; i<numEvt; ++i) {
    t->GetEntry(i);
    cout << "Run/Lumi " << run << " " << lumi << endl;
  }
}
