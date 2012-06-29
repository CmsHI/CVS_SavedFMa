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
#include "loopMpt.h"
#include "compare_prof.h"
using namespace std;

void CompareMptMC()
{
   TH1::SetDefaultSumw2();
   TString infdataname = "fig/06.29_MPT0StudyDataMC/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec4_JetSel2HemSum.root";
   TString infrefname  = "fig/06.29_MPT0StudyDataMC/HisMc_icPu5_trkHPCorr_120_50_2749_eta24_prec0_JetSel2HemSum.root";

   TString insrc="0to12";

   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("t"+insrc);
   cout << "Data: " << infdata->GetName() << " " << tdata->GetEntries() << endl;

   TFile * infref = new TFile(infrefname);   
   TTree * tref = (TTree*)infref->Get("t"+insrc);
   cout << "Ref: " << infref->GetName() << " " << tref->GetEntries() << endl;
   
   TString tag = Form("compMptRecGen_SelJet2_"+insrc);
   float minPt1=120,minPt2=50,sigDPhi=7./8*3.14159;
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);

   TCut sel = leadingSel&&awaySel;
   
   // Legend
   TString titledata = "Rec. Trk.";
   TString titleref  = "Gen. Partl.";
   Compare cmpLeg("cmpLeg","");
   cmpLeg.Legend(0.1,0.18,0.6,0.35);
   cmpLeg.leg->AddEntry((TObject*)0,Form("p_{T,1} > %.0f GeV/c",minPt1),"");
   cmpLeg.leg->AddEntry((TObject*)0,Form("p_{T,2} > %.0f GeV/c",minPt2),"");
   cmpLeg.leg->AddEntry((TObject*)0,"#Delta#phi_{1,2} > #frac{7}{8}#pi","");

   // Output
   string path=infdataname.Data();
   TString outdir = path.substr(0, path.find_last_of('/'));
   cout << "Output: " << outdir << endl;
   gSystem->mkdir(outdir,kTRUE);

   // histograms
   for (int i=0; i<4; ++i) {
//       TCanvas * c2 = new TCanvas(Form("cComp%d",i),Form("cComp%d",i),400,400);
      TCanvas * c2 = new TCanvas(Form("cComp%d",i),Form("cComp%d",i),800,400);
      c2->Divide(2,1);
      Compare cmp(Form("cmp_%d",i),sel);
      cmp.vt.push_back(tdata);
      cmp.vt.push_back(tref);
      cmp.vwt.push_back("");
      cmp.vwt.push_back("");
      float lx1=0.17, ly1=0.76, lx2=0.66, ly2=0.94;
      if (i==0) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         c2->SetLogy(0);
         cmp.Init(nAjBin,AjBins);
         cmp.Project("x:Aj>>");
         cmp.Draw(";A_{J};<#slash{p}_{T}^{#parallel}> (GeV/c)",-20,20);
         TLine * l0 = new TLine(0,0,AjBins[nAjBin],0);
         l0->Draw();
      } else if (i==1) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         c2->SetLogy(0);
         cmp.Init(40,-3.14,3.14);
         cmp.Project("x:phi1>>");
         cmp.Draw(";Leading Jet #phi;<#slash{p}_{T}^{#parallel}> (GeV/c)",-100,100);
         TLine * l0 = new TLine(-3.14,0,3.14,0);
         l0->Draw();
      } else if (i==2) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         c2->SetLogy(0);
         cmp.Init(40,50,250);
         cmp.Project("xhem1incone8:pt1>>");
         cmp.Draw(";Leading Jet p_{T} (GeV/c);<#Sigma p_{T}^{#parallel}(incone)> (GeV/c)",0,500);
         lx1=0.52, ly1=0.18, lx2=0.87, ly2=0.36;
         c2->cd(2);
         cmp.Ratio()->Draw("hist");
      } else if (i==3) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel);
         cmp.vsel.push_back(sigSel);
         c2->SetLogy(0);
         cmp.Init(40,50,250);
         cmp.Project("xhem2incone8:pt2>>");
         cmp.Draw(";SubLeading Jet p_{T} (GeV/c);<#Sigma p_{T}^{#parallel}(incone)> (GeV/c)",0,500);
         lx1=0.52, ly1=0.18, lx2=0.87, ly2=0.36;
         c2->cd(2);
         cmp.Ratio()->Draw("hist");
      }
      c2->cd(1);
      cmpLeg.leg->Draw();
      cmp.Legend(lx1,ly1,lx2,ly2);
      if (insrc="0to12") cmp.leg->AddEntry(cmp.vh[0],Form("0 - 30 %%"),"");
      cmp.leg->AddEntry(cmp.vh[0],titledata,"p");
      cmp.leg->AddEntry(cmp.vh[1],titleref,"l");
      cmp.leg->Draw();
      c2->Print(Form("%s/%s_%d.gif",outdir.Data(),tag.Data(),i));
      c2->Print(Form("%s/%s_%d.pdf",outdir.Data(),tag.Data(),i));
   }
}
