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
#include "compare_prof.h"
using namespace std;

void CompareMpt()
{
   TH1::SetDefaultSumw2();
   TString infdataname="fig/06.26_tree/HisData_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";
   TString infrefname="fig/06.26_tree/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec4.root";

   string path=infdataname.Data();
   TString outdir = path.substr(0, path.find_last_of('/'));
   cout << "Output: " << outdir << endl;
   gSystem->mkdir(outdir,kTRUE);

   TString insrc="0to12";
   TString tag = Form("compMpt"+insrc);

   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("t"+insrc);
   cout << "Data: " << infdata->GetName() << " " << tdata->GetEntries() << endl;

   TFile * infref = new TFile(infrefname);   
   TTree * tref = (TTree*)infref->Get("t"+insrc);
   cout << "Ref: " << infref->GetName() << " " << tref->GetEntries() << endl;
   
   float minPt1=120,minPt2=50,sigDPhi=7./8*3.14159;
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

   TCut sel = leadingSel&&awaySel;
      
   // histograms
   for (int i=0; i<1; ++i) {
      TCanvas * c2 = new TCanvas(Form("cComp%d",i),Form("cComp%d",i),400,400);
      Compare cmp(Form("cmp_%d",i),sel);
      cmp.vt.push_back(tdata);
      cmp.vt.push_back(tref);
      cmp.vwt.push_back("");
      cmp.vwt.push_back("weight");
      float lx1=0.45, ly1=0.6, lx2=0.94, ly2=0.92;
      if (i==0) {
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         c2->SetLogy(0);
         cmp.Init(40,-3.14,3.14);
         cmp.Project("x:phi1>>");
         cmp.Draw(";Leading Jet #phi;<#slash{p}_{T}^{#parallel}> (GeV/c)",-100,100);
         TLine * l0 = new TLine(-3.14,0,3.14,0);
         l0->Draw();
         lx1=0.19, ly1=0.15, lx2=0.63, ly2=0.47;
      }
      cmp.Legend(lx1,ly1,lx2,ly2);
      if (insrc="0to12") cmp.leg->AddEntry(cmp.vh[0],Form("0 - 30 %%"),"");
      cmp.leg->AddEntry(cmp.vh[0],Form("p_{T,1} > %.0f GeV/c",minPt1),"");
      cmp.leg->AddEntry(cmp.vh[0],Form("p_{T,2} > %.0f GeV/c",minPt2),"");
      cmp.leg->AddEntry(cmp.vh[0],"#Delta#phi_{1,2} > #frac{7}{8}#pi","");
      cmp.leg->AddEntry(cmp.vh[0],"PbPb Data","p");
      cmp.leg->AddEntry(cmp.vh[1],"PYTHIA+HYDJET","l");
      cmp.leg->Draw();
      c2->Print(Form("%s/%s_%d.gif",outdir.Data(),tag.Data(),i));
      c2->Print(Form("%s/%s_%d.pdf",outdir.Data(),tag.Data(),i));
   }
}
