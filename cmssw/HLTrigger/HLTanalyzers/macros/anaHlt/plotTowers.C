#include "aliases_fwlite.C"
void plotTowers()
{
  TFile * infile = new TFile("../openhlt/output/Hydjet_BSC_HF_v2_100.root");
  aliases_fwlite(Events);

  TCanvas * cCaloTowerNvsP = new TCanvas("CaloTowerNvsP","CaloTowerNvsP",500,500);
  Events->Draw("NHfN:NHfP>>hCt(100,0,100,100,0,100)","","box");
}
