#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawSpecClosure(
    // HI
    TString infname="histff_tv12hydjuq80repass_j4_j2t2_et90_0601Closure_c0to12_a0to100.root"
    // pp
    //TString infname="histff_tv11_dj80mattpfgmv1_ak3pf_j2t2_an0509trk5.root"
    )
{
  TFile * inf = new TFile(infname);
  TString outdir = "out/20110601";
  gSystem->mkdir(outdir,true);
  TString outname = infname;
  outname.ReplaceAll(".root","");

  Bool_t doRebin = false;

  // Histograms
  TH1D * hSpec_ref = (TH1D*)inf->FindObjectAny("hGenSpecCorr0");
	cout << hSpec_ref << " " << hSpec_ref->GetName() << " " << hSpec_ref->GetEntries() << endl;
  TH1D * hSpec_corr[3], *hSpec_rat[3];
  for (Int_t lv=0; lv<3; ++lv) {
    hSpec_corr[lv] = (TH1D*)inf->FindObjectAny(Form("hRecSpecCorr%d",lv));
		cout << hSpec_corr[lv] << " " << hSpec_corr[lv]->GetName() << " " << hSpec_corr[lv]->GetEntries() << endl;
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
	TH1D * hFrame = (TH1D*)hSpec_ref->Clone("hFrame");
	hFrame->Reset();
  hFrame->SetTitle(";Trk p_{T};dN/dp_{T}");
  hFrame->SetAxisRange(10,100,"X");
  hFrame->SetAxisRange(2e-5,5e1,"Y");	
  handsomeTH1(hFrame);
  c2->Divide(1,2);
  c2->cd(1);
  gPad->SetLogy();
	hFrame->Draw("p");
  hSpec_ref->Draw("same hist");
  hSpec_corr[0]->Draw("sameE");
  hSpec_corr[2]->Draw("sameE");
  c2->cd(2);
  hSpec_rat[0]->SetTitle(";Trk p_{T};ratio");
  hSpec_rat[0]->SetAxisRange(10,100,"X");
  hSpec_rat[0]->SetAxisRange(0,2,"Y");
  handsomeTH1(hSpec_rat[0]);
  hSpec_rat[0]->Draw("E");
  hSpec_rat[2]->Draw("sameE");
  TLine * l = new TLine(10,1,120,1);
  l->SetLineStyle(2);
  l->Draw();

  c2->cd(1);
  TLegend *leg = new TLegend(0.52,0.65,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hSpec_ref,"PYTHIA+HYDJET 0-30%","");
  leg->AddEntry(hSpec_ref,"hiGoodTight","");
  leg->AddEntry(hSpec_ref,"Trk |#eta|<1","");
  leg->AddEntry(hSpec_ref,"Gen. Charged","l");
  leg->AddEntry(hSpec_corr[0],"Raw Reco","p");
  leg->AddEntry(hSpec_corr[2],"Corrected Reco","p");
  leg->Draw();

  c2->Print(outdir+"/Spectra_closure_"+outname+".gif");
}
