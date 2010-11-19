void compRuns()
{
  TChain * oh305 = new TChain("hltanalysis/HltTree");
  oh305->Add("~/scratch02/data/HIAllPhysics/HR10AllPR2/r150305/hltana_*.root");
  TChain * oh476 = new TChain("hltanalysis/HltTree");
  oh476->Add("~/scratch02/data/HIAllPhysics/HR10AllPR2/r150476v2/hltana_*.root");

  TH1D * hLumi305 = new TH1D("hLumi305",";Lumi Sec.;Evts/Lumi",1600,0,1600);
  TH1D * hLumi376 = new TH1D("hLumi476",";Lumi Sec.;Evts/Lumi",1600,0,1600);
  TH1D * hBptx305 = new TH1D("hBptx305",";L1Tech_BPTX_plus_AND_minus.v0_5bx;unit norm.",17,0,17);
  TH1D * hBptx376 = new TH1D("hBptx476",";L1Tech_BPTX_plus_AND_minus.v0_5bx;unit norm.",17,0,17);

  TCanvas * cLumi = new TCanvas("cLumi","cLumi",500,500);
  cLumi->SetLogy();
  oh476->Draw("LumiBlock>>hLumi476","L1_BscMinBiasInnerThreshold1_BptxAND");
  oh305->Draw("LumiBlock>>hLumi305","L1_BscMinBiasInnerThreshold1_BptxAND","same");
  hLumi476->SetMinimum(1);
  hLumi305->SetMinimum(1);
  hLumi476->SetLineColor(2);
  hLumi305->SetLineColor(4);
  hLumi476->Draw();
  hLumi305->Draw("same");
  TLegend *l0= new TLegend(0.51,0.26,0.85,0.44);;
  l0->SetHeader("HIAllPhysics");
  l0->AddEntry(hLumi305,"Run 150305","l");;
  l0->AddEntry(hLumi476,"Run 150476","l");;
  l0->SetFillStyle(0);;
  l0->SetLineColor(0);
  l0->SetTextSize(0.03);
  l0->Draw();

  TCanvas * cBptx = new TCanvas("cBptx","cBptx",500,500);
  oh305->Draw("L1Tech_BPTX_plus_AND_minus.v0_5bx>>hBptx305","L1Tech_BPTX_plus_AND_minus.v0_5bx");
  oh476->Draw("L1Tech_BPTX_plus_AND_minus.v0_5bx>>hBptx476","L1Tech_BPTX_plus_AND_minus.v0_5bx");
  cBptx->SetLogy();
  hBptx305->Scale(1./hBptx305->GetEntries());
  hBptx476->Scale(1./hBptx476->GetEntries());
  hBptx305->SetMarkerStyle(kOpenSquare);
  hBptx305->SetMarkerStyle(kBlue);
  hBptx305->SetMarkerStyle(kOpenSquare);
  hBptx305->SetMarkerColor(kBlue);
  hBptx476->SetMarkerColor(kRed);
  hBptx305->Draw("E");
  hBptx476->Draw("sameE");

  TLegend *l= new TLegend(0.53,0.74,0.85,0.92);;
  l->SetHeader("HIAllPhysics");
  l->AddEntry(hBptx305,"Run 150305","pl");;
  l->AddEntry(hBptx476,"Run 150476","pl");;
  l->SetFillStyle(0);;
  l->SetLineColor(0);
  l->SetTextSize(0.03);
  l->Draw();
}
