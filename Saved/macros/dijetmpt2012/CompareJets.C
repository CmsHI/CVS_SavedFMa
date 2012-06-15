#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
#include "TSystem.h"
#include "commonUtility.h"
#include "compare.h"
using namespace std;

void CompareJets(
   TString outdir="06.14an",
   TString tag = "compJets_DataMc"
)
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   // inputs
   TString infdataname="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
   TString infrefname = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";

   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("tgj");
   cout << infdata->GetName() << endl;

   TFile * infref = new TFile(infrefname);   
   TTree * tref = (TTree*)infref->Get("tgj");
   cout << infref->GetName() << endl;
   
   float minPt1=120,minPt2=50,sigDPhi=2./3*3.14159;
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);
   int cMin=0, cMax=12;
   TCut mycut = Form("cBin>=%d&&cBin<%d",cMin,cMax);

   TCut sel = leadingSel&&awaySel;
      
   // histograms
   for (int i=0; i<4; ++i) {
      TCanvas * c2 = new TCanvas(Form("cComp%d",i),Form("cComp%d",i),400,400);
      Compare cmp(Form("cmp_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vt.push_back(tref);
      cmp.vwt.push_back("");
      cmp.vwt.push_back("weight");
      float lx1=0.45, ly1=0.6, lx2=0.94, ly2=0.92;
      if (i==0) {
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         c2->SetLogy(0);
         cmp.Init(40,0,40);
         cmp.Project("cBin>>");
         cmp.Draw(";Centrality Bin;u.n.",1e-3,0.2);
      }
      if (i==1) {
         cmp.vsel.push_back(mycut);
         cmp.vsel.push_back(mycut);
         c2->SetLogy(1);
         cmp.Init(40,0,3.14159);
         cmp.Project("acos(cos(phi1-phi2))>>");
         cmp.Draw(";#Delta#phi(j1,j2);u.n.",5e-5,0.5);
         lx1=0.22; ly1=0.67; lx2=0.71; ly2=0.92;
      }
      if (i==2) {
         cmp.vsel.push_back(mycut&&sigSel);
         cmp.vsel.push_back(mycut&&sigSel);
         c2->SetLogy(1);
         cmp.Init(60,100,340);
         cmp.Project("pt1>>");
         cmp.Draw(";Leading Jet p_{T} (GeV/c);u.n.",1e-4,0.3);
      }
      if (i==3) {
         cmp.vsel.push_back(mycut&&sigSel);
         cmp.vsel.push_back(mycut&&sigSel);
         c2->SetLogy(0);
         cmp.Init(40,0,0.8);
         cmp.Project("Aj>>");
         cmp.Draw(";A_{J};u.n.",1e-4,0.15);
      }
      cmp.Legend(lx1,ly1,lx2,ly2);
      if (i!=0) cmp.leg->AddEntry(cmp.vh[0],Form("%.0f - %.0f %%",2.5*cMin,2.5*cMax),"");
//       if (i!=0) cmp.leg->AddEntry(cmp.vh[0],Form("%.0f - %.0f \%",cMin*2.5,cMax*2.5),"");
      cmp.leg->AddEntry(cmp.vh[0],Form("p_{T,1} > %.0f GeV/c",minPt1),"");
      cmp.leg->AddEntry(cmp.vh[0],Form("p_{T,2} > %.0f GeV/c",minPt2),"");
      if (i!=1) cmp.leg->AddEntry(cmp.vh[0],"#Delta#phi_{1,2} > #frac{2}{3}#pi","");
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data","p");
      cmp.leg->AddEntry(cmp.vh[1],"PYTHIA+HYDJET","p");
      cmp.leg->Draw();
      c2->Print(Form("%s/%s_%d_c%dto%d.gif",outdir.Data(),tag.Data(),i,cMin,cMax));
      c2->Print(Form("%s/%s_%d_c%dto%d.pdf",outdir.Data(),tag.Data(),i,cMin,cMax));
   }
}
