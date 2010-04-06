#include "aliases.C"
void checkMCDiffr()
{
  TFile * inF = new TFile("../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root");
  TCut evtSel = "((!L1T[36] && !L1T[37] && !L1T[38] && !L1T[39]))&&(L1T[34])&&vz[1]<15&&vz[1]>-15";

  aliases(PixelTree);
  PixelTree->SetAlias("Obs","MinEPz");
  //TString title(";E+Pz (+z);");
  TString title(";Min(E+Pz,E-Pz);");
  TString outfn("plots/inspect/mc/cMinEPz");

  TCanvas * cSDvsNSD = new TCanvas("cSDvsNSD","cSDvsNSD",500,500);
  cSDvsNSD->SetLogy();
  PixelTree->Draw("Obs>>hObsAll(60,0,200)",evtSel);
  hObsAll->SetTitle(title);
  PixelTree->Draw("Obs>>hObsSD(60,0,200)",evtSel && "(evtType==92||evtType==93)","same");
  PixelTree->Draw("Obs>>hObsNSD(60,0,200)",evtSel && "!(evtType==92||evtType==93)","same");
  hObsSD->SetLineColor(kBlue);
  hObsNSD->SetLineColor(kRed+2);
  TLegend *leg2 = new TLegend(0.595,0.8,0.845,0.927,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.03);
  leg2->AddEntry("","SD vs NSD","");
  leg2->Draw();
  cSDvsNSD->Print(outfn+"SDvsNSD.gif");


  TCanvas * cDDvsNDD = new TCanvas("cDDvsNDD","DDvsNDD",500,500);
  hObsAll->Draw();
  PixelTree->Draw("Obs>>hObsDD(60,0,200)",evtSel && "(evtType==94)","same");
  PixelTree->Draw("Obs>>hObsNDD(60,0,200)",evtSel && "(evtType!=94)","same");
  hObsDD->SetLineColor(kBlue);
  hObsNDD->SetLineColor(kRed+2);

  TCanvas * cDFvsND = new TCanvas("cDFvsND","DFvsND",500,500);
  cDFvsND->SetLogy();
  hObsAll->Draw();
  PixelTree->Draw("Obs>>hObsDF(60,0,200)",evtSel && "(evtType==92||evtType==93||evtType==94)","same");
  PixelTree->Draw("Obs>>hObsND(60,0,200)",evtSel && "!(evtType==92||evtType==93||evtType==94)","same");
  hObsDF->SetLineColor(kBlue);
  hObsND->SetLineColor(kRed+2);
  TLegend *leg3 = new TLegend(0.595,0.8,0.845,0.927,NULL,"brNDC");
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);
  leg3->SetTextSize(0.03);
  leg3->AddEntry("","DF vs ND","");
  leg3->Draw();
  cDFvsND->Print(outfn+"DFvsND.gif");
}
