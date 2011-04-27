#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawXiClosurevAj(
    TString infname = "histff_tv8_80_pphptv1_ak3pf_an0427.root"
    //TString infname = "histff_tv8_80_ppgmv1_ak3pf_an0427.root"
    )
{
  TFile * inf = new TFile(infname);

  TH1D * hXi_ref[4][2], *hXi_raw[4][2], *hXi_corr[4][2], *hXi_rawrat[4][2], *hXi_corrrat[4][2];
  TString var = "Xi";
  TString title = ";#xi;";
  //TString var = "PPt";
  //TString title = ";p_{T} (GeV/c);";
  for (Int_t a=0; a<4; ++a) {
    for (Int_t j=0; j<2; ++j) {
      hXi_ref[a][j] = (TH1D*)inf->FindObjectAny(Form("hGen%sCorr0_a%dj%d",var.Data(),a,j));
      hXi_raw[a][j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr0_a%dj%d",var.Data(),a,j));
      hXi_corr[a][j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr2_a%dj%d",var.Data(),a,j));
      // style
      hXi_ref[a][j]->SetLineColor(kRed);
      hXi_raw[a][j]->SetMarkerStyle(kOpenSquare);
      hXi_raw[a][j]->SetMarkerColor(kGray);
      hXi_raw[a][j]->SetLineColor(kGray);
      // ratio
      hXi_rawrat[a][j] = (TH1D*)hXi_raw[a][j]->Clone(Form("%s_rawrat",hXi_raw[a][j]->GetName()));
      hXi_rawrat[a][j]->Divide(hXi_ref[a][j]);
      hXi_corrrat[a][j] = (TH1D*)hXi_corr[a][j]->Clone(Form("%s_corrrat",hXi_corr[a][j]->GetName()));
      hXi_corrrat[a][j]->Divide(hXi_ref[a][j]);
    }
  }

  for (Int_t j=0; j<2; ++j) {
    TString app = Form("Xi_j%d",j);
    TCanvas * c2 = new TCanvas(app,app,1600,700);
    c2->Divide(4,2);
    for (Int_t a=0; a<4; ++a) {
      c2->cd(a+1);
      gPad->SetLogy();
      handsomeTH1(hXi_ref[a][j]);
      hXi_ref[a][j]->Draw("hist");
      hXi_ref[a][j]->SetAxisRange(1e-3,1e2,"Y");
      hXi_raw[a][j]->Draw("sameE");
      hXi_corr[a][j]->Draw("sameE");
      c2->cd(a+5);
      handsomeTH1(hXi_rawrat[a][j]);
      hXi_rawrat[a][j]->SetAxisRange(0,1.5,"Y");
      hXi_rawrat[a][j]->SetTitle(title);
      hXi_rawrat[a][j]->Draw("E");
      hXi_corrrat[a][j]->Draw("sameE");
    }
  }
}
