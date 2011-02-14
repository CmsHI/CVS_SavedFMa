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
void comparePhi(double ajMin=0,double ajMax=1)
{
   char *title="";
   TFile *inf = new TFile("ntv3_data50v2_djcalo_100_50.root");
   TTree *tData = (TTree*) inf->FindObjectAny("ntjt");
   TFile *inf2 = new TFile("ntv3_dj_HyUQ80v3_djcalo_100_50.root");
   TTree *tMC = (TTree*) inf2->FindObjectAny("ntjt");
   
   const int nBin = 10;
   double delta = 0.0; //shift
   double Bins[nBin+1];
   double BinsMC[nBin+1];
   for (int i=0;i<nBin+1;i++)
   {
      Bins[i]=-PI+2*PI/(double)nBin*i;
      BinsMC[i]=-PI+2*PI/(double)nBin*i+delta;
      cout <<Bins[i]<<endl;
   }



   TCanvas *c = new TCanvas("c","",600,600);

   TH1D *p = new TH1D("p","",nBin,BinsMC);
   TH1D *p2 = new TH1D("p2","",nBin,BinsMC);
   p->Sumw2();
   p2->Sumw2();

   TCut evtCut = Form("Aj>%f&&Aj<%f&&nljet>120&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&!maskEvt&&cent<30&&abs(jdphi)>2./3.*3.14159",ajMin,ajMax);

   tMC->Draw("nljphi>>p",evtCut*"weight");
   tData->Draw("nljphi>>p2",evtCut);
   

   p->SetMarkerStyle(20);
   p2->SetMarkerStyle(25);
   p->Scale(1./p->Integral());
   p2->Scale(1./p2->Integral());
   p->SetAxisRange(0,0.25,"Y");
   p->Draw();
   p2->SetMarkerColor(2);
   p2->SetLineColor(2);
   p2->Draw("same");

   p->SetXTitle("#phi_{Leading Jet}");
   p->SetYTitle("Event fraction");

// ====================
   TLegend *leg = new TLegend(0.45,0.68,0.92,0.9);
   leg->SetFillStyle(0);
   leg->SetBorderSize(0);
   leg->SetTextFont(63);
   leg->SetTextSize(16);
   leg->AddEntry("p",Form("%.2f < A_{J} < %.2f",ajMin,ajMax),"");
   leg->AddEntry("p","PYTHIA + HYDJET","pl");
   leg->AddEntry("p2","Data","pl");
   leg->Draw();

   c->SaveAs(Form("compareJetPhi-%.2f-%.2f.gif",ajMin,ajMax));
   c->SaveAs(Form("compareJetPhi-%.2f-%.2f.eps",ajMin,ajMax));


   TCanvas *c2 = new TCanvas("c2","",600,0,600,600);
   TH1D *hRatio = (TH1D*)p->Clone();
   hRatio->SetName("hRatio");
   hRatio->Divide(p2);
   hRatio->SetAxisRange(0.5,3,"Y");
   hRatio->SetYTitle("MC / Data");
   hRatio->Draw();

   TLine *l = new TLine(Bins[0],1,PI,1);
   l->Draw();

   c2->SaveAs(Form("compareJetPhiRatio-%.2f-%.2f.gif",ajMin,ajMax));
   c2->SaveAs(Form("compareJetPhiRatio-%.2f-%.2f.eps",ajMin,ajMax));

}
