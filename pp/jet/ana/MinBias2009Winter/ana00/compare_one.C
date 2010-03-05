#include <iostream>
#include "compareHist.h"
#include "selectionCut.h"
using namespace std;

void compare_one()
{
  char * inf1 = "data_v21_all.root";
  char * inf2 = "mc_v21_1M.root";

  TFile * infile1 = new TFile(inf1);
  TTree * tree1 = (TTree*)infile1->FindObjectAny("dijetTree");

  TFile * infile2 = new TFile(inf2);
  TTree * tree2 = (TTree*)infile2->FindObjectAny("dijetTree");

  selectionCut dataAna(0,3,10,3,2.14);
  TString djCut = TString(dataAna.DJCut);

  cout << "====== total input =====" << endl;
  Double_t totn1 = tree1->GetEntries();
  Double_t totn2 = tree2->GetEntries();
  cout << " tot events in " << inf1 << ": " << totn1 << endl;
  cout << " tot events in " << inf2 << ": " << totn2 << endl;
  cout << "====== selection ===== " << endl;
  cout << "Selection: " << djCut << endl;
  cout << " # passed cuts in " << inf1 << ": " << tree1->GetEntries(djCut) << endl;
  cout << " # passed cuts in " << inf2 << ": " << tree2->GetEntries(djCut) << endl;

  TCanvas *ccompemf = new TCanvas("ccompemf","",500,500);
  //tree1->Draw("nljemf",djCut);
  compareHist compemf(tree1,tree2,"nljemf","djJEmf",djCut.Data(),djCut.Data(),-0.1,1.1,20);
  compemf.AppendToHist(tree1,tree2,"aljemf","djJEmf",djCut.Data(),djCut.Data());
  compemf.Normalize(1);
  compemf.SetHistName1("data");
  compemf.SetHistName2("mc");
  compemf.SetXTitle("jet^{1,2} EMF");
  compemf.SetYTitle("Arbitrary normalization");
  compemf.SetLegend(0.632,0.828,0.927,0.926);
  compemf.SetMinimum(0);
  compemf.Draw("E");
}

