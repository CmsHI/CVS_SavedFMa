#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector.h"

void testCorr(Int_t corrLevel=0,
    Int_t isample=3, // -1 for all samples
    Float_t jet=110, // -1 for all jet bins
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  Int_t mode=0; // 0 for write, 1 for read
  TString outname("TrkCorr2DB0.root");
  Int_t cbin=0;


  Corrector trkCorr("B0");
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
  trkCorr.InspectCorr(corrLevel,isample,cbin,jet,0);
  c2->cd(2);
  trkCorr.InspectCorr(corrLevel,isample,cbin,jet,2,7-etaPM,7+etaPM);
  c2->cd(3);
  trkCorr.InspectCorr(corrLevel,isample,cbin,jet,1);

  TCanvas * c3 = new TCanvas("c3","c3",500,500);
  trkCorr.InspectCorr(0,isample,cbin,jet,2,7-etaPM,7+etaPM,"Ehist");
  trkCorr.InspectCorr(1,isample,cbin,jet,2,7-etaPM,7+etaPM,"Ehistsame");
  c3->Print("TrkCorrInspect.gif");

  if (mode==0) {
    TFile * fout = new TFile(outname,"RECREATE");
    trkCorr.Write();
    fout->Close();
  }
}
