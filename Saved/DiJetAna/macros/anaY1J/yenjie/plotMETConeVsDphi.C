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
void plotMETConeVsDphi()
{
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
   tGen->AddFriend(tReco); 
   tGen->AddFriend(tGen2);
   tGen->AddFriend(tDj);
   setupAlias(tGen);
   setupAlias(tReco);
   
   const int nBin = 4;
   double delta = 0.02; //shift
//   double Bins[nBin+1] = {0,1,2,2.2,2.4,2.6,2.8,3,PI};
//   double BinsMC[nBin+1] = {0+delta,1+delta,2+delta,2.2+delta,2.4+delta,2.6+delta,2.8+delta,3+delta,PI+delta};
   double Bins[nBin+1] = {2,2.2,2.4,5./6*PI,PI};
   double BinsMC[nBin+1] = {2+delta,2.2+delta,2.4+delta,5./6*PI+delta,PI+delta};

   TCanvas *c = new TCanvas("c","",600,600);

   TProfile *p = new TProfile("p","",nBin,BinsMC);
   TProfile *p2 = new TProfile("p2","",nBin,BinsMC);
   TProfile *p3 = new TProfile("p3","",nBin,BinsMC);
   TProfile *p4 = new TProfile("p4","",nBin,Bins);

   TCut evtCut = "nljet>120&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&!maskEvt&&cent<30&&Aj>0.0";

   tGen->Draw("-gen.metOutOfConex:jdphi+0.02>>p","weight"*evtCut);
   tGen->Draw("-gen2.metOutOfConex:jdphi+0.02>>p2","weight"*evtCut);
   tGen->Draw("-reco.metOutOfConex:jdphi+0.02>>p3","weight"*evtCut);
   tRecoData->Draw("-metOutOfConex:jdphi>>p4","1"*evtCut);
   p->SetMarkerStyle(24);
   p2->SetMarkerStyle(25);
   p3->SetMarkerStyle(26);
   p4->SetMarkerStyle(20);

   p->SetAxisRange(-50,70,"Y");
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
   p->SetXTitle("#Delta #phi_{12}");
   p->SetYTitle("<#slash{p}_{T}^{#parallel}> (GeV/c)");

// ====================
   TLegend *leg = new TLegend(0.40,0.68,0.90,0.9);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);
   leg->AddEntry("p","Jets with |#eta|<1.0 only, A_{J}>0.0","");
   leg->AddEntry("p","#slash{p}_{T}^{#parallel} PYTHIA+HYDJET GEN Signal","pl");
   leg->AddEntry("p2","#slash{p}_{T}^{#parallel} PYTHIA+HYDJET GEN S+B","pl");
   leg->AddEntry("p3","#slash{p}_{T}^{#parallel} PYTHIA+HYDJET RECO","pl");
   leg->AddEntry("p4","#slash{p}_{T}^{#parallel} Data RECO","pl");
   leg->Draw();
}
