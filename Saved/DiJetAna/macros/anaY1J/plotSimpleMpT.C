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
#include <TCut.h>

void plotSimpleMpT(){

   TH1::SetDefaultSumw2();
   int color[10] = {1,kGray,kBlue-3,38,kOrange-8,kRed-6,8,9,2};

   //   TFile* inf =  new TFile("/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root");
   TFile* inf =  new TFile("/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root");
   TFile* outf = new TFile("pythia_cutOnly_05.root","recreate");
   TCut centrality("cent<300");
   
   //   TFile* inf =  new TFile("/net/hisrv0001/home/frankma/scratch01/ana/merge/dj_HydQ_DJUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root");
   //   TFile* outf = new TFile("embed_cutOnly_05.root","recreate");
   //   centrality = "cent<30";

   TH1D* h[10][20];
   TH1D* hx[10][20];
   TH1D* hy[10][20];
   TH1D* hw[10][20];
   TH1D* hwx[10][20];
   TH1D* hwy[10][20];

   int nHbins = 100;
   int nPtBins = 5;   
   double trackPtMin[20] = {0.5,0.5,1.5,4,8.,20};
   double trackPtMax[20] = {500,1.5,4,8.,500,500};
   bool upperBounded = false;


   for(int j = 0; j < 5; j++){
      for(int i = 0; i < nPtBins; i++){
	 h[j][i] = new TH1D(Form("h%d_%d",j,i),"",nHbins,-250,250);
	 h[j][i]->SetLineColor(color[i]);
	 h[j][i]->SetMarkerColor(color[i]);

	 hx[j][i] = new TH1D(Form("hx%d_%d",j,i),";#slash{p}_{T}^{#parallel} (GeV/c);Event Fraction",nHbins,-250,250);
         hx[j][i]->SetLineColor(color[i]);
         hx[j][i]->SetMarkerColor(color[i]);

	 hy[j][i] = new TH1D(Form("hy%d_%d",j,i),";#slash{p}_{T}^{#perp} (GeV/c);Event Fraction",nHbins,-250,250);
         hy[j][i]->SetLineColor(color[i]);
         hy[j][i]->SetMarkerColor(color[i]);

	 hw[j][i] = new TH1D(Form("hw%d_%d",j,i),"",nHbins,-250,250);
         hw[j][i]->SetLineColor(color[i]);
         hw[j][i]->SetMarkerColor(color[i]);

         hwx[j][i] = new TH1D(Form("hwx%d_%d",j,i),";#slash{p}_{T}^{#parallel} (GeV/c);",nHbins,-250,250);
         hwx[j][i]->SetLineColor(color[i]);
         hwx[j][i]->SetMarkerColor(color[i]);

         hwy[j][i] = new TH1D(Form("hwy%d_%d",j,i),";#slash{p}_{T}^{#perp} (GeV/c);",nHbins,-250,250);
         hwy[j][i]->SetLineColor(color[i]);
         hwy[j][i]->SetMarkerColor(color[i]);

      }
   }
   
   TTree* t0 = (TTree*)inf->Get("hltanalysis/HltTree");
   TTree* t = (TTree*)inf->Get("djcalo/djTree");

   t->AddFriend(t0);
   t->SetAlias("aj","(nljet-aljet)/(nljet+aljet)");
   t->SetAlias("metx",Form("Sum$(ppt*cos(pndphi)*(ppt>%f))",trackPtMin[0]));
   t->SetAlias("mety",Form("Sum$(ppt*sin(pndphi)*(ppt>%f))",trackPtMin[0]));

   TCanvas* c1 = new TCanvas("c1","",800,400);
   c1->Divide(2,1);
   c1->cd(1);
   if(0){
      t->Draw("metx:(nljet-aljet)/(nljet+aljet)>>p",centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1","prof");
      c1->cd(2);
      t->Draw("mety:(nljet-aljet)/(nljet+aljet)>>p",centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1","prof");
   }
   
   TCanvas* c2 = new TCanvas("c2","",400,400);
   for(int i = 0; i < nPtBins; ++i){

      t->SetAlias("metx",Form("-Sum$(ppt*cos(pndphi)*(ppt>%f))",trackPtMin[i]));
      t->SetAlias("mety",Form("-Sum$(ppt*sin(pndphi)*(ppt>%f))",trackPtMin[i]));
      if(upperBounded){
	 t->SetAlias("metx",Form("-Sum$(ppt*cos(pndphi)*(ppt>%f && ppt<%f))",trackPtMin[i],trackPtMax[i]));
	 t->SetAlias("mety",Form("-Sum$(ppt*sin(pndphi)*(ppt>%f && ppt<%f))",trackPtMin[i],trackPtMax[i]));
      }
      t->SetAlias("met","sqrt(metx*metx + mety*mety)");

      if(1){
	 t->Draw(Form("metx>>hx%d_%d",0,i),centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1","");
      t->Draw(Form("metx>>hx%d_%d",1,i),centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj > 0.24","same");
      t->Draw(Form("metx>>hx%d_%d",2,i),centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj < 0.24","same");
      }
      t->Draw(Form("metx>>hwx%d_%d",0,i),"metx*(nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1)"*centrality,"");
      t->Draw(Form("metx>>hwx%d_%d",1,i),"metx*(nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj > 0.24)"*centrality,"same");
      t->Draw(Form("metx>>hwx%d_%d",2,i),"metx*(nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj < 0.24)"*centrality,"same");
      
      if(1){
	 t->Draw(Form("mety>>hy%d_%d",0,i),centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1","");
	 t->Draw(Form("mety>>hy%d_%d",1,i),centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj > 0.24","same");
	 t->Draw(Form("mety>>hy%d_%d",2,i),centrality&&"nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj < 0.24","same");

	 t->Draw(Form("mety>>hwy%d_%d",0,i),"mety*(nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1)"*centrality,"");
	 t->Draw(Form("mety>>hwy%d_%d",1,i),"mety*(nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj > 0.24)"*centrality,"same");
	 t->Draw(Form("mety>>hwy%d_%d",2,i),"mety*(nljet>100&&abs(nljeta)<2&&aljet>50&&abs(aljeta)<2&&jdphi>2.1 && aj < 0.24)"*centrality,"same");

      }
   }

   TCanvas* c3 = new TCanvas("c3","",400,400);
   hwx[0][nPtBins-1]->Draw();
   for(int i = 0; i < nPtBins; ++i){
      hwx[0][i]->Draw("same");
   }

   TLegend *leg=new TLegend(0.60,0.64,0.98,0.93);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->SetTextFont(63);
   leg->SetTextSize(15);
   for(int i = 0; i < nPtBins; ++i){
      if(upperBounded){
	 leg->AddEntry(hx[0][i],Form("%0.1f<p_{T}^{Track}<%0.1f",trackPtMin[i],trackPtMax[i]),"pl");
      }else{
	 leg->AddEntry(hx[0][i],Form("%0.1f<p_{T}^{Track}",trackPtMin[i]),"pl");
      }
   }
   leg->Draw();

   outf->Write();


}

