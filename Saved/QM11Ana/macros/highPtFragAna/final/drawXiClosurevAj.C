#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawXiClosurevAj(
    //TString infname = "histff_tv8_80_pphptv1_ak3pf_an0428.root"
    //TString infname = "histff_tv8_80_pphptv1_ak3pf_an0428Jet2Pt60to90.root"
    TString infname = "histff_tv8_80_pphptv1_ak3pf_an0428MatRefAj005.root"
    //TString infname = "histff_tv8_80_pphptv1_ak3pfrefjet_an0427.root"
    //TString infname = "histff_tv8_80_ppgmv1_ak3pf_an0427.root"
    )
{
  TFile * inf = new TFile(infname);

  TH1D * hXi_ref[4][2], *hXi_raw[4][2], *hXi_corr[4][2], *hXi_rawrat[4][2], *hXi_corrrat[4][2];
  TH1D * hJetPt[4][2], *hRefJetPt[4][2], *hJetAj[4], *hRefJetAj[4];
  TString var = "Xi";
  TString title = ";#xi;";
  //TString var = "PPt";
  //TString title = ";p_{T} (GeV/c);";
  for (Int_t a=0; a<4; ++a) {
    hJetAj[a] = (TH1D*)inf->FindObjectAny(Form("hRecJetAj_a%dj0",a));
    hRefJetAj[a] = (TH1D*)inf->FindObjectAny(Form("hRecRefJetAj_a%dj0",a));
    for (Int_t j=0; j<2; ++j) {
      hXi_ref[a][j] = (TH1D*)inf->FindObjectAny(Form("hGen%sCorr0_a%dj%d",var.Data(),a,j));
      hXi_raw[a][j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr0_a%dj%d",var.Data(),a,j));
      hXi_corr[a][j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr2_a%dj%d",var.Data(),a,j));
      hJetPt[a][j] = (TH1D*)inf->FindObjectAny(Form("hRecJetPt_a%dj%d",a,j));
      hRefJetPt[a][j] = (TH1D*)inf->FindObjectAny(Form("hRecRefJetPt_a%dj%d",a,j));
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

  TCanvas * c3 = new TCanvas("Aj","Aj",1200,400);
  c3->Divide(4,1);
  for (Int_t a=0; a<4; ++a) {
    c3->cd(a+1);
    hRefJetAj[a]->SetLineColor(2);
    hRefJetAj[a]->SetAxisRange(0,hRefJetAj[a]->GetMaximum()*2,"Y");
    hRefJetAj[a]->Draw("hist");
    hJetAj[a]->Draw("sameE");
  }

  for (Int_t j=0; j<2; ++j) {
    TString app = Form("_j%d",j);
    TCanvas * c2 = new TCanvas("Xi"+app,"Xi"+app,1600,900);
    c2->Divide(4,3);
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
      c2->cd(a+9);
      gPad->SetLogy();
      handsomeTH1(hJetPt[a][j]);
      //hJetPt[a][j]->SetAxisRange(0,1.5,"Y");
      hJetPt[a][j]->SetTitle(Form(";Jet%d p_{T} (GeV/c);",j));
      hJetPt[a][j]->Draw("E");
      hRefJetPt[a][j]->SetLineColor(2);
      hRefJetPt[a][j]->Draw("same hist");
    }

  }
}
