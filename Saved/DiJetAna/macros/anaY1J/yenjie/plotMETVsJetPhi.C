#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLegend.h>
#define PI 3.14159265358979

void setupAlias(TTree* t)
{   
   t->SetAlias("metxMergedAll","(metConex0+metConex1+metConex2+metConex3+metConex4+metConex5)");
   t->SetAlias("metxMerged0","(metConex0)");
   t->SetAlias("metxMerged1","(metConex1)");
   t->SetAlias("metxMerged2","(metConex2)");
   t->SetAlias("metxMerged3","(metConex3)");
   t->SetAlias("metxMerged4","(metConex4+metConex5)");
}
void plotMETVsJetPhi(int num=-1)
{
   char *title="";
   if (num!=-1) {
      title = Form("%d",num);
   }
   TFile *inf = new TFile("nt_dj_mix100_Gen.root");
   TTree *tGen = (TTree*) inf->FindObjectAny("ntjt");
   tGen->SetName("gen");
   TFile *inf2 = new TFile("nt_dj_mix100.root");
   TTree *tReco = (TTree*) inf2->FindObjectAny("ntjt");
   tReco->SetName("reco");
   TFile *inf3 = new TFile("nt_dj_HyUQ80v4_djcalo_genp_100_50.root");
   TTree *tGen2 = (TTree*) inf3->FindObjectAny("ntjt");
   tGen2->SetName("gen2");
   TFile *inf4 = new TFile("dj_HyUQ80v4_djcalo_genp_100_50.root");
   TTree *tDj = (TTree*) inf4->FindObjectAny("djTree");
   tDj->SetName("dj");

   TFile *inf5 = new TFile("nt_dj_data100_cor.root");
   TTree *tRecoData = (TTree*) inf5->FindObjectAny("ntjt");
   tRecoData->SetName("data");
   TFile *inf6 = new TFile("dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_djcalo_100_50.root");
   TTree *tDjData = (TTree*) inf6->FindObjectAny("djTree");
   tDjData->SetName("djdata");
   tGen->AddFriend(tReco); 
   tGen->AddFriend(tGen2);
   tGen->AddFriend(tDj);
   tRecoData->AddFriend(tDjData);
   setupAlias(tGen);
   setupAlias(tReco);
   
   const int nBin = 10;
   double delta = 0.0; //shift
//   double Bins[nBin+1] = {0,1,2,2.2,2.4,2.6,2.8,3,PI};
//   double BinsMC[nBin+1] = {0+delta,1+delta,2+delta,2.2+delta,2.4+delta,2.6+delta,2.8+delta,3+delta,PI+delta};
   double Bins[nBin+1];
   double BinsMC[nBin+1];
   for (int i=0;i<nBin+1;i++)
   {
      Bins[i]=-PI+2*PI/(double)nBin*i;
      BinsMC[i]=-PI+2*PI/(double)nBin*i+delta;
      cout <<Bins[i]<<endl;
   }


   const int nPtBin = 6;
   double ptBins[nPtBin+1] = {0.5,1.0,1.5,4,8,20,1000};

   TCanvas *c = new TCanvas("c","",600,600);

   TProfile *p = new TProfile("p","",nBin,BinsMC);
   TProfile *p2 = new TProfile("p2","",nBin,BinsMC);
   TProfile *p3 = new TProfile("p3","",nBin,BinsMC);
   TProfile *p4 = new TProfile("p4","",nBin,Bins);

   TCut evtCut = "nljet>120&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&!maskEvt&&cent<30&&abs(jdphi)>2./3.*3.14159";

   tGen->Draw(Form("-gen.metOutOfConex%s:nljphi+0.0>>p",title),"weight"*evtCut);
   tGen->Draw(Form("-gen2.metOutOfConex%s:nljphi+0.0>>p2",title),"weight"*evtCut);
   tGen->Draw(Form("-reco.metOutOfConex%s:nljphi+0.0>>p3",title),"weight"*evtCut);
   tRecoData->Draw(Form("-metOutOfConex%s:nljphi>>p4",title),"1"*evtCut);
   p->SetMarkerStyle(24);
   p2->SetMarkerStyle(25);
   p3->SetMarkerStyle(26);
   p4->SetMarkerStyle(20);

   p->SetAxisRange(-80,120,"Y");
   p->Draw();
   p2->SetMarkerColor(2);
   p2->SetLineColor(2);
   p3->SetMarkerColor(4);
   p3->SetLineColor(4);
   p2->Draw("same");
   p3->Draw("same");
   p4->Draw("same");

   TLine *l = new TLine(Bins[0],0,PI,0);
   l->Draw();
   p->SetXTitle("#phi_{Leading Jet}");
   p->SetYTitle("<#slash{p}_{T}^{#parallel}> (GeV/c)");

// ====================
   TLegend *leg = new TLegend(0.45,0.68,0.92,0.9);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);
   leg->AddEntry("p","Jets with |#eta|<1.6","");
   if (num!=-1) {
      leg->AddEntry("p",Form("Tracks with %.1f<p_{T}<%.1f GeV/c",ptBins[num],ptBins[num+1]),"");
   } else {
      leg->AddEntry("p",Form("Tracks with %.1f<p_{T}<%.1f GeV/c",ptBins[0],ptBins[nPtBin]),"");
   }
   leg->AddEntry("p","#slash{p}_{T}^{#parallel} PYTHIA+HYDJET GEN Signal","pl");
   leg->AddEntry("p2","#slash{p}_{T}^{#parallel} PYTHIA+HYDJET GEN S+B","pl");
   leg->AddEntry("p3","#slash{p}_{T}^{#parallel} PYTHIA+HYDJET RECO","pl");
   leg->AddEntry("p4","#slash{p}_{T}^{#parallel} Data RECO","pl");
   leg->Draw();

   c->SaveAs(Form("metOutOfCone%sVsJetPhi.gif",title));
   c->SaveAs(Form("metOutOfCone%sVsJetPhi.eps",title));
}
