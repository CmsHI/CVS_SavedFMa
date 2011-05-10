#include "../../HisMath.C"
#include "../../commonUtility.h"

void drawSpecRat(
    TString infname="histff_tv11data_akpu3pf_an0509trk5.root",
    TString inrefname = "../specRef/toFrank/CORRv1_hcpr_spectra_v4_J50U_all_finec_sub1_HCPR_apr042011_eta_0.0to1.0_jet_0.0to1000.0_hitrackAna_jetMode1_GEN0_reb2_cbin0to1.root"
    )
{
  cout << infname << endl;
  cout << inrefname << endl;
  TFile * inf = new TFile(infname);
  TFile * inref = new TFile(inrefname);
  TString outdir = "out/20110509";
  TString outname = infname+"_vs_apr042011";
  outname.ReplaceAll(".root","");

  Bool_t doRebin=true;
  TFile * outf = new TFile(outdir+"/final"+outname+".root","RECREATE");

  // Histograms
  //TH1D * hSpec_ref = (TH1D*)inf->FindObjectAny("hGenSpecCorr0");
  TH1D * hSpec_ref = (TH1D*)inref->Get("corrTypeOne/hdndpt_full");
  TH1D * hSpec_ana = (TH1D*)inf->FindObjectAny(Form("hRecSpecCorr2"));

  // normalization
  Int_t begbin_ref, endbin_ref, begbin_ana, endbin_ana;
  begbin_ref = hSpec_ref->FindBin(40.01);
  endbin_ref = hSpec_ref->FindBin(200);
  begbin_ana = hSpec_ana->FindBin(40.01);
  endbin_ana = hSpec_ana->FindBin(200);
  cout << "Integral range. ref: " << begbin_ref << "," << endbin_ref << " ana: " << begbin_ana << "," << endbin_ana << endl;
  Float_t area_ref = integrateHistArea(hSpec_ref,begbin_ref,endbin_ref);
  Float_t area_ana = integrateHistArea(hSpec_ana,begbin_ana,endbin_ana);
  Float_t area_rat = area_ana/area_ref;
  cout << "Ref area: " << area_ref << " Ana area: " << area_ana << " ratio ana/ref: " << area_rat << endl;
  Float_t scale_ana = 1.479e-03;
  //Float_t scale_ana = 2.363e-03;
  //Float_t scale_ref = area_rat;
  Float_t scale_ref = 1658.15;
  //scale_ref = 1658.15*137438./198;
  //scale_ref = 1658.15*137438./198/0.47;
  hSpec_ana->Scale(scale_ana);
  hSpec_ref->Scale(scale_ref);
  cout << "final scale_ref: " << scale_ref << endl;

  // rebin
  if (doRebin) {
    hSpec_ana->Rebin(2);
    hSpec_ana->Scale(0.5);
    hSpec_ref->Rebin(2);
    hSpec_ref->Scale(0.5);
  }

  // fit
  TString fitFor3 = "[0]*pow((x+[1]),[2])*exp([3]*x+[4])";
  cout << "Fit3: " << fitFor3 << endl;
  TF1 * fitF3 = new TF1("fitF3",fitFor3.Data(),1,200);
  fitF3->SetParameters(1,1,-5,0.0,1);
  fitF3->SetParLimits(2,-7.0,-2.0);
  fitF3->SetParLimits(3,-1,.0);
  fitF3->SetParLimits(4,0.0,5.0);
  hSpec_ref->Fit("fitF3","0","",10,200);

  // ratio
  //TH1D * hSpec_refrat = (TH1D*)hSpec_ref->Clone("hSpec_refrat");
  //hSpec_refrat->Divide(fitF3);
  TH1D * hSpec_anarat = (TH1D*)hSpec_ana->Clone("hSpec_anarat");
  //hSpec_anarat->Divide(fitF3);
  hSpec_anarat->Divide(hSpec_ref);

  // draw
  TCanvas * c2 = new TCanvas("c2","c2",500,900);
  c2->Divide(1,2);
  c2->cd(1);
  gPad->SetLogy();
  hSpec_ref->SetMarkerStyle(kOpenSquare);
  hSpec_ana->SetAxisRange(0,100,"X");
  hSpec_ana->SetTitle(";Trk p_{T} (GeV/c); #frac{1}{N_{MB}} #frac{dN}{dp_{T}}");
  handsomeTH1(hSpec_ana);
  hSpec_ana->Draw("E");
  hSpec_ref->Draw("Esame");
  //fitF3->Draw("same");
  c2->cd(2);
  hSpec_anarat->SetAxisRange(0,100,"X");
  hSpec_anarat->SetAxisRange(0,3,"Y");
  hSpec_anarat->SetTitle(";Trk p_{T} (GeV/c); ratio");
  handsomeTH1(hSpec_anarat);
  hSpec_anarat->Draw("E");
  hSpec_anarat->Fit("pol0","","",20,120);
  //hSpec_refrat->SetMarkerStyle(kOpenSquare);
  //hSpec_refrat->Draw("Esame");

  TLine * l = new TLine(0,1,100,1);
  l->SetLineStyle(2);
  l->Draw();

  c2->cd(1);
  TLegend *leg = new TLegend(0.52,0.65,0.91,0.91);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->AddEntry(hSpec_ref,"PbPb data 0-5%","");
  leg->AddEntry(hSpec_ref,"Trk |#eta|<1","");
  leg->AddEntry(hSpec_ref,"Charged Spectra","p");
  leg->AddEntry(hSpec_ana,"Jet Analysis","p");
  leg->Draw();

  //c2->Print(outdir+"/Spectra_comp_"+outname+".gif");
}
