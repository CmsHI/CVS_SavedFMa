#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TProfile.h>
#include <TCut.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>

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
void plotMETClosure()
{
   TFile *inf = new TFile("nt_dj_mix100_Gen_05.root");
   TTree *tGen = (TTree*) inf->FindObjectAny("ntjt");
   tGen->SetName("gen");
   TFile *inf2 = new TFile("nt_dj_mix100_05.root");
   TTree *tReco = (TTree*) inf2->FindObjectAny("ntjt");
   tReco->SetName("reco");
   TFile *inf3 = new TFile("nt_dj_HyUQ80v4_djcalo_genp_100_50_05.root");
   TTree *tGen2 = (TTree*) inf3->FindObjectAny("ntjt");
   tGen2->SetName("gen2");
   tGen->AddFriend(tReco);
   tGen->AddFriend(tGen2);
   setupAlias(tGen);
   setupAlias(tReco);
   
   const int nBin = 6;
   double Bins[nBin+1] = {2,2.2,2.4,2.6,2.8,3,PI};

   TCanvas *c = new TCanvas("c","",600,600);

   TProfile *p = new TProfile("p","",nBin,Bins);
   TProfile *p2 = new TProfile("p2","",nBin,Bins);
   TProfile *p3 = new TProfile("p3","",nBin,Bins);

   TCut evtCut = "nljet>120&&abs(nljetacorr)<2&&aljet>50&&abs(aljetacorr)<2&&!maskEvt&&cent<30";

   tGen->Draw("gen.metConex0-reco.metConex0:jdphi>>p","weight"*evtCut);
   tGen->Draw("gen.metx0-reco.metx0:jdphi>>p2","weight"*evtCut);
   tGen->Draw("gen.metOutOfConex0-reco.metOutOfConex0:jdphi>>p3","weight"*evtCut);
   p->SetMarkerStyle(26);
   p3->SetMarkerStyle(25);

   p->SetAxisRange(-50,50,"Y");
   p->Draw();
   p2->SetMarkerColor(2);
   p2->SetLineColor(2);
   p3->SetMarkerColor(4);
   p3->SetLineColor(4);
   p2->Draw("same");
   p3->Draw("same");

   TLine *l = new TLine(2,0,PI,0);
   l->Draw();
   p->SetXTitle("#Delta #phi");
   p->SetYTitle("<#slash{p}_{T}^{#parallel,Gen}-#slash{p}_{T}^{#parallel,Reco}> (GeV/c)");

// ====================
   TLegend *leg = new TLegend(0.10,0.68,0.70,0.96);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);
   leg->Add("p","#slash{p}_{T}^{#parallel} from all tracks");
   leg->Add("p","#slash{p}_{T}^{#parallel} from tracks in the jet cone (#Delta R< 0.5)");
}
