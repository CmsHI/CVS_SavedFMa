#include "compareHist.h"
#include "selectionCut.h"
#include <iostream>
#include <TFile.h>
#include <TCanvas.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
using namespace std;

void count_evt_sel(char * inf1="data_v21_all.root")
{
  TFile * infile1 = new TFile(inf1);
  TTree * tree1 = (TTree*)infile1->FindObjectAny("dijetTree");

  cout << "====== total input =====" << endl;
  Double_t totn1 = tree1->GetEntries();
  cout << " tot events in " << inf1 << ": " << totn1 << endl;

  double etamax=0;
  for (int i=0; i<2; ++i) {
    if (i==0) etamax=3;
    if (i==1) etamax=2;
    cout << endl << "=== |eta|<" << etamax << " ===" << endl;
    selectionCut dataAnaSel0(0,0,10,etamax,2.14);
    selectionCut dataAnaSel1(0,1,10,etamax,2.14);
    selectionCut dataAnaSel2(0,2,10,etamax,2.14);
    selectionCut dataAnaSel3(0,3,10,etamax,2.14);
    selectionCut dataAnaSel4(0,4,10,etamax,2.14);

    cout << "Selection: " << TString(dataAnaSel0.DJCut) << endl;
    cout << " # passed cuts: " << tree1->GetEntries(dataAnaSel0.DJCut) << endl;;
    cout << "Selection: " << TString(dataAnaSel1.DJCut) << endl;
    cout << " # passed cuts: " << tree1->GetEntries(dataAnaSel1.DJCut) << endl;;
    cout << "Selection: " << TString(dataAnaSel2.DJCut) << endl;
    cout << " # passed cuts: " << tree1->GetEntries(dataAnaSel2.DJCut) << endl;;
    cout << "Selection: " << TString(dataAnaSel3.DJCut) << endl;
    cout << " # passed cuts: " << tree1->GetEntries(dataAnaSel3.DJCut) << endl;;
    cout << "Selection: " << TString(dataAnaSel4.DJCut) << endl;
    cout << " # passed cuts: " << tree1->GetEntries(dataAnaSel4.DJCut) << endl;;
  }
}

