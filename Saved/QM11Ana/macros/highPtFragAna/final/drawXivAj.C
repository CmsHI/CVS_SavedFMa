#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawXivAj(
    // === HI ===
    //TString infname = "histff_tv8_80hydjuqv6_akpu3pf_an0428hydj.root"
    //TString infname = "histff_tv8_80hydjuqv6_akpu3pf_an0428hydjFixJ2.root"
    //TString infname = "histff_tv8dataj35u_akpu3pf_an0429.root"
    //TString infname = "histff_tv8dataj35u_akpu3pf_t3pf1_an0429.root"
    //TString infname = "histff_tv11dataj35u_akpu3pf_t2pf0_an0507.root"
    //TString infname = "histff_tv8dataj35u_akpu3pf_t3pf4_an0429.root"
    //TString infname = "histff_tv8dataj35u_akpu3pf_t3pf0_an0429.root"
    // === pp ===
    //TString infname = "histff_tv8_80_pphptv1_ak3pf_an0427.root"
    //TString infname = "histff_tv8_80_pphptv1_ak3pf_an0428Jet2Pt60to90.root"
    //TString infname = "histff_tv8_80_pphptv1_ak3pfrefjet_an0427.root"
    //TString infname = "histff_tv8_80_ppgmv1_ak3pf_an0427.root"
    //TString infname = "histff_tv11ppdatajet_ak3pf_t2pf0_an0507.root"
    //TString infname = "histff_tv11ppdatajet_ak3pf_t2pf0_an0507incltrkcorr.root"
    TString infname = "histff_tv11ppdatajet_ak3pf_hpt_t2pf0_an0507.root"
    //TString infname = "histff_tv11ppdatajet_ak3pf_hpt_t2pf0_an0507incltrkcorr.root"
    )
{
  TH1::SetDefaultSumw2();
  TFile * inf = new TFile(infname);

  TH1D * hXiIncl[2];
  TH1D * hXiCorr[4][2], *hXiCorrrat[4][2];
  TH1D * hJetPt[4][2], *hRefJetPt[4][2], *hJetAj[4], *hRefJetAj[4];

  //TString var = "hGenXiCorr0";
  //TString var = "hRecXiCorr0";
  TString var = "hRecXiCorr2";
  TString title = ";#xi;";
  //TString var = "PPt";
  //TString title = ";p_{T} (GeV/c);";
  Int_t colors[10] = {kGray+2,kViolet,kBlue+1,kGreen+2,kOrange+2,kMagenta,kRed};
  for (Int_t a=0; a<4; ++a) {
    hJetAj[a] = (TH1D*)inf->FindObjectAny(Form("hRecJetAj_a%dj0",a));
  }
  for (Int_t j=0; j<2; ++j) {
    hXiIncl[j] = (TH1D*)inf->FindObjectAny(Form("%s_j%d",var.Data(),j));
    hXiIncl[j]->SetLineColor(kRed);
    for (Int_t a=0; a<4; ++a) {
      hJetPt[a][j] = (TH1D*)inf->FindObjectAny(Form("hRecJetPt_a%dj%d",a,j));
      hXiCorr[a][j] = (TH1D*)inf->FindObjectAny(Form("%s_a%dj%d",var.Data(),a,j));
      // style
      if (a!=3) {
	hXiCorr[a][j]->SetMarkerStyle(kOpenCircle);
	hXiCorr[a][j]->SetMarkerColor(colors[a]);
	hXiCorr[a][j]->SetLineColor(colors[a]);
      }
      // ratio
      hXiCorrrat[a][j] = (TH1D*)hXiCorr[a][j]->Clone(Form("%s_refrat",hXiCorr[a][j]->GetName()));
      hXiCorrrat[a][j]->Divide(hXiCorr[0][j]);
      //hXiCorrrat[a][j]->Divide(hXiIncl[j]);
    }
  }

  for (Int_t j=0; j<2; ++j) {
    TString app = Form("Xi_j%d",j);
    TCanvas * c2 = new TCanvas(app,app,500,900);
    c2->Divide(1,2);
    c2->cd(1);
    handsomeTH1(hXiCorr[0][j]);
    hXiCorr[0][j]->SetAxisRange(0,3,"Y");
    hXiCorr[0][j]->Draw("hist");
    //handsomeTH1(hXiIncl[j]);
    //hXiIncl[j]->SetAxisRange(0,3,"Y");
    //hXiIncl[j]->Draw("hist");
    for (Int_t a=0; a<4; ++a) {
      hXiCorr[a][j]->Draw("sameE");
    }
    c2->cd(2);
    handsomeTH1(hXiCorrrat[0][j]);
    hXiCorrrat[0][j]->SetAxisRange(0,1.5,"Y");
    hXiCorrrat[0][j]->SetTitle(title);
    hXiCorrrat[0][j]->Draw("E");
    for (Int_t a=1; a<4; ++a) {
      hXiCorrrat[a][j]->Draw("sameE");
    }
  }

  TCanvas * c3 = new TCanvas("Aj","Aj",1200,900);
  c3->Divide(4,3);
  for (Int_t a=0; a<4; ++a) {
    c3->cd(a+1);
    //hJetAj[a]->SetAxisRange(0,hJetAj[a]->GetMaximum()*3,"Y");
    hJetAj[a]->Draw("hist");
    c3->cd(a+5);
    gPad->SetLogy();
    hJetPt[a][0]->Draw();
    c3->cd(a+9);
    gPad->SetLogy();
    hJetPt[a][1]->Draw();
  }

  Int_t mksty_jet[2] = { kOpenCircle, kFullCircle };
  Int_t mkcol_jet[2] = { kBlack, kBlack };
  TCanvas * cMuXi = new TCanvas("MuXi","MuXi",1600,800);
  cMuXi->Divide(4,2);
  for (Int_t a=0; a<4; ++a) {
    for (Int_t j=0; j<2; ++j) {
      handsomeTH1(hXiCorr[a][j]);
      hXiCorr[a][j]->SetMarkerStyle(mksty_jet[j]);
      hXiCorr[a][j]->SetMarkerColor(mkcol_jet[j]);
      hXiCorr[a][j]->SetLineColor(mkcol_jet[j]);
      hXiCorr[a][j]->SetAxisRange(0,5,"X");
      hXiCorr[a][j]->SetAxisRange(1e-4,2e3,"Y");
      handsomeTH1(hJetPt[a][j]);
      hJetPt[a][j]->SetMarkerStyle(mksty_jet[j]);
      hJetPt[a][j]->SetMarkerColor(mkcol_jet[j]);
      hJetPt[a][j]->Sumw2();
      //hJetPt[a][j]->Scale(1./hJetPt[a][j]->GetEntries());
      hJetPt[a][j]->SetAxisRange(1e-1,1e7,"Y");
    }
    cMuXi->cd(a+1);
    gPad->SetLogy();
    hXiCorr[a][0]->Draw("E");
    hXiCorr[a][1]->Draw("sameE");
    cMuXi->cd(a+5);
    gPad->SetLogy();
    hJetPt[a][0]->Draw("E");
    hJetPt[a][1]->Draw("sameE");
  }
}
