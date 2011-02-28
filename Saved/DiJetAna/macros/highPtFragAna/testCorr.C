#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector.h"

void testCorr(Int_t mode=0 // 0 for write, 1 for read
    )
{
  Corrector trkCorr;
  if (mode==0) {
    trkCorr.ptRebinFactor_ = 1;
    trkCorr.Init();
  } else if (mode==1) {
    trkCorr.ptRebinFactor_ = 12;
    trkCorr.Init(1,"TrkCorr2D.root");
  }

  trkCorr.sampleMode_ = 0; // 0 for choosing individual sample, 1 for merge samples
  Double_t corr[4];
  cout << trkCorr.GetCorr(10,0,110,0,corr) << endl;

  // Plot 2D Corr.
  gStyle->SetPadRightMargin(0.15);

  TCanvas * c2 = new TCanvas("c2","c2",1300,500);
  c2->Divide(3,1);
  c2->cd(1);
  trkCorr.InspectCorr(0,3,0,110,0);
  c2->cd(2);
  trkCorr.InspectCorr(0,3,0,110,2);
  c2->cd(3);
  trkCorr.InspectCorr(0,3,0,110,1);

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  trkCorr.InspectCorr(0,-1,0,110);

  if (mode==0) {
    TFile * fout = new TFile("TrkCorr2D.root","RECREATE");
    trkCorr.Write();
    fout->Close();
  }
}
