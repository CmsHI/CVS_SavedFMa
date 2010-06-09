#include <iostream>
#include "HisTGroup.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TCanvas.h"
using namespace std;

void hisTGroupDriver()
{
  HisTGroup<TH1D> hgDj1("Dj1");
  cout << "group: " << hgDj1.name_ << endl;
  cout << "nbins: " << hgDj1.nbins_ << " xmin: " << hgDj1.xmin_ << " xmax: " << hgDj1.xmax_ << " " << hgDj1.hSum_ << " " << hgDj1.hAve_ << endl;

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

  HisTGroup<TProfile> hgDj0("Dj0",10,0,10);
  cout << "nbins: " << hgDj0.nbins_ << " xmin: " << hgDj0.xmin_ << " xmax: " << hgDj0.xmax_ << " " << hgDj0.hSum_ << " " << hgDj0.hAve_ << endl;
  hgDj0.Add("Nr");

  TCanvas * c4 = new TCanvas("c4","c4",500,500);
  hgDj0.hm_["Nr"]->Draw();
}
