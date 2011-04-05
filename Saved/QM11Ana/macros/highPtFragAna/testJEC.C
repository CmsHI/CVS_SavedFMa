#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "TROOT.h"
#include "AnaJEC.h"
using namespace std;

void testJEC(Float_t pt=110, Float_t eta=0)
{
  AnaJEC anajec;
  anajec.Init();
  cout<<" corr "<<anajec.GetJEC(pt,eta) <<endl;

}
