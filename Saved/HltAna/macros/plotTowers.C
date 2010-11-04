#include "aliases_fwlite.C"
void plotTowers(
    TString infilename="../openhlt/output/Hydjet_BSC_HF_v2_700.root",
    TString outdir = "out/Hydj/BSC_HF_L1Emul")
{
  TFile * infile = new TFile(infilename);
  aliases_fwlite(Events);

  gSystem->mkdir(outdir.Data(),kTRUE);
  TCanvas * cCaloTowerNvsP = new TCanvas("CaloTowerNvsP","CaloTowerNvsP",500,500);
  Events->Draw("NHfN:NHfP>>hCt(100,0,100,100,0,100)","","colz");
  hCt->SetTitle("HF Towers;# HF+ Towers (E>3GeV);# HF- Towers (E>3GeV)");
  cCaloTowerNvsP->Print(Form("%s/CaloTowerNvsP.gif",outdir.Data()));
}
