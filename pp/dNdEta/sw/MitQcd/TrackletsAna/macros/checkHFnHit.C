{
  TFile * fin = new TFile("../input/pixelTree_124022a3a4-vtxcomp_MB.root");
  TCut dataSel("vz[1]<20&&vz[1]>-20&&(L1T[34]>0&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1&&nLumi>=41&&nLumi<=96&&L1A[0]==1&&L1A[82]==1)&&(nRun==124023)");

  Int_t NBINS=30;
  TH2D * hPM = new TH2D("hPM","pos vs neg;# hit HF- towers;# hit HF+ towers",NBINS,0,NBINS,NBINS,0,NBINS);

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  PixelTree->Draw("nHFp:nHFn>>hPM",dataSel,"colz");

  TLine * l1 = new TLine(1,1,NBINS,1);
  l1->SetLineColor(kRed);
  l1->SetLineWidth(3);
  l1->SetLineStyle(7);
  TLine * l2 = new TLine(1,1,1,NBINS);
  l2->SetLineColor(kRed);
  l2->SetLineWidth(3);
  l2->SetLineStyle(7);
  l1->Draw();
  l2->Draw();

  c2->Print("plots/inspection/nTowerCount.gif");
}
