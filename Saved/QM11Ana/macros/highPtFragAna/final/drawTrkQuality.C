void drawQual(TFile * inf, TCanvas * c, Int_t ic, Float_t ymin, Float_t ymax, Int_t doLog)
{
  c->Divide(3,1);
  c->cd(1);
  if (doLog==1) gPad->SetLogy();
  TH1D * hQual_precut_mc = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPreCut_mc80",ic));
  TH1D * hQual_precut_data = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPreCut_dataj35",ic));
  TH1D * hQual_mc = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPostCut_mc80",ic));
  TH1D * hQual_data = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPostCut_dataj35",ic));
  hQual_precut_mc->Scale(1./hQual_precut_mc->GetEntries());
  hQual_precut_data->Scale(1./hQual_precut_data->GetEntries());
  hQual_mc->Scale(1./hQual_precut_mc->GetEntries());
  hQual_data->Scale(1./hQual_precut_data->GetEntries());

  hQual_precut_mc->SetAxisRange(ymin,ymax,"Y");
  hQual_precut_mc->SetMarkerStyle(kOpenCircle);
  hQual_precut_mc->SetMarkerColor(2);
  hQual_precut_mc->SetLineColor(2);
  hQual_mc->SetLineColor(2);
  hQual_mc->SetLineStyle(2);
  hQual_data->SetLineStyle(2);
  hQual_precut_mc->Draw("E");
  hQual_precut_data->Draw("sameE");
  hQual_mc->Draw("same hist");
  hQual_data->Draw("same hist");

  c->cd(2);
  gPad->SetLogy();
  TH1D * hPPt_precut_mc = inf->FindObjectAny(Form("hPPt_cut%d_mc80",ic-1));
  TH1D * hPPt_precut_data = inf->FindObjectAny(Form("hPPt_cut%d_dataj35",ic-1));
  TH1D * hPPt_mc = inf->FindObjectAny(Form("hPPt_cut%d_mc80",ic));
  TH1D * hPPt_data = inf->FindObjectAny(Form("hPPt_cut%d_dataj35",ic));

  hPPt_precut_mc->SetAxisRange(1e-4,1e1,"Y");
  hPPt_precut_mc->SetMarkerStyle(kOpenCircle);
  hPPt_precut_mc->SetMarkerColor(2);
  hPPt_precut_mc->SetLineColor(2);
  hPPt_mc->SetLineColor(2);
  hPPt_mc->SetLineStyle(2);
  hPPt_data->SetLineStyle(2);
  hPPt_precut_mc->Draw("E");
  hPPt_precut_data->Draw("sameE");
  hPPt_mc->Draw("same hist");
  hPPt_data->Draw("same hist");

  c->cd(3);
  TH1D * hRatMc = hPPt_mc->Clone(Form("%s_rat",hPPt_mc->GetName()));
  hRatMc->Divide(hPPt_precut_mc);
  TH1D * hRatData = hPPt_data->Clone(Form("%s_rat",hPPt_data->GetName()));
  hRatData->Divide(hPPt_precut_data);
  hRatMc->SetAxisRange(0,1.1,"Y");
  hRatMc->SetMarkerStyle(kOpenCircle);
  hRatMc->SetMarkerColor(2);
  hRatMc->SetLineColor(2);
  hRatData->SetLineStyle(1);
  hRatMc->Draw("E");
  hRatData->Draw("sameE");
}

void drawTrkQuality(
    TString infname="trkqualhists_dataj35_mc80.root"
    )
{
  TFile * inf = new TFile(infname);

  TCanvas * c1 = new TCanvas("c1","c1",1200,400);
  drawQual(inf,c1,1,0,0.3,0);

  TCanvas * c2 = new TCanvas("c2","c2",1200,400);
  drawQual(inf,c2,2,0,0.3,0);

  TCanvas * c3 = new TCanvas("c3","c3",1200,400);
  drawQual(inf,c3,3,1e-3,1,1);

  TCanvas * c4 = new TCanvas("c4","c4",1200,400);
  drawQual(inf,c4,4,1e-4,1,1);

  TCanvas * c5 = new TCanvas("c5","c5",1200,400);
  drawQual(inf,c5,5,0,0.3,0);
}
