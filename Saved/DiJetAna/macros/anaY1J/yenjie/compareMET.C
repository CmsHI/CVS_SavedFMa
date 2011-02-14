#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TLine.h>
#include <TCut.h>

void compareMET()
{
   TFile *inf = new TFile ("nt_dj_data100_cor.root");
   TTree *tData = (TTree*)inf->FindObjectAny("ntjt");
   TFile *inf2 = new TFile ("nt_dj_PYTHIA.root");
   TTree *tPYTHIA = (TTree*)inf2->FindObjectAny("ntjt");
   TFile *inf3 = new TFile ("nt_dj_mix100.root");

   TTree *tMix = (TTree*)inf3->FindObjectAny("ntjt");

   TCanvas *c = new TCanvas("c","",600,600);
   TProfile *p = new TProfile("p","",5,0,0.5);
   TProfile *p2 = new TProfile("p2","",5,0,0.5);
   TProfile *p3 = new TProfile("p3","",5,0,0.5);
   TCut evtCut = "nljet>100&&abs(nljetacorr)<2&&aljet>50&&abs(aljetacorr)<2&&jdphi>2./3*TMath::Pi()&&!maskEvt";

   tData->Draw("-metx:Aj>>p","weight"*evtCut,"prof");
   tPYTHIA->Draw("-metx:Aj>>p2",evtCut,"prof");
   tMix->Draw("-metx:Aj>>p3","weight"*evtCut,"prof");

   p3->SetMarkerStyle(4);
   p3->SetMarkerColor(4);
   p3->SetLineColor(4);
   p2->SetMarkerStyle(4);
   p2->SetMarkerColor(2);
   p2->SetLineColor(2);
   p->SetXTitle("A_{J}");
   p->GetXaxis()->CenterTitle();
   p->GetYaxis()->CenterTitle();
   p->SetYTitle("<#slash p_{T}^{#parallel} (GeV/c)>");
   p->SetAxisRange(-30,30,"Y");
   p->Draw("");
   p2->Draw("same");
   p3->Draw("same");
   
   
   TLine *l = new TLine(0,0,100,0);
   l->SetLineStyle(2);
   l->Draw();
    
   TLegend *leg = new TLegend(0.25,0.22,0.61,0.51);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);

   leg->AddEntry(p,"Data","pl");
   leg->AddEntry(p2,"PYTHIA","pl");
   leg->AddEntry(p3,"PYTHIA + HYDJET","pl");

   leg->Draw();


   TCanvas *c2 = new TCanvas("c2","",600,600);
   TH1D *h = new TH1D("h","",50,-200,200);
   TH1D *h2 = new TH1D("h2","",50,-200,200);
   TH1D *h3 = new TH1D("h3","",50,-200,200);

   tData->Draw("-mety>>h",evtCut,"prof");
   tPYTHIA->Draw("-mety>>h2",evtCut,"prof");
   tMix->Draw("-mety>>h3","weight"*(evtCut),"prof");

   h->Sumw2();
   h2->Sumw2();
   h3->Sumw2();

   h->Scale(1./h->Integral(0,1000));
   h2->Scale(1./h2->Integral(0,1000));
   h3->Scale(1./h3->Integral(0,1000));

   h3->SetMarkerStyle(4);
   h3->SetMarkerColor(4);
   h3->SetLineColor(4);
   h2->SetMarkerStyle(4);
   h2->SetMarkerColor(2);
   h2->SetLineColor(2);
   h->SetYTitle("Event Fraction");
   h->GetXaxis()->CenterTitle();
   h->GetYaxis()->CenterTitle();
   h->SetXTitle("#slash p_{T}^{#perp} (GeV/c)");
   h->SetAxisRange(0,0.35,"Y");
   h->Draw("");
   h2->Draw("same");
   h3->Draw("same");
   
   TLegend *leg2 = new TLegend(0.21,0.65,0.57,0.94);
   leg2->SetBorderSize(0);
   leg2->SetFillColor(0);
   leg2->SetFillStyle(0);
   leg2->SetTextFont(63);
   leg2->SetTextSize(16);

   leg2->AddEntry(p,"Data","pl");
   leg2->AddEntry(p2,"PYTHIA","pl");
   leg2->AddEntry(p3,"PYTHIA + HYDJET","pl");

   leg2->Draw();

   TCanvas *c3 = new TCanvas("c3","",600,600);
   TH1D *hB = new TH1D("hB","",50,-200,200);
   TH1D *hB2 = new TH1D("hB2","",50,-200,200);
   TH1D *hB3 = new TH1D("hB3","",50,-200,200);

   tData->Draw("-metx>>hB",evtCut,"prof");
   tPYTHIA->Draw("-metx>>hB2",evtCut,"prof");
   tMix->Draw("-metx>>hB3","weight"*(evtCut),"prof");

   hB->Sumw2();
   hB2->Sumw2();
   hB3->Sumw2();

   hB->Scale(1./hB->Integral(0,1000));
   hB2->Scale(1./hB2->Integral(0,1000));
   hB3->Scale(1./hB3->Integral(0,1000));

   hB3->SetMarkerStyle(4);
   hB3->SetMarkerColor(4);
   hB3->SetLineColor(4);
   hB2->SetMarkerStyle(4);
   hB2->SetMarkerColor(2);
   hB2->SetLineColor(2);
   hB->SetYTitle("Event Fraction");
   hB->GetXaxis()->CenterTitle();
   hB->GetYaxis()->CenterTitle();
   hB->SetXTitle("#slash p_{T}^{#parallel} (GeV/c)");
   hB->SetAxisRange(0,0.2,"Y");
   hB->Draw("");
   hB2->Draw("same");
   hB3->Draw("same");
   
   TLegend *leg3 = new TLegend(0.21,0.65,0.57,0.94);
   leg3->SetBorderSize(0);
   leg3->SetFillColor(0);
   leg3->SetFillStyle(0);
   leg3->SetTextFont(63);
   leg3->SetTextSize(16);

   leg3->AddEntry(p,"Data","pl");
   leg3->AddEntry(p2,"PYTHIA","pl");
   leg3->AddEntry(p3,"PYTHIA + HYDJET","pl");

   leg3->Draw();

}
