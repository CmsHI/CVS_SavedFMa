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

void CompareSimTrack(
   TString outdir="fig/06.20_trk_disc",
   TString tag = "compSimTrk"
)
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   // inputs
   TString infdataname="../ntout/output-hy18dj80_Forest2v21_v1_allTrks_simTrk_xsec_icPu5.root";
   TString infrefname = "../ntout/output-hy18dj80_Forest2v21_v1_allTrks_genp_xsec_icPu5.root";

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
   for (int i=0; i<1; ++i) {
      TCanvas * c2 = new TCanvas(Form("cComp%d",i),Form("cComp%d",i),400,400);
      Compare cmp(Form("cmp_%d",i),sel,1);
      cmp.vt.push_back(tdata);
      cmp.vt.push_back(tref);
      cmp.vwt.push_back("");
      cmp.vwt.push_back("");
      float lx1=0.45, ly1=0.6, lx2=0.94, ly2=0.92;
      if (i==0) {
         c2->SetLogx();
         c2->SetLogy();
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         cmp.Init(100,0,100);
         cmp.Project("genpPt>>");
         cmp.Draw(";p_{T} (GeV/c);u.n.",1e-8,5);
         lx1=0.16; ly1=0.175; lx2=0.65; ly2=0.49;
      }
      cmp.Legend(lx1,ly1,lx2,ly2);
      cmp.leg->AddEntry(cmp.vh[0],Form("%.0f - %.0f %%",2.5*cMin,2.5*cMax),"");
      cmp.leg->AddEntry(cmp.vh[0],"|#eta| < 2.4","");
//       cmp.leg->AddEntry(cmp.vh[0],Form("p_{T,1} > %.0f GeV/c",minPt1),"");
//       cmp.leg->AddEntry(cmp.vh[0],Form("p_{T,2} > %.0f GeV/c",minPt2),"");
//       cmp.leg->AddEntry(cmp.vh[0],"#Delta#phi_{1,2} > #frac{2}{3}#pi","");
      cmp.leg->AddEntry(cmp.vh[0],"Sim Track (Primary)","p");
      cmp.leg->AddEntry(cmp.vh[1],"Gen. Particle","p");
      cmp.leg->Draw();
      c2->Print(Form("%s/%s_%d_c%dto%d.gif",outdir.Data(),tag.Data(),i,cMin,cMax));
      c2->Print(Form("%s/%s_%d_c%dto%d.pdf",outdir.Data(),tag.Data(),i,cMin,cMax));
   }
}
