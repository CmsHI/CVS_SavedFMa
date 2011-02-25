#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector.h"

void testCorr()
{
  Corrector trkCorr;
  trkCorr.Init();

  Double_t corr[4];
  cout << trkCorr.GetCorr(10,0,110,0,corr) << endl;

  // Plot 2D Corr.
  gStyle->SetPadRightMargin(0.15);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  trkCorr.InspectCorr(0,3,0,110);

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  trkCorr.InspectCorr(0,-1,0,110);

  TFile * fout = new TFile("TrkCorr2D.root","RECREATE");
  trkCorr.Write();
  fout->Close();
}
