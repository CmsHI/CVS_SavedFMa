#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawSpecRat(
    TString infname = "histff_tv11ppdatajet_ak3pf_hg_t2pf0_an0507.root",
    TString inrefname = "histff_tv11_80_ppgmv1_ak3pf_an0507.root"
    )
{
  cout << infname << endl;
  cout << inrefname << endl;
  TFile * inf = new TFile(infname);
  TFile * inref = new TFile(inrefname);
  TString outdir = "out/20110507";
  TString outname = infname;
  outname.ReplaceAll(".root","");

  bool hasCorr = true;

  TFile * outf = new TFile(outdir+"/final"+outname+".root","RECREATE");

  TH1D * hXiIncl_ref[2], *hXiIncl_corr[2], *hXiIncl_corrrat[2];
  TH1D * hXi_ref[4][2], *hXi_raw[4][2], *hXi_corr[4][2], *hXi_rawrat[4][2], *hXi_corrrat[4][2];
  TH1D * hJetPt[4][2], *hRefJetPt[4][2], *hJetAj[4], *hRefJetAj[4];
  //TString var = "Xi";
  //TString title = ";#xi;";
  TString var = "PPt";
  TString title = ";p_{T} (GeV/c);";
  for (Int_t j=0; j<2; ++j) {
    hXiIncl_ref[j] = (TH1D*)inref->FindObjectAny(Form("hRec%sCorr0_j%d",var.Data(),j));
    if (hasCorr) {
      hXiIncl_corr[j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr2_j%d",var.Data(),j));
      hXiIncl_corrrat[j] = (TH1D*)hXiIncl_corr[j]->Clone(Form("%s_corrrat",hXiIncl_corr[j]->GetName()));
      hXiIncl_corrrat[j]->Divide(hXiIncl_ref[j]);
    }
  }

  if (hasCorr) {
    TCanvas * c4 = new TCanvas("XiIncl","XiIncl",800,400);
    c4->Divide(2,2);
    for (Int_t j=0; j<2; ++j) {
      c4->cd(j+1);
      hXiIncl_ref[j]->Draw("hist");
      hXiIncl_corr[j]->Draw("sameE");
      c4->cd(j+3);
      hXiIncl_corrrat[j]->SetAxisRange(0,5,"Y");
      hXiIncl_corrrat[j]->Draw();
    }
  }
}
