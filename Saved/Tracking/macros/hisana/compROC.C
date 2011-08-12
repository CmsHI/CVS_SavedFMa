{
  TFile * inf0 = new TFile("../ntout/TMVA_hiOpt1Trk_c0to4_pt50to100.root");
  TFile * inf1 = new TFile("../ntout/TMVA_hiOpt1CalCompTrk_c0to4_pt50to100.root");
  //TFile * inf2 = new TFile("../ntout/TMVA_calocomp_pt20to100.root");
  //TFile * inf3 = new TFile("../ntout/TMVA_hiOpt1Trk_pt50to100.root");
  //TFile * inf4 = new TFile("../ntout/TMVA_hiOpt1CaloCompTrk_pt50to100.root");
  
  TH1F * hRoc0 = (TH1F*)inf0->Get("Method_Cuts/CutsGA/MVA_CutsGA_rejBvsS");
  TH1F * hRoc1 = (TH1F*)inf1->Get("Method_Cuts/CutsGA/MVA_CutsGA_rejBvsS");
  //TH1F * hRoc2 = (TH1F*)inf2->Get("Method_Fisher/Fisher/MVA_Fisher_rejBvsS");
  //TH1F * hRoc3 = (TH1F*)inf3->Get("Method_Cuts/CutsGA/MVA_CutsGA_rejBvsS");
  //TH1F * hRoc4 = (TH1F*)inf4->Get("Method_Cuts/CutsGA/MVA_CutsGA_rejBvsS");
  
  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hRoc0->SetAxisRange(0,1.2,"X");
  hRoc0->SetAxisRange(0,1.2,"Y");
  hRoc0->SetTitle(";Sig Efficiency; Bkg Efficiency;");
  hRoc1->SetLineColor(kRed);
  //hRoc2->SetLineColor(kBlue);
  //hRoc3->SetLineColor(kGreen+2);
  //hRoc4->SetLineColor(kOrange+2);
  
  hRoc0->Draw("c");
  hRoc1->Draw("csame");
  //hRoc2->Draw("csame");
  //hRoc3->Draw("csame");
  //hRoc4->Draw("csame");
  
  //Float_t x[1] = {0.81};
  //Float_t y[1] = {0.98};
  //TGraph * gr = new TGraph(1,x,y);
  //gr->Draw("samep");
                             
  Float_t x2[1] = {0.830935};
  Float_t y2[1] = {0.999762};
  TGraph * gr2 = new TGraph(1,x2,y2);
  gr2->SetMarkerStyle(kOpenCircle);
  gr2->Draw("samep");
  
  TLegend *leg = new TLegend(0.19,0.19,0.58,0.42);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hRoc0,"PYTHIA+HYDJET 0-10%","");
  leg->AddEntry(hRoc0,"TrkQual","l");
  leg->AddEntry(hRoc1,"TrkQual+caloComp","l");
  leg->AddEntry(gr2,"hiGeneralTracks","p");
  leg->Draw();
}