{
  TFile * fin = new TFile("PixelTree-124022-hfcuts.root");
  TH1D * hAll = new TH1D("hAll","All",100,-20,20);
  TH1D * hBx51 = new TH1D("hBx51","Bx51",100,-20,20);
  hBx51->SetLineColor(kBlue);
  hBx51->SetMarkerColor(kBlue);
  TH1D * hBxRest = new TH1D("hBxRest","Rest",100,-20,20);
  hBxRest->SetLineColor(kRed);
  hBxRest->SetMarkerColor(kRed);

  PixelTree->Draw("vz[1]>>hAll","vz[1]<20&&vz[1]>-20&&(nHFp>=1&&nHFn>=1&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1&&nLumi>=41&&nLumi<=96&&L1A[0]==1&&L1A[82]==1)&&(nRun==124023)","E");
  PixelTree->Draw("vz[1]>>hBx51","vz[1]<20&&vz[1]>-20&&(nHFp>=1&&nHFn>=1&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1&&nLumi>=41&&nLumi<=96&&L1A[0]==1&&L1A[82]==1)&&(nRun==124023)&&(nBX==51)","sameE");
  PixelTree->Draw("vz[1]>>hBxRest","vz[1]<20&&vz[1]>-20&&(nHFp>=1&&nHFn>=1&&L1T[36]!=1&&L1T[37]!=1&&L1T[38]!=1&&L1T[39]!=1&&nLumi>=41&&nLumi<=96&&L1A[0]==1&&L1A[82]==1)&&(nRun==124023)&&(nBX!=51)","same Ep");

  TCanvas * c0 = new TCanvas("c0","c0",500,500);
  hBx51->Fit("gaus");
  hBx51->Draw("E");

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hAll->Draw("E");
  hBxRest->Draw("Esame");
  hBx51->Draw("Esame");
}
