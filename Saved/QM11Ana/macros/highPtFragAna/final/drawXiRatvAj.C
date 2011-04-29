#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawXiRatvAj(
    // === HI data ===
    //TString infname = "histff_tv8_80hydjuqv6_akpu3pf_an0428hydj.root"
    //TString infname = "histff_tv8_80hydjuqv6_akpu3pf_an0428hydjFixJ2.root"
    TString infname = "histff_tv8dataj35u_akpu3pf_an0429.root",
    //TString infname = "histff_tv8dataj35u_akpu3pf_t3pf1_an0429.root"
    //TString infname = "histff_tv8dataj35u_akpu3pf_t3pf4_an0429.root",
    TString inrefname = "histff_tv8dataj35u_akpu3pf_t3pf0_an0429.root"
    )
{
  cout << infname << endl;
  cout << inrefname << endl;
  TFile * inf = new TFile(infname);
  TFile * inref = new TFile(inrefname);
  TString outdir = "out/20110429";
  TString outname = infname;
  outname.ReplaceAll(".root","");

  bool hasCorr = false;

  TFile * outf = new TFile(outdir+"/final"+outname+".root","RECREATE");

  TF1 * fxi = new TF1("fxi","[0]*exp(-(1+[1])*x)*(1-exp(-x))^[2]",0,5);
  fxi->SetParameters(10,-0.5,5);

  TF1 * fxirefaj[4][2], * fxiaj[4][2];
  TH1D * hXiIncl_ref[2], *hXiIncl_corr[2], *hXiIncl_corrrat[2];
  TH1D * hXi_ref[4][2], *hXi_raw[4][2], *hXi_corr[4][2], *hXi_rawrat[4][2], *hXi_corrrat[4][2];
  TH1D * hJetPt[4][2], *hRefJetPt[4][2], *hJetAj[4], *hRefJetAj[4];
  TString var = "Xi";
  TString title = ";#xi;";
  //TString var = "PPt";
  //TString title = ";p_{T} (GeV/c);";
  for (Int_t j=0; j<2; ++j) {
    hXiIncl_ref[j] = (TH1D*)inref->FindObjectAny(Form("hRec%sCorr0_j%d",var.Data(),j));
    if (hasCorr) {
      hXiIncl_corr[j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr2_j%d",var.Data(),j));
      hXiIncl_corrrat[j] = (TH1D*)hXiIncl_corr[j]->Clone(Form("%s_corrrat",hXiIncl_corr[j]->GetName()));
      hXiIncl_corrrat[j]->Divide(hXiIncl_ref[j]);
      hXiIncl_ref[j]->Write();
      hXiIncl_corr[j]->Write();
      hXiIncl_corrrat[j]->Write();
    }
  }
  for (Int_t a=0; a<4; ++a) {
    hJetAj[a] = (TH1D*)inf->FindObjectAny(Form("hRecJetAj_a%dj0",a));
    hRefJetAj[a] = (TH1D*)inref->FindObjectAny(Form("hRecJetAj_a%dj0",a));
    for (Int_t j=0; j<2; ++j) {
      hXi_ref[a][j] = (TH1D*)inref->FindObjectAny(Form("hRec%sCorr0_a%dj%d",var.Data(),a,j));
      hXi_raw[a][j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr0_a%dj%d",var.Data(),a,j));
      if (hasCorr) hXi_corr[a][j] = (TH1D*)inf->FindObjectAny(Form("hRec%sCorr2_a%dj%d",var.Data(),a,j));
      hJetPt[a][j] = (TH1D*)inf->FindObjectAny(Form("hRecJetPt_a%dj%d",a,j));
      hRefJetPt[a][j] = (TH1D*)inref->FindObjectAny(Form("hRecJetPt_a%dj%d",a,j));
      // style
      hXi_ref[a][j]->SetMarkerColor(kRed);
      hXi_ref[a][j]->SetLineColor(kRed);
      hXi_ref[a][j]->SetMarkerStyle(kOpenCircle);
      hXi_raw[a][j]->SetMarkerStyle(kOpenSquare);
      hXi_raw[a][j]->SetMarkerColor(kGray+1);
      hXi_raw[a][j]->SetLineColor(kGray+1);
      // ratio
      hXi_rawrat[a][j] = (TH1D*)hXi_raw[a][j]->Clone(Form("%s_rawrat",hXi_raw[a][j]->GetName()));
      hXi_rawrat[a][j]->Divide(hXi_ref[a][j]);
      hXi_ref[a][j]->Write();
      if (hasCorr) {
	hXi_corrrat[a][j] = (TH1D*)hXi_corr[a][j]->Clone(Form("%s_corrrat",hXi_corr[a][j]->GetName()));
	hXi_corrrat[a][j]->Divide(hXi_ref[a][j]);
	hXi_corr[a][j]->Write();
	hXi_corrrat[a][j]->Write();
      }
    }
  }

  if (hasCorr) {
    TCanvas * c4 = new TCanvas("XiIncl","XiIncl",800,400);
    c4->Divide(2,1);
    for (Int_t j=0; j<2; ++j) {
      c4->cd(j+1);
      hXiIncl_corrrat[j]->SetAxisRange(0,1.5,"Y");
      hXiIncl_corrrat[j]->Draw();
    }
  }

  TCanvas * c3 = new TCanvas("Aj","Aj",1200,400);
  c3->Divide(4,1);
  for (Int_t a=0; a<4; ++a) {
    c3->cd(a+1);
    hRefJetAj[a]->SetLineColor(2);
    hRefJetAj[a]->SetAxisRange(0,hRefJetAj[a]->GetMaximum()*3,"Y");
    hRefJetAj[a]->Draw("E");
    hJetAj[a]->Draw("sameE");
  }
  c3->Print(outdir+"/Aj_"+outname+".gif");

  for (Int_t j=0; j<2; ++j) {
    TString app = Form("_j%d",j);
    TCanvas * c2 = new TCanvas("Xi"+app,"Xi"+app,1600,900);
    c2->Divide(4,3);
    for (Int_t a=0; a<4; ++a) {
      c2->cd(a+1);
      gPad->SetLogy();
      handsomeTH1(hXi_ref[a][j]);
      hXi_ref[a][j]->Draw("E");
      hXi_ref[a][j]->SetAxisRange(1e-3,1e2,"Y");
      hXi_raw[a][j]->Draw("sameE");
      // fit
      //Float_t xmin=0,xmax=2.3; // z=0.1 for j=40, p=4
      //if (j==0) xmax=3; // z ~ 0.05 for j=100, p=5
      //fxiaj[a][j] = (TF1*)fxi->Clone(Form("fxi_a%dj%d",a,j));
      fxiaj[a][j] = new TF1(Form("fxicut_a%dj%d",a,j),"[0]*exp(-(1+[1])*x)*(1-exp(-x))^[2]*1/(1+exp((x-[3])/[4]))",0,5);
      if (j==0) {
	fxiaj[a][j]->SetParameters(10,-0.5,5,3.2,0.15);
	fxiaj[a][j]->SetParLimits(1,-2.5,0);
	fxiaj[a][j]->SetParLimits(2,0,10);
	fxiaj[a][j]->SetParLimits(3,3.18,3.5);
	fxiaj[a][j]->SetParLimits(4,0.1,0.2);
	hXi_raw[a][j]->Fit(fxiaj[a][j],"0","",0.1,4.2);
	fxirefaj[a][j] = (TF1*)fxiaj[a][j]->Clone(Form("fxirefcut_a%dj%d",a,j));
	hXi_ref[a][j]->Fit(fxirefaj[a][j],"0","",0.1,4.2);
      }
      if (j==1) {
	fxiaj[a][j]->SetParameters(10,-0.5,5,2.5,0.15);
	fxiaj[a][j]->SetParLimits(1,-2.5,0);
	fxiaj[a][j]->SetParLimits(2,0,10);
	fxiaj[a][j]->SetParLimits(3,2.27,3.2);
	fxiaj[a][j]->SetParLimits(4,0.1,0.2);
	hXi_raw[a][j]->Fit(fxiaj[a][j],"0","",0.1,3.2);
	fxirefaj[a][j] = (TF1*)fxiaj[a][j]->Clone(Form("fxirefcut_a%dj%d",a,j));
	hXi_ref[a][j]->Fit(fxirefaj[a][j],"0","",0.1,3.2);
      }
      //fxiaj[a][j]->SetRange(xmin,xmax);
      fxiaj[a][j]->Draw("same");
      fxirefaj[a][j]->Draw("same");
      if (hasCorr) hXi_corr[a][j]->Draw("sameE");
      c2->cd(a+5);
      handsomeTH1(hXi_rawrat[a][j]);
      hXi_rawrat[a][j]->SetAxisRange(0,1.5,"Y");
      hXi_rawrat[a][j]->SetTitle(title);
      hXi_rawrat[a][j]->Draw("E");
      if (hasCorr) hXi_corrrat[a][j]->Draw("sameE");
      c2->cd(a+9);
      gPad->SetLogy();
      handsomeTH1(hJetPt[a][j]);
      //hJetPt[a][j]->SetAxisRange(0,1.5,"Y");
      hJetPt[a][j]->SetTitle(Form(";Jet%d p_{T} (GeV/c);",j));
      hJetPt[a][j]->Draw("E");
      hRefJetPt[a][j]->SetLineColor(2);
      hRefJetPt[a][j]->Draw("same hist");
    }
    c2->Print(outdir+"/XiClosure"+app+outname+".gif");
    c2->Print(outdir+"/XiClosure"+app+outname+".pdf");
  }
}
