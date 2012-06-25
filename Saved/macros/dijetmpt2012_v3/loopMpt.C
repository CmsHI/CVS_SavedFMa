#include <TFile.h>
#include <TH1.h>
#include <TNtuple.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include "TTree.h"
#include "TSystem.h"
#include "loopMpt.h"
using namespace std;

void loopMpt(
            TString outdir = "./fig/06.25_simtrkloop"
             )
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);
   
   const int nCentBin = 2;
   int centBins[3] = {0,12,40};

   TString infdataname = "../ntout/output-data-Forest2v3_v2_saveTrks_jpt120_icPu5.root";
   TString infmcname="../ntout/output-hy18dj80_Forest2v21_v2_allTrks_simtrk_jpt100_xsec_icPu5.root";
   
   bool isMC=true;
   
   TString infname=infdataname;
   if (isMC) infname=infmcname;
   
   float minJetPt1=120;
   float minJetPt2=50;
   float sigDPhi=3.1415926*2./3;
   float etamax=2.4;
   TString tag = Form("%s/HisData_icPu5_trkHPCorr_%.0f_%.0f_%.0f_eta%.0f",outdir.Data(),minJetPt1,minJetPt2,sigDPhi*1000,etamax*10);
   if (isMC) tag.ReplaceAll("HisData","HisMc");

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
      AnaMPT ana(name);
      ana.isMC = isMC;
      ana.cMin=centBins[c];
      ana.cMax=centBins[c+1];
      ana.minJetPt1 = minJetPt1;
      ana.minJetPt2 = minJetPt2;
      ana.maxJetEta = 1.6;
      ana.sigDPhi = sigDPhi;
      ana.minPt = 0.5;
      ana.maxEta = etamax;
      ana.Init(nt);
//       ana.Loop();
      ana.Loop(5000);
   }
   
   hout->Write();
}

