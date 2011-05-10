#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawSpecClosure(
    // HI
    //TString infname="histff_tv11_80mattpfgmv2_akpu3pf_j2t2_an0509trk5.root"
    // pp
    TString infname="histff_tv11_dj80mattpfgmv1_ak3pf_j2t2_an0509trk5.root"
    )
{
  TFile * inf = new TFile(infname);
  TString outdir = "out/20110509";
  TString outname = infname;
  outname.ReplaceAll(".root","");

  Bool_t doRebin = true;

  // Histograms
  TH1D * hSpec_ref = (TH1D*)inf->FindObjectAny("hGenSpecCorr0");
  TH1D * hSpec_corr[3], *hSpec_rat[3];
  for (Int_t lv=0; lv<3; ++lv) {
    hSpec_corr[lv] = (TH1D*)inf->FindObjectAny(Form("hRecSpecCorr%d",lv));
  }
  // styles
  hSpec_ref->SetLineColor(kRed);
  hSpec_corr[0]->SetMarkerStyle(kOpenSquare);

  // rebin
  if (doRebin) {
    for (Int_t lv=0; lv<3; ++lv) {
      hSpec_corr[lv]->Rebin(2);
      hSpec_corr[lv]->Scale(0.5);
    }
    hSpec_ref->Rebin(2);
    hSpec_ref->Scale(0.5);
  }

  // ratio
  for (Int_t lv=0; lv<3; ++lv) {
    hSpec_rat[lv] = (TH1D*)hSpec_corr[lv]->Clone(Form("%s_rat",hSpec_corr[lv]->GetName()));
    hSpec_rat[lv]->Divide(hSpec_ref);
  }

  // draw
  TCanvas * c2 = new TCanvas("c2","c2",500,900);
  c2->Divide(1,2);
  c2->cd(1);
  gPad->SetLogy();
  hSpec_ref->SetTitle(";Trk p_{T};dN/dp_{T}");
  hSpec_ref->SetAxisRange(0,100,"X");
  hSpec_ref->SetAxisRange(2e-5,5e1,"Y");
  handsomeTH1(hSpec_ref);
  hSpec_ref->Draw("hist");
  hSpec_corr[0]->Draw("sameE");
  hSpec_corr[2]->Draw("sameE");
  c2->cd(2);
  hSpec_rat[0]->SetTitle(";Trk p_{T};ratio");
  hSpec_rat[0]->SetAxisRange(0,100,"X");
  hSpec_rat[0]->SetAxisRange(0,3,"Y");
  handsomeTH1(hSpec_rat[0]);
  hSpec_rat[0]->Draw("E");
  hSpec_rat[2]->Draw("sameE");
  TLine * l = new TLine(0,1,120,1);
  l->SetLineStyle(2);
  l->Draw();

  c2->cd(1);
  TLegend *leg = new TLegend(0.52,0.65,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hSpec_ref,"PYTHIA+HYDJET 0-5%","");
  leg->AddEntry(hSpec_ref,"Trk |#eta|<1","");
  leg->AddEntry(hSpec_ref,"Gen. Charged","l");
  leg->AddEntry(hSpec_corr[0],"Raw Reco","p");
  leg->AddEntry(hSpec_corr[2],"Corrected Reco","p");
  leg->Draw();

  c2->Print(outdir+"/Spectra_closure_"+outname+".gif");
}
