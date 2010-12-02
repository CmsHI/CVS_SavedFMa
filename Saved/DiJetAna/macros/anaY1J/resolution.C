#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLine.h"

//#include "tdrstyle_mod.C"

#include <iostream>

using namespace std;

void resolution() {

  TDirectory *curdir = gDirectory;
  //setTDRStyle();

  TFile *f = new TFile("JES.root", "READ");
  assert(f && !f->IsZombie());

  //assert(f->cd("Standard"));
  //assert(gDirectory->cd("Eta_2.5-3.0"));
  //assert(gDirectory->cd("mc"));
  TDirectory *d = gDirectory;

  curdir->cd();

  TH1D *h1 = (TH1D*)d->Get("hEtNr"); assert(h1);
  TH2D *h2 = (TH2D*)d->Get("hEtNr2D"); assert(h2);

  const double etamin = 2.5;
  const double etamax = 3.0;
  const double ptmin = 5.;
  const double ptmax = 3000./cosh(etamin);

  // Normalize pT spectrum by bin width
  const double ferrmin = 0.02;
  TH1D *h1n = (TH1D*)h1->Clone("h1n");
  for (int i = 1; i != h1n->GetNbinsX()+1; ++i) {
    h1n->SetBinContent(i, h1->GetBinContent(i) / h1->GetBinWidth(i));
    h1n->SetBinError(i, sqrt(pow(h1->GetBinError(i) / h1->GetBinWidth(i),2)
			     + pow(h1n->GetBinContent(i)*0.02,2)));
  }

  // Estimate generator level spectrum so we can fix normalization
  TF1 *f1 = new TF1("f1","[0]*pow(x,[1])"
		    "*pow(1-min(2.*x*cosh([4]),7000.)/7000.,[2])"
		    "*exp([3]/x)",1.,ptmax);
  f1->SetParameters(1e13, -5, 10., -10., etamin);
  f1->FixParameter(4, etamin);
  h1n->Fit(f1, "QRN");
  h1n->Fit(f1, "QRNI");
  cout << Form("N=%1.2fx%1.2f, a=%1.2fx%1.2f, b=%1.2fx%1.2f, g=%1.2fx%1.2f",
	       f1->GetParameter(0)/1e12, f1->GetParError(0)/1e12,
	       f1->GetParameter(1), f1->GetParError(1),
	       f1->GetParameter(2), f1->GetParError(2),
	       f1->GetParameter(3), f1->GetParError(3)) << endl;
  cout << Form("// %1.1f-%1.1f: %1.1f / %d",
	       etamin, etamax,
	       f1->GetChisquare(), f1->GetNDF()) << endl;

  // Plot and fit resolution
  TCanvas *c1 = new TCanvas("c1","c1",6*200,5*200);
  c1->Divide(6,5,-1,-1);

  TH1D *h = new TH1D("h",";p_{T}^{reco} / p_{T}^{gen};1/N/dR_{jet}",
		     200,0,2);
  h->SetMinimum(1e-5);//0.);
  h->SetMaximum(50.);

  TF1 *fit = new TF1("fit","TMath::Gaus(x,[1],[0],1)",0.,2.);
  fit->SetLineColor(kBlue);

  TGraphErrors *g1 = new TGraphErrors(0);
  TGraphErrors *g2 = new TGraphErrors(0);
  TGraphErrors *g2r = new TGraphErrors(0);

  for (int ibin = 1; ibin != h2->GetNbinsX()+1; ++ibin) {

    c1->cd(ibin);
    gPad->SetLogy();
    h->Draw("AXIS");

    TH1D *hp = h2->ProjectionY(Form("hp%d",ibin),ibin,ibin);
    hp->Rebin(4);//2);
    //hp->Scale(1./h1->GetBinContent(ibin));
    TH1D *hpc = (TH1D*)hp->Clone(Form("hpc%d",ibin));
    hpc->Reset();
    for (int i = 1; i != hp->GetNbinsX()+1; ++i) {

      double sc = hp->GetBinWidth(i) * h1->GetBinContent(ibin);
      hp->SetBinContent(i, hp->GetBinContent(i) / sc);
      hp->SetBinError(i, hp->GetBinError(i) / sc);

      if (hp->GetBinLowEdge(i) > 10./h1->GetBinLowEdge(ibin)) {
	hpc->SetBinContent(i, hp->GetBinContent(i));
	hpc->SetBinError(i, hp->GetBinError(i));
      }
    } // for i

    fit->SetLineColor(kBlue);
    fit->FixParameter(1, 1.);
    hpc->Fit(fit,"QRN");
    fit->DrawClone("SAME");

    if (fit->GetNDF()>1) {
      double ptmin = h1->GetBinLowEdge(ibin);
      double ptmax = h1->GetBinLowEdge(ibin+1);
      double y = f1->Integral(ptmin, ptmax) / (ptmax - ptmin);
      double x = f1->GetX(y, ptmin, ptmax);
      int n = g1->GetN();
      int c = sqrt(max(fit->GetChisquare()/fit->GetNDF(),1.));
      g1->SetPoint(n, x, fit->GetParameter(0));
      g1->SetPointError(n, 0., c*fit->GetParError(0));
    }

    fit->SetLineColor(kRed);
    fit->ReleaseParameter(1);
    hpc->Fit(fit,"QRN");
    fit->DrawClone("SAME");

    if (fit->GetNDF()>1) {
      double ptmin = h1->GetBinLowEdge(ibin);
      double ptmax = h1->GetBinLowEdge(ibin+1);
      double y = f1->Integral(ptmin, ptmax) / (ptmax - ptmin);
      double x = f1->GetX(y, ptmin, ptmax);
      int n = g2->GetN();
      int c = sqrt(max(fit->GetChisquare()/fit->GetNDF(),1.));
      g2->SetPoint(n, x, fit->GetParameter(0));
      g2->SetPointError(n, 0., c*fit->GetParError(0));
      g2r->SetPoint(n, x, fit->GetParameter(1));
      g2r->SetPointError(n, 0., c*fit->GetParError(1));
    }

    hp->SetLineColor(kRed);
    hp->Draw("SAME");
    hpc->Draw("SAME");

  }

  c1->SaveAs("eps/resolution_allfits.eps");

  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  c2->SetLogx();

  TH1D *hs = new TH1D("hs",";p_{T} (GeV);#sigma_{p_{T}}/p_{T}",
		      int(ptmax-ptmin),ptmin,ptmax);
  hs->GetXaxis()->SetMoreLogLabels();
  hs->GetXaxis()->SetNoExponent();
  hs->SetMinimum(0.);
  hs->SetMaximum(0.55);
  hs->Draw("AXIS");

  TF1 *fres = new TF1("fres","sqrt(TMath::Sign([0]*[0],[0])/(x*x)"
		      "+[1]*[1]*pow(x,[3]-1) + [2]*[2])", ptmin, ptmax);
  fres->SetParameters(1., 1., 0.05, 0.1);

  g1->SetMarkerStyle(kFullCircle);
  g1->Fit(fres, "QRN");
  g1->Draw("SAMEP");
  fres->SetLineColor(kBlue);
  fres->DrawClone("SAME");

  g2->SetMarkerStyle(kOpenCircle);
  g2->Fit(fres, "QRN");
  g2->Draw("SAMEP");
  fres->SetLineColor(kRed);
  fres->DrawClone("SAME");

  //fres->SetParameters(2.56933,  0.305802, 0, 0.398929);
  fres->SetParameters(5.0913,0.512,0,0.325);
  fres->SetLineColor(kBlack);
  fres->SetLineStyle(kDashed);
  fres->DrawClone("SAME");

  //fres->SetParameters( 1.89978, 0.33427, 0.00000, 0.36547);
  fres->SetParameters(5.0913,0.512,0,0.325);
  fres->SetLineColor(kYellow+2);
  fres->SetLineStyle(kDotted);
  fres->DrawClone("SAME");

  c2->SaveAs("eps/resolution_sigma.eps");


  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  c3->SetLogx();

  TH1D *hr = new TH1D("hr",";p_{T} (GeV);R_{jet}",int(ptmax-ptmin),ptmin,ptmax);
  hr->GetXaxis()->SetMoreLogLabels();
  hr->GetXaxis()->SetNoExponent();
  hr->SetMinimum(0.6);
  hr->SetMaximum(1.05);
  hr->Draw("AXIS");

  TF1 *fr = new TF1("fr","[0]+[1]*pow(x,[2])",ptmin,ptmax);
  fr->SetParameters(1.00,-0.5,0.1);

  g2r->Fit(fr,"QRN");
  fr->Draw("SAME");
  g2r->Draw("SAMEP");

  cout << Form("p0=%1.2fx%1.2f, p1=%1.2fx%1.2f, p2=%1.2fx%1.2f",
	       fr->GetParameter(0), fr->GetParError(0),
	       fr->GetParameter(1), fr->GetParError(1),
	       fr->GetParameter(2), fr->GetParError(2)) << endl;
  cout << Form("// %1.1f-%1.1f: %1.1f / %d",
	       etamin, etamax,
	       fr->GetChisquare(), fr->GetNDF()) << endl;

  TLine *l = new TLine();
  l->SetLineStyle(kDashed);
  l->DrawLine(ptmin, 0.99, ptmax, 0.99);
  l->DrawLine(ptmin, 1.01, ptmax, 1.01);

  c3->SaveAs("eps/resolution_rjet.eps");
}
