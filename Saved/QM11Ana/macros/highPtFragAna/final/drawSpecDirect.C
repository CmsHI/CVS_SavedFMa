#include "../../HisMath.C"

void drawSpecDirect()
{
  TChain * tana = new TChain("tjf");
  TChain * tref = new TChain("tjf");

  tref->Add("../ntout/tranav12_dataj35pfhgtv1repass2_j4_j2t2_et90.root");
  tana->Add("../ntout/tranav12_dataj50corepfhgtv1repass2_j4_j2t2_et90.root");

  TCut evtCut = "cbin>=0&&cbin<2&&jtpt[0]>100&&abs(jteta[0])<2";

  TFile * inf_ptbin = new TFile("../specRef/toFrank/CORRv1_hcpr_spectra_v4_J50U_all_finec_sub1_HCPR_apr042011_eta_0.0to1.0_jet_0.0to1000.0_hitrackAna_jetMode1_GEN0_reb2_cbin0to1.root");
  TH1D * hxbin = (TH1D*)inf_ptbin->Get("corrTypeOne/hdndpt_full");
  vector<Double_t> ptBin;
  for (Int_t i=1; i<=hxbin->GetNbinsX()+1; i+=2) {
    ptBin.push_back(hxbin->GetBinLowEdge(i));
  }

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  gPad->SetLogy();
  Float_t numEvt_ref = tref->Draw("jtpt[0]",evtCut,"hist");
  cout << "ref: " << numEvt_ref << endl;
  Float_t numEvt_ana = tana->Draw("jtpt[0]",evtCut,"sameE");
  cout << "ana: " << numEvt_ana << endl;

  TH1D * hSpec_ana = new TH1D("hSpec_ana",";p_{T} GeV/c;",ptBin.size()-1,&ptBin[0]);
  TH1D * hSpec_ref = new TH1D("hSpec_ref",";p_{T} GeV/c;",ptBin.size()-1,&ptBin[0]);
  tana->Project("hSpec_ana","ppt",evtCut&&"abs(peta)<1");
  tref->Project("hSpec_ref","ppt",evtCut&&"abs(peta)<1");
  hSpec_ana->Sumw2();
  hSpec_ref->Sumw2();
  normHist(hSpec_ana,0,true,1./numEvt_ana);
  normHist(hSpec_ref,0,true,1./numEvt_ref);
  TH1D * hSpec_rat = (TH1D*)hSpec_ana->Clone("hSpec_rat");
  hSpec_rat->Divide(hSpec_ref);

  TCanvas * c3 = new TCanvas("c3","c3",900,400);
  c3->Divide(2,1);
  c3->cd(1);
  gPad->SetLogy();
  hSpec_ref->SetAxisRange(0,103,"X");
  hSpec_ref->Draw("hist");
  hSpec_ana->Draw("sameE");
  TLegend *leg0 = new TLegend(0.29,0.62,0.68,0.90);
  leg0->SetFillStyle(0);
  leg0->SetBorderSize(0);
  leg0->SetTextSize(0.035);
  leg0->AddEntry(hSpec_ana,"0-5%","");
  leg0->AddEntry(hSpec_ana,"hiGoodTight","");
  leg0->AddEntry(hSpec_ana,"No B.S Constrain","");
  leg0->AddEntry(hSpec_ana,"Leading Jet p_{T} > 100 GeV/c, |#eta|<2","");
  leg0->AddEntry(hSpec_ana,"HICore Jet50U","p");
  leg0->AddEntry(hSpec_ref,"HIAll Jet35U","l");
  leg0->Draw();

  c3->cd(2);
  hSpec_rat->SetAxisRange(0,103,"X");
  hSpec_rat->Draw();
  hSpec_rat->Fit("pol0","","",4,100);
  TLegend *leg = new TLegend(0.29,0.74,0.68,0.90);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hSpec_rat,"No B.S Constrain","");
  leg->AddEntry(hSpec_rat,"CorePhy/AllPhy","p");
  leg->Draw();

  TFile * outf = new TFile("RawSpecDirect.root","RECREATE");
  hSpec_ana->Write();
  hSpec_ref->Write();
}
