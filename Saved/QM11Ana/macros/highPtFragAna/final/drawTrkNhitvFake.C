void drawTrkNhitvFake()
{
	TH1::SetDefaultSumw2();
  TChain * tmc = new TChain("PFJetAnalyzer/t");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set1-v1.root");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set2-v1.root");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set3-v1.root");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set4-v1.root");
	
	TCut cut = "jtpt_j4[0]>80&&abs(jteta_j4[0])<2&&trackpt>4";
	
	TH1D * hNHit_rec = new TH1D("hNHit_rec",";# valid hits;# tracks",30,0,30);
	TH1D * hNHit_fak = new TH1D("hNHit_fak",";# valid hits;# tracks",30,0,30);
	
	tmc->Project("hNHit_rec","tracknhits",cut);
	tmc->Project("hNHit_fak","tracknhits",cut&&"trackfake");
	
	TCanvas * c2 = new TCanvas("c2","c2",500,500);
	hNHit_rec->SetLineColor(kRed);
	hNHit_fak->SetMarkerStyle(kOpenSquare);
	hNHit_rec->Draw("hist");
	hNHit_fak->Draw("psame");
	
	TLegend *leg = new TLegend(0.2,0.72,0.5,0.92);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hNHit_rec,"MC, #hat{p}_{T} 80 GeV/c","");
  leg->AddEntry(hNHit_rec,"track p_{T} > 4 GeV/c","");
  leg->AddEntry(hNHit_rec,"All Rec. Tracks","l");
  leg->AddEntry(hNHit_fak,"Fake Tracks","p");
	leg->Draw();
}