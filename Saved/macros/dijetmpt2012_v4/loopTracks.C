#include <TFile.h>
#include <TH1.h>
#include <TNtuple.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include "TTree.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "loopTracks.h"
using namespace std;

void loopTracks(
               TString outdir = "./fig/07.13_F2STA_ZeroEffProblem"
             )
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   const int nCentBin = 1;
   int centBins[2] = {0,12};

   TString infname = "../ntout/output-hy18dj80_Forest2v21_v3_allTrks_Eta8_jpt50eta2_xsec_icPu5.root";
//    TString infname = "../ntout/output-hy18dj80_Forest2v21_v3_allTrks_Eta8_jpt110eta2_xsec_50k_icPu5.root";
   
   int particleRecLevel = 2; // 0 gen, 1 sim, 2 sim mat, 3 rec mat, 4 rec
   
   float minJetPt1=110; // 120, 110
   float minJetPt2=-1;  // 50, -1
   float sigDPhi=-1; // Pi()*7./8, -1
   float etamax=2.4;
   TString tag = Form("%s/HisMc_icPu5_trkHPCorr_%.0f_%.0f_%.0f_eta%.0f_prec%d",outdir.Data(),minJetPt1,minJetPt2,sigDPhi*1000,etamax*10,particleRecLevel);
   tag+="SelfCorr";

   TFile *inf = TFile::Open(infname);
   TTree *nt =(TTree*)inf->FindObjectAny("tgj");
   cout << endl << "# " << endl;
   cout << "# " << infname << ": " << nt->GetEntries() << " events." << endl;
   cout << "# " << endl;

   TString outfname = tag+".root";   
   cout << "Output file: " << outfname << endl;
   TFile* hout = new TFile(outfname,"RECREATE");

   for (int c=0; c<nCentBin; ++c) {
      TString name = Form("%dto%d",centBins[c],centBins[c+1]);
      AnaTrk ana(name);
//       ana.maxEntry = 10000;
      ana.isMC = true;
      ana.particleRecLevel = particleRecLevel;
      ana.cMin=centBins[c];
      ana.cMax=centBins[c+1];
      ana.minJetPt1 = minJetPt1;
      ana.minJetPt2 = minJetPt2;
      ana.maxJetEta = 2;
      ana.sigDPhi = sigDPhi;
      ana.minPt = 0.5; // 0.5, 8
      ana.maxEta = etamax;
      ana.outf = hout;
      ana.Init(nt);
      ana.Loop();

      // Inspect
      TCanvas * c2 = new TCanvas("c"+name,"c"+name,800,400);
      TH1D * hGenp = ana.hGenpPt;
      TH1D * hTrk = ana.hTrkPt;
      TH1D * hCorr = ana.hTrkCorrPt;
      float xmin=0.5, xmax=119.9;
//       TH1D * hGenp = ana.hGenpEta;
//       TH1D * hTrk = ana.hTrkEta;
//       TH1D * hCorr = ana.hTrkCorrEta;
//       float xmin=-2.4, xmax=2.4;
      c2->Divide(2,1);
      c2->cd(1);
      gPad->SetLogx();
      gPad->SetLogy();
      hGenp->SetLineColor(2);
      hTrk->SetMarkerStyle(kOpenCircle);
      hGenp->SetAxisRange(xmin,xmax,"X");
      hGenp->Draw("hist");
      hTrk->Draw("sameE");
      hCorr->Draw("sameE");
      c2->cd(2);
      gPad->SetLogx();
      TH1D * hGenpRat = (TH1D*)hGenp->Clone("hGenpRat");
      hGenpRat->Divide(hGenp);
      TH1D * hTrkRat = (TH1D*)hTrk->Clone("hTrkRat");
      hTrkRat->Divide(hGenp);
      TH1D * hTrkCorrRat = (TH1D*)hCorr->Clone("hTrkCorrRat");
      hTrkCorrRat->Divide(hGenp);
      hGenpRat->SetAxisRange(xmin,xmax,"X");
      hGenpRat->SetAxisRange(0.5,1.2,"Y");
      hGenpRat->Draw("hist");
      hTrkRat->Draw("sameE");
      hTrkCorrRat->Draw("sameE");
      c2->Print(tag+Form("_clos_c%d.gif",c));

      // Compare to Table
      TCanvas * c3 = new TCanvas("c3_"+name,"c3_"+name,800,400);
      TH1D * hHisSim = (TH1D*)ana.vtrkCorr[0]->inputHists_[0][0][0][1]->Project3D("sim_y");
      TH1D * hHisRec = (TH1D*)ana.vtrkCorr[0]->inputHists_[0][0][0][0]->Project3D("rec_y");
      cout << "Table simtracks: " << hHisSim->GetEntries() << endl;
      cout << "Ana simtracks: " << hGenp->GetEntries() << endl;
      cout << "Table tracks: " << hHisRec->GetEntries() << endl;
      cout << "Ana tracks: " << hTrk->GetEntries() << endl;
      c3->Divide(2,1);
      c3->cd(1);
      gPad->SetLogy();
      hHisSim->SetLineColor(2);
      hHisSim->SetAxisRange(xmin,xmax,"X");
      hHisSim->Draw("hist");
      hGenp->Draw("sameE");
      c3->cd(2);
      gPad->SetLogy();
      hHisRec->SetAxisRange(xmin,xmax,"X");
      hHisRec->SetLineColor(2);
      hHisRec->Draw("hist");
      hTrk->Draw("sameE");
      c2->Print(tag+Form("_compTable_c%d.gif",c));
   }
   hout->Write();   
}

