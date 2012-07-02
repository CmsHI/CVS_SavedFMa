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
            TString outdir = "./fig/07.02_JetDPhiFlattening"
             )
{
   TH1::SetDefaultSumw2();
   gSystem->mkdir(outdir,kTRUE);

   const int nCentBin = 1;
   int centBins[3] = {0,12,40};

//    TString infdataname="../ntout/output-data-Forest2v2v3_saveTrks_v0_icPu5.root";
//    TString infmcname = "../ntout/output-hy18dj80_forest2_v0_xsec_icPu5.root";
//    TString infdataname = "../ntout/output-data-Forest2v3_v2_saveTrks_jpt120_icPu5.root";
//    TString infmcname="../ntout/output-hy18dj80_Forest2v21_v1_allTrks_simtrk_jpt95_xsec_icPu5.root";
//    TString infmcname="../ntout/output-hy18dj80_Forest2v21_v2_allTrks_simtrk_jpt100_xsec_icPu5.root";
   TString infdataname="../ntout/output-data-Forest2v3_v3_saveTrks_jpt120_icPu5.root";
//    TString infmcname = "../ntout/output-hy18dj80_Forest2v21_v3_allTrks_simtrk_jpt120_xsec_icPu5.root";
   TString infmcname = "../ntout/output-hy18dj80_Forest2v21_v3_allTrks_Eta8_jpt120eta3_xsec_icPu5.root";
//    TString infmcname = "../ntout/output-hy18dj80_Forest2v21_v3_allTrks_Eta8_gjpt120eta3_xsec_icPu5.root";
   
   bool isMC=true;
   int particleRecLevel = 1; // 0 gen, 1 sim, 2 sim mat, 3 rec mat, 4 rec
   if (!isMC) particleRecLevel=4;
   
   TString infname=infdataname;
   if (isMC) infname=infmcname;
   
   float minJetPt1=120;
   float minJetPt2=50;
   float sigDPhi=3.1415926*7./8;
   float etamax=2.4;
   bool doResCorr = false;
   bool doJetPhiFlat = true;
   TString tag = Form("%s/HisData_icPu5_trkHPCorr_%.0f_%.0f_%.0f_eta%.0f_prec%d_resc%d",outdir.Data(),minJetPt1,minJetPt2,sigDPhi*1000,etamax*10,particleRecLevel,doResCorr);
   if (isMC) tag.ReplaceAll("HisData","HisMc");
   tag+="_JetSelEta3";

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
      ana.doResCorr = doResCorr;
      ana.doJetPhiFlat = doJetPhiFlat;
//       ana.subEvtMode = 1; // -1: all (default), 0: sig, 1: bkg
      ana.particleRecLevel = particleRecLevel;
      ana.cMin=centBins[c];
      ana.cMax=centBins[c+1];
      ana.minJetPt1 = minJetPt1;
      ana.minJetPt2 = minJetPt2;
      ana.maxJetEta = 1.6;
      ana.sigDPhi = sigDPhi;
      ana.minPt = 0.5;
      ana.maxEta = etamax;
      ana.outf = hout;
      ana.Init(nt);
      ana.CalcWeights();
      ana.Loop();
   }
   
   hout->Write();
}

