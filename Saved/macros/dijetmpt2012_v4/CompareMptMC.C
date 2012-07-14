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
//    TString infdataname = "fig/07.13_F2STA_MPT80/HisMc_icPu5_120_50_2749_eta24_prec4_resc0SelfCorr.root";
//    TString infrefname  = "fig/07.13_F2STA_MPT80/HisMc_icPu5_120_50_2749_eta24_prec0_resc0SelfCorr.root";
   TString infdataname = "fig/07.13_F2STA_MPT80Ak3Calo/HisMc_akPu3Calo_120_50_2749_eta24_prec4_resc0SelfCorr.root";
   TString infrefname  = "fig/07.13_F2STA_MPT80Ak3Calo/HisMc_akPu3Calo_120_50_2749_eta24_prec0_resc0SelfCorr.root";

   TString insrc="0to12";

   TFile * infdata = new TFile(infdataname);
   TTree * tdata = (TTree*)infdata->Get("t"+insrc);
   cout << "Data: " << infdata->GetName() << " " << tdata->GetEntries() << endl;

   TFile * infref = new TFile(infrefname);   
   TTree * tref = (TTree*)infref->Get("t"+insrc);
   cout << "Ref: " << infref->GetName() << " " << tref->GetEntries() << endl;
   
   string infpath=infdataname.Data();
   TString src=infpath.substr(infpath.find_last_of('/')+1);
   src.ReplaceAll(".root","");

   TString tag = Form("compMptMc_%s_%s",src.Data(),insrc.Data());
   float minPt1=120,minPt2=50,sigDPhi=7./8*3.14159;
   TCut leadingSel  = Form("abs(eta1)<1.6&&pt1>%.3f",minPt1);
   TCut awaySel     = Form("abs(eta2)<1.6&&pt2>%.3f",minPt2);
   TCut sigSel      = Form("acos(cos(phi2-phi1))>%.3f",sigDPhi);
   TCut ajSel       = "Aj>0.";
   int compMode=1; // 0=ratio, 1=diff
   if (compMode==1) tag.ReplaceAll("compMpt","compMptDiff");

   TString titleJet = "icPu5 Calo Jet";
//    TString titleJet = "Gen. Jet";
   TString titleSig = "|#eta_{Jet}|<1.6  #Delta#phi_{1,2} > #frac{7}{8}#pi";
   TString titleAj = "A_{J} > 0.0";

   TCut sel = leadingSel&&awaySel;
   
   
   // Output
   string path=infdataname.Data();
   TString outdir = path.substr(0, path.find_last_of('/'));
   cout << "Output: " << outdir << endl;
   gSystem->mkdir(outdir,kTRUE);

   // histograms
   for (int i=0; i<7; ++i) {
      // Legend
      TString titledata = "Rec. Trk.";
      TString titleref  = "Gen. Partl.";
      Compare cmpLeg("cmpLeg","");
      float lex1=0.1, ley1=0.14, lex2=0.6, ley2=0.41;

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
         cmp.Init(nAjBin,AjBins);
         cmp.Project("x:Aj>>");
         cmp.Draw(";A_{J};<#slash{p}_{T}^{#parallel}> (GeV/c)",-20,20);
         TLine * l0 = new TLine(0,0,AjBins[nAjBin],0);
         l0->Draw();
//          c2->cd(2);
//          TLine * l1;
//          if (compMode==0) {
//             cmp.Ratio()->Draw("hist");
//             l1 = new TLine(0,0,AjBins[nAjBin],0);
//          } else if (compMode==1) {
//             cmp.Difference()->Draw("hist");
//             l1 = new TLine(0,0,AjBins[nAjBin],0);
//          }
//          l1->SetLineStyle(2);
//          l1->Draw();
      } else if (i==1) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.Init(40,-3.14,3.14);
         cmp.Project("x:phi1>>");
         cmp.Draw(";Leading Jet #phi;<#slash{p}_{T}^{#parallel}> (GeV/c)",-100,100);
         TLine * l0 = new TLine(-3.14,0,3.14,0);
         l0->Draw();
      } else if (i==2||i==3) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.Init(20,50,250);
         if (i==2) {
//             cmp.Project("xhem1incone8:pt1>>");
            cmp.Project("xhem1incone8-xhem1incone8pt0-xhem1incone8pt1-xhem1incone8pt2:pt1>>");
            cmp.Draw(";Leading Jet p_{T} (GeV/c);<#Sigma p_{T}^{#parallel}(incone)> (GeV/c)",0,500);
         }
         if (i==3) {
//             cmp.Project("xhem2incone8:pt2>>");
            cmp.Project("xhem2incone8-xhem2incone8pt0-xhem2incone8pt1-xhem2incone8pt2:pt2>>");
            cmp.Draw(";SubLeading Jet p_{T} (GeV/c);<#Sigma p_{T}^{#parallel}(incone)> (GeV/c)",0,500);
         }
         lex1=0.094; ley1=0.65; lex2=0.594; ley2=0.95;
         lx1=0.19, ly1=0.16, lx2=0.54, ly2=0.34;
         c2->cd(2);
         TLine * l1;
         if (compMode==0) {
            cmp.Ratio()->Draw("hist");
            l1 = new TLine(50,1,250,1);
         } else if (compMode==1) {
            cmp.Difference()->Draw("hist");
            l1 = new TLine(50,0,250,0);
         }
         l1->SetLineStyle(2);
         l1->Draw();
      } else if (i==4||i==5) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.vsel.push_back(sigSel&&ajSel);
         gPad->SetLogx(1);
         cmp.Init(nptrange,ptranges);
         if (i==4) {
            cmp.Project("xhem1pt:ptbin>>");
         }
         if (i==5) {
            cmp.Project("xhem2pt:ptbin>>");
         }
         cmp.Draw(";Track p_{T} (GeV/c);<#Sigma p_{T}^{#parallel}(hemisphere)> (GeV/c)",0,500);
         lex1=0.5; ley1=0.65; lex2=0.88; ley2=0.95;
         lx1=0.53, ly1=0.45, lx2=0.87, ly2=0.64;
         c2->cd(2);
         gPad->SetLogx(1);
         TLine * l1;
         if (compMode==0) {
            cmp.Ratio()->Draw("hist");
            l1 = new TLine(ptranges[0],1,ptranges[nptrange],1);
         } else if (compMode==1) {
            cmp.Difference()->Draw("hist");
            l1 = new TLine(ptranges[0],0,ptranges[nptrange],0);
         }
         l1->SetLineStyle(2);
         l1->Draw();
      } else if (i==6) {
         c2->cd(1);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.vsel.push_back(sigSel&&ajSel);
         cmp.Init(15,50,200);
         cmp.Project("x:pt2>>");
         cmp.Draw(";subleading jet p_{T} (GeV/c);<#slash{p}_{T}^{#parallel}> (GeV/c)",-40,40);
         TLine * l0 = new TLine(50,0,200,0);
         l0->Draw();
         c2->cd(2);
         TLine * l1;
         if (compMode==0) {
            cmp.Ratio()->Draw("hist");
            l1 = new TLine(50,1,250,1);
         } else if (compMode==1) {
            cmp.Difference()->Draw("hist");
            l1 = new TLine(50,0,250,0);
         }
         l1->SetLineStyle(2);
         l1->Draw();
      }
      c2->cd(1);
      cmpLeg.Legend(lex1,ley1,lex2,ley2);
      cmpLeg.leg->AddEntry((TObject*)0,titleJet,"");
      cmpLeg.leg->AddEntry((TObject*)0,Form("p_{T,1} > %.0f GeV/c",minPt1),"");
      cmpLeg.leg->AddEntry((TObject*)0,Form("p_{T,2} > %.0f GeV/c",minPt2),"");
      cmpLeg.leg->AddEntry((TObject*)0,titleSig,"");
      cmpLeg.leg->AddEntry((TObject*)0,titleAj,"");
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
