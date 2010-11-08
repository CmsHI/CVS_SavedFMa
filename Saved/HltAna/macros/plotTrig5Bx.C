TChain * plotTrig5Bx(Int_t runNum=150308,
    TString trig = "L1_BscMinBiasThreshold1_BptxAND"
    )
{
  TChain * oh = new TChain("hltanalysis/HltTree");
  oh->Add(Form("/d101/frankma/data/HIExpress/HR10Exp3/r%d/hltana_*.root",runNum));
  cout << "oh: # events " << oh->GetEntries() << endl;
  oh->Draw("LumiBlock>>hLumi",trig,"goff");
  oh->Draw("LumiBlock>>hLumi5Bx",trig+"_5bx","goff");
  hLumi5Bx->SetXTitle("Lumi Section");
  hLumi5Bx->SetYTitle("# Events");
  hLumi->SetLineColor(2);
  hLumi5Bx->SetMinimum(0.1);
  TCanvas * cLumi = new TCanvas("cLumi","cLumi",500,500);
  cLumi->SetLogy();
  hLumi5Bx->Draw();
  hLumi->Draw("same");
  TLegend *l= new TLegend(0.185,0.17,0.6,0.35);;
  l->SetHeader(Form("Run %d",runNum));;
  l->AddEntry(hLumi,"bx 0","L");;
  l->AddEntry(hLumi5Bx,"bx 0#pm2","L");;
  //l->SetFillStyle(0);;
  l->SetFillColor(0);;
  l->Draw();

  return oh;
}
