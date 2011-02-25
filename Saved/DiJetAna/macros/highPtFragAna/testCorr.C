#include "TCanvas.h"
#include "Corrector.h"

void testCorr()
{
  Corrector trkCorr;
  trkCorr.Init();

  Double_t corr[4];
  cout << trkCorr.GetCorr(10,0,110,0,corr) << endl;

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  c2->SetRightMargin(0.15);
  trkCorr.InspectCorr(0,3,0,110);

  TFile * fout = new TFile("TrkCorr2D.root","RECREATE");
  trkCorr.Write();
  fout->Close();
}
