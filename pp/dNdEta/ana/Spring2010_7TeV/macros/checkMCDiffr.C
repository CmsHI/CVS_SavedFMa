void checkMCDiffr()
{
  TFile * inF = new TFile("../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root");
  TCut evtSel = "((!L1T[36] && !L1T[37] && !L1T[38] && !L1T[39]))&&(L1T[34])&&vz[1]<15&&vz[1]>-15";

  TCanvas * cSDvsNSD = new TCanvas("cSDvsNSD","cSDvsNSD",500,500);
  cSDvsNSD->SetLogy();
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosAll(60,0,200)",evtSel);
  hSumEaddEpPosAll->SetTitle(";E+Pz (+z);");
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosSD(60,0,200)",evtSel && "(evtType==92||evtType==93)","same");
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosNSD(60,0,200)",evtSel && "!(evtType==92||evtType==93)","same");
  hSumEaddEpPosSD->SetLineColor(kBlue);
  hSumEaddEpPosNSD->SetLineColor(kRed+2);
  TLegend *leg2 = new TLegend(0.595,0.8,0.845,0.927,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.03);
  leg2->AddEntry("","SD vs NSD","");
  leg2->Draw();
  cSDvsNSD->Print("cSDvsNSD.gif");


  TCanvas * cDDvsNDD = new TCanvas("cDDvsNDD","DDvsNDD",500,500);
  hSumEaddEpPosAll->Draw();
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosDD(60,0,200)",evtSel && "(evtType==94)","same");
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosNDD(60,0,200)",evtSel && "(evtType!=94)","same");
  hSumEaddEpPosDD->SetLineColor(kBlue);
  hSumEaddEpPosNDD->SetLineColor(kRed+2);

  TCanvas * cDFvsND = new TCanvas("cDFvsND","DFvsND",500,500);
  cDFvsND->SetLogy();
  hSumEaddEpPosAll->Draw();
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosDF(60,0,200)",evtSel && "(evtType==92||evtType==93||evtType==94)","same");
  PixelTree->Draw("SumEaddEpPos>>hSumEaddEpPosND(60,0,200)",evtSel && "!(evtType==92||evtType==93||evtType==94)","same");
  hSumEaddEpPosDF->SetLineColor(kBlue);
  hSumEaddEpPosND->SetLineColor(kRed+2);
  TLegend *leg3 = new TLegend(0.595,0.8,0.845,0.927,NULL,"brNDC");
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);
  leg3->SetTextSize(0.03);
  leg3->AddEntry("","DF vs ND","");
  leg3->Draw();
  cDFvsND->Print("cDFvsND.gif");
}
