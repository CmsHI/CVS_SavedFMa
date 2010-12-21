#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <TF1.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <TChain.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TLegend.h>
#include <TGraphErrors.h>

using namespace std;

void replotSimpleMpT(){

   string type = "Embed";
   string ajLabel[3] = {"ALL","A_{J} > 0.24","A_{J} < 0.24"};
   int version = 0;
   int ajindex = 0;

   TH1::SetDefaultSumw2();
   //   int color[10] = {1,kGray,kBlue-3,38,kOrange-8,kRed-6,8,9,2};
   int color[10] = {1,1,kBlue-3,38,kOrange-8,kRed-6,8,9,2};

   string fname;

   if(type == "DATA"){
      fname = "data_cutOnly_05.root";
   }
   if(type == "Pythia"){
      fname = "pythia_cutOnly_05.root";
   }
   if(type == "Embed"){
      fname = "embed_cutOnly_05.root";
   }

   TFile* inf =  new TFile(fname.data());

   TH1D* h[10][20];
   TH1D* hx[10][20];
   TH1D* hy[10][20];
   TH1D* hwx[10][20];
   TH1D* hwy[10][20];

   bool upperBounded = false;

   int rebin = 4;
   int nHbins = 20;
   int nPtBins = 5;   
   double trackPtMin[20] = {0.5,0.5,1.5,4,8.,20};
   double trackPtMax[20] = {500,1.5,4,8.,20,500};

   TGraphErrors* g = new TGraphErrors(nPtBins);
   g->SetMarkerColor(1);
   g->SetMarkerStyle(20);
   g->SetMarkerSize(1.5);

   for(int j = 0; j < 5; j++){
      for(int i = 0; i < nPtBins; i++){

	 h[j][i] = (TH1D*)inf->Get(Form("h%d_%d",j,i));
	 h[j][i]->SetLineColor(color[i]);
	 h[j][i]->SetMarkerColor(color[i]);

	 hx[j][i] = (TH1D*)inf->Get(Form("hx%d_%d",j,i));
         hx[j][i]->SetLineColor(color[i]);
         hx[j][i]->SetMarkerColor(color[i]);

	 hy[j][i] = (TH1D*)inf->Get(Form("hy%d_%d",j,i));
         hy[j][i]->SetLineColor(color[i]);
         hy[j][i]->SetMarkerColor(color[i]);

	 hwx[j][i] = (TH1D*)inf->Get(Form("hwx%d_%d",j,i));
         hwx[j][i]->SetLineColor(color[i]);
         hwx[j][i]->SetMarkerColor(color[i]);

	 hwy[j][i] = (TH1D*)inf->Get(Form("hwy%d_%d",j,i));
         hwy[j][i]->SetLineColor(color[i]);
         hwy[j][i]->SetMarkerColor(color[i]);

	 hy[j][i]->SetTitle(";#slash{p}_{T}^{#perp} (GeV/c);Event Fraction");
	 hy[j][i]->GetXaxis()->CenterTitle();
         hy[j][i]->GetYaxis()->CenterTitle();
         hx[j][i]->SetTitle(";#slash{p}_{T}^{#parallel} (GeV/c);Event Fraction");

	 hx[j][i]->GetXaxis()->CenterTitle();
         hx[j][i]->GetYaxis()->CenterTitle();

	 hy[j][i]->Rebin(rebin);
         hy[j][i]->SetMarkerSize(0);
         hy[j][i]->SetLineWidth(2.);

	 hx[j][i]->Rebin(rebin);
         hx[j][i]->SetMarkerSize(0);
         hx[j][i]->SetLineWidth(2.);

	 if(hx[j][i]->Integral() > 0) hx[j][i]->Scale(1./hx[j][i]->Integral());
         if(hy[j][i]->Integral() > 0) hy[j][i]->Scale(1./hy[j][i]->Integral());
	 

	 //	 hx[j][i]->Fit("gaus");
         if(hx[j][i]->GetFunction("gaus"))hx[j][i]->GetFunction("gaus")->SetLineColor(color[i]);

      }
   }

   TLegend *leg=new TLegend(0.67,0.64,0.91,0.93);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->SetTextFont(63);
   leg->SetTextSize(15);
   for(int i = 0; i < nPtBins; ++i){
      if(upperBounded){
         leg->AddEntry(hx[ajindex][i],Form("%0.1f<p_{T}^{Track}<%0.1f GeV/c",trackPtMin[i],trackPtMax[i]),"pl");
      }else if(i > 0){
         leg->AddEntry(hx[ajindex][i],Form("p_{T}^{Track}>%0.1f GeV/c",trackPtMin[i]),"pl");
      }
   }

   for(int i = 0; i < nPtBins; i++){
      g->SetPoint(i,hx[ajindex][i]->GetMean(),i);
      g->SetPointError(i,hx[ajindex][i]->GetMeanError(),0);
   }


   TCanvas* c3 = new TCanvas("c3","",600,600);
   c3->SetLogy();

   hx[ajindex][(int)(nPtBins-1)]->SetMaximum(520);
   hx[ajindex][(int)(nPtBins-1)]->Draw();
   for(int i = 0; i < nPtBins; ++i){
      hx[ajindex][i]->Draw("same");
      hx[ajindex][i]->Draw("same hist");
   }
   leg->Draw();

   TLatex *cms = new TLatex(0.2,0.88,"CMS");
   TLatex *snn = new TLatex(0.2,0.80,"PbPb #sqrt{s}_{_{NN}}=2.76 TeV");
   TLatex *lum = new TLatex(0.2,0.72,"#intL dt = 6.7 #mub^{-1}");
   TLatex *cent = new TLatex(0.2,0.64,"0-30%");
   if(ajindex > 0){
      TLatex *ajtxt = new TLatex(0.2,0.56,ajLabel[ajindex].data());
      ajtxt->SetNDC();
      ajtxt->SetTextFont(63);
      ajtxt->SetTextSize(22);
   }

   TLatex *pythia = new TLatex(0.2,0.88,"PYTHIA");
   TLatex *embedded = new TLatex(0.2,0.88,"Hydjet+PYTHIA");

   cms->SetNDC();
   cms->SetTextFont(63);
   cms->SetTextSize(22);
   snn->SetNDC();
   snn->SetTextFont(63);
   snn->SetTextSize(22);
   lum->SetNDC();
   lum->SetTextFont(63);
   lum->SetTextSize(22);
   cent->SetNDC();
   cent->SetTextFont(63);
   cent->SetTextSize(22);
   pythia->SetNDC();
   pythia->SetTextFont(63);
   pythia->SetTextSize(22);
   embedded->SetNDC();
   embedded->SetTextFont(63);
   embedded->SetTextSize(22);

   if(type == "DATA"){
      cms->Draw();
      snn->Draw();
      lum->Draw();
      cent->Draw();
   }
   if(type == "Pythia"){
      pythia->Draw();
   }
   if(type == "Embed"){
      embedded->Draw();
      cent->Draw();
   }
   if(ajindex > 0){
   ajtxt->Draw();
   }

   c3->Print(Form("MpTpllEvtByEvt_%d_%s_%d.gif",ajindex,type.data(),version));
   c3->Print(Form("MpTpllEvtByEvt_%d_%s_%d.eps",ajindex,type.data(),version));
   c3->Print(Form("MpTpllEvtByEvt_%d_%s_%d.C",ajindex,type.data(),version));

   TCanvas* c4 = new TCanvas("c4","",600,600);
   c4->SetLogy();
   hy[ajindex][nPtBins-1]->SetMaximum(520);
   hy[ajindex][nPtBins-1]->Draw();
   for(int i = 0; i < nPtBins; ++i){
      hy[ajindex][i]->Draw("same");
      hy[ajindex][i]->Draw("same hist");
   }
   leg->Draw();

   if(type == "DATA"){
      cms->Draw();
      snn->Draw();
      lum->Draw();
      cent->Draw();
   }
   if(type == "Pythia"){
      pythia->Draw();
   }
   if(type == "Embed"){
      embedded->Draw();
      cent->Draw();
   }
   if(ajindex > 0){
   ajtxt->Draw();
   }

   c4->Print(Form("MpTtrsEvtByEvt_%d_%s_%d.gif",ajindex,type.data(),version));
   c4->Print(Form("MpTtrsEvtByEvt_%d_%s_%d.eps",ajindex,type.data(),version));
   c4->Print(Form("MpTtrsEvtByEvt_%d_%s_%d.C",ajindex,type.data(),version));

}

