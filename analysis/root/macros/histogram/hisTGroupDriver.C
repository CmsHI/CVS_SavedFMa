#include <iostream>
#include "HisTGroup.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TCanvas.h"
using namespace std;

void hisTGroupDriver()
{
  HisTGroup<TH1D> hgDj1("Dj1");
  hgDj1.Print();

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
  hgDj1.Print();

  HisTGroup<TProfile> hgDj0("Dj0",10,0,10);
  hgDj0.Print();
  hgDj0.Add1D("Nr");

  TCanvas * c4 = new TCanvas("c4","c4",500,500);
  hgDj0.hm_["Nr"]->Draw();

  // Test TH2D
  HisTGroup<TH2D> hgDj2("Dj2",10,0,10,10,0,10);
  hgDj2.Print();
  hgDj2.Add2D("NrAw");
  TCanvas * c5 = new TCanvas("c5","c5",500,500);
  hgDj2.hm_["NrAw"]->Fill(1,1);
  hgDj2.hm_["NrAw"]->Draw("colz");
}
