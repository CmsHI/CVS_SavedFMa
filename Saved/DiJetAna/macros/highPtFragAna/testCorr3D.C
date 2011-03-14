#include "TCanvas.h"
#include "TStyle.h"
#include "Corrector3D.h"

void testCorr3D(Int_t corrLevel=0,
    TString mod="B0",
    Int_t isample=3, // -1 for all samples
    Int_t etaPM=2 // +/- 2 for |eta|<1
    )
{
  Int_t mode=1; // 0 for write, 1 for read
  Int_t cbin=0;


  Corrector3D trkCorr(mod);
  trkCorr.Init(1,"TrkCorr2D.root");

  trkCorr.sampleMode_ = 0; // 0 for choosing individual sample, 1 for merge samples
  Double_t corr[4];
  cout << trkCorr.GetCorr(10,0,110,0,corr) << endl;

  // Plot 2D Corr.
  gStyle->SetPadRightMargin(0.15);

  TCanvas * c2 = new TCanvas("c2","c2",1300,500);
  c2->Divide(3,1);
  c2->cd(1);
  TH2D *hCorr2D = (TH2D*)trkCorr.InspectCorr(corrLevel,isample,cbin,4,30,0);
  hCorr2D->Draw("colz");
  c2->cd(2);
  TH1D * hCorrPt = (TH1D*) trkCorr.InspectCorr(corrLevel,isample,cbin,4,30,2,7-etaPM,7+etaPM);
  hCorrPt->Draw("histE");
  c2->cd(3);
  TH1D * hCorrEta = (TH1D*)trkCorr.InspectCorr(corrLevel,isample,cbin,4,30,1);
  hCorrEta->Draw("histE");
}
