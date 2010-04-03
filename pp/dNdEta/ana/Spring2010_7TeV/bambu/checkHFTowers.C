#include "aliases_bambu.C"

void checkHFTowers()
{
  TChain * evColBx = new TChain("Events","coll data");
  evColBx->Add("/d100/data/PromptReco/132440-veryloosecuts_v4/bambu_000_1.root");
  evColBx->Add("/d100/data/PromptReco/132440-veryloosecuts_v4/bambu_000_10.root");

  TChain * evEmpBx = new TChain("Events","empty bx data");
  evEmpBx->Add("/d100/data/PromptReco/132422-veryloosecuts_v2/bambu_000_1*.root");

  TChain * evMc = new TChain("Events","mc");
  evMc->Add("/d100/mc/reco/MinBias/Spring10-START3X_V25_356ReReco-v2/bambu_000_11.root");
  evMc->Add("/d100/mc/reco/MinBias/Spring10-START3X_V25_356ReReco-v2/bambu_000_110.root");
  evMc->Add("/d100/mc/reco/MinBias/Spring10-START3X_V25_356ReReco-v2/bambu_000_111.root");

  TString outDir("plots/inspect");

  aliases_bambu(evColBx);
  aliases_bambu(evEmpBx);
  aliases_bambu(evMc);
  TCut colEvtSel("l1a124&&l1t0&&l1t40");

  int ENBIN = 20;
  TH1D * hHfTECol = new TH1D("hHfTECol","HF Tower E;E^{HF Tower};#",ENBIN,0,ENBIN);
  TH1D * hHfTEEmp = new TH1D("hHfTEEmp","HF Tower E;E^{HF Tower};#",ENBIN,0,ENBIN);
  hHfTEEmp->SetLineColor(kBlue);
  TH1D * hHfTEMc = new TH1D("hHfTEMc","HF Tower E;E^{HF Tower};#",ENBIN,0,ENBIN);
  hHfTEMc->SetLineColor(kRed);
  // get norm
  double empArea = evEmpBx->Draw("ct.E()","abs(ct.Eta())>3 && ct.E()>0 && ct.E()<2");
  double colArea1 = evColBx->Draw("ct.E()",colEvtSel && "abs(ct.Eta())>3 && ct.E()>0 && ct.E()<2");
  double empScale = colArea1/empArea;
  double mcArea = evMc->Draw("ct.E()",Form("abs(ct.Eta())>3 && ct.E()>7 &&ct.E()<%d",ENBIN));
  double colArea2 = evColBx->Draw("ct.E()",colEvtSel && Form("abs(ct.Eta())>3 && ct.E()>7 &&ct.E()<%d",ENBIN));
  double mcScale = colArea2/mcArea;
  // now draw
  evEmpBx->Draw("ct.E()>>hHfTEEmp",Form("abs(ct.Eta())>3 && ct.E()<%d",ENBIN));
  evColBx->Draw("ct.E()>>hHfTECol",colEvtSel && Form("abs(ct.Eta())>3 && ct.E()<%d",ENBIN),"same");
  evMc->Draw("ct.E()>>hHfTEMc",Form("abs(ct.Eta())>3 && ct.E()<%d",ENBIN),"same");

  // scale
  hHfTEEmp->Scale(empScale);
  hHfTEMc->Scale(mcScale);

  // save
  TCanvas * cHFTowerE = new TCanvas("cHFTowerE","cHFTowerE",500,500);
  cHFTowerE->SetLogy();
  hHfTEMc->Draw();
  hHfTEEmp->Draw("same");
  hHfTECol->Draw("Esame");
  TLegend * leg1 = new TLegend(0.522,0.828,0.927,0.926,NULL,"brNDC");
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetTextSize(0.035);
  leg1->AddEntry(hHfTECol,"Run 132440","p");
  leg1->AddEntry(hHfTEEmp,"Empty Bx","l");
  leg1->AddEntry(hHfTEMc,"Pythia D6T","l");
  leg1->Draw();

  gSystem->mkdir(outDir.Data(),kTRUE);
  cHFTowerE->Print(Form("%s/HFTowerE.gif",outDir.Data()));
}
