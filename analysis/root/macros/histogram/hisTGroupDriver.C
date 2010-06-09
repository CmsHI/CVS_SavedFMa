#include <iostream>
#include "HisTGroup.h"
#include "TH1D.h"
#include "TCanvas.h"
using namespace std;

void hisTGroupDriver()
{
  HisTGroup<TH1D> hgDj1("Dj1");
  cout << "group: " << hgDj1.name_ << endl;
  cout << "nbins: " << hgDj1.nbins_ << "xmin: " << hgDj1.xmin_ << " xmax: " << hgDj1.xmax_ << " " << hgDj1.hSum_ << " " << hgDj1.hAve_ << endl;

  // Test add th1
  TH1D * h1 = new TH1D("h1","h1",10,0,10);
  h1->Fill(1);
  h1->Fill(1);
  h1->Fill(5);
  hgDj1.Add(h1,"Nr");
  hgDj1.Add(h1,"NrSmall",0.5);
  
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hgDj1.hm_["Nr"]->Draw();

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  hgDj1.hm_["NrSmall"]->Draw();
}
